/*
 *  linux/include/asm/setup.h
 *
 *  Copyright (C) 1997-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Structure passed to kernel to tell it about the
 *  hardware it's running on.  See Documentation/arm/Setup
 *  for more info.
 */
#ifndef _UAPI__ASMARM_SETUP_H
#define _UAPI__ASMARM_SETUP_H

#define COMMAND_LINE_SIZE 1024

/* The list ends with an ATAG_NONE node. */
#define ATAG_NONE  0x00000000

struct tag_header {
  UINT32 size;
  UINT32 tag;
};

/* The list must start with an ATAG_CORE node */
#define ATAG_CORE  0x54410001

struct tag_core {
  UINT32 flags;    /* bit 0 = read-only */
  UINT32 pagesize;
  UINT32 rootdev;
};

/* it is allowed to have multiple ATAG_MEM nodes */
#define ATAG_MEM  0x54410002

struct tag_mem32 {
  UINT32 size;
  UINT32 start;    /* physical start address */
};

/* VGA text type displays */
#define ATAG_VIDEOTEXT  0x54410003

struct tag_videotext {
  UINT8 x;
  UINT8 y;
  UINT16 video_page;
  UINT8 video_mode;
  UINT8 video_cols;
  UINT16 video_ega_bx;
  UINT8 video_lines;
  UINT8 video_isvga;
  UINT16 video_points;
};

/* describes how the ramdisk will be used in kernel */
#define ATAG_RAMDISK  0x54410004

struct tag_ramdisk {
  UINT32 flags;    /* bit 0 = load, bit 1 = prompt */
  UINT32 size;    /* decompressed ramdisk size in _kilo_ bytes */
  UINT32 start;    /* starting block of floppy-based RAM disk image */
};

/* describes where the compressed ramdisk image lives (virtual address) */
/*
 * this one accidentally used virtual addresses - as such,
 * it's deprecated.
 */
#define ATAG_INITRD  0x54410005

/* describes where the compressed ramdisk image lives (physical address) */
#define ATAG_INITRD2  0x54420005

struct tag_initrd {
  UINT32 start;    /* physical start address */
  UINT32 size;    /* size of compressed ramdisk image in bytes */
};

/* board serial number. "64 bits should be enough for everybody" */
#define ATAG_SERIAL  0x54410006

struct tag_serialnr {
  UINT32 low;
  UINT32 high;
};

/* board revision */
#define ATAG_REVISION  0x54410007

struct tag_revision {
  UINT32 rev;
};

/* initial values for vesafb-type framebuffers. see struct screen_info
 * in include/linux/tty.h
 */
#define ATAG_VIDEOLFB  0x54410008

struct tag_videolfb {
  UINT16 lfb_width;
  UINT16 lfb_height;
  UINT16 lfb_depth;
  UINT16 lfb_linelength;
  UINT32 lfb_base;
  UINT32 lfb_size;
  UINT8 red_size;
  UINT8 red_pos;
  UINT8 green_size;
  UINT8 green_pos;
  UINT8 blue_size;
  UINT8 blue_pos;
  UINT8 rsvd_size;
  UINT8 rsvd_pos;
};

/* command line: \0 terminated string */
#define ATAG_CMDLINE  0x54410009

struct tag_cmdline {
  CHAR8 cmdline[1];  /* this is the minimum size */
};

/* acorn RiscPC specific information */
#define ATAG_ACORN  0x41000101

struct tag_acorn {
  UINT32 memc_control_reg;
  UINT32 vram_pages;
  UINT8 sounddefault;
  UINT8 adfsdrives;
};

/* footbridge memory clock, see arch/arm/mach-footbridge/arch.c */
#define ATAG_MEMCLK  0x41000402

struct tag_memclk {
  UINT32 fmemclk;
};

struct tag {
  struct tag_header hdr;
  union {
    struct tag_core core;
    struct tag_mem32 mem;
    struct tag_videotext videotext;
    struct tag_ramdisk ramdisk;
    struct tag_initrd initrd;
    struct tag_serialnr serialnr;
    struct tag_revision revision;
    struct tag_videolfb videolfb;
    struct tag_cmdline cmdline;

    /*
     * Acorn specific
     */
    struct tag_acorn acorn;

    /*
     * DC21285 specific
     */
    struct tag_memclk memclk;
  } u;
};

struct tagtable {
  UINT32 tag;
  INTN (*parse) (CONST struct tag *);
};

#define tag_member_present(tag,member)        \
  ((unsigned long)(&((struct tag *)0L)->member + 1)  \
    <= (tag)->hdr.size * 4)

#define tag_next(t)  ((struct tag *)((UINT32 *)(t) + (t)->hdr.size))
#define tag_size(type)  ((sizeof(struct tag_header) + sizeof(struct type)) >> 2)

#define for_each_tag(t,base)    \
  for (t = base; t->hdr.size; t = tag_next(t))

#endif /* _UAPI__ASMARM_SETUP_H */

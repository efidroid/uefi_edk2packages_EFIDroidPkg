/*
 * Copyright (c) 2008, Google Inc.
 * All rights reserved.
 *
 * Copyright (c) 2009-2015, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the 
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <Library/LKEnvLib.h>
#include <Library/fbcon.h>

#include "font5x12.h"

#define RGB565_BLACK		0x0000
#define RGB565_WHITE		0xffff

#define RGB888_BLACK            0x000000
#define RGB888_WHITE            0xffffff

#define FONT_WIDTH		5
#define FONT_HEIGHT		12

static struct fbcon_config *config = NULL;
static uint32_t			BGCOLOR;
static uint32_t			FGCOLOR;
static struct fbcon_pos		local_cur_pos;
static struct fbcon_pos		*cur_pos = NULL;
static struct fbcon_pos		max_pos;

struct fbcon_pos* fbcon_get_position(void) {
	return cur_pos;
}

void fbcon_set_position(struct fbcon_pos* pos) {
	cur_pos = pos;
}

static void fbcon_drawglyph(char *pixels, uint32_t paint, unsigned stride,
			    unsigned bpp, unsigned *glyph, unsigned scale_factor)
{
	unsigned x, y, i, j, k;
	unsigned data, temp;
	uint32_t fg_color = paint;
	stride -= FONT_WIDTH * scale_factor;

	data = glyph[0];
	for (y = 0; y < FONT_HEIGHT / 2; ++y) {
		temp = data;
		for (i = 0; i < scale_factor; i++) {
			data = temp;
			for (x = 0; x < FONT_WIDTH; ++x) {
				if (data & 1) {
					for (j = 0; j < scale_factor; j++) {
						fg_color = paint;
						for (k = 0; k < bpp; k++) {
							*pixels = (unsigned char) fg_color;
							fg_color = fg_color >> 8;
							pixels++;
						}
					}
				}
				else
				{
					for (j = 0; j < scale_factor; j++) {
						for (k = 0; k < bpp; k++) {
							*pixels = (unsigned char) 0x00;
							pixels++;
						}
					}
				}
				data >>= 1;
			}
			pixels += (stride * bpp);
		}
	}

	data = glyph[1];
	for (y = 0; y < FONT_HEIGHT / 2; ++y) {
		temp = data;
		for (i = 0; i < scale_factor; i++) {
			data = temp;
			for (x = 0; x < FONT_WIDTH; ++x) {
				if (data & 1) {
					for (j = 0; j < scale_factor; j++) {
						fg_color = paint;
						for (k = 0; k < bpp; k++) {
							*pixels = (unsigned char) fg_color;
							fg_color = fg_color >> 8;
							pixels++;
						}
					}
				}
				else
				{
					for (j = 0; j < scale_factor; j++) {
						for (k = 0; k < bpp; k++) {
							*pixels = (unsigned char) 0x00;
							pixels++;
						}
					}
				}
				data >>= 1;
			}
			pixels += (stride * bpp);
		}
	}

}

void fbcon_flush(void)
{
	/* ignore anything that happens before fbcon is initialized */
	if (!config)
		return;

	unsigned total_x, total_y;
	unsigned bytes_per_bpp;

	if (config->update_start)
		config->update_start();
	if (config->update_done)
		while (!config->update_done());

	total_x = config->width;
	total_y = config->height;
	bytes_per_bpp = ((config->bpp) / 8);
	arch_clean_invalidate_cache_range((addr_t) config->base, (total_x * total_y * bytes_per_bpp));
}

/* TODO: Take stride into account */
static void fbcon_scroll_up(void)
{
	uint32_t num_lines = max_pos.y-cur_pos->y;
	uint32_t bpp = (config->bpp / 8);
	uint32_t off_bytes = config->width * bpp * num_lines * FONT_HEIGHT;

	uint8_t *dst = config->base;
	uint8_t *src = dst + off_bytes;
	unsigned count = config->width*config->height*bpp - off_bytes;

	memmove(dst, src, count);
	memset(dst+count, BGCOLOR, config->width*config->height*bpp - count);

	fbcon_flush();
}

void fbcon_clear(void)
{
	/* ignore anything that happens before fbcon is initialized */
	if (!config)
		return;

	memset(config->base, BGCOLOR, config->width * config->height * (config->bpp/8));
	cur_pos->x = 0;
	cur_pos->y = 0;
}

void fbcon_putc(char c)
{
	char *pixels;
	unsigned scale_factor = PcdGet64 (PcdFbConScaleFactor);

	/* ignore anything that happens before fbcon is initialized */
	if (!config)
		return;

	if((unsigned char)c > 127)
		return;

	if((unsigned char)c < 32) {
		if(c == '\n')
			goto newline;
		else if (c == '\r') {
			cur_pos->x = 0;
			return;
		}
		else
			return;
	}

	if (cur_pos->x == 0 && (unsigned char)c == ' ')
		return;

	BOOLEAN intstate = ArmGetInterruptState();
	ArmDisableInterrupts();

	pixels = config->base;
	pixels += cur_pos->y * ((config->bpp / 8) * FONT_HEIGHT * config->width);
	pixels += cur_pos->x * scale_factor * ((config->bpp / 8) * (FONT_WIDTH + 1));

	fbcon_drawglyph(pixels, FGCOLOR, config->stride, (config->bpp / 8),
			font5x12 + (c - 32) * 2, scale_factor);

	cur_pos->x++;
	if (cur_pos->x >= (int)(max_pos.x / scale_factor))
		goto newline;

	if (intstate)
		ArmEnableInterrupts();

	return;

newline:
	cur_pos->y += scale_factor;
	cur_pos->x = 0;
	if((uint32_t)cur_pos->y > max_pos.y-scale_factor) {
		cur_pos->y = max_pos.y - scale_factor;
		fbcon_scroll_up();
	} else
		fbcon_flush();

	if (intstate)
		ArmEnableInterrupts();
}

void fbcon_setup(struct fbcon_config *_config)
{
	ASSERT(_config);

	config = _config;

	switch (config->format) {
		case FB_FORMAT_RGB565:
		BGCOLOR = RGB565_BLACK;
		FGCOLOR = RGB565_WHITE;
		break;

		case FB_FORMAT_RGB888:
		BGCOLOR = RGB888_BLACK;
		FGCOLOR = RGB888_WHITE;
		break;
	default:
		dprintf(CRITICAL, "unknown framebuffer pixel format\n");
		ASSERT(0);
		break;
	}

	cur_pos = &local_cur_pos;
	cur_pos->x = 0;
	cur_pos->y = 0;

	max_pos.x = config->width / (FONT_WIDTH+1);
	max_pos.y = (config->height - 1) / FONT_HEIGHT;
}

struct fbcon_config* fbcon_display(void)
{
	return config;
}

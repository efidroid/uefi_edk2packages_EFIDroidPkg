#ifndef _MMC_PRIVATE_H
#define _MMC_PRIVATE_H

#define BLOCK_SIZE                0x200

unsigned int mmc_boot_main(unsigned char slot, unsigned int base);
unsigned int mmc_boot_read_from_card(struct mmc_boot_host *host,
				     struct mmc_boot_card *card,
				     unsigned long long data_addr,
				     unsigned int data_len, unsigned int *out);
unsigned int mmc_write(unsigned long long data_addr,
		       unsigned int data_len, unsigned int *in);

unsigned int mmc_read(unsigned long long data_addr, unsigned int *out,
		      unsigned int data_len);
unsigned mmc_get_psn(void);

unsigned int mmc_boot_write_to_card(struct mmc_boot_host *host,
				    struct mmc_boot_card *card,
				    unsigned long long data_addr,
				    unsigned int data_len, unsigned int *in);

unsigned int mmc_erase_card(unsigned long long data_addr,
			    unsigned long long data_len);

struct mmc_boot_host *get_mmc_host(void);
struct mmc_boot_card *get_mmc_card(void);
void mmc_mclk_reg_wr_delay(void);
void mmc_boot_mci_clk_enable(void);
void mmc_boot_mci_clk_disable(void);

#endif // _MMC_PRIVATE_H

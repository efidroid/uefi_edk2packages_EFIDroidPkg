#ifndef _MMC_PRIVATE_H
#define _MMC_PRIVATE_H

#define BLOCK_SIZE                0x200

unsigned int mmc_boot_main(unsigned char slot, unsigned int base);
unsigned int mmc_write(unsigned long long data_addr,
		       unsigned int data_len, unsigned int *in);

unsigned int mmc_read(unsigned long long data_addr, unsigned int *out,
		      unsigned int data_len);
unsigned mmc_get_psn(void);

unsigned int mmc_erase_card(unsigned long long data_addr,
			    unsigned long long data_len);

void mmc_mclk_reg_wr_delay(void);
void mmc_boot_mci_clk_enable(void);
void mmc_boot_mci_clk_disable(void);
uint8_t card_supports_ddr_mode(void);
uint8_t card_supports_hs200_mode(void);
uint64_t mmc_get_device_capacity(void);
void mmc_put_card_to_sleep(void);

#endif // _MMC_PRIVATE_H

#ifndef _MMC_PRIVATE_H
#define _MMC_PRIVATE_H

#define BLOCK_SIZE                0x200

struct mmc_device* mmc_boot_main(unsigned char slot, unsigned int base);
unsigned int mmc_write(struct mmc_device *dev, unsigned long long data_addr,
		       unsigned int data_len, unsigned int *in);

unsigned int mmc_read(struct mmc_device *dev, unsigned long long data_addr, unsigned int *out,
		      unsigned int data_len);
unsigned mmc_get_psn(struct mmc_card *card);

unsigned int mmc_erase_card(struct mmc_device *dev, unsigned long long data_addr,
			    unsigned long long data_len);

void mmc_mclk_reg_wr_delay(struct mmc_host *host);
void mmc_boot_mci_clk_enable(struct mmc_device *dev);
void mmc_boot_mci_clk_disable(struct mmc_device *dev);
uint8_t card_supports_ddr_mode(struct mmc_device *dev);
uint8_t card_supports_hs200_mode(struct mmc_device *dev);
uint64_t mmc_get_device_capacity(struct mmc_device *dev);
uint32_t mmc_get_device_blocksize(struct mmc_device *dev);
void mmc_put_card_to_sleep(struct mmc_device *dev);

#endif // _MMC_PRIVATE_H

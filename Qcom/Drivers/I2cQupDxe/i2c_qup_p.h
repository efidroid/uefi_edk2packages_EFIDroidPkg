#ifndef _I2C_QUP_PRIVATE_H
#define _I2C_QUP_PRIVATE_H

/* Function Definitions */
struct qup_i2c_dev *qup_i2c_init(uint8_t gsbi_id,
				 unsigned clk_freq, unsigned src_clk_freq);
struct qup_i2c_dev *qup_blsp_i2c_init(uint8_t blsp_id, uint8_t qup_id,
				      uint32_t clk_freq, uint32_t src_clk_freq);
int qup_i2c_deinit(struct qup_i2c_dev *dev);
int qup_i2c_xfer(struct qup_i2c_dev *dev, struct i2c_msg msgs[], int num);

#endif

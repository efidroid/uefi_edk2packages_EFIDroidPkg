#ifndef _GPIO_PRIVATE_H
#define _GPIO_PRIVATE_H

/* APIs: exposed for other drivers */
/* API: Hdrive control for tlmm pins */
void tlmm_set_hdrive_ctrl(struct tlmm_cfgs *, uint8_t);
/* API:  Pull control for tlmm pins */
void tlmm_set_pull_ctrl(struct tlmm_cfgs *, uint8_t);

#endif // _GPIO_PRIVATE_H

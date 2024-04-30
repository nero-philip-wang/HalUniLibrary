#include "u_common.h"

// 代码空间0-16k, 数据空间16-20k
// 每个page 128 byte, 共0-31个page
#define USER_FLASH_START FLASH_END + 1 - FLASH_SECTOR_SIZE

#define FLASH_PAGE_PRE_SECTOR FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE

// 0xffffffff 代表已查处，未写入
#define PAGE_IN_USE 0xF0F0F0F0ul

// 0xf0f0f0f0 page使用中
#define PAGE_EMPTY UINT32_MAX

extern void uEraseFlash(uint8_t lock);

extern void uWriteFlash(uint32_t *src);

extern volatile uint32_t *uReadFlash();

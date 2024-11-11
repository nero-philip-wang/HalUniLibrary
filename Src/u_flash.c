#include "u_flash.h"

#define FLASH_OPTR_CONSTANT 0x4F55B0AA
#define FLASH_SDKR_CONSTANT 0xE0EF1F10
#define FLASH_WRPR_CONSTANT 0xF0000FFF

#ifdef FLASH_OPT_PROGRAM_ENABLED

#if defined(__GNUC__) && !defined(__CC_ARM) /* GNU Compiler */
const uint32_t u32ICG[] __attribute__((section(".opt_sec"))) =
#elif defined(__CC_ARM)
const uint32_t u32ICG[] __attribute__((at(0x1FFF0E80))) =
#elif defined(__ICCARM__)
__root const uint32_t u32ICG[] @0x1FFF0E80 =
#else
#error "unsupported compiler!!"
#endif
    {
        0x4F55B0AA,
        0xE0EF1F10,
        0xFFFFFFFF,
        0x0000FFFF,
    };

#endif /* FLASH_OPT_PROGRAM_ENABLED */

/*
每个page 128 byte，每次必须写入一个page，即32个int/float,
前4个byte = 32bit
0xffffffff 代表全部可用，没有数据写入
0xffffffff << 1 代表 数据已经写入到1个数据位
0xffffffff << 31 代表 数据已经写入到31个数据位, 即已写满
*/
// struct flashDataTemplate
// {
//   uint32_t ptr;
//   uint32_t[31] data;
// };

// uint32_t uReadWord(uint32_t *pageAddr)
// {
//   uint32_t ptr = HW32_REG(pageAddr);
//   if (ptr == 0xffffffff)
//     return 0 uint8_t bit = 0;
//   while (ptr << ++bit)
//   {
//   }
//   bit = 31 - bit return HW32_REG(pageAddr + bit)
// }
// uint32_t uWriteWord(uint32_t *pageAddr, uint32_t value)
// {
// }

void uEraseFlash(uint8_t lock)
{
  uint32_t SECTORError = 0;
  FLASH_EraseInitTypeDef EraseInitStruct;
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORERASE;
  EraseInitStruct.SectorAddress = USER_FLASH_START;
  EraseInitStruct.NbSectors = 1;

  if (lock)
    HAL_FLASH_Unlock();
  HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
  if (lock)
    HAL_FLASH_Lock();
}

void uWriteFlash(uint32_t *src)
{
  uint32_t pageAddr = USER_FLASH_START;
  while (HW32_REG(pageAddr) == PAGE_IN_USE && pageAddr < FLASH_END)
  {
    pageAddr += FLASH_PAGE_SIZE;
  }
  HAL_FLASH_Unlock();

  src[0] = PAGE_IN_USE;
  if (pageAddr > FLASH_END)
  {
    uEraseFlash(0);
    pageAddr = USER_FLASH_START;
  }
  HAL_FLASH_PageProgram(pageAddr, src);
  HAL_FLASH_Lock();
}

volatile uint32_t *uReadFlash()
{
  uint32_t pageAddr = USER_FLASH_START;
  while (HW32_REG(pageAddr) == PAGE_IN_USE && pageAddr < FLASH_END)
  {
    pageAddr += FLASH_PAGE_SIZE;
  }
  pageAddr -= FLASH_PAGE_SIZE;
  if (pageAddr < USER_FLASH_START)
    return NULL;
  return (volatile uint32_t *)pageAddr;
}
#ifndef __W25QXX_H
#define __W25QXX_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

extern SPI_HandleTypeDef hspi1;

/* Commands */
#define W25Q_WRITE_ENABLE      0x06
#define W25Q_READ_STATUS1      0x05
#define W25Q_PAGE_PROGRAM      0x02
#define W25Q_READ_DATA         0x03
#define W25Q_SECTOR_ERASE      0x20
#define W25Q_CHIP_ERASE        0xC7
#define W25Q_DEVICE_ID         0x9F

/* CS pin (change if needed) */
#define W25Q_CS_PORT GPIOA
#define W25Q_CS_PIN  GPIO_PIN_4

/* API */
void W25Q_Init(void);
uint32_t W25Q_ReadID(void);

void W25Q_WriteEnable(void);
void W25Q_WaitBusy(void);

void W25Q_SectorErase(uint32_t addr);

void W25Q_PageWrite(uint32_t addr, uint8_t *data, uint16_t size);
void W25Q_Read(uint32_t addr, uint8_t *buffer, uint16_t size);

/* Struct helpers */
void W25Q_WriteStruct(uint32_t addr, void *data, uint16_t size);
void W25Q_ReadStruct(uint32_t addr, void *data, uint16_t size);

#endif

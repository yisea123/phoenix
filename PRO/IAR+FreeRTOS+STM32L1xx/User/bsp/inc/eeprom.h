#ifndef __EEPROM_H__
#define __EEPROM_H__

#define EEPROM_BASE_ADDR 0x08080000
#define EEPROM_BYTE_SZIE 0x1FFF

//可调用FLASH_Status DATA_EEPROM_ProgramByte(uint32_t Address, uint8_t Data)；
//调用前先调用DATA_EEPROM_Unlock()；解除eeprom的锁定
//再调用DATA_EEPROM_FixedTimeProgramCmd()；写之前先自动擦除
//最后再调用DATA_EEPROM_Lock()；锁定eeprom，防止误操作。

FLASH_Status eeprom_write_bytes(uint16_t addr,uint8_t *buffer,uint16_t length);

void eeprom_read_bytes(uint16_t addr,uint8_t *buffer,uint16_t length);

#endif

#include "vl53l1_platform.h"
#include "vl53l1_api.h"
#include "stm32l4xx_hal.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

#define I2C_TIME_OUT_BASE   10
#define I2C_TIME_OUT_BYTE   1

VL53L1_Error VL53L1_WriteMulti(VL53L1_DEV Dev, uint16_t index, uint8_t *pdata, uint32_t count)
{
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    uint32_t timeout = I2C_TIME_OUT_BASE + count * I2C_TIME_OUT_BYTE;

    if (HAL_I2C_Mem_Write(&hi2c1,
                          Dev->i2c_slave_address,   // 0x52
                          index,
                          I2C_MEMADD_SIZE_16BIT,   // indirizzo registro = 16 bit
                          pdata,
                          count,
                          timeout) != HAL_OK) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
    }

    return Status;
}

VL53L1_Error VL53L1_ReadMulti(VL53L1_DEV Dev, uint16_t index, uint8_t *pdata, uint32_t count)
{
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    uint32_t timeout = I2C_TIME_OUT_BASE + count * I2C_TIME_OUT_BYTE;

    if (HAL_I2C_Mem_Read(&hi2c1,
                         Dev->i2c_slave_address,
                         index,
                         I2C_MEMADD_SIZE_16BIT,
                         pdata,
                         count,
                         timeout) != HAL_OK) {
        Status = VL53L1_ERROR_CONTROL_INTERFACE;
    }

    return Status;
}

VL53L1_Error VL53L1_WrByte(VL53L1_DEV Dev, uint16_t index, uint8_t data)
{
    return VL53L1_WriteMulti(Dev, index, &data, 1);
}

VL53L1_Error VL53L1_WrWord(VL53L1_DEV Dev, uint16_t index, uint16_t data)
{
    uint8_t buffer[2];
    buffer[0] = (uint8_t)(data >> 8);
    buffer[1] = (uint8_t)(data & 0xFF);
    return VL53L1_WriteMulti(Dev, index, buffer, 2);
}

VL53L1_Error VL53L1_WrDWord(VL53L1_DEV Dev, uint16_t index, uint32_t data)
{
    uint8_t buffer[4];
    buffer[0] = (uint8_t)(data >> 24);
    buffer[1] = (uint8_t)(data >> 16);
    buffer[2] = (uint8_t)(data >> 8);
    buffer[3] = (uint8_t)(data & 0xFF);
    return VL53L1_WriteMulti(Dev, index, buffer, 4);
}

VL53L1_Error VL53L1_RdByte(VL53L1_DEV Dev, uint16_t index, uint8_t *data)
{
    return VL53L1_ReadMulti(Dev, index, data, 1);
}

VL53L1_Error VL53L1_RdWord(VL53L1_DEV Dev, uint16_t index, uint16_t *data)
{
    uint8_t buffer[2];
    VL53L1_Error Status = VL53L1_ReadMulti(Dev, index, buffer, 2);
    if (Status == VL53L1_ERROR_NONE) {
        *data = ((uint16_t)buffer[0] << 8) | buffer[1];
    }
    return Status;
}

VL53L1_Error VL53L1_RdDWord(VL53L1_DEV Dev, uint16_t index, uint32_t *data)
{
    uint8_t buffer[4];
    VL53L1_Error Status = VL53L1_ReadMulti(Dev, index, buffer, 4);
    if (Status == VL53L1_ERROR_NONE) {
        *data = ((uint32_t)buffer[0] << 24) |
                ((uint32_t)buffer[1] << 16) |
                ((uint32_t)buffer[2] << 8)  |
                ((uint32_t)buffer[3]);
    }
    return Status;
}

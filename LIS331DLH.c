#include "LIS331DLH.h"

extern I2C_HandleTypeDef hi2c1;

static void Error(void){
  LED_OFF;
}

static void I2Cx_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value){
  HAL_StatusTypeDef status = HAL_OK;
   status = HAL_I2C_Mem_Write(&hi2c1, Addr,(uint16_t) Reg, 
                    I2C_MEMADD_SIZE_8BIT, &Value, 1, 0x10000);
   if(status != HAL_OK) Error();
}

static uint8_t I2Cx_ReadData(uint16_t Addr, uint8_t Reg){
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0x00;
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT,
                            &value, 1, 0x10000);
  if(status != HAL_OK) Error();
  return value;
}

static uint8_t LISS1DLH_ReadReg(uint8_t Reg){
  uint8_t result;
  result = I2Cx_ReadData(READ_DATA, Reg);
  return result;
}

static void LISS1DLH_WriteReg(uint8_t Reg, uint8_t Value){
  I2Cx_WriteData(WRITE_DATA , Reg, Value);
}

static void LISS1DLH_SetPowerMode(uint8_t mode){
  uint8_t reg = 0x00;
  reg = LISS1DLH_ReadReg(CTRL_REG1) & ~MASK_POWER_MODE;
  reg |= mode & MASK_POWER_MODE;
  LISS1DLH_WriteReg(CTRL_REG1, reg);
}

static void LISS1DLH_SetOutputDataRate(uint8_t data_rate){
  uint8_t reg = 0x00;
  reg = LISS1DLH_ReadReg(CTRL_REG1) & ~MASK_OUTPUT_DATA_RATE;
  reg |= data_rate & MASK_OUTPUT_DATA_RATE;
  LISS1DLH_WriteReg(CTRL_REG1, reg);
}

static void LISS1DLH_SetAxisEnable(uint8_t axis){
  uint8_t reg = 0x00;
  reg = LISS1DLH_ReadReg(CTRL_REG1) & ~MASK_AXIS;
  reg |= axis & MASK_AXIS;
  LISS1DLH_WriteReg(CTRL_REG1, reg);
}

void LISS1DLH_Init(void){
  uint8_t value = 0x00;
  value = LISS1DLH_ReadReg(WHO_AM_I_REG);
  if( value != WHO_AM_I_VALUE){
    Error();
    return;
  }
  LED_ON;
  LISS1DLH_SetPowerMode(NORMAL_MODE);
  LISS1DLH_SetOutputDataRate(OUTPUT_DATA_RATE_400);
  LISS1DLH_SetAxisEnable(ENABLE_X_Y_Z);
}


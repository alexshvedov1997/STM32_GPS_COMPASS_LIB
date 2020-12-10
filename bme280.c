#include "bme280.h"

extern I2C_HandleTypeDef hi2c1;
BME280_CalibData CalibData;

static void Error(void){
  LED_OFF;
}

static void I2Cx_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Write(&hi2c1, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 0x10000);
  if(status != HAL_OK) Error();
}

static uint8_t I2Cx_ReadData(uint16_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 0x10000);
  if(status != HAL_OK) Error();
  return value;
}

static void I2Cx_ReadData16(uint16_t Addr, uint8_t Reg, uint16_t *value){
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) value, 2, 0x10000);
  if(status != HAL_OK) Error();
}

static void I2Cx_ReadData24(uint16_t Addr, uint8_t Reg, uint32_t *value){
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT,
                             (uint8_t*) value, 3,0x10000);
  if(status != HAL_OK) Error();
}

static void BME280_WriteReg(uint8_t Reg, uint8_t value){
  I2Cx_WriteData(BME280_ADDRESS, Reg, value);
}

static uint8_t BME280_ReadReg(uint8_t Reg){
  uint8_t result =  I2Cx_ReadData(BME280_ADDRESS , Reg);
  return result;
}

static uint8_t ReadStatus(void){
  uint8_t status = BME280_ReadReg(BME280_STATUS_REG)&0x09;
  return status;
}

static void BME280_ReadReg_S16(uint8_t Reg, int16_t* value){
  I2Cx_ReadData16(BME280_ADDRESS, Reg, (uint16_t*) value);
}

static void BME280_ReadReg_U16(uint8_t Reg, uint16_t* value){
  I2Cx_ReadData16(BME280_ADDRESS, Reg, value);
}

void BME280_ReadCoefficients(void)
{
  BME280_ReadReg_U16(BME280_REGISTER_DIG_T1,&CalibData.dig_T1);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_T2,&CalibData.dig_T2);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_T3,&CalibData.dig_T3);
  BME280_ReadReg_U16(BME280_REGISTER_DIG_P1,&CalibData.dig_P1);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P2,&CalibData.dig_P2);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P3,&CalibData.dig_P3);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P4,&CalibData.dig_P4);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P5,&CalibData.dig_P5);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P6,&CalibData.dig_P6);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P7,&CalibData.dig_P7);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P8,&CalibData.dig_P8);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_P9,&CalibData.dig_P9);
  CalibData.dig_H1 = BME280_ReadReg(BME280_REGISTER_DIG_H1);
  BME280_ReadReg_S16(BME280_REGISTER_DIG_H2,&CalibData.dig_H2);
  CalibData.dig_H3 = BME280_ReadReg(BME280_REGISTER_DIG_H3);
  CalibData.dig_H4 = (BME280_ReadReg(BME280_REGISTER_DIG_H4) << 4) | (BME280_ReadReg(BME280_REGISTER_DIG_H4+1) & 0xF);
  CalibData.dig_H5 = (BME280_ReadReg(BME280_REGISTER_DIG_H5+1) << 4) | (BME280_ReadReg(BME280_REGISTER_DIG_H5) >> 4);
  CalibData.dig_H6 = (int8_t)BME280_ReadReg(BME280_REGISTER_DIG_H6);
}

//---------------------------------------------
uint16_t BME280_ReadCalibData(void){
return CalibData.dig_T1;
}
//---------------------------------------------

static void BME280_SetStandby(uint8_t tsb) {
  uint8_t reg = 0x00;
  reg = BME280_ReadReg(BME280_REG_CONFIG)& ~BME280_STBY_MSK ;
  reg |= tsb&BME280_STBY_MSK;
  BME280_WriteReg(BME280_REG_CONFIG, reg);
}

static void BME280_SetFilter(uint8_t filter){
  uint8_t reg = 0x00;
  reg = BME280_ReadReg(BME280_REG_CONFIG)& ~BME280_FILTER_MSK;
  reg |= filter & BME280_FILTER_MSK;
  BME280_WriteReg(BME280_REG_CONFIG, reg);
}

void BME280_SetOversamplingTemper(uint8_t osrs){
  uint8_t reg = 0x00;
  reg = BME280_ReadReg(BME280_REG_CTRL_MEAS) & ~BME280_OSRS_T_MSK;
  reg |= osrs&BME280_OSRS_T_MSK;
  BME280_WriteReg(BME280_REG_CTRL_MEAS, reg);
}

void BME280_SetOversamplingPressure(uint8_t osrs){
  uint8_t reg =0x00;
  reg = BME280_ReadReg(BME280_REG_CTRL_MEAS) & ~BME280_OSRS_P_MSK ;
  reg |= osrs&BME280_OSRS_P_MSK;
  BME280_WriteReg(BME280_REG_CTRL_MEAS, reg);
}

void BME280_SetOversamplingHum(uint8_t osrs){
  uint8_t reg = 0x00;
  reg = BME280_ReadReg(BME280_REG_CTRL_HUM)& ~BME280_OSRS_H_MSK;
  reg |= osrs&BME280_OSRS_H_MSK;
  BME280_WriteReg(BME280_OSRS_H_MSK, reg);
  reg = BME280_ReadReg(BME280_REG_CTRL_MEAS);
  BME280_WriteReg(BME280_REG_CTRL_MEAS, reg);
}

void BME280_Init(void){
  uint8_t value = 0;
  uint32_t value32 = 0;
  value = BME280_ReadReg(BME280_REG_ID);
  if(value != BME280_ID ){
    Error();
    return;
  }
  LED_ON;
  BME280_WriteReg(BME280_SOFTRESET_REG, BME280_SOFTRESET_VALUE);
  while(ReadStatus()&BME280_STATUS_IN_UPDATE);
  BME280_ReadCoefficients();
  BME280_SetStandby(BME280_STBY_1000);
  BME280_SetFilter(BME280_FILTER_4);
  BME280_SetOversamplingTemper(BME280_OSRS_T_x4);
  BME280_SetOversamplingPressure(BME280_OSRS_P_x2);
  BME280_SetOversamplingHum(BME280_OSRS_H_x1);
}


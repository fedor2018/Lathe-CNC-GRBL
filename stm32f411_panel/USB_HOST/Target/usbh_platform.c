/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbh_platform.c

  * @brief          : This file implements the USB platform
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbh_platform.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* External variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

/**
  * @brief  Drive VBUS.
  * @param  state : VBUS state
  *          This parameter can be one of the these values:
  *           - 1 : VBUS Active
  *           - 0 : VBUS Inactive
  */
void MX_DriverVbusFS(uint8_t state)
{
  /* USER CODE BEGIN PREPARE_I2C_REG_VBUS_FS */
  uint8_t Component_Reg  = 0;
  /* USER CODE END PREPARE_I2C_REG_VBUS_FS */
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t Component_Addr = 0 << 1;
  /* USER CODE BEGIN PREPARE_I2C_DATA_VBUS_FS */
  uint8_t data = state;
  /* USER CODE END PREPARE_I2C_DATA_VBUS_FS */
  uint8_t data_tmp = 0;
  status = HAL_I2C_Mem_Read(&hi2c1, Component_Addr, (uint16_t)Component_Reg, I2C_MEMADD_SIZE_8BIT, &data_tmp, 1, 100);
  data |= data_tmp;
  status = HAL_I2C_Mem_Write(&hi2c1,Component_Addr,(uint16_t)Component_Reg, I2C_MEMADD_SIZE_8BIT,&data, 1, 100);
  /* USER CODE BEGIN CHECK_STATUS_VBUS_FS */
  /* Check the communication status */
  if(status != HAL_OK)
  {

  }
  /* USER CODE END CHECK_STATUS_VBUS_FS */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

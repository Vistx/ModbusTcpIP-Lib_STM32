/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "wizchip_conf.h"
#include "socket.h"
#include <string.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define W5500_CS_PORT GPIOA
#define W5500_CS_PIN  GPIO_PIN_4
#define W5500_RST_PORT GPIOA
#define W5500_RST_PIN  GPIO_PIN_1

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MODBUS_PORT 502
#define SOCK_MODBUS 0
uint8_t rxBuf[512];
uint8_t txBuf[512];

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t mac[6] = {0x00,0x08,0xDC,0x01,0x02,0x03};
uint8_t ip[4]  = {192,168,0,129};
uint8_t gw[4]  = {192,168,0,1};
uint8_t sn[4]  = {255,255,255,0};

uint16_t holdingRegs[10] = {123, 456, 789, 321, 654, 987, 111, 222, 333, 444};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void cs_sel(void)   { HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, GPIO_PIN_RESET); }
static void cs_desel(void) { HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, GPIO_PIN_SET);   }

static void reset_w5500(void) {
    HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);
}

static uint8_t spi_rb(void) {
    uint8_t rb=0;
    HAL_SPI_Receive(&hspi1, &rb, 1, HAL_MAX_DELAY);
    return rb;
}
static void spi_wb(uint8_t wb) {
    HAL_SPI_Transmit(&hspi1, &wb, 1, HAL_MAX_DELAY);
}

/* ------------------- Modbus Processor ------------------- */
int modbus_process(uint8_t *req, int len, uint8_t *resp) {
    if (len < 8) return 0; // too short

    uint16_t protoId = (req[2] << 8) | req[3];
    uint8_t  unitId  = req[6];
    uint8_t  func    = req[7];

    if (protoId != 0) return 0; // must be 0 for Modbus

    /* ---- Function 0x03: Read Holding Registers ---- */
    if (func == 3 && len >= 12) {
        uint16_t startAddr = (req[8] << 8) | req[9];
        uint16_t quantity  = (req[10] << 8) | req[11];

        if (quantity > 10) quantity = 10;

        resp[0] = req[0]; resp[1] = req[1];
        resp[2] = 0; resp[3] = 0;
        resp[4] = 0; resp[5] = 3 + (quantity * 2);
        resp[6] = unitId;
        resp[7] = func;
        resp[8] = quantity * 2;

        for (int i = 0; i < quantity; i++) {
            uint16_t val = holdingRegs[startAddr + i];
            resp[9 + i*2]  = val >> 8;
            resp[10 + i*2] = val & 0xFF;
        }
        return 9 + (quantity * 2);
    }

    /* ---- Function 0x06: Write Single Register ---- */
    if (func == 6 && len >= 12) {
        uint16_t regAddr = (req[8] << 8) | req[9];
        uint16_t value   = (req[10] << 8) | req[11];

        if (regAddr < 10) holdingRegs[regAddr] = value;

        memcpy(resp, req, len); // echo back
        return len;
    }

    /* ---- Function 0x10: Write Multiple Registers ---- */
    if (func == 16 && len >= 13) {
        uint16_t startAddr = (req[8] << 8) | req[9];
        uint16_t quantity  = (req[10] << 8) | req[11];

        for (int i = 0; i < quantity; i++) {
            uint16_t value = (req[13 + i*2] << 8) | req[14 + i*2];
            if ((startAddr + i) < 10) holdingRegs[startAddr + i] = value;
        }

        // Response is echo of header + startAddr + quantity
        resp[0] = req[0]; resp[1] = req[1];
        resp[2] = 0; resp[3] = 0;
        resp[4] = 0; resp[5] = 6;
        resp[6] = unitId;
        resp[7] = func;
        resp[8] = req[8]; resp[9] = req[9];
        resp[10] = req[10]; resp[11] = req[11];
        return 12;
    }

    /* ---- Unsupported function → Exception ---- */
    resp[0] = req[0]; resp[1] = req[1];
    resp[2] = 0; resp[3] = 0;
    resp[4] = 0; resp[5] = 3;
    resp[6] = unitId;
    resp[7] = func | 0x80;
    resp[8] = 0x01; // illegal function
    return 9;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  reset_w5500();
      reg_wizchip_cs_cbfunc(cs_sel, cs_desel);
      reg_wizchip_spi_cbfunc(spi_rb, spi_wb);

      uint8_t txsize[8] = {2,2,0,0,0,0,0,0};
      uint8_t rxsize[8] = {2,2,0,0,0,0,0,0};
      wizchip_init(txsize, rxsize);

      setSHAR(mac);
      setSIPR(ip);
      setGAR(gw);
      setSUBR(sn);
      uint32_t prev_task  = HAL_GetTick();
      const uint32_t TASK_INTERVAL  = 100; // ms

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (socket(SOCK_MODBUS, Sn_MR_TCP, MODBUS_PORT, 0) != SOCK_MODBUS) {
	              close(SOCK_MODBUS);
	              continue;
	          }
	          if (listen(SOCK_MODBUS) != SOCK_OK) {
	              close(SOCK_MODBUS);
	              continue;
	          }

	          while (1) {
	              uint8_t status = getSn_SR(SOCK_MODBUS);
	              uint32_t now = HAL_GetTick();
	              if (status == SOCK_ESTABLISHED) {
	                  int len = recv(SOCK_MODBUS, rxBuf, sizeof(rxBuf));
	                  if (len > 0) {
	                      int rlen = modbus_process(rxBuf, len, txBuf);
	                      if (rlen > 0) send(SOCK_MODBUS, txBuf, rlen);
	                      if ((now - prev_task) >= TASK_INTERVAL) {
	                    	                          prev_task += TASK_INTERVAL;
	                    	                          // do short task (e.g., read ADC, update counters, state machine)
	                    	                          holdingRegs[0]++;
	                    	                      }
	                  }
	              } else if (status == SOCK_CLOSE_WAIT) {
	                  disconnect(SOCK_MODBUS);
	              } else if (status == SOCK_CLOSED) {
	                  close(SOCK_MODBUS);
	                  break;
	              }
	              HAL_Delay(10);

	          }
	      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RST_Pin|CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RST_Pin CS_Pin */
  GPIO_InitStruct.Pin = RST_Pin|CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

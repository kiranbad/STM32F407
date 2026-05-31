/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : FreeRTOS Simple Task Creation
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* Task function prototypes */
void vTask1_handler(void *params);
void vTask2_handler(void *params);

/* Task handles */
TaskHandle_t xTaskHandle1;
TaskHandle_t xTaskHandle2;

/*============================================================================*/

int main(void)
{
    /* HAL Initialization */
    HAL_Init();

    /* Configure System Clock */
    SystemClock_Config();

    /* Initialize GPIO */
    MX_GPIO_Init();

    /* Create Task-1 */
    xTaskCreate(vTask1_handler, "Task-1", 256, NULL, 2, &xTaskHandle1);

    /* Create Task-2 */
    xTaskCreate(vTask2_handler, "Task-2", 256, NULL, 2, &xTaskHandle2);

    /* Start Scheduler */
    vTaskStartScheduler();

    /* Infinite loop */
    while (1)
    {

    }
}

/*============================================================================*/

/* Task-1 */
void vTask1_handler(void *params)
{
    while(1)
    {
        /* Toggle Green LED (PD12) */
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

        /* Delay 500 ms */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/*============================================================================*/

/* Task-2 */
void vTask2_handler(void *params)
{
    while(1)
    {
        /* Toggle Orange LED (PD13) */
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);

        /* Delay 1000 ms */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*============================================================================*/

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable Power Control Clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Configure Voltage Scaling */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Configure HSI Oscillator and PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 50;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure CPU, AHB and APB clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/*============================================================================*/

/**
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO Port Clock */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Initial LED State = OFF */
    HAL_GPIO_WritePin(GPIOD,
                       GPIO_PIN_12 | GPIO_PIN_13,
                       GPIO_PIN_RESET);

    /* Configure PD12 and PD13 as Output */
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/*============================================================================*/

/**
  * @brief Error Handler
  * @retval None
  */
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {

    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief Reports source file and source line number
  * @param file: source file name
  * @param line: line number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{

}

#endif


#include <stdio.h>
#include <string.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

// Macros
#define TRUE          1
#define FALSE         0
#define NOT_PRESSED   FALSE
#define PRESSED       TRUE

UART_HandleTypeDef huart2;
TaskHandle_t ledTaskHandle = NULL;

uint8_t button_status_flag = NOT_PRESSED;
char usr_msg[100];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void prvSetupHardware(void);
static void prvSetupUart(void);
static void prvSetupGPIO(void);

void led_task_handler(void *params);
void button_task_handler(void *params);

void printmsg(char *msg)
{
	for(uint32_t i = 0;i < strlen(msg);i++)
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)&msg[i], 1, 100);
	}
}
int main(void)
{

	HAL_Init();
	  SystemClock_Config();

	  prvSetupHardware();

	  // create tasks
	  xTaskCreate(led_task_handler,
	              "LED-TASK",
	              configMINIMAL_STACK_SIZE,
	              NULL,
	              1,
	              &ledTaskHandle);

	  xTaskCreate(button_task_handler,
	              "BUTTON-TASK",
	              configMINIMAL_STACK_SIZE,
	              NULL,
	              1,
	              NULL);


	  vTaskStartScheduler();
	  while (1)
	  {
	    /* USER CODE END WHILE */

	    /* USER CODE BEGIN 3 */
	  }
	  /* USER CODE END 3 */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */


/* USER CODE BEGIN 4 */
void led_task_handler(void *params)
{
    uint32_t receivedCount;

    sprintf(usr_msg,"LED Task Waiting...\r\n");
    printmsg(usr_msg);

    while(1)
    {
        xTaskNotifyWait(
                0,
                0xFFFFFFFF,
                &receivedCount,
                portMAX_DELAY);

        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

        sprintf(usr_msg,
                "Button pressed %lu times\r\n",
                receivedCount);

        printmsg(usr_msg);
    }
}


void button_task_handler(void *params)
{
    uint8_t currentState;
    uint8_t buttonPressCount=0;
    uint8_t previousState = 0;

    while(1)
    {
        currentState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

        /* Detect rising edge */
        if((currentState == GPIO_PIN_SET) &&
           (previousState == GPIO_PIN_RESET))
        {
            buttonPressCount++;

            sprintf(usr_msg,
                    "Button Pressed\r\n");
            printmsg(usr_msg);

            xTaskNotify(
                    ledTaskHandle,
                    buttonPressCount,
                    eSetValueWithOverwrite);
        }

        previousState = currentState;

        /* debounce */
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}



static void prvSetupUart(void)
{
	// enable the uart2 peripheral clock
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//PA2 is UART2_TX, PA3 is UART_RX
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}



static void prvSetupHardware(void)
{
	// setup LED and Button
	prvSetupGPIO();
	// setup UART
	prvSetupUart();
}

static void prvSetupGPIO(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct = {0};

	 __HAL_RCC_GPIOA_CLK_ENABLE();
	 __HAL_RCC_GPIOD_CLK_ENABLE();

	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

	  /*Configure GPIO pin : PA0 */
	  GPIO_InitStruct.Pin = GPIO_PIN_0;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /*Configure GPIO pin : PD12 */
	  GPIO_InitStruct.Pin = GPIO_PIN_12;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */


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

#ifdef  USE_FULL_ASSERT
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

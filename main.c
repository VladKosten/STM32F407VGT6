/**
* \file      main.c
* \brief     The main file of the project. The project is a template for the STM32F407VG microcontroller.
             Initialization of the microcontroller and the system clock is performed here. Call application functions.

* \authors   Vladislav Kosten (vladkosten@gmail.com)
* \copyright Copyright (c) 2024 Vladislav Kosten. All rights reserved.
* \warning   A warning may be placed here...
* \bug       Bug report may be placed here...
*/
//===============================================================================[ INCLUDE ]========================================================================================
#include "stm32f4xx_hal.h"
#include "stm32f407xx.h"
#include "assert.h"
#include "app_startup.h"

//=====================================================================[ INTERNAL MACRO DEFINITIONS ]===============================================================================

//====================================================================[ INTERNAL DATA TYPES DEFINITIONS ]===========================================================================

//===============================================================[ INTERNAL FUNCTIONS AND OBJECTS DECLARATION ]=====================================================================
/**
 * \brief  System Clock Configuration
 * \note   The system Clock is configured as follow:
 *           System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 168000000
 *            HCLK(Hz)                       = 168000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)            = 8000000
 * \param[in]  None
 * \param[out] None
 * \retval     None
*/
void SystemClock_Config(void);

/**
 * \brief      Init the timer which will be used as a time base for HAL.
 * \param[in]  TickPriority: Tick interrupt priority.
 * \param[out] None
 * \retval     HAL status, HAL_OK if all operations are OK, HAL_ERROR otherwise.
*/
void Gpio_Init(void);

/**
 * \brief  The TIM14 interrupt handler.
*/
TIM_HandleTypeDef htim14;


//============================================================================ [MAIN FUNCTION ]=================================================================================

int main(void)
{

  /* Init Hal */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Init GPIO */
  Gpio_Init();

  /* main loop */
  while (1)
  {
    AppStartupErr_t err = AppStartup();
    assert(err == APP_STARTUP_NO_ERR);
    (void)err;
    while(0);
  }

}

//============================================================================ [PRIVATE FUNCTIONS ]=================================================================================

/**
 * \brief  System Clock Configuration
 * \note   The system Clock is configured as follow:
 *           System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 168000000
 *            HCLK(Hz)                       = 168000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)            = 8000000
 * \param[in]  None
 * \param[out] None
 * \retval     None
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

/* Enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;

  /* Enable PLL */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;

  /* Set source of PLL */
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  /* Set the PLL multiplication factor */

  RCC_OscInitStruct.PLL.PLLM = 8;

  /* Set the PLL division factor */
  RCC_OscInitStruct.PLL.PLLN = 336;

  /* Set the PLL division factor for PLLP */
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;

  /* Set the PLL division factor for PLLQ */
  RCC_OscInitStruct.PLL.PLLQ = 7;

  /* *8Mhz HSE, 336Mhz VCO, 168Mhz for system clock, 48Mhz for USB, 48Mhz for SDIO, 48Mhz for RNG */

  /* Wait while all ok*/
  while (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)

  /* Enable HCLK - AHB, PCLK1 - APB1, PCLK2 - APB2, SYSCLK - System Clock */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;

  /* Set the SYSCLK source */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

  /* Set the AHB clock divider, 168 Mhz - Max */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

  /* Set the APB1 clock divider, 42 Mhz - Max */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;

  /* Set the APB2 clock divider 84 Mhz - Max */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  /* Wait while all ok, set flash latency - page 81 on RM0090 */
  while(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK);
}

void Gpio_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable clock for GPIOD */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  /* Init */
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* Enable clock for GPIOA */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA0 in input mode */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  /* Int */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * \brief      Init the timer which will be used as a time base for HAL.
 * \param[in]  TickPriority: Tick interrupt priority.
 * \param[out] None
 * \retval     HAL status, HAL_OK if all operations are OK, HAL_ERROR otherwise.
*/
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  RCC_ClkInitTypeDef    clkconfig;
  uint32_t              uwTimclock = 0;
  uint32_t              uwPrescalerValue = 0;
  uint32_t              pFLatency;
  /*Configure the TIM14 IRQ priority */
  HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, TickPriority ,0);

  /* Enable the TIM14 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);

  /* Enable TIM14 clock */
  __HAL_RCC_TIM14_CLK_ENABLE();

  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

  /* Compute TIM14 clock */
  uwTimclock = 2*HAL_RCC_GetPCLK1Freq();
  /* Compute the prescaler value to have TIM14 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

  /* Initialize TIM14 */
  htim14.Instance = TIM14;

  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM14CLK/1000) - 1]. to have a (1/1000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  htim14.Init.Period = (1000000U / 1000U) - 1U;
  htim14.Init.Prescaler = uwPrescalerValue;
  htim14.Init.ClockDivision = 0;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;

  if(HAL_TIM_Base_Init(&htim14) == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    return HAL_TIM_Base_Start_IT(&htim14);
  }

  /* Return function status */
  return HAL_ERROR;
}

/**
  * \brief      Suspend Tick increment.
  * \note       Disable the tick increment by disabling TIM14 update interrupt.
  * \param[in]  None
  * \param[out] None
  * \retval     None
  */
void HAL_SuspendTick(void)
{
  /* Disable TIM14 update Interrupt */
  __HAL_TIM_DISABLE_IT(&htim14, TIM_IT_UPDATE);
}

/**
  * \brief      Resume Tick increment.
  * \note       Enable the tick increment by Enabling TIM14 update interrupt.
  * \param[in]  None
  * \param[out] None
  * \retval     None
  */
void HAL_ResumeTick(void)
{
  /* Enable TIM14 Update interrupt */
  __HAL_TIM_ENABLE_IT(&htim14, TIM_IT_UPDATE);
}

/**
  * \brief      Microcontroller system initialization
  * \param[in]  None
  * \param[out] None
  * \retval     None
  */
void HAL_MspInit(void)
{

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

}

/**
  * \brief      Period elapsed callback in non blocking mode
  * \note       This function is called  when TIM14 interrupt took place, inside
  *             HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  *             a global variable "uwTick" used as application time base.
  * \param[in]  htim - TIM handle
  * \param[out] None
  * \retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }

}

/**
  * \brief      This function handles TIM14 global interrupt.
  * \note       This function is called inside HAL_TIM_IRQHandler(). It makes a direct call to
  *             HAL_TIM_PeriodElapsedCallback() to use HAL IncTick() in the user application.
  * \param[in]  None
  * \param[out] None
  * \retval     None
  */
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{

  /* Call IRQ Handler */
  HAL_TIM_IRQHandler(&htim14);

}


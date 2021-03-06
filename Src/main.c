
/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"
#include "dcmi.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */
#include "sdram.h"
#include "ov2640.h"
#include "wifi.h"
#include "image_processing.h"
#include "STM32F746SPI1_OLED.h"
#include "STM32F746I2C1_MPU6050.h"
#include "Complementary_fusion_filter.h"
#include "Button_Process.h"
#include "Motor.h"
#include "Control.h"
#include "delay.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define WARNING_ON() HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET)
#define WARNING_OFF() HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET)
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void CPU_CACHE_Enable(void);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

    /* USER CODE BEGIN 1 */

    CPU_CACHE_Enable();
    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

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
    MX_DMA_Init();
    MX_DCMI_Init();
    MX_I2C2_Init();
    MX_FMC_Init();
    MX_TIM7_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_TIM12_Init();

    /* USER CODE BEGIN 2 */
    BSP_SDRAM_Initialization_sequence(REFRESH_COUNT);
    ov2640_Init(ov2640_R160x120);
    // WIFI_Transparent_Init();
    ov2640_Config(OV2640_ADDR, ov2640_CONTRAST_BRIGHTNESS, OV2640_CONTRAST_LEVEL4, OV2640_BRIGHTNESS_LEVEL0);
    ov2640_SetYUV();
    //   ov2640_ContinuousStart(ov2640_FRAME_BUFFER);
    ov2640_SnapshotStart(ov2640_FRAME_BUFFER);

    Statue_Init();
    //状态初始化 在button_process文件里面

    //OLED_Init();
    //OLED液晶屏初始化

    MPU_Init();
    //MPU6050初始化

    Gyro_OFFSET();
    //陀螺仪初始校准

    MotorInit();
    //初始化电机

    while (thisStatus != BALL_SCAN);
    //等待直到预扫描完成

    X_MotorStop();
	Y_MotorStop();    
    //电机抱死并延时

    delay_ms2(1000);
    delay_ms2(1000);


    HAL_TIM_Base_Start_IT(&htim7);
    //时序中断开始

    WARNING_OFF();
	




    
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

        //    if (hdcmi.State == HAL_DCMI_STATE_SUSPENDED)
        //    {
        //      /*Process image*/
        //      Img_Process();
        //      HAL_DCMI_Resume(&hdcmi);
        //    }
    }
    /* USER CODE END 3 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 200;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Activate the Over-Drive mode 
    */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_I2C2;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);

    /**Configure the Systick interrupt time 
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick 
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

//时序定时器中断回调函数(htim7)
int16_t X_PWM = 0;
int16_t Y_PWM = 0;
int16_t dx = 0;
int16_t dy = 0;
int16_t x = 0;
int16_t y = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	

	
    if (htim->Instance == htim7.Instance)
    {
        static uint8_t SequenceNum = 0; //时序计数变量
        //static int8_t PWM = 0;
        //static uint8_t flag = 0;

        
        if (SequenceNum == 1)
        {
            MPU6050_USE_Data_Get();
            //获取MPU6050的数据(包含了一阶低通滤波和IIR滤波)

            Complementary_Fusion_Filter();
            //互补融合滤波提取角度

            if (Real_Angle.X_Real_Angle < -1200 || Real_Angle.X_Real_Angle > 1500)
            {
                X_MotorStop();
                Y_MotorStop();
                WARNING_ON();
            }
            else if (Real_Angle.Y_Real_Angle < -1500 || Real_Angle.Y_Real_Angle > 1500)
            {
                X_MotorStop();
                Y_MotorStop();
                WARNING_ON();
            }
            else
            {
                WARNING_OFF();
            }
            // if(Real_Angle.X_Real_Angle > 1500)
            //  {
            //     Direction_X_back_1();
            //  }

            // if(Real_Angle.X_Real_Angle < -400)
            // {
            //     Direction_X_to_1();
            // }

            // if (Real_Angle.Y_Real_Angle > 1100)
            // {
            //     Direction_Y_back_2();
            // }
            // if (Real_Angle.Y_Real_Angle < -1100)
            // {
            //     Direction_Y_to_2();
            // }

            //mpu6050_send_data(sensor.acc.origin.x, sensor.acc.origin.y, sensor.gyro.origin.x, \
                               Real_Angle.X_Real_Angle, Real_Angle.Y_Real_Angle, sensor.gyro.origin.y);

            //发送上位机观察波形

            
        }


        
        if (SequenceNum == 10)
        {
            

            x = ballPos[0].col - region[4].col;
            y = ballPos[0].row - region[4].row;

            dy = ballPos[0].row - ballPos[1].row;
            dx = ballPos[0].col - ballPos[1].col;

            X_PWM = X_LQR_Control(x, dx, Real_Angle.X_Real_Angle, sensor.gyro.origin.x);
            Y_PWM = Y_LQR_Control(y, dy, Real_Angle.Y_Real_Angle, sensor.gyro.origin.y);

            //X_Motor_Control(Y_PWM);
            //Y_Motor_Control(X_PWM);

            mpu6050_send_data(X_PWM, Y_PWM, sensor.gyro.averag.x, \
                    sensor.gyro.averag.y, Real_Angle.X_Real_Angle, Real_Angle.Y_Real_Angle);

            //mpu6050_send_data(sensor.acc.origin.x, sensor.acc.origin.y, sensor.gyro.origin.x,  \
                               Real_Angle.X_Real_Angle, Real_Angle.Y_Real_Angle, sensor.gyro.origin.y);  

  

            SequenceNum = 0;
        }
        SequenceNum++;
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

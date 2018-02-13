/**
  ******************************************************************************
  * @file    Project/main.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Init();
void Delay(uint16_t nCount);

void main(void)
{
  Init();

  while (1)
  {
    // Toggle a bit in the output data register to blink the LED
    Delay(1000);
    GPIO_WriteReverse(PORT_LED, PIN_LED);
  }
}


void Init()
{
    // LED
    GPIO_Init(PORT_LED, PIN_LED, GPIO_MODE_OUT_PP_LOW_SLOW);

    // Segments
    GPIO_Init(PORT_SEG, PIN_SEG_LATCH|PIN_SEG_DATA|PIN_SEG_CLK, GPIO_MODE_OUT_PP_LOW_SLOW);

    // Digits. High - disable
    GPIO_Init(PORT_DIG, PIN_DIG_1|PIN_DIG_2|PIN_DIG_3, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // Buttons
    GPIO_Init(PORT_BTN, PIN_BTN_UP|PIN_BTN_DN, GPIO_MODE_IN_PU_IT);

    // Controls
    GPIO_Init(PORT_C, PIN_C_8|PIN_C_4|PIN_C_2|PIN_C_1|PIN_C_05, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PORT_C16, PIN_C_16, GPIO_MODE_OUT_PP_LOW_SLOW);

}

void Delay(uint16_t nCount)
{
  uint8_t i;
  for(; nCount !=0 ; nCount--) {
    for(i=255;i!=0;i--) {}
  }
}

// This is called by some of the SPL files on error.
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  (void)file;
  (void)line;

  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

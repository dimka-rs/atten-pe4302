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
uint32_t count = 0;
uint32_t delay_ms;
uint8_t  cur_digit = 0;

const uint8_t Digits[] = 
{
SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F,
SEG_B|SEG_C,
SEG_A|SEG_B|SEG_D|SEG_E|SEG_G,
SEG_A|SEG_B|SEG_C|SEG_D|SEG_G,
SEG_B|SEG_C|SEG_F|SEG_G,
SEG_A|SEG_C|SEG_D|SEG_F|SEG_G,
SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,
SEG_A|SEG_B|SEG_C,
SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,
SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G
};


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Init();
void DelayMs(uint32_t delay);
void SetDigit(uint8_t digit);

void main(void)
{
    Init();

    while (1)
    {
        // Dig1 on
        GPIO_WriteLow(PORT_DIG, PIN_DIG_1);
        DelayMs(DIGIT_ON_MS);
        // Dig1 off
        GPIO_WriteHigh(PORT_DIG, PIN_DIG_1);
        DelayMs(DIGIT_OFF_MS);
        // Dig2 on
        GPIO_WriteLow(PORT_DIG, PIN_DIG_2);
        DelayMs(DIGIT_ON_MS);
        // Dig2 off
        GPIO_WriteHigh(PORT_DIG, PIN_DIG_2);
        DelayMs(DIGIT_OFF_MS);
        // Dig3 on
        GPIO_WriteLow(PORT_DIG, PIN_DIG_3);
        DelayMs(DIGIT_ON_MS);
        //Dig3 off
        GPIO_WriteHigh(PORT_DIG, PIN_DIG_3);
        DelayMs(DIGIT_OFF_MS);

        if(count > 1000){
            count = 0;
            cur_digit++;
            if(cur_digit > 9) cur_digit = 0;
            SetDigit(cur_digit);
        }
    }
}


void Init()
{
    // Clock
    // HSIDIV=/8, CKM=HSIDIV, CPUDIV=/1
    // Not required, defaults are used
    //CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV8);
    //CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);

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

    // Timer 4
    // Used to get 1 ms clock
    // 2 MHz / 16 = 125000 Hz
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_16, 125);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);

    enableInterrupts();
}


void DelayMs(uint32_t delay)
{
    delay_ms = delay;
    while(delay_ms);
}

void SetDigit(uint8_t digit)
{
    uint8_t i;

    digit = Digits[digit];
    for(i = 0; i < 8; i++){
        if( (digit & (128 >> i)) == 0){
            GPIO_WriteLow(PORT_SEG, PIN_SEG_DATA);
        } else {
            GPIO_WriteHigh(PORT_SEG, PIN_SEG_DATA);
        }
        GPIO_WriteLow(PORT_SEG, PIN_SEG_CLK);
        DelayMs(1);
        GPIO_WriteHigh(PORT_SEG, PIN_SEG_CLK);
    }
    GPIO_WriteLow(PORT_SEG, PIN_SEG_LATCH);
    DelayMs(1);
    GPIO_WriteHigh(PORT_SEG, PIN_SEG_LATCH);
}

/* --- ISR --- */

void tim4_isr(void) __interrupt(ITC_IRQ_TIM4_OVF) {

    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    delay_ms--;
    count++;
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

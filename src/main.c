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
#define UPDATE_INTVL 500 //display update every X ms
#define ATTEN_MAX 63 //max attenuation is 31.5 dB, I use doubled value

uint32_t count = 0;
uint32_t delay_ms;
int8_t atten = 0;
uint8_t Display[] = {0, 0, 0};

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
SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G,
0
};


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Init();
void DelayMs(uint32_t delay);
void SetDigit(uint8_t digit, uint8_t dot);

void main(void)
{
    Init();

    while (1)
    {
        // Dig1 on
        SetDigit(Display[0], 0);
        GPIO_WriteLow(PORT_DIG, PIN_DIG_1);
        DelayMs(DIGIT_ON_MS);
        // Dig1 off
        GPIO_WriteHigh(PORT_DIG, PIN_DIG_1);
        DelayMs(DIGIT_OFF_MS);
        // Dig2 on
        SetDigit(Display[1], 1);
        GPIO_WriteLow(PORT_DIG, PIN_DIG_2);
        DelayMs(DIGIT_ON_MS);
        // Dig2 off
        GPIO_WriteHigh(PORT_DIG, PIN_DIG_2);
        DelayMs(DIGIT_OFF_MS);
        // Dig3 on
        SetDigit(Display[2], 0);
        GPIO_WriteLow(PORT_DIG, PIN_DIG_3);
        DelayMs(DIGIT_ON_MS);
        //Dig3 off
        GPIO_WriteHigh(PORT_DIG, PIN_DIG_3);
        DelayMs(DIGIT_OFF_MS);

        if(count > UPDATE_INTVL){
            count = 0;
            if(atten > ATTEN_MAX) atten = 0;
            if(atten < 0) atten = ATTEN_MAX;
            if(atten % 2 == 0) {
                Display[2] = 0;
            } else {
                Display[2] = 5;
            }
            Display[1] = (atten / 2) % 10;
            Display[0] = (atten / 2) / 10;
            // dim lead zero
            if(Display[0] == 0) Display[0] = 10;
            //C05 is PC3, so shit left 3 bits
            GPIO_Write(PORT_C, atten << 3);
            // 31 is doubled 15.5 db
            if(atten > 31) {
                GPIO_WriteHigh(PORT_C16, PIN_C_16);
            } else {
                GPIO_WriteLow(PORT_C16, PIN_C_16);
            }
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
    GPIO_Init(PORT_LED, PIN_LED, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // Segments
    GPIO_Init(PORT_SEG, PIN_SEG_LATCH|PIN_SEG_DATA|PIN_SEG_CLK, GPIO_MODE_OUT_PP_LOW_SLOW);

    // Digits. High - disable
    GPIO_Init(PORT_DIG, PIN_DIG_1|PIN_DIG_2|PIN_DIG_3, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // Buttons are pulled up. React to fall
    GPIO_Init(PORT_BTN, PIN_BTN_UP|PIN_BTN_DN, GPIO_MODE_IN_PU_IT);
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);

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

void SetDigit(uint8_t digit, uint8_t dot)
{
    uint8_t i;

    digit = Digits[digit];
    if(dot) digit += SEG_H;
    for(i = 0; i < 8; i++){
        if( (digit & (128 >> i)) == 0){
            GPIO_WriteLow(PORT_SEG, PIN_SEG_DATA);
        } else {
            GPIO_WriteHigh(PORT_SEG, PIN_SEG_DATA);
        }
        GPIO_WriteLow(PORT_SEG, PIN_SEG_CLK);
        GPIO_WriteHigh(PORT_SEG, PIN_SEG_CLK);
    }
    GPIO_WriteLow(PORT_SEG, PIN_SEG_LATCH);
    GPIO_WriteHigh(PORT_SEG, PIN_SEG_LATCH);
}

/* --- ISR --- */

void tim4_isr(void) __interrupt(ITC_IRQ_TIM4_OVF) {

    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    delay_ms--;
    count++;
}


void portd_isr(void) __interrupt(ITC_IRQ_PORTD){
    uint8_t read;

    //TODO: clear it bit?
    read = GPIO_ReadInputData(PORT_BTN);
    //Here I check which button is _not_ pressed (==1)
    if(read & PIN_BTN_UP) atten--; 
    if(read & PIN_BTN_DN) atten++;
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

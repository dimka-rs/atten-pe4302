#ifndef _MAIN_H_
#define _MAIN_H_

#define PORT_SEG GPIOD
#define PIN_SEG_LATCH GPIO_PIN_4
#define PIN_SEG_DATA  GPIO_PIN_5
#define PIN_SEG_CLK   GPIO_PIN_6

#define PORT_DIG GPIOA
#define PIN_DIG_1 GPIO_PIN_1
#define PIN_DIG_2 GPIO_PIN_2
#define PIN_DIG_3 GPIO_PIN_3

#define PORT_BTN GPIOD
#define PIN_BTN_UP GPIO_PIN_3
#define PIN_BTN_DN GPIO_PIN_2

#define PORT_C GPIOC
#define PIN_C_8  GPIO_PIN_7
#define PIN_C_4  GPIO_PIN_6
#define PIN_C_2  GPIO_PIN_5
#define PIN_C_1  GPIO_PIN_4
#define PIN_C_05 GPIO_PIN_3

/* don't forget to pull up */
#define PORT_C16 GPIOB
#define PIN_C_16 GPIO_PIN_4

#define PORT_LED GPIOB
#define PIN_LED GPIO_PIN_5

/* 7 seg digits */
#define SEG_A 0x01
#define SEG_B 0x02
#define SEG_C 0x04
#define SEG_D 0x08
#define SEG_E 0x10
#define SEG_F 0x20
#define SEG_G 0x40
#define SEG_H 0x80

#define DIGIT_ON_MS 3
#define DIGIT_OFF_MS 3

#endif

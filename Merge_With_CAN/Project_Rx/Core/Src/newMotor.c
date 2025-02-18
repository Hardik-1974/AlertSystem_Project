#include "stm32f4xx_hal.h"

void TIM4_ms_Delay(uint32_t delay);

// Definitions for User-Defined Functions

	void GPIO_Init(){
	    // Enable the Clock for GPIOA
	    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	    // Set PA5 and PA6 in alternate function mode
	    GPIOA->MODER |= ( GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 );

	    // Configuring PA5 and PA6 as pull down
	    GPIOA->PUPDR |= ( GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR6_1 );

	    // Select AF1 for PA5 and AF2 for PA6
	    GPIOA->AFR[0] |= ( GPIO_AFRL_AFRL5_0 | GPIO_AFRL_AFRL6_1 );
	}

	void TIM2_Init(){
	    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	    TIM2->PSC = 16-1; //Setting the clock frequency to 1MHz.
	    TIM2->ARR = 20000; // Total period of the timer = 20ms
	    TIM2->CNT = 0;
	    TIM2->CCMR1 = 0x0060; //PWM mode for the timer
	    TIM2->CCER |= 1; //Enable channel 1 as output
	    TIM2->CCR1 = 5000; // Pulse width for PWM
	}

	void TIM3_Init(){
	    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	    TIM3->PSC = 16-1; //Setting the clock frequency to 1MHz.
	    TIM3->ARR = 20000; // Total period of the timer = 20ms
	    TIM3->CNT = 0;
	    TIM3->CCMR1 = 0x0060; //PWM mode for the timer
	    TIM3->CCER |= 1; //Enable channel 1 as output
	    TIM3->CCR1 = 5000; // Pulse width for PWM
	}
/*
	void TIM4_ms_Delay(uint32_t delay){
	    RCC->APB1ENR |= 1<<2; //Start the clock for the timer peripheral
	    TIM4->PSC = 16000-1; //Setting the clock frequency to 1kHz.
	    TIM4->ARR = (delay); // Total period of the timer
	    TIM4->CNT = 0;
	    TIM4->CR1 |= 1; //Start the Timer
	    while(!(TIM4->SR & TIM_SR_UIF)){} //Polling the update interrupt flag
	    TIM4->SR &= ~(0x0001); //Reset the update interrupt flag
	}
*/
	void Motor_Control(uint8_t direction, uint16_t speed){
	    // direction = 1 => Clockwise Rotations
	    // direction = 0 => Clockwise Rotations

	    if (direction == 0){
	        /* Sending PWM to IN1 and switching off input to IN2 */

	        // Waiting for the pulse to finish, if any
	        while (TIM3->CNT <= TIM3->CCR1 ){};

	        // Then switching off Timer 3
	        TIM3->CR1 &= ~TIM_CR1_CEN;

	        // Adding a Delay of 1ms
	        TIM4_ms_Delay(1);

	        // Starting Timer 2
	        TIM2->CR1 |= TIM_CR1_CEN;

	        // Changing the speed for the motor
	        TIM2->CCR1 = (uint16_t)((speed/225.0)*20000);
	    }
	    else{
	        /* Sending PWM to IN2 and switching off input to IN1 */

	        // Waiting for the pulse to finish, if any
	        while (TIM2->CNT <= TIM2->CCR1 ){};

	        // Then switching off Timer 2
	        TIM2->CR1 &= ~TIM_CR1_CEN;

	        // Adding a Delay of 1ms
	        TIM4_ms_Delay(1);

	        // Starting Timer 3
	        TIM3->CR1 |= TIM_CR1_CEN;

	        // Changing the speed for the motor
	        TIM3->CCR1 = (uint16_t)((speed/225.0)*20000);
	    }
	}

	void TIM4_ms_Delay(uint32_t delay){
	    RCC->APB1ENR |= 1<<2; //Start the clock for the timer peripheral
	    TIM4->PSC = 16000-1; //Setting the clock frequency to 1kHz.
	    TIM4->ARR = (delay); // Total period of the timer
	    TIM4->CNT = 0;
	    TIM4->CR1 |= 1; //Start the Timer
	    while(!(TIM4->SR & TIM_SR_UIF)){} //Polling the update interrupt flag
	    TIM4->SR &= ~(0x0001); //Reset the update interrupt flag
	}


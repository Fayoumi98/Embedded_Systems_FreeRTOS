/*
 * Embedded_RTOS_Project.c
 * Author : Fayoumi
 */

#include <avr/io.h>
#include "include/headers.h"
#include "include/ADC.h"
#include "include/LCD.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "FreeRTOSConfig.h"
#include "include/PWM.h"

/*
	mode state : 
				1 for mode A
				2 for mode B
				3 for mode C

*/
uint8_t mode_state = 1;

// create handlers to manipulate each task state
TaskHandle_t modeA_H;
TaskHandle_t modeB_H;
TaskHandle_t modeC_H;

// create LCD Semaphore
SemaphoreHandle_t LCD_Flag;

// mode A implementation (system for an oven)
void modeA(void)
{	
	uint16_t temperature;
	
	while(1)
	{
		// read ADC channel 0
		temperature = ADC_read(0);
		
		// if ADC0 > 100 buzzer duty = 20 led duty = 100
		if((temperature > 100) && (temperature <300))
		{
			PWM_OC1A_set_duty(100);
			PWM_OC0_set_duty(20);
		}
		// if ADC0 > 300 buzzer duty = 40 led duty = 70
		else if((temperature > 300) && (temperature <600))
		{
			PWM_OC1A_set_duty(70);
			PWM_OC0_set_duty(40);
		}
		// if ADC0 > 600 buzzer duty = 60 led duty = 30
		else if((temperature > 600) && (temperature <900))
		{
			PWM_OC1A_set_duty(30);
			PWM_OC0_set_duty(60);
		}
		// if ADC0 > 900 buzzer duty = 80 led duty = 10
		else if(temperature > 900)
		{
			PWM_OC1A_set_duty(10);
			PWM_OC0_set_duty(80);
		}
		// if ADC0 < 100 buzzer duty = 0 led duty = 0
		else
		{
			PWM_OC1A_set_duty(0);
			PWM_OC0_set_duty(0);
		}
		
		if(xSemaphoreTake(LCD_Flag,50))
		{
			// Display current mode A
			LCD_write_command(0x1);
			LCD_write_command(0x80);
			LCD_write_string(( uint8_t*)" Mode A Active");
			LCD_write_command(0xC0);
			LCD_write_string("M");
			LCD_write_num(mode_state);
			
			LCD_write_string((uint8_t*)" Temp = ");
			LCD_write_num(temperature);
			xSemaphoreGive(LCD_Flag);
			
		}
		vTaskDelay(1);
	}
}

// mode B implementation (Emergency lighting system)
void modeB(void)
{
	uint8_t Light;
	uint8_t duty;

	while(1)
	{
		// read ADC channel 1
		Light = ADC_read(1);
		duty = ((Light-93)/251.0)*100;
		
		// set duty cycle
		PWM_OC1A_set_duty(duty);
		
	
		if(xSemaphoreTake(LCD_Flag,50))
		{
			// Display current mode B
			LCD_write_command(0x1);
			LCD_write_command(0x80);
			LCD_write_string(( uint8_t*)" Mode B Active");
			LCD_write_command(0xC0);
			LCD_write_num(mode_state);

			LCD_write_string((uint8_t*)"   duty = ");
			LCD_write_num(duty);
			xSemaphoreGive(LCD_Flag);
		}
		vTaskDelay(1);
	}
}

// mode C implementation (Calculator)
void modeC(void)
{
	uint8_t num1;
	uint8_t num2;
	uint8_t op;
	uint8_t result;
	uint8_t step = 0;

    while(1)
    {

		if (xSemaphoreTake(LCD_Flag, 50)) 
		{
			LCD_write_command(0x1);
			LCD_write_command(0x80);
			LCD_write_string(( uint8_t*)" Mode C Active");
			xSemaphoreGive(LCD_Flag);
		}
		
		uint8_t key = keypad_read();
		
		// Check for a valid key press
		if (key != -1)
		{
			if (step == 0)
			{
				num1 = key;
				step++;
			}
			else if (step == 1)
			{
				op = key;
				step++;
			}
			else if (step == 2)
			{
				num2 = key;
				step++;
			}
			else if (step == 3)
			{
				if (key == '=')
				{
					if (op == '+')
					{
						result = num1 + num2;
					}
					else if (op == '-')
					{
						result = num1 - num2;
					}
					else if (op == '*')
					{
						result = num1 * num2;
					}
					else if (op == '/')
					{
						if (num2 != 0)
						{
							result = num1 / num2;
						}
						else
						{
							result = 0; // Handle division by zero
						}
					}
					step = 0;
				}
				
				 LCD_write_command(0xC0);
				 LCD_write_string(num1 + '0');
				 LCD_write_string(op);
				 LCD_write_string(num2 + '0');
				 LCD_write_string('=');
				 LCD_write_num(result);
				
				
			}
			
			vTaskDelay(1);
		}
    }
}



int main(void)
{
	LCD_Flag = xSemaphoreCreateBinary();
	xSemaphoreGive(LCD_Flag);
		
	// initialize LCD display
	LCD_init();
	
	// initialize ADC
	ADC_init();
	
	// PD2 Input pin (pull up)
	PORTD = 1<<2;
	// GICR general interrupt control register
	// enables masked interrupts INT0 INT1 INT2
	// BIT6 External interrupt request 0 enable
	GICR |= 1<<INT0;
	// SREG : Status Register (Non Memory Mapped Register)
	// Note : SREG has a virtual address
	// Global interrupt flag enabled
	SREG |= 1<<7;
	
	// Initialize Timer 0 (Fast PWM)
	PWM_OC0_init();
	// Initialize Timer 1 (Fast PWM)
	PWM_OC1A_init();
	
	// Initialize keypad
	keypad_init();
	
	// create a task for each mode of operation
    xTaskCreate(modeA, "mode_A", 128, NULL, 1, &modeA_H);
    xTaskCreate(modeB, "mode_B", 128, NULL, 1, &modeB_H);
    xTaskCreate(modeC, "mode_C", 128, NULL, 4, &modeC_H);
 
	// enable scheduler
    vTaskStartScheduler();
    
}




ISR(INT0_vect)
{

	/*
	mode state : 
				1 for mode A
				2 for mode B
				3 for mode C

	*/
	if (mode_state==1)
	{
		LCD_write_command(0x1);
		vTaskSuspend(modeB_H);
		vTaskSuspend(modeC_H);
		vTaskResume(modeA_H);
		
	}
	else if (mode_state==2)
	{
		LCD_write_command(0x1);
		vTaskSuspend(modeA_H);
		vTaskSuspend(modeC_H);
		vTaskResume(modeB_H);
	}
	else if (mode_state==3)
	{
		LCD_write_command(0x1);
		vTaskSuspend(modeA_H);
		vTaskSuspend(modeB_H);
		vTaskResume(modeC_H);
	}

	if(mode_state==3)
	{
		mode_state = 1;
	}
	else
	{
		mode_state++;
	}
	
}
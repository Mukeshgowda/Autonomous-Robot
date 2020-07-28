#include <stdlib.h>
#include "dorobo32.h"
#include "FreeRTOS.h"
#include "task.h"
#include "trace.h"
#include "adc.h"
#include "motor.h"
#include "stm32f0xx_hal.h"
int speed =60;
static void robot_travel();
void stop()
{

	motor_set(0,0);// Left Motor
	motor_set(2,0);//  Right Motor
	motor_set(1,0);
}
void moveback()
{
	int i =0;
	while(i<100)
	{
	motor_set(0,-speed);// Left Motor
	motor_set(2,speed);//  Right Motor
	motor_set(1,0); // Back motor
    vTaskDelay(1);
    i++;
	}
}
void turnleft()
{
	int i =0;
	while(i<100)
	{
	motor_set(0,-speed+10);// Left Motor
	motor_set(2,-speed+10);//  Right Motor
	//motor_set(1,-speed);
    vTaskDelay(1);
    i++;
	}
}
void turnright()
{
	int i =0;
	while(i<100)
	{
	motor_set(0,speed-20);// Left Motor
	motor_set(2,speed-20);//  Right Motor
	//motor_set(1,speed);
    vTaskDelay(1);
    i++;
	}

}
void moveforward()
{

	motor_set(0,speed);// Left Motor
	motor_set(1,0); // Centre Motor
	motor_set(2,-speed);//  Right Motor
    vTaskDelay(1);

}
void moveforward2()
{
	int i =0;
	while(i<100)
	{
 	motor_set(0,speed);// Left Motor
	motor_set(1,0); // Centre Motor
	motor_set(2,-speed);//  Right Motor
	vTaskDelay(5);
	i++;
	}
}

int main()
{

    dorobo_init();			//Call dorobo_init() function to initialize HAL, Clocks, Timers etc.	
	

    xTaskCreate(robot_travel,"Robo Run", 512, NULL, 2, NULL);	//xTaskCreate(robot_travel, "robo run", 512, NULL, 2, NULL);	//create Robot travel



	vTaskStartScheduler();	//start the freertos scheduler

	return 0;				//should not be reached!
}

static void robot_travel()
{
	uint8_t Target1,Target2,switch1,switch2;
			uint32_t Proximity1=0,Proximity2=0;
			trace_init();
			digital_init();
			adc_init();


			while (1)
			{

				Target1 = digital_get_pin(3); // Target 1
				Target2 = digital_get_pin(4);  // target 2
				switch1 = digital_get_pin(1);  // switch 1
				switch2 = digital_get_pin(2);  // switch 2
				Proximity1 = adc_get_value(0); //Proximity1
				Proximity2 = adc_get_value(9);//Proximity2
				tracef("TARGET DETECTOR 1: %i",Target1);
				tracef("TARGET DETECTOR 2: %i\n",Target2);
				tracef("SWITCH 1: %i",switch1);
				tracef("SWITCH 2: %i\n",switch2);
				tracef(" Proximity 1: %i",Proximity1);
				tracef(" Proximity 2: %i\n",Proximity2);

				if(Target1 == 0 && Target2 == 0 ) //moveforward();
				{
					tracef(" Target detected happy married life");

					//while(!(switch1==0)&&(switch2==0))
					while((Target1 == 0 && (Proximity1 < 2000 || Proximity2 < 1500)))

					{

						Proximity1 = adc_get_value(0); //Proximity1
						Proximity2 = adc_get_value(9); // Proximity2
						Target1 = digital_get_pin(3);
						//switch1 = digital_get_pin(1);
						//switch2 = digital_get_pin(2);
						tracef(" Moving forward");
						motor_set(0,speed);// Left Motor
						motor_set(1,0); // Centre Motor
						motor_set(2,-speed);
						if (switch1==0 || switch2==0)
							{
							goto end;
							}
					}

					tracef(" Proximity 1: %i",Proximity1);
					tracef(" Proximity 2: %i\n",Proximity2);
					tracef(" Target %i",Target1);
					tracef("SWITCH 1: %i",switch1);
					tracef("SWITCH 2: %i",switch2);

					if (switch1==0)
						{
						tracef(" Halting");
						goto end;

						}

				}


				else
				{
					if((Proximity2 > 900) && (Proximity1 > 2000))
					{
						for(int i = 0; i < 3 ;i++)
						{
						moveback();
						}
					}

		        switch(!((Proximity2 > 1200)))
		        {
		        case 0: stop();
		        	    moveback();
		                stop();
				        turnleft();
				      //  moveforward2();
				        //turnright();
				        break;

		        default: moveforward();
		                break;
		        }

		        switch(!((Proximity1 > 2000)))
		        {
		        case 0: stop();
		        	    moveback();
		        	    stop();
				        turnright();
				       //moveforward2();
				       // turnleft();
				        break;

		        default: moveforward();
		                break;
		        }

				}

				vTaskDelay(50);
			}
			end:
			stop();


}


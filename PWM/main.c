#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

uint32_t Periodo;

void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

void pwm_config(void){
		//Configura PWM Clock
		SysCtlPWMClockSet(SYSCTL_PWMDIV_64); // Divisor de frecuencias desdes SYSCTL_PWMDIV_1 hasta SYSCTL_PWMDIV_64
	    //Habilita perifericos
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	    //Configura PF1 Pin para PWM
	    GPIOPinConfigure(GPIO_PF1_M1PWM5);
	    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
	    //Configura PWM Options
	    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	    //Set the Period (expressed in clock ticks)
	    Periodo = SysCtlClockGet();
	    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, Periodo);
	    //Set PWM duty-50% (Period /2)
	    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,Periodo/2);
	    // Habilita el generador PWM
	    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	    // Enciende el pin de salida
	    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT , true);
}

void pwm_set(int x){
	float y=(float)x/100;
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,Periodo*y);
}

int main(void){
   SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //Configuración del reloj del procesador
   pwm_config();

    while(1)
    {
        delayMS(1000);
        pwm_set(20);//Porcentaje del ciclo de trabajo
        delayMS(1000);
        pwm_set(40);
        delayMS(1000);
        pwm_set(60);
        delayMS(1000);
        pwm_set(80);
        delayMS(1000);
        pwm_set(100);
        delayMS(100);
        pwm_set(95);
    }

}


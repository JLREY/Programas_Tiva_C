// Ing. Electrónica 2018, Luis Rey y Diego 804-A
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

const int Freq = 10; // Frecuencia del timer

int main(void) {
	uint32_t ui32Period;
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//configura el reloj
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//habilita el periferico GPIO, parpadeo azul
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //configural los pines de salida GPIO
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//habilita el periferico del Timer 0
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);// configura el modo de operacion del timer en periodo completo

	ui32Period = (SysCtlClockGet())/Freq ; //SysCtlClockGet: determina la frecuencia del reloj del procesador
											//relación de 1s=1Hz

	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period); //carga al temporizador, el valor deseado

	IntEnable(INT_TIMER0A);// habilita la inrrupcion para el timer 0A
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//habilita la interrupción para el timer A timeout
	IntMasterEnable();//Permite que el procesador responda a las interrupciones
	TimerEnable(TIMER0_BASE, TIMER_A);// Habilita el funcionamiento del timer A

	while(1)  {  }
}

void Timer0IntHandler(void) {//Secuencia de interrupción

	// Limpia la interrupción del timer
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // ==============================================================
    //
    //					CODIGO AQUI (°_°)/
    //
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))  {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
	}  else  {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	}

    // ==============================================================

}


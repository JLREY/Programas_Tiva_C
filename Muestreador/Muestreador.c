// Ing. Electrónica 2018, Luis Rey y Diego 804-A
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/adc.h"

//Constantes
const int Freq = 1000; // Frecuencia del timer

//variables
uint32_t ADC0Value; //Variable para almacenar los datos leidos del adc
volatile float valor;

void cofigurar(void){
		uint32_t Periodo;
		//SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//configura el reloj
		SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); // Reloj a 80 MHz

	    // *** Habilitación de periféricos
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);//Habilitacion del periferico ADC0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//habilita el periferico del Timer 0

		// *** ADC0
		ADCHardwareOversampleConfigure(ADC0_BASE,1);
		ADCReferenceSet(ADC0_BASE,ADC_REF_INT);
		ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_TIMER, 0);//. Configurar el muestreo del ADC disparado por el timer
		ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);//ADC_CTL_IE configura la interrupcion cuando el paso se complete,
		ADCSequenceEnable(ADC0_BASE, 1);// Especiifaca la secuencia de muestreo	//ADC_CTL_END define este como el ultimo paso //ADC_CTL_CHx x=0,1,2...23 puerto pin del ADC

		// *** Timer0
		TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);// configura el modo de operacion del timer en periodo completo
		Periodo = (SysCtlClockGet())/Freq ; //SysCtlClockGet: determina la frecuencia del reloj del procesador
												//relación de 1s=1Hz
		TimerLoadSet(TIMER0_BASE, TIMER_A, Periodo); //carga al temporizador, el valor deseado
		TimerControlTrigger(TIMER0_BASE,TIMER_A,true);

		// *** Interrupciones
		ADCIntEnable(ADC0_BASE,1);// Habilita la inrrupcion para el ADC
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);// Habilita la interrupción para el timer A timeout
		IntEnable(INT_TIMER0A); // Activa la inrrupcion para el timer
		IntEnable(INT_ADC0SS1); // Activa la inrrupcion para el ADC

		IntMasterEnable();//Permite que el procesador responda a las interrupciones

		TimerEnable(TIMER0_BASE, TIMER_A);// Habilita el funcionamiento del timer A
}


int main(void) {
	cofigurar();

	while(1)  {
		valor = ADC0Value * 3.3 / 4095;
	    // ==============================================================
	    //
	    //					CODIGO AQUI (°_°)/
	    //
	    // ==============================================================
	}
}

void Timer0IntHandler(void) {//Secuencia de interrupción
	// Limpia la interrupción del timer
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void ADC0SS1IntHandler(void){//Secuencia de interrupción
	ADCIntClear(ADC0_BASE, 1);//limpia el registro de interrupcion
    ADCSequenceDataGet(ADC0_BASE, 1, &ADC0Value); //asigna la lectura a la variable ADC0Value
}

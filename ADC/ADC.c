// Ing. Electrónica 2018, Luis Rey y Diego 804-A
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //Establecimiento de un reloj de a 40 MHz

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);//Habilitacion del periferico ADC0
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);//. Configurar el muestreo del ADC a 1M sps con alta prioridad
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);//ADC_CTL_IE configura la interrupcion cuando el paso se complete,
																				//ADC_CTL_END define este como el ultimo paso
																				//ADC_CTL_CHx x=0,1,2...23 puerto pin del ADC
	ADCSequenceEnable(ADC0_BASE, 1);// Especiifaca la secuencia de muestreo

	//variables
	float valor;
	uint32_t ADC0Value; //Variable para almacenar los datos leidos del adc

	while(1)  {
	    ADCIntClear(ADC0_BASE, 1);//limpia el registro de interrupcion
	    ADCProcessorTrigger(ADC0_BASE, 1);	//activa la secuencia de muestreo
	    while(!ADCIntStatus(ADC0_BASE, 1, false)) //devuelve el estado de la interrupcion que no se ha procesado
	    {}
	    ADCSequenceDataGet(ADC0_BASE, 1, &ADC0Value); //asigna la secuencia de datos muestreados a la
	    												//varaible especificada en memoria deve ser de tamaño correcto
	    valor = ADC0Value * 3.3 / 4095;

	    // ==============================================================
	    //
	    //					CODIGO AQUI (°_°)/
	    //
	    // ==============================================================
	}
}

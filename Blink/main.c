// Ing. Electrónica 2018, Luis Rey y Diego 804-A
//Librerias requeridas por la tarjeta
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


int main(void){//Programa Principal
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//Configuración del reloj del procesador

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Habilita el periferico GPIO
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //Configura los pines de salida GPIO de acuerdo al puerto (PORTF) utilizado.

	while(1){ //loop infinito

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, 0); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de 0.
		SysCtlDelay(10000000); //Espera 1 segundos

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, 2); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de 2.
		SysCtlDelay(10000000); //Espera 1 segundos

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, 4); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de 4.
		SysCtlDelay(10000000); //Espera 1 segundos

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 6); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de 6.
		SysCtlDelay(10000000); //Espera 1 segundos

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de 8.
		SysCtlDelay(10000000); //Espera 1 segundos

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 10); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de 10.
		SysCtlDelay(10000000); //Espera 1 segundos

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 12); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de 12.
		SysCtlDelay(10000000); //Espera 1 segundos

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 14); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de 14.
		SysCtlDelay(10000000); //Espera 1 segundos

	}
}

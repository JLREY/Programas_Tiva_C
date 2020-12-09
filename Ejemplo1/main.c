// Ing. Electrónica 2018, Luis Rey y Diego 804-A
//Librerias requeridas por la tarjeta
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

//Cabeceras de fución
void Apagar();
void Color();

//Variables Globales
volatile int x=0;

int main(void){//Programa Principal
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//Configuración del reloj del procesador
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Habilita el periferico GPIO
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //Configura los pines de salida GPIO de acuerdo al puerto (PORTF) utilizado.

	int i; 	//Variables Locales

	for (i=0; i<8;i++){ //Ciclo iterativo
		Color(); // Llamada a una funcion sin parametros
		x=x+2;
	}
	Apagar(0); // Llamada a una funcion con parametros
	return(0);
}

void Color(){
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, x); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de x.
	SysCtlDelay(10000000); //Espera 1 segundos
}

void Apagar(int y){
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, y); //Escribe a la salida de los pines del puerto (PORTF) seleccionado, el valor de x.
}

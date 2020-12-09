// Ing. Electrónica 2018, Luis Rey y Diego 804-A
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/uart.h"

void UART_Config(void){
	// *** Habilitación de periféricos
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Habilitar modulo UART
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Habilitar PINES

	// *** Configuracion de PINES
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// *** Configuracion del modulo UART
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	//UARTDisable(UART0_BASE); // Deshabilitacion del modulo UART
}


int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//configura el reloj

	UART_Config();// Llamada a la funcion

	char c='a';
	while(1)  {

		if (UARTCharsAvail(UART0_BASE))
			c = UARTCharGet(UART0_BASE); //Recepcion de caracteres

		UARTCharPut(UART0_BASE, c); //Envio de caracteres
	    // ==============================================================
	    //
	    //					CODIGO AQUI (°_°)/
	    //
	    // ==============================================================

	}
}

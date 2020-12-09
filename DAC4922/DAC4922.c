// Ing. Electrónica 2018, Luis Rey y Diego 804-A

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"



const uint32_t muestras[16]={2048, 2832, 3496, 3940, 4095, 3940, 3496, 2832, 2048, 1264, 600, 156, 0, 156, 600, 1264};


void DAC_config(){
	// *** Habilitación de periféricos
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// *** Configuracion de PINES
    GPIOPinConfigure(GPIO_PA2_SSI0CLK); // CLK pin
    GPIOPinConfigure(GPIO_PA5_SSI0TX);  // TX pin
    GPIOPinConfigure(GPIO_PA3_SSI0FSS); // SS pin
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_3 ); // Configuracion de pines para moudulo SSI

    // *** Configuracion del modulo SSI
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 20000000, 16);
    SSIEnable(SSI0_BASE); // Habilitacion de SSI0
}


int main(void)
{
    uint8_t ind = 0;
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); // Reloj a 80 MHz

    DAC_config();
    while(1)
    {
        SSIDataPut(SSI0_BASE, (0x3000 | muestras[ind]) & 0x3FFF); //Envio de datos
        while(SSIBusy(SSI0_BASE)){} //Espera termino de transmicion

	    // ==============================================================
	    //
	    //					CODIGO AQUI (°_°)/
	    //
        ind++;
        ind %= 16;
	    // ==============================================================
    }
}

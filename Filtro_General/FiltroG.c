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
#include <math.h>
#include "fdacoefs.h"


//Constantes
const int Freq = 10000; // Frecuencia del timer
#define pi 3.141592	 //Constante pi
#define N 20	 //Constante pi
//variables
uint32_t ADC0Value; //Variable para almacenar los datos leidos del adc
volatile double valor;
volatile int flat;

void cofigurar(void){
		uint32_t Periodo;
//		SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); // Reloj a 80 MHz
		SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//configura el reloj

	    // *** Habilitación de periféricos
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);//Habilitacion del periferico ADC0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//habilita el periferico del Timer 0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0); //Habilitacion del periferico SSI0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Habilitacion del periferico portA

		// *** ADC0
		ADCHardwareOversampleConfigure(ADC0_BASE,1);
		ADCReferenceSet(ADC0_BASE,ADC_REF_INT);
		ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_TIMER, 0);//. Configurar el muestreo del ADC disparado por el timer
		ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);//ADC_CTL_IE configura la interrupcion cuando el paso se complete,
		ADCSequenceEnable(ADC0_BASE, 1);// Especiifaca la secuencia de muestreo	//ADC_CTL_END define este como el ultimo paso //ADC_CTL_CHx x=0,1,2...23 puerto pin del ADC

		// *** Configuracion del modulo SSI
	    GPIOPinConfigure(GPIO_PA2_SSI0CLK); // CLK pin
	    GPIOPinConfigure(GPIO_PA5_SSI0TX);  // TX pin
	    GPIOPinConfigure(GPIO_PA3_SSI0FSS); // SS pin
	    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_3 ); // Configuracion de pines para moudulo SSI
	    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 20000000, 16);
	    SSIEnable(SSI0_BASE); // Habilitacion de SSI0

		// *** Timer0
		TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);// configura el modo de operacion del timer en periodo completo
		Periodo = (SysCtlClockGet())/Freq ; //SysCtlClockGet: determina la frecuencia del reloj del procesador
												//relación de 1s=1Hz
		TimerLoadSet(TIMER0_BASE, TIMER_A, Periodo); //carga al temporizador, el valor deseado
		TimerControlTrigger(TIMER0_BASE,TIMER_A,true);

		// *** Interrupciones
		IntEnable(INT_TIMER0A);// habilita la inrrupcion para el timer 0A
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//habilita la interrupción para el timer A timeout
		IntEnable(INT_ADC0SS1);
		ADCIntEnable(ADC0_BASE,1);

		IntMasterEnable();//Permite que el procesador responda a las interrupciones
		TimerEnable(TIMER0_BASE, TIMER_A);// Habilita el funcionamiento del timer A
}

int main(void) {
	cofigurar();
	double x[N]={0}; //Arreglo para lecturas x[3]=Actual, x[2]=Con un retrazo, x[1]=Con 2 retrazos, etc
	double y[N]={0}; //Arreglo para salidas y[3]=Actual, y[2]=Con un retrazo, y[1]=Con 2 retrazos, etc

	while(1) {

	    // ==============================================================
		//					CODIGO AQUI (°_°)/

		x[0]=(double) ADC0Value;

		y[0]=(x[0]*NUM[0]+x[1]*NUM[1]+x[2]*NUM[2]+x[3]*NUM[3]+x[4]*NUM[4]+x[5]*NUM[5]+x[6]*NUM[6]+x[7]*NUM[7]+x[8]*NUM[8]+x[9]*NUM[9]+x[10]*NUM[10]+x[11]*NUM[11]+x[12]*NUM[12]+x[13]*NUM[13]+x[14]*NUM[14]+x[15]*NUM[15]+x[16]*NUM[16]+x[17]*NUM[17]+x[18]*NUM[18]+x[19]*NUM[19]+x[20]*NUM[20]-(y[1]*DEN[1]+y[2]*DEN[2]+y[3]*DEN[3]+y[4]*DEN[4]+y[5]*DEN[5]+y[6]*DEN[6]+y[7]*DEN[7]+y[8]*DEN[8]+y[9]*DEN[9]+y[10]*DEN[10]+y[11]*DEN[11]+y[12]*DEN[12]+y[13]*DEN[13]+y[14]*DEN[14]+y[15]*DEN[15]+y[16]*DEN[16]+y[17]*DEN[17]+y[18]*DEN[18]+y[19]*DEN[19]+y[20]*DEN[20]))/DEN[0]; //funcion del filtro
		//y[0]=(x[0]*NUM[0]+x[1]*NUM[1]+x[2]*NUM[2]+x[3]*NUM[3]+x[4]*NUM[4]+x[5]*NUM[5]+x[6]*NUM[6]+x[7]*NUM[7]+x[8]*NUM[8]+x[9]*NUM[9]+x[10]*NUM[10]-(y[1]*DEN[1]+y[2]*DEN[2]+y[3]*DEN[3]+y[4]*DEN[4]+y[5]*DEN[5]+y[6]*DEN[6]+y[7]*DEN[7]+y[8]*DEN[8]+y[9]*DEN[9]+y[10]*DEN[10]))/DEN[0]; //funcion del filtro
		//y[0]=(x[0]*NUM[0]+x[1]*NUM[1]+x[2]*NUM[2]-(y[1]*DEN[1]+y[2]*DEN[2]))/DEN[0];

		uint32_t salida = (uint32_t)y[0];

		SSIDataPut(SSI0_BASE, (0x3000 | salida ) & 0x3FFF); //Envio de datos DAC
		while(SSIBusy(SSI0_BASE)){} //Espera termino de transmicion

		y[20]=y[19];y[19]=y[18];y[18]=y[17];y[17]=y[16];y[16]=y[15];y[15]=y[14];y[14]=y[13];y[13]=y[12];y[12]=y[11];y[11]=y[10];y[10]=y[9];y[9]=y[8];y[8]=y[7];y[7]=y[6];y[6]=y[5];y[5]=y[4];y[4]=y[3];y[3]=y[2];y[2]=y[1];y[1]=y[0];
		x[20]=x[19];x[19]=x[18];x[18]=x[17];x[17]=x[16];x[16]=x[15];x[15]=x[14];x[14]=x[13];x[13]=x[12];x[12]=x[11];x[11]=x[10];x[10]=x[9];x[9]=x[8];x[8]=x[7];x[7]=x[6];x[6]=x[5];x[5]=x[4];x[4]=x[3];x[3]=x[2];x[2]=x[1];x[1]=x[0];


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

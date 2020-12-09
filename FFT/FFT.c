// Ing. Electrónica 2018, Luis Rey y Diego 804-A
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/ssi.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include <stdio.h>

//Constantes
#define pi 3.141592
#define N 32
//#define num_div 8
const int Freq = 32000; // Frecuencia del timer

//variables
float e_r [N][N];
float e_i [N][N];


int rec=0;
float magnitud=0;
float fase=0;
uint32_t tiempo;
float absFFT[N];
float sabsFFT[N];
float lectura;

float x2[32]={1.7071,1.1564,0.5460,0.1090,0.0123,0.2929,0.8436,1.4540,1.8910,1.9877,1.7071,1.1564,0.5460,0.1090,0.0123,0.2929,0.8436,1.4540,1.8910,1.9877,1.7071,1.1564,0.5460,0.1090,0.0123,0.2929,0.8436,1.4540,1.8910,1.9877,1.7071,1.1564};
float x1[N]={0};
float y_r[N];
float y_i[N];
float syr[N];
float syi[N];
uint32_t ADC0Value; //Variable para almacenar los datos leidos del adc
int pausa;

void cofigurar(void){
		uint32_t Periodo;

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
		IntEnable(INT_TIMER0A);// habilita la inrrupcion para el timer 0A
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//habilita la interrupción para el timer A timeout
		IntEnable(INT_ADC0SS1);
		ADCIntEnable(ADC0_BASE,1);


		IntMasterEnable();//Permite que el procesador responda a las interrupciones
		TimerEnable(TIMER0_BASE, TIMER_A);// Habilita el funcionamiento del timer A
}

void serial_Config(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Habilitar modulo UART
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	//UARTDisable(UART0_BASE); // Deshabilitacion del modulo
}


void FT_cte(void){
	int k;
	int n;
	for (k=0; k<N ; k++){
	    for (n=0;n<N;n++){
		      e_r[k][n]= cosf((2*pi/N)*k*n);
			  e_i[k][n]= sinf((2*pi/N)*k*n)*(-1);
	    }
	}
}

void complejo(float x, float y){
	magnitud=sqrt(powf(x,2)+powf(y,2));
	fase= atan (y/x);
}

int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//configura el reloj
	cofigurar();
	serial_Config();
	int k,n;
	float x;

	//============================================FPU
	FPUEnable();
	FPULazyStackingEnable();
	//============================================Tick
	uint32_t ui32Value;
	SysTickPeriodSet(1);
	SysTickEnable();

	FT_cte();
	ui32Value = SysTickValueGet();

	float y_r[N/2];
	float y_i[N/2];
	int y1[2];

	float div=N/2;


	//============================================

	while(1)  {
		if(rec>=N){
			y1[0]=x1[1];
			y1[1]=x1[N/2+1];

			for(k=0;k<N/div;k++){
				y_r[k]=0;
				y_i[k]=0;
				absFFT[k]=0;
				for(n=0;n<N;n++){
					y_r[k]=y_r[k] +x1[y1[k]]*e_r[k][n];
					y_i[k]=y_i[k] +x1[y1[k]]*e_i[k][n];
				}
				x=powf(y_r[k],2)+powf(y_i[k],2);
				absFFT[k]=sqrt(x);
			}

			for(k=0;k<N/2;k++){
				sabsFFT[k]=absFFT[(N/2)+k];
				sabsFFT[k+(N/2)]=absFFT[k];
			}

			rec=0;
			UARTCharPut(UART0_BASE, 0x7E);
			for(n=0;n<N;n++){
				char *data=(char *)&sabsFFT[n];
				for(k=0; k<4*N; k++){
					UARTCharPut(UART0_BASE, data[k]);
				}
			}
		}
	}
}


void Timer0IntHandler(void) {//Secuencia de interrupción
	// Limpia la interrupción del timer
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void ADC0SS1IntHandler(void){//Secuencia de interrupción
	ADCIntClear(ADC0_BASE, 1);//limpia el registro de interrupcion
	if(rec<N){
		ADCSequenceDataGet(ADC0_BASE, 1, &ADC0Value);
		while(!ADCIntStatus(ADC0_BASE, 1, false)){} //devuelve el estado de la interrupcion que no se a procesado
		x1[rec]=(((float)ADC0Value)*3.3/4096)-1.65;//La resta es para bajar el offset de 1.65 v
		rec++;
	}
}





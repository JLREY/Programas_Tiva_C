#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include <math.h>

#define pi 3.141592	 //Constante pi

volatile float magnitud=0;
volatile float fase=0;

void complejo(float x, float y){
	magnitud=sqrt(pow(x,2)+pow(y,2));	//raiz del cuadrado de x mas el cuadrado de y
	fase= atan (y/x);	//tangente inversa de y/x
}

int main(void){
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	int x[16];
	int y=4;
	float a=3,b=2,c=1;

	while(1){
		int i;
		for(i=0;1<16;i++){
			x[i] = cos(pi*i)+sin(i*pi);
		}
		a = sqrt(pow(x[1],2)+pow(y,4));
		b = atan (y/x[6]);
		c=a+b;
		c=a*b;
		c=pow(a,b); //a potencia de b
		complejo(a,b);
	}

}

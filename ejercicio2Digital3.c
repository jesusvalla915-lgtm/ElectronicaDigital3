/*Realizar un programa que configure el puerto P0.0 y P2.0 para que provoquen una interrupción por flanco de subida.
 * Si la interrupción es por P0.0 guardar el valor binario 100111 en la variable "auxiliar", si es por P2.0 guardar el valor binario 111001011010110.
 *
 * */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void configurarPuertos(void);
void configurarInterrupciones(void);
void delay_ms(uint32_t milisegundos);
volatile uint16_t auxiliar;
int main(void) {
	configurarPuertos();
	configurarInterrupciones();
	while(1){

	}
    return 0 ;
}

void configurarPuertos(void){
	LPC_GPIO0->FIODIR &= ~(0x01); //Esto es Poner un cero en el bit 0 para definirlo como entrada
	LPC_GPIO2->FIODIR &= ~(0x01); //Esto es poner un cero en el bit 0 para definirlo como entrada 
}

void configurarInterrupciones(void){
	LPC_GPIOINT->IO0IntEnR |= 0x01;  // Flanco de subida en P0.0 (Bit 0)
	LPC_GPIOINT->IO2IntEnR |= 0x01;  // Flanco de subida en P2.0 (Bit 0)
	NVIC_EnableIRQ(EINT3_IRQn);      // Habilitar en el controlador maestro
}

void EINT3_IRQHandler(void){

	if(LPC_GPIOINT->IO0IntStatR & 0x01){
		auxiliar = 0b100111;		//Asigno el valor a la variable
		LPC_GPIOINT -> IO0IntClr = 0x01;	//Limpio la Bandera
	}
	if(LPC_GPIOINT ->IO2IntStatR & 0x01){
		auxiliar = 0b111001011010110;
		LPC_GPIOINT->IO2IntClr = 0x01;
	}


}

void delay_ms(uint32_t milisegundos){
	for(int i = 0; i < milisegundos; i++){
        SysTick->LOAD = 0x1869F;
        SysTick->VAL = 0;
        SysTick->CTRL = (1 << 0) | (1 << 2);
        while ((SysTick->CTRL & (1 << 16)) == 0);
        SysTick->CTRL = 0;
	}
}

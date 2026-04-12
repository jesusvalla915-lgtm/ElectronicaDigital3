

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void configurarPuertos(); // Configure los puertos de la placa para que sean salidas
void mostrarDisplay(); //Le pase a el p2 los valores del display
void delay_ms(uint32_t milisegundos); //retardo
uint8_t numDisplay[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

int main(void) {
	configurarPuertos();
	while(1){
		mostrarDisplay();
	}
    return 0 ;
}

void configurarPuertos(void){
	LPC_GPIO2->FIODIR |= 0xFF; //Configure los pines del GPIO2 como salida esto es equivalente a 00000000 00000000 00000000 11111111;
}

void mostrarDisplay(void){
	for(int i = 0;i<10;i++){
		LPC_GPIO2 -> FIOPIN = numDisplay[i];
		delay_ms(1000);
	}
};

void delay_ms(uint32_t milisegundos){
	for(int i=0;i<milisegundos;i++){
	// 1. Cargar el valor de recarga
	SysTick->LOAD = 0x1869F;

	// 2. Limpiar el contador actual (escribir cualquier valor lo reinicia a 0)
	SysTick->VAL = 0;

	// 3. Encender el SysTick:
	// Bit 0 = 1 (Enable) | Bit 2 = 1 (Usar reloj del CPU)
	SysTick->CTRL = (1 << 0) | (1 << 2);

	// 4. Quedarse atrapado aquí hasta que el bit 16 (COUNTFLAG) sea '1'
	while ((SysTick->CTRL & (1 << 16)) == 0);

	// 5. Apagar el SysTick
	SysTick->CTRL = 0;
	}
}

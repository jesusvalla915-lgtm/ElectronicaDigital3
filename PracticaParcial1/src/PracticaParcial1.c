#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void configurarPuertos(void);
void configurarInterrupciones(void);
void delay_ms(uint32_t milisegundos);

int main(void) {
	configurarPuertos();
	configurarInterrupciones();

    // El programa principal se queda aquí tranquilamente.
    // Solo actuará cuando el hardware detecte los flancos.
	while(1){

	}
    return 0 ;
}

void configurarPuertos(void){
	LPC_GPIO0->FIODIR |= 0x03;    // P0.0 y P0.1 como salidas
	LPC_GPIO0->FIOSET = 0x03;     // Los pines P0.0 y P0.1 quedan en estado alto por defecto
	LPC_GPIO2->FIODIR &= ~(0x03); // P2.0 y P2.1 como entradas
}

void configurarInterrupciones(void){
	LPC_GPIOINT->IO2IntEnR |= 0x01;  // Flanco de subida en P2.0 (Bit 0)
	LPC_GPIOINT->IO2IntEnF |= 0x02;  // Flanco de bajada en P2.1 (Bit 1)
	NVIC_EnableIRQ(EINT3_IRQn);      // Habilitar en el controlador maestro
}

void EINT3_IRQHandler(void){
	// --- CASO P2.0 (Flanco Subida) ---
    if (LPC_GPIOINT->IO2IntStatR & 0x01) {
        uint16_t seq1 = 0b010011010;

        for(int i = 8; i >= 0; i--) {
            if((seq1 >> i) & 0x01) {      // Siempre leemos con 0x01
                LPC_GPIO0->FIOSET = (1 << 0); // Sale por P0.0
            } else {
                LPC_GPIO0->FIOCLR = (1 << 0);
            }
            delay_ms(10);
        }

        LPC_GPIO0->FIOSET = (1 << 0);   // Volver a 1 (estado por defecto)
        LPC_GPIOINT->IO2IntClr = 0x01;  // ¡Limpiar bandera de P2.0!
    }

    // --- CASO P2.1 (Flanco Bajada) ---
    if (LPC_GPIOINT->IO2IntStatF & 0x02) {  // Revisamos la bandera del P2.1 (0x02)
        uint16_t seq2 = 0b011100110;

        for(int i = 8; i >= 0; i--){
        	if((seq2 >> i) & 0x01){       // La máscara de lectura sigue siendo 0x01
        		LPC_GPIO0->FIOSET = (1 << 1); // Sale por P0.1
        	} else {
        		LPC_GPIO0->FIOCLR = (1 << 1);
        	}
        	delay_ms(10);
        }

        LPC_GPIO0->FIOSET = (1 << 1);   // Volver a 1 (estado por defecto para P0.1)
        LPC_GPIOINT->IO2IntClr = 0x02;  // ¡Limpiar bandera de P2.1!
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

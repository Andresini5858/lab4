/*
 * File:   main.c
 * Author: Andrés Lemus
 * Laboratorio 4
 * Created on October 8, 2022, 10:05 AM
 */
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <math.h> //libreria para utilizar exponentes
#define _XTAL_FREQ 4000000

int bandera=0;
int bandera1=0;
void setup(void);
void contador(void);
void contadors(void);
void contadorr(void);
void antirrebote(void);
void antirrebote1(void);

void main(void){
    setup();
    while(1){   
    }
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0; //puerto A como salida
    PORTA = 0;
    TRISBbits.TRISB7 = 1; //puerto b7 como entrada
    TRISBbits.TRISB6 = 1; //puerto b7 como entrada
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    
    OPTION_REGbits.nRBPU = 0;
    INTCONbits.GIE = 1; //Activar interrupciones globales
    INTCONbits.RBIE = 1; //Activar interrupciones puertob
    WPUBbits.WPUB7 = 1; // Activar pullup puerto b7
    WPUBbits.WPUB6 = 1; // Activar pullup puerto b6
    IOCBbits.IOCB7 = 1; //activar interrupt on-change b7
    IOCBbits.IOCB6 = 1; //activar interrupt on-change b6
    
    OSCCONbits.IRCF0 = 1; //Oscilador a 4MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1; //Oscialdor interno
}

void __interrupt() isr(void){
    if (INTCONbits.RBIF == 1){ //revisar bandera de interrupcion
        INTCONbits.RBIF = 0; //limpiar bandera 
        contador();
       
    }
}

void contador(void){
    if (PORTBbits.RB7 == 0){
        antirrebote();}
    if (PORTBbits.RB7 == 1) {  
        contadors();
    }
    if (PORTBbits.RB6 == 0){
        antirrebote1();}
    if (PORTBbits.RB6 == 1) {  
        contadorr();
    }
}



void contadors(void){
    if (bandera == 1){
        PORTD++;
        bandera = 0;
    }
}

void contadorr(void){
    if (bandera1 == 1){
        PORTD--;
        bandera1 = 0;
    }
}

void antirrebote(void){
    bandera = 1;
}

void antirrebote1(void){
    bandera1 = 1;
}
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

int bandera=0; // variable para el antirrebote del botón de incrementar
int bandera1=0; // variable para el antirrebote del botón de decrementar
int dis=0; // variable para guardar el valor del ADRESH
unsigned char nums[] = { //arreglo para valores del display de 7 segementos
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5 
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01100111, //9
    0b01110111, //A
    0b01111100, //B
    0b00111001, //C
    0b01011110, //D
    0b01111001, //E
    0b01110001, //F
};

unsigned int H1=0; // variable para guardar las "unidades" del valor hexadecimal
unsigned int H2=0; // variable para guardar las "decenas" del valor hexadecimal

void setup(void); //función de configuración
void setupADC(void); //función de configuración del ADC
void contador(void); //función del contador
void displays(void); //función de multiplexeo de displays y alarma
void ADC(void); //función de interrupción del ADC

void main(void){
    setup(); //llamar al setup
    setupADC(); //llamar la configutacion del ADC 
    while(1){ //loop principal
        ADCON0bits.GO = 1; //iniciar conversión ADC
        displays(); //función de displays      
    }
}

void setup(void){
    ANSEL = 0; // puertos digitales
    ANSELH = 0b00100000; //puertos digitales excepto el puerto ANS12 o RC0
    TRISA = 0; //puerto A como salida
    PORTA = 0; // limpiar puerto A
    TRISBbits.TRISB7 = 1; //puerto b7 como entrada
    TRISBbits.TRISB6 = 1; //puerto b6 como entrada
    TRISBbits.TRISB0 = 1; //puerto b0 como entrada
    PORTB = 0; // limpiar puerto B
    TRISC = 0; // puerto C como salida
    PORTC = 0; // limpiar puerto C
    TRISD = 0; // puerto D como salida
    PORTD = 0; // limpiar puerto D
    
    OPTION_REGbits.nRBPU = 0;
    INTCONbits.GIE = 1; //Activar interrupciones globales
    INTCONbits.PEIE = 1; //Activar interrupciones periféricas
    INTCONbits.RBIE = 1; //Activar interrupciones puerto B
    PIE1bits.ADIE = 1; // Habiliar interrupcion del conversor ADC
    WPUBbits.WPUB7 = 1; // Activar pullup puerto b7
    WPUBbits.WPUB6 = 1; // Activar pullup puerto b6
    IOCBbits.IOCB7 = 1; //activar interrupt on-change b7
    IOCBbits.IOCB6 = 1; //activar interrupt on-change b6
    
    OSCCONbits.IRCF0 = 0; //Oscilador a 4MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1; //Oscialdor interno
}

void setupADC(void){
    ADCON0bits.ADCS1 = 0; // Fosc/ 8        
    ADCON0bits.ADCS0 = 1; // =======      
    
    ADCON1bits.VCFG1 = 0; // Referencia VSS (0 Volts)
    ADCON1bits.VCFG0 = 0; // Referencia VDD (3.3 Volts)
    
    ADCON1bits.ADFM = 0;  // Justificado hacia izquierda
    
    ADCON0bits.CHS3 = 1; // Canal AN12
    ADCON0bits.CHS2 = 1;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        
    
    ADCON0bits.ADON = 1; // Habilitamos el ADC
    __delay_us(100); //delay de 100 us
}

void ADC(void){
        PIR1bits.ADIF = 0; //limpiar bandera del conversor ADC
        dis = ADRESH; //guardar variables del ADRESH 
        __delay_ms(10); // delay de 10 ms
}

void displays(void){ // alarma (POST-LAB)
    if (dis >= PORTD){ //revisar si el valor del ADRESH es mayor al Puerto D
        PORTCbits.RC2 = 1; //encender alarma
    }
    else { //revisar si el valor del ADRESH es mayor al Puerto D
        PORTCbits.RC2 = 0;    } // apagar alarma
    
    H1 = (dis%16); //convertir el valor de ADRESH a hexadecimal
    H2 = (dis/16); //convertir el valor de ADRESH a hexadecimal
    
    PORTA = nums[H1]; //mostrar unidades en el puerto A (display)
    PORTCbits.RC0 = 1; //multiplexeo
    PORTCbits.RC1 = 0;
    
    __delay_ms(5); //delay de 5 ms
    
    PORTA = nums[H2]; //mostrar decenas en el puerto A (display)
    PORTCbits.RC0 = 0; //multiplexeo
    PORTCbits.RC1 = 1;
    
    __delay_ms(5); //delay de 5 ms
}    

void __interrupt() isr(void){ //interrupciones
    if (INTCONbits.RBIF == 1){ //revisar bandera de interrupcion del puerto B
        contador(); // llamar al contador
        INTCONbits.RBIF = 0; //limpiar bandera  
    }
    if (PIR1bits.ADIF == 1){ //revisar bandera de interrupcion del conversor ADC
        ADC(); //llamar a la funcion ADC
    }
}

void contador(void){ 
    if (PORTBbits.RB6 == 0){ //revisar si se presiono el botón de incrementar
        bandera = 1;} //activar bandera
    if (PORTBbits.RB6 == 1 && bandera == 1){ //revisar si se dejo de presionar el botón y la bandera está en 1
        __delay_ms(10);
        PORTD++; //incrementar
        bandera = 0; // limpiar bandera
    }
    if (PORTBbits.RB7 == 0){ //revisar si se presiono el botón de decrementar
        bandera1 = 1;} //activar bandera
    if (PORTBbits.RB7 == 1 && bandera1 == 1){ //revisar si se dejo de presionar el botón y la bandera está en 1
        __delay_ms(10);
        PORTD--; //decrementar el puerto
        bandera1 = 0; // limpiar bandera
    }
}

// Dispositivo:    PIC 16F887
// Hadward:        potenciometro, LCD
// Autor:          Dylan Ixcayau 18486
// Fecha Creada:   22 de julio del 2021
// Ultima Actualizacion:    

// ----------------- Laboratorio No.2 ------------------------------------------
//Configuración PIC16F887
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#pragma config WRT = OFF        // Flash Progra
//****DirectivasCompliador*****//
#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f887.h>
#include "ADC.h"      //Importar libreria creada para ADC
#include "LCD.h"
#include <string.h>

//-----------------------------declaracion_de_variables-------------------------
uint8_t contador = 0;       
int pot1 = 0;
int pot2 = 0;
int val3 = 0;
int banderas = 0;
float potenciometro1 = 0;
float potenciometro2 = 0;
char s[20];


//-----------------------------funciones----------------------------------------
void setup(void);           
void Envio(char bt);
void cadena(char st[]);
void sensores(void);

//-----------------------------Interrupciones-----------------------------------

void __interrupt()isr(void)
{
    recibir_valoresADC();   //Funcion de libreria_ADC
}

//--------------------------Funcion_principal-----------------------------------
void main(void) {
    setup();               //llamamos configuraciones
    __delay_us(50);         
    ADCON0bits.GO = 1;     //Bandera de GO levantada
    unsigned int a;         
    Lcd_Init();            //Funcion de libreria LCD
    while(1){
        potenciometro1 = (pot1*0.0196);   //Conversion a voltaje de 0 a 5
        potenciometro2 = (pot2*0.0196);
        val3 = contador;                        //Contador
        
        sprintf(s, "%.1f", potenciometro1);     //String para LCD en decimal
        Lcd_Set_Cursor(1,2);                    //Posicion del cursor
        Lcd_Write_String("S1");                 //String
        Lcd_Set_Cursor(1,6);
        Lcd_Write_String("S2");
        Lcd_Set_Cursor(1,11);
        Lcd_Write_String("S3");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(s);

        sprintf(s, "%.1f", potenciometro2);     //String para LCD en decimal
        Lcd_Set_Cursor(2,6);
        Lcd_Write_String(s);
        
        sprintf(s, "%d", val3);                 //String para LCD
        Lcd_Set_Cursor(2,12);
        Lcd_Write_String(s);
        __delay_ms(80);

        sensores();                             //se llama a la funcion sensores
        inicio_conversionADC();                 //funcion de ibreria ADC 
        
        if(PIR1bits.RCIF){                      //Bandera de UART
            char escritura = RCREG;             //variable para escribir
            if(escritura == 43){                //si la variable es igual a + en ASCCI
                contador++;                     //Aumenta la varibale contador
            }
            if(escritura == 45){                //si la variable es igual a - en ASCCI
                contador--;                     //Decrementa la variable contador     
            }
        }
    }
}
//-------------------------Configuraciones-------------------------------------
void setup(void)
{ 
    ANSEL = 0b00000011;
    ANSELH = 0x00;     //Salidas Digitales
    TRISE = 0x00; 
    TRISA = 0x03;
    TRISB = 0x00;
    TRISD = 0x00;
    
    PORTA = 0x00; 
    PORTE = 0x00;
    PORTB = 0x00;
    PORTD = 0x00;
//----------------------------configuracion_oscilador---------------------------
    OSCCONbits. IRCF2 = 1; 
    OSCCONbits. IRCF1 = 1;
    OSCCONbits. IRCF0 = 0;  //4Mhz
    OSCCONbits. SCS = 1;

//----------------------------Configuracion_ADC---------------------------------
    
    ADCON1bits.ADFM = 0;        //justificado a la izquierda 
    ADCON1bits.VCFG0 = 0;       //Voltage de referencia VDD
    ADCON1bits.VCFG1 = 0;       //Voltage de referencia VSS
    ADCON0bits.ADCS = 1;        //Fosc/8
    ADCON0bits.CHS = 0;         //Empezamos con el canal 0
    __delay_us(100);
    ADCON0bits.ADON = 1;        //ADC encendido
    
//------------------------------------configuracion_UART------------------------
    
    TXSTAbits.SYNC = 0;             //Modo asíncrono
    TXSTAbits.BRGH = 1;             //BAUD RATE
    BAUDCTLbits.BRG16 = 0; 
    SPBRG = 25;                     //Registros para BAUD RATE
    SPBRGH = 0; 
    RCSTAbits.SPEN = 1;         //Habilitar puerto asíncrono
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;         //Habilitar recepción de datos 
    TXSTAbits.TXEN = 1;         //Habilitar transmision
    
 //-----------------------------------Configuracion_de_interrupciones-----------
    INTCONbits. GIE = 1;        //Globales
    INTCONbits. RBIE = 1;       //IOCB
    INTCONbits. RBIF = 0;
    INTCONbits.PEIE = 1;        //Perifericas
    PIE1bits.ADIE = 1;          //ADC
    PIR1bits.ADIF = 0;      
}

//---------------------------------Envio_de_caracter----------------------------
void Envio(char caracter)
{
    while(!TXIF);               //Espera que envie dato TXIF = 1 constantemente
    TXREG = caracter;           //Carga el caracter a TXREG y envía 
    return; 
}

//-------------------------------funcion_de_cadena------------------------------
void cadena(char st[])
{
    int i = 0; 
    while (st[i] !=0)       //revisar la posicion de valor de i 
    {
        Envio(st[i]);       //enviar caracter de esa posicion 
        i++;                //incrementar variable para pasar a otra posicion 
    }                       //de cadena 
    return;
}

//--------------------------Funcion_String_para_UART----------------------------
void sensores(void)
{
    sprintf(s, "\r S1=%d \n", pot1);   //String_de_terminal
    cadena(s);
    
    sprintf(s, "\n S2=%d", pot2);      
    cadena(s);
    
    sprintf(s, "\n S3=%d", contador);
    cadena(s);
}

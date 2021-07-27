
//Author: Dylan Ixcayau 18486
#include "LCD.h"

void Lcd_Cmd(char a){       //se cargan los comandos a la lcd para inicializar
	PORTBbits.RB7 =0;       //0 para poder escribir valores a la lcd 
	PORTD = a;          // cargamos lo que se desea escribir a la lcd 
    PORTBbits.RB6 = 1;     //1 la lcd para que lea los valores cargados              
    __delay_ms(4);      
    PORTBbits.RB6 = 0;    //regresamos a 0 para leer mas               
}


void Lcd_Clear(void){  //se limpia la lcd utilizando los comandos de la misma 
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) //funcion el cursor de la lcd 
{
	char temp,z,y; //variables char 
	if(a == 1)
	{
	  temp = 0x80 + b - 1; //posicion del cursor 
		//z = temp>>8;
		//y = temp & 0x0F;
		Lcd_Cmd(temp);    //leemos la posicion del cursor 
		
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1; //posicion del cursor 
		//z = temp>>8;
		//y = temp & 0x0F;
		Lcd_Cmd(temp); //leemos la posicion del cursor 
		
	}
}

void Lcd_Write_Char(char a){    
    PORTBbits.RB7 = 1; //1 en la lcd para que pueda leer lo que se mande           
    PORTD = a;         //mandamos el valor a la lcd        
    PORTBbits.RB6 = 1;   //leemos el valor char                    
    __delay_us(40);              
    PORTBbits.RB6 = 0;    //esperamos que otro valor ingrese a la lcd                  
}

void Lcd_Write_String(char *a)
{
	int i;// inicialiamos la variable i
	for(i=0;a[i]!='\0';i++) //Leemos el string 
	    Lcd_Write_Char(a[i]); //mandamos el char a la lcd 
}

void Lcd_Init(){           
    
    PORTBbits.RB7 = 0;   //LCD en 0 para poder leer los comandos                
       
    Lcd_Cmd(0x00);        // 0b00000000      
    __delay_ms(15);         
    
   
    Lcd_Cmd(0x30);       // 0b00110000        
    __delay_us(160);        
    
    Lcd_Cmd(0x38);      // 0b00111000  
    Lcd_Cmd(0x01);     // 0b00000001         
    Lcd_Cmd(0x06);     // 0b00000110         
    Lcd_Cmd(0x0F);      // 0b00001111         
   
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}

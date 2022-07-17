#Include <18F4550.h>
#Device ADC=10
#Use Delay(CLOCK=20M)
#Fuses HS, NOWDT, NOPROTECT, PUT, BROWNOUT, NOLVP, NOMCLR
#USE Standard_IO(A)
#USE Standard_IO(B)
#USE Standard_IO(C)
#USE Standard_IO(D)

// Pines Pantalla LCD
#Define LCD_DB4 PIN_B4
#Define LCD_DB5 PIN_B5
#Define LCD_DB6 PIN_B6
#Define LCD_DB7 PIN_B7
#Define LCD_RS PIN_B2
#Define LCD_E PIN_B3
#Include <LCD_16X2.c> // Libreria LCD

Float s1,s2,s3,s4; // Variables de Sensores

Void Main (){
   LCD_Init();
   Setup_ADC_Ports(AN0_TO_AN1);
   Setup_ADC(ADC_Clock_Internal);

   While(1){
      Set_ADC_channel(0);
      Delay_us(10);
      s1 = Read_ADC();
      
      Set_ADC_channel(1);
      Delay_us(10);
      s2 = Read_ADC();
      
      LCD_Clear();
      LCD_Gotoxy(1,1);
      Printf(LCD_Putc,"ADC: %f",s1);
      LCD_Gotoxy(1,2);
      Printf(LCD_Putc,"ADC: %f",s2);
      Delay_ms(100);  
   }
}
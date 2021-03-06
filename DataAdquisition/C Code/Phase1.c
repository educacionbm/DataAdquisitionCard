#Include <18F4550.h>
#Device ADC=10
#Use Delay(CLOCK=20M)
#Fuses HS, NOWDT, NOPROTECT, PUT, BROWNOUT, NOLVP, NOMCLR
#USE Standard_IO(A)
#USE Standard_IO(B)
#USE Standard_IO(C)
#USE Standard_IO(D)

#Include <LCD420.c> // Libreria LCD

Float s1,s2,s3,s4; // Variables de Sensores

Void Main (){
   LCD_Init();
   Setup_ADC_Ports(AN0_TO_AN3);
   Setup_ADC(ADC_Clock_Internal);

   While(1){
      Set_ADC_channel(0);
      Delay_us(10);
      s1 = Read_ADC()*0.55;
      
      Set_ADC_channel(1);
      Delay_us(10);
      s2 = Read_ADC()*0.44;
      
      Set_ADC_channel(2)*0.33;
      Delay_us(10);
      s3 = Read_ADC();
      
      Set_ADC_channel(3)*0.22;
      Delay_us(10);
      s4 = Read_ADC();
      
      LCD_Putc("\f");
      LCD_Gotoxy(1,1);
      Printf(LCD_Putc,"ADC: %f",s1);
      LCD_Gotoxy(1,2);
      Printf(LCD_Putc,"ADC: %f",s2);
       LCD_Gotoxy(1,3);
      Printf(LCD_Putc,"ADC: %f",s3);
      LCD_Gotoxy(1,4);
      Printf(LCD_Putc,"ADC: %f",s4);
      Delay_ms(100);  
   }
}

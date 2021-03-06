#Include <18F4550.h>
#Device ADC=10
#Use Delay(CLOCK=20M)
#Fuses HS, NOWDT, NOPROTECT, PUT, BROWNOUT, NOLVP, NOMCLR
#Fuses HSPLL, NODEBUG, USBDIV, PLL5, CPUDIV1, VREGEN // Fuses para la Com USB

#USE Standard_IO(A)
#USE Standard_IO(B)
#USE Standard_IO(C)
#USE Standard_IO(D)

#Define USB_HID_Device TRUE
#Define USB_EP1_TX_ENABLE USB_ENABLE_INTERRUPT
#Define USB_EP1_TX_SIZE 8
#Define USB_EP1_RX_ENABLE USB_ENABLE_INTERRUPT
#Define USB_EP1_RX_SIZE 8

#Include <PIC18_USB.h>
#Include <MY_USB_DESC_HID.h>
#Include <usb.c>

#Define no_usb PIN_D0 //LED Espera de conexion 
#Define si_usb PIN_D1 //LED USB Conectado

#Include <LCD420.c> // Libreria LCD

Float s1,s2,s3,s4; // Variables de Sensores
int8 dato_tx[8];

Void Main (){
   LCD_Init();
   Setup_ADC_Ports(AN0_TO_AN3);
   Setup_ADC(ADC_Clock_Internal);
   Output_High(no_usb);
   Output_Low(si_usb);
   USB_Init();
   USB_Task();
   USB_Wait_For_Enumeration();
   Output_High(si_usb);
   Output_Low(no_usb);
   
   While(1){
     USB_Task(); 
      If(USB_Enumerated())
      {
         Set_ADC_channel(0);
         Delay_us(10);
         s1 = Read_ADC()*0.55;
      
         Set_ADC_channel(1);
         Delay_us(10);
         int16 s2 = Read_ADC()*0.44;
         
         dato_tx[0] = make8(s2,0); 
         dato_tx[1] = make8(s2,1);
         USB_Put_Packet(1,dato_tx,2,USB_DTS_TOGGLE);
         
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
         //Printf(LCD_Putc,"ADC: %f",s2);
          LCD_Gotoxy(1,3);
         Printf(LCD_Putc,"ADC: %f",s3);
         LCD_Gotoxy(1,4);
         Printf(LCD_Putc,"ADC: %f",s4);
         Delay_ms(100);  
      }
   }
}

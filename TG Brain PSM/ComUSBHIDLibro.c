///////////////////////////////////////////////////////////////////////////////
/*-------------------------------Fusibles------------------------------------*/

#INCLUDE <18F4550.H> //Indica el uC que se va usar
#DEVICE ADC=8 //activando el convertidor ana-dig
#USE DELAY(CLOCK=20M) // Frecuencia de trabajo
#FUSES HS  // Velocidad del reloj   
#FUSES NOWDT // Desactivar el perro guardian
#FUSES NOPROTECT //  No protege c?digo
#FUSES PUT //  Espera de un tiempo de inicio
#FUSES NOBROWNOUT // No reseteo por chequeo de la frecuencia ni fuente
#FUSES NOLVP // No voltaje bajo de programaci?n 
#FUSES NOMCLR // Reset Digital
#FUSES HSPLL
#FUSES NODEBUG
#FUSES USBDIV
#FUSES PLL5
#FUSES CPUDIV1
#FUSES VREGEN

/*--------------------------------Comunicacion USB---------------------------*/

#define USB_HID_DEVICE TRUE //Tipo de comunicacion USB HID verdadera.
//Cambia a on EP1 para la transferencia de entrada IN bulk/interrupt.
#define USB_EP1_TX_ENABLE USB_ENABLE_INTERRUPT
#define USB_EP1_TX_SIZE 8
//Cambia a on EP1 para la transferencia de salida OUT bulk/interrupt.
#define USB_EP1_RX_ENABLE USB_ENABLE_INTERRUPT
#define USB_EP1_RX_SIZE 8
#include <pic18_usb.h> //Funciones de bajo nivel (hardware) que serviran en usb.c
#include <my_usb_desc_hid.h> //Descriptores del dispositivo.
#include <usb.c> //Libreran para el manejo del USB.

///////////////////////////////////////////////////////////////////////////////
/*---------Definiendo puerto, renombrando pines y creando variables----------*/

#Byte Port_A=0xF80
#Byte port_B=0xF81 //Identificador del Puerto B.
#Byte port_C=0xF82
#Byte port_D=0xF83

#define LED1 PIN_D0 //LED1para la espera de la conexión USB
#define LED2 PIN_D1 //LED2, se enciende cuando el USB está conectado.

int8 Salida[8]; //Variable de salida para envio de la informacion entrada analogica.
int8 Entrada[8]; //Variable para recibir el estado de los interruptores en LABVIEW.
LONG BITS; // variable almacena bits

///////////////////////////////////////////////////////////////////////////////

void main()
{
        set_tris_b(0x00); //Define el Puerto B como salida.
        output_b(0x00); //Define el Puerto B en 0
        setup_adc(ADC_CLOCK_INTERNAL); //Fija el Clock interno para el ADC.
        set_adc_channel(0); //Fija el canal 0 para el ADC.
        output_high(LED1); //Activa LED1 indicando que se inicia la conexion.
        output_low(LED2); //LED2 continua apagado.
        usb_init(); //Inicializa el USB.
        usb_task(); //Monitorea el estado de la conexion conectandose y desconectandose automticamente.
        usb_wait_for_enumeration(); //Espera infinitamente hasta que el dispositivo sea.enumerado.
        output_high(LED2); //Apaga LED2 de espera.
        output_low(LED1); //Indica que el dispositivo se enumera y se establece la comunicacion.
        
        while (TRUE)
        {
                setup_adc_ports(AN0);
                DELAY_MS(1);
                BITS=READ_ADC();
                usb_task(); //Monitorea el estado de la conexion conectandose y desconectandose automaticamente.
                if (usb_enumerated())
                { //Si el dispositivo esta enumerado
                        Salida[0]=BITS; //Lee el valor del ADC.
                        //Envia al bus USB el paquete de datos.
                        usb_put_packet(1, Salida, 1, USB_DTS_TOGGLE);
                        if (usb_kbhit(1))
                        { //Si hay datos en el buffer del USB….?
                                usb_get_packet(1, Entrada, 1); //Recibe el dato la variable entrada.
                                port_b= Entrada[0]; //Asigna al Puerto B el byte de Entrada.
                        }
                }
        } //Fin del lazo infinito.
} //Fin del main.

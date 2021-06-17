int RELAY = 2;                                  //Definimos que el relay se encuentra en el pin digital 2

#include <SPI.h>                              //Este es el protocolo para la transferencia de datos entre los periféricos (SPI)
#include <MFRC522.h>                         //Dicha librería es propia de nuestro módulo RFID-RC522 (modelo MFRC522)

//LCD config
#include <Wire.h>                         //La utilizamos para comunicar la placa con serial data (SDA) y reloj serie (SCL)
#include <LiquidCrystal_I2C.h>           //Esta es la librería para usar la pantalla LCD con el módulo i2c
LiquidCrystal_I2C lcd(0x27,20,4);       //La dirección de nuestra pantalla

#define RST_PIN  9                    //Pin 9 para resetar nuestra tarjeta RFID-RC522 
#define SS_PIN  10                   //Pin 10 para el intercambio de datos seriales (SDA)
MFRC522 mfrc522(SS_PIN, RST_PIN);   //Crea a la tarjeta RFID-RC522 como un nuevo objeto

//Variables boleanas inicializadas
boolean first_read=false;  
boolean normal_mode=true;


void setup() {
  SPI.begin();                                                //Inicializa el bus de datos SPI
  mfrc522.PCD_Init();                                        //inicializa la tarjeta MFRC522  
  pinMode(RELAY, OUTPUT);                                   //Fija el RELAY como una salida
  digitalWrite(RELAY, HIGH);                               //Inicializa el RELAY en alto, para que nuestra chapa este abierta
  Serial.begin(9600);                                     //Inicializa la comunicación serial 
  
  //Config de la pantalla LCD
  lcd.init();                                          //Inicialización
  lcd.backlight();                                    //Trabajaremos con la luz encendida 
  lcd.setCursor(0,0);                                //Nos ubica en el primer fila y primer columna
  lcd.print("Identificate");                        //Así podeos desplegar cualquier texto que se encuentre entre las comillas (de 16 dígitos por fila)
}

byte ActualUID[4];                               //Aquí se guarda la dirección cada que lee una nueva

byte USER1[4]= {0xD5, 0xE0, 0x53, 0x63} ;      //Aquí se introduce la dirección en hexadecimal de nuestro primer TAG (usuario 1)
byte USER2[4]= {0x19, 0x32, 0xE1, 0x2A} ;     //Se repite el paso para el segundo usuario
byte USER3[4]= {0xB1, 0xBA, 0x0C, 0xFD} ;    //No confundir con la dirección en decimal
//byte USER4[4]= {0x00, 0x00, 0x00, 0x00} ; //Dirección vacía en caso de requerir de otro usuario 


void loop() {

  if(normal_mode)
 {
    
    if ( mfrc522.PICC_IsNewCardPresent())                                              //Verifica si hay una nueva dirección (TAG) frente a la tarjeta
          {  

              if ( mfrc522.PICC_ReadCardSerial())                                   //Selecciona el TAG encontrado
              {
               
                    for (byte i = 0; i < mfrc522.uid.size; i++) {                //Almacenamos la dirección del TAG (4 bytes) con un ciclo for
                      ActualUID[i]=mfrc522.uid.uidByte[i];          
                    } 

                                                                             //Aqui es donde se ejecutan las acciones según el resultado de la comparación                       
                            if(!first_read)
                            {                                              //de los TAGS registrados, con el TAG encontrado

//A continuación veremos las acciones a realizar si la dirección del TAG encontrado coincide con la dirección del TAG del usuario 1
                              
                                    if(compareArray(ActualUID,USER1))                 //Aquí se hace la comparación
                                    {
                                        lcd.setCursor(0,0);                         //Impresión en pantalla
                                        lcd.print("Estimado jefe,");        
                                        lcd.setCursor(0,1);
                                        lcd.print("gusto en verle");
                                        delay(1000);
                                        digitalWrite(RELAY, LOW);              //Se manda el relay a 0 (al ser lógica invertida le llegan 12V)
                                        delay(2000);                          //Se mantiene así por 2 segundos (se retrae la chapa)
                                        digitalWrite(RELAY,HIGH);            //Vuelve a 1 (alto) donde la chapa vuelve a cerrarse 
                                        lcd.setCursor(0,0);
                                        lcd.print("                ");     //Limpiamos pantalla 
                                        lcd.setCursor(0,1);
                                        lcd.print("                ");   //Limpiamos pantalla
                                    }


//El siguiente caso es para el usuario 2, donde se repite lo mismo con una ligera variación con lo desplegado en pantalla solo para variar
                                    
                                    else if(compareArray(ActualUID,USER2))
                                    {
                                        lcd.setCursor(0,0);
                                        lcd.print("Otro dia mas de ");
                                        lcd.setCursor(0,1);
                                        lcd.print("servir a tu jefe");
                                        delay(1000);
                                        digitalWrite(RELAY, LOW);
                                        delay(2000);
                                        digitalWrite(RELAY,HIGH);
                                        lcd.setCursor(0,0);
                                        lcd.print("                ");
                                        lcd.setCursor(0,1);
                                        lcd.print("                ");
                                    }


//Se repite el proceso para el usuario 3
                                    
                                    else if(compareArray(ActualUID,USER3))
                                    {
                                        lcd.setCursor(0,0);
                                        lcd.print("Bienvenida  seas");
                                        lcd.setCursor(0,1);
                                        lcd.print("Martha Nora");
                                        delay(1000);
                                        digitalWrite(RELAY, LOW);
                                        delay(2000);
                                        digitalWrite(RELAY,HIGH);
                                        lcd.setCursor(0,0);
                                        lcd.print("                ");
                                        lcd.setCursor(0,1);
                                        lcd.print("                ");
                                        
                                    }

//De no encontrar una coincidencia con los TAGS registrados, despliega un mensaje, no abre la chapa y limpia pantalla
                                                                
                                    else
                                    {
                                        lcd.setCursor(0,0);
                                        lcd.print("Acceso  Denegado");
                                        delay(2000);  
                                        lcd.setCursor(0,0);
                                        lcd.print("                ");
                                        lcd.setCursor(0,1);
                                        lcd.print("                ");
                                      }
                               }
                                                    
                            mfrc522.PICC_HaltA(); // Terminamos la lectura de la tarjeta actual
                       }
               }
       }
} 


//Compara los 4 bytes de los usuarios registrados y el TAG encontrado
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

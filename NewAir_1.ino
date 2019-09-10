/*-------------------------------------------------------------------
 Standalone Sketch to use with a Arduino uno and a
 Sharp Optical Dust Sensor GP2Y1010AU0F
---------------------------------------------------------------------
// Conexión del GP2Y1010 y Shield LCD.
// GP2Y1010:  Pin1     -> A 5V a través de resistencia de 220 ohmios
//            Pin2     -> Arduino GND
//            Pin3     -> Arduino Digital 2
//            Pin4     -> Arduino GND
//            Pin5     -> Arduino Análogo 1
//            Pin6     -> Arduino 5V
//          
// LCD:     DB7         -> Arduino Digital 7
//          DB6         -> Arduino Digital 6
//          DB5         -> Arduino Digital 5
//          DB4         -> Arduino Digital 4
//          E           -> Arduino Digital 9
//          RS          -> Arduino Digital 8
-------------------------------------------------------------------*/
#include <LiquidCrystal.h>  //Librería para manipular LCD
#include "SoftwareSerial.h"

SoftwareSerial ceropollution(2,3); //Rx, Tx

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);    //Crea objeto para controlar la LCD

int measurePin = A1;        //Pin de lectura del sensor
int ledPower = 2;           //Pin de activación del LED del sensor
 
int samplingTime = 280;     //Tiempo de encendido del LED debe ser 320 micro segundos (280 + 40)
int iPolucion = 0;

float voMeasured = 0;       //Variable para guardar valor leído en el sensor (0-1023)
float calcVoltage = 0;      //Variable para guardar valor de voltaje calculado  en el sensor (0-5V)
float dustDensity = 0;      //Variable para guardar valor de densidad de polvo leído en el sensor (0-0,8 mg/m3)

//-------------------------------------------------------------------
void setup(){
  Serial.begin(9600);         //Inicializa comunicación serial
  ceropollution.begin(9600);
  pinMode(ledPower,OUTPUT);   //Configura pin como salida para controlar LED del sensor

  lcd.begin(16, 2);           // Inicializar el LCD
  lcd.setCursor(4,0);
  lcd.print("Colegio");       // Mensaje de inicio
  lcd.setCursor(2,1);
  lcd.print("Las Americas "); 
  delay(5000);
  lcd.clear();                //Limpia la pantalla
  lcd.setCursor(0,0);
  lcd.print("Contaminacion de");   
  lcd.setCursor(0,1);
  lcd.print("Aire en Kennedy");     // Mensaje de inicio
  delay(5000);
  lcd.clear();                      
  lcd.home();                       //Se posiciona en la coordenada (0,0)             
  lcd.print("Cantidad deGas: ");
  lcd.setCursor(5, 1);
  lcd.print(" ppm");              //Imprime unidades de medida de densidad del polvo en el aire
}

//-------------------------------------------------------------------
void loop(){
  digitalWrite(ledPower,LOW);           //Enciende LED del sensor
 
  voMeasured = analogRead(measurePin);  //Lee valor de densidad del polvo
 
  digitalWrite(ledPower,HIGH);          //Apaga LED del sensor
  delay(100);         //Completa tiempo de ciclo
 
  iPolucion = map(voMeasured, 0, 1023, 0, 100);   //Convierte valor decimal leido a un valor de voltaje
  
  //Ecuación de linealidad tomada de http://www.howmuchsnow.com/arduino/airquality/ realizada por Chris Nafis (c) 2012

  lcd.setCursor(0, 1);                  //Se posiciona para imprimir valor de densidad de polvo
  lcd.print(iPolucion);               //Imprime valor de densidad de polvo
  ceropollution.print(iPolucion);
  lcd.setCursor(15, 1);                 //Se traslada a última posición
  lcd.blink();                          //Resalta cursor...
  delay (489);                          //durante medio segundo
  
  /*Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  Serial.print(" - Dust Density: ");
  Serial.println(dustDensity);*/
  
  lcd.noBlink();                        //Quita resaltado del cursor...
  delay(500);                           //durante medio segundo
}

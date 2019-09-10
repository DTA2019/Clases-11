#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"
SoftwareSerial paleta(9, 12); //Rx, Tx

//DFPLAYER
//Pin 1 conectado a 5V
//Pin 2 RX conectado a través de resistencia de 1K al pin digital 11 del Arduino
//Pin 3 TX conectado al pin digital 10 del Arduino
//Pin 6 conectado al parlante
//Pin 7 conectado a GND
//Pin 8 conectado al parlante

SoftwareSerial mySoftwareSerial(10, 11); //Crea instancia de comunicación serial por software
DFRobotDFPlayerMini myDFPlayer; //Crea instancia para reproductor MP3

void setup() {
  //configura pines digitales 2 al 5 como entradas (SENSORES TOUCH CAPACITIVOS):
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  //configura pines digitales 6 al 8 como entradas (SENSORES INFRAR-ROJO):
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  
   
  //Configura pines de salida para LEDs
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  
  paleta.begin(9600);// put your setup code here, to run once:

  mySoftwareSerial.begin(9600);         //Inicia comunicación serial a 9600 Bytes/s
  myDFPlayer.begin(mySoftwareSerial);   //Usa mySoftwareSerial para comunicarse con el DFPlayer
  myDFPlayer.volume(28);                //Volumen entre 0-30-

  Serial.begin(9600);
}

void loop() {
  digitalWrite(13, HIGH);
  /*Serial.print(digitalRead(2));
  Serial.print(digitalRead(3));
  Serial.print(digitalRead(4));
  Serial.print(digitalRead(5));
  Serial.println("");*/
  delay(100);
  digitalWrite(13, LOW);
  
  if(digitalRead(2)==HIGH){
    digitalWrite(A0, HIGH);
    myDFPlayer.playMp3Folder(1);  //reproduce mp3 ubicado en SD:/MP3/0001.mp3 ...alerta
    delay(18000);                  //durante el tiempo determinado
    myDFPlayer.pause();
    paleta.println("Me duele algo");
    digitalWrite(A0, LOW);
  }
  
  if(digitalRead(3)==HIGH){
    digitalWrite(A1, HIGH);
    myDFPlayer.playMp3Folder(2);  //reproduce mp3 ubicado en SD:/MP3/0002.mp3 ...jugar
    delay(17000);                  //durante el tiempo determinado
    myDFPlayer.pause();
    paleta.println("quiero jugar");
    digitalWrite(A1, LOW);
  }
  
  if(digitalRead(4)==HIGH){
    digitalWrite(A2, HIGH);
    myDFPlayer.playMp3Folder(3);  //reproduce mp3 ubicado en SD:/MP3/0003.mp3 ...Comer
    delay(20000);                  //durante el tiempo determinado
    myDFPlayer.pause();
    paleta.println("quiero comer");
    digitalWrite(A2, LOW);
  }

  if(digitalRead(5)==HIGH){
    digitalWrite(A3, HIGH);
    myDFPlayer.playMp3Folder(4);  //reproduce mp3 ubicado en SD:/MP3/0004.mp3 ...baño
    delay(11000);                  //durante el tiempo determinado
    myDFPlayer.pause();
    paleta.println("quiero ir al baño");
    digitalWrite(A3, LOW);
  }
}

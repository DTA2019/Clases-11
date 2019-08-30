//Adaptado de: https://www.luisllamas.es/arduino-motor-dc-tb6612fng/

const int pinPWMA = 6;
const int pinAIN2 = 7;
const int pinAIN1 = 8;
const int pinBIN1 = 9;
const int pinBIN2 = 10;
const int pinPWMB = 11;
const int pinSTBY = 12;
 
const int waitTime = 2000;    //espera
const int speed = 200;        //velocidad de giro

void setup()
{
   pinMode(pinAIN2, OUTPUT);
   pinMode(pinAIN1, OUTPUT);
   pinMode(pinPWMA, OUTPUT);
   pinMode(pinBIN1, OUTPUT);
   pinMode(pinBIN2, OUTPUT);
   pinMode(pinPWMB, OUTPUT);
}
 
void loop()
{
//Motores adelante
   digitalWrite(pinSTBY, HIGH); //Habilita motores
   digitalWrite(pinAIN1, HIGH);
   digitalWrite(pinAIN2, LOW);
   analogWrite(pinPWMA, speed);
   digitalWrite(pinBIN1, HIGH);
   digitalWrite(pinBIN2, LOW);
   analogWrite(pinPWMB, speed);
   delay(2000);
   
//Detiene motores
   digitalWrite(pinSTBY, LOW); //Deshabilita motores
   digitalWrite(pinAIN1, LOW);
   digitalWrite(pinAIN2, LOW);
   analogWrite(pinPWMA, 0);
   digitalWrite(pinBIN1, LOW);
   digitalWrite(pinBIN2, LOW);
   analogWrite(pinPWMB, 0);
   delay(1000);
   
//Motores atrás
   digitalWrite(pinSTBY, HIGH); //Habilita motores
   digitalWrite(pinAIN1, LOW);
   digitalWrite(pinAIN2, HIGH);
   analogWrite(pinPWMA, speed);
   digitalWrite(pinBIN1, LOW);
   digitalWrite(pinBIN2, HIGH);
   analogWrite(pinPWMB, speed);
   delay(2000);

//Detiene motores
   digitalWrite(pinSTBY, LOW); //Deshabilita motores
   digitalWrite(pinAIN1, LOW);
   digitalWrite(pinAIN2, LOW);
   analogWrite(pinPWMA, 0);
   digitalWrite(pinBIN1, LOW);
   digitalWrite(pinBIN2, LOW);
   analogWrite(pinPWMB, 0);
   delay(1000);
   
//Giro en sentido horario
   digitalWrite(pinSTBY, HIGH); //Habilita motores
   digitalWrite(pinAIN1, HIGH);
   digitalWrite(pinAIN2, LOW);
   analogWrite(pinPWMA, speed);
   digitalWrite(pinBIN1, LOW);
   digitalWrite(pinBIN2, HIGH);
   analogWrite(pinPWMB, speed);
   delay(2000);
   
//Detiene motores
   digitalWrite(pinSTBY, LOW); //Deshabilita motores
   digitalWrite(pinAIN1, LOW);
   digitalWrite(pinAIN2, LOW);
   analogWrite(pinPWMA, 0);
   digitalWrite(pinBIN1, LOW);
   digitalWrite(pinBIN2, LOW);
   analogWrite(pinPWMB, 0);
   delay(1000);

//Giro en sentido antihorario
   digitalWrite(pinSTBY, HIGH); //Habilita motores
   digitalWrite(pinAIN1, LOW);
   digitalWrite(pinAIN2, HIGH);
   analogWrite(pinPWMA, speed);
   digitalWrite(pinBIN1, HIGH);
   digitalWrite(pinBIN2, LOW);
   analogWrite(pinPWMB, speed);
   delay(2000);
   
//Detiene motores
   digitalWrite(pinSTBY, LOW); //Deshabilita motores
   digitalWrite(pinAIN1, LOW);
   digitalWrite(pinAIN2, LOW);
   analogWrite(pinPWMA, 0);
   digitalWrite(pinBIN1, LOW);
   digitalWrite(pinBIN2, LOW);
   analogWrite(pinPWMB, 0);
   delay(3000);
}
 

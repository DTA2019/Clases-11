//Adaptado de: https://courses.ideate.cmu.edu/16-223/f2016/text/ex/Arduino/DRV8833-motor-driver/DRV8833-motor-driver.html
const int VelMotorDer=100;
const int VelMotorIzq=100;

void setup() {
  pinMode(5, OUTPUT);  //
  pinMode(6, OUTPUT);  //
  pinMode(9, OUTPUT);  //
  pinMode(10, OUTPUT);  //
}

void loop() {
//Motores adelante
    digitalWrite(5,LOW); //
    analogWrite(6, VelMotorIzq);
    digitalWrite(9,LOW);
    analogWrite(10, VelMotorDer);
    delay(2000);
    
//Detiene motores
    digitalWrite(5,LOW); //
    digitalWrite(6,LOW); //
    digitalWrite(9,LOW);
    digitalWrite(10,LOW); //
    delay(1000);

//Motores atrás
    digitalWrite(6,LOW); //
    analogWrite(5, VelMotorIzq);
    digitalWrite(10,LOW);
    analogWrite(9, VelMotorDer);
    delay(2000);
    
//Detiene motores
    digitalWrite(5,LOW); //
    digitalWrite(6,LOW); //
    digitalWrite(9,LOW);
    digitalWrite(10,LOW); //
    delay(1000);

//Gira motores en sentido Horario
    digitalWrite(5,LOW); //
    analogWrite(6, VelMotorIzq);
    digitalWrite(10,LOW);
    analogWrite(9, VelMotorDer);
    delay(2000);
    
//Detiene motores
    digitalWrite(5,LOW); //
    digitalWrite(6,LOW); //
    digitalWrite(9,LOW);
    digitalWrite(10,LOW); //
    delay(1000);

//Gira motores en sentido Antihorario
    digitalWrite(6,LOW); //
    analogWrite(5, VelMotorIzq);
    digitalWrite(9,LOW);
    analogWrite(10, VelMotorDer);
    delay(2000);
    
//Detiene motores
    digitalWrite(5,LOW); //
    digitalWrite(6,LOW); //
    digitalWrite(9,LOW);
    digitalWrite(10,LOW); //
    delay(3000);
     
}

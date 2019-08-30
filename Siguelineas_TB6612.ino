/********************************************************
  Programa para Robot Seguidor de Línea Negra.
  Detecta línea negra o fondo blanco con tres sensores
  CNY70, cuando detecta un obstáculo (sensor por ultra-
  sonidos HC-SR04), se detiene y activa mecanismo de 
  respuesta (servo-motor MG-90S).
  Elaborado por: Ing. Yesid Rodríguez Lozano
  *******************************************************
 
  *******************************************************
         CONEXIÓN
  *******************************************************
  ARDUINO         TB6612             
  GND     -->     GND
  5V      -->     VCC, VM
  Pin D6  -->     PWMA
  Pin D7  -->     AIN2
  Pin D8  -->     AIN1
  Pin D9  -->     BIN1
  Pin D10 -->     BIN2
  Pin D11 -->     PWMB
  Pin D12 -->     STBY
                  A1    -->   MotorDerecho
                  A2    -->   MotorDerecho
                  B1    -->   MotorIzquierdo
                  B2    -->   MotorIzquierdo
  *******************************************************
  ARDUINO         Sensores CNY70
  GND     -->     Pin -
  5V      -->     Pin +
  Pin A0  -->     Pin 2
  Pin A1  -->     Pin 3
  Pin A2  -->     Pin 4
  *******************************************************
  ARDUINO         Sensor HC-SR04
  GND     -->     GND
  5V      -->     VCC
  Pin D2  -->     Trig
  Pin D4  -->     Echo
  *******************************************************
  ARDUINO         Servo-Motor MG90S
  GND     -->     Cable Café
  5V      -->     Cable Rojo
  Pin D3  -->     Cable Amarillo
  ********************************************************/

//*******************************************************
//        LIBRERÍAS
//*******************************************************
  #include <EEPROM.h>   //Librería para administrar datos de la memoria EEPROM
  #include <Servo.h>    //Librería para manipular servo-motores
//*******************************************************

//*******************************************************
//        VARIABLES GLOBALES
//*******************************************************
  Servo ServoPimPon;    //Crea objeto de tipo Servo, llamado ServoPimPon
  int Lectura_IR_A0, Lectura_IR_A1, Lectura_IR_A2;
  byte Umbral_IR_A0, Umbral_IR_A1, Umbral_IR_A2;
  boolean Final = LOW;
//*******************************************************

//*******************************************************
//        CONSTANTES
//*******************************************************
  const int VelMotorDer = 100;    //Permiten variar la velocidad general de los motores,...
  const int VelMotorIzq = 100;    //para compensar pequeñas variaciones de fabricación.
  const int pinPWMA = 6;
  const int pinAIN2 = 7;
  const int pinAIN1 = 8;
  const int pinBIN1 = 9;
  const int pinBIN2 = 10;
  const int pinPWMB = 11;
  const int pinSTBY = 12;
//*******************************************************

//*******************************************************
//*******************************************************
void setup() {
  //Configura pines de salida
  pinMode(13, OUTPUT);  //LED                                     
  pinMode(pinAIN2, OUTPUT); pinMode(pinAIN1, OUTPUT); pinMode(pinPWMA, OUTPUT); pinMode(pinBIN1, OUTPUT); pinMode(pinBIN2, OUTPUT); pinMode(pinPWMB, OUTPUT);  //Motores
  pinMode(2, OUTPUT); //Sensor HC-SR04
  //Configura pines de entrada 
  pinMode(A0, INPUT); pinMode(A1, INPUT); pinMode(A2, INPUT); //Sensores IR CNY-70 
  pinMode(4, INPUT);  //Sensor HC-SR04
  
  Serial.begin(9600); //Inicializa comunicación por el puerto USB
  analogWrite(3, 80); //Coloca servo-motor en posición de 10º
  //CalibraSensoresIR();
  Umbral_IR_A0 = EEPROM.read(0x00); Umbral_IR_A1 = EEPROM.read(0x01); Umbral_IR_A2 = EEPROM.read(0x02);   //Obtiene umbrales de sensores IR, guardados en memoria EEPROM
  //ImprimirLecturas(Umbral_IR_A0, Umbral_IR_A1, Umbral_IR_A2);
  
  ParpadeaLED(2, 15);
}

//*******************************************************
//*******************************************************
void loop() {
  int iDistancia = LeerSensorDistancia(5);  
  
  if (iDistancia == 3 && Final == LOW){
    delay(20);
    iDistancia = LeerSensorDistancia(5);
    if(iDistancia == 3){
      digitalWrite(13, HIGH);
      DetenerMotores(100);
      ServoPimPon.attach(3);    //Define el pin D3 para controlar el servo-motor
      for(int i = 10; i < 100; i++){
        ServoPimPon.write(i);    //Coloca el eje del servo-motor en la posición: 10º
        delay(20);
      }
      ServoPimPon.detach();
      DetenerMotores(100);
      MotoresAtras();
      delay(100);
      DetenerMotores(100);
      Final = HIGH;
    }
  }

  else if (Final == LOW) {
    LeerSensoresIR(10);
    //ImprimirLecturas(Lectura_IR_A0, Lectura_IR_A1, Lectura_IR_A2);
  
    if(Lectura_IR_A0 < Umbral_IR_A0 && Lectura_IR_A1 > Umbral_IR_A1 && Lectura_IR_A2 < Umbral_IR_A2){
      DetenerMotores(50);
      MotoresAdelante();
      Serial.println("Robot Adelante");        //Envía el resultado por el Puerto USB 
      delay(100);   
    }

    else if(Lectura_IR_A0 > Umbral_IR_A0 && Lectura_IR_A1 < Umbral_IR_A1 && Lectura_IR_A2 < Umbral_IR_A2){
      DetenerMotores(50);
      MotoresGiroCCW();
      Serial.println("Robot gira CCW");
      delay(100);    
    }

    else if(Lectura_IR_A0 < Umbral_IR_A0 && Lectura_IR_A1 < Umbral_IR_A1 && Lectura_IR_A2 > Umbral_IR_A2){
      DetenerMotores(50);
      MotoresGiroCW();
      Serial.println("Robot gira CW");
      delay(100);
    }

    else if(Lectura_IR_A0 < Umbral_IR_A0 && Lectura_IR_A1 < Umbral_IR_A1 && Lectura_IR_A2 < Umbral_IR_A2){
      DetenerMotores(50);
      MotoresAtras();
      Serial.println("Robot atrás");
      delay(100);
    }

    else if(Lectura_IR_A0 > Umbral_IR_A0 && Lectura_IR_A1 > Umbral_IR_A1 && Lectura_IR_A2 > Umbral_IR_A2){
      DetenerMotores(50);
      MotoresAtras();
      Serial.println("Robot atrás");
      delay(100);
    }

    else{
      Serial.println("Robot igual");
      delay(100);
    }
  }
  
  //ParpadeaLED(2, 50);
}

//*******************************************************
//*******************************************************

//*******************************************************
//        FUNCIONES
//*******************************************************
void ParpadeaLED(int iRepeticiones, int iRetardo){
  for(int i = 1; i <= iRepeticiones; i++){
    digitalWrite(13,HIGH);
    delay(iRetardo);
    digitalWrite(13,LOW);
    delay(iRetardo);
  }
}

//*******************************************************
void ImprimirLecturas(int Lectura1, int Lectura2, int Lectura3){
  Serial.println("Lecturas obtenidas para A0, A1 y A2 = ");        //Envía el resultado por el Puerto USB:
  Serial.print(Lectura1);
  Serial.print("\t");
  Serial.print(Lectura2);
  Serial.print("\t");
  Serial.println(Lectura3);
}

//*******************************************************
void LeerSensoresIR(int iNumeroLecturas){
  Lectura_IR_A0 = 0;
  Lectura_IR_A1 = 0;
  Lectura_IR_A2 = 0;
  for(int i = 0; i < iNumeroLecturas; i++){
    Lectura_IR_A0 = Lectura_IR_A0 + map((analogRead(A0)), 0, 1023, 0, 100);
    Lectura_IR_A1 = Lectura_IR_A1 + map((analogRead(A1)), 0, 1023, 0, 100);
    Lectura_IR_A2 = Lectura_IR_A2 + map((analogRead(A2)), 0, 1023, 0, 100);
    delay(1);
  }
  Lectura_IR_A0 = Lectura_IR_A0 / iNumeroLecturas;
  Lectura_IR_A1 = Lectura_IR_A1 / iNumeroLecturas;
  Lectura_IR_A2 = Lectura_IR_A2 / iNumeroLecturas;
}

//*******************************************************
int LeerSensorDistancia(int iNumeroLecturas){
  int t = 0;  //Tiempo en microsegundos
  int d = 0;  //Distancia en centimetros
  for(int i = 0; i < iNumeroLecturas; i++){
    digitalWrite(2, HIGH);
    delayMicroseconds(10);  //Envia un pulso de 10us por el pin conectado el Trigger (D2)
    digitalWrite(2, LOW); 
    t = pulseIn(4, HIGH,1200);  //Obtiene el ancho del pulso del pin conectado al Echo (D4)...
    d = d + (t / 59);  //convierte el tiempo a una distancia en cm (d=v*t/2) y acumula el valor
    delayMicroseconds(2000);
    Serial.println("Lecturas del sensor de distancia:"); Serial.print(t); Serial.print("  "); Serial.println(d); //Envía el resultado por el Puerto USB:
  }
  d = d / iNumeroLecturas;  //Obtiene la distancia promedio a partir de todas las lecturas
  Serial.println("Lecturas del sensor de distancia:"); Serial.print(t); Serial.print("  "); Serial.println(d); //Envía el resultado por el Puerto USB:
  delay(300); 
  return (d); //Al salir de la función, devuelve la distancia en cm
}

//*******************************************************
void MotoresAdelante(){
  digitalWrite(pinSTBY, HIGH); //Habilita motores
  digitalWrite(pinAIN1, HIGH);
  digitalWrite(pinAIN2, LOW);
  analogWrite(pinPWMA, VelMotorDer);
  digitalWrite(pinBIN1, HIGH);
  digitalWrite(pinBIN2, LOW);
  analogWrite(pinPWMB, VelMotorIzq);
}

//*******************************************************
void MotoresAtras(){
  digitalWrite(pinSTBY, HIGH); //Habilita motores
  digitalWrite(pinAIN1, LOW);
  digitalWrite(pinAIN2, HIGH);
  analogWrite(pinPWMA, VelMotorDer);
  digitalWrite(pinBIN1, LOW);
  digitalWrite(pinBIN2, HIGH);
  analogWrite(pinPWMB, VelMotorIzq);
}

//*******************************************************
void MotoresGiroCW(){
  digitalWrite(pinSTBY, HIGH); //Habilita motores
  digitalWrite(pinAIN1, HIGH);
  digitalWrite(pinAIN2, LOW);
  analogWrite(pinPWMA, VelMotorDer);
  digitalWrite(pinBIN1, LOW);
  digitalWrite(pinBIN2, LOW);
  analogWrite(pinPWMB, 0);
}

//*******************************************************
void MotoresGiroCCW(){
  digitalWrite(pinSTBY, HIGH); //Habilita motores
  digitalWrite(pinAIN1, LOW);
  digitalWrite(pinAIN2, LOW);
  analogWrite(pinPWMA, 0);
  digitalWrite(pinBIN1, HIGH);
  digitalWrite(pinBIN2, LOW);
  analogWrite(pinPWMB, VelMotorIzq);
}

//*******************************************************
void DetenerMotores(int TiempoDetenidos){
  digitalWrite(pinSTBY, LOW); //Deshabilita motores
  digitalWrite(pinAIN1, LOW);
  digitalWrite(pinAIN2, LOW);
  analogWrite(pinPWMA, 0);
  digitalWrite(pinBIN1, LOW);
  digitalWrite(pinBIN2, LOW);
  analogWrite(pinPWMB, 0);
  delay(TiempoDetenidos);
}

//*******************************************************
void CalibraSensoresIR(){
  int miSensorNeg[10][3];
  int miSensorBln[10][3];
  
  Serial.println("Lectura Sensores CNY70 en A0, A1 y A2 para el Blanco = ");        //Envía el resultado por el puerto USB:
  for(int ii=0; ii<10; ii++){
    miSensorBln[ii][0] = map((analogRead(A0)), 0, 1023, 0, 100);
    miSensorBln[ii][1] = map((analogRead(A1)), 0, 1023, 0, 100);
    miSensorBln[ii][2] = map((analogRead(A2)), 0, 1023, 0, 100);
    ParpadeaLED(5, 50);
    Serial.print(miSensorBln[ii][0]);
    Serial.print("\t");
    Serial.print(miSensorBln[ii][1]);
    Serial.print("\t");
    Serial.println(miSensorBln[ii][2]);
  }

  ParpadeaLED(1, 1000);
  
  Serial.println("Lectura Sensores CNY70 en A0, A1 y A2 para el Negro = ");        //Envía el resultado por el Puerto USB:
  for(int ii=0; ii<10; ii++){
    miSensorNeg[ii][0] = map((analogRead(A0)), 0, 1023, 0, 100);
    miSensorNeg[ii][1] = map((analogRead(A1)), 0, 1023, 0, 100);
    miSensorNeg[ii][2] = map((analogRead(A2)), 0, 1023, 0, 100);
    ParpadeaLED(3, 100);
    Serial.print(miSensorNeg[ii][0]);
    Serial.print("\t");
    Serial.print(miSensorNeg[ii][1]);
    Serial.print("\t");
    Serial.println(miSensorNeg[ii][2]);
  }

  byte UmbralSensorA0 = 0, UmbralSensorA1 = 0, UmbralSensorA2 = 0;
  int PromedioBlnA0 = 0, PromedioBlnA1 = 0, PromedioBlnA2 = 0;
  int PromedioNegA0 = 0, PromedioNegA1 = 0, PromedioNegA2 = 0;
  
  for(int ii=0; ii<10; ii++){
    PromedioBlnA0 = PromedioBlnA0 + miSensorBln[ii][0];
    PromedioBlnA1 = PromedioBlnA1 + miSensorBln[ii][1];
    PromedioBlnA2 = PromedioBlnA2 + miSensorBln[ii][2];

    PromedioNegA0 = PromedioNegA0 + miSensorNeg[ii][0];
    PromedioNegA1 = PromedioNegA1 + miSensorNeg[ii][1];
    PromedioNegA2 = PromedioNegA2 + miSensorNeg[ii][2];
  }
  
  PromedioBlnA0 = PromedioBlnA0 / 10;
  PromedioBlnA1 = PromedioBlnA1 / 10;
  PromedioBlnA2 = PromedioBlnA2 / 10;
  PromedioNegA0 = PromedioNegA0 / 10;
  PromedioNegA1 = PromedioNegA1 / 10;
  PromedioNegA2 = PromedioNegA2 / 10;  

  UmbralSensorA0 = (PromedioBlnA0 + PromedioNegA0) / 2;
  UmbralSensorA1 = (PromedioBlnA1 + PromedioNegA1) / 2;
  UmbralSensorA2 = (PromedioBlnA2 + PromedioNegA2) / 2;

  Serial.println("Promedio Sensores A0, A1 y A2 para el Blanco = ");        //Envía el resultado por el Puerto USB:
  Serial.print(PromedioBlnA0);
  Serial.print("\t");
  Serial.print(PromedioBlnA1);
  Serial.print("\t");
  Serial.println(PromedioBlnA2);

  Serial.println("Promedio Sensores A0, A1 y A2 para el Negro = ");        //Envía el resultado por el Puerto USB:
  Serial.print(PromedioNegA0);
  Serial.print("\t");
  Serial.print(PromedioNegA1);
  Serial.print("\t");
  Serial.println(PromedioNegA2);

  Serial.println("Umbrales Sensores A0, A1 y A2 = ");        //Envía el resultado por el Puerto USB:
  Serial.print(UmbralSensorA0);
  Serial.print("\t");
  Serial.print(UmbralSensorA1);
  Serial.print("\t");
  Serial.println(UmbralSensorA2);

  EEPROM.write(0x00, UmbralSensorA0);
  EEPROM.write(0x01, UmbralSensorA1);
  EEPROM.write(0x02, UmbralSensorA2);
}


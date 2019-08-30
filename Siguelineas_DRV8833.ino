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
  ARDUINO         DRV8833             
  GND     -->     GND
  5V      -->     VCC
  Pin D5  -->     IN1
  Pin D6  -->     IN2
  Pin D9  -->     IN3
  Pin D10 -->     IN4
                  OUT1    -->   MotorDerecho
                  OUT2    -->   MotorDerecho
                  OUT3    -->   MotorIzquierdo
                  OUT4    -->   MotorIzquierdo
                  EEP     -->   NC
                  ULT     -->   NC
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
//*******************************************************

//*******************************************************
//        VARIABLES GLOBALES
//*******************************************************
  int Lectura_IR_A0, Lectura_IR_A1, Lectura_IR_A2;
  byte Umbral_IR_A0, Umbral_IR_A1, Umbral_IR_A2;
  boolean Final = LOW;
//*******************************************************

//*******************************************************
//        CONSTANTES
//*******************************************************
  const int VelMotorDer = 200;    //Permiten variar la potencia general de los motores,...
  const int VelMotorIzq = 200; //para compensar pequeñas variaciones de fabricación.
  //Los siguientes valores definen la velocidad de desplazamiento del robot
  const int VelRobot = 15
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  ;         //Valor del tiempo que los motores están encendidos en cada ciclo
  const int RetardoRobot = 10;    //Valor del tiempo que los motores están apagados en cada ciclo
//*******************************************************

//*******************************************************
//*******************************************************
void setup() {
  //Configura pines de salida
  pinMode(13, OUTPUT);  //LED                                     
  pinMode(5, OUTPUT); pinMode(6, OUTPUT); pinMode(9, OUTPUT); pinMode(10, OUTPUT);  //Motores
  pinMode(2, OUTPUT); //Sensor HC-SR04
  pinMode(3, OUTPUT); //Salida PWM para controlar Servo-Motor
  //Configura pines de entrada 
  pinMode(A0, INPUT); pinMode(A1, INPUT); pinMode(A2, INPUT); //Sensores IR CNY-70 
  pinMode(A6, INPUT); //Interruptor para calibrar sensores IR
  pinMode(4, INPUT);  //Sensor HC-SR04
  
  Serial.begin(9600); //Inicializa comunicación por el puerto USB
  analogWrite(3, 90); //Coloca servo-motor en posición de 90º
  if (analogRead (A6) > 100){
    CalibraSensoresIR();  //Colocar sensores IR sobre el fondo blanco mientras el LED titila rápido, despues, sobre la línea negra mientras el LED titila lento
    delay(1000);  //tiempo antes de que se activen los motores
  }
  Umbral_IR_A0 = EEPROM.read(0x00); Umbral_IR_A1 = EEPROM.read(0x01); Umbral_IR_A2 = EEPROM.read(0x02);   //Obtiene umbrales de sensores IR, guardados en memoria EEPROM
  ImprimirLecturas(Umbral_IR_A0, Umbral_IR_A1, Umbral_IR_A2);
  
  ParpadeaLED(2, 15); //Parpadea LED del pin 13, dos veces con intervalo de 15 ms
}

//*******************************************************
//*******************************************************
void loop() {
  int iDistancia = LeerSensorDistancia(1);  
  
  if (iDistancia == 3 && Final == LOW){
    DetenerMotores(100);
    iDistancia = LeerSensorDistancia(10);
    if(iDistancia == 3){
      digitalWrite(13, HIGH);
      DetenerMotores(100);
      for(int i = 90; i <= 230; i++){
        analogWrite(3, i);    //Mueve el eje del servo-motor de la posición: 90º a la posición 180º
        delay(15);
      }
      delay(2000);
      for(int i = 230; i >= 90; i--){
        analogWrite(3, i);    //Mueve el eje del servo-motor de la posición: 180º a la posición 90º
        delay(15);
      }
      delay(500);
      DetenerMotores(100);
      MotoresAtras();
      delay(200);
      DetenerMotores(100);
      Final = HIGH;
    }
  }

  else if (Final == LOW) {
    LeerSensoresIR(10);
    ImprimirLecturas(Lectura_IR_A0, Lectura_IR_A1, Lectura_IR_A2);

    //Si solamente el sensor del centro está sobre la línea negra, el robot avanza
    if(Lectura_IR_A0 < Umbral_IR_A0 && Lectura_IR_A1 > Umbral_IR_A1 && Lectura_IR_A2 < Umbral_IR_A2){
      DetenerMotores(RetardoRobot); //Retardo para disminuir velocidad del robot en este movimiento        
      MotoresAdelante();
      //Serial.println("Robot Adelante");        //Envía el resultado por el Puerto USB 
      delay(VelRobot);   
    }

    //Si solamente el sensor de la derecha está sobre la línea negra, gira en sentido contrario a las manecillas del reloj
    else if(Lectura_IR_A0 > Umbral_IR_A0 && Lectura_IR_A1 < Umbral_IR_A1 && Lectura_IR_A2 < Umbral_IR_A2){
      DetenerMotores(RetardoRobot * 2);  //Retardo para disminuir velocidad del robot en este movimiento
      MotoresGiroCCW();
      //Serial.println("Robot gira CCW");
      delay(VelRobot / 2);    
    }

    //Si el sensor de la derecha y el del centro, están sobre la línea negra, gira en sentido contrario a las manecillas del reloj
    else if(Lectura_IR_A0 > Umbral_IR_A0 && Lectura_IR_A1 > Umbral_IR_A1 && Lectura_IR_A2 < Umbral_IR_A2){
      DetenerMotores(RetardoRobot * 2);  //Retardo para disminuir velocidad del robot en este movimiento
      MotoresGiroCCW();
      //Serial.println("Robot gira CCW");
      delay(VelRobot / 2);    
    }

    //Si solamente el sensor de la izquierda está sobre la línea negra, gira en el sentido de las manecillas del reloj
    else if(Lectura_IR_A0 < Umbral_IR_A0 && Lectura_IR_A1 < Umbral_IR_A1 && Lectura_IR_A2 > Umbral_IR_A2){
      DetenerMotores(RetardoRobot * 2); //Retardo para disminuir velocidad del robot en este movimiento
      MotoresGiroCW();
      //Serial.println("Robot gira CW");
      delay(VelRobot / 2);
    }

    //Si el sensor de la izquierda y el del centro, están sobre la línea negra, gira en el sentido de las manecillas del reloj
    else if(Lectura_IR_A0 < Umbral_IR_A0 && Lectura_IR_A1 > Umbral_IR_A1 && Lectura_IR_A2 > Umbral_IR_A2){
      DetenerMotores(RetardoRobot * 2); //Retardo para disminuir velocidad del robot en este movimiento
      MotoresGiroCW();
      //Serial.println("Robot gira CW");
      delay(VelRobot / 2);
    }

    //Si todos los sensores están sobre la línea blanca, el robot retrocede
    else if(Lectura_IR_A0 < Umbral_IR_A0 && Lectura_IR_A1 < Umbral_IR_A1 && Lectura_IR_A2 < Umbral_IR_A2){
      DetenerMotores(RetardoRobot * 2); //Retardo para disminuir velocidad del robot en este movimiento
      MotoresAtras();
      //Serial.println("Robot atrás");
      delay(VelRobot / 2);
    }

    //Si todos los sensores están sobre la línea negra, el robot retrocede
    else if(Lectura_IR_A0 > Umbral_IR_A0 && Lectura_IR_A1 > Umbral_IR_A1 && Lectura_IR_A2 > Umbral_IR_A2){
      DetenerMotores(RetardoRobot * 2); //Retardo para disminuir velocidad del robot en este movimiento
      MotoresAtras();
      //Serial.println("Robot atrás");
      delay(VelRobot / 2);
    }

    //Si solamente el sensor del centro está sobre fondo blanco, el robot avanza
    if(Lectura_IR_A0 > Umbral_IR_A0 && Lectura_IR_A1 < Umbral_IR_A1 && Lectura_IR_A2 > Umbral_IR_A2){
      DetenerMotores(RetardoRobot); //Retardo para disminuir velocidad del robot en este movimiento        
      MotoresAdelante();
      //Serial.println("Robot Adelante");        //Envía el resultado por el Puerto USB 
      delay(VelRobot);   
    }

    else{
      //Serial.println("Robot igual");
      delay(VelRobot);
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
    delayMicroseconds(50);  //Envia un pulso de 10us por el pin conectado el Trigger (D2)
    digitalWrite(2, LOW); 
    t = pulseIn(4, HIGH,1200);  //Obtiene el ancho del pulso del pin conectado al Echo (D4)...
    d = d + (t / 59);  //convierte el tiempo a una distancia en cm (d=v*t/2) y acumula el valor
    delay(10);
    //Serial.println("Lecturas del sensor de distancia:"); Serial.print(t); Serial.print("  "); Serial.println(d); //Envía el resultado por el Puerto USB:
  }
  d = d / iNumeroLecturas;  //Obtiene la distancia promedio a partir de todas las lecturas
  //Serial.println("Promedio del sensor de distancia:"); Serial.print(t); Serial.print("  "); Serial.println(d); //Envía el resultado por el Puerto USB:
  return (d); //Al salir de la función, devuelve la distancia en cm
}

//*******************************************************
void MotoresAdelante(){
  digitalWrite(5,LOW); //
  analogWrite(6, 100);
  digitalWrite(9,LOW);
  analogWrite(10, 100);
}

//*******************************************************
void MotoresAtras(){
  digitalWrite(6,LOW); //
  analogWrite(5, VelMotorIzq);
  digitalWrite(10,LOW);
  analogWrite(9, VelMotorDer);
}

//*******************************************************
void MotoresGiroCW(){
  digitalWrite(5,LOW); //
  analogWrite(6, VelMotorIzq);
  digitalWrite(10,LOW);
  analogWrite(9, LOW);
}

//*******************************************************
void MotoresGiroCCW(){
  digitalWrite(6,LOW); //
  analogWrite(5, LOW);
  digitalWrite(9,LOW);
  analogWrite(10, VelMotorDer);
}

//*******************************************************
void DetenerMotores(int TiempoDetenidos){
  digitalWrite(5,LOW); //
  digitalWrite(6,LOW); //
  digitalWrite(9,LOW);
  digitalWrite(10,LOW); //
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


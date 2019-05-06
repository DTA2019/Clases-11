/*
 * Programa que configura Arduino y módulo ESP8266 como Servidor Web
 * Carga página web con texto + botones de comandos y modifica el estado de un LED... 
 * de acuerdo al botón seleccionado.
 * 
 * Adaptado de: https://naylampmechatronics.com/blog/21_Tutorial-ESP8266-Parte-I.html
 *              https://www.luisllamas.es/arduino-wifi-esp8266-esp01/
 */
 
#include <SoftwareSerial.h> 
SoftwareSerial esp8266(2,3); // arduino Rx (pin 2) ---- ESP8266 Tx, Tx (pin 3) ---- ESP8266 Rx

#define PinLED 7

void setup(){
 Serial.begin(9600);  //Velocidad del monitor serial del arduino
 esp8266.begin(9600); //Velocidad del ESP8255
 
 pinMode(PinLED,OUTPUT);
 digitalWrite(PinLED,LOW);
 
 sendData("AT+RST\r\n",500);      // resetear módulo
 sendData("AT+CWMODE=3\r\n",1000); // configurar como Cliente/Servidor
 //sendData("AT+RST\r\n",2000);      // resetear módulo
 sendData("AT+CWJAP=\"SSID\",\"CONTRASEÑA\"\r\n",8000); //SSID y contraseña para unirse a red 
 sendData("AT+CIFSR\r\n",1000);    // obtener dirección IP
 sendData("AT+CIPMUX=1\r\n",1000); // configurar para multiples conexiones
 sendData("AT+CIPSERVER=1,80\r\n",1000);         // servidor en el puerto 80
}
void loop(){
 if(esp8266.available()){   // revisar si hay mensaje del ESP8266
  if(esp8266.find("+IPD,")){ // revisar si el servidor recibio datos
    //responder y cerrar la conexión para que el navegador no se quede cargando 
    // página web a enviar
    delay(1500); // esperar que lleguen los datos hacia el buffer
    String webpage = "";
    int conexionID = esp8266.read()-48; // obtener el ID de la conexión para poder responder

    esp8266.find("data="); // buscar el texto "?data="
    int state = (esp8266.read()-48); // Obtener el estado del pin a mostrar
    digitalWrite(PinLED, state); // Cambiar estado del pin
    while(esp8266.available()){
      char c = esp8266.read();
      Serial.print(c);
    }
    
    webpage += "HTTP/1.1 200 OK\r\nnContent-Type: /html\r\nConnection: close\r\n\r\n";
    webpage += "<html>\n<head>\n<title>DTA_WiFi</title>\n</head>\n<body>";
    webpage += "<h2>Arduino UNO - Control LED</h2>";
    webpage += "<button onClick=location.href='./?data=1'>ON</button>";
    webpage += "<button onClick=location.href='./?data=0'>OFF</button>";
    webpage += "</body></html>";
    //webpage += "<h1>LED_13 = apagado!</h1>";
    
    // comando para enviar página web
    String comandoWebpage = "AT+CIPSEND=";
    comandoWebpage+=conexionID;
    comandoWebpage+=",";
    comandoWebpage+=webpage.length();
    comandoWebpage+="\r\n";
    sendData(comandoWebpage,1000);
    sendData(webpage,1000);
    
    
 
    // comando para terminar conexión
    String comandoCerrar = "AT+CIPCLOSE=";
    comandoCerrar+=conexionID;
    comandoCerrar+="\r\n";
    sendData(comandoCerrar,3000);
  }
 }
}
/*
Enviar comando al esp8266 y verificar la respuesta del módulo, todo esto dentro del tiempo timeout
*/
void sendData(String comando, const int timeout){
 long int time = millis(); // medir el tiempo actual para verificar timeout
 esp8266.print(comando); // enviar el comando al ESP8266
 while( (time+timeout) > millis()){ //mientras no haya timeout
  while(esp8266.available()){ //mientras haya datos por leer
  // Leer los datos disponibles
    char c = esp8266.read(); // leer el siguiente caracter
    Serial.print(c);
  }
 } 
 return;
}

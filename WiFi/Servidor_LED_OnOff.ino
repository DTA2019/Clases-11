/*
 * Programa que configura Arduino y módulo ESP8266 como Servidor Web
 * Carga página web con texto + botones de comandos y modifica el estado de un LED... 
 * de acuerdo al botón seleccionado.
 * 
 * Adaptado de: https://www.luisllamas.es/arduino-wifi-esp8266-esp01/
 */
 
#include "ESP8266.h"
#include <SoftwareSerial.h>
 
const char* SSID = "SSID";
const char* PASSWORD = "CONTRASEÑA";
 
 
SoftwareSerial softSerial(2, 3); // RX, TX
ESP8266 wifi(softSerial);
 
void setup(void)
{
   pinMode(7, OUTPUT);
 
   Serial.begin(9600);
   Serial.print("Inicio de configuración\r\n");
   
   wifi.restart();
   delay(500);
   if (wifi.setOprToStationSoftAP()) {
      Serial.print("Modo Cliente/Servidor OK\r\n");
   }
   else {
      Serial.print("Modo Cliente/Servidor ERROR\r\n");
   }
 
   if (wifi.joinAP(SSID, PASSWORD)) {
      Serial.print("Conexión a red WiFi OK\r\n");
      Serial.print("IP: ");
      Serial.println(wifi.getLocalIP().c_str());
   }
   else {
      Serial.print("Conexión a red WiFi ERROR\r\n");
   }
 
   if (wifi.enableMUX()) {
      Serial.print("Multiples conexiones OK\r\n");
   }
   else {
      Serial.print("Multiples conexiones ERROR\r\n");
   }
 
   if (wifi.startTCPServer(80)) {
      Serial.print("Inicio de servidor TCP OK\r\n");
   }
   else {
      Serial.print("Inicio de servidor TCP ERROR\r\n");
   }
 
   if (wifi.setTCPServerTimeout(20)) {
      Serial.print("Servidor TCP TimeOut 20 seg. OK\r\n");
   }
   else {
      Serial.print("Servidor TCP TimeOut 20 seg. ERROR\r\n");
   }
 
   Serial.println("Configuración finalizada\r\n");
}
 
#define wifiWrite(A) wifi.send(mux_id, (uint8_t*) A, sizeof(A) - 1);
void loop(void)
{
   uint8_t buffer[128] = { 0 };
   uint8_t mux_id;
 
   uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
   if (len > 0) {
      Serial.print("Dato recibido: ");
      Serial.print(mux_id);
      Serial.print("\r\n");
 
      wifiWrite("HTTP/1.1 200 OK\r\nnContent-Type: /html\r\nConnection: close\r\n\r\n");
      wifiWrite("<html>\n<head>\n<title>DTA_WiFi</title>\n</head>\n<body>");
      wifiWrite("<h2>Arduino UNO - Control LED</h2>");
      wifiWrite("<button onClick=location.href='./?data=1'>ON</button>");
      wifiWrite("<button onClick=location.href='./?data=0'>OFF</button>");
      wifiWrite("</body></html>");
 
      Serial.println("Envio finalizado");
 
      for (uint32_t i = 0; i < len; i++) {
         char c = (char)buffer[i];
         if (c == '?')
         {
            if ((char)buffer[i + 6] == '1')
            {
               digitalWrite(7, HIGH);
               Serial.println("LED ON");
            }
            else
            {
               digitalWrite(7, LOW);
               Serial.println("LED OFF");
            }
 
            break;
         }
      }
   }
}

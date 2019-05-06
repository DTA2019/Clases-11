/*COMANDOS AT
 * AT : Verifica comunicación con el módulo. Debe recibirse OK como respuesta.
 * AT+UART_DEF=9600,8,1,0,0 : Establece velocidad de comunicación del módulo a 9600 baudios, 8 bits de datos, 1 bit de stop, sin paridad, sin control de flujo 
    // La velocidad de fábrica depende del modelo de módulo ESP (9600, 19200, 38400, 74880, 115200, 230400, 460800  y 921600)
    // siendo habituales 9600 y 115200 
 * AT+CWMODE=3 : Configura el modo de funcionamiento (n= 1:Cliente, 2:Servidor, 3:Cliente/Servidor)
 * AT+RST : Resetea el módulo (siempre debe ejecutarse después de cambiar el modo de funcionamiento)
 * AT+CWLAP : Listado con redes WiFi disponibles
 * AT+CWJAP="SSID","Contraseña" : Conecta con la red WiFi SSID y la contraseña correspondiente
 * AT+CIFSR : Devuelve la dirección IP asignada
 * AT+CIPMUX=1 : habilita múltiples conexiones simultaneas
 * AT+CIPSERVER=1,80 : arranca un servicio web (con el número de servicio = 1) en el puerto 80
 * AT+CIPSTO=20 : Establece tiempo máximo de espera al servir una página web
 * AT+CIPSEND="conexionID","cantidaddeDatos","comandos Html" : Envía página web
 * AT+CIPCLOSE="conexionID","cantidaddeDatos","comandos Html" : Cierra página web
 */


 
#include "SoftwareSerial.h"
SoftwareSerial softSerial(2, 3); // RX, TX
 
void setup()
{
   Serial.begin(9600);
   softSerial.begin(9600);
}
 
void loop()
// enviar los datos de la consola serial al ESP-01, 
// y mostrar lo enviado por el ESP-01 a nuestra consola
{
   if (softSerial.available())
   {
      Serial.print((char)softSerial.read());
   }
   if (Serial.available())
   {
      softSerial.print((char)Serial.read());
   }
}

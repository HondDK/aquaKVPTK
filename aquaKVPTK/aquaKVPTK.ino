
#include <Wire.h>

#include "DS3231.h"

#include "DS3231.h" //нужно включить все необходимые библиотеки для работы с часами.

#include <ESP8266WebServer.h> // подключение к сети

#include <OneWire.h>// температура 

#include <DallasTemperature.h>// температура 

// Шина данных подключена к выводу D2 на ESP8266
#define ONE_WIRE_BUS D2

// Настройка объекта oneWire для связи с любыми устройствами OneWire
OneWire oneWire(ONE_WIRE_BUS);

// Передаем ссылку на объект oneWire объекту Dallas Temperature.
DallasTemperature sensors(&oneWire);

float tempSensors;

uint8_t sensor1[8] = { 0x28, 0xEE, 0xD5, 0x64, 0x1A, 0x16, 0x02, 0xEC  };

//подключение к интернету
const char* ssid = "YourNetworkName";   // SSID
const char* password = "YourPassword";  // пароль

ESP8266WebServer server(80); 

Serial.begin(115200);
delay(100);
  
sensors.begin(); 








char compileTime[] = __TIME__; //время компиляции.


void setup()

{

Serial.println("Connecting to ");
Serial.println(ssid);

// подключиться к вашей локальной wi-fi сети
WiFi.begin(ssid, password);

// проверить, выполнено ли подключение wi-fi сети
while (WiFi.status() != WL_CONNECTED) 
{
  delay(1000);
  Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected..!");
Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

Serial.println("");
Serial.println("WiFi connected..!");
Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

server.on("/", handle_OnConnect);
server.onNotFound(handle_NotFound);

server.begin();
Serial.println("HTTP server started");


clock.begin(); //включение часов.

byte hour = getInt(compileTime, 0);

byte minute = getInt(compileTime, 2);

byte second = getInt(compileTime, 4); //получение времени.

clock.fillByHMS(hour, minute, second); //подготовка для записывания в модуль времени.

clock.setTime(); //происходит запись полученной информации во внутреннюю память, начало считывания времени.

}

void loop()

{
 server.handleClient();
}

void handle_OnConnect() //получение значений температуры
{
  sensors.requestTemperatures();
  tempSensor1 = sensors.getTempC(sensor1); // Получить значение температуры
  
  server.send(200, "text/html", SendHTML(tempSensor1)); 
}

void handle_NotFound()// обработка ошибки 404
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float tempSensor1,float tempSensor2,float tempSensor3)// верстка страницы
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Temperature Monitor</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Аквариум</h1>\n";
  ptr +="<p>Температура воды: ";
  ptr +=tempSensor1;
  ptr +="&deg;C</p>";
  ptr +="&deg;C</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  
}
}

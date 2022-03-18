//АКВАРИУМ ПРОЕКТ 


#include <Wire.h>

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

uint8_t sensor1[8] = { 0x28, 0xEE, 0xD5, 0x64, 0x1A, 0x16, 0x02, 0xEC  }; // температура 

//подключение к интернету
const char* ssid = "KVPTK_Guest";   // SSID
const char* password = "YourPassword";  // пароль

ESP8266WebServer server(80); 

Serial.begin(115200);
delay(100);
  
sensors.begin(); 

const int relayPin1 = D1;     // Пин к которому подключен реле
int relaySTATE1 = LOW; // состояние реле 

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

  //Получение реального времени
  pinMode(RTCPowerPin, OUTPUT);
  digitalWrite(RTCPowerPin, HIGH);
  RTC.squareWave(SQWAVE_NONE);
  setSyncProvider(RTC.get);
  //set the system time to 17h 35m on 22 March 2015
  //setTime(17, 35, 0, 22, 3, 2015);
  RTC.set(now());
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);
  //инициализация контактный контакт подачи в качестве входного сигнала:
  pinMode(feedPin, INPUT);

  
  pinMode(relayPin1, OUTPUT);// Указываем вывод RELAY как выход //cвет в аквариуме
  digitalWrite(relayPin1, LOW);  
   
 /* pinMode(relay2,OUTPUT);// Указываем вывод RELAY как выход //cвет ультрафиолет
  digitalWrite(relay2, LOW);   
  
  pinMode(relay3,OUTPUT);// Указываем вывод RELAY как выход //воздух растениям
  digitalWrite(relay3, LOW);   

  pinMode(relay4,OUTPUT);// Указываем вывод RELAY как выход //вода из аквариума к растениям
  digitalWrite(relay4, LOW);   
  
  pinMode(relay5,OUTPUT);// Указываем вывод RELAY как выход //вода от растений в аквариум
  digitalWrite(relay5, LOW);   

  pinMode(relay6,OUTPUT);// Указываем вывод RELAY как выход //температура воды
  digitalWrite(relay6, LOW);   
 
  pinMode(relay7,OUTPUT);// Указываем вывод RELAY как выход //кормление рыб
  digitalWrite(relay7, LOW);   
*/
Serial.println("");
Serial.println("WiFi connected..!");
Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

server.on("/", handle_OnConnect);
server.onNotFound(handle_NotFound);

server.begin();
Serial.println("HTTP server started");
}

void loop()

{
   server.handleClient(); // подключение сервера 
   
  //digitalWrite(motorPin, LOW);
  //digitalWrite(RTCPowerPin, HIGH); //Включение питания RTC 
  //delay(50); //проверка 
  
  RTC.setAlarm(ALM1_MATCH_HOURS, 0, 0, 8, 0); //свет с 8 часов 
  if(RTC.alarm(ALARM_1){
   digitalWrite(relay1, HIGH);   
   relaySTATE1 = HIGH;     
  }
  RTC.setAlarm(ALM2_MATCH_HOURS, 0, 0, 20, 0); //выключение света с 8 часов вечера
   if(RTC.alarm(ALARM_2){
    digitalWrite(relay1, LOW);
    relaySTATE1 = LOW;   
  }
  
  
  //digitalWrite(RTCPowerPin, LOW); //Выключение питания RTC
  //digitalWrite(RTCPowerPin, HIGH);
  
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

String SendHTML(float tempSensor1)// верстка страницы
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
  ptr +="<button>Свет в аквариуме ";
  ptr +="</button>\n";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
  
}
}

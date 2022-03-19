
//АКВАРИУМ ПРОЕКТ 
#include <Wire.h>

#include "DS3231.h" //нужно включить все необходимые библиотеки для работы с часами.

#include <ESP8266WebServer.h> // подключение сервера 
#include <ESP8266WiFi.h> // для потключения интернета 
#include <FS.h> // для работы с файловой системой 
#include <ESP8266FtpServer.h> // для работы с сервером 

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

Serial.begin(115200);
delay(100);
  
sensors.begin(); 

const byte relayPin1 = D1;     // Пин к которому подключен реле
int relaySTATE1 = LOW; // состояние реле 


//подключение к интернету
const char* ssid = "KVPTK_Guest";   // SSID
const char* password = "YourPassword";  // пароль
ESP8266WebServer HTTP(80); 
FtpServer ftpSrv;



void setup()

{
  pinMode(relayPin1, OUTPUT);// Указываем вывод RELAY как выход //cвет в аквариуме
  digitalWrite(relayPin1, LOW);  
  
  Serial.begin(9600); // вывод данных на сервер 

  WiFi.softAP(ssid); // Создаем точку доступа 

  SPIFFS.begin();// инициализация работы с файловой системой 
  HTTP.begin();// инициализация веб сервера 
  ftpSrv.begin("relayPin1","relayPin1");// подняние фтп сервера 

  Serial.print("\nMy IP connect via Web-Browser or FTP: ");
  Serial.println(WiFi.softAPIP()); // вывод локального айпи 
  Serial.println("\n");


  //HTPP запросы 
  
  HTTP.on("/relay_switch", [] (){
    HTTP.send(200, "text/plain", relay_switch()); 
  });
  HTTP.on("/relay_status", [] (){
    HTTP.send(200, "text/plain", relayPin1_status()); 
  });
  HTTP.onNotFound([] (){
    if(!handleFileRead(HTTP.uri()))
    HTTP.send(404, "text/plain", "Not Found"); 
  });



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




}

void loop()

{
  HTTP.handleClient();//обработчик HTTP cобытий
  ftpSrv.handleFTP();//обработчик фтп соединений 
   
  //digitalWrite(motorPin, LOW);
  //digitalWrite(RTCPowerPin, HIGH); //Включение питания RTC 
  //delay(50); //проверка 
  
  RTC.setAlarm(ALM1_MATCH_HOURS, 0, 0, 8, 0); //свет с 8 часов 
  if(RTC.alarm(ALARM_1){
   digitalWrite(relayPin1, HIGH);   
   relaySTATE1 = HIGH;     
  }
  RTC.setAlarm(ALM2_MATCH_HOURS, 0, 0, 20, 0); //выключение света с 8 часов вечера
   if(RTC.alarm(ALARM_2){
    digitalWrite(relayPin1, LOW);
    relaySTATE1 = LOW;   
  }
  
  
  //digitalWrite(RTCPowerPin, LOW); //Выключение питания RTC
  //digitalWrite(RTCPowerPin, HIGH);
  
}

//переключение реле 
String relay_switch(){
  byte state;
  if(digitalRead(relayPin1))
     state = 0;
  else
     state = 1;
    digitalWrite(relayPin1,state);
    return String(state);
}

// получение статуса реле 
String relay_status(){ 
  byte state;
  if(digitalRead(relayPin1))
     state = 1;
  else
     state = 0;
    return String(state);
}


bool handleFileRead(String path){
if(path.endsWith("/")) path += "index.html";
String contentType = getContentType(path);
if(SPIFFS.exists(path)){
File file = SPIFFS.open(path,"r");
size_t sent = HTTP.streamFile(file, contentType);
file.close();
return true;
}
return false;
}

String getContentType(String filename){
if (filename.endsWith(".html")) return "text/html";
else if (filename.endsWith(".css")) return "text/css";
else if (filename.endsWith(".js")) return "application/javascript";
else if (filename.endsWith(".png")) return "image/png";
else if (filename.endsWith(".jpg")) return "image/jpeg";
return "text/plain";


}






void handle_OnConnect() //получение значений температуры
{
  sensors.requestTemperatures();
  tempSensor1 = sensors.getTempC(sensor1); // Получить значение температуры
  
  server.send(200, "text/html", SendHTML(tempSensor1)); 
}



}

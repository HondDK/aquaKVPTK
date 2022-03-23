

    //АКВАРИУМ ПРОЕКТ 


    #include <DallasTemperature.h>                            // для температуры
    #include <OneWire.h>                                      // для температуры
    
    
    #include <ESP8266WebServer.h>                             // подключение сервера 
    #include <ESP8266WiFi.h>                                  // для потключения интернета 
    #include <FS.h>                                           // для работы с файловой системой 
    #include <ESP8266FtpServer.h>                             // для работы с сервером 
    
    #include <iarduino_RTC.h>                                 // библиотека с часами RTC https://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/
    
    iarduino_RTC RTC(RTC_DS3231);  
    uint8_t D, M, Y, h, m, s, W;    
    
    //  Определяем системное время:                           // Время загрузки скетча.
    const char* strM="JanFebMarAprMayJunJulAugSepOctNovDec";  // Определяем массив всех вариантов текстового представления текущего месяца.
    const char* sysT=__TIME__;                                // Получаем время компиляции скетча в формате "SS:MM:HH".
    const char* sysD=__DATE__;                                // Получаем дату  компиляции скетча в формате "MMM:DD:YYYY", где МММ - текстовое представление текущего месяца, например: Jul.
    
    //  Парсим полученные значения sysT и sysD в массив i:    // Определяем массив «i» из 6 элементов типа int, содержащий следующие значения: секунды, минуты, часы, день, месяц и год компиляции скетча.
    const int i[6] {(sysT[6]-48)*10+(sysT[7]-48), (sysT[3]-48)*10+(sysT[4]-48), (sysT[0]-48)*10+(sysT[1]-48), (sysD[4]-48)*10+(sysD[5]-48), ((int)memmem(strM,36,sysD,3)+3-(int)&strM[0])/3, (sysD[9]-48)*10+(sysD[10]-48)};
    
    
    
    
    const int oneWireBus = D7;     // температура подключен к пину
    OneWire oneWire(oneWireBus);
    DallasTemperature sensors(&oneWire);
    
    const byte relayPin1 = D1;     // Пин к которому подключен реле Свет аквариума
    const byte relayPin2 = D2;     // Свет ультрафи 
    const byte relayPin3 = D3;     // Воздух к растениям
    const byte relayPin4 = D4;     // Вода из аквариума к растениям 
    const byte relayPin5 = D5;     // Вода от растений в аквариум
    const byte relayPin6 = D6;     // Кормушка рыб
    const byte relayPin7 = D6;     // Обогреватель 
    
    int relaySTATE1 = LOW;         // состояние реле Свет аквариума
    int relaySTATE2 = LOW;         // состояние реле ультрафи 
    int relaySTATE3 = LOW;         // Воздух к растениям
    int relaySTATE4 = LOW;         // Вода из аквариума к растениям 
    int relaySTATE5 = LOW;         // Вода от растений в аквариум
    int relaySTATE6 = LOW;         // Кормушка рыб
    int relaySTATE7 = LOW;         // Обогреватель 
    
    float aquaTEMP = 21.00;        // Какая температура нужна в аквариуме
    
    //подключение к интернету
    const char* ssid = "KVPTK_Guest";   // SSID
    const char* password = "YourPassword";  // пароль
    ESP8266WebServer HTTP(80); 
    FtpServer ftpSrv;
    


void setup()

{
    Serial.begin(9600);                                   // Инициируем передачу данных в монитор последовательного порта
    
    RTC.begin();                                          // Инициируем RTC модуль
    RTC.settime(i[0],i[1],i[2],i[3],i[4],i[5]);           // Устанавливаем время в модуль: i[0] сек, i[1] мин, i[2] час, i[3] день, i[4] месяц, i[5] год, без указания дня недели.
//  RTC.settime(i[0],i[1],i[2],i[3],i[4],i[5], 2);        // Можно установить время с указанием дня недели, где последний параметр, это день недели (указывается вручную) в формате: 0-воскресенье, 1-понедельник, ... , 6-суббота

  
    pinMode(relayPin1, OUTPUT);               // Указываем вывод RELAY как выход //cвет в аквариуме
    pinMode(relayPin2, OUTPUT);               // Свет ультрафи 
    pinMode(relayPin3, OUTPUT);               // Воздух к растениям
    pinMode(relayPin4, OUTPUT);               // Вода из аквариума к растениям 
    pinMode(relayPin5, OUTPUT);               // Вода от растений в аквариум
    pinMode(relayPin6, OUTPUT);               // Кормушка рыб
    pinMode(relayPin7, OUTPUT);               // Обогреватель
    
    digitalWrite(relayPin1, LOW); 
    digitalWrite(relayPin2, LOW);  
    digitalWrite(relayPin3, LOW);  
    digitalWrite(relayPin4, LOW);  
    digitalWrite(relayPin5, LOW);  
    digitalWrite(relayPin6, LOW);   
    digitalWrite(relayPin7, LOW);  
     
    sensors.begin(); // температура
 
    WiFi.softAP(ssid);                       // Создаем точку доступа 
    SPIFFS.begin();                          // инициализация работы с файловой системой 
    HTTP.begin();                            // инициализация веб сервера 
    ftpSrv.begin("relayPin1","relayPin1");   // подняние фтп сервера 
  
    Serial.print("\nMy IP connect via Web-Browser or FTP: ");
    Serial.println(WiFi.softAPIP());         // вывод локального айпи 
    Serial.println("\n");
  

    //HTPP запросы 
    
    HTTP.on("/relay_switch_lighting", [] (){                              // свет
     HTTP.send(200, "text/plain", relay_switch_lighting()); 
      });
      HTTP.on("/relay_status_lighting", [] (){
        HTTP.send(200, "text/plain", relay_status_lighting()); 
      });

     HTTP.on("/relay_switch_lighting_purple", [] (){                        // свет ультрафи
      HTTP.send(200, "text/plain", relay_switch_lighting_purple()); 
    });
    HTTP.on("/relay_status_lighting_purple", [] (){
      HTTP.send(200, "text/plain", relay_status_lighting_purple()); 
    });

     HTTP.on("/relay_switch_air", [] (){                                    // воздух
      HTTP.send(200, "text/plain", relay_switch_air()); 
    });
    HTTP.on("/relay_status_air", [] (){
      HTTP.send(200, "text/plain", relay_status_air()); 
    });
    
    HTTP.on("/relay_switch_water_out", [] (){                               // вода из аквариума
      HTTP.send(200, "text/plain", relay_switch_air()); 
    });
    HTTP.on("/relay_status_water_out", [] (){
      HTTP.send(200, "text/plain", relay_status_air()); 
    });

     HTTP.on("/relay_switch_water_in", [] (){                               // вода в аквариум
      HTTP.send(200, "text/plain", relay_switch_air()); 
    });
    HTTP.on("/relay_status_water_in", [] (){
      HTTP.send(200, "text/plain", relay_status_air()); 
    });

     HTTP.on("/relay_switch_temp", [] (){                                   // обогреватель
      HTTP.send(200, "text/plain", relay_switch_air()); 
    });
    HTTP.on("/relay_status_temp", [] (){
      HTTP.send(200, "text/plain", relay_status_air()); 
    });

     HTTP.on("/aqua_temp()", [] (){
      HTTP.send(200, "text/plain", aqua_temp()); 
    });
    HTTP.onNotFound([] (){
      if(!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "Not Found"); 
    });

}

void loop()

{
    
    HTTP.handleClient();                                    // обработчик HTTP cобытий
    ftpSrv.handleFTP();                                     // обработчик фтп соединений 
   
    sensors.requestTemperatures();                          // получение температуры  
    float temperatureC = sensors.getTempCByIndex(0);        // получение температуры 
  
  
    if(millis()%1000==0){                                   // Если прошла 1 секунда
          Serial.println(RTC.gettime("d-m-Y, H:i:s, D"));   // Выводим время в монитор порта, одной строкой
          delay(2);                                         // Приостанавливаем скетч на 2 мс.
      }
      
   if(millis()%1000==0){                                  // если прошла 1 секунда
        RTC.gettime();                                    // Считываем текущее время из модуля.
        D = RTC.day;                                      // Получаем текущий день месяца 1-31.
        M = RTC.month;                                    // Получаем текущий месяц       1-12.
        Y = RTC.year;                                     // Получаем текущий год         0-99.
        h = RTC.Hours;                                    // Получаем текущие часы        0-23.
        m = RTC.minutes;                                  // Получаем текущие минуты      0-59.
        s = RTC.seconds;                                  // Получаем текущие секунды     0-59.
        W = RTC.weekday;                                  // Получаем текущий день недели 0-6.
        Serial.println((String) D+"-"+M+"-"+Y+", "+h+":"+m+":"+s+", "+W); // Выводим время в монитор порта, одной строкой.
        delay(2);                                         // приостанавливаем скетч на 2 мс.
    }
   
    //RTC.setAlarm(ALM1_MATCH_HOURS, 0, 0, 8, 0);           //свет с 8 часов + ульрафи
    if(h == 8 && m == 0){
     digitalWrite(relayPin1, HIGH);   
     relaySTATE1 = HIGH;     
    }
   // RTC.setAlarm(ALM2_MATCH_HOURS, 0, 0, 20, 0);          //выключение света с 8 часов вечера + ульрафи
    if(h == 20 && m == 0){
      digitalWrite(relayPin1, LOW);
      relaySTATE1 = LOW;   
    }
    
   
  
     //включение ультрафи 
    if(h >= 8 && m == 0){
     digitalWrite(relayPin2, HIGH);   
     relaySTATE2 = HIGH;     
    }                                                         //Свет ультрафи 
      //выключение ультрафи 
    if(h >= 8 && m == 5){
     digitalWrite(relayPin2, LOW);   
     relaySTATE2 = LOW;   
    }
    if(h >= 20 && relaySTATE2 == HIGH){
     digitalWrite(relayPin2, LOW);   
     relaySTATE2 = LOW;  
    }
  
    //Воздух к растениям
    if(m == 0){
      digitalWrite(relayPin3, HIGH);   
      relaySTATE3 = HIGH;  
    }
    if(m == 20){
      digitalWrite(relayPin3, LOW);   
      relaySTATE3 = LOW;  
    }
    
    //Вода из аквариума к растениям и вода от растений к аквариуму
    if(h >= 8 && m == 0){
     digitalWrite(relayPin4, HIGH);   
     digitalWrite(relayPin5, HIGH);   
     relaySTATE4 = HIGH;     
     relaySTATE5 = HIGH;     
    }                                                        
    if(h >= 8 && m == 15){
     digitalWrite(relayPin4, LOW);   
     digitalWrite(relayPin5, LOW);   
     relaySTATE4 = LOW;     
     relaySTATE5 = LOW;   
    }
    if(h >= 20 && relaySTATE4 == HIGH || relaySTATE5 == HIGH){
     digitalWrite(relayPin4, LOW);   
     digitalWrite(relayPin5, LOW);   
     relaySTATE4 = LOW;     
     relaySTATE5 = LOW;   
    }
  
  
    //Кормушка
    if(h == 8 && m == 1){
      digitalWrite(relayPin6, HIGH);    
      delay(5000);
      digitalWrite(relayPin6, LOW);    
      }
   if(h == 18 && m == 1){
      digitalWrite(relayPin6, HIGH);    
      delay(5000);
      digitalWrite(relayPin6, LOW);    
      }
  

    //Температура 
    if(round(aquaTEMP) < round(temperatureC)){
      digitalWrite(relayPin7, HIGH);   
      relaySTATE7 = HIGH; 
    }
    if(round(aquaTEMP) > round(temperatureC) ){
      digitalWrite(relayPin7, LOW);   
      relaySTATE7 = LOW;  
    }
    
  }

    //Температура на сервер
    String aqua_temp(){
      float temperatureC = sensors.getTempCByIndex(0);        // получение температуры 
      return String (temperatureC);
    }


    //переключение реле 
    String relay_switch_lighting(){        // свет
      byte state;
      if(digitalRead(relayPin1))
         state = 0;
      else
         state = 1;
        digitalWrite(relayPin1,state);
        return String(state);
    }
    
    // получение статуса реле 
    String relay_status_lighting(){       // свет
      byte state;
      if(digitalRead(relayPin1))
         state = 1;
      else
         state = 0;
        return String(state);
    }
    
    //переключение реле 
    String relay_switch_lighting_purple(){  // свет ультрафи
      byte state;
      if(digitalRead(relayPin2))
         state = 0;
      else
         state = 1;
        digitalWrite(relayPin2,state);
        return String(state);
    }
    
    // получение статуса реле 
    String relay_status_lighting_purple(){  // свет ультрафи
      byte state;
      if(digitalRead(relayPin2))
         state = 1;
      else
         state = 0;
        return String(state);
    }
    
    //переключение реле 
    String relay_switch_air(){              // воздух
      byte state;
      if(digitalRead(relayPin3))
         state = 0;
      else
         state = 1;
        digitalWrite(relayPin3,state);
        return String(state);
    }
    
    // получение статуса реле 
    String relay_status_air(){              // воздух
      byte state;
      if(digitalRead(relayPin3))
         state = 1;
      else
         state = 0;
        return String(state);
    }

    String relay_switch_water_out(){              // вода из аквариума
      byte state;
      if(digitalRead(relayPin4))
         state = 0;
      else
         state = 1;
        digitalWrite(relayPin4,state);
        return String(state);
    }
    
    // получение статуса реле 
    String relay_status_water_out(){              // вода из аквариума
      byte state;
      if(digitalRead(relayPin4))
         state = 1;
      else
         state = 0;
        return String(state);
    }

      String relay_switch_water_in(){              // вода в аквариум
      byte state;
      if(digitalRead(relayPin5))
         state = 0;
      else
         state = 1;
        digitalWrite(relayPin5,state);
        return String(state);
    }
    
    // получение статуса реле 
    String relay_status_water_in(){              // вода в аквариум
      byte state;
      if(digitalRead(relayPin5))
         state = 1;
      else
         state = 0;
        return String(state);
    }


       String relay_switch_temp(){              // обогреватель
      byte state;
      if(digitalRead(relayPin7))
         state = 0;
      else
         state = 1;
        digitalWrite(relayPin7,state);
        return String(state);
    }
    
    // получение статуса реле 
    String relay_status_temp(){              // обогреватель
      byte state;
      if(digitalRead(relayPin7))
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

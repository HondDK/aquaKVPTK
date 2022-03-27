


    //АКВАРИУМ ПРОЕКТ 


    #include <DallasTemperature.h>                            // для температуры
    #include <OneWire.h>                                      // для температуры
    #include <Wire.h>                                         // Подключаем библиотеку Wire для работы с шиной I2C
    
    #include <ESP8266SSDP.h>   
    #include <FS.h>                                           // для работы с файловой системой 
   
    #include <ESP8266WebServer.h>                             // подключение сервера 
    #include <ESP8266WiFi.h>                                  // для потключения интернета 
    #include <ESP8266HTTPUpdateServer.h>
    #include <NTPClient.h>
    #include <WiFiUdp.h>
    String AP       = "aboba";           // Здесь храним название точки доступа (роутера), к которой будем подключаться
    String PASSWORD = "222212003D@#";       // Здесь храним пароль для подключения к точке доступа (роутеру)
    String SSDP_Name = "AQUA_KVPTK";          // Здесь определяем название устройства в сетевом окружении

    
    
    
    #include <iarduino_RTC.h>                                 // библиотека с часами RTC https://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/
    iarduino_RTC watch(RTC_DS3231);  
  
                                                             //
    //  Определяем системное время:                           // Время загрузки скетча.
    const char* strM="JanFebMarAprMayJunJulAugSepOctNovDec";  // Определяем массив всех вариантов текстового представления текущего месяца.
    const char* sysT=__TIME__;                                // Получаем время компиляции скетча в формате "SS:MM:HH".
    const char* sysD=__DATE__;                                // Получаем дату  компиляции скетча в формате "MMM:DD:YYYY", где МММ - текстовое представление текущего месяца, например: Jul.
    //  Парсим полученные значения sysT и sysD в массив i:    // Определяем массив «i» из 6 элементов типа int, содержащий следующие значения: секунды, минуты, часы, день, месяц и год компиляции скетча.
    const int i[6] {(sysT[6]-48)*10+(sysT[7]-48), (sysT[3]-48)*10+(sysT[4]-48), (sysT[0]-48)*10+(sysT[1]-48), (sysD[4]-48)*10+(sysD[5]-48), ((int)memmem(strM,36,sysD,3)+3-(int)&strM[0])/3, (sysD[9]-48)*10+(sysD[10]-48)};
    
    
    //const int oneWireBus = D8;     // температура подключен к пину
   // OneWire oneWire(oneWireBus);
    //DallasTemperature sensors(&oneWire);
    
    const byte relayPin1 = D3;     // Пин к которому подключен реле Свет аквариума
    const byte relayPin2 = D4;     // Свет ультрафи 
    const byte relayPin3 = D5;     // Воздух к растениям
    const byte relayPin4 = D6;     // Вода из аквариума к растениям 
    const byte relayPin5 = D7;     // Вода от растений в аквариум
    const byte relayPin6 = D8;     // Кормушка рыб
    const byte relayPin7 = 03;     // Обогреватель 
    
    int relaySTATE1 = LOW;         // состояние реле Свет аквариума
    int relaySTATE2 = LOW;         // состояние реле ультрафи 
    int relaySTATE3 = LOW;         // Воздух к растениям
    int relaySTATE4 = LOW;         // Вода из аквариума к растениям 
    int relaySTATE5 = LOW;         // Вода от растений в аквариум
    int relaySTATE6 = LOW;         // Кормушка рыб
    int relaySTATE7 = LOW;         // Обогреватель 
    
    float aquaTEMP = 21.00;        // Какая температура нужна в аквариуме
    
    const long utcOffsetInSeconds = 21600;
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    ESP8266WebServer HTTP(80); 
void setup()

{
    Serial.begin(9600);                                   // Инициируем передачу данных в монитор последовательного порта
    watch.begin();                                        // Инициируем RTC модуль
    
    WiFi.mode(WIFI_STA);                        // Определяем режим работы Wi-Fi модуля в режиме клиента
    WiFi.begin(AP.c_str(), PASSWORD.c_str());   // Подключаемся к точке доступа (роутеру)
    HTTP.begin();                               // Инициализируем Web-сервер
    SPIFFS.begin();                             // Инициализируем файловую систему
    SSDP_init();                                 // Вызываем функцию инициализации SSDP (функция описана ниже)
    //HTTP.begin();                              // инициализация веб сервера 
    //ftpSrv.begin("relay","relay");            // подняние фтп сервера 
    //SPIFFS.begin();                              // инициализация работы с файловой системой 
    Serial.print("\nMy IP connect via Web-Browser or FTP: ");
    Serial.println(WiFi.softAPIP());               // вывод локального айпи 
    Serial.println("\n");
    
   
  
    pinMode(relayPin1, OUTPUT);               // Указываем вывод RELAY как выход //cвет в аквариуме
    pinMode(relayPin2, OUTPUT);               // Свет ультрафи 
    pinMode(relayPin3, OUTPUT);               // Воздух к растениям
    pinMode(relayPin4, OUTPUT);               // Вода из аквариума к растениям 
    pinMode(relayPin5, OUTPUT);               // Вода от растений в аквариум
    pinMode(relayPin6, OUTPUT);               // Кормушка рыб
    pinMode(relayPin7, OUTPUT);               // Обогреватель
    
    digitalWrite(relayPin1, HIGH); 
    digitalWrite(relayPin2, HIGH);  
    digitalWrite(relayPin3, HIGH);  
    digitalWrite(relayPin4, HIGH);  
    digitalWrite(relayPin5, HIGH);  
    digitalWrite(relayPin6, HIGH);   
    digitalWrite(relayPin7, HIGH);  
     
//    sensors.begin(); // температура
    
  

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

    // HTTP.on("/aqua_temp", [] (){
     // HTTP.send(200, "text/plain", aqua_temp()); 
   // });
     HTTP.on("/status_time", [] (){
      HTTP.send(200, "text/plain", status_time()); 
    });
    HTTP.onNotFound([] (){
      if(!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "Not Found"); 
    });
    
   
        
    
}

void loop()

{
    
    HTTP.handleClient();                                    // обработчик HTTP cобытий
    //ftpSrv.handleFTP();                                     // обработчик фтп соединений 
   
    //sensors.requestTemperatures();                          // получение температуры  
    //float temperatureC = sensors.getTempCByIndex(0);        // получение температуры 
  
 
    timeClient.update();
    watch.settime(timeClient.getSeconds(),timeClient.getMinutes(),timeClient.getHours());   // Устанавливаем время в модуль: i[0] сек, i[1] мин, i[2] час, i[3] день, i[4] месяц, i[5] год, без указания дня недели.      
           
   if(millis()%1000==0){                                  // если прошла 1 секунда
       Serial.println(watch.gettime("d-m-Y, H:i:s, D")); // 
        Serial.print(daysOfTheWeek[timeClient.getDay()]);
        Serial.print(", ");
        Serial.print(timeClient.getHours());
        Serial.print(":");
        Serial.print(timeClient.getMinutes());
        Serial.print(":");
        Serial.println(timeClient.getSeconds());
    }

      

    
      
  /*
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
      //delay(5000);
      digitalWrite(relayPin6, LOW);    
      }
   if(h == 18 && m == 1){
      digitalWrite(relayPin6, HIGH);    
      //delay(5000);
      digitalWrite(relayPin6, LOW);    
      }
  
/*
    //Температура 
    if(round(aquaTEMP) < round(temperatureC)){
      digitalWrite(relayPin7, HIGH);   
      relaySTATE7 = HIGH; 
    }
    if(round(aquaTEMP) > round(temperatureC) ){
      digitalWrite(relayPin7, LOW);   
      relaySTATE7 = LOW;  
    }
    */
  }

    //Температура на сервер
    //String aqua_temp(){
     // float temperatureC = sensors.getTempCByIndex(0);        // получение температуры 
     // return String (temperatureC);
  //  }

  
   String status_time(){  
     String time_aqua = watch.gettime(" H:i:s") ;
     return String(time_aqua);
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



  bool handleFileRead(String path){                       // Функция работы с файловой системой
  if(path.endsWith("/")) path += "index.html";           // Если устройство вызывается по корневому адресу, то должен вызываться файл index.htm (добавляем его в конец адреса)
  String contentType = getContentType(path);            // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
  String pathWithGz = path + ".gz";                     // Заводим еще один путь с адресом заархивированной версии файла из адреса обращения
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){ // Если в файловой системе существует заархивированный или простой файл по адресу обращения
    if(SPIFFS.exists(pathWithGz))                       //  Если существует заархивированный файл,
      path += ".gz";                                    //    то добавляем в адрес обращения окончание, указывающее на архив
    File file = SPIFFS.open(path, "r");                 //  Открываем файл для чтения
    size_t sent = HTTP.streamFile(file, contentType);   //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
    file.close();                                       //  Закрываем файл
    return true;                                        //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
  }
  return false;                                         // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
}

    String getContentType(String filename){
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".png")) return "image/png";
    else if (filename.endsWith(".jpg")) return "image/jpeg";
    return "text/plain";
    
    }

    void SSDP_init(void) {                                                  // Функция формирующая описательные свойства устройства для его обнаружения в сетевом окружении
  HTTP.on("/description.xml", HTTP_GET, []() {                          // Если происходит обращение к файлу /description.xml в корне устройства
    SSDP.schema(HTTP.client());                                         // Создаем SSDP-схему описания устройства
  });
  SSDP.setDeviceType("upnp:rootdevice");                                // Тип устройства 
  SSDP.setSchemaURL("description.xml");                                 // Файл с описанием
  SSDP.setHTTPPort(80);                                                 // Порт работы с HTTP
  SSDP.setName(SSDP_Name);                                              // Нзвание устройства в сетевом окружении
  SSDP.setSerialNumber(ESP.getChipId());                                // Серийный номер (для заполнения используем ID чипа ESP)
  SSDP.setURL("/");                                                     // Адрес по которому производится обращение к устройству при инициализации подключения
  SSDP.setModelName(SSDP_Name);                                         // Название модели устройства
  SSDP.setModelNumber("000000000001");                                  // Номер модели
  SSDP.setModelURL("KVPTK");                                 // Страница с описанием данной модели в Интернет
  SSDP.setManufacturer("KVPTK");                                       // Производитель
  SSDP.setManufacturerURL("KVPTK");                          // Адрес сайта производителя
  SSDP.begin();                                                         // Отдаем сформированную SSDP-схему описания устройства
}

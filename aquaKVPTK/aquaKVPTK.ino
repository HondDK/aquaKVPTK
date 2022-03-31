


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
  
                                                             
    //  Определяем системное время:                           // Время загрузки скетча.
    const char* strM="JanFebMarAprMayJunJulAugSepOctNovDec";  // Определяем массив всех вариантов текстового представления текущего месяца.
    const char* sysT=__TIME__;                                // Получаем время компиляции скетча в формате "SS:MM:HH".
    const char* sysD=__DATE__;                                // Получаем дату  компиляции скетча в формате "MMM:DD:YYYY", где МММ - текстовое представление текущего месяца, например: Jul.
    //  Парсим полученные значения sysT и sysD в массив i:    // Определяем массив «i» из 6 элементов типа int, содержащий следующие значения: секунды, минуты, часы, день, месяц и год компиляции скетча.
    const int i[6] {(sysT[6]-48)*10+(sysT[7]-48), (sysT[3]-48)*10+(sysT[4]-48), (sysT[0]-48)*10+(sysT[1]-48), (sysD[4]-48)*10+(sysD[5]-48), ((int)memmem(strM,36,sysD,3)+3-(int)&strM[0])/3, (sysD[9]-48)*10+(sysD[10]-48)};

    
    const long utcOffsetInSeconds = 21600; // устанавливаем часовой пояс
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);//берем время с интернета
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
  
    //const int oneWireBus = D8;     // температура подключен к пину
   // OneWire oneWire(oneWireBus);
    //DallasTemperature sensors(&oneWire);
    unsigned long timing;
    
    const byte relayPin1 = D3;     // Пин к которому подключен реле Свет аквариума
    const byte relayPin2 = D4;     // Свет ультрафи 
    const byte relayPin3 = D5;     // Воздух к растениям
    const byte relayPin4 = D6;     // Вода из аквариума к растениям 
    const byte relayPin5 = D7;     // Вода от растений в аквариум
    const byte relayPin6 = D8;     // Кормушка рыб
    const byte relayPin7 = 03;     // Обогреватель 
    
    int relaySTATE1 = HIGH;         // состояние реле Свет аквариума
    int relaySTATE2 = HIGH;         // состояние реле ультрафи 
    int relaySTATE3 = HIGH;         // Воздух к растениям
    int relaySTATE4 = HIGH;         // Вода из аквариума к растениям 
    int relaySTATE5 = HIGH;         // Вода от растений в аквариум
    int relaySTATE6 = HIGH;         // Кормушка рыб
    int relaySTATE7 = HIGH;         // Обогреватель 
    
   
    int lighting_hours_on = 8;              // Свет аквариума таймер
    int lighting_min_on = 0;
    int lighting_hours_off = 20;            // Свет аквариума таймер
    int lighting_min_off = 0;
    
    int lighting_purple_hours_on = 8 ;     // ультрафи  таймер
    int lighting_purple_min_on = 0;
    int lighting_purple_hours_off = 20 ;     // ультрафи  таймер
    int lighting_purple_min_off = 5;
    
    int air_hours_on;                 // Воздух к растениям таймер
    int air_min_on = 0;
    int air_hours_off;                 // Воздух к растениям таймер
    int air_min_off = 20;
    
    int water_out_hours_on;           // Вода из аквариума к растениям и вода от растений в аквариум
    int water_out_min_on = 0;
    int water_out_hours_off;           // Вода из аквариума к растениям и вода от растений в аквариум
    int water_out_min_off = 15;
    
    
    int food_hours_on_1 = 8;                // Кормушка рыб таймер
    int food_min_on_1 = 0;
    int food_hours_on_2 = 18;                // Кормушка рыб таймер
    int food_min_on_2 = 0 ;
 
    float aquaTEMP = 21.00;        // Какая температура нужна в аквариуме


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
      HTTP.send(200, "text/plain", relay_switch_water_out()); 
    });
    HTTP.on("/relay_status_water_out", [] (){
      HTTP.send(200, "text/plain", relay_status_water_out()); 
    });

     HTTP.on("/relay_switch_water_in", [] (){                               // вода в аквариум
      HTTP.send(200, "text/plain", relay_switch_water_in()); 
    });
    HTTP.on("/relay_status_water_in", [] (){
      HTTP.send(200, "text/plain", relay_status_water_in()); 
    });

     HTTP.on("/relay_switch_temp", [] (){                                   // обогреватель
      HTTP.send(200, "text/plain", relay_switch_temp()); 
    });
    HTTP.on("/relay_status_temp", [] (){
      HTTP.send(200, "text/plain", relay_status_temp()); 
    });

    // HTTP.on("/aqua_temp", [] (){
     // HTTP.send(200, "text/plain", aqua_temp()); 
   // });
   
     HTTP.on("/status_time", [] (){
      HTTP.send(200, "text/plain", status_time()); 
    });

     HTTP.on("/timer_time_lighting_h_on", [] (){
      HTTP.send(200, "text/plain", f_lighting_hours_on()); 
    });
    HTTP.on("/timer_time_lighting_m_on", [] (){
      HTTP.send(200, "text/plain", f_lighting_min_on()); 
    });
     HTTP.on("/timer_time_lighting_h_off", [] (){
      HTTP.send(200, "text/plain", f_lighting_hours_off()); 
    });
    HTTP.on("/timer_time_lighting_m_off", [] (){
      HTTP.send(200, "text/plain", f_lighting_min_off()); 
    });
    
     HTTP.on("/timer_time_lighting_purple_m_on", [] (){
      HTTP.send(200, "text/plain", f_lighting_purple_min_on()); 
    });
    HTTP.on("/timer_time_lighting_purple_m_off", [] (){
      HTTP.send(200, "text/plain", f_lighting_purple_min_off()); 
    });

     
    HTTP.on("/timer_air_m_off", [] (){
      HTTP.send(200, "text/plain", f_air_min_off()); 
    });
    
    //Вода из аквариума к растениям и вода от растений в аквариум
    HTTP.on("/timer_time_water_out_m_off", [] (){
      HTTP.send(200, "text/plain", f_water_out_min_off()); 
    });

    //Кормушка
      HTTP.on("/timer_time_food_h_1", [] (){
      HTTP.send(200, "text/plain", f_food_hours_on_1()); 
    });
    HTTP.on("/timer_time_food_m_1", [] (){
      HTTP.send(200, "text/plain", f_food_min_on_1()); 
    });
    //Кормушка
      HTTP.on("/timer_time_food_h_2", [] (){
      HTTP.send(200, "text/plain", f_food_hours_on_2()); 
    });
    HTTP.on("/timer_time_food_m_2", [] (){
      HTTP.send(200, "text/plain", f_food_min_on_2()); 
    });


    HTTP.on("/setup_timer_time_min_1", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_1()); 
        });
     HTTP.on("/setup_timer_time_min_2", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_2()); 
        });
         HTTP.on("/setup_timer_time_min_3", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_3()); 
        });
         HTTP.on("/setup_timer_time_min_4", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_4()); 
        });
         HTTP.on("/setup_timer_time_min_5", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_5()); 
        });
         HTTP.on("/setup_timer_time_min_6", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_6()); 
        });
         HTTP.on("/setup_timer_time_min_7", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_7()); 
        });
         HTTP.on("/setup_timer_time_min_8", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_8()); 
        });
         HTTP.on("/setup_timer_time_min_9", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_9()); 
        });
        HTTP.on("/setup_timer_time_min_10", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_10()); 
        });
        HTTP.on("/setup_timer_time_min_11", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_11()); 
        });
        HTTP.on("/setup_timer_time_min_12", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_12()); 
        });
        HTTP.on("/setup_timer_time_min_13", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_13()); 
        });
        HTTP.on("/setup_timer_time_min_14", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_14()); 
        });
        HTTP.on("/setup_timer_time_min_15", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_15()); 
        });
        HTTP.on("/setup_timer_time_min_16", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_16()); 
        });
        HTTP.on("/setup_timer_time_min_17", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_17()); 
        });
        HTTP.on("/setup_timer_time_min_18", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_18()); 
        });
        HTTP.on("/setup_timer_time_min_19", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_19()); 
        });
        HTTP.on("/setup_timer_time_min_20", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_20()); 
        });
        HTTP.on("/setup_timer_time_min_21", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_21()); 
        });
        HTTP.on("/setup_timer_time_min_22", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_22()); 
        });
        HTTP.on("/setup_timer_time_min_23", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_23()); 
        });
        HTTP.on("/setup_timer_time_min_24", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_24()); 
        });
        HTTP.on("/setup_timer_time_min_25", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_25()); 
        });
        HTTP.on("/setup_timer_time_min_26", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_26()); 
        });
        HTTP.on("/setup_timer_time_min_27", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_27()); 
        });
        HTTP.on("/setup_timer_time_min_28", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_28()); 
        });
         HTTP.on("/setup_timer_time_min_29", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_29()); 
        });
        HTTP.on("/setup_timer_time_min_30", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_30()); 
        });
         HTTP.on("/setup_timer_time_min_31", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_31()); 
        });
        HTTP.on("/setup_timer_time_min_32", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_32()); 
        });
         HTTP.on("/setup_timer_time_min_33", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_33()); 
        });
        HTTP.on("/setup_timer_time_min_34", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_34()); 
        });
         HTTP.on("/setup_timer_time_min_35", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_35()); 
        });
        HTTP.on("/setup_timer_time_min_36", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_36()); 
        });
         HTTP.on("/setup_timer_time_min_37", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_37()); 
        });
        HTTP.on("/setup_timer_time_min_38", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_38()); 
        });
         HTTP.on("/setup_timer_time_min_39", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_39()); 
        });
        HTTP.on("/setup_timer_time_min_40", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_40()); 
        });
         HTTP.on("/setup_timer_time_min_41", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_41()); 
        });
        HTTP.on("/setup_timer_time_min_42", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_42()); 
        });
         HTTP.on("/setup_timer_time_min_43", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_43()); 
        });
        HTTP.on("/setup_timer_time_min_44", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_44()); 
        });
         HTTP.on("/setup_timer_time_min_45", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_45()); 
        });
        HTTP.on("/setup_timer_time_min_46", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_47()); 
        });
         HTTP.on("/setup_timer_time_min_48", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_48()); 
        });
        HTTP.on("/setup_timer_time_min_49", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_49()); 
        });
         HTTP.on("/setup_timer_time_min_50", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_50()); 
        });
        HTTP.on("/setup_timer_time_min_51", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_51()); 
        });
         HTTP.on("/setup_timer_time_min_52", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_52()); 
        });
        HTTP.on("/setup_timer_time_min_53", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_53()); 
        });
        HTTP.on("/setup_timer_time_min_54", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_54()); 
        });
        HTTP.on("/setup_timer_time_min_55", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_55()); 
        });
        HTTP.on("/setup_timer_time_min_56", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_56()); 
        });
        HTTP.on("/setup_timer_time_min_57", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_57()); 
        });
        HTTP.on("/setup_timer_time_min_58", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_58()); 
        });
         HTTP.on("/setup_timer_time_min_59", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_59()); 
        });
        HTTP.on("/setup_timer_time_min_60", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_60()); 
        });


       HTTP.on("/setup_timer_time_h_1", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_1()); 
        });
         HTTP.on("/setup_timer_time_h_2", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_2()); 
        });
         HTTP.on("/setup_timer_time_h_3", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_3()); 
        });
         HTTP.on("/setup_timer_time_h_4", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_4()); 
        });
         HTTP.on("/setup_timer_time_h_5", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_5()); 
        });
         HTTP.on("/setup_timer_time_h_6", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_6()); 
        });
         HTTP.on("/setup_timer_time_h_7", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_7()); 
        });
         HTTP.on("/setup_timer_time_h_8", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_8()); 
        });
         HTTP.on("/setup_timer_time_h_8", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_8()); 
        });
         HTTP.on("/setup_timer_time_h_9", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_9()); 
        });
         HTTP.on("/setup_timer_time_h_10", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_10()); 
        });
         HTTP.on("/setup_timer_time_h_11", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_11()); 
        });
         HTTP.on("/setup_timer_time_h_12", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_12()); 
        });
         HTTP.on("/setup_timer_time_h_13", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_13()); 
        });
         HTTP.on("/setup_timer_time_h_14", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_14()); 
        });
         HTTP.on("/setup_timer_time_h_15", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_15()); 
        });
         HTTP.on("/setup_timer_time_h_16", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_16()); 
        });
         HTTP.on("/setup_timer_time_h_17", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_17()); 
        });
         HTTP.on("/setup_timer_time_h_18", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_18()); 
        });
         HTTP.on("/setup_timer_time_h_19", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_19()); 
        });
         HTTP.on("/setup_timer_time_h_20", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_20()); 
        });
         HTTP.on("/setup_timer_time_h_21", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_21()); 
        });
         HTTP.on("/setup_timer_time_h_22", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_22()); 
        });
         HTTP.on("/setup_timer_time_h_23", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_h_23()); 
        });


    HTTP.onNotFound([] (){
      if(!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "Not Found"); 
    });

}

void loop()

{
    
    HTTP.handleClient();                                    // обработчик HTTP cобытий
    timeClient.update();                                    // для получения времени с инернета
    
    //sensors.requestTemperatures();                          // получение температуры  
    //float temperatureC = sensors.getTempCByIndex(0);        // получение температуры 
  
    
    watch.settime(timeClient.getSeconds(),timeClient.getMinutes(),timeClient.getHours());   // Устанавливаем время в модуль(взятое с интернета): i[0] сек, i[1] мин, i[2] час, i[3] день, i[4] месяц, i[5] год, без указания дня недели.      
           
   if(millis()%1000==0){                                  // если прошла 1 секунда
       Serial.println(watch.gettime("d-m-Y, H:i:s, D"));  // вывод времени на ds3231
       
        Serial.print(daysOfTheWeek[timeClient.getDay()]); // вывод времени с интернета
        Serial.print(", ");                               // вывод времени с интернета
        Serial.print(timeClient.getHours());              // вывод времени с интернета
        Serial.print(":");                                // вывод времени с интернета
        Serial.print(timeClient.getMinutes());            // вывод времени с интернета
        Serial.print(":");                                // вывод времени с интернета
        Serial.println(timeClient.getSeconds());          // вывод времени с интернета
    }

   //Установка таймеров
   
   // освещение
    if(watch.Hours >= lighting_hours_on && watch.minutes >= lighting_min_on && watch.Hours <= lighting_hours_off){
     digitalWrite(relayPin1, LOW);   
     relaySTATE1 = LOW;     
    }
   
    if(watch.Hours >= lighting_hours_off && watch.minutes >= lighting_min_off){
      digitalWrite(relayPin1, HIGH);
      relaySTATE1 = HIGH;   
    }
    
   
  
     //включение ультрафи 
    if(watch.Hours >= lighting_purple_hours_on && watch.minutes == lighting_purple_min_on && watch.Hours <= lighting_purple_hours_off && watch.seconds == 1){
     digitalWrite(relayPin2, LOW);   
     relaySTATE2 = LOW;     
    }                                                         //Свет ультрафи 
      //выключение ультрафи 
    if(watch.Hours >= lighting_purple_hours_on && watch.minutes == lighting_purple_min_off && watch.Hours <= lighting_purple_hours_off && watch.seconds == 1){
     digitalWrite(relayPin2, HIGH);   
     relaySTATE2 = HIGH;   
    }
    if(watch.Hours >= lighting_purple_hours_off && relaySTATE2 == LOW && watch.seconds == 1){
     digitalWrite(relayPin2, HIGH);   
     relaySTATE2 = HIGH;  
    }
  
    //Воздух к растениям
    if(watch.minutes >= air_min_on && watch.minutes <= air_min_off && watch.seconds == 1){
      digitalWrite(relayPin3, LOW);   
      relaySTATE3 = LOW;  
    }
    if(watch.minutes >= air_min_off && watch.seconds == 1){
      digitalWrite(relayPin3, HIGH);   
      relaySTATE3 = HIGH;  
    }
    
    //Вода из аквариума к растениям и вода от растений к аквариуму
    if(watch.Hours >= 8 && watch.minutes >= water_out_min_on && watch.Hours <= 20 && watch.seconds == 1){
     digitalWrite(relayPin4, LOW);   
     digitalWrite(relayPin5, LOW);   
     relaySTATE4 = LOW;     
     relaySTATE5 = LOW;     
    }                                                        
    if(watch.Hours >= 8 && watch.minutes >= water_out_min_off && watch.Hours <= 20 && watch.seconds == 1){
     digitalWrite(relayPin4, HIGH);   
     digitalWrite(relayPin5, HIGH);   
     relaySTATE4 = HIGH;     
     relaySTATE5 = HIGH;   
    }
    if(watch.Hours >= 20 && watch.seconds == 1){
     digitalWrite(relayPin4, HIGH);   
     digitalWrite(relayPin5, HIGH);   
     relaySTATE4 = HIGH;     
     relaySTATE5 = HIGH;   
    }
  
  
    //Кормушка
    if(watch.Hours == food_hours_on_1 && watch.minutes == food_min_on_1 && watch.seconds == 1){
      digitalWrite(relayPin6, LOW);    
      }
       //Кормушка
    if(watch.Hours == food_hours_on_1 && watch.minutes == food_min_on_1 && watch.seconds == 10){
      digitalWrite(relayPin6, HIGH);   
      }
   if(watch.Hours == food_hours_on_2 && watch.minutes == food_min_on_2 && watch.seconds == 1){
      digitalWrite(relayPin6, LOW);    
      }
   if(watch.Hours == food_hours_on_2 && watch.minutes == food_min_on_2 && watch.seconds == 10){
      digitalWrite(relayPin6, HIGH);   
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


   //получение времени
   String status_time(){  
     String time_aqua = watch.gettime("H:i") ;
     return String(time_aqua);
    }

  int setup_timer_time(){
    
    switch(){

    }

  }

  void setup_timer_time_min_1(){
    lighting_purple_min_off = 1;    
  }
  void setup_timer_time_min_2(){
    lighting_purple_min_off = 2;    
  }
  void setup_timer_time_min_3(){
    lighting_purple_min_off = 3;    
  }
  void setup_timer_time_min_4(){
    lighting_purple_min_off = 4;    
  }
  void setup_timer_time_min_5(){
    lighting_purple_min_off = 5;    
  }
  void setup_timer_time_min_6(){
    lighting_purple_min_off = 6;    
  }
  void setup_timer_time_min_7(){
    lighting_purple_min_off = 7;    
  }
  void setup_timer_time_min_8(){
    lighting_purple_min_off = 8;    
  }
  void setup_timer_time_min_9(){
    lighting_purple_min_off = 9;    
  }
  void setup_timer_time_min_10(){
    lighting_purple_min_off = 10;    
  }
  void setup_timer_time_min_11(){
    lighting_purple_min_off = 11;    
  }
  void setup_timer_time_min_12(){
    lighting_purple_min_off = 12;    
  }
  void setup_timer_time_min_12(){
    lighting_purple_min_off = 12;    
  }
  void setup_timer_time_min_13(){
    lighting_purple_min_off = 13;    
  }
  void setup_timer_time_min_14(){
    lighting_purple_min_off = 14;    
  }
  void setup_timer_time_min_15(){
    lighting_purple_min_off = 15;    
  }
  void setup_timer_time_min_16(){
    lighting_purple_min_off = 16;    
  }
  void setup_timer_time_min_17(){
    lighting_purple_min_off = 17;    
  }
  void setup_timer_time_min_18(){
    lighting_purple_min_off = 18;    
  }
  void setup_timer_time_min_19(){
    lighting_purple_min_off = 19;    
  }
  void setup_timer_time_min_20(){
    lighting_purple_min_off = 20;    
  }
  void setup_timer_time_min_21(){
    lighting_purple_min_off = 21;    
  }
  void setup_timer_time_min_22(){
    lighting_purple_min_off = 22;    
  }
  void setup_timer_time_min_23(){
    lighting_purple_min_off = 23;    
  }
  void setup_timer_time_min_24(){
    lighting_purple_min_off = 24;    
  }
  void setup_timer_time_min_25(){
    lighting_purple_min_off = 25;    
  }
  void setup_timer_time_min_26(){
    lighting_purple_min_off = 26;    
  }
  void setup_timer_time_min_27(){
    lighting_purple_min_off = 27;    
  }
  void setup_timer_time_min_28(){
    lighting_purple_min_off = 28;    
  }
  void setup_timer_time_min_29(){
    lighting_purple_min_off = 29;    
  }
  void setup_timer_time_min_30(){
    lighting_purple_min_off = 30;    
  }
  void setup_timer_time_min_31(){
    lighting_purple_min_off = 31;    
  }
  void setup_timer_time_min_32(){
    lighting_purple_min_off = 32;    
  }
  void setup_timer_time_min_33(){
    lighting_purple_min_off = 33;    
  }
  void setup_timer_time_min_34(){
    lighting_purple_min_off = 34;    
  }
  void setup_timer_time_min_35(){
    lighting_purple_min_off = 35;    
  }
  void setup_timer_time_min_36(){
    lighting_purple_min_off = 36;    
  }
  void setup_timer_time_min_37(){
    lighting_purple_min_off = 37;    
  }
  void setup_timer_time_min_38(){
    lighting_purple_min_off = 38;    
  }
  void setup_timer_time_min_39(){
    lighting_purple_min_off = 39;    
  }
  void setup_timer_time_min_40(){
    lighting_purple_min_off = 40;    
  }
   void setup_timer_time_min_41(){
    lighting_purple_min_off = 41;    
  }
  void setup_timer_time_min_42(){
    lighting_purple_min_off = 42;    
  }
  void setup_timer_time_min_43(){
    lighting_purple_min_off = 43;    
  }
  void setup_timer_time_min_44(){
    lighting_purple_min_off = 44;    
  }
  void setup_timer_time_min_45(){
    lighting_purple_min_off = 45;    
  }
  void setup_timer_time_min_46(){
    lighting_purple_min_off = 46;    
  }
  void setup_timer_time_min_47(){
    lighting_purple_min_off = 47;    
  }
  void setup_timer_time_min_48(){
    lighting_purple_min_off = 48;    
  }
  void setup_timer_time_min_49(){
    lighting_purple_min_off = 49;    
  }
  void setup_timer_time_min_50(){
    lighting_purple_min_off = 50;    
  }
 void setup_timer_time_min_51(){
    lighting_purple_min_off = 51;    
  }
  void setup_timer_time_min_52(){
    lighting_purple_min_off = 52;    
  }
  void setup_timer_time_min_53(){
    lighting_purple_min_off = 53;    
  }
  void setup_timer_time_min_54(){
    lighting_purple_min_off = 54;    
  }
  void setup_timer_time_min_55(){
    lighting_purple_min_off = 55;    
  }
  void setup_timer_time_min_56(){
    lighting_purple_min_off = 56;    
  }
  void setup_timer_time_min_57(){
    lighting_purple_min_off = 57;    
  }
  void setup_timer_time_min_58(){
    lighting_purple_min_off = 58;    
  }
  void setup_timer_time_min_59(){
    lighting_purple_min_off = 59;    
  }
  void setup_timer_time_min_60(){
    lighting_purple_min_off = 60;    
  }



    
  String f_lighting_hours_on(){
   return String(lighting_hours_on); 
      }
  String f_lighting_min_on(){
    return String(lighting_min_on);
    }
  String f_lighting_hours_off(){
   return String(lighting_hours_off); 
      }
  String f_lighting_min_off(){
    return String(lighting_min_off);
    }
  
    String f_lighting_purple_min_on(){
        return String(lighting_purple_min_on);      
          }
    String f_lighting_purple_min_off(){
        return String(lighting_purple_min_off);      
          }
  
    String f_air_min_off(){
      return String(air_min_off);
      }

     String f_water_out_min_off(){
      return String(water_out_min_off);
     }


     String f_food_hours_on_1(){
      return String(food_hours_on_1);
      }
   String f_food_hours_on_2(){
      return String(food_hours_on_2);
      }

    String f_food_min_on_1(){
      return String(food_min_on_1);
    }
    String f_food_min_on_2(){
          return String(food_min_on_2);
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
         state = 0;
      else
         state = 1;
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
         state = 0;
      else
         state = 1;
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
         state = 0;
      else
         state = 1;
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
         state = 0;
      else
         state = 1;
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
         state = 0;
      else
         state = 1;
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
         state = 0;
      else
         state = 1;
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




    //АКВАРИУМ ПРОЕКТ 

    #include <OneWire.h>                                      // для температуры
    #include <DallasTemperature.h>                            // для температуры
   
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
   // String AP       = "KVPTK_Guest";           // Здесь храним название точки доступа (роутера), к которой будем подключаться
   // String PASSWORD = "network15";       // Здесь храним пароль для подключения к точке доступа (роутеру)

    #include <iarduino_RTC.h>                                 // библиотека с часами RTC https://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/
    iarduino_RTC watch(RTC_DS3231);  
  
                                                             
    //  Определяем системное время:                           // Время загрузки скетча.
    const char* strM="JanFebMarAprMayJunJulAugSepOctNovDec";  // Определяем массив всех вариантов текстового представления текущего месяца.
    const char* sysT=__TIME__;                                // Получаем время компиляции скетча в формате "SS:MM:HH".
    const char* sysD=__DATE__;                                // Получаем дату  компиляции скетча в формате "MMM:DD:YYYY", где МММ - текстовое представление текущего месяца, например: Jul.
    //  Парсим полученные значения sysT и sysD в массив i:    // Определяем массив «i» из 6 элементов типа String, содержащий следующие значения: секунды, минуты, часы, день, месяц и год компиляции скетча.
    const int i[6] {(sysT[6]-48)*10+(sysT[7]-48), (sysT[3]-48)*10+(sysT[4]-48), (sysT[0]-48)*10+(sysT[1]-48), (sysD[4]-48)*10+(sysD[5]-48), ((int)memmem(strM,36,sysD,3)+3-(int)&strM[0])/3, (sysD[9]-48)*10+(sysD[10]-48)};

    
    const long utcOffsetInSeconds = 21600; // устанавливаем часовой пояс
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);//берем время с интернета
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
  
  
    
    
    const byte relayPin1 = 1;     // Пин к которому подключен реле Свет аквариума
    const byte relayPin2 = D4;     // Свет ультрафи 
    const byte relayPin3 = D5;     // Воздух к растениям
    const byte relayPin4 = D6;     // Вода из аквариума к растениям 
    const byte relayPin5 = D7;     // Вода от растений в аквариум
    const byte relayPin6 = D8;     // Кормушка рыб
    const byte relayPin7 = 3;     // Обогреватель 
     const byte relayPin8 = 16;     // Воздух к рыбам

    
    int relaySTATE1 = HIGH;         // состояние реле Свет аквариума
    int relaySTATE2 = HIGH;         // состояние реле ультрафи 
    int relaySTATE3 = HIGH;         // Воздух к растениям
    int relaySTATE4 = HIGH;         // Вода из аквариума к растениям 
    int relaySTATE5 = HIGH;         // Вода от растений в аквариум
    int relaySTATE6 = HIGH;         // Кормушка рыб
    int relaySTATE7 = HIGH;         // Обогреватель 
    int relaySTATE8 = HIGH;         // Воздух к растениям 
   
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

    int air_fish_min_on = 0;
    int air_fish_min_off = 20;
    
    int water_out_hours_on;           // Вода из аквариума к растениям и вода от растений в аквариум
    int water_out_min_on = 0;
    int water_out_hours_off;           // Вода из аквариума к растениям и вода от растений в аквариум
    int water_out_min_off = 15;
    
    
    int food_hours_on_1 = 8;                // Кормушка рыб таймер
    int food_min_on_1 = 0;
    int food_hours_on_2 = 18;                // Кормушка рыб таймер
    int food_min_on_2 = 0 ;
 
    float aquaTEMP = 29.00;        // Какая температура нужна в аквариуме


    ESP8266WebServer HTTP(80); 


    const int oneWireBus = D3;     // температура подключен к пину
    OneWire oneWire(oneWireBus);
    DallasTemperature sensors(&oneWire);
    
void setup()

{
    Serial.begin(9600);                                  // Инициируем передачу данных в монитор последовательного порта
    
    WiFi.mode(WIFI_STA);                        // Определяем режим работы Wi-Fi модуля в режиме клиента
    WiFi.begin(AP.c_str(), PASSWORD.c_str());   // Подключаемся к точке доступа (роутеру)
    HTTP.begin();                               // Инициализируем Web-сервер
    SPIFFS.begin();                             // Инициализируем файловую систему
    SSDP_init();                                 // Вызываем функцию инициализации SSDP (функция описана ниже)

    sensors.begin(); // температура
    watch.begin();                                        // Инициируем RTC модуль
    
    pinMode(relayPin1, OUTPUT);               // Указываем вывод RELAY как выход //cвет в аквариуме
    pinMode(relayPin2, OUTPUT);               // Свет ультрафи 
    pinMode(relayPin3, OUTPUT);               // Воздух к растениям
    pinMode(relayPin4, OUTPUT);               // Вода из аквариума к растениям 
    pinMode(relayPin5, OUTPUT);               // Вода от растений в аквариум
    pinMode(relayPin6, OUTPUT);               // Кормушка рыб
    pinMode(relayPin7, OUTPUT);               // Обогреватель
    pinMode(relayPin8, OUTPUT);               // Воздух к рыбам

    
    digitalWrite(relayPin1, HIGH); 
    digitalWrite(relayPin2, HIGH);  
    digitalWrite(relayPin3, HIGH);  
    digitalWrite(relayPin4, HIGH);  
    digitalWrite(relayPin5, HIGH);  
    digitalWrite(relayPin6, HIGH);   
    digitalWrite(relayPin7, HIGH);  
    digitalWrite(relayPin8, HIGH);  
   
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

       HTTP.on("/aqua_temp", [] (){
       HTTP.send(200, "text/plain", aqua_temp()); 
        });
        
   HTTP.on("/get_aqua_temp", [] (){
       HTTP.send(200, "text/plain", get_aqua_temp()); 
        });
        
     HTTP.on("/status_time", [] (){
      HTTP.send(200, "text/plain", status_time()); 
    });

     HTTP.on("/relay_switch_air_fish", [] (){                                   // обогреватель
      HTTP.send(200, "text/plain", relay_switch_air_fish()); 
    });
    HTTP.on("/relay_status_air_fish", [] (){
      HTTP.send(200, "text/plain", relay_status_air_fish()); 
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

    


    HTTP.on("/setup_timer_time_min_1_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_1_purple()); 
        });
     HTTP.on("/setup_timer_time_min_2_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_2_purple()); 
        });
         HTTP.on("/setup_timer_time_min_3_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_3_purple()); 
        });
         HTTP.on("/setup_timer_time_min_4_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_4_purple()); 
        });
         HTTP.on("/setup_timer_time_min_5_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_5_purple()); 
        });
         HTTP.on("/setup_timer_time_min_6_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_6_purple()); 
        });
         HTTP.on("/setup_timer_time_min_7_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_7_purple()); 
        });
         HTTP.on("/setup_timer_time_min_8_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_8_purple()); 
        });
         HTTP.on("/setup_timer_time_min_9_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_9_purple()); 
        });
        HTTP.on("/setup_timer_time_min_10_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_10_purple()); 
        });
        HTTP.on("/setup_timer_time_min_11_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_11_purple()); 
        });
        HTTP.on("/setup_timer_time_min_12_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_12_purple()); 
        });
        HTTP.on("/setup_timer_time_min_13_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_13_purple()); 
        });
        HTTP.on("/setup_timer_time_min_14_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_14_purple()); 
        });
        HTTP.on("/setup_timer_time_min_15_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_15_purple()); 
        });
        HTTP.on("/setup_timer_time_min_16_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_16_purple()); 
        });
        HTTP.on("/setup_timer_time_min_17_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_17_purple()); 
        });
        HTTP.on("/setup_timer_time_min_18_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_18_purple()); 
        });
        HTTP.on("/setup_timer_time_min_19_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_19_purple()); 
        });
        HTTP.on("/setup_timer_time_min_20_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_20_purple()); 
        });
        HTTP.on("/setup_timer_time_min_21_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_21_purple()); 
        });
        HTTP.on("/setup_timer_time_min_22_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_22_purple()); 
        });
        HTTP.on("/setup_timer_time_min_23_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_23_purple()); 
        });
        HTTP.on("/setup_timer_time_min_24_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_24_purple()); 
        });
        HTTP.on("/setup_timer_time_min_25_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_25_purple()); 
        });
        HTTP.on("/setup_timer_time_min_26_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_26_purple()); 
        });
        HTTP.on("/setup_timer_time_min_27_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_27_purple()); 
        });
        HTTP.on("/setup_timer_time_min_28_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_28_purple()); 
        });
         HTTP.on("/setup_timer_time_min_29_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_29_purple()); 
        });
        HTTP.on("/setup_timer_time_min_30_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_30_purple()); 
        });
         HTTP.on("/setup_timer_time_min_31_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_31_purple()); 
        });
        HTTP.on("/setup_timer_time_min_32_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_32_purple()); 
        });
         HTTP.on("/setup_timer_time_min_33_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_33_purple()); 
        });
        HTTP.on("/setup_timer_time_min_34_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_34_purple()); 
        });
         HTTP.on("/setup_timer_time_min_35_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_35_purple()); 
        });
        HTTP.on("/setup_timer_time_min_36_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_36_purple()); 
        });
         HTTP.on("/setup_timer_time_min_37_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_37_purple()); 
        });
        HTTP.on("/setup_timer_time_min_38_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_38_purple()); 
        });
         HTTP.on("/setup_timer_time_min_39_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_39_purple()); 
        });
        HTTP.on("/setup_timer_time_min_40_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_40_purple()); 
        });
         HTTP.on("/setup_timer_time_min_41_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_41_purple()); 
        });
        HTTP.on("/setup_timer_time_min_42_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_42_purple()); 
        });
         HTTP.on("/setup_timer_time_min_43_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_43_purple()); 
        });
        HTTP.on("/setup_timer_time_min_44_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_44_purple()); 
        });
         HTTP.on("/setup_timer_time_min_45_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_45_purple()); 
        });
        HTTP.on("/setup_timer_time_min_46_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_47_purple()); 
        });
         HTTP.on("/setup_timer_time_min_48_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_48_purple()); 
        });
        HTTP.on("/setup_timer_time_min_49_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_49_purple()); 
        });
         HTTP.on("/setup_timer_time_min_50_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_50_purple()); 
        });
        HTTP.on("/setup_timer_time_min_51_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_51_purple()); 
        });
         HTTP.on("/setup_timer_time_min_52_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_52_purple()); 
        });
        HTTP.on("/setup_timer_time_min_53_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_53_purple()); 
        });
        HTTP.on("/setup_timer_time_min_54_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_54_purple()); 
        });
        HTTP.on("/setup_timer_time_min_55_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_55_purple()); 
        });
        HTTP.on("/setup_timer_time_min_56_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_56_purple()); 
        });
        HTTP.on("/setup_timer_time_min_57_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_57_purple()); 
        });
        HTTP.on("/setup_timer_time_min_58_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_58_purple()); 
        });
         HTTP.on("/setup_timer_time_min_59_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_59_purple()); 
        });
        HTTP.on("/setup_timer_time_min_60_purple", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_60_purple()); 
        });





          HTTP.on("/setup_timer_time_min_1_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_1_air()); 
        });
     HTTP.on("/setup_timer_time_min_2_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_2_air()); 
        });
         HTTP.on("/setup_timer_time_min_3_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_3_air()); 
        });
         HTTP.on("/setup_timer_time_min_4_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_4_air()); 
        });
         HTTP.on("/setup_timer_time_min_5_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_5_air()); 
        });
         HTTP.on("/setup_timer_time_min_6_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_6_air()); 
        });
         HTTP.on("/setup_timer_time_min_7_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_7_air()); 
        });
         HTTP.on("/setup_timer_time_min_8_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_8_air()); 
        });
         HTTP.on("/setup_timer_time_min_9_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_9_air()); 
        });
        HTTP.on("/setup_timer_time_min_10_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_10_air()); 
        });
        HTTP.on("/setup_timer_time_min_11_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_11_air()); 
        });
        HTTP.on("/setup_timer_time_min_12_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_12_air()); 
        });
        HTTP.on("/setup_timer_time_min_13_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_13_air()); 
        });
        HTTP.on("/setup_timer_time_min_14_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_14_air()); 
        });
        HTTP.on("/setup_timer_time_min_15_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_15_air()); 
        });
        HTTP.on("/setup_timer_time_min_16_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_16_air()); 
        });
        HTTP.on("/setup_timer_time_min_17_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_17_air()); 
        });
        HTTP.on("/setup_timer_time_min_18_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_18_air()); 
        });
        HTTP.on("/setup_timer_time_min_19_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_19_air()); 
        });
        HTTP.on("/setup_timer_time_min_20_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_20_air()); 
        });
        HTTP.on("/setup_timer_time_min_21_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_21_air()); 
        });
        HTTP.on("/setup_timer_time_min_22_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_22_air()); 
        });
        HTTP.on("/setup_timer_time_min_23_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_23_air()); 
        });
        HTTP.on("/setup_timer_time_min_24_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_24_air()); 
        });
        HTTP.on("/setup_timer_time_min_25_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_25_air()); 
        });
        HTTP.on("/setup_timer_time_min_26_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_26_air()); 
        });
        HTTP.on("/setup_timer_time_min_27_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_27_air()); 
        });
        HTTP.on("/setup_timer_time_min_28_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_28_air()); 
        });
         HTTP.on("/setup_timer_time_min_29_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_29_air()); 
        });
        HTTP.on("/setup_timer_time_min_30_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_30_air()); 
        });
         HTTP.on("/setup_timer_time_min_31_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_31_air()); 
        });
        HTTP.on("/setup_timer_time_min_32_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_32_air()); 
        });
         HTTP.on("/setup_timer_time_min_33_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_33_air()); 
        });
        HTTP.on("/setup_timer_time_min_34_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_34_air()); 
        });
         HTTP.on("/setup_timer_time_min_35_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_35_air()); 
        });
        HTTP.on("/setup_timer_time_min_36_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_36_air()); 
        });
         HTTP.on("/setup_timer_time_min_37_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_37_air()); 
        });
        HTTP.on("/setup_timer_time_min_38_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_38_air()); 
        });
         HTTP.on("/setup_timer_time_min_39_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_39_air()); 
        });
        HTTP.on("/setup_timer_time_min_40_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_40_air()); 
        });
         HTTP.on("/setup_timer_time_min_41_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_41_air()); 
        });
        HTTP.on("/setup_timer_time_min_42_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_42_air()); 
        });
         HTTP.on("/setup_timer_time_min_43_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_43_air()); 
        });
        HTTP.on("/setup_timer_time_min_44_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_44_air()); 
        });
         HTTP.on("/setup_timer_time_min_45_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_45_air()); 
        });
        HTTP.on("/setup_timer_time_min_46_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_47_air()); 
        });
         HTTP.on("/setup_timer_time_min_48_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_48_air()); 
        });
        HTTP.on("/setup_timer_time_min_49_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_49_air()); 
        });
         HTTP.on("/setup_timer_time_min_50_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_50_air()); 
        });
        HTTP.on("/setup_timer_time_min_51_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_51_air()); 
        });
         HTTP.on("/setup_timer_time_min_52_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_52_air()); 
        });
        HTTP.on("/setup_timer_time_min_53_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_53_air()); 
        });
        HTTP.on("/setup_timer_time_min_54_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_54_air()); 
        });
        HTTP.on("/setup_timer_time_min_55_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_55_air()); 
        });
        HTTP.on("/setup_timer_time_min_56_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_56_air()); 
        });
        HTTP.on("/setup_timer_time_min_57_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_57_air()); 
        });
        HTTP.on("/setup_timer_time_min_58_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_58_air()); 
        });
         HTTP.on("/setup_timer_time_min_59_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_59_air()); 
        });
        HTTP.on("/setup_timer_time_min_60_air", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_60_air()); 
        });




         HTTP.on("/setup_timer_time_min_1_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_1_water()); 
        });
     HTTP.on("/setup_timer_time_min_2_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_2_water()); 
        });
         HTTP.on("/setup_timer_time_min_3_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_3_water()); 
        });
         HTTP.on("/setup_timer_time_min_4_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_4_water()); 
        });
         HTTP.on("/setup_timer_time_min_5_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_5_water()); 
        });
         HTTP.on("/setup_timer_time_min_6_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_6_water()); 
        });
         HTTP.on("/setup_timer_time_min_7_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_7_water()); 
        });
         HTTP.on("/setup_timer_time_min_8_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_8_water()); 
        });
         HTTP.on("/setup_timer_time_min_9_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_9_water()); 
        });
        HTTP.on("/setup_timer_time_min_10_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_10_water()); 
        });
        HTTP.on("/setup_timer_time_min_11_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_11_water()); 
        });
        HTTP.on("/setup_timer_time_min_12_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_12_water()); 
        });
        HTTP.on("/setup_timer_time_min_13_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_13_water()); 
        });
        HTTP.on("/setup_timer_time_min_14_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_14_water()); 
        });
        HTTP.on("/setup_timer_time_min_15_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_15_water()); 
        });
        HTTP.on("/setup_timer_time_min_16_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_16_water()); 
        });
        HTTP.on("/setup_timer_time_min_17_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_17_water()); 
        });
        HTTP.on("/setup_timer_time_min_18_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_18_water()); 
        });
        HTTP.on("/setup_timer_time_min_19_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_19_water()); 
        });
        HTTP.on("/setup_timer_time_min_20_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_20_water()); 
        });
        HTTP.on("/setup_timer_time_min_21_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_21_water()); 
        });
        HTTP.on("/setup_timer_time_min_22_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_22_water()); 
        });
        HTTP.on("/setup_timer_time_min_23_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_23_water()); 
        });
        HTTP.on("/setup_timer_time_min_24_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_24_water()); 
        });
        HTTP.on("/setup_timer_time_min_25_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_25_water()); 
        });
        HTTP.on("/setup_timer_time_min_26_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_26_water()); 
        });
        HTTP.on("/setup_timer_time_min_27_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_27_water()); 
        });
        HTTP.on("/setup_timer_time_min_28_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_28_water()); 
        });
         HTTP.on("/setup_timer_time_min_29_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_29_water()); 
        });
        HTTP.on("/setup_timer_time_min_30_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_30_water()); 
        });
         HTTP.on("/setup_timer_time_min_31_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_31_water()); 
        });
        HTTP.on("/setup_timer_time_min_32_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_32_water()); 
        });
         HTTP.on("/setup_timer_time_min_33_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_33_water()); 
        });
        HTTP.on("/setup_timer_time_min_34_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_34_water()); 
        });
         HTTP.on("/setup_timer_time_min_35_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_35_water()); 
        });
        HTTP.on("/setup_timer_time_min_36_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_36_water()); 
        });
         HTTP.on("/setup_timer_time_min_37_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_37_water()); 
        });
        HTTP.on("/setup_timer_time_min_38_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_38_water()); 
        });
         HTTP.on("/setup_timer_time_min_39_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_39_water()); 
        });
        HTTP.on("/setup_timer_time_min_40_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_40_water()); 
        });
         HTTP.on("/setup_timer_time_min_41_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_41_water()); 
        });
        HTTP.on("/setup_timer_time_min_42_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_42_water()); 
        });
         HTTP.on("/setup_timer_time_min_43_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_43_water()); 
        });
        HTTP.on("/setup_timer_time_min_44_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_44_water()); 
        });
         HTTP.on("/setup_timer_time_min_45_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_45_water()); 
        });
        HTTP.on("/setup_timer_time_min_46_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_47_water()); 
        });
         HTTP.on("/setup_timer_time_min_48_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_48_water()); 
        });
        HTTP.on("/setup_timer_time_min_49_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_49_water()); 
        });
         HTTP.on("/setup_timer_time_min_50_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_50_water()); 
        });
        HTTP.on("/setup_timer_time_min_51_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_51_water()); 
        });
         HTTP.on("/setup_timer_time_min_52_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_52_water()); 
        });
        HTTP.on("/setup_timer_time_min_53_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_53_water()); 
        });
        HTTP.on("/setup_timer_time_min_54_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_54_water()); 
        });
        HTTP.on("/setup_timer_time_min_55_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_55_water()); 
        });
        HTTP.on("/setup_timer_time_min_56_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_56_water()); 
        });
        HTTP.on("/setup_timer_time_min_57_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_57_water()); 
        });
        HTTP.on("/setup_timer_time_min_58_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_58_water()); 
        });
         HTTP.on("/setup_timer_time_min_59_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_59_water()); 
        });
        HTTP.on("/setup_timer_time_min_60_water", [] (){
          HTTP.send(200, "text/plain", setup_timer_time_min_60_water()); 
        });


/*
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

*/

         HTTP.on("/setup_temp_10", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_10()); 
        });
         HTTP.on("/setup_temp_11", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_11()); 
        });
         HTTP.on("/setup_temp_12", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_12()); 
        });
         HTTP.on("/setup_temp_13", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_13()); 
        });
         HTTP.on("/setup_temp_14", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_14()); 
        });
         HTTP.on("/setup_temp_15", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_15()); 
        });
         HTTP.on("/setup_temp_16", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_16()); 
        });
         HTTP.on("/setup_temp_17", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_17()); 
        });
         HTTP.on("/setup_temp_18", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_18()); 
        });
         HTTP.on("/setup_temp_19", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_19()); 
        });
         HTTP.on("/setup_temp_20", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_20()); 
        });
         HTTP.on("/setup_temp_21", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_21()); 
        });
         HTTP.on("/setup_temp_22", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_22()); 
        });
         HTTP.on("/setup_temp_23", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_23()); 
        });
          HTTP.on("/setup_temp_24", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_24()); 
        });
         HTTP.on("/setup_temp_25", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_25()); 
        });
         HTTP.on("/setup_temp_26", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_26()); 
        });
         HTTP.on("/setup_temp_27", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_27()); 
        });
         HTTP.on("/setup_temp_28", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_28()); 
        });
         HTTP.on("/setup_temp_29", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_29()); 
        });
         HTTP.on("/setup_temp_30", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_30()); 
        });
          HTTP.on("/setup_temp_31", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_31()); 
        });
         HTTP.on("/setup_temp_32", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_32()); 
        });
         HTTP.on("/setup_temp_33", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_33()); 
        });
         HTTP.on("/setup_temp_34", [] (){
          HTTP.send(200, "text/plain", setup_temp_c_34()); 
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
    
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    
    
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
    if(watch.Hours >= lighting_purple_hours_on && watch.minutes == lighting_purple_min_on && watch.Hours <= lighting_purple_hours_off){
     digitalWrite(relayPin2, LOW);   
     relaySTATE2 = LOW;     
    }                                                         //Свет ультрафи 
      //выключение ультрафи 
    if(watch.Hours >= lighting_purple_hours_on && watch.minutes == lighting_purple_min_off && watch.Hours <= lighting_purple_hours_off){
     digitalWrite(relayPin2, HIGH);   
     relaySTATE2 = HIGH;   
    }
    if(watch.Hours >= lighting_purple_hours_off && relaySTATE2 == LOW ){
     digitalWrite(relayPin2, HIGH);   
     relaySTATE2 = HIGH;  
    }
  
    //Воздух к растениям
    if(watch.minutes >= air_min_on && watch.minutes <= air_min_off ){
      digitalWrite(relayPin3, LOW);   
      relaySTATE3 = LOW;  
    }
    if(watch.minutes >= air_min_off ){
      digitalWrite(relayPin3, HIGH);   
      relaySTATE3 = HIGH;  
    }
    
    //Вода из аквариума к растениям и вода от растений к аквариуму
    if(watch.Hours >= 8 && watch.minutes >= water_out_min_on && watch.Hours < 20 ){
     digitalWrite(relayPin4, LOW);   
     digitalWrite(relayPin5, LOW);   
     relaySTATE4 = LOW;     
     relaySTATE5 = LOW;     
    }                                                        
    if(watch.Hours >= 8 && watch.minutes >= water_out_min_off && watch.Hours < 20 ){
     digitalWrite(relayPin4, HIGH);   
     digitalWrite(relayPin5, HIGH);   
     relaySTATE4 = HIGH;     
     relaySTATE5 = HIGH;   
    }
    /*
    if(watch.Hours >= 20 ){
     digitalWrite(relayPin4, HIGH);   
     digitalWrite(relayPin5, HIGH);   
     relaySTATE4 = HIGH;     
     relaySTATE5 = HIGH;   
    }
    */
  
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
  

   // Температура на сервер
    String aqua_temp(){
    float temperatureC = sensors.getTempCByIndex(0);        // получение температуры 
      return String(temperatureC);
    }


   //получение времени
   String status_time(){  
     String time_aqua = watch.gettime("H:i") ;
     return String(time_aqua);
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






  

   String setup_temp_c_10(){
    aquaTEMP = 10;
    return String(1);
    }

   String setup_temp_c_11(){
    aquaTEMP = 11;
    return String(1);
    }

    String setup_temp_c_12(){
    aquaTEMP = 12;
    return String(1);
    }

   String setup_temp_c_13(){
    aquaTEMP = 13;
    return String(1);
    }

    String setup_temp_c_14(){
    aquaTEMP = 14;
    return String(1);
    }

   String setup_temp_c_15(){
    aquaTEMP = 15;
    return String(1);
    }

String setup_temp_c_16(){
    aquaTEMP = 16;
    return String(1);
    }

   String setup_temp_c_17(){
    aquaTEMP = 17;
    return String(1);
    }

String setup_temp_c_18(){
    aquaTEMP = 18;
    return String(1);
    }

   String setup_temp_c_19(){
    aquaTEMP = 19;
    return String(1);
    }

String setup_temp_c_20(){
    aquaTEMP = 20;
    return String(1);
    }

   String setup_temp_c_21(){
    aquaTEMP = 21;
    return String(1);
    }

String setup_temp_c_22(){
    aquaTEMP = 22;
    return String(1);
    }

   String setup_temp_c_23(){
    aquaTEMP = 23;
    return String(1);
    }

   String setup_temp_c_24(){
    aquaTEMP = 24;
    return String(1);
    }

String setup_temp_c_25(){
    aquaTEMP = 25;
    return String(1);
    }

   String setup_temp_c_26(){
    aquaTEMP = 26;
    return String(1);
    }
 String setup_temp_c_27(){
    aquaTEMP = 27;
    return String(1);
    }

String setup_temp_c_28(){
    aquaTEMP = 28;
    return String(1);
    }

   String setup_temp_c_29(){
    aquaTEMP = 29;
    return String(1);
    }
String setup_temp_c_30(){
    aquaTEMP = 30;
    return String(1);
    }

   String setup_temp_c_31(){
    aquaTEMP = 31;
    return String(1);
    }
 String setup_temp_c_32(){
    aquaTEMP = 32;
    return String(1);
    }

   String setup_temp_c_33(){
    aquaTEMP = 33;
    return String(1);
    }     
   String setup_temp_c_34(){
    aquaTEMP = 34;
    return String(1);
    }     
   


  String get_aqua_temp(){
    return String(aquaTEMP);
    }










  String setup_timer_time_min_1_purple(){
    lighting_purple_min_off = 1;   
    return String(1);  
  }
  String setup_timer_time_min_2_purple(){
    lighting_purple_min_off = 2;   
    return String(1);  
  }
  String setup_timer_time_min_3_purple(){
    lighting_purple_min_off = 3;
    return String(1);  
  }
  String setup_timer_time_min_4_purple(){
    lighting_purple_min_off = 4;    
    return String(1); 
  }
  String setup_timer_time_min_5_purple(){
    lighting_purple_min_off = 5;    
    return String(1); 
  }
  String setup_timer_time_min_6_purple(){
    lighting_purple_min_off = 6;    
    return String(1); 
  }
  String setup_timer_time_min_7_purple(){
    lighting_purple_min_off = 7;
    return String(1);   
  }
  String setup_timer_time_min_8_purple(){
    lighting_purple_min_off = 8;  
    return String(1);   
  }
  String setup_timer_time_min_9_purple(){
    lighting_purple_min_off = 9;    
    return String(1); 
  }
  String setup_timer_time_min_10_purple(){
    lighting_purple_min_off = 10;    
    return String(1); 
  }
  String setup_timer_time_min_11_purple(){
    lighting_purple_min_off = 11;    
    return String(1); 
  }
  String setup_timer_time_min_12_purple(){
    lighting_purple_min_off = 12;    
    return String(1); 
  }
  String setup_timer_time_min_13_purple(){
    lighting_purple_min_off = 13;    
    return String(1); 
  }
  String setup_timer_time_min_14_purple(){
    lighting_purple_min_off = 14;
    return String(1);     
  }
  String setup_timer_time_min_15_purple(){
    lighting_purple_min_off = 15;   
    return String(1); 
  }
  String setup_timer_time_min_16_purple(){
    lighting_purple_min_off = 16;
    return String(1);     
  }
  String setup_timer_time_min_17_purple(){
    lighting_purple_min_off = 17;    
    return String(1); 
  }
  String setup_timer_time_min_18_purple(){
    lighting_purple_min_off = 18;    
    return String(1); 
  }
  String setup_timer_time_min_19_purple(){
    lighting_purple_min_off = 19;    
    return String(1); 
  }
  String setup_timer_time_min_20_purple(){
    lighting_purple_min_off = 20;    
    return String(1); 
  }
  String setup_timer_time_min_21_purple(){
    lighting_purple_min_off = 21;   
    return String(1);  
  }
  String setup_timer_time_min_22_purple(){
    lighting_purple_min_off = 22;  
    return String(1);   
  }
  String setup_timer_time_min_23_purple(){
    lighting_purple_min_off = 23;   
    return String(1);  
  }
  String setup_timer_time_min_24_purple(){
    lighting_purple_min_off = 24; 
    return String(1);    
  }
  String setup_timer_time_min_25_purple(){
    lighting_purple_min_off = 25; 
    return String(1);    
  }
  String setup_timer_time_min_26_purple(){
    lighting_purple_min_off = 26;   
    return String(1);  
  }
  String setup_timer_time_min_27_purple(){
    lighting_purple_min_off = 27; 
    return String(1);    
  }
  String setup_timer_time_min_28_purple(){
    lighting_purple_min_off = 28;    
    return String(1); 
  }
  String setup_timer_time_min_29_purple(){
    lighting_purple_min_off = 29;
    return String(1);     
  }
  String setup_timer_time_min_30_purple(){
    lighting_purple_min_off = 30;    
    return String(1); 
  }
  String setup_timer_time_min_31_purple(){
    lighting_purple_min_off = 31;    
    return String(1); 
  }
  String setup_timer_time_min_32_purple(){
    lighting_purple_min_off = 32;    
    return String(1); 
  }
  String setup_timer_time_min_33_purple(){
    lighting_purple_min_off = 33;    
    return String(1); 
  }
  String setup_timer_time_min_34_purple(){
    lighting_purple_min_off = 34;    
    return String(1); 
  }
  String setup_timer_time_min_35_purple(){
    lighting_purple_min_off = 35;    
    return String(1); 
  }
  String setup_timer_time_min_36_purple(){
    lighting_purple_min_off = 36;    
    return String(1); 
  }
  String setup_timer_time_min_37_purple(){
    lighting_purple_min_off = 37;    
    return String(1); 
  }
  String setup_timer_time_min_38_purple(){
    lighting_purple_min_off = 38;    
    return String(1); 
  }
  String setup_timer_time_min_39_purple(){
    lighting_purple_min_off = 39;    
    return String(1); 
  }
  String setup_timer_time_min_40_purple(){
    lighting_purple_min_off = 40;    
    return String(1); 
  }
   String setup_timer_time_min_41_purple(){
    lighting_purple_min_off = 41;    
    return String(1); 
  }
  String setup_timer_time_min_42_purple(){
    lighting_purple_min_off = 42;    
    return String(1); 
  }
  String setup_timer_time_min_43_purple(){
    lighting_purple_min_off = 43;    
    return String(1); 
  }
  String setup_timer_time_min_44_purple(){
    lighting_purple_min_off = 44;    
    return String(1); 
  }
  String setup_timer_time_min_45_purple(){
    lighting_purple_min_off = 45;
    return String(1);     
  }
  String setup_timer_time_min_46_purple(){
    lighting_purple_min_off = 46;    
    return String(1); 
  }
  String setup_timer_time_min_47_purple(){
    lighting_purple_min_off = 47;    
    return String(1); 
  }
  String setup_timer_time_min_48_purple(){
    lighting_purple_min_off = 48;    
    return String(1); 
  }
  String setup_timer_time_min_49_purple(){
    lighting_purple_min_off = 49;    
    return String(1); 
  }
  String setup_timer_time_min_50_purple(){
    lighting_purple_min_off = 50;    
    return String(1); 
  }
 String setup_timer_time_min_51_purple(){
    lighting_purple_min_off = 51;    
    return String(1); 
  }
  String setup_timer_time_min_52_purple(){
    lighting_purple_min_off = 52;    
    return String(1); 
  }
  String setup_timer_time_min_53_purple(){
    lighting_purple_min_off = 53;   
    return String(1);  
  }
  String setup_timer_time_min_54_purple(){
    lighting_purple_min_off = 54;    
    return String(1); 
  }
  String setup_timer_time_min_55_purple(){
    lighting_purple_min_off = 55;    
    return String(1); 
  }
  String setup_timer_time_min_56_purple(){
    lighting_purple_min_off = 56;    
    return String(1); 
  }
  String setup_timer_time_min_57_purple(){
    lighting_purple_min_off = 57;    
    return String(1); 
  }
  String setup_timer_time_min_58_purple(){
    lighting_purple_min_off = 58;    
    return String(1); 
  }
  String setup_timer_time_min_59_purple(){
    lighting_purple_min_off = 59;    
    return String(1); 
  }
  String setup_timer_time_min_60_purple(){
    lighting_purple_min_off = 60;    
    return String(1); 
  }






  String setup_timer_time_min_1_air(){
    air_min_off = 1;   
    return String(1);  
  }
  String setup_timer_time_min_2_air(){
    air_min_off = 2;   
    return String(1);  
  }
  String setup_timer_time_min_3_air(){
    air_min_off = 3;
    return String(1);  
  }
  String setup_timer_time_min_4_air(){
    air_min_off = 4;    
    return String(1); 
  }
  String setup_timer_time_min_5_air(){
    air_min_off = 5;    
    return String(1); 
  }
  String setup_timer_time_min_6_air(){
    air_min_off = 6;    
    return String(1); 
  }
  String setup_timer_time_min_7_air(){
    air_min_off = 7;
    return String(1);   
  }
  String setup_timer_time_min_8_air(){
    air_min_off = 8;  
    return String(1);   
  }
  String setup_timer_time_min_9_air(){
    air_min_off = 9;    
    return String(1); 
  }
  String setup_timer_time_min_10_air(){
    air_min_off = 10;    
    return String(1); 
  }
  String setup_timer_time_min_11_air(){
    air_min_off = 11;    
    return String(1); 
  }
  String setup_timer_time_min_12_air(){
    air_min_off = 12;    
    return String(1); 
  }
  String setup_timer_time_min_13_air(){
    air_min_off = 13;    
    return String(1); 
  }
  String setup_timer_time_min_14_air(){
    air_min_off = 14;
    return String(1);     
  }
  String setup_timer_time_min_15_air(){
    air_min_off = 15;   
    return String(1); 
  }
  String setup_timer_time_min_16_air(){
    air_min_off = 16;
    return String(1);     
  }
  String setup_timer_time_min_17_air(){
    air_min_off = 17;    
    return String(1); 
  }
  String setup_timer_time_min_18_air(){
    air_min_off = 18;    
    return String(1); 
  }
  String setup_timer_time_min_19_air(){
    air_min_off = 19;    
    return String(1); 
  }
  String setup_timer_time_min_20_air(){
    air_min_off = 20;    
    return String(1); 
  }
  String setup_timer_time_min_21_air(){
    air_min_off = 21;   
    return String(1);  
  }
  String setup_timer_time_min_22_air(){
    air_min_off = 22;  
    return String(1);   
  }
  String setup_timer_time_min_23_air(){
    air_min_off = 23;   
    return String(1);  
  }
  String setup_timer_time_min_24_air(){
    air_min_off = 24; 
    return String(1);    
  }
  String setup_timer_time_min_25_air(){
    air_min_off = 25; 
    return String(1);    
  }
  String setup_timer_time_min_26_air(){
    air_min_off = 26;   
    return String(1);  
  }
  String setup_timer_time_min_27_air(){
    air_min_off = 27; 
    return String(1);    
  }
  String setup_timer_time_min_28_air(){
    air_min_off = 28;    
    return String(1); 
  }
  String setup_timer_time_min_29_air(){
    air_min_off = 29;
    return String(1);     
  }
  String setup_timer_time_min_30_air(){
    air_min_off = 30;    
    return String(1); 
  }
  String setup_timer_time_min_31_air(){
    air_min_off = 31;    
    return String(1); 
  }
  String setup_timer_time_min_32_air(){
    air_min_off = 32;    
    return String(1); 
  }
  String setup_timer_time_min_33_air(){
    air_min_off = 33;    
    return String(1); 
  }
  String setup_timer_time_min_34_air(){
    air_min_off = 34;    
    return String(1); 
  }
  String setup_timer_time_min_35_air(){
    air_min_off = 35;    
    return String(1); 
  }
  String setup_timer_time_min_36_air(){
    air_min_off = 36;    
    return String(1); 
  }
  String setup_timer_time_min_37_air(){
    air_min_off = 37;    
    return String(1); 
  }
  String setup_timer_time_min_38_air(){
    air_min_off = 38;    
    return String(1); 
  }
  String setup_timer_time_min_39_air(){
    air_min_off = 39;    
    return String(1); 
  }
  String setup_timer_time_min_40_air(){
    air_min_off = 40;    
    return String(1); 
  }
   String setup_timer_time_min_41_air(){
    air_min_off = 41;    
    return String(1); 
  }
  String setup_timer_time_min_42_air(){
    air_min_off = 42;    
    return String(1); 
  }
  String setup_timer_time_min_43_air(){
    air_min_off = 43;    
    return String(1); 
  }
  String setup_timer_time_min_44_air(){
    air_min_off = 44;    
    return String(1); 
  }
  String setup_timer_time_min_45_air(){
    air_min_off = 45;
    return String(1);     
  }
  String setup_timer_time_min_46_air(){
    air_min_off = 46;    
    return String(1); 
  }
  String setup_timer_time_min_47_air(){
    air_min_off = 47;    
    return String(1); 
  }
  String setup_timer_time_min_48_air(){
    air_min_off = 48;    
    return String(1); 
  }
  String setup_timer_time_min_49_air(){
    air_min_off = 49;    
    return String(1); 
  }
  String setup_timer_time_min_50_air(){
    air_min_off = 50;    
    return String(1); 
  }
 String setup_timer_time_min_51_air(){
    air_min_off = 51;    
    return String(1); 
  }
  String setup_timer_time_min_52_air(){
    air_min_off = 52;    
    return String(1); 
  }
  String setup_timer_time_min_53_air(){
    air_min_off = 53;   
    return String(1);  
  }
  String setup_timer_time_min_54_air(){
    air_min_off = 54;    
    return String(1); 
  }
  String setup_timer_time_min_55_air(){
    air_min_off = 55;    
    return String(1); 
  }
  String setup_timer_time_min_56_air(){
    air_min_off = 56;    
    return String(1); 
  }
  String setup_timer_time_min_57_air(){
    air_min_off = 57;    
    return String(1); 
  }
  String setup_timer_time_min_58_air(){
    air_min_off = 58;    
    return String(1); 
  }
  String setup_timer_time_min_59_air(){
    air_min_off = 59;    
    return String(1); 
  }
  String setup_timer_time_min_60_air(){
    air_min_off = 60;    
    return String(1); 
  }

  String setup_timer_time_min_1_water(){
    water_out_min_off = 1;   
    return String(1);  
  }
  String setup_timer_time_min_2_water(){
    water_out_min_off = 2;   
    return String(1);  
  }
  String setup_timer_time_min_3_water(){
    water_out_min_off = 3;
    return String(1);  
  }
  String setup_timer_time_min_4_water(){
    water_out_min_off = 4;    
    return String(1); 
  }
  String setup_timer_time_min_5_water(){
    water_out_min_off = 5;    
    return String(1); 
  }
  String setup_timer_time_min_6_water(){
    water_out_min_off = 6;    
    return String(1); 
  }
  String setup_timer_time_min_7_water(){
    water_out_min_off = 7;
    return String(1);   
  }
  String setup_timer_time_min_8_water(){
    water_out_min_off = 8;  
    return String(1);   
  }
  String setup_timer_time_min_9_water(){
    water_out_min_off = 9;    
    return String(1); 
  }
  String setup_timer_time_min_10_water(){
    water_out_min_off = 10;    
    return String(1); 
  }
  String setup_timer_time_min_11_water(){
    water_out_min_off = 11;    
    return String(1); 
  }
  String setup_timer_time_min_12_water(){
    water_out_min_off = 12;    
    return String(1); 
  }
  String setup_timer_time_min_13_water(){
    water_out_min_off = 13;    
    return String(1); 
  }
  String setup_timer_time_min_14_water(){
    water_out_min_off = 14;
    return String(1);     
  }
  String setup_timer_time_min_15_water(){
    water_out_min_off = 15;   
    return String(1); 
  }
  String setup_timer_time_min_16_water(){
    water_out_min_off = 16;
    return String(1);     
  }
  String setup_timer_time_min_17_water(){
    water_out_min_off = 17;    
    return String(1); 
  }
  String setup_timer_time_min_18_water(){
    water_out_min_off = 18;    
    return String(1); 
  }
  String setup_timer_time_min_19_water(){
    water_out_min_off = 19;    
    return String(1); 
  }
  String setup_timer_time_min_20_water(){
    water_out_min_off = 20;    
    return String(1); 
  }
  String setup_timer_time_min_21_water(){
    water_out_min_off = 21;   
    return String(1);  
  }
  String setup_timer_time_min_22_water(){
    water_out_min_off = 22;  
    return String(1);   
  }
  String setup_timer_time_min_23_water(){
    water_out_min_off = 23;   
    return String(1);  
  }
  String setup_timer_time_min_24_water(){
    water_out_min_off = 24; 
    return String(1);    
  }
  String setup_timer_time_min_25_water(){
    water_out_min_off = 25; 
    return String(1);    
  }
  String setup_timer_time_min_26_water(){
    water_out_min_off = 26;   
    return String(1);  
  }
  String setup_timer_time_min_27_water(){
    water_out_min_off = 27; 
    return String(1);    
  }
  String setup_timer_time_min_28_water(){
    water_out_min_off = 28;    
    return String(1); 
  }
  String setup_timer_time_min_29_water(){
    water_out_min_off = 29;
    return String(1);     
  }
  String setup_timer_time_min_30_water(){
    water_out_min_off = 30;    
    return String(1); 
  }
  String setup_timer_time_min_31_water(){
    water_out_min_off = 31;    
    return String(1); 
  }
  String setup_timer_time_min_32_water(){
    water_out_min_off = 32;    
    return String(1); 
  }
  String setup_timer_time_min_33_water(){
    water_out_min_off = 33;    
    return String(1); 
  }
  String setup_timer_time_min_34_water(){
    water_out_min_off = 34;    
    return String(1); 
  }
  String setup_timer_time_min_35_water(){
    water_out_min_off = 35;    
    return String(1); 
  }
  String setup_timer_time_min_36_water(){
    water_out_min_off = 36;    
    return String(1); 
  }
  String setup_timer_time_min_37_water(){
    water_out_min_off = 37;    
    return String(1); 
  }
  String setup_timer_time_min_38_water(){
    water_out_min_off = 38;    
    return String(1); 
  }
  String setup_timer_time_min_39_water(){
    water_out_min_off = 39;    
    return String(1); 
  }
  String setup_timer_time_min_40_water(){
    water_out_min_off = 40;    
    return String(1); 
  }
   String setup_timer_time_min_41_water(){
    water_out_min_off = 41;    
    return String(1); 
  }
  String setup_timer_time_min_42_water(){
    water_out_min_off = 42;    
    return String(1); 
  }
  String setup_timer_time_min_43_water(){
    water_out_min_off = 43;    
    return String(1); 
  }
  String setup_timer_time_min_44_water(){
    water_out_min_off = 44;    
    return String(1); 
  }
  String setup_timer_time_min_45_water(){
    water_out_min_off = 45;
    return String(1);     
  }
  String setup_timer_time_min_46_water(){
    water_out_min_off = 46;    
    return String(1); 
  }
  String setup_timer_time_min_47_water(){
    water_out_min_off = 47;    
    return String(1); 
  }
  String setup_timer_time_min_48_water(){
    water_out_min_off = 48;    
    return String(1); 
  }
  String setup_timer_time_min_49_water(){
    water_out_min_off = 49;    
    return String(1); 
  }
  String setup_timer_time_min_50_water(){
    water_out_min_off = 50;    
    return String(1); 
  }
 String setup_timer_time_min_51_water(){
    water_out_min_off = 51;    
    return String(1); 
  }
  String setup_timer_time_min_52_water(){
    water_out_min_off = 52;    
    return String(1); 
  }
  String setup_timer_time_min_53_water(){
    water_out_min_off = 53;   
    return String(1);  
  }
  String setup_timer_time_min_54_water(){
    water_out_min_off = 54;    
    return String(1); 
  }
  String setup_timer_time_min_55_water(){
    water_out_min_off = 55;    
    return String(1); 
  }
  String setup_timer_time_min_56_water(){
    water_out_min_off = 56;    
    return String(1); 
  }
  String setup_timer_time_min_57_water(){
    water_out_min_off = 57;    
    return String(1); 
  }
  String setup_timer_time_min_58_water(){
    water_out_min_off = 58;    
    return String(1); 
  }
  String setup_timer_time_min_59_water(){
    water_out_min_off = 59;    
    return String(1); 
  }
  String setup_timer_time_min_60_water(){
    water_out_min_off = 60;    
    return String(1); 
  }


    String relay_switch_air_fish(){
      byte state;
      if(digitalRead(relayPin8))
         state = 0;
      else
         state = 1;
        digitalWrite(relayPin8,state);
        return String(state);
      }
      
    String relay_status_air_fish(){       // воздух для рыб
      byte state;
      if(digitalRead(relayPin8))
         state = 0;
      else
         state = 1;
        return String(state);
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

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <Wire.h>
#include <string.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Replace with your network credentials
const char* ssid     = "netis_BF4066";
const char* password = "head2020";

// REPLACE with your Domain name and URL path or IP address with path
//const char* serverName_public = "http://alexgorlov99.ru/post-esp-data.php";
const char* serverName_gasboiler_send_data_localRaspberry = "http://192.168.1.7/post-gasboiler-data.php";
const char* serverName_gasboiler_get_data_localRaspberry = "http://192.168.1.7/get-gasboiler-lastdata.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

void clean_part_array(char * array);
void clean_all_array(char * array);
int read_gasboiler_measurements();


int counter = 0;
char data[300] = {0};
char boiler_status[10] = {0};
char current_temp[10] = {0};
char temp_setpoint[10] = {0};
char work_mode[10] = {0};

char count[10] = {0};
int k = 0;
int i = 0;
uint32_t timer_millis = 0;
int FLAG = 1;


IPAddress local_IP(192, 168, 1, 12);
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

void setup()
{
  Serial.begin(115200, SERIAL_8N2);
  WiFi.begin(ssid, password);
  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  //Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    //delay(500);
    //Serial.print(".");
  }
  delay(6000);
  Serial.write("WiFi-OK/");
}
void loop()
{
    FLAG = 1;
    //отправка данных метеостанции в БД только при получении их от мк по UART(~каждые 5 мин) 
    if( Serial.available() > 0 ) 
    {   
      sprintf(data,"%s",Serial.readString().c_str());
      FLAG = read_gasboiler_measurements();
      counter++;
      delay(0);
      if(WiFi.status()== WL_CONNECTED)
      {
        WiFiClient client;
        HTTPClient http_Raspberry;
        if (FLAG == 1)
        {
          http_Raspberry.begin(client, serverName_gasboiler_send_data_localRaspberry);
          http_Raspberry.addHeader("Content-Type", "application/x-www-form-urlencoded");
          String httpRequestData_Raspberry = "api_key=" + apiKeyValue + "&Mode=" + work_mode
                              + "&Status=" + boiler_status + "&Temp_Current=" + current_temp +"&Temp_Setpoint=" + temp_setpoint + "";                    
          int httpResponseCode_Raspberry = http_Raspberry.POST(httpRequestData_Raspberry);   
          http_Raspberry.end();
        }
      }
      delay(0);
      clean_all_array(boiler_status);
      clean_all_array(current_temp);
      clean_all_array(temp_setpoint);
      clean_all_array(work_mode);
      clean_all_array(data);
      clean_all_array(count);
      delay(0);
    }
    //отправка данных в МК о состоянии котла
    else if (abs(timer_millis - millis()) == 10000)
    {
      if(WiFi.status()== WL_CONNECTED)
      {
        WiFiClient client;
        HTTPClient http_Raspberry;
        http_Raspberry.begin(client, serverName_gasboiler_get_data_localRaspberry);
        http_Raspberry.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String httpRequestData_Raspberry = "api_key=" + apiKeyValue + "";                    
        int httpResponseCode_Raspberry = http_Raspberry.POST(httpRequestData_Raspberry);  
        String answer =  http_Raspberry.getString();
        
        Serial.print("BD"+answer+"/");
        http_Raspberry.end();
      }
      timer_millis = millis();
      delay(0);
    }
    //проверка состояния wi-fi соединения
    else
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.write("WiFi-ERROR/");
        WiFi.reconnect();
        delay(1000);
        WiFi.begin(ssid, password);
        WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
        while(WiFi.status() != WL_CONNECTED) 
        {
          
        }
        Serial.write("WiFi-OK/");
      }
    }

}

void clean_part_array(char * array)
{
  for (int i = 35; i < strlen(array); i++)
  {
    array[i] = NULL;
  }
}

void clean_all_array(char * array)
{
  memset(array, 0, sizeof(char) * strlen(array));//РѕС‡РёСЃС‚РєР° РјР°СЃСЃРёРІР°
}

int read_gasboiler_measurements()
{
      uint32_t timer;         // РїРµСЂРµРјРµРЅРЅР°СЏ С‚Р°Р№РјРµСЂР°
      timer = millis();
      //считывание статуса котла
      while(data[i] != ' ')
      {
        boiler_status[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      if (strcmp(boiler_status,"0") == 0) strcpy(boiler_status,"OFF");
      else if (strcmp(boiler_status,"1") == 0) strcpy(boiler_status,"ON");
      //Serial.println(boiler_status);
      k = 0;
      i++;
      timer = millis();
      //считывание температуры текущей
      while(data[i] != ' ')
      {
        current_temp[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(current_temp);
      k = 0;
      i++;
      timer = millis();
      //считывание уставки
      while(data[i] != ' ')
      {
        temp_setpoint[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(temp_setpoint);
      k = 0;
      i++;
      timer = millis();
      //считывание режима работы
      while(data[i] != ' ')
      {
        work_mode[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      if (strcmp(work_mode,"0") == 0) strcpy(work_mode,"AUTO");
      else if (strcmp(work_mode,"1") == 0) strcpy(work_mode,"MANUAL");
      //Serial.println(work_mode);
      k = 0;
      i = 0;
      return 1;
}




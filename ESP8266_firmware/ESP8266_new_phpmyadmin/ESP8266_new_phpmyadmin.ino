#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Replace with your network credentials
const char* ssid     = "ASUS";
const char* password = "head2020";

// REPLACE with your Domain name and URL path or IP address with path
//const char* serverName_public = "http://alexgorlov99.ru/post-esp-data.php";
const char* serverName_localRaspberry = "http://192.168.1.7/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

void clean_part_array(char * array);
void clean_all_array(char * array);
int read_measurements();


int counter = 0;
char data[300] = {0};
char temp_street[10] = {0};
char temp_home[10] = {0};
char hum_street[10] = {0};
char hum_home[10] = {0};
char pressure[10] = {0};
char wind_speed[10] = {0};
char rain[10] = {0};
char VBAT[10] = {0};
char wind_direct[10] = {0};
char receive_time[20] = {0};
char count[10] = {0};
int k = 0;
int i = 0;
uint32_t timer = 0;
int FLAG = 1;


IPAddress local_IP(192, 168, 1, 10);
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
    delay(500);
    //Serial.print(".");
  }
  delay(20000);
  Serial.write("WiFi-OK/");
  delay(2000);
  Serial.print((String)"SSID-"+ssid+"/");
  //Serial.write("SSID-ASUS/");
  delay(2000);
  Serial.print((String)"PSWD-"+password+"/");
  //Serial.write("PSWD-head2020/");
  delay(2000);
  Serial.print("IP-"+local_IP.toString()+"/");
  //Serial.write("IP-192.168.0.1/");
  /*Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());*/
}
void loop()
{
    //ESP.restart();
    FLAG = 1;

    if( Serial.available() > 0 ) 
    {   
      sprintf(data,"%s",Serial.readString().c_str());
      //Serial.println(data);
      FLAG = read_measurements();
      counter++;
      delay(0);
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.write("WiFi-ERROR/");
        WiFi.disconnect();
        delay(1000);
        WiFi.begin(ssid, password);
        WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
        while(WiFi.status() != WL_CONNECTED) 
        {
          delay(500);
          //Serial.print(".");  
        }
        Serial.write("WiFi-OK/");
      }
      //Check WiFi connection status
      if(WiFi.status()== WL_CONNECTED)
      {
        WiFiClient client;
        HTTPClient http_Raspberry;
        if (FLAG == 1)
        {
          // Your Domain name with URL path or IP address with path
          http_Raspberry.begin(client, serverName_localRaspberry);
        
          // Specify content-type header
          //http_public.addHeader("Content-Type", "application/x-www-form-urlencoded");
          http_Raspberry.addHeader("Content-Type", "application/x-www-form-urlencoded");
          // Prepare your HTTP POST request data
          String httpRequestData_Raspberry = "api_key=" + apiKeyValue + "&HomeTemp=" + temp_home
                              + "&StreetTemp=" + temp_street + "&HomeHum=" + hum_home +"&StreetHum=" + hum_street
                              + "&Pressuare=" + pressure + "&WindSpeed=" + wind_speed 
                              + "&WindDirect=" + wind_direct + "&Rain=" + rain + "&BatteryCharge=" + VBAT + "&MeasureTime=" + receive_time + "";                    
          //String httpRequestData_Raspberry = "api_key=tPmAT5Ab3j7F9&HomeTemp=12.0&StreetTemp=9.0&HomeHum=35.0&StreetHum=40.0&Pressuare=750&WindSpeed=12&WindDirect=N&Rain=10&BatteryCharge=3.7&MeasureTime=12:18:22,12/НОЯ/22";
      
          // Send HTTP POST request
          int httpResponseCode_Raspberry = http_Raspberry.POST(httpRequestData_Raspberry);   
          /*if (httpResponseCode_Raspberry>0) 
          {
            Serial.print("HTTP Response code Raspberry: ");
            Serial.println(httpResponseCode_Raspberry);
          }
          else 
          {
            Serial.print("Error code Raspberry: ");
            Serial.println(httpResponseCode_Raspberry);
          }*/
          // Free resources
          http_Raspberry.end();
        }
      }
      else 
      {
        //Serial.println("WiFi Disconnected");
      }
      delay(0);
      clean_all_array(receive_time);
      clean_all_array(temp_street);
      clean_all_array(temp_home);
      clean_all_array(hum_street);
      clean_all_array(hum_home);
      clean_all_array(wind_speed);
      clean_all_array(rain);
      clean_all_array(pressure);
      clean_all_array(VBAT);
      clean_all_array(wind_direct);
      clean_all_array(data);
      clean_all_array(count);
      delay(0);
      //delay(1000);
    }
    else
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.write("WiFi-ERROR/");
        WiFi.disconnect();
        delay(1000);
        WiFi.begin(ssid, password);
        WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
        while(WiFi.status() != WL_CONNECTED) 
        {
          delay(500);
          Serial.print(".");  
        }
        Serial.write("WiFi-OK/");
      }
      //Serial.println("No data");
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
  memset(array, 0, sizeof(char) * strlen(array));//очистка массива
}

int read_measurements()
{
      uint32_t timer;         // переменная таймера
      timer = millis();
      //считываем ул.темпер.
      while(data[i] != ' ')
      {
        temp_street[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(temp_street);
      k = 0;
      i++;
      timer = millis();
      //считываем дом.темпер.
      while(data[i] != ' ')
      {
        temp_home[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(temp_home);
      k = 0;
      i++;
      timer = millis();
      //считываем ул.влажность
      while(data[i] != ' ')
      {
        hum_street[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(hum_street);
      k = 0;
      i++;
      timer = millis();
      //считываем дом.влажность
      while(data[i] != ' ')
      {
        hum_home[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(hum_home);
      k = 0;
      i++;
      timer = millis();
      //считываем давление
      while(data[i] != ' ')
      {
        pressure[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(pressure);
      k = 0;
      i++;
      timer = millis();
      //считываем осадки
      while(data[i] != ' ')
      {
        rain[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(rain);
      k = 0;
      i++;
      timer = millis();
      //считываем заряд АКБ
      while(data[i] != ' ')
      {
        VBAT[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(VBAT);
      k = 0;
      i++;
      timer = millis();
      //считываем скор.ветра
      while(data[i] != ' ')
      {
        wind_speed[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(wind_speed);
      k = 0;
      i++;
      timer = millis();
      //считываем напр.ветра
      while(data[i] != ' ')
      {
        wind_direct[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(wind_direct);
      k = 0;
      i++;
      timer = millis();
      //считываем дату
      while(data[i] != ' ')
      {
        receive_time[k] = data[i];
        k++;
        i++;
        if ((millis()-timer)>=3000)
        {
          return 0;
        }
        delay(0);
      }
      //Serial.println(receive_time);   
      k = 0;
      i = 0;
      return 1;
}



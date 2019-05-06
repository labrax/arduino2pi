
/*
Compiled set of sensors to monitor the room.
Author: github.com/labrax
*/

#include "main.h"


Sensor pir(D1, SENSOR_DIGITAL, 0);
Sensor phr(A0, SENSOR_ANALOG, 10);
Measurement<double> temp;
#define TEMP_SENSOR_DIGITAL D3


/*
DS18B20 BLOCK
*/
OneWire oneWire(TEMP_SENSOR_DIGITAL);
DallasTemperature temp_sensor(&oneWire);


/*
CONTROL BLOCK
*/
#define PRINT_WINDOW 10000

unsigned long nextPrint = 0;
unsigned long curTime;


void setup() {
  Serial.begin(115200);

  WiFi.begin("VM274904-2G", "zjccfgca");

  pir.begin();
  phr.begin();
}

void loop(){
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.print("WiFi connected!");
    Serial.println(WiFi.localIP());
    temp.reset();
    pir.reset();
    phr.reset();
    nextPrint = nextPrint + PRINT_WINDOW;
  }

  temp_sensor.requestTemperatures();
  temp.add_measurement(temp_sensor.getTempCByIndex(0));

  pir.read();
  phr.read();
  print_all();
}

void print_all() {
  curTime = millis();
  if(curTime >= nextPrint) {
    if(send_all() == 204) {
      Serial.print(curTime); Serial.print(",");
      Serial.print(pir.get_max()); Serial.print(",");
      Serial.print(phr.get_max()); Serial.print(",");
      Serial.print(temp.get_max());
      Serial.println();

      phr.reset();
      pir.reset();
      temp.reset();
    }
    nextPrint = nextPrint + PRINT_WINDOW;
  }
}

int send_data(String message) {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("No WiFi!");
    return -1;
  }
  HTTPClient http;
  http.begin("http://192.168.0.200:8086/write?db=roomsensors");
  http.addHeader("Content-Type", "text/plain");
  int httpCode = http.POST("window,host=vvv-d1-mini " + message);
  Serial.println(httpCode);   //Print HTTP return code
  http.end();  //Close connection
  return httpCode;
}

int send_all() {
  String message = "photoresistor=" + String(phr.get_max()) + ",passiveinfrared=" + String(pir.get_max()) + ",temperature=" + String(temp.get_max());
  return send_data(message);
  /*send_data("photoresistor", String(phr.get_max()), "volts");
  send_data("passiveinfrared", String(pir.get_max()), "binary");
  send_data("temperature", String(temp.get_max()), "celsius");*/
}

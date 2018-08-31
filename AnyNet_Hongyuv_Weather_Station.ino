#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include "WeatherStationHongyuv.h"
#include "eseyeaws.h"

#define ledpin 13

/* Set up the software serial for the AnyNet Click, the Modbus library and the Hongyuv library */
SoftwareSerial mySerial(11,12); //RX, TX
ModbusMaster node;
station station;

//Create timing variables, with a delay of 5 minutes
unsigned long prev_time = millis();
uint32_t delay_time = 300000;

void Get_Data(void);
void Send_Data(void);
void error(void);

// The structure to hold the returned weather data
struct Weather{
    float humid;
    float wind_speed;
    float temp;
    float pressure;
    float rain;
    float altitude;
    int wind_dir;
  }weather;

//Configuration for the Modbus library
void preTransmission(){
  digitalWrite(RT, 1);
}
void postTransmission(){
  digitalWrite(RT, 0);
}

void setup() {
  //Set LED and the RS485 module Read/Write pin as outputs
  pinMode(RT, OUTPUT);
  pinMode(ledpin, OUTPUT);
  digitalWrite(RT, 0);
  digitalWrite(ledpin, 0);

  //Begin hardware and software serial ports
  Serial.begin(HONGYUV_BAUD_RATE,SERIAL_8E1);
  mySerial.begin(9600);
  while(!mySerial | !Serial){
  }
  
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  //Begin connection to Weather Station
  node.begin(1, Serial);
  
  //Open topic between AnyNet Click and AWS account to pass data
  mySerial.println("AT+AWSPUBOPEN=0,\"MyTopic\"");
  mySerial.flush();
  delay(8000);
}


bool cycle = true;
void loop()
{
  //Check time since last iteration
  unsigned long current_time = millis();
  if((unsigned long) current_time - prev_time >= delay_time){
    //error();
    prev_time = current_time;
    cycle = true;
  }
  
  if(cycle){
    digitalWrite(ledpin,1);
    Get_Data();
    Send_Data();
    cycle = false;
    digitalWrite(ledpin,0);
   }
}


void Get_Data(){

  //Read data from station
  station.read_data(node);

  //Populate structure with the returned data
  weather.humid = station.convert(HUMIDITY);
  weather.wind_speed = station.convert(WIND_SPEED);
  weather.temp = station.convert(TEMPERATURE);
  weather.pressure = (station.convert(AIR_PRESSURE)/1000);
  //Note: rain pretty useless without matching refresh rate of weather station rain register
  weather.rain = station.convert(PRECIPITATION_INTENSITY);
  weather.altitude = station.convert(ALTITUDE);
  weather.wind_dir = station.process(WIND_DIRECTION);
}

void Send_Data(){
  
  //Notify AnyNet Click that device will attempt to send data to AWS
  mySerial.println("AT+AWSPUBLISH=0,88");
  Serial.flush();
  delay(8000);

  //Send data to AWS
  mySerial.print("Humidity: ");
  mySerial.println(weather.humid);
  mySerial.print("Pressure: ");
  mySerial.println(weather.pressure);
  mySerial.print("Rain: ");
  mySerial.println(weather.rain);
  mySerial.print("Temperature: ");
  mySerial.println(weather.temp);
  mySerial.print("Wind Direction: ");
  mySerial.println(weather.wind_dir);
  Serial.flush();
  delay(8000);

  char buff[8];
  String string = "";
  float farray[8] = {weather.humid,weather.temp,weather.pressure,weather.rain,weather.wind_dir,weather.wind_speed};
  String sarray[8]= {"{humid: ",",temp: ",",pressure: ",",rain: ",",direction: ",",wind_speed: "};
  for(int i=0; i<6;i++){
    string+= sarray[i];
    dtostrf(farray[i],4,2,buff);
    string+= buff;
  }

}

/*
void error(void){
  while(true){
  digitalWrite(ledpin,1);
  delay(500);
  digitalWrite(ledpin,0);
  delay(500);
  }
}
*/


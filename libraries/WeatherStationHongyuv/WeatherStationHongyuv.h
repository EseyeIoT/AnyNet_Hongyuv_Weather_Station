/* Written by Nick Lehrter */
#ifndef hongyuv_h
#define hongyuv_h
#endif
#include "Arduino.h"
#include "ModbusMaster.h"

/* A set of defines for all of the 2-bit registers present in the weather station. These will
allow a user to abstract away from registers in receiving data */
#define WIND_SPEED 0
#define TEMPERATURE 1
#define HUMIDITY 2
#define AIR_PRESSURE 3
#define PRECIPITATION_INTENSITY 4
#define ACCUMULATED_PRECIPITATION 5
#define GPS_SPEED 6
#define LONGITUDE 7
#define LATITUDE 8
#define DUST_CONCENTRATION 9
#define VISIBILITY 10
#define RADIATION_ILLUMINANCE 11
#define HONGYUV_INTERNAL 12
#define RADIATION_POWER 13
#define ALTITUDE 14

/* A second set for the individual registers in the station */
#define DEVICE_STATE 0
#define WIND_DIRECTION 1
#define COMPASS_HEADING 2
#define PRECIPITATION_TYPE 3
#define INTENSITY_UNIT 4
#define GPS_HEADING 5
#define COMPASS_CORRECTED_WIND_DIRECTION 6


/* Defines for use of Modbus master */
#define RT 5
#define HONGYUV_BAUD_RATE 9600

/* The class to store and process the data returned from the weather station */
class station
{
    public:
        station();
        void populate();
        float convert(int choice);
        float process(int selection);
        void read_data(ModbusMaster node);
        
    private:
        uint16_t datas[50];
        float func(uint16_t u1,uint16_t u2);
};
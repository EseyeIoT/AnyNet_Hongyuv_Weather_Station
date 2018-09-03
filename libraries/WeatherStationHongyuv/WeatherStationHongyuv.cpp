#include "WeatherStationHongyuv.h"

station::station(){};

// A function to take two big endian 16 bit registers and process into one 32 bit float
float station::func(uint16_t u1,uint16_t u2){
    union thing{
    uint32_t initial;
    float complete;
    }convert;
    convert.initial = ((unsigned long)u2 <<16)|u1;
    return convert.complete;
}

//A method to read two 16 bit registers and return a 32 bit float
float station::convert(int choice){
    int array[] = {2,4,6,8,12,14,18,21,23,25,27,29,31,33,36};
    int u1 = array[choice];
    int u2 = u1+1;
    return func(datas[u1],datas[u2]);
}

//A method to read a 16 bit register
float station::process(int selection){
    int array[] = {0,1,10,11,16,20,35};
    int memory_location = array[selection];
    return(datas[memory_location]);
}

//A method to use the ModbusMaster library to gather data
void station::read_data(ModbusMaster node){
    node.readHoldingRegisters(00,0x026);

    for(int i=0; i<50;i++){
        datas[i] = node.getResponseBuffer(i);
        Serial.println(node.getResponseBuffer(i));
    }
}

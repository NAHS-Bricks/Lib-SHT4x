#ifndef NAHS_BRICKS_LIB_SHT4X_H
#define NAHS_BRICKS_LIB_SHT4X_H

#include <Arduino.h>
#include <Wire.h>

typedef uint8_t SHT4x_SerialNumber[4];
typedef enum class SHT4x_Precision : uint8_t {
    high,
    medium,
    low
} SHT4x_Precision_t;

class NahsBricksLibSHT4x {
    private:
        uint8_t _addr;
        bool _triggered = false;
        bool _dataPulled = false;
        TwoWire *_wirePort;
        uint16_t _t;
        uint16_t _h;
        uint8_t _precision;
    public:
        NahsBricksLibSHT4x();
        bool begin(uint8_t addr = 0x44, TwoWire &wirePort = Wire);
        bool isConnected();
        void triggerRead();
        void setPrecision(SHT4x_Precision_t p);
        double getT();
        double getH();
        void getSN(uint8_t* sn);
        String snToString(SHT4x_SerialNumber sn);
    private:
        void _pullData();
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SHT4X)
extern NahsBricksLibSHT4x SHT4x;
#endif

#endif // NAHS_BRICKS_LIB_SHT4X_H

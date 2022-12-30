#include "nahs-Bricks-Lib-SHT4x.h"


NahsBricksLibSHT4x::NahsBricksLibSHT4x() {
    _precision = 0xFD;
}

bool NahsBricksLibSHT4x::begin(uint8_t addr, TwoWire &wirePort) {
    _addr = addr;
    _wirePort = &wirePort;
    return isConnected();
}

bool NahsBricksLibSHT4x::isConnected() {
    _pullData();
    _wirePort->beginTransmission(_addr);
    uint8_t r = _wirePort->endTransmission();

    return (r == 0);
}

void NahsBricksLibSHT4x::triggerRead() {
    if(!_triggered || _dataPulled) {
        _wirePort->beginTransmission(_addr);
        _wirePort->write(_precision);
        _wirePort->endTransmission();

        _triggered = true;
        _dataPulled = false;
    }
}

void NahsBricksLibSHT4x::setPrecision(SHT4x_Precision_t p) {
    if (p == SHT4x_Precision::high) _precision = 0xFD;
    else if (p == SHT4x_Precision::medium) _precision = 0xF6;
    else _precision = 0xE0;
}

double NahsBricksLibSHT4x::getT() {
    _pullData();
    return (_t / 65535.0) * 175.0 - 45;
}

double NahsBricksLibSHT4x::getH() {
    _pullData();
    double h = (_h / 65535.0) * 125.0 - 6;
    if (h > 100) return 100.0;
    if (h < 0) return 0.0;
    return h;
}

void NahsBricksLibSHT4x::getSN(uint8_t* sn) {
    _pullData();
    _wirePort->beginTransmission(_addr);
    _wirePort->write(0x89);
    _wirePort->endTransmission();
    delay(8);

    _wirePort->requestFrom(_addr, (uint8_t) 6);
    sn[0] = _wirePort->read();
    sn[1] = _wirePort->read();
    _wirePort->read();
    sn[2] = _wirePort->read();
    sn[3] = _wirePort->read();
    _wirePort->read();
}

String NahsBricksLibSHT4x::snToString(SHT4x_SerialNumber sn) {
    String str = "";
    for (uint8_t i = 0; i < 4; ++i) {
        if (sn[i] < 16 ) str += String(0, HEX);
        str += String(sn[i], HEX);
    }
    return str;
}

void NahsBricksLibSHT4x::_pullData() {
    if (!_dataPulled && _triggered) {
        for (uint8_t i = 0; i < 20; ++i) {
            uint8_t r = _wirePort->requestFrom(_addr, (uint8_t)6);
            if (r == 0) {
                delay(1);
                continue;
            }
            _t = (_wirePort->read() << 8);
            _t |= _wirePort->read();
            _wirePort->read();
            _h = (_wirePort->read() << 8);
            _h |= _wirePort->read();
            _wirePort->read();
            _dataPulled = true;
            _triggered = false;
            break;
        }
    }
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SHT4X)
NahsBricksLibSHT4x SHT4x;
#endif

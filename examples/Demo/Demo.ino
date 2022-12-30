#include <nahs-Bricks-Lib-SHT4x.h>

SHT4x_SerialNumber sn;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(15);
  Serial.println();
  Serial.println(SHT4x.begin());
  SHT4x.getSN(sn);
  SHT4x.setPrecision(SHT4x_Precision::high);  // Default is also high, so this statement is not necessary in this case
  SHT4x.triggerRead();
}

void loop() {
  Serial.println(SHT4x.isConnected());
  Serial.println(SHT4x.snToString(sn));
  Serial.println(SHT4x.getT());
  Serial.println(SHT4x.getH());
  SHT4x.triggerRead();
  delay(1000);
}
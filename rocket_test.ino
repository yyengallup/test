#include <i2c_t3.h>


#define ADDR 0x68

#define PWR_MNG 0x6B
#define USR_CNTRL 0x6A
#define GYRO_SETUP 0x1B

#define GYRO_X_HIGH 0x43
#define GYRO_X_LOW 0x44

#define GYRO_Y_HIGH 0x45
#define GYRO_Y_LOW 0x46

#define GYRO_Z_HIGH 0x47
#define GYRO_Z_LOW 0x48

int16_t x, y, z;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  write(PWR_MNG, 0);
  write(USR_CNTRL, 0);

  write(GYRO_SETUP, (0b01<<3)); //set gyro full scale, 00 at 250dps to 11 at 2000dps highest

}



void loop() {
  byte high, low;

  high = read(GYRO_X_HIGH);
  low = read(GYRO_X_LOW);

  x = (high<<8) | low;

  high = read(GYRO_Y_HIGH);
  low = read(GYRO_Y_LOW);

  y = (high<<8) | low;

  high = read(GYRO_Z_HIGH);
  low = read(GYRO_Z_LOW);

  z = (high<<8) | low;

  Serial.print("x: ");
  Serial.print(getDPS(x));
  Serial.print(" y: ");
  Serial.print(getDPS(y));
  Serial.print(" z: ");
  Serial.println(getDPS(z));

  delay(1000);
}


byte read(unsigned short reg) {
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(ADDR, 1, false);
  byte val = Wire.read();
  Wire.endTransmission(true);
  return val;

}

void write(unsigned short reg, byte data) {
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);
}

float getDPS(int16_t val) {
  return val * 500 / 32768;
}

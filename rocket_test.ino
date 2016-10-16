#include <i2c_t3.h>


#define ADDR 0x68 //i2c address of sensor

#define PWR_MNG 0x6B //register address of pwr management
#define USR_CNTRL 0x6A //register address of user control
#define GYRO_SETUP 0x1B //register address of gryoscope setup

#define GYRO_BASE 0x43

#define OFFSET_X_HIGH 0x0
#define OFFSET_X_LOW 0x1

#define OFFSET_Y_HIGH 0x2
#define OFFSET_Y_LOW 0x3

#define OFFSET_Z_HIGH 0x4
#define OFFSET_Z_LOW 0x5

int16_t x, y, z;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  write(PWR_MNG, 0);
  write(USR_CNTRL, 0);

  write(GYRO_SETUP, (0b01<<3)); //set gyro full scale, 00 at 250dps to 11 at 2000dps highest

}



void loop() {
  readGYRO();

  Serial.print("x: ");
  Serial.print(getDPS(x));
  Serial.print(" y: ");
  Serial.print(getDPS(y));
  Serial.print(" z: ");
  Serial.println(getDPS(z));

  delay(1000);
}

void readGyro() {
  byte high, low;

  high = read(GYRO_BASE + OFFSET_X_HIGH);
  low = read(GYRO_BASE + OFFSET_X_LOW);

  x = (high<<8) | low;

  high = read(GYRO_BASE + OFFSET_Y_HIGH);
  low = read(GYRO_BASE + OFFSET_Y_LOW);

  y = (high<<8) | low;

  high = read(GYRO_BASE + OFFSET_Z_HIGH);
  low = read(GYRO_BASE + OFFSET_Z_LOW);

  z = (high<<8) | low;
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

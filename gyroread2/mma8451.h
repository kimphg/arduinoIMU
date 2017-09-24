
/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define MMA8451_DEFAULT_ADDRESS                 (0x1D)    // if A is GND, its 0x1C
/*=========================================================================*/
#include <Wire.h>
#define MMA8451_REG_OUT_X_MSB     0x01
#define MMA8451_REG_SYSMOD        0x0B
#define MMA8451_REG_WHOAMI        0x0D
#define MMA8451_REG_XYZ_DATA_CFG  0x0E
#define MMA8451_REG_PL_STATUS     0x10
#define MMA8451_REG_PL_CFG        0x11
#define MMA8451_REG_CTRL_REG1     0x2A
#define MMA8451_REG_CTRL_REG2     0x2B
#define MMA8451_REG_CTRL_REG4     0x2D
#define MMA8451_REG_CTRL_REG5     0x2E



#define MMA8451_PL_PUF            0
#define MMA8451_PL_PUB            1
#define MMA8451_PL_PDF            2
#define MMA8451_PL_PDB            3
#define MMA8451_PL_LRF            4
#define MMA8451_PL_LRB            5
#define MMA8451_PL_LLF            6
#define MMA8451_PL_LLB            7


typedef enum
{
  MMA8451_RANGE_8_G           = 0b10,   // +/- 8g
  MMA8451_RANGE_4_G           = 0b01,   // +/- 4g
  MMA8451_RANGE_2_G           = 0b00    // +/- 2g (default value)
} mma8451_range_t;


/* Used with register 0x2A (MMA8451_REG_CTRL_REG1) to set bandwidth */
typedef enum
{
  MMA8451_DATARATE_800_HZ     = 0b000, //  800Hz
  MMA8451_DATARATE_400_HZ     = 0b001, //  400Hz
  MMA8451_DATARATE_200_HZ     = 0b010, //  200Hz
  MMA8451_DATARATE_100_HZ     = 0b011, //  100Hz
  MMA8451_DATARATE_50_HZ      = 0b100, //   50Hz
  MMA8451_DATARATE_12_5_HZ    = 0b101, // 12.5Hz
  MMA8451_DATARATE_6_25HZ     = 0b110, // 6.25Hz
  MMA8451_DATARATE_1_56_HZ    = 0b111, // 1.56Hz

  MMA8451_DATARATE_MASK       = 0b111
} mma8451_dataRate_t;


/**************************************************************************/
/*!
    @brief  Reads 8-bits from the specified register
*/
/**************************************************************************/

static inline void i2cwrite(uint8_t x) {
  #if ARDUINO >= 100
  Wire.write((uint8_t)x);
  #else
  Wire.send(x);
  #endif
}
static inline uint8_t i2cread(void) {
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}

void writeRegister8(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MMA8451_DEFAULT_ADDRESS);
  i2cwrite((uint8_t)reg);
  i2cwrite((uint8_t)(value));
  Wire.endTransmission();
}
uint8_t readRegister8(uint8_t reg) {
    Wire.beginTransmission(MMA8451_DEFAULT_ADDRESS);
    i2cwrite(reg);
    Wire.endTransmission(false); // MMA8451 + friends uses repeated start!!

    Wire.requestFrom(MMA8451_DEFAULT_ADDRESS, 1);
    if (! Wire.available()) return -1;
    return (i2cread());
}

bool beginMMA8451() {
  Wire.begin();

  /* Check connection */
  uint8_t deviceid = readRegister8(MMA8451_REG_WHOAMI);
  if (deviceid != 0x1A)
  {
    /* No MMA8451 detected ... return false */
    //Serial.println(deviceid, HEX);
    return false;
  }

  writeRegister8(MMA8451_REG_CTRL_REG2, 0x40); // reset

  while (readRegister8(MMA8451_REG_CTRL_REG2) & 0x40);

  // enable 4G range
  writeRegister8(MMA8451_REG_XYZ_DATA_CFG, MMA8451_RANGE_4_G);
  // High res
  writeRegister8(MMA8451_REG_CTRL_REG2, 0x02);
  // DRDY on INT1
  writeRegister8(MMA8451_REG_CTRL_REG4, 0x01);
  writeRegister8(MMA8451_REG_CTRL_REG5, 0x01);

  // Turn on orientation config
  writeRegister8(MMA8451_REG_PL_CFG, 0x40);

  // Activate at max rate, low noise mode
  writeRegister8(MMA8451_REG_CTRL_REG1, 0x01 | 0x04);

  /*
  for (uint8_t i=0; i<0x30; i++) {
    Serial.print("$");
    Serial.print(i, HEX); Serial.print(" = 0x");
    Serial.println(readRegister8(i), HEX);
  }
  */

  return true;
}


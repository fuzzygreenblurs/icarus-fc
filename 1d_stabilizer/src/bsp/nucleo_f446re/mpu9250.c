#include "main.h"
#include "hal/imu.h"
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

// MPU9250 register map
#define REG_PWR_MGMT_1     0x6B
#define REG_CONFIG         0x1A
#define REG_GYRO_CONFIG    0x1B
#define REG_ACCEL_CONFIG   0x1C
#define REG_ACCEL_CONFIG2  0x1D
#define REG_SMPLRT_DIV     0x19
#define REG_WHO_AM_I       0x75
#define REG_SAMPLE_START   0x3B // ACCEL_XOUT_H

#define SAMPLE_SIZE_BYTES    14
#define LSB_TO_MS2         (9.80665f / 8192.0f)
#define LSB_TO_DPS         (1.0f / 16.4f)

// polling paradigm 
static uint8_t tx_byte(uint8_t data) {
  while(!LL_SPI_IsActiveFlag_TXE(SPI2));
  LL_SPI_TransmitData8(SPI2, data);

  // RXNE: receive data register not empty
  // set when a new byte is received
  while(!LL_SPI_IsActiveFlag_RXNE(SPI2));
  return LL_SPI_ReceiveData8(SPI2);
}

static void cs_low(void) {
  // SPI: active-low chip-select pin to select target device
  LL_GPIO_ResetOutputPin(IMU_CS_GPIO_Port, IMU_CS_Pin);
}

static void cs_high(void) {
  // SPI: disengage connection with target device
  LL_GPIO_SetOutputPin(IMU_CS_GPIO_Port, IMU_CS_Pin);
}

static uint8_t read_reg(uint8_t reg) {
  cs_low();
  tx_byte(reg | 0x80);
  uint8_t val = tx_byte(0x00);
  cs_high();
  return val;
}

static void write_reg(uint8_t reg, uint8_t val) {
  cs_low();
  tx_byte(reg & 0x7F);
  tx_byte(val);
  cs_high();
}

imu_status_t imu_init(void) {
  write_reg(REG_PWR_MGMT_1, 0x80);    // reset MPU9250
  LL_mDelay(100);

  write_reg(REG_PWR_MGMT_1, 0x01);    // wake + PLL (autoselected)
  LL_mDelay(100);

  write_reg(REG_CONFIG,         0x02); // gyro DLPF = 92hz
  write_reg(REG_GYRO_CONFIG,    0x18); // gyro +/- 2000dps
  write_reg(REG_ACCEL_CONFIG,   0x08); // accel +/- 4g
  write_reg(REG_ACCEL_CONFIG2,  0x02); // accel DLPF = 92hz
  write_reg(REG_SMPLRT_DIV,     0x00); // internal sample rate: 1khz

  if(imu_is_healthy()) {
    return IMU_OK;
  }

  return IMU_ERR_NOT_READY;
}

static void burst_read(uint8_t *buf) {
  cs_low();
  tx_byte(REG_SAMPLE_START | 0x80);
  for(size_t i = 0; i < SAMPLE_SIZE_BYTES; i++) {
    buf[i] = tx_byte(0x00);
  }
  cs_high();
}

bool imu_is_healthy(void) {
  return read_reg(REG_WHO_AM_I) == 0x71;
}

imu_status_t imu_read_sample(imu_sample_t* out) {
  uint8_t raw_buf[14];
  burst_read(raw_buf);

  int16_t ax = (raw_buf[0]  << 8 | raw_buf[1]);
  int16_t ay = (raw_buf[2]  << 8 | raw_buf[3]);
  int16_t az = (raw_buf[4]  << 8 | raw_buf[5]);
  int16_t gx = (raw_buf[8]  << 8 | raw_buf[9]);
  int16_t gy = (raw_buf[10] << 8 | raw_buf[11]);
  int16_t gz = (raw_buf[12] << 8 | raw_buf[13]);

  out->accel_x = (float)ax * LSB_TO_MS2;
  out->accel_y = (float)ay * LSB_TO_MS2;
  out->accel_z = (float)az * LSB_TO_MS2;
  
  out->gyro_x  = (float)gx * LSB_TO_DPS;
  out->gyro_y  = (float)gy * LSB_TO_DPS;
  out->gyro_z  = (float)gz * LSB_TO_DPS;

  return IMU_OK;
}

// futureproofing: TODO
imu_t* imu_get(imu_id_t imu_id) { 
  return NULL;
}



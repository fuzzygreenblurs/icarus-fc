#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
  IMU_ID_PRIMARY = 0,
  IMU_ID_SECONDARY = 1,
  IMU_ID_COUNT
} imu_id_t;

// opaque-pointer for BSP abstraction 
typedef struct imu_t imu_t;

typedef struct imu_sample_t {
  float accel_x;  // accel: [ms2]
  float accel_y;
  float accel_z;

  float gyro_x;   // gyro: dps 
  float gyro_y;
  float gyro_z;
} imu_sample_t;

typedef enum {
  IMU_OK = 0,
  IMU_ERR_INVALID_ARG,
  IMU_ERR_TIMEOUT,
  IMU_ERR_NOT_READY,
  IMU_ERR_COUNT
} imu_status_t;

imu_status_t imu_init(void); 
imu_status_t imu_read_sample(imu_sample_t* out);
imu_t* imu_get(imu_id_t imu_id);
bool imu_is_healthy(void);

#endif

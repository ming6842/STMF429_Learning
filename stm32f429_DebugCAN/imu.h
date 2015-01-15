#ifndef FILE_IMU_H
#define FILE_IMU_H

#include "stm32f4xx_conf.h"


typedef struct imu_unscaled_data_t {
	int16_t acc[3];
	int16_t gyro[3];
	int16_t mag[3];
	int16_t temp;
} imu_unscaled_data_t;


typedef struct imu_data_t {
	float acc[3];
	float gyro[3];
	float mag[3];
	float temp;
} imu_data_t;

typedef struct imu_calibrated_offset_t {
	int16_t acc[3];
	int16_t gyro[3];
	int16_t mag[3];
	float acc_scale[3];
	float gyro_scale[3];
	float mag_scale[3];
} imu_calibrated_offset_t;


#ifdef USE_MAG_HMC5983
	
#define magnetometer_initialize_config() hmc5983_initialize_config()
#define magnetometer_initialize(offset) hmc5983_initialize_system(offset)
#define magnetometer_update(rawdata) hmc5983_update(rawdata)
#define magnetometer_scale_data(rawdata,scaled_data,offset) hmc5983_convert_to_scale(rawdata, scaled_data,offset)

#include "hmc5983.h"
#endif

#ifdef USE_IMU_MPU9250
#define imu_update(rawdata) mpu9250_read_accel_temp_gyro(rawdata)
#define imu_initialize_config() mpu9250_initialize_config()
#define imu_initialize(offset,count) mpu9250_initialize_system(offset,count)
#define imu_scale_data(rawdata,scaled_data,offset) mpu9250_convert_to_scale(rawdata, scaled_data,offset)
#define imu_calibrate_gyro_offset(offset,count) mpu9250_calibrate_gyro_offset(offset,count)

#include "mpu9250.h"

#endif  


#endif
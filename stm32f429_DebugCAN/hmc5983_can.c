#include "hmc5983_can.h"

uint8_t hmc5983_can_checkAndUpdateIMU(imu_unscaled_data_t *imu_unscaledData, CanRxMsg* MagCANRXMessage){

 uint8_t hmc5983_buffer[6];

	hmc5983_buffer[0] = MagCANRXMessage->Data[0];
	hmc5983_buffer[1] = MagCANRXMessage->Data[1];
	hmc5983_buffer[2] = MagCANRXMessage->Data[2];
	hmc5983_buffer[3] = MagCANRXMessage->Data[3];
	hmc5983_buffer[4] = MagCANRXMessage->Data[4];
	hmc5983_buffer[5] = MagCANRXMessage->Data[5];


	imu_unscaledData->mag[0] = -(int16_t)(((uint16_t)hmc5983_buffer[0] << 8) | (uint16_t)hmc5983_buffer[1]);
	imu_unscaledData->mag[2] = -(int16_t)(((uint16_t)hmc5983_buffer[2] << 8) | (uint16_t)hmc5983_buffer[3]);
	imu_unscaledData->mag[1] =  (int16_t)(((uint16_t)hmc5983_buffer[4] << 8) | (uint16_t)hmc5983_buffer[5]);


}

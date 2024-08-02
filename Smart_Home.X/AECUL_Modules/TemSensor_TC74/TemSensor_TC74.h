
#ifndef TEMSENSOR_TC74_H
#define	TEMSENSOR_TC74_H

#include "../../mcc_generated_files/mcc.h"
#include "../../mcc_generated_files/examples/i2c_master_example.h"

uint8_t TempSensor_TC74_Read_Temp(i2c_address_t TC74_Address);
void print_temperature_value(uint8_t temperature);

#endif	/* TEMSENSOR_TC74_H */


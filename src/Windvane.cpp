/**
 *******************************************************************************
 *******************************************************************************
 *
 *	License :
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 
 *******************************************************************************
 *******************************************************************************
 *
 *
 *    @file   Windvane.cpp
 *    @author gilou
 *    @date   23 mai 2017
 *    @brief  The windvane class permit to read/average/print value from adc.
 *
 *    On the windlogger the windvane is connected on the ADC6.
 *
 */

#include "Arduino.h"

#include <Windvane.h>

/******************************************************************************
 * Class constructor and destructor
 */
Windvane::Windvane(unsigned char id):m_id(id) {
	m_eeprom_addr = 1000 + m_id*50;		/**< This calc permit to store sensor data each 16 bytes after the EEPROM_OFFSET(30) */

	load_param();
}

Windvane::~Windvane() {
}


/******************************************************************************
 * Sensor's methods
 */
// This method clear data array.
void Windvane::clear(unsigned char measure_max){
	for(unsigned char i = 0; i < measure_max; ++i){
		m_data[i] = 0;
	}
}

// This method read the adc(id) value and multiply it by m_factor and add m_offset
void Windvane::read_value(unsigned char measure_number){
	m_data[measure_number]=(analogRead(m_id)*m_factor)+m_offset; // channel 6 for the windvane
}

// This method calculate the average from the data array.
void Windvane::calc_average(unsigned char measure_max){
	if(m_enable){
		m_average=0;
		for(int i =0; i< measure_max;++i)
		{
			m_average+=m_data[i];
		}
		m_average/= measure_max;
	}
}


// Print in string "Vane id"
char* Windvane::print(char *string){
	char temp_char[6];
	strcpy(string,"Vane ");
	strcat(string,itoa(m_id,temp_char,10));
	return string;
}


// This method print the sensor configuration for the sensor. It's a good idea to overload this function to do it more explicit for each sensor.
// todo reimplemente print_config
void Windvane::print_config(){
	Serial.println("Windvane :");
	Serial.print("	*51 enable :	");Serial.print(m_enable);Serial.println("			enable : 1, disable : 0");
	Serial.print("	*52 factor:	");Serial.print(m_factor);Serial.println("		can be a float value. ex: 42.42");
	Serial.print("	*53 offset:	");Serial.print(m_offset);Serial.println("		can be a float value. ex: 42.42");
}

// The config method permit to update parameters
void Windvane::config(char *stringConfig){
	uint8_t item = stringConfig[2]-'0';	// convert item in char

	double arg_f = atof(stringConfig + 4);	// convert the second part, the value in double to cover all possibilities.
	unsigned char arg_uc = (unsigned char)arg_f;
	switch (item) {
		case 1:	// enable or disable anemo1
			if(arg_uc==0)m_enable = false;	// disable
			else m_enable = true;				// enable
			update_param();
		break;
		case 2:	// Set factor value
			m_factor = arg_f;
			update_param();
			break;
		case 3:	// Set offset value
			m_offset = arg_f;
			update_param();
			break;
		default:
			Serial.print("Bad request : ");Serial.println(item);
	}
}

// Use this method for debugging or calibration accuracy
void Windvane::print_data_array()const{
	char temp[10];
	Serial.print("\r\n");
	for(int i=0;i<10;i++)
	{
		Serial.print(i);Serial.print('	');Serial.print(dtostrf(m_data[i],0,1,temp));Serial.print("\r\n");
	}
}

/******************************************************************************
 * sens_param management
 */
// Load saved parameters for sensors from the eeprom
void Windvane::load_param(){
	m_factor = eeprom_read_float((float*)m_eeprom_addr);
	m_offset = eeprom_read_float((float*)m_eeprom_addr+5);
	m_enable = eeprom_read_byte((unsigned char*)m_eeprom_addr+10);
}

// Update saved parameters for sensors in the eeprom
void Windvane::update_param(){
	eeprom_update_float((float*)m_eeprom_addr,m_factor);
	eeprom_update_float((float*)m_eeprom_addr+5,m_offset);
	eeprom_update_byte((unsigned char*)m_eeprom_addr+10,m_enable);
}

// Initialize the eeprom memory and the sens_param array
// todo initialize_param
void Windvane::initialize_param(){

}



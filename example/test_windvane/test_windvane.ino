#include "Arduino.h"
#include <Windvane.h>

const unsigned char measure_max = 4;

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	Windvane vane(6);
	vane.set_enable(true);
	vane.set_factor(0.3515625);		// *360/1024 : convert in degre
	vane.set_offset(0);
	vane.clear(measure_max);

	Serial.begin(115200);
	Serial.println("Test the anemometer library");

	for(unsigned char measure=0; measure < measure_max; ++measure){	// do measure_max measures

		Serial.println("stamp");

		vane.read_value(measure);
	}


	vane.calc_average(measure_max);

	char temp[25];
	vane.print_data_array();
	Serial.print(vane.print(temp));
	Serial.print(" ");
	Serial.print(vane.print_average(0, temp));		// degree accuracy is enough
	Serial.println("°");

	Serial.println("Reset to restart");
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
}

/*
 Name:		Adjutant.ino
 Created:	8/26/2016 2:41:20 PM
 Author:	konra
*/

#include <Wire/Wire.h>

#include "LMP91000.h"

// Gas sensor circuit controls
#define PSEL0_PIN 5
#define PSEL1_PIN 4
#define PSEL2_PIN 0
#define MENB_PIN 13

// Sensors and components
Components::LMP91000 co_sensor = Components::LMP91000(MENB_PIN);

void setup() {
	// Intialize UART connection
	Serial.begin(9600);
	while (!Serial);

	// Initialize I2C connection
	Wire.begin();

	// Initialze sensors and components
	co_sensor.Begin();
}

void loop() {
	// Do stuff...
}

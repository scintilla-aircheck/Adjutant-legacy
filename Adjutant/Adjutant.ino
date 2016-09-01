/*
 Name:		Adjutant.ino
 Created:	8/26/2016 2:41:20 PM
 Author:	konra
*/

#include <Wire.h>

#include "PSTAT.h"
#include "SPEC.h"
#include "SDS021.h"

// UART circuit controls
#define SSRX_PIN 14
#define SSTX_PIN 12

// Gas sensor circuit controls
#define PSEL0_PIN 4
#define PSEL1_PIN 5
#define PSEL2_PIN 13
#define MENB_PIN 16

// Sensors and components
Components::PSTAT gas = Components::PSTAT(PSEL0_PIN, PSEL1_PIN, PSEL2_PIN, MENB_PIN);
Components::SDS021 dust = Components::SDS021(SSRX_PIN, SSTX_PIN);
//Components::SKM61 gps = Components::SKM61();

void setup() {
	// Intialize UART connection
	Serial.begin(9600);
	while (!Serial); // Necessary for USB
	Serial.println("Hardware serial initialized.");

	// Initialize software serial connection
	Serial.print("Initializing dust sensor...");
	dust.Begin();
	dust.PassiveMode(true);
	Serial.println("Done!");

	// Initialize I2C connection
	Serial.print("Initializing I2C bus...");
	Wire.pins(2, 0);
	Wire.begin();
	Serial.println("Done!");

	// Initialize SPEC sensors
	Serial.print("Initializing PSTAT circuit...");
	gas.Begin();
	Serial.println("Done!");

	Serial.print("Configuring ADC circuit...");
	gas.ADC(true, Components::MCP3425::EResolution::d16Bit, Components::MCP3425::EGain::x1);
	Serial.println("Done!");

	Serial.print("Configuring O2 sensor...");
	gas.Configure(0, Components::SPEC::CO);
	Serial.println("Done!");

	//gas.Configure(1, Components::SPEC::O3);
}

void loop() {
	Serial.println("----------");

	dust.Query();
	dust.Update();

	Serial.print("PM 2.5: ");
	Serial.println(dust.PM2_5(), 1);

	Serial.print("PM 10: ");
	Serial.println(dust.PM10(), 1);

	Serial.print("VOUT: ");
	Serial.println(gas.ADC(), 4);

	delay(10000);
}

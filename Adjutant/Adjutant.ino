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
#define PSTAT0_PIN 4
#define PSTAT1_PIN 5
#define PSTAT2_PIN 13
#define MENB_PIN 16

// Sensors and components
Components::SDS021 dust = Components::SDS021(SSRX_PIN, SSTX_PIN);
Components::PSTAT gas = Components::PSTAT(PSTAT0_PIN, PSTAT1_PIN, PSTAT2_PIN, MENB_PIN);
//Components::SKM61 gps = Components::SKM61(SSRX_PIN, SSTX_PIN);

void setup() {
	// Intialize UART connection
	Serial.begin(9600);
	while (!Serial); // Necessary for USB
	Serial.println("Hardware serial initialized.");

	// Initialize I2C connection
	Serial.print("Initializing I2C bus...");
	Wire.pins(2, 0);
	Wire.begin();
	Serial.println("Done!");

	// Initialize software serial connection
	Serial.print("Initializing dust sensor...");
	dust.Begin();
	dust.PassiveMode(true);
	dust.Awake(false);
	dust.Update();
	Serial.println("Done!");

	// Initialize SPEC sensors
	Serial.print("Initializing PSTAT circuit...");
	gas.Begin();
	gas.ADC(true, Components::MCP3425::EResolution::d16Bit, Components::MCP3425::EGain::x1);
	gas.Configure(0, Components::SPEC::CO);
	//gas.Configure(1, Components::SPEC::O3);
	Serial.println("Done!");
}

void loop() {

	// Wake up and query dust sensor
	dust.Awake(true);
	delay(30 * 1000);
	dust.Query();
	if (dust.Update())
	{
		Serial.print("PM 2.5: ");
		Serial.println(dust.PM2_5(), 1);

		Serial.print("PM 10: ");
		Serial.println(dust.PM10(), 1);
	}
	dust.Awake(false);

	// Wake up an query gas sensors
	//for (int i = 0; i < 5; i++)
	//{
	//	gas.Select(i);
	//	gas.Awake(true);
	//
	//	Serial.print(gas.Target());
	//	Serial.print(": ");
	//	Serial.println(gas.PPM());
	//
	//	gas.Awake(false);
	//}

	// DEBUG: Gas voltage
	Serial.print("VOUT: ");
	Serial.println(gas.ADC(), 4);

	delay(30 * 1000);
}

/*
 Name:		Adjutant.ino
 Created:	8/26/2016 2:41:20 PM
 Author:	konra
*/

#include <Wire.h>

#include "PSTAT.h"

// Gas sensor circuit controls
#define PSEL0_PIN 4
#define PSEL1_PIN 5
#define PSEL2_PIN 13
#define MENB_PIN 16

// Sensors and components
Components::PSTAT gas = Components::PSTAT(PSEL0_PIN, PSEL1_PIN, PSEL2_PIN, MENB_PIN);
//Components::SDS021 dust = Components::SDS021();
//Components::SKM61 gps = Components::SKM61();

void setup() {
	// Intialize Serial/UART connection
	Serial.begin(9600);
	while (!Serial); // Necessary for USB
	Serial.println("UART initialized.");

	// Initialize I2C connection
	Wire.pins(2, 0);
	Wire.begin();
	Serial.println("I2C initialized.");

	// Initialize SPEC sensors
	gas.Begin();
	gas.ADC(true, Components::MCP3425::EResolution::d16Bit, Components::MCP3425::EGain::x1);
	//gas.Configure(0, Components::SPEC::ETarget::CO);
	//gas.Configure(1, Components::SPEC::ETarget::O3);
	Serial.println("SPEC sensors configured.");

	// -------------------------------------------------------------------------

	// DEBUG: Initialze CO sensor
	Serial.print("DEBUG: Waiting for gas sensor...");
	while (!gas.isReady())
	{
		Serial.print(".");
		delay(10);
	}
	Serial.println("DONE!");

	gas.isLocked(false);
	gas.TIAGain(Components::LMP91000::ETIAGain::R350k);
	gas.RLoad(Components::LMP91000::ERLoad::R10);
	gas.ExtRef(true);
	gas.IntZero(Components::LMP91000::EIntZero::d20pct);
	gas.BiasSign(true);
	gas.Bias(Components::LMP91000::EBias::d1pct);
	gas.FETShort(false);
	gas.OpMode(Components::LMP91000::EOpMode::ThreeCell);
	Serial.println("DEBUG: CO sensor configured.");

	// DEBUG: Configure CO sensor
	Serial.println("----- SPEC CO -----");

	Serial.println("STATUS");
	Serial.print("  Status: ");
	Serial.println(gas.isReady());

	Serial.println("LOCK");
	Serial.print("  Locked: ");
	Serial.println(gas.isLocked());

	Serial.println("TIACN");
	Serial.print("  TIA gain: ");
	Serial.println((byte)gas.TIAGain());

	Serial.print("  RLoad: ");
	Serial.println((byte)gas.RLoad());

	Serial.println("TIACN");
	Serial.print("  ExtRef: ");
	Serial.println(gas.ExtRef());

	Serial.print("  IntZero: ");
	Serial.println((byte)gas.IntZero());

	Serial.print("  BiasSign: ");
	Serial.println(gas.BiasSign());

	Serial.print("  Bias: ");
	Serial.println((byte)gas.Bias());

	Serial.println("MODECN");
	Serial.print("  FETShort: ");
	Serial.println(gas.FETShort());

	Serial.print("  OpMode: ");
	Serial.println((byte)gas.OpMode());
}

byte input;

void loop() {
	if (Serial.available() > 0)
	{
		input = Serial.parseInt();
		gas.Select(input);
	}
}

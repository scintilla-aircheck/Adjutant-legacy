/*
 Name:		Adjutant.ino
 Created:	8/26/2016 2:41:20 PM
 Author:	konra
*/

#include <Wire/Wire.h>

#include "SPEC.h"

// Gas sensor circuit controls
#define PSEL0_PIN 5
#define PSEL1_PIN 4
#define PSEL2_PIN 0
#define MENB_PIN 16

// Sensors and components
Components::SPEC gas;

void setup() {
	// Intialize Serial/UART connection
	Serial.begin(9600);
	while (!Serial); // Necessary for USB
	Serial.println("UART initialized.");

	// Initialize I2C connection
	Wire.begin();
	Serial.println("I2C initialized.");

	// Initialize controllers
	gas = Components::SPEC(PSEL0_PIN, PSEL1_PIN, PSEL2_PIN, MENB_PIN);
	// dust = Components::SDS021()
	
	// DEBUG: Initialze CO sensor
	Serial.print("Waiting for gas sensor...");
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
	Serial.println("CO sensor configured...");

	// Configure CO sensor
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

void loop() {
}

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
#define MENB_PIN 16

// Sensors and components
Components::LMP91000 co_sensor = Components::LMP91000(MENB_PIN);

void setup() {
	// Intialize UART connection
	Serial.begin(9600);
	while (!Serial); // Necessary for USB
	Serial.println("UART initialized...");

	// Initialize I2C connection
	Wire.begin();
	Serial.println("I2C initialized...");

	pinMode(MENB_PIN, OUTPUT);
	digitalWrite(MENB_PIN, LOW);

	// Initialze CO sensor
	co_sensor.Begin();
	while (!co_sensor.isReady());
	co_sensor.isLocked(false);
	co_sensor.TIAGain(Components::LMP91000::ETIAGain::R350k);
	co_sensor.RLoad(Components::LMP91000::ERLoad::R10);
	co_sensor.ExtRef(true);
	co_sensor.IntZero(Components::LMP91000::EIntZero::d20pct);
	co_sensor.BiasSign(true);
	co_sensor.Bias(Components::LMP91000::EBias::d1pct);
	co_sensor.FETShort(false);
	co_sensor.OpMode(Components::LMP91000::EOpMode::ThreeCell);
	Serial.println("CO sensor configured...");

	// Configure CO sensor
	Serial.println("----- SPEC CO -----");

	Serial.println("STATUS");
	Serial.print("  Status: ");
	Serial.println(co_sensor.isReady());

	Serial.println("LOCK");
	Serial.print("  Locked: ");
	Serial.println(co_sensor.isLocked());

	Serial.println("TIACN");
	Serial.print("  TIA gain: ");
	Serial.println((byte)co_sensor.TIAGain());

	Serial.print("  RLoad: ");
	Serial.println((byte)co_sensor.RLoad());

	Serial.println("TIACN");
	Serial.print("  ExtRef: ");
	Serial.println(co_sensor.ExtRef());

	Serial.print("  IntZero: ");
	Serial.println((byte)co_sensor.IntZero());

	Serial.print("  BiasSign: ");
	Serial.println(co_sensor.BiasSign());

	Serial.print("  Bias: ");
	Serial.println((byte)co_sensor.Bias());

	Serial.println("MODECN");
	Serial.print("  FETShort: ");
	Serial.println(co_sensor.FETShort());

	Serial.print("  OpMode: ");
	Serial.println((byte)co_sensor.OpMode());
}

void loop() {
	if (Serial.available() > 0) {
		byte input = Serial.parseInt();
		co_sensor.isLocked(input);
		Serial.print("isLocked: ");
		Serial.println(co_sensor.isLocked());
	}
}

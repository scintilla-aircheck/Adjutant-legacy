#pragma once

#include <Arduino.h>
#include <Wire/Wire.h>

namespace Components
{
	class LMP91000
	{
	public:
		LMP91000(byte menb_pin);
		~LMP91000();

		// LMP91000 register address map
		static enum class ERegister {
			Status = 0x00,		// Read-only	/	Not lockable
			Lock = 0x01,		// Read/Write	/	     "
			TIACN = 0x10,		//     "		/	  Lockable
			REFCN = 0x11,		//     "		/	     "
			MODECN = 0x12,		//     "		/	Not lockable
		};

		// TIA gain resistor settings
		static enum class ETIAGain {
			External,
			R2_75k,
			R3_5k,
			R7k,
			R14k,
			R35k,
			R120k,
			R350k
		};

		// Load resistor settings
		static enum class ERLoad {
			R10,
			R33,
			R50,
			R100
		};

		// Internal zero settings
		static enum class EIntZero {
			d20pct,
			d50pct,
			d67pct,
			Bypass
		};

		// Voltage bias settings
		static enum class EBias {
			d0pct,
			d1pct,
			d2pct,
			d4pct,
			d6pct,
			d8pct,
			d10pct,
			d12pct,
			d14pct,
			d16pct,
			d18pct,
			d20pct,
			d22pct,
			d24pct
		};

		// Operating mode settings
		static enum class EOpMode {
			DeepSleep = 0x00,
			TwoCell = 0x01,
			Standby = 0x02,
			ThreeCell = 0x03,
			TIAOff = 0x06,
			TIAOn = 0x07		// Temperature sensing mode
		};

		static const byte I2CAddress = 0x48; // 1001 000

		// Device ready status
		bool isReady();

		// TIACN / REFCN lock status
		bool isLocked();
		void isLocked(bool locked);

		// TIA gain resistor setting
		ETIAGain TIAGain();
		void TIAGain(ETIAGain gain);

		// Load resistor setting
		ERLoad RLoad();
		void RLoad(ERLoad resistance);

		// External reference voltage setting
		bool ExtRef();
		void ExtRef(bool external);

		// Internal zero voltage setting
		EIntZero IntZero();
		void IntZero(EIntZero int_z);

		// Bias voltage polarity setting
		bool BiasSign();
		void BiasSign(bool positive);

		// Bias voltage setting
		EBias Bias();
		void Bias(EBias step);

		// FET short switch setting
		bool FETShort();
		void FETShort(bool enabled);

		// Operating mode setting
		EOpMode OpMode();
		void OpMode(EOpMode mode);
		
	private:
		// The MENB pin number
		byte MENB_Pin_;

		// Set the current target register
		byte setRegister(ERegister target);

		// Get/Set a target register byte
		byte getRawValue(ERegister target);
		byte setRawValue(ERegister target, byte value);

		// Get/set a specific value within a target register byte
		byte getValue(ERegister target, byte mask = 1, byte offset = 0);
		byte setValue(ERegister target, byte value, byte mask = 1, byte offset = 0);
	};
}

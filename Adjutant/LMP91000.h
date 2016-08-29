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
		enum class ERegister {
			Status = 0x00,		// Read-only	/	Not lockable
			Lock = 0x01,		// Read/Write	/	     "
			TIACN = 0x10,		//     "		/	  Lockable
			REFCN = 0x11,		//     "		/	     "
			MODECN = 0x12,		//     "		/	Not lockable
		};

		// TIA gain resistor settings
		enum class ETIAGain {
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
		enum class ERLoad {
			R10,
			R33,
			R50,
			R100
		};

		// Internal zero settings
		enum class EIntZero {
			d20pct,
			d50pct,
			d67pct,
			Bypass
		};

		// Voltage bias settings
		enum class EBias {
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
		enum class EOpMode {
			DeepSleep = 0x00,
			TwoCell = 0x01,
			Standby = 0x02,
			ThreeCell = 0x03,
			TIAOff = 0x06,
			TIAOn = 0x07		// Temperature sensing mode
		};

		static const byte I2CAddress = 0x48; // 1001 000

		// Set pin states, etc.
		void Begin();

		// Device ready status
		bool isReady();

		// TIACN / REFCN lock status (true is locked)
		bool isLocked();
		void isLocked(bool locked);

		// TIA gain resistor setting
		ETIAGain TIAGain();
		void TIAGain(ETIAGain gain);

		// Load resistor setting
		ERLoad RLoad();
		void RLoad(ERLoad resistance);

		// External reference voltage setting (true is external)
		bool ExtRef();
		void ExtRef(bool external);

		// Internal zero voltage setting
		EIntZero IntZero();
		void IntZero(EIntZero int_z);

		// Bias voltage polarity setting (true is positive)
		bool BiasSign();
		void BiasSign(bool positive);

		// Bias voltage setting
		EBias Bias();
		void Bias(EBias step);

		// FET short switch setting (true is enables)
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

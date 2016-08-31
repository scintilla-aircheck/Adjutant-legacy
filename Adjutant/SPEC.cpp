#include "SPEC.h"

namespace Components
{
	SPEC::SPEC() : LMP91000() {}

	SPEC::SPEC(byte select_0, byte select_1, byte select_2, byte menb) : LMP91000(menb)
	{
		// Associate control pins
		Address_[0] = select_0;
		Address_[1] = select_1;
		Address_[2] = select_2;
	}

	SPEC::~SPEC() {}
	
	void SPEC::Begin()
	{
		// Call parent method
		LMP91000::Begin();

		// Set pin states
		pinMode(Address_[0], OUTPUT);
		pinMode(Address_[1], OUTPUT);
		pinMode(Address_[2], OUTPUT);
	}

	void SPEC::Configure(byte addr, ETarget target)
	{
		Select(addr);
		if (this->isReady())
			if (target == ETarget::CO) {}
	}

	double SPEC::ADC()
	{
		return ADC_.Voltage();
	}

	void SPEC::ADC(bool continuous, MCP3425::EResolution resolution, MCP3425::EGain gain)
	{
		ADC_.Configure(continuous, resolution, gain);
	}

	void SPEC::Select(byte target)
	{
		// Parse byte and set pin state
		for (byte i = 0; i < 3; i++)
			if (target & (1 << i))
				digitalWrite(Address_[i], HIGH);
			else
				digitalWrite(Address_[i], LOW);
	}
}

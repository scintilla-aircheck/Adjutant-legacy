#include "SPEC.h"

Components::SPEC::SPEC() : LMP91000() {}

Components::SPEC::SPEC(byte select_0, byte select_1, byte select_2, byte menb) : LMP91000(menb)
{
	// Associate control pins
	Address_[0] = select_0;
	Address_[1] = select_1;
	Address_[2] = select_2;

	// Set pin states
	pinMode(Address_[0], OUTPUT);
	pinMode(Address_[1], OUTPUT);
	pinMode(Address_[2], OUTPUT);
}

Components::SPEC::~SPEC() {}

void Components::SPEC::Configure(byte addr, ETarget target)
{
	Select(addr);
		if (this->isReady())
			if (target == ETarget::CO) {}
}

void Components::SPEC::Select(byte target)
{
	// Parse byte and set pin state
	for (byte i = 0; i < 3; i++)
		if (target & (1 << i))
			digitalWrite(Address_[i], HIGH);
		else
			digitalWrite(Address_[i], LOW);
}

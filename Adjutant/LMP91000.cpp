#include "LMP91000.h"

Components::LMP91000::LMP91000() {}

Components::LMP91000::LMP91000(byte menb_pin)
{
	// Associate MENB pin
	MENBPin_ = menb_pin;

	// Set MENB pin state
	pinMode(MENBPin_, OUTPUT);
	digitalWrite(MENBPin_, HIGH);
}

Components::LMP91000::~LMP91000() {}

bool Components::LMP91000::isReady()
{
	return getValue(ERegister::Status);
}

bool Components::LMP91000::isLocked()
{
	return getValue(ERegister::Lock);
}

void Components::LMP91000::isLocked(bool locked)
{
	setValue(ERegister::Lock, locked);
}

Components::LMP91000::ETIAGain Components::LMP91000::TIAGain()
{
	return (ETIAGain)getValue(ERegister::TIACN, 7, 2);
}

void Components::LMP91000::TIAGain(ETIAGain gain)
{
	setValue(ERegister::TIACN, (byte)gain, 7, 2);
}

Components::LMP91000::ERLoad Components::LMP91000::RLoad()
{
	return (ERLoad)getValue(ERegister::TIACN, 3, 0);
}

void Components::LMP91000::RLoad(ERLoad resistance)
{
	setValue(ERegister::TIACN, (byte)resistance, 3, 0);
}

bool Components::LMP91000::ExtRef()
{
	return getValue(ERegister::REFCN, 1, 7);
}

void Components::LMP91000::ExtRef(bool external)
{
	setValue(ERegister::REFCN, external, 1, 7);
}

Components::LMP91000::EIntZero Components::LMP91000::IntZero()
{
	return (EIntZero)getValue(ERegister::REFCN, 3, 5);
}

void Components::LMP91000::IntZero(EIntZero int_z)
{
	setValue(ERegister::REFCN, (byte)int_z, 3, 5);
}

bool Components::LMP91000::BiasSign()
{
	return getValue(ERegister::REFCN, 1, 4);
}

void Components::LMP91000::BiasSign(bool positive)
{
	setValue(ERegister::REFCN, positive, 1, 4);
}

Components::LMP91000::EBias Components::LMP91000::Bias()
{
	return (EBias)getValue(ERegister::REFCN, 13, 0);
}

void Components::LMP91000::Bias(EBias bias)
{
	setValue(ERegister::REFCN, (byte)bias, 13, 0);
}

bool Components::LMP91000::FETShort()
{
	return getValue(ERegister::MODECN, 1, 7);
}

void Components::LMP91000::FETShort(bool enabled)
{
	setValue(ERegister::MODECN, enabled, 1, 7);
}

Components::LMP91000::EOpMode Components::LMP91000::OpMode()
{
	return (EOpMode)getValue(ERegister::MODECN, 7);
}

void Components::LMP91000::OpMode(EOpMode mode)
{
	setValue(ERegister::MODECN, (byte)mode, 7);
}

/// Sets the LMP91002 memory register to an intended target
byte Components::LMP91000::setRegister(ERegister target)
{
	digitalWrite(MENBPin_, LOW);
	Wire.beginTransmission(I2CAddress);
	Wire.write((byte)target);
	return Wire.endTransmission();
	digitalWrite(MENBPin_, HIGH);
}

/// Sets a target LMP91002 memory register to an intended value
byte Components::LMP91000::getRawValue(ERegister target)
{
	// Set target register
	setRegister(target);

	// Read target register
	digitalWrite(MENBPin_, LOW);
	Wire.requestFrom((int)I2CAddress, 1);
	return Wire.read();
	digitalWrite(MENBPin_, HIGH);
}

/// Sets a target LMP91002 memory register to an intended value
byte Components::LMP91000::setRawValue(ERegister target, byte value)
{
	digitalWrite(MENBPin_, LOW);
	Wire.beginTransmission(I2CAddress);
	Wire.write((byte)target);
	Wire.write(value);
	return Wire.endTransmission();
	digitalWrite(MENBPin_, HIGH);
}

/// Gets a specific value from a target LMP91002 register
byte Components::LMP91000::getValue(ERegister target, byte mask, byte offset)
{
	mask <<= offset;
	byte ext_value = getRawValue(target);
	return (ext_value & mask) >> offset;
}

/// Sets a specific value to a target LMP91002 memory register
byte Components::LMP91000::setValue(ERegister target, byte value, byte mask, byte offset)
{
	value <<= offset;
	mask <<= offset;
	byte ext_value = getRawValue(target);
	ext_value = (ext_value & ~mask) | value;
	return setRawValue(target, ext_value);
}

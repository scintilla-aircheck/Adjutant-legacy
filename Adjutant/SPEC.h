#pragma once

#include <Arduino.h>

#include "LMP91000.h"
#include "MCP3425.h"

namespace Components
{
	class SPEC : public LMP91000
	{
	public:
		SPEC();
		SPEC(byte addr_0_pin, byte addr_1_pin, byte addr_2_pin, byte menb_pin);
		~SPEC();

		// Target SPEC sensor
		enum class ETarget {
			CO,
			O3,
			SO2,
			NO2,
			H2S
		};

		void Begin();

		void Configure(byte addr, ETarget target);

		// ADC interface
		double ADC();
		void ADC(bool continuous, MCP3425::EResolution resolution, MCP3425::EGain gain);

	private:
		byte Address_[3];
		MCP3425 ADC_;

		void Select(byte target);
	};
}

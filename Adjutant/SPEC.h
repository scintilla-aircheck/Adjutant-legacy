#pragma once

namespace Components
{
	namespace SPEC
	{
		LMP91000::Configuration CO = {
			LMP91000::ETIAGain::R350k,
			LMP91000::ERLoad::R10,
			true,
			LMP91000::EIntZero::d20pct,
			true,
			LMP91000::EBias::d1pct,
			false,
			LMP91000::EOpMode::ThreeCell
		};
	}
}


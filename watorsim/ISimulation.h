#pragma once

#include <ostream>
#include <vector>
#include <cstdint>

#include "IObject.h"

namespace wator
{
	struct SIMULATION_PARAMETERS
	{
		unsigned Width;
		unsigned Height;
		char FishSymbol;
		char SharkSymbol;
		char WatorSymbol;
		unsigned FishBreed;
		unsigned SharkBreed;
		unsigned SharkStarve;
		unsigned InitialFishCount;
		unsigned InitialSharkCount;
	};

		
	struct SIMULATION_STATUS
	{
		unsigned FishCount;
		unsigned SharkCount;
		unsigned Iterations;
		std::string Data;
	};

	struct ISimulation : public IObject
	{
		virtual void Init(
			const SIMULATION_PARAMETERS& parameters
		) = 0;

		virtual void InitDefault() = 0;

		
		virtual void SetConfig(const unsigned uFishBreed,
			const unsigned uSharkBreed,
			const unsigned uSharkStarve
		) = 0;
		

		virtual void Reset() = 0;

		virtual inline const SIMULATION_PARAMETERS& _getParameters() const = 0;
		__declspec(property(get = _getParameters)) const SIMULATION_PARAMETERS& Parameter;

		virtual SIMULATION_STATUS Step() = 0;
	};
}


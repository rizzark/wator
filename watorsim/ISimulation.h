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
		unsigned FishBreed;
		unsigned SharkBreed;
		unsigned SharkStarve;
		unsigned InitialFishCount;
		unsigned InitialSharkCount;
	};


	struct ISimulation : public IObject
	{
		virtual void Init(
			const SIMULATION_PARAMETERS& parameters,
			std::ostream*				 posCSVLog = NULL
		) = 0;

		virtual void InitDefault(std::ostream* posCSVLog = NULL
		) = 0;

		virtual void SetConfig(const unsigned uFishBreed,
			const unsigned uSharkBreed,
			const unsigned uSharkStarve
		) = 0;

		virtual void Reset() = 0;

		virtual void Get(char* pcData, const size_t sizData) = 0;
		virtual bool IsFish(const unsigned pos) const = 0;
		virtual bool IsShark(const unsigned pos) const = 0;

		virtual inline const SIMULATION_PARAMETERS& _getParameters() const = 0;
		__declspec(property(get = _getParameters)) const SIMULATION_PARAMETERS& Parameter;

		virtual inline unsigned _getFishCount() const = 0;
		__declspec(property(get = _getFishCount)) unsigned FishCount;
		virtual inline unsigned _getSharkCount() const = 0;
		__declspec(property(get = _getSharkCount)) unsigned SharkCount;
		virtual inline unsigned _getIterations() const = 0;
		__declspec(property(get = _getIterations)) unsigned Iterations;

		virtual void Step() = 0;

		virtual std::ostream* SetLog(std::ostream* posCSVLog) = 0;

		virtual inline const std::vector<std::pair<unsigned, unsigned> >& GetHistory() const = 0;
	};
}


#pragma once

#include <ostream>
#include <vector>
#include <cstdint>

#include "IObject.h"

namespace wator
{
	struct ISimulation : public IObject
	{
		virtual void Init(const unsigned width,
			const unsigned height,
			const unsigned uFishBreed,
			const unsigned uSharkBreed,
			const unsigned uSharkStarve,
			const unsigned uFishCount,
			const unsigned uSharkCount,
			std::ostream* posCSVLog = NULL
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


		virtual inline unsigned _getFishCount() const = 0;
		__declspec(property(get = _getFishCount)) unsigned FishCount;
		virtual inline unsigned _getSharkCount() const = 0;
		__declspec(property(get = _getSharkCount)) unsigned SharkCount;
		virtual inline unsigned _getIterations() const = 0;
		__declspec(property(get = _getIterations)) unsigned Iterations;

		virtual inline unsigned _getWidth() const = 0;
		__declspec(property(get = _getWidth)) unsigned Width;
		virtual inline unsigned _getHeight() const = 0;
		__declspec(property(get = _getHeight)) unsigned Height;

		virtual inline unsigned _getSharkStart() const = 0;
		__declspec(property(get = _getSharkStart)) unsigned SharkStart;
		virtual inline unsigned _getFishStart() const = 0;
		__declspec(property(get = _getFishStart)) unsigned FishStart;
		virtual inline unsigned _getFishBreed() const = 0;
		__declspec(property(get = _getFishBreed)) unsigned FishBreed;
		virtual inline unsigned _getSharkBreed() const = 0;
		__declspec(property(get = _getSharkBreed)) unsigned SharkBreed;
		virtual inline unsigned _getSharkStarve() const = 0;
		__declspec(property(get = _getSharkStarve)) unsigned SharkStarve;

		virtual void Step() = 0;

		virtual std::ostream* SetLog(std::ostream* posCSVLog) = 0;

		virtual inline const std::vector<std::pair<unsigned, unsigned> >& GetHistory() const = 0;
	};
}


#pragma once

#include "ISimulation.h"

namespace wator
{
	class Sim1 : public ISimulation
	{
	public:
		static const char CHAR_FISH;
		static const char CHAR_SHARK;
		static const char CHAR_WATER;

		static const unsigned DEFAULT_WIDTH;
		static const unsigned DEFAULT_HEIGHT;
		static const unsigned DEFAULT_FISHBREED;
		static const unsigned DEFAULT_SHARKBREED;
		static const unsigned DEFAULT_SHARKSTARVE;
		static const unsigned DEFAULT_FISHCOUNT;
		static const unsigned DEFAULT_SHARKCOUNT;

		Sim1();

		Sim1(const unsigned width,
			const unsigned height,
			const unsigned uFishBreed,
			const unsigned uSharkBreed,
			const unsigned uSharkStarve,
			const unsigned uFishCount,
			const unsigned uSharkCount,
			std::ostream* posCSVLog = NULL
		);

		virtual ~Sim1();

		virtual void Init(const unsigned width,
			const unsigned height,
			const unsigned uFishBreed,
			const unsigned uSharkBreed,
			const unsigned uSharkStarve,
			const unsigned uFishCount,
			const unsigned uSharkCount,
			std::ostream* posCSVLog = NULL
		);

		virtual void Init(
			const SIMULATION_PARAMETERS& args,
			const unsigned fishCount,
			const unsigned sharkCount,
			std::ostream* log = NULL
		);

		virtual void InitDefault(std::ostream* posCSVLog = NULL
		);

		virtual void SetConfig(const unsigned uFishBreed,
			const unsigned uSharkBreed,
			const unsigned uSharkStarve
		);

		virtual void Reset();

		virtual void Get(char* pcData, const size_t sizData);

		virtual inline bool IsFish(const unsigned pos) const { return m_pbFish[pos] != 0; }
		virtual inline bool IsShark(const unsigned pos) const { return m_pbShark[pos] != 0; }

		virtual inline unsigned _getFishCount() const { return m_uFishCount; }
		virtual inline unsigned _getSharkCount() const { return m_uSharkCount; }
		virtual inline unsigned _getIterations() const { return m_uLoops; }

		virtual inline unsigned _getWidth() const { return m_parameters.Width; }
		virtual inline unsigned _getHeight() const { return m_parameters.Height; }

		virtual inline unsigned _getSharkStart() const { return m_uSharkStart; }
		virtual inline unsigned _getFishStart() const { return m_uFishStart; }
		virtual inline unsigned _getFishBreed() const { return m_parameters.FishBreed; }
		virtual inline unsigned _getSharkBreed() const { return m_parameters.SharkBreed; }
		virtual inline unsigned _getSharkStarve() const { return m_parameters.SharkStarve; }

		virtual void Step();

		virtual std::ostream* SetLog(std::ostream* posCSVLog
		);

		virtual inline const std::vector<std::pair<unsigned, unsigned> >& GetHistory() const
		{
			return m_vHistory;
		}

		virtual inline void Delete()
		{
			delete this;
		} // end - Delete


	protected:
		virtual void ClearFields();

		virtual void MoveFish(const unsigned uCurPos);
		virtual void MoveShark(const unsigned uCurPos);

		virtual void GetNeighbours(const unsigned uOfs, unsigned* puOfs);

		virtual void FishAdd(const unsigned uPos);
		virtual void FishMove(const unsigned uFrom, const unsigned uTo);
		virtual void FishAddAge(const unsigned uPos, const unsigned uDelta = 1);
		virtual void FishKill(const unsigned uPos);

		virtual void SharkAdd(const unsigned uPos);
		virtual void SharkMove(const unsigned uFrom, const unsigned uTo);
		virtual void SharkAddAge(const unsigned uPos, const unsigned uDelta = 1);
		virtual void SharkKill(const unsigned uPos);

	private:
		SIMULATION_PARAMETERS m_parameters;
		//unsigned	   m_uWidth;
		//unsigned	   m_uHeight;
		size_t		   m_sizField;
		//unsigned	   m_uFishBreed;
		//unsigned	   m_uSharkBreed;
		//unsigned	   m_uSharkStarve;

		unsigned	   m_uFishStart;
		unsigned	   m_uSharkStart;

		std::uint8_t* m_pbFish;
		std::uint8_t* m_pbFishMove;
		std::uint8_t* m_pbFishBreed;
		std::uint8_t* m_pbShark;
		std::uint8_t* m_pbSharkMove;
		std::uint8_t* m_pbSharkBreed;
		std::uint8_t* m_pbSharkStarve;
		unsigned	   m_uFishCount;
		unsigned	   m_uSharkCount;
		unsigned	   m_uLoops;

		std::ostream* m_posCSVLog;
		std::vector<std::pair<unsigned, unsigned> > m_vHistory;
		const size_t							   m_sizHistory;

		void init();
	};
}


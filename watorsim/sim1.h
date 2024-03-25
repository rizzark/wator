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

		virtual ~Sim1();

		virtual void Init(
			const SIMULATION_PARAMETERS& parameters,
			std::ostream* posCSVLog = NULL
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

		virtual inline const SIMULATION_PARAMETERS& _getParameters() const { return m_parameters; }


		virtual inline unsigned _getFishCount() const { return m_uFishCount; }
		virtual inline unsigned _getSharkCount() const { return m_uSharkCount; }
		virtual inline unsigned _getIterations() const { return m_uLoops; }

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
		size_t		   m_sizField;

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


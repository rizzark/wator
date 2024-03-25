#pragma once

#include "ISimulation.h"
#include "IRandomNumberProvider.h"

namespace wator
{
	class Sim1 : public ISimulation
	{
	public:
		Sim1(IRandomNumberProvider& rnd);

		virtual ~Sim1();

		virtual void Init(
			const SIMULATION_PARAMETERS& parameter
		);

		virtual void InitDefault();

		virtual void SetConfig(const unsigned uFishBreed,
			const unsigned uSharkBreed,
			const unsigned uSharkStarve
		);

		virtual void Reset();

		virtual inline const SIMULATION_PARAMETERS& _getParameters() const { return m_parameter; }

		virtual SIMULATION_STATUS Step();

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
		IRandomNumberProvider& m_rnd;
		SIMULATION_PARAMETERS m_parameter;
		size_t				  m_sizField;
		SIMULATION_STATUS	  m_status;

		std::uint8_t* m_pbFish;
		std::uint8_t* m_pbFishMove;
		std::uint8_t* m_pbFishBreed;
		std::uint8_t* m_pbShark;
		std::uint8_t* m_pbSharkMove;
		std::uint8_t* m_pbSharkBreed;
		std::uint8_t* m_pbSharkStarve;

		void init();
	};
}




#include "sim1.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


namespace wator
{
Sim1::Sim1(IRandomNumberProvider& rnd) 
	: m_rnd(rnd), 
	m_pbFish(NULL),
	m_pbFishMove(NULL),
	m_pbFishBreed(NULL),
	m_pbShark(NULL),
	m_pbSharkMove(NULL),
	m_pbSharkBreed(NULL),
	m_pbSharkStarve(NULL)
{
	InitDefault();
} // end - Sim1::Sim1


Sim1::~Sim1()
{
	delete[] m_pbFish; m_pbFish = NULL;
	delete[] m_pbFishMove; m_pbFishMove = NULL;
	delete[] m_pbFishBreed; m_pbFishBreed = NULL;
	delete[] m_pbShark; m_pbShark = NULL;
	delete[] m_pbSharkMove; m_pbSharkMove = NULL;
	delete[] m_pbSharkBreed; m_pbSharkBreed = NULL;
	delete[] m_pbSharkStarve; m_pbSharkStarve = NULL;
} // end - Sim1::~WatorSim


void Sim1::Init(const SIMULATION_PARAMETERS& parameter)
{
	unsigned u = 0;

	delete[] m_pbFish; m_pbFish = NULL;
	delete[] m_pbFishMove; m_pbFishMove = NULL;
	delete[] m_pbFishBreed; m_pbFishBreed = NULL;
	delete[] m_pbShark; m_pbShark = NULL;
	delete[] m_pbSharkMove; m_pbSharkMove = NULL;
	delete[] m_pbSharkBreed; m_pbSharkBreed = NULL;
	delete[] m_pbSharkStarve; m_pbSharkStarve = NULL;

	m_parameter = parameter;
	m_sizField = parameter.Width * parameter.Height;
	m_pbFish = new std::uint8_t[m_sizField];
	m_pbFishMove = new std::uint8_t[m_sizField];
	m_pbFishBreed = new std::uint8_t[m_sizField];
	m_pbShark = new std::uint8_t[m_sizField];
	m_pbSharkMove = new std::uint8_t[m_sizField];
	m_pbSharkBreed = new std::uint8_t[m_sizField];
	m_pbSharkStarve = new std::uint8_t[m_sizField];

	Reset();
} // end - Sim1::Init


void Sim1::InitDefault()
{
	SIMULATION_PARAMETERS args;

	args.Width = 100;
	args.Height = 80;
	args.FishBreed = 5;
	args.SharkBreed = 7;
	args.SharkStarve = 4;
	args.InitialFishCount = 200;
	args.InitialSharkCount = 250;
	args.WatorSymbol = ' ';
	args.FishSymbol = '+';
	args.SharkSymbol = '#';

	Init(args);
} // end - Sim1::InitDefault


void Sim1::SetConfig(const unsigned uFishBreed,
	const unsigned uSharkBreed,
	const unsigned uSharkStarve)
{
	m_parameter.FishBreed = uFishBreed;
	m_parameter.SharkBreed = uSharkBreed;
	m_parameter.SharkStarve = uSharkStarve;
} // end - Sim1::SetConfig


void Sim1::Reset()
{
	unsigned u = 0;

	m_status.FishCount = 0;
	m_status.SharkCount = 0;
	m_status.Iterations = 0;

	ClearFields();
	for (u = 0; u < m_parameter.InitialFishCount; u++)
	{
		unsigned uRandom = 0;

		do
		{
			uRandom = m_rnd.Get(static_cast<unsigned>(m_sizField - 1));
		} while (m_pbFish[uRandom] != 0);
		FishAdd(uRandom);
		m_status.FishCount++;
	}

	for (u = 0; u < m_parameter.InitialSharkCount; u++)
	{
		unsigned uRandom = 0;

		do
		{
			uRandom = m_rnd.Get(static_cast<unsigned>(m_sizField - 1));
		} while (m_pbFish[uRandom] != 0 || m_pbShark[uRandom] != 0);
		SharkAdd(uRandom);
		m_status.SharkCount++;
	}
} // end - Sim1::Reset


SIMULATION_STATUS Sim1::Step()
{
	unsigned u = 0;

	memset(m_pbFishMove, 0, m_sizField);
	memset(m_pbSharkMove, 0, m_sizField);

	for (u = 0; u < m_sizField; u++)
	{
		if (m_pbFish[u] != 0 && m_pbFishMove[u] == 0)
			MoveFish(u);
		else if (m_pbShark[u] != 0 && m_pbSharkMove[u] == 0)
			MoveShark(u);
	}

	m_status.Data.clear();
	m_status.Data.resize(m_sizField, m_parameter.WatorSymbol);
		
	m_status.FishCount = m_status.SharkCount = 0;
	for (u = 0; u < m_sizField; u++)
	{
		if (m_pbFish[u] != 0)
		{
			m_status.FishCount++;
			m_status.Data[u] = m_parameter.FishSymbol;
		}
		if (m_pbShark[u] != 0)
		{
			m_status.SharkCount++;
			m_status.Data[u] = m_parameter.SharkSymbol;
		}
	}

	m_status.Iterations++;
	return m_status;
} // end - Sim1::Step


void Sim1::ClearFields()
{
	memset(m_pbFish, 0, m_sizField);
	memset(m_pbFishMove, 0, m_sizField);
	memset(m_pbFishBreed, 0, m_sizField);
	memset(m_pbShark, 0, m_sizField);
	memset(m_pbSharkMove, 0, m_sizField);
	memset(m_pbSharkBreed, 0, m_sizField);
	memset(m_pbSharkStarve, 0, m_sizField);
} // end - Sim1::ClearFields


void Sim1::MoveFish(const unsigned uCurPos)
{
	unsigned puNeighbour[8];
	unsigned puFree[8];
	unsigned u = 0;
	unsigned uFree = 0;
	unsigned uMoveOfs = -1;
	unsigned uBreedOfs = -1;

	memset(puNeighbour, 0, sizeof(puNeighbour));
	memset(puFree, 0, sizeof(puFree));

	GetNeighbours(uCurPos, puNeighbour);
	for (u = 0; u < 8; u++)
	{
		if (m_pbFish[puNeighbour[u]] == 0 && m_pbShark[puNeighbour[u]] == 0)
		{
			puFree[uFree] = puNeighbour[u];
			uFree++;
		}
	}

	if (uFree > 1)
	{
		do
		{
			uBreedOfs = puFree[m_rnd.Get(uFree - 1)];
			uMoveOfs = puFree[m_rnd.Get(uFree - 1)];
		} while (uBreedOfs == uMoveOfs);
	}
	else if (uFree > 0)
	{
		uBreedOfs = puFree[m_rnd.Get(uFree - 1)];
		uMoveOfs = -1;
	}

	FishAddAge(uCurPos);
	if (m_pbFishBreed[uCurPos] > m_parameter.FishBreed && uBreedOfs != -1)
	{
		FishAdd(uBreedOfs);
		m_pbFishBreed[uCurPos] = 1;
	}
	if (uMoveOfs != -1)
		FishMove(uCurPos, uMoveOfs);
} // end - Sim1::MoveFish


void Sim1::MoveShark(const unsigned uCurPos)
{
	unsigned uPos = uCurPos;
	unsigned puNeighbour[8];
	unsigned puFree[8];
	unsigned u = 0;
	unsigned uFree = 0;
	unsigned uMoveOfs = -1;
	unsigned uBreedOfs = -1;

	memset(puNeighbour, 0, sizeof(puNeighbour));
	memset(puFree, 0, sizeof(puFree));

	GetNeighbours(uPos, puNeighbour);
	for (u = 0; u < 8; u++)
	{
		if (m_pbShark[puNeighbour[u]] == 0)
		{
			puFree[uFree] = puNeighbour[u];
			uFree++;
		}
	}

	if (uFree > 1)
	{
		do
		{
			uBreedOfs = puFree[m_rnd.Get(uFree - 1)];
			uMoveOfs = puFree[m_rnd.Get(uFree - 1)];
		} while (uBreedOfs == uMoveOfs);
	}
	else if (uFree > 0)
	{
		uBreedOfs = puFree[m_rnd.Get(uFree - 1)];
		uMoveOfs = -1;
	}

	SharkAddAge(uPos);
	if (m_pbSharkBreed[uPos] > m_parameter.SharkBreed && uBreedOfs != -1)
	{
		if (m_pbFish[uBreedOfs] != 0)
			FishKill(uBreedOfs);

		SharkAdd(uBreedOfs);
		m_pbSharkBreed[uPos] = 1;
	}
	if (uMoveOfs != -1)
	{
		if (m_pbFish[uMoveOfs] != 0)
		{
			FishKill(uMoveOfs);
			m_pbSharkStarve[uPos] = 0;
		}

		SharkMove(uPos, uMoveOfs);
		uPos = uMoveOfs;
	}

	if (m_pbSharkStarve[uPos] > m_parameter.SharkStarve)
		SharkKill(uPos);
} // end - Sim1::MoveShark


void Sim1::GetNeighbours(const unsigned uOfs,
	unsigned* puOfs)
{
	const unsigned x = uOfs % m_parameter.Width;
	const unsigned y = uOfs / m_parameter.Width;
	const unsigned x1 = (x % m_parameter.Width) ? x - 1 : x + m_parameter.Width - 1;
	const unsigned x2 = ((x % m_parameter.Width) != (m_parameter.Width - 1)) ? x + 1 : x - m_parameter.Width + 1;
	const unsigned y1 = (y % m_parameter.Height) ? y - 1 : y + m_parameter.Height - 1;
	const unsigned y2 = ((y % m_parameter.Height) != (m_parameter.Height - 1)) ? y + 1 : y - m_parameter.Height + 1;

	puOfs[0] = y1 * m_parameter.Width + x1;
	puOfs[1] = y1 * m_parameter.Width + x;
	puOfs[2] = y1 * m_parameter.Width + x2;
	puOfs[3] = y * m_parameter.Width + x1;
	puOfs[4] = y * m_parameter.Width + x2;
	puOfs[5] = y2 * m_parameter.Width + x1;
	puOfs[6] = y2 * m_parameter.Width + x;
	puOfs[7] = y2 * m_parameter.Width + x2;
} // end - Sim1::GetFreeNeighbour


void Sim1::FishAdd(const unsigned uPos)
{
	m_pbFish[uPos] = 1;
	m_pbFishMove[uPos] = 1;
	m_pbFishBreed[uPos] = 1;
} // end - Sim1::FishAdd


void Sim1::FishMove(const unsigned uFrom,
	const unsigned uTo)
{
	m_pbFish[uTo] = m_pbFish[uFrom];
	m_pbFish[uFrom] = 0;
	m_pbFishBreed[uTo] = m_pbFishBreed[uFrom];
	m_pbFishBreed[uFrom] = 0;
	m_pbFishMove[uTo] = 1;
} // end - Sim1::FishMove


void Sim1::FishAddAge(const unsigned uPos,
	const unsigned uDelta)
{
	m_pbFish[uPos] += uDelta;
	m_pbFishBreed[uPos] += uDelta;
} // end - Sim1::FishAddAge


void Sim1::FishKill(const unsigned uPos)
{
	m_pbFish[uPos] = 0;
	m_pbFishBreed[uPos] = 0;
	m_pbFishMove[uPos] = 0;
} // end - Sim1::FishKill


void Sim1::SharkAdd(const unsigned uPos)
{
	m_pbShark[uPos] = 1;
	m_pbSharkMove[uPos] = 1;
} // end - Sim1::SharkAdd


void Sim1::SharkMove(const unsigned uFrom,
	const unsigned uTo)
{
	m_pbShark[uTo] = m_pbShark[uFrom];
	m_pbShark[uFrom] = 0;
	m_pbSharkBreed[uTo] = m_pbSharkBreed[uFrom];
	m_pbSharkBreed[uFrom] = 0;
	m_pbSharkStarve[uTo] = m_pbSharkStarve[uFrom];
	m_pbSharkStarve[uFrom] = 0;
	m_pbSharkMove[uTo] = 1;
} // end - Sim1::SharkMove


void Sim1::SharkAddAge(const unsigned uPos,
	const unsigned uDelta)
{
	m_pbShark[uPos] += uDelta;
	m_pbSharkBreed[uPos] += uDelta;
	m_pbSharkStarve[uPos] += uDelta;
} // end - Sim1::SharkAddAge


void Sim1::SharkKill(const unsigned uPos)
{
	m_pbShark[uPos] = 0;
	m_pbSharkBreed[uPos] = 0;
	m_pbSharkStarve[uPos] = 0;
	m_pbSharkMove[uPos] = 0;
} // end - Sim1::SharkKill

}

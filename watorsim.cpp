

#include "stdafx.h"
#include "watorsim.h"

#ifdef _DEBUG
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


const char WatorSim::CHAR_FISH  = '+';
const char WatorSim::CHAR_SHARK = '#';
const char WatorSim::CHAR_WATER = ' ';

const unsigned WatorSim::DEFAULT_WIDTH		 = 100;
const unsigned WatorSim::DEFAULT_HEIGHT		 = 80;
const unsigned WatorSim::DEFAULT_FISHBREED 	 = 5;
const unsigned WatorSim::DEFAULT_SHARKBREED	 = 7;
const unsigned WatorSim::DEFAULT_SHARKSTARVE = 4;
const unsigned WatorSim::DEFAULT_FISHCOUNT	 = 200;
const unsigned WatorSim::DEFAULT_SHARKCOUNT  = 250;



WatorSim::WatorSim()
{
	Init(DEFAULT_WIDTH,DEFAULT_HEIGHT,DEFAULT_FISHBREED,DEFAULT_SHARKBREED,DEFAULT_SHARKSTARVE,DEFAULT_FISHCOUNT,DEFAULT_SHARKCOUNT);
} // end - WatorSim::WatorSim


WatorSim::WatorSim(const unsigned width,
				   const unsigned height,
				   const unsigned uFishBreed,
				   const unsigned uSharkBreed,
				   const unsigned uSharkStarve,
				   const unsigned uFishCount,
				   const unsigned uSharkCount)
{
	Init(width,height,uFishBreed,uSharkBreed,uSharkStarve,uFishCount,uSharkCount);
} // end - WatorSim::WatorSim


WatorSim::~WatorSim()
{
	delete[] m_pbFish; m_pbFish=NULL;
	delete[] m_pbFishMove; m_pbFishMove=NULL;
	delete[] m_pbFishBreed; m_pbFishBreed=NULL;
	delete[] m_pbShark; m_pbShark=NULL;
	delete[] m_pbSharkMove; m_pbSharkMove=NULL;
	delete[] m_pbSharkBreed; m_pbSharkBreed=NULL;
	delete[] m_pbSharkStarve; m_pbSharkStarve=NULL;
} // end - WatorSim::~WatorSim


void WatorSim::Init(const unsigned width,
				    const unsigned height,
				    const unsigned uFishBreed,
				    const unsigned uSharkBreed,
				    const unsigned uSharkStarve,
					const unsigned uFishCount,
					const unsigned uSharkCount)
{
	unsigned u = 0;

	m_uWidth		= width;
	m_uHeight		= height;
	m_sizField		= width * height;
	m_uFishBreed	= uFishBreed;
	m_uSharkBreed	= uSharkBreed;
	m_uSharkStarve	= uSharkStarve;
	m_uFishStart	= uFishCount;
	m_uSharkStart	= uSharkCount;
	m_pbFish		= new byte[m_sizField];
	m_pbFishMove	= new byte[m_sizField];
	m_pbFishBreed	= new byte[m_sizField];
	m_pbShark		= new byte[m_sizField];
	m_pbSharkMove	= new byte[m_sizField];
	m_pbSharkBreed	= new byte[m_sizField];
	m_pbSharkStarve = new byte[m_sizField];

	Reset();
} // end - WatorSim::Init


void WatorSim::SetConfig(const unsigned uFishBreed,
						 const unsigned uSharkBreed,
						 const unsigned uSharkStarve)
{
	m_uFishBreed   = uFishBreed;
	m_uSharkBreed  = uSharkBreed;
	m_uSharkStarve = uSharkStarve;
} // end - WatorSim::SetConfig


void WatorSim::Reset()
{
	unsigned u = 0;

	m_uFishCount	= 0;
	m_uSharkCount	= 0;
	m_uLoops		= 0;

	ClearFields();
	for(u=0; u<m_uFishStart; u++)
	{
		unsigned uRandom = 0;
		
		do
		{
			uRandom = (unsigned)(rand() % m_sizField);
		}
		while(m_pbFish[uRandom]!=0);
		FishAdd(uRandom);
		m_uFishCount++;
	}

	for(u=0; u<m_uSharkStart; u++)
	{
		unsigned uRandom = 0;
		
		do
		{
			uRandom = (unsigned)(rand() % m_sizField);
		}
		while(m_pbFish[uRandom]!=0 || m_pbShark[uRandom]!=0);
		SharkAdd(uRandom);
		m_uSharkCount++;
	}
} // end - WatorSim::Reset


void WatorSim::Get(char			*pcData,
				   const size_t sizData)
{
	unsigned u = 0;

	memset(pcData,CHAR_WATER,sizData);
	m_uFishCount = m_uSharkCount = 0;
	for(u=0; u<m_sizField && u<sizData; u++)
	{
		char c = 0;

		if(m_pbFish[u])
		{
			c = CHAR_FISH;
			m_uFishCount++;
		}
		else if(m_pbShark[u])
		{
			c = CHAR_SHARK;
			m_uSharkCount++;
		}
		else 
			c = CHAR_WATER;

		pcData[u] = c;
	}
} // end - WatorSim::Get


void WatorSim::Step()
{
	unsigned u = 0;

	memset(m_pbFishMove,0,m_sizField);
	memset(m_pbSharkMove,0,m_sizField);

	for(u=0; u<m_sizField; u++)
	{
		if(m_pbFish[u]!=0 && m_pbFishMove[u]==0)
			MoveFish(u);
		else if(m_pbShark[u]!=0 && m_pbSharkMove[u]==0)
			MoveShark(u);
	}

	m_uFishCount = m_uSharkCount = 0;
	for(u=0; u<m_sizField; u++)
	{
		if(m_pbFish[u]!=0)
			m_uFishCount++;
		if(m_pbShark[u]!=0)
			m_uSharkCount++;
	}

	m_uLoops++;
} // end - WatorSim::Step


void WatorSim::ClearFields()
{
	memset(m_pbFish,0,m_sizField);
	memset(m_pbFishMove,0,m_sizField);
	memset(m_pbFishBreed,0,m_sizField);
	memset(m_pbShark,0,m_sizField);
	memset(m_pbSharkMove,0,m_sizField);
	memset(m_pbSharkBreed,0,m_sizField);
	memset(m_pbSharkStarve,0,m_sizField);
} // end - WatorSim::ClearFields


void WatorSim::MoveFish(const unsigned uCurPos)
{
	unsigned puNeighbour[8];
	unsigned puFree[8];
	unsigned u		   = 0;
	unsigned uFree	   = 0;
	unsigned uMoveOfs  = -1;
	unsigned uBreedOfs = -1;

	memset(puNeighbour,0,sizeof(puNeighbour));
	memset(puFree,0,sizeof(puFree));

	GetNeighbours(uCurPos,puNeighbour);
	for(u=0; u<8; u++)
	{
		if(m_pbFish[puNeighbour[u]]==0 && m_pbShark[puNeighbour[u]]==0)
		{
			puFree[uFree] = puNeighbour[u];
			uFree++;
		}
	}

	if(uFree>1)
	{
		do
		{
			uBreedOfs = puFree[rand() % uFree];
			uMoveOfs = puFree[rand() % uFree];
		}
		while(uBreedOfs == uMoveOfs);
	}
	else if(uFree>0)
	{
		uBreedOfs = puFree[rand() % uFree];
		uMoveOfs = -1;
	}

	FishAddAge(uCurPos);
	if(m_pbFishBreed[uCurPos]>m_uFishBreed && uBreedOfs!=-1)
	{
		FishAdd(uBreedOfs);
		m_pbFishBreed[uCurPos] = 1;
	}
	if(uMoveOfs!=-1)
		FishMove(uCurPos,uMoveOfs);
} // end - WatorSim::MoveFish


void WatorSim::MoveShark(const unsigned uCurPos)
{
	unsigned uPos	   = uCurPos;
	unsigned puNeighbour[8];
	unsigned puFree[8];
	unsigned u		   = 0;
	unsigned uFree	   = 0;
	unsigned uMoveOfs  = -1;
	unsigned uBreedOfs = -1;

	memset(puNeighbour,0,sizeof(puNeighbour));
	memset(puFree,0,sizeof(puFree));

	GetNeighbours(uPos,puNeighbour);
	for(u=0; u<8; u++)
	{
		if(m_pbShark[puNeighbour[u]]==0)
		{
			puFree[uFree] = puNeighbour[u];
			uFree++;
		}
	}

	if(uFree>1)
	{
		do
		{
			uBreedOfs = puFree[rand() % uFree];
			uMoveOfs = puFree[rand() % uFree];
		}
		while(uBreedOfs == uMoveOfs);
	}
	else if(uFree>0)
	{
		uBreedOfs = puFree[rand() % uFree];
		uMoveOfs = -1;
	}

	SharkAddAge(uPos);
	if(m_pbSharkBreed[uPos]>m_uSharkBreed && uBreedOfs!=-1)
	{
		if(m_pbFish[uBreedOfs]!=0)
			FishKill(uBreedOfs);

		SharkAdd(uBreedOfs);
		m_pbSharkBreed[uPos] = 1;
	}
	if(uMoveOfs!=-1)
	{
		if(m_pbFish[uMoveOfs]!=0)
		{
			FishKill(uMoveOfs);
			m_pbSharkStarve[uPos] = 0;
		}

		SharkMove(uPos,uMoveOfs);
		uPos = uMoveOfs;
	}

	if(m_pbSharkStarve[uPos]>m_uSharkStarve)
		SharkKill(uPos);
} // end - WatorSim::MoveShark


void WatorSim::GetNeighbours(const unsigned uOfs,
							 unsigned		*puOfs)
{
	const unsigned x  = uOfs % m_uWidth;
	const unsigned y  = uOfs / m_uWidth;
	const unsigned x1 = (x % m_uWidth) ? x-1 : x+m_uWidth-1;
	const unsigned x2 = ((x % m_uWidth)!=(m_uWidth-1)) ? x+1 : x-m_uWidth+1;
	const unsigned y1 = (y % m_uHeight) ? y-1 : y+m_uHeight-1;
	const unsigned y2 = ((y % m_uHeight)!=(m_uHeight-1)) ? y+1 : y-m_uHeight+1;

	puOfs[0] = y1*m_uWidth + x1;
	puOfs[1] = y1*m_uWidth + x;
	puOfs[2] = y1*m_uWidth + x2;
	puOfs[3] = y*m_uWidth + x1;
	puOfs[4] = y*m_uWidth + x2;
	puOfs[5] = y2*m_uWidth + x1;
	puOfs[6] = y2*m_uWidth + x;
	puOfs[7] = y2*m_uWidth + x2;
} // end - WatorSim::GetFreeNeighbour


void WatorSim::FishAdd(const unsigned uPos)
{
	m_pbFish[uPos]		= 1;
	m_pbFishMove[uPos]  = 1;
	m_pbFishBreed[uPos] = 1;
} // end - WatorSim::FishAdd


void WatorSim::FishMove(const unsigned uFrom,
						const unsigned uTo)
{
	m_pbFish[uTo] = m_pbFish[uFrom];
	m_pbFish[uFrom] = 0;
	m_pbFishBreed[uTo] = m_pbFishBreed[uFrom];
	m_pbFishBreed[uFrom] = 0;
	m_pbFishMove[uTo] = 1;
} // end - WatorSim::FishMove


void WatorSim::FishAddAge(const unsigned uPos,
						  const unsigned uDelta)
{
	m_pbFish[uPos]+=uDelta;
	m_pbFishBreed[uPos]+=uDelta;
} // end - WatorSim::FishAddAge


void WatorSim::FishKill(const unsigned uPos)
{
	m_pbFish[uPos]		= 0;
	m_pbFishBreed[uPos] = 0;
	m_pbFishMove[uPos]  = 0;
} // end - WatorSim::FishKill


void WatorSim::SharkAdd(const unsigned uPos)
{
	m_pbShark[uPos] = 1;
	m_pbSharkMove[uPos] = 1;
} // end - WatorSim::SharkAdd


void WatorSim::SharkMove(const unsigned uFrom,
						 const unsigned uTo)
{
	m_pbShark[uTo]		   = m_pbShark[uFrom];
	m_pbShark[uFrom]	   = 0;
	m_pbSharkBreed[uTo]	   = m_pbSharkBreed[uFrom];
	m_pbSharkBreed[uFrom]  = 0;
	m_pbSharkStarve[uTo]   = m_pbSharkStarve[uFrom];
	m_pbSharkStarve[uFrom] = 0;
	m_pbSharkMove[uTo]	   = 1;
} // end - WatorSim::SharkMove


void WatorSim::SharkAddAge(const unsigned uPos,
						   const unsigned uDelta)
{
	m_pbShark[uPos]		  += uDelta;
	m_pbSharkBreed[uPos]  += uDelta;
	m_pbSharkStarve[uPos] += uDelta;
} // end - WatorSim::SharkAddAge


void WatorSim::SharkKill(const unsigned uPos)
{
	m_pbShark[uPos]		  = 0;
	m_pbSharkBreed[uPos]  = 0;
	m_pbSharkStarve[uPos] = 0;
	m_pbSharkMove[uPos]   = 0;
} // end - WatorSim::SharkKill

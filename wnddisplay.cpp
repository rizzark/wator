/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#include "stdafx.h"
#include "wnddisplay.h"
#include "watorsim.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

const std::wstring WndDisplay::CLASSNAME = L"Wator:WndClass:WndDisplay";



WndDisplay::WndDisplay() : tbase2::windows::gui::Wnd(),
						   m_pcData(NULL),
						   m_sizData(0),
						   m_uDataWidth(0),
						   m_uDataHeight(0),
						   m_crWater(RGB(0,0,0)),
						   m_crFish(RGB(0,0,0)),
						   m_crShark(RGB(0,0,0))
{
} // end - WndDisplay::WndDisplay


WndDisplay::~WndDisplay()
{
	delete[] m_pcData; m_pcData=NULL;
} // end - WndDisplay::~WndDisplay


bool WndDisplay::RegisterClass(HINSTANCE hinst)
{
	WNDCLASS wc;

	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
	wc.hCursor		 = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon		 = NULL;
	wc.hInstance	 = hinst;
	wc.lpfnWndProc	 = NULL;
	wc.lpszClassName = CLASSNAME.c_str();
	wc.lpszMenuName	 = NULL;
	wc.style		 = CS_HREDRAW | CS_VREDRAW;

	return Wnd::RegisterClass(wc);
} // end - WndDisplay::RegisterClass


bool WndDisplay::SetData(const char		*pcData,
						 const unsigned uWidth,
						 const unsigned uHeight)
{
	bool flReturn = false;

	if(pcData==NULL)
		throw tbase2::InvalidArgumentError(__TFILE__,__LINE__,__TFUNCTION__,_T("pcData"));
	if(uWidth==0 || uHeight==0)
		throw tbase2::InvalidArgumentError(__TFILE__,__LINE__,__TFUNCTION__,_T("uWidth/uHeight"));

	const size_t sizRequired = uWidth * uHeight;

	if(m_sizData<=sizRequired)
	{
		delete[] m_pcData; m_pcData=new char[sizRequired];
		if(m_pcData==NULL)
			throw tbase2::AllocationError(__TFILE__,__LINE__,_T("new char[sizRequired]"),sizRequired*sizeof(char));
		m_sizData= sizRequired;
	}

	m_uDataWidth = uWidth;
	m_uDataHeight = uHeight;

	memcpy(m_pcData,pcData,sizRequired);
	flReturn = true;

	return flReturn;
} // end - WndDisplay::SetData


bool WndDisplay::OnPaint()
{
	if(m_pcData==NULL)
		return true;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(*this,&ps);

	try
	{
		tbase2::windows::gdi::DeviceContext dc(hdc);
		const size_t				sizData = m_uDataWidth * m_uDataHeight;
		const COLORREF				crWhite = GetSysColor(COLOR_WINDOW);
		tbase2::windows::gdi::Brush brWater(m_crWater);
		tbase2::windows::gdi::Brush brFish(m_crFish);
		tbase2::windows::gdi::Brush brShark(m_crShark);
//		tbase2::windows::gdi::Brush brDiagram(m_
		tbase2::windows::gdi::Pen   pnWater(PS_SOLID,1,m_crWater);
		tbase2::windows::gdi::Pen   pnFish(PS_SOLID,1,m_crFish);
		tbase2::windows::gdi::Pen   pnShark(PS_SOLID,1,m_crShark);
		tbase2::windows::gdi::Pen	pnWhite(PS_SOLID,1,crWhite);
		tbase2::windows::gdi::Rect	rcClient;
		unsigned					x		= 0;
		unsigned					y	    = 0;


		GetClientRect(rcClient);
		const unsigned uUnitW = rcClient.width() / m_uDataWidth;
		const unsigned uUnitH = rcClient.height() / m_uDataHeight;

		dc.SetPen(pnWhite);
		for(unsigned u=0; u<sizData; u++)
		{
			if(m_pcData[u]==WatorSim::CHAR_FISH)
			{
				dc.SetBrush(brFish);
				dc.SetPen(pnFish);
			}
			else if(m_pcData[u]==WatorSim::CHAR_SHARK)
			{
				dc.SetBrush(brShark);
				dc.SetPen(pnShark);
			}
			else
			{
				dc.SetBrush(brWater);
				dc.SetPen(pnWater);
			}

			if(uUnitW>4 && uUnitH>4)
				dc.SetPen(pnWhite);

			Rectangle(dc,x,y,x+uUnitW,y+uUnitH);
			x += uUnitW;
			if((u+1)%m_uDataWidth==0)
			{
				x = 0;
				y += uUnitH;
			}
		}
		EndPaint(*this,&ps); hdc=NULL;
	}
	catch(...)
	{
		EndPaint(*this,&ps); hdc=NULL;
	}

	return true;
} // end - WndDisplay::OnPaint


bool WndDisplay::OnDestroy()
{
	delete[] m_pcData; m_pcData=NULL;
	return true;
} // end - WndDisplay::OnDestroy

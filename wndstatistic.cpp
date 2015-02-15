/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#include "stdafx.h"
#include "wndstatistic.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

extern tbase2::windows::Module gl_Module;


const std::wstring WndStatistic::CLASSNAME  = L"Wator:WndClass:WndStatistic";
const unsigned	   WndStatistic::SAVECYCLES = 4000;


WndStatistic::WndStatistic() : tbase2::windows::gui::Wnd(),
							   m_crFish(RGB(0,0,0)),
							   m_crShark(RGB(0,0,0)),
							   m_uMax(0)
{
} // end - WndStatistic::WndStatistic


WndStatistic::~WndStatistic()
{
} // end - WndStatistic::~WndStatistic


bool WndStatistic::RegisterClass(HINSTANCE hinst)
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
} // end - WndStatistic::RegisterClass


void WndStatistic::AddData(const unsigned uFishCount,
						   const unsigned uSharkCount,
						   const unsigned uMax)
{
	if(m_vFishCount.size()==SAVECYCLES)
	{
//		m_arrFishCount.shl();
//		m_arrSharkCount.shl();
	}

	m_uMax = uMax;
	m_vFishCount.push_back(uFishCount);
	m_vSharkCount.push_back(uSharkCount);
	m_uCycles++;

	TBASE2_ASSERT(m_vSharkCount.size()==m_vFishCount.size());
} // end - WndStatistic::AddData


void WndStatistic::ClearData()
{
	m_vFishCount.clear();
	m_vSharkCount.clear();
	m_uCycles = 0;
} // end - WndStatistic::ClearData


bool WndStatistic::OnCreate(CREATESTRUCT *pcs,
							LRESULT		 &lres)
{
	ClearData();

	m_vFishCount.push_back(0);
	m_vSharkCount.push_back(0);

	return 0;
} // end - WndStatistic::OnCreate


bool WndStatistic::OnPaint()
{
	if(m_vSharkCount.size()==0)
		return true;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(*this,&ps);

	try
	{
		tbase2::windows::gdi::DeviceContext dc(hdc);
		const COLORREF					    crDiagram = GetSysColor(COLOR_BTNTEXT);
		tbase2::windows::gdi::Font			fnt(dc,12,gl_strDlgFont.c_str());
		tbase2::windows::gdi::Pen			pnFish(PS_SOLID,1,m_crFish);
		tbase2::windows::gdi::Pen			pnShark(PS_SOLID,1,m_crShark);
		tbase2::windows::gdi::Pen			pnDiagram(PS_SOLID,1,crDiagram);
		tbase2::windows::gdi::Font			fntDiagram(dc,6,gl_strDlgFont.c_str());
		tbase2::windows::gdi::Rect			rcClient;

		GetClientRect(&rcClient);

		dc.SetFont(fnt);
		dc.SetTextColor(crDiagram);

		dc.SetTextAlign(TA_LEFT|TA_TOP);
		dc.TextOut(5, 5,gl_Module.String(IDS_CYCLES).c_str());
		dc.TextOut(5,30,gl_Module.String(IDS_FISHES).c_str());
		dc.TextOut(5,55,gl_Module.String(IDS_SHARKS).c_str());
				
		dc.SetTextAlign(TA_RIGHT|TA_TOP);

		TBASE2_ASSERT(m_vFishCount.size()>0 && m_vSharkCount.size()>0);
		dc.TextOut(150, 5,std::to_wstring(m_uCycles));
		dc.TextOut(150,30,std::to_wstring(m_vFishCount[m_vFishCount.size()-1]));
		dc.TextOut(150,55,std::to_wstring(m_vSharkCount[m_vSharkCount.size()-1]));

		const unsigned uWidth = rcClient.width();
//		uHeight = rcClient.bottom - rcClient.top;

		if(uWidth-160 > 100)
		{
			const unsigned uDiaMax	  = rcClient.height() - 10;
			std::wstring   wstrMax	  = std::to_wstring(m_uMax);
			std::wstring   wstrHalf	  = std::to_wstring(m_uMax/2);
			unsigned	   x		  = rcClient.width() - 50;
			unsigned	   y          = rcClient.height() - 5;
			int			   iOfs		  = static_cast<int>(m_vSharkCount.size()-1);
			int			   iFishLast  = -1;
			int			   iSharkLast = -1;

			dc.SetPen(pnDiagram);
			MoveToEx(hdc,160,y+1,NULL);
			LineTo(hdc,x+1,y+1);
			LineTo(hdc,x+1,y-uDiaMax);
			LineTo(hdc,x+5,y-uDiaMax);
			MoveToEx(hdc,x+1,y-uDiaMax/2,NULL);
			LineTo(hdc,x+5,y-uDiaMax/2);

			dc.SetFont(fntDiagram);
			SetTextAlign(hdc,TA_LEFT|TA_TOP);
			SetTextColor(hdc,crDiagram);
			dc.TextOut(x+6,y-uDiaMax,wstrMax);
			dc.TextOut(x+6,y-uDiaMax/2,wstrHalf);


			for(; x>160 && iOfs>0; x--,iOfs--)
			{
				unsigned uShark = uDiaMax * m_vSharkCount[iOfs] / m_uMax;
				unsigned uFish  = uDiaMax * m_vFishCount[iOfs] / m_uMax;

				dc.SetPen(pnFish);
				if(iFishLast==-1)
				{
					iFishLast = y-uFish;
					MoveToEx(hdc,x,iFishLast,NULL);
				}
				else
				{
					MoveToEx(hdc,x+1,iFishLast,NULL);
					iFishLast = y-uFish;
					LineTo(hdc,x,iFishLast);
				}

				dc.SetPen(pnShark);
				if(iSharkLast==-1)
				{
					iSharkLast = y-uShark;
					MoveToEx(hdc,x,iSharkLast,NULL);
				}
				else
				{
					MoveToEx(hdc,x+1,iSharkLast,NULL);
					iSharkLast = y-uShark;
					LineTo(hdc,x,iSharkLast);
				}
			}
		}
		EndPaint(*this,&ps); hdc=NULL;
	}
	catch(...)
	{
		EndPaint(*this,&ps); hdc=NULL;
	}

	return true;
} // end - WndStatistic::OnPaint




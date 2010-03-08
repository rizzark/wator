

#include "stdafx.h"
#include "wndstatistic.h"

#ifdef _DEBUG
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

const tk::string WndStatistic::CLASSNAME  = "Wator:WndClass:WndStatistic";
const unsigned	 WndStatistic::SAVECYCLES = 4000;


TKW32_BEGIN_MESSAGETABLE(WndStatistic)
	TKW32_WM_CREATE(OnCreate)
	TKW32_WM_PAINT(OnPaint)
	TKW32_WM_DESTROY(OnDestroy)
TKW32_END_MESSAGETABLE(tkw32::Wnd)


WndStatistic::WndStatistic() : tkw32::Wnd(),
							   m_crFish(RGB(0,0,0)),
							   m_crShark(RGB(0,0,0)),
							   m_uMax(0)
{
	m_arrSharkCount.resize(SAVECYCLES,0);
	m_arrFishCount.resize(SAVECYCLES,0);
} // end - WndStatistic::WndStatistic


WndStatistic::~WndStatistic()
{
} // end - WndStatistic::~WndStatistic


bool WndStatistic::RegisterClass(HINSTANCE hinst)
{
	WNDCLASS wc;

	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.hCursor		 = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon		 = NULL;
	wc.hInstance	 = hinst;
	wc.lpfnWndProc	 = NULL;
	wc.lpszClassName = CLASSNAME;
	wc.lpszMenuName	 = NULL;
	wc.style		 = CS_HREDRAW | CS_VREDRAW;

	return Wnd::RegisterClass(wc);
} // end - WndStatistic::RegisterClass


void WndStatistic::AddData(const unsigned uFishCount,
						   const unsigned uSharkCount,
						   const unsigned uMax)
{
	TKASSERT(m_arrFishCount.getsize()==m_arrSharkCount.getsize(),"Array size differs");

	if(m_arrFishCount.getsize()==SAVECYCLES)
	{
		m_arrFishCount.shl();
		m_arrSharkCount.shl();
	}

	m_uMax = uMax;
	m_arrFishCount.add(uFishCount);
	m_arrSharkCount.add(uSharkCount);
	m_uCycles++;
} // end - WndStatistic::AddData


void WndStatistic::ClearData()
{
	m_arrFishCount.clear(false);
	m_arrSharkCount.clear(false);
	m_uCycles = 0;
} // end - WndStatistic::ClearData


LRESULT WndStatistic::OnCreate(CREATESTRUCT *pcs)
{
	m_arrSharkCount.clear(false);
	m_arrFishCount.clear(false);

	m_arrSharkCount.add(0);
	m_arrFishCount.add(0);
	m_uCycles = 0;

	return 0;
} // end - WndStatistic::OnCreate


void WndStatistic::OnPaint(HDC		   hdc,
						   PAINTSTRUCT &ps)
{
	TKASSERT(m_arrFishCount.getsize()==m_arrSharkCount.getsize(),"Array size differs");

	tkw32::Font	   fnt;
	tkw32::Pen	   pnFish;
	tkw32::Pen	   pnShark;
	tkw32::Pen	   pnDiagram;
	const COLORREF crDiagram = GetSysColor(COLOR_BTNTEXT);
	tkw32::Font	   fntDiagram;

	if(m_arrSharkCount.getsize()==0)
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,-1,"No data available!");
	else if(!fnt.CreatePointFont(hdc,12,"MS Shell Dlg"))
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create font!");
	else if(!pnFish.CreatePen(PS_SOLID,1,m_crFish))
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create pen!");
	else if(!pnShark.CreatePen(PS_SOLID,1,m_crShark))
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create pen!");
	else if(!pnDiagram.CreatePen(PS_SOLID,1,crDiagram))
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create pen!");
	else if(!fntDiagram.CreatePointFont(hdc,6,"MS Shell Dlg"))
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create font!");
	else
	{
		RECT		rcClient;
		unsigned	uWidth    = 0;
		unsigned	uHeight   = 0;
		tk::string  strMsg	  = "";
		tk::string  strCycles = tk::string::mkString("%u",m_uCycles);
		tk::string  strSharks = tk::string::mkString("%u",m_arrSharkCount[(unsigned)m_arrSharkCount.getsize()-1]);
		tk::string  strFish   = tk::string::mkString("%u",m_arrFishCount[(unsigned)m_arrFishCount.getsize()-1]);

		GetClientRect(*this,&rcClient);
		uWidth = rcClient.right - rcClient.left;
		uHeight = rcClient.bottom - rcClient.top;

		SelectFont(hdc,fnt);
		SetTextColor(hdc,crDiagram);

		SetTextAlign(hdc,TA_LEFT|TA_TOP);
		strMsg = "Cycles:";
		TextOut(hdc,5,5,strMsg,(int)strMsg.length());
		strMsg = "Fishes:";
		TextOut(hdc,5,30,strMsg,(int)strMsg.length());
		strMsg = "Sharks:";
		TextOut(hdc,5,55,strMsg,(int)strMsg.length());

		
		SetTextAlign(hdc,TA_RIGHT|TA_TOP);
		TextOut(hdc,150,5,strCycles,(int)strCycles.length());
		TextOut(hdc,150,30,strFish,(int)strFish.length());
		TextOut(hdc,150,55,strSharks,(int)strSharks.length());


		if(uWidth-160 > 100)
		{
			const unsigned uDiaMax	  = uHeight - 10;
			tk::string	   strMax	  = tk::string::mkString("%u",m_uMax);
			tk::string	   strHalf	  = tk::string::mkString("%u",m_uMax/2);
			unsigned	   x		  = uWidth - 50;
			unsigned	   y          = uHeight - 5;
			int			   iOfs		  = (int)m_arrSharkCount.getsize()-1;
			int			   iFishLast  = -1;
			int			   iSharkLast = -1;

			SelectPen(hdc,pnDiagram);
			MoveToEx(hdc,160,y+1,NULL);
			LineTo(hdc,x+1,y+1);
			LineTo(hdc,x+1,y-uDiaMax);
			LineTo(hdc,x+5,y-uDiaMax);
			MoveToEx(hdc,x+1,y-uDiaMax/2,NULL);
			LineTo(hdc,x+5,y-uDiaMax/2);

			SelectFont(hdc,fntDiagram);
			SetTextAlign(hdc,TA_LEFT|TA_TOP);
			SetTextColor(hdc,crDiagram);
			TextOut(hdc,x+6,y-uDiaMax,strMax,(int)strMax.length());
			TextOut(hdc,x+6,y-uDiaMax/2,strHalf,(int)strHalf.length());


			for(; x>160 && iOfs>0; x--,iOfs--)
			{
				unsigned uShark = uDiaMax * m_arrSharkCount[iOfs] / m_uMax;
				unsigned uFish  = uDiaMax * m_arrFishCount[iOfs] / m_uMax;

				SelectPen(hdc,pnFish);
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

				SelectPen(hdc,pnShark);
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
	}
} // end - WndStatistic::OnPaint


void WndStatistic::OnDestroy()
{
} // end - WndStatistic::OnDestroy


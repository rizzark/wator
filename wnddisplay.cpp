

#include "stdafx.h"
#include "wnddisplay.h"
#include "watorsim.h"

#ifdef _DEBUG
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

const tk::string WndDisplay::CLASSNAME = "Wator:WndClass:WndDisplay";


TKW32_BEGIN_MESSAGETABLE(WndDisplay)
	TKW32_WM_PAINT(OnPaint)
	TKW32_WM_DESTROY(OnDestroy)
TKW32_END_MESSAGETABLE(tkw32::Wnd)


WndDisplay::WndDisplay() : tkw32::Wnd(),
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.hCursor		 = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon		 = NULL;
	wc.hInstance	 = hinst;
	wc.lpfnWndProc	 = NULL;
	wc.lpszClassName = CLASSNAME;
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
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,-1,"No buffer specified!");
	else if(uWidth==0 || uHeight==0)
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,-1,"Invalid buffer size specified!");
	else
	{
		const size_t sizRequired = uWidth * uHeight;

		if(m_sizData<=sizRequired)
		{
			delete[] m_pcData; m_pcData=new char[sizRequired];
			m_sizData= sizRequired;
		}

		m_uDataWidth = uWidth;
		m_uDataHeight = uHeight;

		memcpy(m_pcData,pcData,sizRequired);
		flReturn = true;
	}

	return flReturn;
} // end - WndDisplay::SetData


void WndDisplay::OnPaint(HDC		 hdc,
						 PAINTSTRUCT &ps)
{
	if(hdc==NULL)
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"BeginPaint failed");
	else if(m_pcData)
	{
		const size_t   sizData = m_uDataWidth * m_uDataHeight;
		tkw32::Brush   brWater;
		tkw32::Brush   brFish;
		tkw32::Brush   brShark;
		tkw32::Brush   brDiagram;
		tkw32::Pen     pnWater;
		tkw32::Pen     pnFish;
		tkw32::Pen     pnShark;
		tkw32::Pen	   pnWhite;
		const COLORREF crWhite   = GetSysColor(COLOR_WINDOW);


		if(!brWater.CreateSolidBrush(m_crWater))
			tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create brush");
		else if(!brFish.CreateSolidBrush(m_crFish))
			tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create brush");
		else if(!brShark.CreateSolidBrush(m_crShark))
			tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create brush");
		else if(!pnWater.CreatePen(PS_SOLID,1,m_crWater))
			tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create pen");
		else if(!pnFish.CreatePen(PS_SOLID,1,m_crFish))
			tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create pen");
		else if(!pnShark.CreatePen(PS_SOLID,1,m_crShark))
			tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create pen");
		else if(!pnWhite.CreatePen(PS_SOLID,1,crWhite))
			tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create pen");
		else
		{
			RECT		   rcClient;
			unsigned	   uWidth    = 0;
			unsigned	   uHeight   = 0;
			unsigned	   uUnitW    = 0;
			unsigned	   uUnitH    = 0;
			unsigned	   uFrame    = 0;
			unsigned	   u	     = 0;
			unsigned	   x		 = 0;
			unsigned	   y	     = 0;
			unsigned	   uShark		= 0;
			unsigned	   uFish		= 0;

			GetClientRect(*this,&rcClient);

			uWidth  = rcClient.right - rcClient.left;
			uHeight = rcClient.bottom - rcClient.top;

			uUnitW = uWidth / m_uDataWidth;
			uUnitH = uHeight / m_uDataHeight;


			SelectPen(hdc,pnWhite);
			for(u=0; u<sizData; u++)
			{
				if(m_pcData[u]==WatorSim::CHAR_FISH)
				{
					SelectBrush(hdc,brFish);
					SelectPen(hdc,pnFish);
				}
				else if(m_pcData[u]==WatorSim::CHAR_SHARK)
				{
					SelectBrush(hdc,brShark);
					SelectPen(hdc,pnShark);
				}
				else
				{
					SelectBrush(hdc,brWater);
					SelectPen(hdc,pnWater);
				}

				if(uUnitW>4 && uUnitH>4)
					SelectPen(hdc,pnWhite);

				Rectangle(hdc,x,y,x+uUnitW,y+uUnitH);
				x += uUnitW;
				if((u+1)%m_uDataWidth==0)
				{
					x = 0;
					y += uUnitH;
				}
			}
/*
			uShark = m_wator.GetSharkCount() * (uFrameBottom-10) / uMaxUnits;
			uFish  = m_wator.GetFishCount() * (uFrameBottom-10) / uMaxUnits;

			SelectBrush(hdc,brDiagram);
			SelectPen(hdc,pnDiagram);
			y = rcClient.bottom - 5;
			Rectangle(hdc,0,y,uDiaWidth,rcClient.bottom-95);
			

			SelectPen(hdc,pnFish);
			for(u=0; u<m_uOfs; u++)
			{
				uFish = m_puFish[u] * (uFrameBottom-10) / uMaxUnits;

				if(u==0)
					MoveToEx(hdc,u,y-2,NULL);
				else
					LineTo(hdc,u,y-uFish-2);
			}
			SelectPen(hdc,pnShark);
			for(u=0; u<m_uOfs; u++)
			{
				uFish = m_puShark[u] * (uFrameBottom-10) / uMaxUnits;

				if(u==0)
					MoveToEx(hdc,u,y-2,NULL);
				else
					LineTo(hdc,u,y-uFish-2);
			}

			nomfc::string strFish  = "Fishes:";
			nomfc::string strShark = "Sharks:";
			nomfc::string strLoops = "Iterations:";

			SelectFont(hdc,fnt);
			SetTextAlign(hdc,TA_LEFT|TA_TOP);
			SetTextColor(hdc,crDiagram);
			y = rcClient.bottom-95;
			TextOut(hdc,uDiaWidth+5,y,strFish,(int)strFish.length());
			TextOut(hdc,uDiaWidth+5,y+20,strShark,(int)strShark.length());
			TextOut(hdc,uDiaWidth+5,y+40,strLoops,(int)strLoops.length());

			strFish  = nomfc::string::mkString("%u",m_wator.GetFishCount());
			strShark = nomfc::string::mkString("%u",m_wator.GetSharkCount()); 
			strLoops = nomfc::string::mkString("%u",m_wator.GetIterations());
			SetTextAlign(hdc,TA_RIGHT|TA_TOP);
			TextOut(hdc,uDiaWidth+200,y,strFish,(int)strFish.length());
			TextOut(hdc,uDiaWidth+200,y+20,strShark,(int)strShark.length());
			TextOut(hdc,uDiaWidth+200,y+40,strLoops,(int)strLoops.length());
*/
		}
	}
} // end - WndDisplay::OnPaint


void WndDisplay::OnDestroy()
{
	delete[] m_pcData; m_pcData=NULL;
} // end - WndDisplay::OnDestroy

/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once

#include "stdafx.h"
#include "wndwator.h"
#include "dlgsettings.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

extern tbase2::windows::Module gl_Module;


static const UINT_PTR TIMER_STEP = 111;

static const UINT_PTR PBT_SETTINGS   = 1000;
static const UINT_PTR WND_DISPLAY    = 1001;
static const UINT_PTR WND_STATISTIC  = 1002;
static const UINT_PTR PBT_STARTSTOP  = 1003;
static const UINT_PTR PBT_SINGLESTEP = 1004;
static const UINT_PTR PBT_RESTART	 = 1005;
static const UINT_PTR SLD_INTERVALL  = 1006;



const std::wstring WndWator::CLASSNAME = L"Wator:WndClass:WndWator";


WndWator::WndWator(WatorSim			 &wator,
				   const std::locale &locale) : tbase2::windows::gui::Wnd(),
												m_wator(wator),
												m_locale(locale),
												m_ltDlg(5,5),
												m_ltSim(0,2),
												m_ltButtons(0,2),
												m_Renderer(locale),
												m_flRunning(false),
												m_iDisplayPane(-1),
												m_iStatisticPane(-1),
												m_uIntervall(DlgSettings::DEFAULT_INTERVALL)
{
	try
	{
		static const DWORD dwStyle  = WS_CAPTION | 
									  WS_SYSMENU | 
									  WS_MINIMIZEBOX | 
									  WS_MAXIMIZEBOX | 
									  WS_VISIBLE | 
									  WS_THICKFRAME;

		if(!RegisterClass(gl_Module))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("WndWator::RegisterClass"),GetLastError());

		const int x		 = CW_USEDEFAULT;
		const int y		 = CW_USEDEFAULT;
		const int width  = CW_USEDEFAULT;
		const int height = CW_USEDEFAULT;

		CreateEx(WndWator::CLASSNAME.c_str(),gl_Module.String(IDS_TITLE).c_str(),dwStyle,WS_EX_CONTROLPARENT,x,y,width,height,NULL,NULL,gl_Module,NULL);
	}
	catch(std::exception &error)
	{
	}
	catch(...)
	{
	}
} // end - WndWator::WndWator


WndWator::~WndWator()
{
} // end - WndWator::~WndWator


bool WndWator::RegisterClass(HINSTANCE hinst)
{
	bool flReturn = false;

	try
	{
		WNDCLASS wc;

		wc.cbClsExtra	 = 0;
		wc.cbWndExtra	 = 0;
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE+1);
		wc.hCursor		 = LoadCursor(NULL,IDC_ARROW);
		wc.hIcon		 = NULL;
		wc.hInstance	 = hinst;
		wc.lpfnWndProc	 = NULL;
		wc.lpszClassName = CLASSNAME.c_str();
		wc.lpszMenuName	 = NULL;
		wc.style		 = CS_HREDRAW | CS_VREDRAW;

		flReturn = Wnd::RegisterClass(wc);
	}
	catch(...)
	{
		flReturn = false;
	}

	return flReturn;
} // end - WndWator::RegisterClass


bool WndWator::PreTranslateMsg(MSG &msg)
{
	return IsDialogMessage(*this,&msg) ? true : false;
} // end - WndWator::PreTranslateMsg


void WndWator::PostRestart()
{
	PostMessage(WM_COMMAND,PBT_RESTART,0);
} // end - WndWator::PostRestart

			
bool WndWator::OnCreate(CREATESTRUCT *pcs,
						LRESULT		 &lres)
{
	try
	{
		const DWORD							dwButtonStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
		const DWORD							dwSliderStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
		const DWORD							dwTxtStyle	  = WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE;
		const int							iButtonW	  = 80;
		const int							iButtonH	  = 20;
		tbase2::windows::gdi::DeviceContext dc(*this);

		if(!m_fntDlg.CreatePointFont(dc,8,gl_strDlgFont.c_str()))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("m_fntDlg.CreatePointFont"),GetLastError());
		
		m_pbtSettings.Create(*this,dwButtonStyle,0,PBT_SETTINGS,gl_Module.String(IDS_SETTINGS).c_str(),0,0,iButtonW,iButtonH);
		m_pbtStartStop.Create(*this,dwButtonStyle,0,PBT_STARTSTOP,gl_Module.String(IDS_START).c_str(),0,0,iButtonW,iButtonH);
		m_pbtSingleStep.Create(*this,dwButtonStyle,0,PBT_SINGLESTEP,gl_Module.String(IDS_SINGLESTEP).c_str(),0,0,iButtonW,iButtonH);
		m_pbtRestart.Create(*this,dwButtonStyle,0,PBT_RESTART,gl_Module.String(IDS_RESTART).c_str(),0,0,iButtonW,iButtonH);
		m_sldIntervall.Create(*this,dwSliderStyle,0,SLD_INTERVALL,0,0,iButtonW,iButtonH);
		m_stcIntervall.Create(*this,dwTxtStyle,0,-1,0,0,iButtonW,iButtonH);

		//m_stcIntervall.SetText(gl_Module.String(IDS_INTERVALL));

		m_sldIntervall.SetRange(30,100,false);
		m_sldIntervall.SetPos(m_uIntervall,true);

		m_pbtSettings.SetFont(m_fntDlg,true);
		m_pbtStartStop.SetFont(m_fntDlg,true);
		m_pbtSingleStep.SetFont(m_fntDlg,true);
		m_pbtRestart.SetFont(m_fntDlg,true);

		m_ltButtons.AddItem(-1,m_pbtSettings,false,false);
		m_ltButtons.AddItem(-1,m_pbtStartStop,false,false);
		m_ltButtons.AddItem(-1,m_pbtSingleStep,false,false);
		m_ltButtons.AddItem(-1,m_pbtRestart,false,false);
		m_ltButtons.AddItem(-1,m_stcIntervall,false,false);
		m_ltButtons.AddItem(-1,m_sldIntervall,false,false);
		m_ltButtons.AddItem(-1,0,0,false,true);

		m_ltSim.AddItem(-1,160,200,true,true);

		m_ltDlg.AddItem(-1,&m_ltSim,true,true);
		m_ltDlg.AddItem(-1,&m_ltButtons,false,true);

		SetLayout(&m_ltDlg);

		OnStartStop();
		lres = 0;
	}
	catch(...)
	{
		lres = -1;
	}

	return true;
} // end - WndWator::OnCreate


bool WndWator::OnTimer(UINT_PTR  uTimer,
					   TIMERPROC proc)
{
	bool flReturn = true;

	if(uTimer==TIMER_STEP)
		OnSingleStep();
	else
		flReturn = false;

	return flReturn;
} // end - WndWator::OnTimer


bool WndWator::OnCommand(WORD wIDFrom,
						 WORD wType,
						 HWND hwndFrom)
{
	bool flReturn = true;

	if(wIDFrom==PBT_SETTINGS)
		OnSettings();
	else if(wIDFrom==PBT_STARTSTOP)
		OnStartStop();
	else if(wIDFrom==PBT_SINGLESTEP)
		OnSingleStep();
	else if(wIDFrom==PBT_RESTART)
		OnRestart();
	else
		flReturn = false;

	return flReturn;
} // end - Wator::OnCommand


bool WndWator::OnHScroll(WORD nScroll,		//!< Scroll-command
						 WORD nPos,		//!< Scroll position for SB_THUMBPOSITON of SB_THUMBTRACK
						 HWND hwndFrom)	//!< Sending window, NULL for standard scroll bar
{
	bool flReturn = true;

	if(hwndFrom==m_sldIntervall)
	{
		std::wstringstream wssDbg;
		if(nScroll==TB_ENDTRACK)
		{
			const unsigned	   uPos = m_sldIntervall.GetPos();

			wssDbg << L"Pos: " << uPos << std::endl;
			OutputDebugString(wssDbg.str().c_str());

			m_uIntervall = uPos;
			KillTimer(TIMER_STEP);
			SetTimer(TIMER_STEP,m_uIntervall);
		}
		flReturn = true;
	}
	else
		flReturn = false;

	return flReturn;
} // end - WndWator::OnHScroll


bool WndWator::OnClose()
{
	PostQuitMessage(0);
	return true;
} // end - WndWator::OnClose


bool WndWator::OnDestroy()
{
	m_pbtSettings.DestroyWindow();
	m_pbtStartStop.DestroyWindow();
	m_pbtSingleStep.DestroyWindow();
	m_pbtRestart.DestroyWindow();
	m_sldIntervall.DestroyWindow();

	KillTimer(TIMER_STEP);

	return true;
} // end - WndWator::OnDestroy


bool WndWator::OnPaint()
{
	PAINTSTRUCT ps;
	HDC			hdc = BeginPaint(*this,&ps);

	try
	{
		tbase2::windows::gdi::DeviceContext dc(hdc);

		UpdateDisplay(dc);
		EndPaint(*this,&ps); hdc=NULL;
	}
	catch(...)
	{
		EndPaint(*this,&ps); hdc=NULL;
	}

	return true;
} // end - WndWator::OnPaint


void WndWator::OnStartStop()
{
	if(m_flRunning)
	{
		KillTimer(TIMER_STEP);
		m_pbtStartStop.SetText(gl_Module.String(IDS_START).c_str());
		m_flRunning = false;
	}
	else
	{
		SetTimer(TIMER_STEP,m_uIntervall);
		m_pbtStartStop.SetText(gl_Module.String(IDS_STOP).c_str());
		m_flRunning = true;
	}

	m_pbtSingleStep.EnableWindow(!m_flRunning);
} // end - WndWator::OnStartStop


void WndWator::OnSettings()
{
	DlgSettings dlg(m_wator,m_Renderer,m_locale);
	bool		flStartAgain = false;

	if(m_flRunning)
	{
		OnStartStop();
		flStartAgain = true;
	}

	if(dlg.Modal(*this)==IDOK)
		Invalidate();

	if(flStartAgain)
		OnStartStop();
} // end - WndWator::OnSettings


void WndWator::OnSingleStep()
{
	tbase2::windows::gdi::DeviceContext dc(*this);

	m_wator.Step();
	const unsigned uFishCount  = m_wator.GetFishCount();
	const unsigned uSharkCount = m_wator.GetSharkCount();
	UpdateDisplay(dc);

	if(uFishCount==0 || uSharkCount==0)
	{
		std::wstring wstrMsg;

		if(uSharkCount==0)
			wstrMsg = gl_Module.String(IDS_ALL_SHARK_DEAD);
		else
			wstrMsg = gl_Module.String(IDS_ALL_FISH_DEAD);

		OnStartStop();
		MessageBox(*this,wstrMsg.c_str(),L"",MB_OK|MB_ICONINFORMATION);
		PostMessage(WM_COMMAND,PBT_RESTART,0);
	}
} // end - WndWator::OnSingleStep


void WndWator::OnRestart()
{
	DlgSettings dlg(m_wator,m_Renderer,m_locale,true);

	if(m_flRunning)
		OnStartStop();

	if(dlg.Modal(*this)==IDOK)
	{
		m_wator.Reset();
		if(!m_flRunning)
			OnStartStop();
	}
	m_flStartup = false;
} // end - WndWator::OnRestart


void WndWator::UpdateDisplay(tbase2::windows::gdi::DeviceContext &dc)
{
	try
	{
		tbase2::windows::gdi::Rect			rcRender = m_ltSim.GetRect();
		tbase2::windows::gdi::Bitmap		bmpBuffer(dc,rcRender.width(),rcRender.height());
		tbase2::windows::gdi::DeviceContext dcTmp(dc,bmpBuffer);

		m_Renderer.Render(m_wator,dcTmp,rcRender.width(),rcRender.height());
		BitBlt(dc,0,0,rcRender.width(),rcRender.height(),dcTmp,0,0,SRCCOPY);
	}
	catch(std::exception &error)
	{
		tbase2::debug::Rethrow(__TFILE__,__LINE__,__TFUNCTION__,error);
	}
	catch(...)
	{
		throw tbase2::UnexpectedError(__TFILE__,__LINE__,__TFUNCTION__);
	}
} // end - WndWator::UpdateDisplay

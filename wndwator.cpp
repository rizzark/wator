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


const std::wstring WndWator::CLASSNAME = L"Wator:WndClass:WndWator";


WndWator::WndWator(WatorSim &wator) : tbase2::windows::gui::Wnd(),
									  m_wator(wator),
									  m_ltDlg(0,2),
									  m_ltSim(0,2),
									  m_ltButtons(0,2),
									  m_pcData(NULL),
									  m_sizData(0),
									  m_crWater(RGB(255,255,255)),
									  m_crFish(RGB(0,255,0)),
									  m_crShark(RGB(255,0,0)),
									  m_flRunning(false)
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

		if(!CreateEx(WndWator::CLASSNAME.c_str(),gl_Module.String(IDS_TITLE).c_str(),dwStyle,WS_EX_CONTROLPARENT,x,y,width,height,NULL,NULL,gl_Module,NULL))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("wnd.CreateEx"),GetLastError());
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
	delete[] m_pcData; m_pcData=NULL;
} // end - WndWator::~WndWator


bool WndWator::RegisterClass(HINSTANCE hinst)
{
	bool flReturn = false;

	try
	{
		if(!WndDisplay::RegisterClass(hinst))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("WndDisplay::RegisterClass"),GetLastError());
		if(!WndStatistic::RegisterClass(hinst))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("WndStatistic::RegisterClass"),GetLastError());

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


bool WndWator::OnCreate(CREATESTRUCT *pcs,
						LRESULT		 &lres)
{
	try
	{
		const DWORD							dwButtonStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
		const int							iButtonW	  = 80;
		const int							iButtonH	  = 20;
		tbase2::windows::gdi::DeviceContext dc(*this);

		if(!m_fntDlg.CreatePointFont(dc,8,gl_strDlgFont.c_str()))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("m_fntDlg.CreatePointFont"),GetLastError());
		
		if(!m_pbtSettings.Create(*this,dwButtonStyle,0,PBT_SETTINGS,gl_Module.String(IDS_SETTINGS).c_str(),0,0,iButtonW,iButtonH))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("m_pbtSettings.Create"),GetLastError());

		if(!m_pbtStartStop.Create(*this,dwButtonStyle,0,PBT_STARTSTOP,gl_Module.String(IDS_STOP).c_str(),0,0,iButtonW,iButtonH))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("m_pbtStartStop.Create"),GetLastError());

		if(!m_pbtSingleStep.Create(*this,dwButtonStyle,0,PBT_SINGLESTEP,gl_Module.String(IDS_SINGLESTEP).c_str(),0,0,iButtonW,iButtonH))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("m_pbtSingleStep.Create"),GetLastError());

		if(!m_pbtRestart.Create(*this,dwButtonStyle,0,PBT_RESTART,gl_Module.String(IDS_RESTART).c_str(),0,0,iButtonW,iButtonH))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("m_pbtRestart.Create"),GetLastError());

		if(!m_wndDisplay.CreateEx(WndDisplay::CLASSNAME.c_str(),L"",WS_CHILD|WS_VISIBLE,0,0,0,100,100,*this,reinterpret_cast<HMENU>(WND_DISPLAY),pcs->hInstance,NULL))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("m_wndDisplay.CreateEx"),GetLastError());

		if(!m_wndStatistic.CreateEx(WndStatistic::CLASSNAME.c_str(),L"",WS_CHILD|WS_VISIBLE,0,0,0,160,100,*this,reinterpret_cast<HMENU>(WND_STATISTIC),pcs->hInstance,NULL))
			throw tbase2::WinAPIError(__TFILE__,__LINE__,_T("m_wndStatistic.CreateEx"),GetLastError());

		m_pbtSettings.SetFont(m_fntDlg,true);
		m_pbtStartStop.SetFont(m_fntDlg,true);
		m_pbtSingleStep.SetFont(m_fntDlg,true);
		m_pbtRestart.SetFont(m_fntDlg,true);

		m_ltButtons.AddItem(-1,m_pbtSettings,false,false);
		m_ltButtons.AddItem(-1,m_pbtStartStop,false,false);
		m_ltButtons.AddItem(-1,m_pbtSingleStep,false,false);
		m_ltButtons.AddItem(-1,m_pbtRestart,false,false);
		m_ltButtons.AddItem(-1,0,0,false,true);

		m_ltSim.AddItem(-1,m_wndDisplay,true,true);
		m_ltSim.AddItem(-1,m_wndStatistic,true,false);

		m_ltDlg.AddItem(-1,&m_ltSim,true,true);
		m_ltDlg.AddItem(-1,&m_ltButtons,false,true);

		SetLayout(&m_ltDlg);

		m_wndDisplay.SetColorFish(m_crFish);
		m_wndDisplay.SetColorShark(m_crShark);
		m_wndDisplay.SetColorWater(m_crWater);

		m_wndStatistic.SetColorFish(m_crFish);
		m_wndStatistic.SetColorShark(m_crShark);

		m_flStartup = true;
		PostMessage(WM_COMMAND,PBT_RESTART,0);
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
	m_wndDisplay.DestroyWindow();
	m_wndStatistic.DestroyWindow();

	delete[] m_pcData; m_pcData=NULL;
	m_sizData = 0;
	KillTimer(TIMER_STEP);

	return true;
} // end - WndWator::OnDestroy


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
		SetTimer(TIMER_STEP,90);
		m_pbtStartStop.SetText(gl_Module.String(IDS_STOP).c_str());
		m_flRunning = true;
	}

	m_pbtSingleStep.EnableWindow(!m_flRunning);
} // end - WndWator::OnStartStop


void WndWator::OnSettings()
{
	DlgSettings dlg(m_wator);

	if(m_flRunning)
		OnStartStop();

	dlg.Modal(*this);
} // end - WndWator::OnSettings


void WndWator::OnSingleStep()
{
	const size_t sizRequired = m_wator.GetWidth() * m_wator.GetHeight();

	if(m_sizData<=sizRequired)
	{
		delete[] m_pcData; 
		m_pcData=new char[sizRequired];
		m_sizData = sizRequired;
	}

	if(m_pcData)
	{
		m_wator.Step();

		memset(m_pcData,0,m_sizData);
		m_wator.Get(m_pcData,sizRequired);
		m_wndDisplay.SetData(m_pcData,m_wator.GetWidth(),m_wator.GetHeight());
		InvalidateRect(m_wndDisplay,NULL,FALSE);

		m_wndStatistic.AddData(m_wator.GetFishCount(),m_wator.GetSharkCount(),(unsigned)sizRequired);
		InvalidateRect(m_wndStatistic,NULL,TRUE);
	}
} // end - WndWator::OnSingleStep


void WndWator::OnRestart()
{
	DlgSettings dlg(m_wator,true);

	if(m_flRunning)
		OnStartStop();

	//if(dlg.Modal(*this)==IDOK)
	{
		m_wator.Reset();
		m_wndStatistic.ClearData();
		if(!m_flRunning)
			OnStartStop();
	}
/*
	else if(m_flStartup)
		PostMessage(WM_CLOSE,0,0);	
	else
		OnStartStop();
*/
	m_flStartup = false;
} // end - WndWator::OnRestart


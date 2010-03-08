
#include "stdafx.h"
#include "wndwator.h"
#include "dlgsettings.h"

#ifdef _DEBUG
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


static const UINT_PTR TIMER_STEP = 111;

static const UINT_PTR PBT_SETTINGS   = 1000;
static const UINT_PTR WND_DISPLAY    = 1001;
static const UINT_PTR WND_STATISTIC  = 1002;
static const UINT_PTR PBT_STARTSTOP  = 1003;
static const UINT_PTR PBT_SINGLESTEP = 1004;
static const UINT_PTR PBT_RESTART	 = 1005;


const tk::string WndWator::CLASSNAME = "Wator:WndClass:WndWator";


TKW32_BEGIN_MESSAGETABLE(WndWator)
	TKW32_WM_CREATE(OnCreate)
	TKW32_WM_TIMER(OnTimer)
	TKW32_WM_CLOSE(OnClose)
	TKW32_WM_DESTROY(OnDestroy)

	TKW32_WM_COMMAND(PBT_STARTSTOP,OnStartStop)
	TKW32_WM_COMMAND(PBT_SETTINGS,OnSettings)
	TKW32_WM_COMMAND(PBT_SINGLESTEP,OnSingleStep)
	TKW32_WM_COMMAND(PBT_RESTART,OnRestart)
TKW32_END_MESSAGETABLE(tkw32::Wnd)


WndWator::WndWator(WatorSim &wator) : tkw32::Wnd(),
									  m_wator(wator),
									  m_hwndSettings(NULL),
									  m_hwndStartStop(NULL),
									  m_hwndSingleStep(NULL),
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
} // end - WndWator::WndWator


WndWator::~WndWator()
{
	delete[] m_pcData; m_pcData=NULL;
} // end - WndWator::~WndWator


bool WndWator::RegisterClass(HINSTANCE hinst)
{
	bool flReturn = false;

	if(!WndDisplay::RegisterClass(hinst))
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot register window class!");
	else if(!WndStatistic::RegisterClass(hinst))
		tklib::TraceError(__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot register window class!");
	else
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

		flReturn = Wnd::RegisterClass(wc);
	}

	return flReturn;
} // end - WndWator::RegisterClass


LRESULT WndWator::OnCreate(CREATESTRUCT *pcs)
{
	LRESULT lres = -1;
	HDC		hdc	 = GetDC(*this);

	if(!m_fntDlg.CreatePointFont(hdc,8,"MS Shell Dlg"))
		tklib::TraceError(*this,__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create font!");
	else if(!IsWindow(m_hwndSettings=CreateWindow("BUTTON","Settings",WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,0,80,20,*this,(HMENU)PBT_SETTINGS,pcs->hInstance,NULL)))
		tklib::TraceError(*this,__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create window!");
	else if(!IsWindow(m_hwndStartStop=CreateWindow("BUTTON","Stop",WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,0,80,20,*this,(HMENU)PBT_STARTSTOP,pcs->hInstance,NULL)))
		tklib::TraceError(*this,__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create window!");
	else if(!IsWindow(m_hwndSingleStep=CreateWindow("BUTTON","Einzelschritt",WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,0,80,20,*this,(HMENU)PBT_SINGLESTEP,pcs->hInstance,NULL)))
		tklib::TraceError(*this,__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create window!");
	else if(!IsWindow(m_hwndRestart=CreateWindow("BUTTON","Neustart",WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,0,80,20,*this,(HMENU)PBT_RESTART,pcs->hInstance,NULL)))
		tklib::TraceError(*this,__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create window!");
	else if(!m_wndDisplay.Create(WndDisplay::CLASSNAME,"",WS_CHILD|WS_VISIBLE,0,0,100,100,*this,(HMENU)WND_DISPLAY,pcs->hInstance,NULL))
		tklib::TraceError(*this,__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create display window!");
	else if(!m_wndStatistic.Create(WndStatistic::CLASSNAME,"",WS_CHILD|WS_VISIBLE,0,0,160,100,*this,(HMENU)WND_STATISTIC,pcs->hInstance,NULL))
		tklib::TraceError(*this,__FUNCTION__,__FILE__,__LINE__,GetLastError(),"Cannot create statistic window!");
	else
	{
		SendMessage(m_hwndSettings,WM_SETFONT,(WPARAM)(HFONT)m_fntDlg,MAKELPARAM(TRUE,FALSE));
		SendMessage(m_hwndStartStop,WM_SETFONT,(WPARAM)(HFONT)m_fntDlg,MAKELPARAM(TRUE,FALSE));
		SendMessage(m_hwndSingleStep,WM_SETFONT,(WPARAM)(HFONT)m_fntDlg,MAKELPARAM(TRUE,FALSE));
		SendMessage(m_hwndRestart,WM_SETFONT,(WPARAM)(HFONT)m_fntDlg,MAKELPARAM(TRUE,FALSE));

		m_ltButtons.AddItem(-1,m_hwndSettings,false,false);
		m_ltButtons.AddItem(-1,m_hwndStartStop,false,false);
		m_ltButtons.AddItem(-1,m_hwndSingleStep,false,false);
		m_ltButtons.AddItem(-1,m_hwndRestart,false,false);
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
		PostMessage(*this,WM_COMMAND,PBT_RESTART,0);
		lres = 0;
	}

	if(hdc)
	{
		ReleaseDC(*this,hdc);
		hdc = NULL;
	}


	lres = 0;

	return lres;
} // end - WndWator::OnCreate


void WndWator::OnTimer(UINT_PTR  uTimer,
					   TIMERPROC proc)
{
	if(uTimer==TIMER_STEP)
		OnSingleStep();
} // end - WndWator::OnTimer


void WndWator::OnClose()
{
	PostQuitMessage(0);
} // end - WndWator::OnClose


void WndWator::OnDestroy()
{
	tkw32::DESTROYWINDOW(m_hwndSettings);
	tkw32::DESTROYWINDOW(m_hwndStartStop);
	tkw32::DESTROYWINDOW(m_hwndSingleStep);
	tkw32::DESTROYWINDOW(m_hwndRestart);
	tkw32::DESTROYWINDOW(m_wndDisplay.m_hwnd);
	tkw32::DESTROYWINDOW(m_wndStatistic.m_hwnd);
	
	delete[] m_pcData; m_pcData=NULL;
	m_sizData = 0;
	KillTimer(*this,TIMER_STEP);
} // end - WndWator::OnDestroy


void WndWator::OnStartStop()
{
	if(m_flRunning)
	{
		KillTimer(*this,TIMER_STEP);
		SetDlgItemText(*this,PBT_STARTSTOP,"Start");
		m_flRunning = false;
	}
	else
	{
		SetTimer(*this,TIMER_STEP,90,NULL);
		SetDlgItemText(*this,PBT_STARTSTOP,"Stop");
		m_flRunning = true;
	}

	EnableDlgItem(PBT_SINGLESTEP,!m_flRunning);
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

	if(dlg.Modal(*this)==IDOK)
	{
		m_wator.Reset();
		m_wndStatistic.ClearData();
		if(!m_flRunning)
			OnStartStop();
	}
	else if(m_flStartup)
		PostMessage(*this,WM_CLOSE,0,0);	
	else
		OnStartStop();

	m_flStartup = false;
} // end - WndWator::OnRestart


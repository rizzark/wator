
#ifndef _WATOR_WNDWATOR_INCLUDED_
	#define _WATOR_WNDWATOR_INCLUDED_

	#include "watorsim.h"
	#include "wnddisplay.h"
	#include "wndstatistic.h"

	class WndWator : public tkw32::Wnd
	{
		TKW32_DECLARE_MESSAGETABLE()

		public:
				static const tk::string CLASSNAME;

						WndWator(WatorSim &wator);
				virtual ~WndWator();
				
				static bool RegisterClass(HINSTANCE hinst);

		protected:
					virtual LRESULT OnCreate (CREATESTRUCT *pcs);
					virtual void	OnTimer	 (UINT_PTR uTimer, TIMERPROC proc);
					virtual void	OnClose  ();
					virtual void	OnDestroy();

					virtual void OnStartStop ();
					virtual void OnSettings  ();
					virtual void OnSingleStep();
					virtual void OnRestart	 ();

		private:
				WatorSim	   &m_wator;
				tkw32::Font    m_fntDlg;
				HWND		   m_hwndSettings;
				HWND		   m_hwndStartStop;
				HWND		   m_hwndSingleStep;
				HWND		   m_hwndRestart;
				WndDisplay	   m_wndDisplay;
				WndStatistic   m_wndStatistic;
				tkw32::HLayout m_ltDlg;
				tkw32::VLayout m_ltSim;
				tkw32::VLayout m_ltButtons;
				char		   *m_pcData;
				size_t		   m_sizData;
				COLORREF	   m_crWater;
				COLORREF	   m_crFish;
				COLORREF	   m_crShark;
				bool		   m_flRunning;
				bool		   m_flStartup;
	};

#endif

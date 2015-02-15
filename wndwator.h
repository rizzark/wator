/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once

#include "watorsim.h"
#include "wnddisplay.h"
#include "wndstatistic.h"


class WndWator : public tbase2::windows::gui::Wnd
{
	public:
			static const std::wstring CLASSNAME;

					WndWator(WatorSim &wator);
			virtual ~WndWator();
				
			static bool RegisterClass(HINSTANCE hinst);

			virtual bool PreTranslateMsg(MSG &msg
			);

	protected:
				virtual bool OnCreate(CREATESTRUCT *pcs,
									  LRESULT	   &lres
				);

				virtual bool OnTimer(UINT_PTR  uID,		//!< ID of timer
									 TIMERPROC fpTimer	//!< Timerproc
				);

				virtual bool OnCommand(WORD wIDFrom,	//!< ID of sender
									   WORD wType,		//!< Type of sender (0=Menu, 1=Accelerator)
									   HWND hwndFrom	//!< Handle of sender
				);

				virtual bool OnClose(
				);

				virtual bool OnDestroy(
				);

				virtual void OnStartStop ();
				virtual void OnSettings  ();
				virtual void OnSingleStep();
				virtual void OnRestart	 ();

	private:
			WatorSim					  &m_wator;
			tbase2::windows::gdi::Font	  m_fntDlg;
			tbase2::windows::gui::Button  m_pbtSettings;
			tbase2::windows::gui::Button  m_pbtStartStop;
			tbase2::windows::gui::Button  m_pbtSingleStep;
			tbase2::windows::gui::Button  m_pbtRestart;
			WndDisplay					  m_wndDisplay;
			WndStatistic				  m_wndStatistic;
			tbase2::windows::gui::HLayout m_ltDlg;
			tbase2::windows::gui::VLayout m_ltSim;
			tbase2::windows::gui::VLayout m_ltButtons;
			char						  *m_pcData;
			size_t						  m_sizData;
			COLORREF					  m_crWater;
			COLORREF					  m_crFish;
			COLORREF					  m_crShark;
			bool						  m_flRunning;
			bool						  m_flStartup;
};


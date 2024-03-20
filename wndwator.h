/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once

#include "renderer.h"


class WndWator : public tbase2::windows::gui::Wnd
{
	public:
			static const std::wstring CLASSNAME;

					WndWator(wator::ISimulation &wator,
							 const std::locale  &locale
					);

			virtual ~WndWator();
				
			static bool RegisterClass(HINSTANCE hinst);

			virtual bool PreTranslateMsg(MSG &msg
			);


			virtual void PostRestart();

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

				virtual bool OnHScroll(WORD nScroll,		//!< Scroll-command
									   WORD nPos,		//!< Scroll position for SB_THUMBPOSITON of SB_THUMBTRACK
									   HWND hwndFrom	//!< Sending window, NULL for standard scroll bar
				);

				virtual bool OnClose(
				);

				virtual bool OnDestroy(
				);

				virtual bool OnPaint(
				);

				virtual void OnStartStop ();
				virtual void OnSettings  ();
				virtual void OnSingleStep();
				virtual void OnRestart	 ();

				virtual void UpdateDisplay(tbase2::windows::gdi::DeviceContext &dc
				);

	private:
			wator::ISimulation			  &m_wator;
			Renderer					  m_Renderer;
			const std::locale			  &m_locale;
			tbase2::windows::gdi::Font	  m_fntDlg;
			tbase2::windows::gui::Button  m_pbtSettings;
			tbase2::windows::gui::Button  m_pbtStartStop;
			tbase2::windows::gui::Button  m_pbtSingleStep;
			tbase2::windows::gui::Button  m_pbtRestart;
			tbase2::windows::gui::Slider  m_sldIntervall;
			tbase2::windows::gui::Static  m_stcIntervall;
			tbase2::windows::gui::HLayout m_ltDlg;
			tbase2::windows::gui::VLayout m_ltSim;
			tbase2::windows::gui::VLayout m_ltButtons;
			bool						  m_flRunning;
			bool						  m_flStartup;
			int							  m_iDisplayPane;
			int							  m_iStatisticPane;
			unsigned					  m_uIntervall;
};


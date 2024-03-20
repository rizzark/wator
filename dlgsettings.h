/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once

#include "renderer.h"


class RGBDefinition
{
	public:
					RGBDefinition(const std::locale &locale
					);

			virtual ~RGBDefinition();

			virtual void Subclass(HWND			 hwndParent,
								  const unsigned redID,
								  const unsigned greenID,
								  const unsigned blueID,
								  const unsigned previewID,
								  const unsigned buttonID
			);

			virtual void SetValue(const COLORREF &color
			);

			virtual inline COLORREF GetValue() const	{return m_color;}

			virtual bool OnCtlColorEdit(HDC    hdc,
										HWND   hwndEdit,
										HBRUSH &hbrResult
			);

			virtual bool OnCmdNotify(WORD wIDFrom,	//!< ID of sender
									 WORD wNotify,	//!< Notification code
									 HWND hwndFrom	//!< Handle of sender
			);


			virtual void ChooseColor(HWND hwndParent
			);

	private:
			tbase2::windows::gui::EditNumber<unsigned> m_edfRed;;
			tbase2::windows::gui::EditNumber<unsigned> m_edfGreen;
			tbase2::windows::gui::EditNumber<unsigned> m_edfBlue;
			tbase2::windows::gui::Static			   m_Preview;
			COLORREF								   m_color;
			tbase2::windows::gdi::Brush				   m_brPreview;
};




class DlgSettings : public tbase2::windows::gui::Dlg
{
	public:
			static const unsigned DEFAULT_INTERVALL;


					DlgSettings(wator::ISimulation &ws, 
								Renderer		   &renderer,
								const std::locale  &locale,
								const bool		   flRestart=false
					);

			virtual ~DlgSettings();

	protected:
				virtual void OnInitDialog(HWND hwndFocus,
										  BOOL &flReturn
				);

				virtual void OnOK();

				virtual bool OnCommand(WORD wIDFrom,	//!< ID of sender
									   WORD wType,		//!< Type of sender (0=Menu, 1=Accelerator)
									   HWND hwndFrom	//!< Handle of sender
				);

				virtual bool OnCmdNotify(WORD wIDFrom,	//!< ID of sender
										 WORD wNotify,	//!< Notification code
										 HWND hwndFrom	//!< Handle of sender
				);


				virtual bool OnCtlColorEdit(HDC    hdc,
											HWND   hwndEdit,
											HBRUSH &hbrResult
				);
				

				virtual void DataToDialog();

				virtual bool DialogToData(const bool flValidate
				);

	private:
			wator::ISimulation						   &m_ws;
			Renderer								   &m_renderer;
			const bool								   m_flRestart;
			tbase2::windows::gui::EditNumber<unsigned> m_edfWidth;
			tbase2::windows::gui::EditNumber<unsigned> m_edfHeight;
			tbase2::windows::gui::EditNumber<unsigned> m_edfFishStart;
			tbase2::windows::gui::EditNumber<unsigned> m_edfSharkStart;
			tbase2::windows::gui::EditNumber<unsigned> m_edfFishBreed;
			tbase2::windows::gui::EditNumber<unsigned> m_edfSharkBreed;
			tbase2::windows::gui::EditNumber<unsigned> m_edfSharkStarve;
			RGBDefinition							   m_rgbFish;
			RGBDefinition							   m_rgbShark;
			RGBDefinition							   m_rgbWater;
};


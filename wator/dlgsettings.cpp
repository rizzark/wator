/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once

#include "stdafx.h"
#include "dlgsettings.h"
#include "resource.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


extern tbase2::windows::Module gl_Module;


const unsigned DlgSettings::DEFAULT_INTERVALL = 90;


DlgSettings::DlgSettings(wator::ISimulation &ws,
						 Renderer		    &renderer,
						 const std::locale  &locale,
						 const bool		    flRestart) : tbase2::windows::gui::Dlg(GetModuleHandle(NULL),MAKEINTRESOURCE(DLG_WATOR)),
														 m_ws(ws),
 														 m_renderer(renderer),
														 m_flRestart(flRestart),
														 m_edfWidth(locale),
														 m_edfHeight(locale),
														 m_edfFishStart(locale),
														 m_edfSharkStart(locale),
														 m_edfFishBreed(locale),
 														 m_edfSharkBreed(locale),
														 m_edfSharkStarve(locale),
														 m_rgbFish(locale),
														 m_rgbShark(locale),
														 m_rgbWater(locale)
{
} // end - DlgSettings::DlgSettings


DlgSettings::~DlgSettings()
{
} // end - DlgSettings::~DlgSettings


void DlgSettings::OnInitDialog(HWND hwndFocus,
							   BOOL &flReturn)
{
	m_edfWidth.SubclassDlgItem(*this,EDF_WATOR_SIZEX);
	m_edfHeight.SubclassDlgItem(*this,EDF_WATOR_SIZEY);
	m_edfFishStart.SubclassDlgItem(*this,EDF_WATOR_FISHSTART);
	m_edfSharkStart.SubclassDlgItem(*this,EDF_WATOR_SHARKSTART);
	m_edfFishBreed.SubclassDlgItem(*this,EDF_WATOR_FISHBREED);
	m_edfSharkBreed.SubclassDlgItem(*this,EDF_WATOR_SHARKBREED);
	m_edfSharkStarve.SubclassDlgItem(*this,EDF_WATOR_SHARKSTARVE);
	m_rgbFish.Subclass(*this,EDF_FISH_RED,EDF_FISH_GREEN,EDF_FISH_BLUE,STC_FISH_PREVIEW,PBT_FISH_COLOR);
	m_rgbShark.Subclass(*this,EDF_SHARK_RED,EDF_SHARK_GREEN,EDF_SHARK_BLUE,STC_SHARK_PREVIEW,PBT_SHARK_COLOR);
	m_rgbWater.Subclass(*this,EDF_WATER_RED,EDF_WATER_GREEN,EDF_WATER_BLUE,STC_WATER_PREVIEW,PBT_WATER_COLOR);

	m_edfWidth.EnableWindow(m_flRestart);
	m_edfHeight.EnableWindow(m_flRestart);
	m_edfFishStart.EnableWindow(m_flRestart);
	m_edfSharkStart.EnableWindow(m_flRestart);

	DataToDialog();
	flReturn = TRUE;
} // end - DlgSettings::OnInitDialog


void DlgSettings::OnOK()
{
	if(DialogToData(true))
		EndDialog(*this,IDOK);
} // end - DlgSettings::OnOK


bool DlgSettings::OnCommand(WORD wIDFrom,
							WORD wType,
							HWND hwndFrom)
{
	bool flReturn = true;

	if(wIDFrom==PBT_WATOR_DEFAULT)
	{
		m_ws.InitDefault();
		m_renderer.SetDefault();
		DataToDialog();
	}
	else if(wIDFrom==PBT_WATOR_RESTORE)
		DataToDialog();
	else if(wIDFrom==PBT_FISH_COLOR)
		m_rgbFish.ChooseColor(*this);
	else if(wIDFrom==PBT_SHARK_COLOR)
		m_rgbShark.ChooseColor(*this);
	else if(wIDFrom==PBT_WATER_COLOR)
		m_rgbWater.ChooseColor(*this);
	else
		flReturn = true;

	return flReturn;
} // end - DlgSettings::OnCommand


bool DlgSettings::OnCmdNotify(WORD wIDFrom,
							  WORD wType,
							  HWND hwndFrom)
{
	bool flReturn = true;

	if(m_rgbFish.OnCmdNotify(wIDFrom,wType,hwndFrom))
		;
	else if(m_rgbShark.OnCmdNotify(wIDFrom,wType,hwndFrom))
		;
	else if(m_rgbWater.OnCmdNotify(wIDFrom,wType,hwndFrom))
		;
	else
		flReturn = false;

	return flReturn;
} // end - DlgSettings::OnCmdNotify


bool DlgSettings::OnCtlColorEdit(HDC	hdc,
								 HWND	hwndEdit,
								 HBRUSH &hbrResult)
{
	bool flReturn = true;

	if(m_rgbFish.OnCtlColorEdit(hdc,hwndEdit,hbrResult))
		;
	else if(m_rgbShark.OnCtlColorEdit(hdc,hwndEdit,hbrResult))
		;
	else if(m_rgbWater.OnCtlColorEdit(hdc,hwndEdit,hbrResult))
		;
	else
		flReturn = false;

	return flReturn;
} // end - DlgSettings::OnCtlColorEdit


void DlgSettings::DataToDialog()
{
	auto pars = m_ws.Parameter;
	m_edfWidth.SetValue(pars.Width);
	m_edfHeight.SetValue(pars.Height);
	m_edfFishStart.SetValue(pars.InitialFishCount);
	m_edfSharkStart.SetValue(pars.InitialSharkCount);
	m_edfFishBreed.SetValue(pars.FishBreed);
	m_edfSharkBreed.SetValue(pars.SharkBreed);
	m_edfSharkStarve.SetValue(pars.SharkStarve);

	m_rgbFish.SetValue(m_renderer.GetColorFish());
	m_rgbShark.SetValue(m_renderer.GetColorShark());
	m_rgbWater.SetValue(m_renderer.GetColorWater());
} // end - DlgSettings::DataToDialog


bool DlgSettings::DialogToData(const bool flValidate)
{
	bool flReturn = false;

	try
	{
		wator::SIMULATION_PARAMETERS parameters;


		parameters.Width			 = m_edfWidth.GetValue();
		parameters.Height			 = m_edfHeight.GetValue();
		parameters.InitialFishCount  = m_edfFishStart.GetValue();
		parameters.InitialSharkCount = m_edfSharkStart.GetValue();
		parameters.FishBreed		 = m_edfFishBreed.GetValue();
		parameters.SharkBreed		 = m_edfSharkBreed.GetValue();
		parameters.SharkStarve		 = m_edfSharkStarve.GetValue();
		

		m_renderer.SetColorFish(m_rgbFish.GetValue());
		m_renderer.SetColorShark(m_rgbShark.GetValue());
		m_renderer.SetColorWater(m_rgbWater.GetValue());

		if(flValidate)
		{
			if(parameters.Width==0)
				throw tbase2::validation::ValidationFailed(m_edfWidth.GetID(),tbase2::StringtableMsg(gl_Module,MSG_WIDTH_REQUIRED));
			if(parameters.Height==0)
				throw tbase2::validation::ValidationFailed(m_edfHeight.GetID(),tbase2::StringtableMsg(gl_Module,MSG_HEIGHT_REQUIRED));

			const unsigned uMaxFields = parameters.Width * parameters.Height;
			if((parameters.InitialFishCount+parameters.InitialSharkCount)>uMaxFields)
				throw tbase2::validation::ValidationFailed(m_edfFishStart.GetID(),tbase2::StringtableMsg(gl_Module,MSG_TOO_MANY_ANIMALS,1,std::to_wstring(uMaxFields).c_str()));

			if (parameters.Width != m_ws.Parameter.Width
				|| parameters.Height != m_ws.Parameter.Height
				|| parameters.InitialFishCount != m_ws.Parameter.InitialFishCount
				|| parameters.InitialSharkCount != m_ws.Parameter.InitialSharkCount)
			{
				if (!m_flRestart)
					throw tbase2::validation::ValidationFailed(m_edfWidth.GetID(), tbase2::StringtableMsg(gl_Module, MSG_RESTART_REQUIRED));
				m_ws.Init(parameters);
			}
			else
				m_ws.SetConfig(parameters.FishBreed, parameters.SharkBreed, parameters.SharkStarve);
		}

		flReturn = true;
	}
	catch(tbase2::validation::ValidationFailed &error)
	{
		HWND			   hwndDlgItem = GetDlgItem(error.m_uID);
		const std::wstring wstrMsg	   = error.m_pmsg ? error.m_pmsg->Text(GetThreadLocale()) : gl_Module.String(IDS_INVALID_INPUT);

		MessageBox(*this,wstrMsg.c_str(),L"",MB_OK|MB_ICONEXCLAMATION);
		::SetFocus(hwndDlgItem);
	}
	catch(std::exception &error)
	{
		tbase2::debug::Rethrow(__TFILE__,__LINE__,__TFUNCTION__,error);
	}
	catch(...)
	{
		throw tbase2::UnexpectedError(__TFILE__,__LINE__,__TFUNCTION__);
	}

	return flReturn;
} // end - DlgSettings::DataToDialog




RGBDefinition::RGBDefinition(const std::locale &locale) : m_edfRed(locale),
														  m_edfGreen(locale),
														  m_edfBlue(locale),
														  m_color(RGB(0,0,0))
{
} // end - RGBDefinition::RGBDefinition


RGBDefinition::~RGBDefinition()
{
} // end - RGBDefinition


void RGBDefinition::Subclass(HWND			hwndParent,
							 const unsigned redID,
							 const unsigned greenID,
							 const unsigned blueID,
							 const unsigned previewID,
							 const unsigned buttonID)
{
	try
	{
		m_edfRed.SubclassDlgItem(hwndParent,redID);
		m_edfGreen.SubclassDlgItem(hwndParent,greenID);
		m_edfBlue.SubclassDlgItem(hwndParent,blueID);
		m_Preview.SubclassDlgItem(hwndParent,previewID);
	}
	catch(std::exception &error)
	{
		tbase2::debug::Rethrow(__TFILE__,__LINE__,__TFUNCTION__,error);
	}
	catch(...)
	{
		throw tbase2::UnexpectedError(__TFILE__,__LINE__,__TFUNCTION__);
	}
} // end - RGBDefinition::Subclass


void RGBDefinition::SetValue(const COLORREF &color)
{
	m_color = color;
	m_edfRed.SetValue(GetRValue(m_color));
	m_edfGreen.SetValue(GetGValue(m_color));
	m_edfBlue.SetValue(GetBValue(m_color));

	m_brPreview.CreateSolidBrush(m_color);
	m_Preview.Invalidate();
} // end - RGBDefinition::SetValue


bool RGBDefinition::OnCtlColorEdit(HDC    hdc,
								   HWND   hwndEdit,
								   HBRUSH &hbrResult)
{
	bool flReturn = false;

	if(hwndEdit==m_Preview)
	{
		//SetTextColor(hdc,m_color);
		SetBkColor(hdc,m_color);
		hbrResult = m_brPreview;
		flReturn = true;
	}

	return flReturn;
} // end - RGBDefinition::OnCtlColorEdit


bool RGBDefinition::OnCmdNotify(WORD wIDFrom,
								WORD wType,
								HWND hwndFrom)
{
	bool flReturn = false;

	if((hwndFrom==m_edfRed || hwndFrom==m_edfGreen || hwndFrom==m_edfBlue) && wType==EN_KILLFOCUS)
	{
		const unsigned red   = m_edfRed.GetValue();
		const unsigned green = m_edfGreen.GetValue();
		const unsigned blue  = m_edfBlue.GetValue();

		SetValue(RGB(red,green,blue));
		flReturn = true;
	}

	return flReturn;
} // end - RGBDefinition::OnCmdNotify


void RGBDefinition::ChooseColor(HWND hwndParent)
{
	CHOOSECOLOR cc;
	COLORREF    pcrCustom[16];

	memset(&cc,0,sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner   = hwndParent;
	cc.rgbResult   = m_color;
	cc.lpCustColors = pcrCustom;
	cc.Flags	   = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT | CC_SOLIDCOLOR;

	if(!::ChooseColor(&cc))
	{
		DWORD dwError = CommDlgExtendedError();
		if(dwError)
			throw tbase2::windows::gui::CommDlgError(__TFILE__,__LINE__,L"::ChooseColor",dwError);
	}

	SetValue(cc.rgbResult);
} // end - RGBDefinition::ChooseColor

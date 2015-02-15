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


DlgSettings::DlgSettings(WatorSim   &ws,
						 const bool flRestart) : tbase2::windows::gui::Dlg(GetModuleHandle(NULL),MAKEINTRESOURCE(DLG_WATOR)),
												 m_ws(ws),
												 m_flRestart(flRestart)
{
} // end - DlgSettings::DlgSettings


DlgSettings::~DlgSettings()
{
} // end - DlgSettings::~DlgSettings


void DlgSettings::OnInitDialog()
{
	EnableDlgItem(EDF_WATOR_SIZEX,m_flRestart);
	EnableDlgItem(EDF_WATOR_SIZEY,m_flRestart);
	EnableDlgItem(EDF_WATOR_FISHSTART,m_flRestart);
	EnableDlgItem(EDF_WATOR_SHARKSTART,m_flRestart);

	ShowWindow(SW_SHOW);
	SetActiveWindow();
//	tbase2::windows::gui::Dlg::OnInitDialog();
} // end - DlgSettings::OnInitDialog

/*
void DlgSettings::Update(void *pData)
{
	unsigned uWidth		  = m_ws.GetWidth();
	unsigned uHeight	  = m_ws.GetHeight();
	unsigned uFishStart	  = m_ws.GetFishStart();
	unsigned uSharkStart  = m_ws.GetSharkStart();
	unsigned uFishBreed	  = m_ws.GetFishBreed();
	unsigned uSharkBreed  = m_ws.GetSharkBreed();
	unsigned uSharkStarve = m_ws.GetSharkStarve();
	unsigned uMaxFields	  = 0;

	UpdateUnsigned(pData,EDF_WATOR_SIZEX,uWidth);
	if(IsUpdateValidate(pData) && uWidth==0)
		UpdateFailed(pData,"Sie müssen eine Breite angeben!");
	UpdateUnsigned(pData,EDF_WATOR_SIZEY,uHeight);
	if(IsUpdateValidate(pData) && uHeight==0)
		UpdateFailed(pData,"Sie müssen eine Höhe angeben!");

	if(IsUpdateValidate(pData))
		uMaxFields = uWidth * uHeight;

	UpdateUnsigned(pData,EDF_WATOR_FISHSTART,uFishStart);
	UpdateUnsigned(pData,EDF_WATOR_SHARKSTART,uSharkStart);
	if(IsUpdateValidate(pData) && (uFishStart+uSharkStart)>uMaxFields)
		UpdateFailed(pData,"Sie haben zu viele Tiere angegeben!");

	UpdateUnsigned(pData,EDF_WATOR_FISHBREED,uFishBreed);
	UpdateUnsigned(pData,EDF_WATOR_SHARKBREED,uSharkBreed);
	UpdateUnsigned(pData,EDF_WATOR_SHARKSTARVE,uSharkStarve);

	if(IsUpdateFromDlg(p
	Data))
	{
		if(	   uWidth!=m_ws.GetWidth() 
			|| uHeight!=m_ws.GetHeight() 
			|| uFishStart!=m_ws.GetFishStart()
			|| uSharkStart!=m_ws.GetSharkStart())
		{
			TKASSERT(m_flRestart,"Dies ist NUR mit Restart Option möglich!");

			m_ws.Init(uWidth,uHeight,uFishBreed,uSharkBreed,uSharkStarve,uFishStart,uSharkStart);
		}
		else
			m_ws.SetConfig(uFishBreed,uSharkBreed,uSharkStarve);
	}
} // end - DlgSettings::Update
*/

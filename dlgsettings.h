/********************************************************************************************************
$Date$
$Revision$
$Author$
$HeadURL$
********************************************************************************************************/

#pragma once

#include "watorsim.h"

class DlgSettings : public tbase2::windows::gui::Dlg
{
	public:
					DlgSettings(WatorSim   &ws, 
								const bool flRestart=false
					);

			virtual ~DlgSettings();

	protected:
				virtual void OnInitDialog();

	private:
			WatorSim   &m_ws;
			const bool m_flRestart;
};



#ifndef _WATOR_DLGSETTINGS_INCLUDED_
	#define _WATOR_DLGSETTINGS_INCLUDED_

	#include "watorsim.h"

	class DlgSettings : public tkw32::Dlg
	{
		TKW32_DECLARE_MESSAGETABLE()

		public:
						DlgSettings(WatorSim &ws, const bool flRestart=false);
				virtual ~DlgSettings();

		protected:
					virtual void OnInitDialog();
					virtual void Update		 (void *pData);

		private:
				WatorSim   &m_ws;
				const bool m_flRestart;
	};

#endif

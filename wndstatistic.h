
#ifndef _WATOR_WNDSTATISTIC_INCLUDED_
	#define _WATOR_WNDSTATISTIC_INCLUDED_

	class WndStatistic : public tkw32::Wnd
	{
		TKW32_DECLARE_MESSAGETABLE()

		public:
				static const tk::string CLASSNAME;
				static const unsigned	SAVECYCLES;

						WndStatistic();
				virtual ~WndStatistic();

				static bool RegisterClass(HINSTANCE hinst);

				virtual void AddData  (const unsigned uFishCount, const unsigned uSharkCount, const unsigned uMax);
				virtual void ClearData();

				virtual inline void SetColorFish (const COLORREF color) {m_crFish=color;}
				virtual inline void SetColorShark(const COLORREF color) {m_crShark=color;}

		protected:
					virtual LRESULT OnCreate(CREATESTRUCT *pcs);
					virtual void OnPaint  (HDC hdc, PAINTSTRUCT &ps);
					virtual void OnDestroy();

		private:	
				tk::array<unsigned> m_arrSharkCount;
				tk::array<unsigned> m_arrFishCount;
				unsigned			m_uCycles;
				COLORREF			m_crFish;
				COLORREF			m_crShark;
				unsigned			m_uMax;
	};
	

#endif

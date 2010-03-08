
#ifndef _WATOR_WNDDISPLAY_INCLUDED_
	#define _WATOR_WNDDISPLAY_INCLUDED_

	class WndDisplay : public tkw32::Wnd
	{
		TKW32_DECLARE_MESSAGETABLE()

		public:
				static const tk::string CLASSNAME;

						WndDisplay();
				virtual ~WndDisplay();

				static bool RegisterClass(HINSTANCE hinst);

				virtual bool SetData(const char *pcData, const unsigned uWidth, const unsigned uHeight);

				virtual inline void SetColorWater(const COLORREF color) {m_crWater=color;}
				virtual inline void SetColorFish (const COLORREF color) {m_crFish=color;}
				virtual inline void SetColorShark(const COLORREF color) {m_crShark=color;}

		protected:
					virtual void OnPaint  (HDC hdc, PAINTSTRUCT &ps);
					virtual void OnDestroy();

		private:	
				char	 *m_pcData;
				size_t	 m_sizData;
				unsigned m_uDataWidth;
				unsigned m_uDataHeight;
				COLORREF m_crWater;
				COLORREF m_crFish;
				COLORREF m_crShark;
	};

#endif

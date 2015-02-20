
#ifndef _WATOR_WATORSIM_INCLUDED_
	#define _WATOR_WATORSIM_INCLUDED_

	class WatorSim
	{
		public:
				static const char CHAR_FISH;
				static const char CHAR_SHARK;
				static const char CHAR_WATER;

				static const unsigned DEFAULT_WIDTH;
				static const unsigned DEFAULT_HEIGHT;
				static const unsigned DEFAULT_FISHBREED;
				static const unsigned DEFAULT_SHARKBREED;
				static const unsigned DEFAULT_SHARKSTARVE;
				static const unsigned DEFAULT_FISHCOUNT;
				static const unsigned DEFAULT_SHARKCOUNT;

						WatorSim();

						WatorSim(const unsigned width, 
								 const unsigned height, 
								 const unsigned uFishBreed, 
								 const unsigned uSharkBreed, 
								 const unsigned uSharkStarve, 
								 const unsigned uFishCount, 
								 const unsigned uSharkCount,
								 std::ostream   *posCSVLog = NULL
						);

				virtual ~WatorSim();

				virtual void Init(const unsigned width, 
								  const unsigned height, 
								  const unsigned uFishBreed, 
								  const unsigned uSharkBreed, 
								  const unsigned uSharkStarve, 
								  const unsigned uFishCount, 
								  const unsigned uSharkCount,
								  std::ostream   *posCSVLog = NULL
				);

				virtual void InitDefault(std::ostream *posCSVLog = NULL
				);

				virtual void SetConfig(const unsigned uFishBreed, 
									   const unsigned uSharkBreed, 
									   const unsigned uSharkStarve
				);

				virtual void Reset		();

				virtual void Get(char *pcData, const size_t sizData);

				virtual inline unsigned GetFishCount () const {return m_uFishCount;}
				virtual inline unsigned GetSharkCount() const {return m_uSharkCount;}
				virtual inline unsigned GetIterations() const {return m_uLoops;}

				virtual inline unsigned GetWidth () const {return m_uWidth;}
				virtual inline unsigned GetHeight() const {return m_uHeight;}

				virtual inline unsigned GetSharkStart () const {return m_uSharkStart;}
				virtual inline unsigned GetFishStart  () const {return m_uFishStart;}
				virtual inline unsigned GetFishBreed  () const {return m_uFishBreed;}
				virtual inline unsigned GetSharkBreed () const {return m_uSharkBreed;}
				virtual inline unsigned GetSharkStarve() const {return m_uSharkStarve;}

				virtual void Step();

				virtual std::ostream* SetLog(std::ostream *posCSVLog
				);

				virtual inline const std::vector<std::pair<unsigned,unsigned> >& GetHistory() const
				{
					return m_vHistory;
				}

		protected:
					virtual void ClearFields();

					virtual void MoveFish(const unsigned uCurPos);
					virtual void MoveShark(const unsigned uCurPos);

					virtual void GetNeighbours(const unsigned uOfs, unsigned *puOfs);

					virtual void FishAdd   (const unsigned uPos);
					virtual void FishMove  (const unsigned uFrom, const unsigned uTo);
					virtual void FishAddAge(const unsigned uPos, const unsigned uDelta=1);
					virtual void FishKill  (const unsigned uPos);

					virtual void SharkAdd	(const unsigned uPos);
					virtual void SharkMove	(const unsigned uFrom, const unsigned uTo);
					virtual void SharkAddAge(const unsigned uPos, const unsigned uDelta=1);
					virtual void SharkKill	(const unsigned uPos);

		private:
				unsigned	   m_uWidth;
				unsigned	   m_uHeight;
				size_t		   m_sizField;
				unsigned	   m_uFishBreed;
				unsigned	   m_uSharkBreed;
				unsigned	   m_uSharkStarve;

				unsigned	   m_uFishStart;
				unsigned	   m_uSharkStart;

				byte		   *m_pbFish;
				byte		   *m_pbFishMove;
				byte		   *m_pbFishBreed;
				byte		   *m_pbShark;
				byte		   *m_pbSharkMove;
				byte		   *m_pbSharkBreed;
				byte		   *m_pbSharkStarve;
				unsigned	   m_uFishCount;
				unsigned	   m_uSharkCount;
				unsigned	   m_uLoops;

				std::ostream							   *m_posCSVLog;
				std::vector<std::pair<unsigned,unsigned> > m_vHistory;
				const size_t							   m_sizHistory;

				void init();
	};

#endif

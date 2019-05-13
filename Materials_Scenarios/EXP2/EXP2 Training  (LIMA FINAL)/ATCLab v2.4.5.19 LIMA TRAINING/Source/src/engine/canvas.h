/**


 * @file:       atc_canvas.h
 *
 *              ATC_Engine.
 *              Model component.
 *
 * @author:     seth  
 * @email:      atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:34:37 PM 01/12/2003
 * $Id: canvas.h,v 1.18.2.7 2015/08/19 11:33:56 Aaron Yeung Exp $
 * $Id: canvas.h,v 1.18.2.7 2015/08/19 11:33:56 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_CANVAS_HDR__
#define __ATC_CANVAS_HDR__


#define AIRCRAFT_MAP \
		std::map< std::string, AircraftSprite * >
#define AIRCRAFT_MAP_PAIR \
		std::pair< const std::string, AircraftSprite * >
#define DBL_PAIR \
		std::pair< double, double >
#define LOCATION_VECTOR \
		std::vector< Location >

#include <qcanvas.h>
#include <windows.h>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>
#include <set>
#include <list>
#include "atc.h"
#include "atc_types.h"
#include "map_impl.h"
#include "ReminderBox.h"
#include "canvas_tools.h"
#include "canvasview.h"
#include "n_back_task.h"
#include "interrupt_task.h"
#include "lab_engine.h"
#include "trialtask.h"

#if defined(_MSC_VER)
#  pragma comment(lib, "Winmm.lib")
#endif

namespace atc
{
	class AircraftAgent;
	class AircraftSprite;
	class BearingRangeLine;
	class RequirementTool;
	class ScaleXHair;
	class SectorSprite;
	class Transformation;
	class VectorTool;
	class WayPointSprite;

	struct AircraftParam;
	struct AircraftTypeParam;

	struct canvas_error : public std::runtime_error
	{
		canvas_error(const std::string& msg)
			:std::runtime_error(msg)
		{
		}
	};

	struct SoundFile
	{
		std::string m_fileName;
		std::set<int> m_startTimes;
		std::set<int> m_endTimes;
	};

	class Canvas : public QCanvas 
	{
		Q_OBJECT

	private:
		static const int CHUNK_COUNT;

	public:
		Canvas( QObject * = 0, const char * = 0 );
		~Canvas();

		void resize( const QRect& );
		void show_clock();
        void ShowClock(bool a_show = true);

		void AddReminderBox(const ReminderBoxParams&	a_params);

		void AddDivisionLine (const std::string& a_id,
								const int&		a_x1,
								const int&		a_y1,
								const int&		a_x2,
								const int&		a_y2,
								const QColor&	a_color = QColor(DEFAULT_DIVISION_LINE_COLOR),
								const uint&		a_width = DEFAULT_DIVISION_LINE_WIDTH);

		bool SetDivisionLineDisplayTime(const std::string& a_id,
										const int& a_startTime,
										const int& a_endTime);
		void add_brl ( int, int );
		void add_brl ( AircraftSprite * );
		void add_brl ( WayPointSprite * );
		void hook_brl( int, int );
		void hook_brl( AircraftSprite * );
		void hook_brl( WayPointSprite * );
		void select_brl( BearingRangeLine * );
		void remove_brl( BearingRangeLine * );

//		void show_scale( bool );
		void add_scale(	double, double, double, double, double, double );
		void move_scale( QPoint );
		VectorTool * vector_tool();
		void add_vector( AircraftSprite * );
		void move_vector( QPoint );
		void cancel_vector();
		void pause()  { emit pause_request();  }
		void resume() { emit resume_request(); }
		void add_sector       ( const AreaDefinition & );
		void add_active_sector( const AreaDefinition & );
		void add_weather      ( const AreaDefinition& );
		void add_waypoint     ( const std::string&, double, double );
		void add_route        ( const LOCATION_VECTOR & );
		void add_aircraft( AircraftAgent * );
		double   to_internal_length( double ) const;
		DBL_PAIR to_internal_point ( const double, const double ) const;
		QPoint   to_internal_qpoint( const double, const double ) const;
		double   to_user_length( double ) const;
		DBL_PAIR to_user_point ( const double, const double ) const;
		QPoint   to_user_qpoint( const double, const double ) const;
		
        void SetCanvasView(CanvasView* a_canvasView);

		void SetReminderBoxFlashColor(	const std::string&	a_flashColor);

		void SetReminderBoxFlashTime(	unsigned int		a_flashStartTime,
										unsigned int		a_flashEndTime);

		void SetReminderBoxDisplayTime(	int					a_displayStartTime,
										int					a_displayEndTime);

		void AddSoundFile(	const std::string&			a_fileName, 
							const int&					a_startTime = INVALID_VALUE, 
							const int&					a_endTime = INVALID_VALUE);

		bool SetSoundFilePlayTime(	const std::string&	a_fileName, 
									const int&			a_startTime, 
									const int&			a_endTime = INVALID_VALUE);

        void SetRunMode(RUN_MODE a_runMode);

		void DeselectAllAircraft();
		AircraftSprite* SelectedAircraft();
		void DisableReminderBox();
		long CurrentTime() const { return m_currentTime; }
		ReminderBox* GetReminderBox();
        void ShowInterruptScreen();
        void SetInterruptParams(pact::InterruptionParams a_params);
        void SetnBackTaskParams(pact::nBackTaskParams a_params);
        void SetNotifyMissedAcceptance(bool a_notify);
        void SetNotifyMissedHandoff(bool a_notify);
        void SetDisableCalloutRotation(bool a_disable = false);
        void ShowNBackTask();
        void HideNBackTask();
        void HideInterruptScreen();
        void CreateInfoBox(pact::TrialInfoBoxParams a_params);

	public slots:
		void update_aircraft();
		void HandleAircraftFailedAcceptanceTask(const char*);
		void HandleAircraftFailedHandoffTask(const char*);
		//void HandleAircrafConflict(std::pair<std::string, std::string>);
		void HandleTrialTimeout(long);
		void set_clock( long );
		void stop_clock();
		void start_clock();
		void set_tool( QString );
		void HandleScoreUpdate(float, float, float);

	private:
		void build_sector( SectorSprite *, const QPointArray &);
		QPointArray& area_points( const AreaDefinition &, QPointArray & );
		double bearing( QPoint, QPoint ) const;  

		void conflict_detection();
		
		void register_warning( const char *msg ) { emit warning(msg); }
		void show_score();
		bool PlayMediaFile(LPCTSTR szFile);
		bool StopPlayingMediaFile(LPCTSTR szFile);

	signals:
		void pause_request();
		void resume_request();

		void event_recorded( KeyValues & );
		void TrialTimeout();
		void FailedAcceptanceTask(const char*);
		void FailedHandoffTask(const char*);
		void Conflicted(std::pair<std::string, std::string>);

	private: // members

		AIRCRAFT_MAP _aircraft;

		Transformation *_2internal;

		std::vector< WayPointSprite * >        _waypoints;
		std::list  < BearingRangeLine * >      _brls;

		ScaleXHair       *_scale;
		BearingRangeLine *_brl;
		VectorTool       *_vector;

		QCanvasText* _clock;
		QCanvasText* _tool_id;
		QCanvasText* _tool_name;
        std::auto_ptr<QCanvasText> m_info_box;
		QCanvasText* _total_score;
        CanvasView* m_canvasView;
        std::auto_ptr<nBackTask>                            m_nBackTask;
        pact::nBackTaskParams                               m_nBackTaskParams;
        std::auto_ptr<InterruptTask>                        m_interruptTask;
        pact::InterruptionParams                            m_interruptParams;
		std::auto_ptr<ReminderBox>							m_reminderBox;
		std::vector<std::pair<std::string, std::string>*>	_conflict_aircraft;
		std::map<std::string, DivisionLine*>				m_divisionLines;
		std::vector<SoundFile*>								m_soundFiles;
		long        m_currentTime;
        RUN_MODE    m_runMode;
        bool        m_notifyMissedAcceptance;
        bool        m_notifyMissedHandoff;
        bool        m_disableCalloutRotation;
        bool        m_showClock;
	}; 

	//
	// atc::AircraftMap
	//
	typedef AIRCRAFT_MAP      ACSpriteMap;
	typedef AIRCRAFT_MAP_PAIR ACSpriteMapPair;

	//
	// 
	//
	typedef LOCATION_VECTOR RouteList;

	/*!
	 *
	 */
	typedef std::vector<WayPointSprite*> WayPointVector;
	typedef WayPointVector::iterator WayPointVectorIt;
	typedef WayPointVector::const_iterator WayPointVectorCIt;


	/*!
	 *
	 */
	typedef std::list< BearingRangeLine * > BRLList;
	typedef BRLList::const_iterator BRLListCIt;

}; 


#undef AIRCRAFT_MAP
#undef AIRCRAFT_MAP_PAIR
#undef DBL_PAIR
#undef POINT_VECTOR

#endif 

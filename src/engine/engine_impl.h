/**
 * @file:       atc_engine_imp.h
 * @file:       atc_engine_imp.h
 *
 *              ATC_Engine project definitions - c++ header .
 *              An implementation of atc::Engine Interface.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $ 
 *              File created 12:24:10 PM 03/04/04
 * $Id: engine_impl.h,v 1.12.2.7 2015/08/19 11:33:57 Aaron Yeung Exp $
 * $Id: engine_impl.h,v 1.12.2.7 2015/08/19 11:33:57 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_ENGINE_IMPL_HDR__ 
#define __ATC_ENGINE_IMPL_HDR__


#ifdef WIN32
	#pragma warning( disable:4786 )
#endif


#include "engine.h"

#include "param_impl.h"
#include "map_impl.h"
#include "sky_impl.h"

#include <qwmatrix.h>
#include <memory>
#include <stdexcept>

//
// Qt classes
//

class QTime;


namespace atc{

	//
	// atc classes
	//
	class Airspace;
	class Canvas;
	class Clock;
	class LabEngine;


	/*!
	 */
	struct AircraftData;
	class  AircraftPath;

	class Transformation;


	/*!
	 * \struct engine_error
	 */
	struct engine_error : public std::runtime_error{
		engine_error(const std::string& msg) : std::runtime_error(msg){}
	};


	/*!
	 * \class EngineImpl
	 * \brief Implementation of the atc::Engine interface
	 */
	class EngineImpl : public Engine
	{
		Q_OBJECT

	public:

		EngineImpl();
		~EngineImpl();

	public: // [new]
		void build();
		void TrialTime(unsigned int trial_time);

	public: // engine interface

		Param& param();
		Map& map();
		Sky& sky();
		
		QWidget* create_view(QWidget*, const char*)const;

	public: // new engine api
		void add_aircraft( const AircraftData &, const AircraftPath & );
        void ShowClock(bool a_show = true);

	public slots: // engine interface

		void init();
		
		void start();;
		void stop();

		void reset() {};
		void clear() {};
		void HandleSecondsUpdate(long);

	private:

		//params
		void process_params();

		//map
		void process_map();
		void process_sectors( const SectorMap & );
		void process_sector( const SectorDefinition );
		void process_routes( const RouteMap & );
		void process_locations( const LocationMap & );

		//sky
		void process_sky();
		void process_aircraft(const AircraftParam&);

		// general
//		QPointArray& area_points( const AreaDefinition &, QPointArray & ); 

		// scale
		void process_scale();

        void SetCanvasView(CanvasView* a_canvasView);

		void ProcessReminderBox(const ReminderBoxParams&	a_params);

		void SetReminderBoxFlashColor(	const std::string&	a_flashColor);

		void SetReminderBoxFlashTime(	unsigned int		a_flashStartTime, 
										unsigned int		a_flashEndTime);

		void SetReminderBoxDisplayTime(	int					a_displayStartTime,
										int					a_displayEndTime);

		void ProcessSoundFile(	const std::string&		a_fileName,
								const int&				a_startTime = INVALID_VALUE,
								const int&				a_endTime = INVALID_VALUE);

		void SetSoundFilePlayTime(	const std::string&	a_fileName,
									const int&			a_startTime,
									const int&			a_endTime = INVALID_VALUE);

		void ProcessDivisionLine(			const std::string&	a_id,
											const int&			a_x1,
											const int&			a_y1,
											const int&			a_x2,
											const int&			a_y2,
											const QColor&		a_color = QColor(atc::DEFAULT_DIVISION_LINE_COLOR),
											const uint&			a_width = atc::DEFAULT_DIVISION_LINE_WIDTH);
		
        void SetDivisionLineDisplayTime (	const std::string& a_id,
											const int& a_displayStartTime,
											const int& a_displayEndTime);


        void SetnBackTaskParams(pact::nBackTaskParams a_params);
        void SetInterruptParams(pact::InterruptionParams a_params);
        void SetNotifyMissedAcceptance(bool a_notify);
        void SetNotifyMissedHandoff(bool a_notify);
        void SetDisableCalloutRotation(bool a_disable = false);
        void SetInfoBox(pact::TrialInfoBoxParams a_params);

	private: // members [new]

		std::auto_ptr< LabEngine >	_atclab;
		std::auto_ptr< Airspace >	_airspace;
		std::auto_ptr< Canvas >		_canvas;
		std::auto_ptr< Clock >		_clock;

	private: // members [old]

		ParamImpl      *_params;
		MapImpl        *_map;
		SkyImpl        *_sky;
	};

}

#endif

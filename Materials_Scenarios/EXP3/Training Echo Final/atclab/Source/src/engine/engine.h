/**
 * @file:       atc_engine.h
 *
 *              ATC_Engine project definitions - c++ header .
 *              Class Interface for atc::Engine library class .
 *
 * @author:     seth  
 *
 * @version:    $Name:  $ 
 *              File created 12:24:10 PM 03/04/04
 *
 * $Id: engine.h,v 1.15.2.6 2015/08/19 11:33:56 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_ENGINE_HDR__ 
#define __ATC_ENGINE_HDR__

// define dll interface
#ifdef WIN32
	#include <windows.h>
//	#ifdef ATC_EXPORTS
//		#define ATC_API __declspec(dllexport)
//	#else
//		#define ATC_API __declspec(dllimport)
//	#endif
//#else
	#define ATC_API
#endif


#include <qobject.h>
#include <vector>
// include interfaces for setting up engine
#include "param.h"
#include "map.h"
#include "sky.h"
#include "canvas_tools.h"
#include "ReminderBox.h"
#include "CanvasView.h"
#include "trialtask.h"

/*!
 * \class QWidget
 */
class QWidget;

namespace atc{

	/*!
	 * class atc::Engine atc_engine.h
	 *
	 * @todo document this interface
	 */
	class ATC_API Engine : public QObject
	{
		Q_OBJECT

	public:
	
		virtual ~Engine(){};

		virtual atc::Param& param() = 0;
		virtual atc::Map& map() = 0;
		virtual atc::Sky& sky() = 0;

		virtual QWidget* create_view(QWidget*, const char*)const = 0;
		virtual void SetTimeEvent(unsigned int timeout)
		{
			_time_events[timeout] = false;
		}

		virtual void TrialTime(unsigned int trial_time)
		{
			_trial_time = trial_time;
		}

        virtual void SetCanvasView(CanvasView* a_canvasView) = 0;

		virtual void ProcessReminderBox(const ReminderBoxParams&	a_params) = 0;

		virtual void SetReminderBoxFlashColor(	const std::string&	a_flashColor) = 0;
		
		virtual void SetReminderBoxFlashTime(	unsigned int		a_flashStartTime, 
												unsigned int		a_flashEndTime) = 0;

		virtual void SetReminderBoxDisplayTime(	int a_displayStartTime,
												int a_displayEndTime) = 0;

		virtual void ProcessSoundFile(	const std::string&		a_fileName,
										const int&				a_startTime = INVALID_VALUE,
										const int&				a_endTime = INVALID_VALUE) = 0;
		
		virtual void SetSoundFilePlayTime(	const std::string&	a_fileName,
											const int&			a_startTime,
											const int&			a_endTime = INVALID_VALUE) = 0;

		virtual void ProcessDivisionLine(			const std::string&	a_id,
													const int&			a_x1,
													const int&			a_y1,
													const int&			a_x2,
													const int&			a_y2,
													const QColor&		a_color = QColor(atc::DEFAULT_DIVISION_LINE_COLOR),
													const uint&			a_width = atc::DEFAULT_DIVISION_LINE_WIDTH) = 0;
		virtual void SetDivisionLineDisplayTime (	const std::string&	a_id,
													const int&			a_displayStartTime,
													const int&			a_displayEndTime) = 0;

        virtual void SetInterruptParams(pact::InterruptionParams a_params) = 0;

        virtual void SetnBackTaskParams(pact::nBackTaskParams a_params) = 0;

        virtual void SetNotifyMissedAcceptance(bool a_notify) = 0;

        virtual void SetNotifyMissedHandoff(bool a_notify) = 0;

        virtual void SetDisableCalloutRotation(bool a_disable = false) = 0;

        virtual void SetInfoBox(pact::TrialInfoBoxParams a_params) = 0;

        virtual void ShowClock(bool a_show = true) = 0;

	public slots:

		virtual void init() = 0;

		virtual void start() = 0;
//		virtual void pause() = 0;
//		virtual void resume() = 0;
		virtual void stop() = 0;
		
		virtual void reset() = 0;
		virtual void clear() = 0;

	signals:

		void tick(unsigned long);
		void timed_out(unsigned long);
		void timed_out();

		void conflicts();
		void TrialTimeout(long elapsed_time);
		void TimeEvent(int);
		void FailedAcceptanceTask(const char*);
		void FailedHandoffTask(const char*);
		void UpdateScores(float, float, float);
		void AircraftConflictResolved(std::string, std::string, int, int, int);
		void AircraftConflictFalseAlarm();
		void Conflicted(std::pair<std::string, std::string>);

	protected:
		unsigned int _trial_time;
		std::map<unsigned int, bool> _time_events;
	};

	typedef atc::Engine* create_t();
	typedef void destroy_t(atc::Engine*);

}

//#ifdef
	extern "C" ATC_API atc::Engine* create_engine();
	extern "C" ATC_API void destroy_engine(atc::Engine* e);
//#endif

#endif

/**
 * @file:       pact_trialtask.cpp
 *
 * 		        ATC-Lab Presentation Module
 *              Trial Presentation Task.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/11/04
 *
 * $Id: trialtask.cpp,v 1.13.2.11 2015/03/23 13:41:47 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#if defined (_MSC_VER)
#pragma comment(lib, "Winmm.lib")
#endif

#include "trialtask.h"
#include "ExcelLogger.h"
#include "engine/engine.h"
#include "ScoreLogger.h"
#include "canvasview.h"
#include <qlayout.h>

using namespace pact;

//------------------------------------------------------------------------------
// class constants
//

const char* TrialTask::MAIN_TARGET = "main_event_target";


//------------------------------------------------------------------------------
// construction/destruction
//

/*!
 */
TrialTask::TrialTask(const std::string& a_soundFile)
	  : Task()
	  , _engine( 0 )
	  , _ui_clock(new atc::Clock())
      , m_soundFile(a_soundFile.c_str())
{
	_engine = create_engine();
	connect(_engine, SIGNAL(conflicts()), this, SIGNAL(conflicts()));
	connect(_engine, SIGNAL(TrialTimeout(long)), 
						this, SLOT(HandleTrialTimeout(long)));
	connect(_engine, SIGNAL(TimeEvent(int)), this, SLOT(HandleTimeEvent(int)));
	connect(_engine, SIGNAL(FailedAcceptanceTask(const char*)),
			this, SLOT(HandleAircraftFailedAcceptanceTask(const char*)));
	connect(_engine, SIGNAL(FailedHandoffTask(const char*)),
			this, SLOT(HandleAircraftFailedHandoffTask(const char*)));
	connect(this, SIGNAL(UpdateScores(float, float, float)), 
			_engine, SIGNAL(UpdateScores(float, float, float)));
	connect(_engine, SIGNAL(AircraftConflictResolved(std::string, std::string, int, int, int)),
			this, SLOT(HandleAircraftConflictResolved(std::string, std::string, int, int, int)));
	connect(_engine, SIGNAL(AircraftConflictFalseAlarm()),
			this, SLOT(HandleAircraftConflictFalseAlarm()));
	connect(_engine, SIGNAL(Conflicted(std::pair<std::string, std::string>)),
			this, SLOT(HandleAircraftConflicted(std::pair<std::string, std::string>)));

    m_soundFile = a_soundFile.c_str();
    m_sound = new QSound(m_soundFile, this);

    if (m_soundFile != QString(""))
    {
        m_sound->play();
    }
}

/*!
 */
TrialTask::~TrialTask() {
    if (m_soundFile != QString(""))
    {
        m_sound->stop();
    }

    delete m_sound;

    ExcelLogging::ExcelLogger::instance()->LogTrialScore(_trial_name);
    ExcelLogging::ExcelLogger::instance()->WriteResultsForTrial(_trial_name);

	destroy_engine( _engine );

}

//------------------------------------------------------------------------------

/*!
 */
void TrialTask::initialize() {
	_engine->init();

	QWidget* view = _engine->create_view( this, MAIN_TARGET );

    _engine->SetCanvasView(dynamic_cast<atc::CanvasView*>(view));

	connect(view, SIGNAL(FailedAcceptanceTask(const char*)),
			this, SLOT(HandleAircraftFailedAcceptanceTask(const char*)));
	connect(view, SIGNAL(SucceededAcceptanceTask(const char*)),
			this, SLOT(HandleAircraftSucceededAcceptanceTask(const char*)));
	connect(view, SIGNAL(FailedHandoffTask(const char*)),
			this, SLOT(HandleAircraftFailedHandoffTask(const char*)));
	connect(view, SIGNAL(SucceededHandoffTask(const char*)),
			this, SLOT(HandleAircraftSucceededHandoffTask(const char*)));
	connect(this, SIGNAL(started()), view, SLOT(HandleTrialTaskStarted()));
	connect(this, SIGNAL(paused()), view, SLOT(HandleTrialTaskPaused()));

	QVBoxLayout* layout = new QVBoxLayout( this );
	layout->addWidget( view );
}

void TrialTask::SetTrialName(std::string trial_name)
{
	_trial_name = trial_name;
	ExcelLogging::ExcelLogger::instance()->LogFlashDetailsScriptName(trial_name);
}

void TrialTask::SetTimeEvent(unsigned int timeout)
{
	_engine->SetTimeEvent(timeout);
}

void TrialTask::SetUIClock(unsigned int interval)
{
	std::auto_ptr<atc::Clock> ui_clock(new atc::Clock());

	ui_clock->set_interval(interval);
	_ui_clock = ui_clock;

	connect(_ui_clock.get(), SIGNAL(tick()), this, SIGNAL(UIClockTick()));
	connect(_ui_clock.get(), SIGNAL(tick()), this, SLOT(HandleUIClockTick()));
}

void 
TrialTask::SetInterruptParams(InterruptionParams a_params)
{
    _engine->SetInterruptParams(a_params);    
}

void
TrialTask::SetnBackTaskParams(nBackTaskParams a_params)
{
    _engine->SetnBackTaskParams(a_params);
}

void 
TrialTask::SetNotifyMissedAcceptance(bool a_notify)
{
    _engine->SetNotifyMissedAcceptance(a_notify);
}

void
TrialTask::SetDisableCalloutRotation(bool a_disable)
{
    _engine->SetDisableCalloutRotation(a_disable);
}

void 
TrialTask::SetNotifyMissedHandoff(bool a_notify)
{
    _engine->SetNotifyMissedHandoff(a_notify);
}

void
TrialTask::ShowClock(bool a_show)
{
    _engine->ShowClock(a_show);
}
//------------------------------------------------------------------------------
// [protected] virtual
//

/*!
 * 
 */
void TrialTask::keyPressEvent( QKeyEvent* e ) {
	try
	{
		Task::keyPressEvent( e );

		int key = e->key();
		if ( e->state() == Qt::NoButton ) {
			
			//if      ( atc::ATC_PAUSE_KEY  == key ) pause();
			//else if ( atc::ATC_RESUME_KEY == key ) resume();
			//else                              return;

			e->accept();
		}
	}
	catch (...)
	{
		logging::Logger::instance()->error("An error occurred calling TrialTask::keyPressEvent");
	}
}

/*!
 */
void TrialTask::showEvent( QShowEvent* e ) {
	Task::showEvent( e );
	_engine->start();
	emit started();
}

//------------------------------------------------------------------------------
//

/*!
 * Pause/Resume trial task
 */
void TrialTask::pause()  
{ 
	_engine->stop(); 
	emit paused();
}

void TrialTask::resume() 
{ 
	_engine->start(); 
	emit started();
}

void TrialTask::HandleTrialTimeout(long elapsed_time)
{
	terminate();
}

void TrialTask::HandleAircraftFailedAcceptanceTask(const char* callsign)
{
	float totalAcceptanceTaskScore		= 0.0;
	float totalConflictScore			= 0.0;
	float totalHandoffTaskScore		= 0.0;

	Score::ScoreLogger::instance()->LogAcceptanceTaskScore(_trial_name.c_str(), Score::CORRECT_STATUS_INCORRECT);
	Score::ScoreLogger::instance()->GetCumulativeTotalScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

	emit UpdateScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);
}

void TrialTask::HandleAircraftSucceededAcceptanceTask(const char* callsign)
{
	float totalAcceptanceTaskScore	= 0.0;
	float totalConflictScore		= 0.0;
	float totalHandoffTaskScore	= 0.0;

	Score::ScoreLogger::instance()->LogAcceptanceTaskScore(_trial_name.c_str(), Score::CORRECT_STATUS_CORRECT);
	Score::ScoreLogger::instance()->GetCumulativeTotalScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

	emit UpdateScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);
}

void 
TrialTask::HandleAircraftFailedHandoffTask(const char* callsign)
{
	float totalAcceptanceTaskScore	= 0.0;
	float totalConflictScore		= 0.0;
	float totalHandoffTaskScore		= 0.0;

	Score::ScoreLogger::instance()->LogHandoffTaskScore(_trial_name.c_str(), Score::CORRECT_STATUS_INCORRECT);
	Score::ScoreLogger::instance()->GetCumulativeTotalScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

	emit UpdateScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);
}

void 
TrialTask::HandleAircraftSucceededHandoffTask(const char* callsign)
{
	float totalAcceptanceTaskScore	= 0.0;
	float totalConflictScore		= 0.0;
	float totalHandoffTaskScore		= 0.0;

	Score::ScoreLogger::instance()->LogAcceptanceTaskScore(_trial_name.c_str(), 
															Score::CORRECT_STATUS_CORRECT);	
	Score::ScoreLogger::instance()->GetCumulativeTotalScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

	emit UpdateScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);
}

void TrialTask::HandleAircraftConflictResolved(std::string conflict_aircraft1, 
												std::string conflict_aircraft2, 
												int acceptance_time, 
												int conflict_time, 
												int resolved_time)
{
	float totalAcceptanceTaskScore	= 0.0;
	float totalConflictScore		= 0.0;
	float totalHandoffTaskScore	= 0.0;
	Score::ScoreLogger::instance()->LogConflictResolvedScore(_trial_name.c_str(), 
																acceptance_time, 
																conflict_time, 
																resolved_time);
	
	Score::ScoreLogger::instance()->GetCumulativeTotalScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

	emit UpdateScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

}

void TrialTask::HandleAircraftConflictFalseAlarm()
{
	float totalAcceptanceTaskScore	= 0.0;
	float totalConflictScore		= 0.0;
	float totalHandoffTaskScore	= 0.0;

	Score::ScoreLogger::instance()->LogConflictFalseAlarm(_trial_name.c_str());

	Score::ScoreLogger::instance()->GetCumulativeTotalScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

	emit UpdateScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

}

void TrialTask::UIClockStart()
{
	if (_ui_clock.get())
	{
		_ui_clock->start();
	}
}

void TrialTask::HandleUIClockTick()
{
	if (_ui_clock.get())
	{
		_ui_clock->stop();
	}
}

void TrialTask::HandleTimeEvent(int time_event_no)
{
	switch (time_event_no)
	{
	case 0:
		emit TimeEvent1();
		break;
	case 1:
		emit TimeEvent2();
		break;
	case 2:
		emit TimeEvent3();
		break;
	case 3:
		emit TimeEvent4();
		break;
	default:
		emit TimeEvent1();
		break;
	}
}

void TrialTask::HandleAircraftConflicted(std::pair<std::string, std::string>)
{
	float totalAcceptanceTaskScore	= 0.0;
	float totalConflictScore		= 0.0;
	float totalHandoffTaskScore	= 0.0;

	Score::ScoreLogger::instance()->LogConflictScore(_trial_name.c_str());

	Score::ScoreLogger::instance()->GetCumulativeTotalScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);

	emit UpdateScores(totalAcceptanceTaskScore, totalConflictScore, totalHandoffTaskScore);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

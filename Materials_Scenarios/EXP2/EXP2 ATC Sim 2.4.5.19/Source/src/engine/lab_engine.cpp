/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: lab_engine.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.6.2.7 $ )
      @date     : $Date: 2015/02/23 13:34:42 $
      @state    : $State: Exp $

      $Log: lab_engine.cpp,v $
      Revision 1.6.2.7  2015/02/23 13:34:42  Aaron Yeung
      - Added logging of interruption tasks to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element				Description							Values
                *******************************************************************************************************************
      	  interruption_type 			Type of interruption						"Interruption" or "NBack"
          	  start_time				Programmed start time of interruption
      	  end_time				Programmed end time of interruption
          	  show_full_screen			Whether the task was shown in full screen			0 = no, 1 = yes
          	  external_program			Name of external program that was run
          	  auto_accept				Whether aircraft were auto accepted during interruption		0 = no, 1 = yes
          	  auto_handoff				Whether aircraft were auto handed off during interruption	0 = no, 1 = yes

      	 - Added logging of interruption tasks to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                Field			Description							Values
                *******************************************************************************************************************
      	  Trial			Name of the trial that the interruption occurred in
      	  Interruption Type	Type of interruption						"Interruption" or "NBack"
          	  Start Time		Programmed start time of interruption
      	  End Time		Programmed end time of interruption
          	  Show Full Screen	Whether the task was shown in full screen			0 = no, 1 = yes
          	  External Program	Name of external program that was run
          	  Auto Accept		Whether aircraft were auto accepted during interruption		0 = no, 1 = yes
          	  Auto Handoff		Whether aircraft were auto handed off during interruption	0 = no, 1 = yes

      	 - Added additional field in the aircraft flash details section of the output .csv file called "Automatic" to indicate if an aircraft is
      	   automatically accepted or handed off. Values are 0 (not automatic) or 1 (automatic acceptance/handoff)

      	 - Added logging of reminder box dismiss button times to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  reminder_box		Tag to indicate that the reminder box dismiss button was pressed	Aircraft callsign that had the dismiss button pressed
      	  dismiss_button	Tag to indicate that the reminder box dismiss button was pressed	"pressed"
      	  clicked_time		Millisecond time that the dismiss button was pressed

      	- Added logging of reminder box dismiss button times to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                Field			Description								Values
                *******************************************************************************************************************
      	  Trial  		Name of the trial that the dismiss button was pressed
      	  Callsign		Aircraft callsign that had the dismiss button pressed
      	  Dismiss Time		Time in seconds into the trial that the dismiss button was pressed
      	  Dismiss Time HP	Time in milliseconds into the trial that the dismiss button was pressed

      	- Added calculation of Nback task results based on participant's inputs/non-inputs in the N back task. The are described as follows:
      	  ***********************************************************************************************************************
      	  Result		Description
      	  ***********************************************************************************************************************
      	  Hit			Participant correctly responds to a stimuli that was the same as the stimuli N times before
      	  Miss			Participant fails to respond to a stimuli that was the same as the stimuli N times before
      	  Correct rejection	Participant correct ignores the stimuli which is not the same as the stimulis N times before
      	  False alarm		Paricipant incorrectly responds to a stimuli that was not the same as the stimuli N times before

      	 - Added logging of N back task results to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  NBackTask_response	Stimuli order of the stimuli that was presented
      	  stimuli		The actual stimuli that was displayed to the participant
      	  response		The result of the paritcipant's response				hit, miss, false_alarm, correct_rejection

      	 - Added logging of N back task results to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  Trial	  		Name of the trial that the N back response was generated in
      	  Stimuli Number	Stimuli order of the stimuli that was presented
      	  Stimuli		The actual stimuli that was displayed to the participant
      	  Response		The result of the paritcipant's response				hit, miss, false_alarm, correct_rejection

      	 - Added a count down timer for the blank interruption task. It can be enabled/disabled with the XML below:
      	  *******************************************************************************************************************
                XML Element		Attribute	Description							Values
                *******************************************************************************************************************
      	  atc:interruption	 atc:show_timer	Enable/disable countdown timer in the blank interruption task	'true' or 'false'

      	  Example:
      	  <atc:interruption atc:start='3' atc:end='21' atc:auto_handoff='true' atc:auto_accept='false' atc:show_blank_screen='true' atc:show_timer='true' atc:external_program='"calc.exe"'/>

      	  - Added ability to enable/disable error pop up dialogs when a paricipant misses an aircraft acceptance or handoff.
      	    This can be achieved with the XML below:
      	  ***********************************************************************************************************************************************
                XML Element	Attribute			Description									Values
                ***********************************************************************************************************************************************
      	  atc:trial	atc:notify_missed_acceptance	Enable/disable pop up error dialog when a paricipant misses an acceptance	'true' or 'false'
      			atc:notify_missed_handoff	Enable/disable pop up error dialog when a paricipant misses a handoff		'true' or 'false'

      	  Example:
      	  <atc:trial atc:idx='test2' atc:param='default' atc:map="map1" atc:sky="script2" atc:ui="ui001" atc:notify_missed_acceptance='false' atc:notify_missed_handoff='false'>

      Revision 1.6.2.6  2015/02/16 14:25:59  Aaron Yeung
      Fixed corrupted files

      Revision 1.6.2.3  2014/12/16 11:50:18  Aaron Yeung
      2.4.5.10  - Modified aircraft behaviour such that if aircraft are ready to be accepted (flashing orange) or ready to be handed off (flashing blue) at the time of interruption, that aircraft will automatically be accepted or handed off.

      Revision 1.6.2.2  2014/12/10 13:26:18  Aaron Yeung
      ATC Version 2.4.5.8:

      - Implemented scriptable interruption task with configurable start time, end time, automatic aircraft acceptance,
      	    automatic aircraft handoff and the displaying of a full black screen
                *******************************************************************************************************************
                XML Element		Attributes  		Description
                *******************************************************************************************************************
      	  atc:interruption	atc:start		Trial time when the interruption begins
      				atc:end			Trial time when the interruption ends
      				atc:auto_handoff	Set if aircraft are automatically handed off during the interruption (true/false)
      				atc:auto_accept		Set if aircraft are automatically accepted during the interruption (true/false)
      				atc:show_blank_screen	Set if a full black screen is to be displayed to hide the aircraft simulation (true/false)
      				atc:external_program	Not implemented yet

      Revision 1.6.2.1  2012/03/14 20:18:49  Aaron Yeung
      Added millisecond timing to the Excel log file

      Revision 1.6  2010/04/08 13:19:31  Aaron Yeung
      Initial attempt at adding reminder box to individual aircraft

      Revision 1.5  2008/10/21 13:57:51  Aaron Yeung
      - Added functionality for experimenter to re-ask a question if the participant gives the wrong answer.
      - Added new xml element
      <questionBox>
              <atc:answer></atc:answer>
              <atc:incorrectResponse></atc:incorrectRespons>
      </questionBox>

      Revision 1.4  2007/07/26 12:16:29  Aaron Yeung
      no message

      Revision 1.3  2007/07/15 04:02:09  Aaron Yeung
      Added xml scriptable trial time

      Revision 1.2  2007/07/08 04:50:09  Aaron Yeung
      Added signal handler for the clock in airspace to update each aircraft's active attribute depending on whether they have been set to enter the screen yet

      Revision 1.1.1.2  2007/06/21 12:22:44  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.11  2007/02/27 06:07:17  seth
      implemented clock

      Revision 1.10  2006/11/01 04:53:36  seth
      created installer

      Revision 1.9  2006/10/26 05:59:52  seth
      working implementation of experiment time logging

      Revision 1.8  2006/10/25 04:49:58  seth
      fixing logging

      Revision 1.7  2006/10/20 04:49:23  seth
      reduced dimensions of workload popup question && initial implementation of experiment time (for inclusion in log file output)

      Revision 1.6  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.5  2006/09/14 07:28:06  seth
      inital implementation of sector controller state

      Revision 1.4  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.3  2006/08/29 08:44:56  seth
      reimplementing mvc pattern

      Revision 1.2  2006/08/28 06:21:48  seth
      refactoring mvc

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "lab_engine.h"

#include "aircraft_agent.h"
#include "airspace.h"
#include "canvas.h"
#include "clock.h"

#include "logging/logger.h"
#include "timer.h"
#include <windows.h>

using namespace atc;
using std::make_pair;
using std::string;


////////////////////////////////////////////////////////////////////////////////
//
// atc::LabEngine
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
LabEngine::LabEngine()
	  : _airspace()
	  , _canvas()
	  , _clock()
	  , _trial_time(0)
      , m_runMode(RUN_MODE_NORMAL)
{
    connect(this, SIGNAL (event_recorded(KeyValues &)),
            this, SLOT   (handle_information(KeyValues &)));
}

/*!
 *
 */
LabEngine::~LabEngine() {}


//------------------------------------------------------------------------------
// [public] intialization - builder interface
//

/*!
 * Set the airspace for the engine
 *
 * The engine will take ownership of the airspace.
 *
 * @param airspace is a valid airspace
 */
void LabEngine::set_airspace( AirspacePtr &airspace ) { 
	_airspace = airspace;
	airspace->initialize();
	initalize_visualizer();

	connect( 
		    airspace.get(), SIGNAL ( event_recorded    ( KeyValues & ) )
		  , this,           SLOT   ( handle_information( KeyValues & ) )
	);
}

/*!
 * Set the canvas visualizer for the engine
 *
 * The engine will take ownership of the canvas.
 *
 * @param canvas is a valid canvas
 */
void LabEngine::set_canvas( std::auto_ptr< Canvas > &canvas ) {
	_canvas = canvas;
    
    _canvas->SetInterruptParams(m_interruptParams);
    _canvas->SetnBackTaskParams(m_nBackTaskParams);

	initalize_visualizer();

	connect( 
		    canvas.get(), SIGNAL ( event_recorded    ( KeyValues & ) )
		  , this,         SLOT   ( handle_information( KeyValues & ) )
	);
}


/*!
 * Set the clock for the engine
 *
 * The engine will take ownership of the clock.
 *
 * @param clock 
 */
void LabEngine::set_clock( ClockPtr &clock ) {
	_clock = clock;

	connect(_clock.get(),	SIGNAL(seconds_update(long)),
			_airspace.get(),SLOT(UpdateAircraft(long)));

	connect(_clock.get(),	SIGNAL(seconds_update(long)),
			this,			SLOT(HandleClockUpdate(long)));

	connect(_clock.get(), SIGNAL (event_recorded    (KeyValues &)),
		    this,         SLOT   (handle_information(KeyValues &)));

	connect(_clock.get(),	SIGNAL(tick(int)),
		    _airspace.get(),SLOT(advance(int)));

	initalize_visualizer();
}

//------------------------------------------------------------------------------
// [public]
//

/*!
 *
 */
void 
LabEngine::start() 
{ 
	_clock->start();
    Timer::instance()->start();
}

void 
LabEngine::stop()  
{ 
	_clock->stop(); 
}

void
LabEngine::SetInterruptParams(pact::InterruptionParams a_params)
{
    m_interruptParams = a_params;
}

void 
LabEngine::SetnBackTaskParams(pact::nBackTaskParams a_params)
{
    m_nBackTaskParams = a_params;
}

//------------------------------------------------------------------------------
// [public] slots:
//

void LabEngine::HandleClockUpdate(long seconds_elapsed)
{
	if (_trial_time != 0)
	{
        bool start_interruption =   // interrupt task conditions
                                    ((m_interruptParams.m_start != atc::INVALID_VALUE) &&
                                    (seconds_elapsed >= m_interruptParams.m_start) &&
                                    (m_runMode != RUN_MODE_INTERRUPTION) &&
                                    (seconds_elapsed <= m_interruptParams.m_end)) ||
                                    
                                    // nback task conditions
                                    ((m_nBackTaskParams.m_start != atc::INVALID_VALUE) &&
                                    (seconds_elapsed >= m_nBackTaskParams.m_start) &&
                                    (m_runMode != RUN_MODE_INTERRUPTION) &&
                                    (seconds_elapsed <= m_nBackTaskParams.m_end));

        if (start_interruption)
        {
            m_runMode = RUN_MODE_INTERRUPTION;

            _canvas->SetRunMode(m_runMode);

            if (m_interruptParams.m_start != atc::INVALID_VALUE)
            {
                _airspace->SetAircraftAutoAccept(m_interruptParams.m_auto_accept);
                _airspace->SetAircraftAutoHandoff(m_interruptParams.m_auto_handoff);

                if (m_interruptParams.m_show_blank_screen)
                {
                    _canvas->ShowInterruptScreen();
                }

                if (m_interruptParams.m_external_program != "")
                {
                    wchar_t* open = new wchar_t[4096];
                    MultiByteToWideChar(CP_ACP, 0, "open", -1, open, 4096);

                    wchar_t* command = new wchar_t[4096];
                    MultiByteToWideChar(CP_ACP, 0, m_interruptParams.m_external_program.c_str(), -1, command, 4096);

                    ShellExecute(NULL, open, command, NULL, NULL, SW_SHOWNORMAL);
                }
            }

            if (m_nBackTaskParams.m_start != atc::INVALID_VALUE)
            {
                _airspace->SetAircraftAutoAccept(m_nBackTaskParams.m_auto_accept);
                _airspace->SetAircraftAutoHandoff(m_nBackTaskParams.m_auto_handoff);

                if (m_nBackTaskParams.m_show_task)
                {
                    _canvas->ShowNBackTask();
                }
            }
        }

        bool end_interruption = // interrupt task conditions
                                ((m_interruptParams.m_end != atc::INVALID_VALUE) &&
                                (seconds_elapsed >= m_interruptParams.m_end) &&
                                (m_runMode != RUN_MODE_NORMAL)) ||

                                // nback task conditions
                                ((m_nBackTaskParams.m_end != atc::INVALID_VALUE) &&
                                (seconds_elapsed >= m_nBackTaskParams.m_end) &&
                                (m_runMode != RUN_MODE_NORMAL));
        
        if (end_interruption)
        {
            m_runMode = RUN_MODE_NORMAL;

            _canvas->SetRunMode(m_runMode);
            _airspace->SetAircraftAutoAccept(false);
            _airspace->SetAircraftAutoHandoff(false);

            if (m_interruptParams.m_show_blank_screen)
            {
                _canvas->HideInterruptScreen();
            }

            if ((m_nBackTaskParams.m_end != atc::INVALID_VALUE) &&
                (m_nBackTaskParams.m_show_task))
            {
                _canvas->HideNBackTask();
            }
        }

		if (seconds_elapsed >= _trial_time)
		{
			emit TrialTimeout(seconds_elapsed);
		}
	}
}

/*!
 *
 */
void LabEngine::handle_information( KeyValues &kvs ) {
	kvs.push_front( make_pair( 
			string( "elapsed_time" ), to_string( _clock->elapsed_time() ) 
	) );

    kvs.push_front(make_pair(string("hpc_elapsed_time"), to_string(Timer::instance()->elapsed_time())));

	logging::KVPairs data;
	
	KeyValues::const_iterator kv = kvs.begin();
	for ( ; kv != kvs.end(); ++kv ) {
		data.push_back( *kv );
	}
	logging::Logger::instance()->info( data );

	//emit state_data( kvs );
}


//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 * Initialize the visualizer
 *
 * Create visual representation for each aircraft in airspace and connect
 * signal/slot mechanisms.
 */
void LabEngine::initalize_visualizer() {
	if ( ! _canvas.get() || ! _airspace.get() || ! _clock.get() ) return;

	// add aircraft to canvas
	Airspace::iterator ac = _airspace->begin();
	for ( ; ac != _airspace->end(); ++ac ) {
		_canvas->add_aircraft( ac->second );
	}

	//
	_canvas->update_aircraft();

	//
	QObject::connect( 
			_airspace.get(), SIGNAL( advanced() )
		  , _canvas.get(), SLOT( update_aircraft() )
	);

	QObject::connect(
			_canvas.get(), SIGNAL( pause_request() ) 
		  , _clock.get(), SLOT( stop() )
	);

	QObject::connect(
			_canvas.get(), SIGNAL( resume_request() )
		  , _clock.get(), SLOT( start() )
	);

	QObject::connect(
		    _clock.get(), SIGNAL( seconds_update( long ) )
		  , _canvas.get(), SLOT( set_clock( long ) )
	);

	QObject::connect(	_clock.get(),			SIGNAL( seconds_update(long)),
						_canvas->GetReminderBox(), SLOT(UpdateSeconds(long)));

	QObject::connect(
		    _clock.get(), SIGNAL( stopped() )
		  , _canvas.get(), SLOT( stop_clock() )
	);
	QObject::connect(
		    _clock.get(), SIGNAL( started() )
		  , _canvas.get(), SLOT( start_clock() )
	);

}
////////////////////////////////////////////////////////////////////////////////

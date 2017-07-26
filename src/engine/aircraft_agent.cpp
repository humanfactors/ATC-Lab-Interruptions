/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: aircraft_agent.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.24.2.7 $ )
      @date     : $Date: 2015/03/05 11:46:14 $
      @state    : $State: Exp $

      $Log: aircraft_agent.cpp,v $
      Revision 1.24.2.7  2015/03/05 11:46:14  Aaron Yeung
      Decreased aircraft acceptance time window from 20 seconds to 15 seconds

      Revision 1.24.2.6  2015/02/23 13:34:41  Aaron Yeung
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

      Revision 1.24.2.5  2014/12/16 11:50:18  Aaron Yeung
      2.4.5.10  - Modified aircraft behaviour such that if aircraft are ready to be accepted (flashing orange) or ready to be handed off (flashing blue) at the time of interruption, that aircraft will automatically be accepted or handed off.

      Revision 1.24.2.4  2014/12/10 13:26:17  Aaron Yeung
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

      Revision 1.24.2.3  2014/12/02 13:48:59  Aaron Yeung
      Implemented scriptable handoff key for individual aircraft - <atc:handoffkey>
      Default handoff key is still the 'h' key

      Revision 1.24.2.2  2014/12/02 12:06:36  Aaron Yeung
      Added scriptable altitude change time restriction to aircraft <atc:altitudeChangeEnableTime>

      Revision 1.24.2.1  2012/03/14 20:17:54  Aaron Yeung
      Added millisecond timing to the Excel log file

      Revision 1.24  2009/04/27 13:37:25  Aaron Yeung
      Cosmetic cleanup

      Revision 1.23  2008/10/21 13:57:50  Aaron Yeung
      - Added functionality for experimenter to re-ask a question if the participant gives the wrong answer.
      - Added new xml element
      <questionBox>
              <atc:answer></atc:answer>
              <atc:incorrectResponse></atc:incorrectRespons>
      </questionBox>

      Revision 1.22  2008/07/12 19:44:34  Aaron Yeung
      Added a pre handoff state to aircraft so that they flash blue when they exit the sector

      Revision 1.21  2008/06/30 11:11:51  Aaron Yeung
      Added flashing aircraft for handoff task
      Modified aircraft acceptance and handoff tasks so that a count of how many times an aircraft is accepted/handed off

      Revision 1.20  2007/08/08 14:47:53  Aaron Yeung
      Exclude points scoring for resolving an aircraft conflict when its conflicting pair has already been resolved

      Revision 1.19  2007/08/08 14:03:31  Aaron Yeung
      Only allow score to be incremented once when the user correctly resolves a conflict

      Revision 1.18  2007/08/08 13:51:58  Aaron Yeung
      Added penalty if the user changes the flight level of an aircraft that is scripted to conflict at times less than the acceptance time and greater than the conflict time

      Revision 1.17  2007/08/01 15:17:13  Aaron Yeung
      Added scoring for conflicts

      Revision 1.16  2007/07/31 14:49:47  Aaron Yeung
      Added Excel logging functionality

      Revision 1.15  2007/07/26 10:22:05  Aaron Yeung
      Disallowed acceptance of aircraft that are not in the proposed state

      Revision 1.14  2007/07/22 07:34:27  Aaron Yeung
      Added dialog box when pairs of aircraft conflict

      Revision 1.13  2007/07/18 14:32:02  Aaron Yeung
      Added Coordination Request functionality for aircrafts

      Revision 1.12  2007/07/17 12:15:06  Aaron Yeung
      Enabled pausing of AircraftAgent proposed_state_change_timer when pausing the whole trial. Please note that there is an issue when pausing and a user has already accepted an aircraft but not completed the acceptance task. May need to revert back to the previous version checked in on the 16th July 07

      Revision 1.11  2007/07/16 10:54:06  Aaron Yeung
      Modified acceptance task so that a user can continue to try to press the correct key for an acceptance task until the time limit is exceeded. Previously, if the user pressed the wrong key, they would not be able to re-enter the correct key again

      Revision 1.10  2007/07/15 07:47:40  Aaron Yeung
      Added Utilities singleton

      Revision 1.9  2007/07/15 07:18:17  Aaron Yeung
      Included aircraft callsign in failed acceptance task dialog box

      Revision 1.8  2007/07/10 13:46:45  Aaron Yeung
      Added functionality to calculate aircraft conflicts

      Revision 1.7  2007/07/10 11:50:41  Aaron Yeung
      Removed CSOVEROUT and handoff time functionality

      Revision 1.6  2007/07/09 12:38:40  Aaron Yeung
      Fixed code so that aircraft with start times of 0 seconds appear on screen at 0 seconds

      Revision 1.5  2007/07/09 08:34:00  Aaron Yeung
      Added timeout for when an aircraft changes to the CS_PROPOSED for the user to complete the aircraft acceptance task or an error dialog box will be displayed and the aircraft will automaticallly go into the CS_ACCEPTED_TASK_COMPLETE state

      Revision 1.4  2007/07/09 00:36:24  Aaron Yeung
      renamed aircraft accepted state blink items to use the word flash instead

      Revision 1.3  2007/07/08 17:05:10  Aaron Yeung
      Added method to return blink params stored in aircraft_agent

      Revision 1.2  2007/07/08 04:47:29  Aaron Yeung
      Added UpdateAircraft timer handler to set an aircraft active x seconds (determined by XML config fille) after the trial has started

      Revision 1.1.1.2  2007/06/21 12:22:41  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.32  2007/02/26 08:03:45  seth
      automatic handoff

      Revision 1.31  2006/11/07 03:05:44  seth
      added additional logging

      Revision 1.30  2006/11/01 04:53:36  seth
      created installer

      Revision 1.29  2006/10/25 04:49:58  seth
      fixing logging

      Revision 1.28  2006/10/18 03:58:33  seth
      snap functionality for selection & fixed bug causing access violation once aircraft had reached destination

      Revision 1.27  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.26  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.25  2006/09/24 00:28:03  seth
      fixed vectoring and rerouting

      Revision 1.24  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.23  2006/09/20 06:44:01  seth
      vector selection box + some bug fix

      Revision 1.22  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.21  2006/09/18 12:41:07  seth
      refactored aircraft agent state updates - blink is causing access violation!

      Revision 1.20  2006/09/18 06:41:19  seth
      updating state calcutlations

      Revision 1.19  2006/09/17 12:45:35  seth
      implementing solution mechanisms

      Revision 1.18  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.17  2006/09/14 07:28:06  seth
      inital implementation of sector controller state

      Revision 1.16  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.15  2006/09/11 06:13:44  seth
      working on route tool

      Revision 1.14  2006/09/07 06:46:21  seth
      reimplementing tools - history and short route probe

      Revision 1.13  2006/09/06 06:22:23  seth
      transformation problems

      Revision 1.12  2006/09/04 04:47:10  seth
      fixed various problems with performance data + heading calcs

      Revision 1.11  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.10  2006/08/29 08:44:56  seth
      reimplementing mvc pattern

      Revision 1.9  2006/08/24 04:12:51  seth
      seperating model from visuals

      Revision 1.8  2006/08/21 06:21:43  seth
      fixing bad performance data cruise speed

      Revision 1.7  2006/08/18 06:11:38  seth
      bug fixes

      Revision 1.6  2006/08/15 11:48:46  seth
      aircraft agents initialized with performance data

      Revision 1.5  2006/08/07 06:22:46  seth
      implemtation of performance data

      Revision 1.4  2006/07/19 08:39:18  seth
      implementing aircraft agent

      Revision 1.3  2006/06/22 06:28:41  seth
      refactoring for performance data inclusion

      Revision 1.2  2006/06/15 03:58:57  seth
      refactoring aircraft agents

      Revision 1.1  2006/06/07 05:50:14  seth
      refactor prior to implementation of performance data lookup


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "aircraft_agent.h"

#include "aircraft_data.h"
#include "aircraftsprite.h"
#include "canvas.h"

#include "logging/logger.h"

#include "performance/factory.h"
#include "performance/performance.h"

#include <cmath>
#include <qtimer.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <math.h>
#include "Utilities.h"
#include "logging/ExcelLogger.h"
#include "timer.h"
#include <algorithm>

#ifdef _WIN32
	#include <assert.h>
#endif

using namespace atc;
using std::make_pair;
using std::string;

////////////////////////////////////////////////////////////////////////////////
//
// atc::AircraftAgent
//
//------------------------------------------------------------------------------
// construction/destruction
//
const int PROPOSED_STATE_TIMEOUT_PERIOD = 15000;	//milliseconds
const int PRE_HANDOFF_STATE_TIMEOUT_PERIOD	= 15000;	//milliseconds
/*!
 *
 */
AircraftAgent::AircraftAgent( QObject *parent, const char *name )
	  : QObject( parent, name )
	  , _active( false )
	  , _cfl( NAN )
	  , _vel( NAN )
	  , _data( 0 )
	  , _path( 0 )
	  , _state( 0 )
	  , _history( new AircraftHistory() )
	  , _performance( 0 )
	  //, _overout_time( 0 )
	  //, _handoff_time( 0 )
	  , _vectoring( false )
	  , _conflicted(false)
	  , _conflicted_last_update( false )
	  , _require_level( false )
	  , _tmp_throttle( 0.0 )
	  , _current_time(0)
	  , _proposed_state_change_start_time(0)
	  , _proposed_state_change_timer(0)
	  , _conflict_details()
	  , _conflict_resolved(false)
	  , m_acceptanceCount(0)
	  , m_preHandoffStartTime(0)
	  , m_handoffCount(0)
      , m_enableAltitudeChange(false)
      , m_autoAccept(false)
      , m_autoHandoff(false)
{}

/*!
 *
 */
AircraftAgent::~AircraftAgent() {
//	if ( _performance ) delete _performance;	// do not own
	if ( _history     ) delete _history;
//	if ( _state       ) delete _state;
	if ( _path        ) delete _path;
	if ( _data        ) delete _data;
}

//-----------------------------------------------------------------------------
// [public] initialize
//

/*!
 * Set attributes describing aircraft
 */
void AircraftAgent::set_data( const AircraftData &data ) {
	assert( ! _data );
	_data = new AircraftData( data );
	_performance = performance::Factory::performance( data.type );
}

/*!
 * Set the initial flight path for aircraft
 */
void AircraftAgent::set_path( const AircraftPath &path ) {
	assert( ! _path );
	if ( ! path.is_valid() ) { 
		throw aircraft_agent_error( "invalid aircraft flight path" );
	}

	_path = new AircraftPath( path );

	initialize_state();
}

void
AircraftAgent::SetAutoAccept(bool a_autoAccept)
{
    m_autoAccept = a_autoAccept;

    // If the aircraft is currently in CS_PROPOSED state, automatically accept it
    if ((_state->control == CS_PROPOSED) && m_autoAccept)
	{
        // Automatically accept aircraft in interruption mode
        set_control(CS_ACCEPTED_TASK_INCOMPLETE);
	}
}

void
AircraftAgent::SetAutoHandoff(bool a_autoHandoff)
{
    m_autoHandoff = a_autoHandoff;

    // If the aircraft is currently in CS_PRE_HANDOFF state, automatically handoff
    if ((_state->control == CS_PRE_HANDOFF) && m_autoHandoff)
	{
        set_control(CS_HANDOFF);
	}
}

void AircraftAgent::UpdateAircraft(long current_time)
{
	_current_time = current_time;

	if ((!_active) && (current_time >= _data->start_time) && (!_path->done()))
	{
		_active = true;
	}

    if ((!m_enableAltitudeChange) && (_current_time >= _data->m_altitudeChangeEnableTime))
    {
        m_enableAltitudeChange = true;
    }
}

//-----------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
void AircraftAgent::advance_position( int ms ) {
	if ( ! _active ) return;

	// check for activation
	_history->add_event( _state );
	advance_state( ms );
	//activate( _state );
}

/*!
 *
 */
bool AircraftAgent::collides_with( AircraftAgent *other_aircraft, double hdoms, double vdoms) 
{
	bool collide = false;

	if (active() && other_aircraft->active())
	{
		double vertical_diff = z() - other_aircraft->z();
		double x_diff = x() - other_aircraft->x();
		double y_diff = y() - other_aircraft->y();
		double horizontal_distance = sqrt(((x_diff * x_diff) + (y_diff * y_diff)));
		
		collide = ((horizontal_distance <= hdoms) && (fabs(vertical_diff) <= vdoms));
	}

	if (collide)
	{
		bool found = false;

		for (std::vector<std::string>::iterator it = _conflicting_aircrafts.begin();
				it != _conflicting_aircrafts.end(); it++)
		{
			if (*it == std::string(other_aircraft->callsign()))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			_conflicting_aircrafts.push_back(std::string(other_aircraft->callsign()));
		}
		
		if (!_conflicted_last_update)
		{
			_conflicted_last_update = true;
			emit Conflicted(std::make_pair(std::string(callsign()), std::string(other_aircraft->callsign())));
		}

		ExcelLogging::ExcelLogger::instance()->LogInterventionDetailsMissedAircraft(std::string(callsign()));
	}
	else
	{
		for (std::vector<std::string>::iterator it = _conflicting_aircrafts.begin();
				it != _conflicting_aircrafts.end(); it++)
		{
			if (*it == std::string(other_aircraft->callsign()))
			{
				_conflicting_aircrafts.erase(it);
				break;
			}
		}
	}

	return collide;
}

std::vector<std::string> AircraftAgent::CollidesWith() 
{
	return _conflicting_aircrafts;
}

/*!
 *
 */
void AircraftAgent::set_cfl( int cfl ) {
	KeyValues kvs;
	kvs.push_back( make_pair( string( "solution" ), string( "level_variation" ) ) );
	kvs.push_back( make_pair( string( "new_cfl"  ), to_string( cfl ) ) );
	kvs.push_back( make_pair( string( "old_cfl"  ), to_string( _cfl ) ) );
	record_event( kvs );

	_cfl = cfl;

	ExcelLogging::ExcelLogger::instance()->LogInterventionDetails(std::string(callsign()), 
											_current_time, 
                                            Timer::instance()->elapsed_time(),
											cfl);
	if (ShouldConflict())
	{
		if ((_current_time >= _data->conflict_acceptance_time) &&
			(_current_time < _data->conflict_time))
		{
			if (!_conflict_resolved)
			{
				_conflict_resolved = true;

				emit AircraftConflictResolved(	callsign(),
												_data->conflict_aircraft,
												_data->conflict_acceptance_time, 
												_data->conflict_time,
												_current_time);
			}
		}
		else
		{
			emit AircraftConflictFalseAlarm();
		}
	}
	else
	{
		// False Alarm
		emit AircraftConflictFalseAlarm();
	}

	// @todo: check valid
	// @todo: invalidate path cfl's
}

/*!
 * Set the cleared velocity ( similar to cleared flight level concept )
 *
 * Velocity is dependant on altitude so that changes in altitude will effect
 * actual velocity and a specific cleared velocity may not be appropriate at 
 * a different level. The "cleared" velocity is therefore implemented in terms
 * of the throttle setting
 */
void AircraftAgent::set_velocity( int v ) {
	double old_t = _vel;
	double old_v = _state->velocity;
	double new_t = _performance->throttle_setting( z(), v );
	double new_v = v;
	double alt   = _state->z;

	_vel = new_t;
	// @todo: check valid
	// @todo: invalidate path velocities

	KeyValues kvs;
	kvs.push_back( make_pair( string( "solution"     ), string( "speed_variation" ) ) );
	kvs.push_back( make_pair( string( "new_velocity" ), to_string( new_v ) ) );
	kvs.push_back( make_pair( string( "new_throttle" ), to_string( new_t ) ) );
	kvs.push_back( make_pair( string( "old_velocity" ), to_string( old_v ) ) );
	kvs.push_back( make_pair( string( "old_throttle" ), to_string( old_t ) ) );
	kvs.push_back( make_pair( string( "altitude"     ), to_string( alt   ) ) );
	record_event( kvs );
}

/*!
 *
 */
int AircraftAgent::accept()  { 
	if (_state->control == CS_PROPOSED)
	{
		KeyValues kvs;
		kvs.push_back( make_pair( string( "new_control" ),    string( "accepted"      ) ) );
		kvs.push_back( make_pair( string( "old_control" ), to_string( _state->control ) ) );
		record_event( kvs );

		set_control(CS_ACCEPTED_TASK_INCOMPLETE);
		
		// If this aircraft was previously incorrectly accepted, the m_acceptanceCount would
		// be less than 0. Now that it is in acceptance mode, set m_acceptanceCount back to
		// 0 so that the aircraft can only be accepted once.
		if (m_acceptanceCount < 0) 
		{
			m_acceptanceCount = 0;
		}
		m_acceptanceCount++;
		
		if (_proposed_state_change_timer)
		{
			_proposed_state_change_timer->stop();
			_proposed_state_change_start_time = 0;
			delete _proposed_state_change_timer;
		}
	}
	else
	{
		// Disregard acceptance attempts when the aircraft has previously been 
		// successfully accepted
		if (m_acceptanceCount <= 0)
		{
			m_acceptanceCount--;
		}
		else
		{
			m_acceptanceCount++;
		}
	}

	return m_acceptanceCount;
}

/*!
 *
 */
int 
AircraftAgent::handoff() 
{ 
	if (_state->control == CS_PRE_HANDOFF)
	{
		KeyValues kvs;
		kvs.push_back( make_pair( string( "new_control" ),    string( "handoff"       ) ) );
		kvs.push_back( make_pair( string( "old_control" ), to_string( _state->control ) ) );
		record_event( kvs );

		set_control(CS_HANDOFF);
		
		// If this aircraft was previously incorrectly handed off, the m_handoffCount would
		// be less than 0. Now that it is in handoff mode, set m_handoffCount back to
		// 0 so that the aircraft can only be handed off once.
		if (m_handoffCount < 0)
		{
			m_handoffCount = 0;
		}

		m_handoffCount++;

		if (m_preHandoffStateTimer.get())
		{
			m_preHandoffStateTimer->stop();
			m_preHandoffStartTime = 0;

			QTimer* timer = m_preHandoffStateTimer.release();
			delete timer;
		}
	}
	else
	{
		// Disregard handoff attempts when the aircraft has previously been 
		// successfully handed off
		if (m_handoffCount <= 0)
		{
			m_handoffCount--;
		}
		else
		{
			m_handoffCount++;
		}
	}
	//_handoff_time = 0;

	return m_handoffCount;
}

/*!
 *
 */
void AircraftAgent::set_control( ControlState cs ){
	if ( _state->control != cs ) {
		_state->control = cs;

		if (cs == CS_PROPOSED)
		{
            ExcelLogging::ExcelLogger::instance()->LogAcceptanceFlashDetails(   callsign(), 
                                                                                _current_time, 
                                                                                Timer::instance()->elapsed_time());

            if (!m_autoAccept)
            {
			    _proposed_state_change_timer = new QTimer(this);
			    _proposed_state_change_timer->start(PROPOSED_STATE_TIMEOUT_PERIOD, true);
			    _proposed_state_change_start_time = _current_time;

			    connect(_proposed_state_change_timer, SIGNAL(timeout()), 
					    SLOT(HandleProposedStateTimeout()));
            }
            else
            {
                // Automatically accept aircraft in interruption mode
                set_control(CS_ACCEPTED_TASK_INCOMPLETE);                
            }
        }
		else if (cs == CS_PRE_HANDOFF)
		{
            ExcelLogging::ExcelLogger::instance()->LogHandoffFlashDetails(  callsign(), 
                                                                            _current_time,
                                                                            Timer::instance()->elapsed_time());

            if (!m_autoHandoff)
            {
			    m_preHandoffStateTimer = std::auto_ptr<QTimer>(new QTimer(this));
			    m_preHandoffStateTimer->start(PRE_HANDOFF_STATE_TIMEOUT_PERIOD, true);
			    m_preHandoffStartTime = _current_time;

			    connect(m_preHandoffStateTimer.get(), SIGNAL(timeout()),
					    SLOT(HandlePreHandoffTimeout()));
            }
            else
            {
                // Automatically handoff aircraft in interruption mode
                set_control(CS_HANDOFF);
            }
		}
        else if ((cs == CS_ACCEPTED_TASK_INCOMPLETE) || (cs == CS_ACCEPTED_TASK_COMPLETE))
        {
            ExcelLogging::ExcelLogger::instance()->LogAcceptanceTime(   callsign(), 
                                                                        _current_time,
                                                                        Timer::instance()->elapsed_time(),
                                                                        m_autoAccept);

            KeyValues kvs;
	        kvs.push_back(make_pair(string("aircraft_accepted"), callsign()));
            kvs.push_back(make_pair(string("auto_accept" ), to_string(m_autoAccept)));
            kvs.push_back(make_pair(string("acceptance_time"), to_string(_current_time)));
	        record_event(kvs);
        }
        else if (cs == CS_HANDOFF)
        {
            ExcelLogging::ExcelLogger::instance()->LogHandoffTime(  callsign(), 
                                                                    _current_time, 
                                                                    Timer::instance()->elapsed_time(),
                                                                    m_autoHandoff);

            KeyValues kvs;
	        kvs.push_back(make_pair(string("aircraft_handoff"), callsign()));
	        kvs.push_back(make_pair(string("auto_handoff" ), to_string(m_autoHandoff)));
            kvs.push_back(make_pair(string("handoff_time"), to_string(_current_time)));
	        record_event(kvs);
        }
	}
}

/*!
 * Vector aircraft to new point
 */
void AircraftAgent::vector( double x, double y ) {
	vector( atan2( y - _state->y, x - _state->x ) );
}

/*!
 * Vector aircraft on new heading
 */
void AircraftAgent::vector( double a ) {
	double tmp_a = _state->heading;

	_vectoring = true;
	_state->heading = a;

	KeyValues kvs;
	kvs.push_back( make_pair( string( "solution"    ), string( "vector" ) ) );
	kvs.push_back( make_pair( string( "new_heading" ), to_string( a     ) ) );
	kvs.push_back( make_pair( string( "old_heading" ), to_string( tmp_a ) ) );
	record_event( kvs );
}

/*
 *
 */
void AircraftAgent::reroute( const AircraftPathToken *t, Point p ) {
	double tmp_a = _state->heading;

	_vectoring = false;
	_path->reroute( t, p, position() );
	_state->heading = atan2( p.second - y(), p.first - x() );

	KeyValues kvs;
	kvs.push_back( make_pair( string( "solution"    ), string( "reroute" ) ) );
	kvs.push_back( make_pair( string( "new_heading" ), to_string( _state->heading ) ) );
	kvs.push_back( make_pair( string( "old_heading" ), to_string( tmp_a           ) ) );
	kvs.push_back( make_pair( string( "xcoord"      ), to_string( p.first         ) ) );
	kvs.push_back( make_pair( string( "ycoord"      ), to_string( p.second        ) ) );
	record_event( kvs );
}

/*!
 *
 */
void AircraftAgent::require_level( int z ) {
	// @todo: to simple ??

	KeyValues kvs;
	kvs.push_back( make_pair( string( "solution"     ), string( "requirement") ) );
	kvs.push_back( make_pair( string( "new_level"    ), to_string( z    ) ) );
	kvs.push_back( make_pair( string( "old_level"    ), to_string( _cfl ) ) );
	kvs.push_back( make_pair( string( "old_throttle" ), to_string( _state->throttle ) ) );
	record_event( kvs );

	set_cfl( z );
	_require_level = true;
	_tmp_throttle = _vel;
	_vel = 1.0;				// expediate
}

/*!
 *
 */
double AircraftAgent::speed() const {
	return _state->velocity;
}

int
AircraftAgent::HandoffKey() const
{
    int key = AC_HANDOFF_KEY;

    if (_data->m_handoffKey.size() == 1)
    {
        // Convert to upper case as the key presses captured by canvasview are in upper case
        std::string str = _data->m_handoffKey;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        key = str[0];
    }
    else
    {
        if (_data->m_handoffKey == "key_up")
        {
            key = (int) Qt::Key_Up;
        }
        else if (_data->m_handoffKey == "key_down")
        {
            key = (int) Qt::Key_Down;
        }
        else if (_data->m_handoffKey == "key_left")
        {
            key = (int) Qt::Key_Left;
        }
        else if (_data->m_handoffKey == "key_right")
        {
            key = (int) Qt::Key_Right;
        }
    }

    return key;
}

/*!
 *
 */
Point AircraftAgent::position( int ms ) {
	// current position
	if ( !ms ) 
		return Point( x(), y() );

	double s = ms * _state->velocity / (SIXTY_SQR * ONE_THOUSAND);

	// follow vector
	if ( _vectoring )
		return Point( x() + s * cos( a() ), y() + s * sin( a() ) );

	// position on path
	return _path->future_point( Point( x(), y() ), s );
}

/*!
 *
 */
const char * AircraftAgent::callsign() const { return _data->call; }
const char * AircraftAgent::type    () const { return _data->type; }
AcceptedStateFlashParams AircraftAgent::FlashParams(void) const
{
	return _data->accepted_state_flash_params;
}

UserInputState AircraftAgent::UserInputState() const
{
	return _data->user_input_state;
}

int
AircraftAgent::CoordinationRequestKey() const
{
	int asciiCode = static_cast<int>(_data->coordinationRequestKey);

	return asciiCode;
}

/*!
 *
 */
double AircraftAgent::calt() const { return _cfl; }

ControlState AircraftAgent::control() const { return _state->control; }
double       AircraftAgent::x()       const { return _state->x; }
double       AircraftAgent::y()       const { return _state->y; }
double       AircraftAgent::z()       const { return _state->z; }
double       AircraftAgent::a()       const { return _state->heading; }

/*!
 *
 */
double AircraftAgent::vel_min() const { 
	return _performance->cruise_speed_minimum( z() );
}
double AircraftAgent::vel_std() const { 
	return _performance->cruise_speed_standard( z() );
}
double AircraftAgent::vel_max() const { 
	return _performance->cruise_speed_maximum( z() );
}

/*!
 *
 */
double AircraftAgent::cfl_min() const { 
	return 0;
}
double AircraftAgent::cfl_std() const { 
	return _performance->cruising_altitude();
}
double AircraftAgent::cfl_max() const { 
	return _performance->ceiling();
}

//-----------------------------------------------------------------------------
// [public] slots
//


//-----------------------------------------------------------------------------
// [private]
//

/*!
 * 
 */
void AircraftAgent::activate() {
	_active = true;
	// @todo: 
}

/*!
 * 
 */
void AircraftAgent::deactivate() {
	_active = false;
	QTimer::singleShot( 0, this, SIGNAL( deactivated() ) );

	// @todo: 
}

/*!
 * 
 */
bool AircraftAgent::has_path() {
	return _path && _path->is_valid();
}

/*!
 * 
 */
void AircraftAgent::initialize_state() {
	if ( ! has_path() ) 
		throw aircraft_agent_error( "aircraft's flight path not defined" );
	if ( _state ) 
		throw aircraft_agent_error( "aircraft already initialized" );

	_active = (_data->start_time <= _current_time); 
	
	const AircraftPathToken *path = _path->data();

	bool is_level         = false;
	bool not_accelerating = false;

	//
	// set
	//
	long         tt = 0;
	ControlState cs = CS_NONE;

	//
	// set position
	//
	double x = path->x0;
	double y = path->y0;

	//
	// set heading
	//
	double  b = atan2( path->y1 - y ,  path->x1 - x );

	//
	// set altitude
	//
	double z = _isnan( path->z0 ) ? _performance->cruising_altitude() 
			 :                      path->z0
			 ;

	//
	// set cruising velocity
	//
	double vel = _isnan( path->v0 ) ? _performance->cruise_speed_standard( z )
		       :                      path->v0
		       ;

	//
	// update throttle setting
	//
	double t = _performance->throttle_setting( z, vel );

	//
	// set cleared flight level
	//
	if ( _isnan( path->z1 ) ) {
		_cfl = z;
		is_level = true;
	}
	else {
		_cfl = path->z1;
	}

	//
	// set cleared velocity
	//
	if ( _isnan( path->v1 ) ) {
		_vel = t;
		not_accelerating = true;
	}
	else {
		_vel = _performance->throttle_setting( z, path->v1 );
	}

	//
	// update airspeed
	//
	double v = ( is_level ) ?  vel
	         : ( _cfl > z ) ?  _performance->climb_speed( z, t )
	         :                 _performance->climb_speed( z, t )
	         ;

	//
	// update climb rate
	//
	double c = ( is_level ) ?  0.0
	         : ( _cfl > z ) ?  _performance->climb_rate( z, t )
	         :                -_performance->descent_rate( z, t )
	         ;

	//
	// update acceleration
	//
	double a = ( not_accelerating ) ? 0.0
	         :                        _performance->acceleration( z, t )
	         ;

	//
	// new state
	//
	update_state( new AircraftState( tt, cs, x, y, z, b, v, a, c, t ) );
}

/*!
 * 
 */
void AircraftAgent::advance_state( int ms ) {
	assert( _state );

	bool is_level         = false;
	bool not_accelerating = false;

	// current state (update time tag)
	long         tag = _state->tag + ms;
	ControlState  cs = _state->control;
	double         x = _state->x;
	double         y = _state->y;
	double         z = _state->z;
	double         b = _state->heading;
	double         v = _state->velocity;
	double         a = _state->acceleration;
	double         c = _state->climb_rate;
	double         t = _state->throttle;

	// current position on flight path
	const AircraftPathToken *path = _path->data();

	// update period in seconds
	double period = (double)ms / ONE_THOUSAND;   // period in seconds
	
	//
	// calc position
	//
	double deltaX = period * v * cos( b ) / SIXTY_SQR;	// knots -> nM this period
	double deltaY = period * v * sin( b ) / SIXTY_SQR;	//

	double tdist = hypot( path->x1 - x, path->y1 - y );
	double ddist = hypot( deltaX, deltaY );

	if ( ddist >= tdist ) {
		ddist -= tdist;
		_path->next();
		QTimer::singleShot( 0, this, SIGNAL( waypoint() ) );

		if ( _path->done() ) {
			deactivate();
			return;
		}
		else {
			path = _path->data();
			
			b = atan2( path->y1 - path->y0, path->x1 - path->x0 );

			x = path->x0 + ddist * cos( b );
			y = path->y0 + ddist * sin( b );

			if ( ! _isnan( path->z1 ) ) { _cfl = path->z1; }
			if ( ! _isnan( path->v1 ) ) { 
				_vel = _performance->throttle_setting( z, path->v1 ); 
			}
		}
	}
	else {
		x += deltaX;
		y += deltaY;
	}

	//
	// calc altitude
	//
	double deltaZ = period * c / SIXTY;		// feet/min -> feet this period

	if ( abs( _cfl - z ) <= abs( deltaZ ) ) { 
		z = _cfl;
		is_level = true;

		if ( _require_level ) {
			_require_level = false;
			_vel = _tmp_throttle;
		}

	}
	else { 
		z += deltaZ; 
	}

	//
	// calc cruising velocity
	// 
	double deltaV = period * a / SIXTY;		// Knt/min -> Knts this period
	double    vel = _performance->cruise_speed( z, t );
	double   tvel = _performance->cruise_speed( z, _vel );

	if ( abs( tvel - vel ) <= abs( deltaV ) ) { 
		vel = tvel;    
		not_accelerating = true;
		t = _vel;
	}
	else {	
		vel += deltaV; 
		t = _performance->throttle_setting( z, vel );
	}

	//
	// update airspeed
	//
	v = ( is_level ) ?  vel
	  : ( _cfl > z ) ?  _performance->climb_speed( z, t )
	  :                 _performance->climb_speed( z, t )
	  ;

	//
	// update climb rate
	//
	c = ( is_level ) ?  0.0
	  : ( _cfl > z ) ?  _performance->climb_rate( z, t )
	  :                -_performance->descent_rate( z, t )
	  ;

	//
	// update acceleration
	//
	a = ( not_accelerating ) ? 0.0
	  : ( _vel > t         ) ? _performance->acceleration( z, t )
	  :                        -_performance->deceleration( z, t )
	  ;

	//
	// new state
	//
	update_state( new AircraftState( tag, cs, x, y, z, b, v, a, c, t ) );
}

/*!
 *
 */
void AircraftAgent::update_state( AircraftState *state ) {
	_state = state;
		
	QTimer::singleShot( 0, this, SIGNAL( updated() ) );

	KeyValues kvs;
	kvs.push_back( make_pair( string( "type"    ), to_string( _data->type       ) ) );
	kvs.push_back( make_pair( string( "control" ), to_string( state->control    ) ) );
	kvs.push_back( make_pair( string( "xpos"    ), to_string( state->x          ) ) );
	kvs.push_back( make_pair( string( "ypos"    ), to_string( state->y          ) ) );
	kvs.push_back( make_pair( string( "alt"     ), to_string( state->z          ) ) );
	kvs.push_back( make_pair( string( "vel"     ), to_string( state->velocity   ) ) );
	kvs.push_back( make_pair( string( "head"    ), to_string( state->heading    ) ) );
	kvs.push_back( make_pair( string( "climb"   ), to_string( state->climb_rate ) ) );
	kvs.push_back( make_pair( string( "power"   ), to_string( state->throttle   ) ) );
	record_event( kvs );
}

bool AircraftAgent::ShouldConflict() const
{
	return !((_data->conflict_trial_name == std::string("")) && 
			(_data->conflict_acceptance_time == INVALID_VALUE) &&
			(_data->conflict_time == INVALID_VALUE));
}

/*!
 *
 */
void AircraftAgent::record_event( KeyValues &kvs ) {
	kvs.push_front( make_pair( string( "call" ), callsign() ) );
	emit event_recorded( kvs );
}

void AircraftAgent::HandleProposedStateTimeout()
{	
	if (_proposed_state_change_timer)
	{
		_proposed_state_change_timer->stop();
		_proposed_state_change_start_time = 0;
		delete _proposed_state_change_timer;

		if (_state->control != CS_ACCEPTED_TASK_COMPLETE)
		{
			_state->control = CS_ACCEPTED_TASK_COMPLETE;

			ExcelLogging::ExcelLogger::instance()->LogAcceptanceError(
				callsign(), 
				ExcelLogging::TASK_ERROR_INCORRECT);

			emit FailedAcceptanceTask(callsign());
		}

		emit ProposedStateTimeout();
	}
}

void
AircraftAgent::HandlePreHandoffTimeout()
{
	if (m_preHandoffStateTimer.get())
	{
		m_preHandoffStateTimer->stop();
		QTimer* timer = m_preHandoffStateTimer.release();
		delete timer;

		m_preHandoffStartTime = 0;
		
		_state->control = CS_HANDOFF;

		ExcelLogging::ExcelLogger::instance()->LogHandoffError(
				callsign(), 
				ExcelLogging::TASK_ERROR_INCORRECT);

		emit FailedHandoffTask(callsign());
	}
}

void AircraftAgent::HandlePauseRequest()
{
	if (_proposed_state_change_timer)
	{
		_proposed_state_change_timer->stop();
	}

	if (m_preHandoffStateTimer.get())
	{
		m_preHandoffStateTimer->stop();
	}
}

void AircraftAgent::HandleResumeRequest()
{
	if (_proposed_state_change_timer)
	{
		_proposed_state_change_timer->start(PROPOSED_STATE_TIMEOUT_PERIOD - 
			((_current_time - _proposed_state_change_start_time) * 1000));
	}

	if (m_preHandoffStateTimer.get())
	{
		m_preHandoffStateTimer->start(PRE_HANDOFF_STATE_TIMEOUT_PERIOD - 
			((_current_time - m_preHandoffStartTime) * 1000));
	}
}
////////////////////////////////////////////////////////////////////////////////

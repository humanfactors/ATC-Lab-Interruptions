/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: aircraft_agent.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.23.2.3 $ )
      @date     : $Date: 2014/12/10 13:26:17 $
      @state    : $State: Exp $

      $Log: aircraft_agent.h,v $
      Revision 1.23.2.3  2014/12/10 13:26:17  Aaron Yeung
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

      Revision 1.23.2.2  2014/12/02 13:48:59  Aaron Yeung
      Implemented scriptable handoff key for individual aircraft - <atc:handoffkey>
      Default handoff key is still the 'h' key

      Revision 1.23.2.1  2014/12/02 12:06:36  Aaron Yeung
      Added scriptable altitude change time restriction to aircraft <atc:altitudeChangeEnableTime>

      Revision 1.23  2010/04/11 02:31:47  Aaron Yeung
      Added reminder boxes that can be attached to aircraft. These reminder boxes move with the aircraft that they are attached to

      Revision 1.22  2010/04/08 13:19:30  Aaron Yeung
      Initial attempt at adding reminder box to individual aircraft

      Revision 1.21  2008/10/21 13:57:51  Aaron Yeung
      - Added functionality for experimenter to re-ask a question if the participant gives the wrong answer.
      - Added new xml element
      <questionBox>
              <atc:answer></atc:answer>
              <atc:incorrectResponse></atc:incorrectRespons>
      </questionBox>

      Revision 1.20  2008/07/12 19:51:26  Aaron Yeung
      Added scriptable coordination request key such that an experimenter can script what key a user needs to press for a coordination request aircraft
      Added pre-handoff state to aircraft so they flash blue when exiting the sector

      Revision 1.19  2008/06/30 11:06:23  Aaron Yeung
      Added aircraft handoff task

      Revision 1.18  2007/08/08 14:47:53  Aaron Yeung
      Exclude points scoring for resolving an aircraft conflict when its conflicting pair has already been resolved

      Revision 1.17  2007/08/08 14:03:31  Aaron Yeung
      Only allow score to be incremented once when the user correctly resolves a conflict

      Revision 1.16  2007/08/01 15:16:52  Aaron Yeung
      Added scoring for conflicts

      Revision 1.15  2007/07/26 10:22:05  Aaron Yeung
      Disallowed acceptance of aircraft that are not in the proposed state

      Revision 1.14  2007/07/22 07:34:27  Aaron Yeung
      Added dialog box when pairs of aircraft conflict

      Revision 1.13  2007/07/18 14:32:02  Aaron Yeung
      Added Coordination Request functionality for aircrafts

      Revision 1.12  2007/07/17 12:15:06  Aaron Yeung
      Enabled pausing of AircraftAgent proposed_state_change_timer when pausing the whole trial. Please note that there is an issue when pausing and a user has already accepted an aircraft but not completed the acceptance task. May need to revert back to the previous version checked in on the 16th July 07

      Revision 1.11  2007/07/16 10:54:19  Aaron Yeung
      Modified acceptance task so that a user can continue to try to press the correct key for an acceptance task until the time limit is exceeded. Previously, if the user pressed the wrong key, they would not be able to re-enter the correct key again

      Revision 1.10  2007/07/15 07:47:41  Aaron Yeung
      Added Utilities singleton

      Revision 1.9  2007/07/15 07:18:17  Aaron Yeung
      Included aircraft callsign in failed acceptance task dialog box

      Revision 1.8  2007/07/10 13:46:17  Aaron Yeung
      Added functionality to calculate aircraft conflicts

      Revision 1.7  2007/07/10 11:50:42  Aaron Yeung
      Removed CSOVEROUT and handoff time functionality

      Revision 1.6  2007/07/09 12:42:44  Aaron Yeung
      Fixed code so that aircrafts with start time of 0 will appear on screen at 0 seconds

      Revision 1.5  2007/07/09 08:34:00  Aaron Yeung
      Added timeout for when an aircraft changes to the CS_PROPOSED for the user to complete the aircraft acceptance task or an error dialog box will be displayed and the aircraft will automaticallly go into the CS_ACCEPTED_TASK_COMPLETE state

      Revision 1.4  2007/07/09 00:36:25  Aaron Yeung
      renamed aircraft accepted state blink items to use the word flash instead

      Revision 1.3  2007/07/08 17:05:10  Aaron Yeung
      Added method to return blink params stored in aircraft_agent

      Revision 1.2  2007/07/08 04:54:12  Aaron Yeung
      Added UpdateAircraft clock event handler to update the active attribute of each aircraft depending on whether they are set to enter the screen at a certain time

      Revision 1.1.1.2  2007/06/21 12:22:41  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.27  2007/02/26 08:03:45  seth
      automatic handoff

      Revision 1.26  2006/10/26 05:59:52  seth
      working implementation of experiment time logging

      Revision 1.25  2006/10/25 04:49:58  seth
      fixing logging

      Revision 1.24  2006/10/18 03:58:33  seth
      snap functionality for selection & fixed bug causing access violation once aircraft had reached destination

      Revision 1.23  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.22  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.21  2006/09/24 00:28:03  seth
      fixed vectoring and rerouting

      Revision 1.20  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.19  2006/09/20 06:44:01  seth
      vector selection box + some bug fix

      Revision 1.18  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.17  2006/09/17 12:45:35  seth
      implementing solution mechanisms

      Revision 1.16  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.15  2006/09/14 07:28:06  seth
      inital implementation of sector controller state

      Revision 1.14  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.13  2006/09/12 05:43:43  seth
      aircraft route tool working implementation

      Revision 1.12  2006/09/11 06:13:44  seth
      working on route tool

      Revision 1.11  2006/09/07 06:46:21  seth
      reimplementing tools - history and short route probe

      Revision 1.10  2006/09/06 06:22:23  seth
      transformation problems

      Revision 1.9  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.8  2006/08/29 08:44:56  seth
      reimplementing mvc pattern

      Revision 1.7  2006/08/24 04:12:51  seth
      seperating model from visuals

      Revision 1.6  2006/08/18 06:11:38  seth
      bug fixes

      Revision 1.5  2006/08/15 11:48:46  seth
      aircraft agents initialized with performance data

      Revision 1.4  2006/08/07 06:22:46  seth
      implemtation of performance data

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

#ifndef __ATC_AIRCRAFT_AGENT_HDR__
#define __ATC_AIRCRAFT_AGENT_HDR__


#include <qobject.h>
#include "atc_types.h"
#include "aircraftparam.h"
#include "aircraft_data.h"
#include <stdexcept>
#include <qtimer.h>

namespace performance {
	
	//
	// performance::Performance
	//
	class Performance;

};


namespace atc {

	//
	// class declaration
	//

	struct AircraftData;
	class  AircraftHistory;
	class  AircraftPath;
	struct AircraftPathToken;
	class  AircraftPerformance;
	class  AircraftPlan;
	struct AircraftState;
	enum   ControlState;


	//
	// atc::aircraft_agent_error
	//

	struct aircraft_agent_error : public std::runtime_error{
		aircraft_agent_error(const std::string& msg)
			:std::runtime_error(msg) {}
	};

	struct ConflictDetails
	{
		ConflictDetails()
			: trial(""), acceptance_time(INVALID_VALUE), conflict_time(INVALID_VALUE)
		{
		}

		std::string trial;
		int acceptance_time;
		int conflict_time;
	};

	//
	// atc::AircraftAgent
	//

	class AircraftAgent : public QObject {
		Q_OBJECT

	public:
		AircraftAgent( QObject * = 0, const char * = 0 );
		~AircraftAgent();

	public:
		void set_data( const AircraftData & );
		void set_path( const AircraftPath & );
		bool is_active() const { return _active; }
		bool is_vectoring() const { return _vectoring; }

		void set_conflict()   { _conflicted = true;  }
		void clear_conflict() { _conflicted = false; }
		bool InConflict() const { return _conflicted; }
		void SetConflictResolved(bool resolved) {_conflict_resolved = resolved;}

		void advance_position( int );
		void UpdateAircraft(long current_time);

		void vector( double, double );
		void vector( double );
		void reroute( const AircraftPathToken *, Point );
		void require_level( int );

		//void update_position();
		bool collides_with( AircraftAgent *, double, double );
		std::vector<std::string> CollidesWith();

		void set_cfl( int );
		void set_velocity( int );

		int accept();
		int handoff();

		void set_control( ControlState );
        void SetAutoAccept(bool a_autoAccept);
        void SetAutoHandoff(bool a_autoHandoff);
		
		Point position( int = 0 );

		const char * callsign() const;
		const char * type    () const;

		double       calt    () const;
		double       speed   () const;

		double x() const;
		double y() const;
		double z() const;
		double a() const;

		double vel_min() const;
		double vel_std() const;
		double vel_max() const;

		double cfl_min() const;
		double cfl_std() const;
		double cfl_max() const;

		AircraftData* GetAircraftData() const { return _data; }
		const AircraftPath    & path()    const { return *_path; }
		const AircraftHistory & history() const { return *_history; }
		AcceptedStateFlashParams FlashParams(void) const;
		UserInputState UserInputState() const;
		int CoordinationRequestKey() const;

		ControlState control() const;
		bool active() const { return _active; }
		long CurrentTime() const { return _current_time; }
        bool CanChangeAltitude() const { return m_enableAltitudeChange; }
        int HandoffKey() const;

	private:
		void activate();
		void deactivate();

		bool has_path();
		void initialize_state();
		void advance_state( int );
		void update_state( AircraftState * );
		bool ShouldConflict() const;
		void record_event( KeyValues & );

	signals:
		void updated();
		void waypoint();
		void deactivated();

		void event_recorded( KeyValues & );
		void ProposedStateTimeout();
		void FailedHandoffTask(const char*);
		void FailedAcceptanceTask(const char*);
		void Conflicted(std::pair<std::string, std::string>);
		void AircraftConflictResolved(std::string, std::string, int, int, int);
		void AircraftConflictFalseAlarm();

	public slots:
		void HandleProposedStateTimeout();
		void HandlePreHandoffTimeout();
		void HandlePauseRequest();
		void HandleResumeRequest();

	private:
		bool					_active;

		double					_cfl;		// cleared flight level
		double					_vel;		// target velocity
		QTimer*					_proposed_state_change_timer;
		std::auto_ptr<QTimer>	m_preHandoffStateTimer;

		AircraftData        *_data;
	    AircraftPath		*_path;
		AircraftState       *_state;
		AircraftHistory     *_history;

		const performance::Performance *_performance;

		//int    _overout_time;
		//int    _handoff_time;
		bool						_vectoring;
		bool						_conflicted;
		bool						_conflicted_last_update;
		bool						_require_level;
		double						_tmp_throttle;
		long						_current_time;
		bool						_conflict_resolved;
		unsigned int				_proposed_state_change_start_time;
		unsigned int				m_preHandoffStartTime;
		std::vector<std::string>	_conflicting_aircrafts;
		ConflictDetails				_conflict_details;
        bool                        m_enableAltitudeChange;
		int							m_acceptanceCount;
		int							m_handoffCount;
        bool                        m_autoAccept;
        bool                        m_autoHandoff;
	};

	//
	// 
	//
	typedef performance::Performance ACPerformance;

};


#endif
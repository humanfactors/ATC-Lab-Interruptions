/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: aircraft_data.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.11.2.2 $ )
      @date     : $Date: 2014/12/02 13:48:59 $
      @state    : $State: Exp $

      $Log: aircraft_data.h,v $
      Revision 1.11.2.2  2014/12/02 13:48:59  Aaron Yeung
      Implemented scriptable handoff key for individual aircraft - <atc:handoffkey>
      Default handoff key is still the 'h' key

      Revision 1.11.2.1  2014/12/02 12:06:36  Aaron Yeung
      Added scriptable altitude change time restriction to aircraft <atc:altitudeChangeEnableTime>

      Revision 1.11  2010/04/11 02:31:47  Aaron Yeung
      Added reminder boxes that can be attached to aircraft. These reminder boxes move with the aircraft that they are attached to

      Revision 1.10  2010/04/08 13:19:30  Aaron Yeung
      Initial attempt at adding reminder box to individual aircraft

      Revision 1.9  2008/07/12 19:50:31  Aaron Yeung
      Added scriptable coordination request key such that an experimenter can script what key a user needs to press for a coordination request aircraft

      Revision 1.8  2007/08/08 14:47:53  Aaron Yeung
      Exclude points scoring for resolving an aircraft conflict when its conflicting pair has already been resolved

      Revision 1.7  2007/08/01 15:15:52  Aaron Yeung
      Added scoring for conflicts

      Revision 1.6  2007/07/18 14:32:02  Aaron Yeung
      Added Coordination Request functionality for aircrafts

      Revision 1.5  2007/07/09 08:34:21  Aaron Yeung
      Added timeout for when an aircraft changes to the CS_PROPOSED for the user to complete the aircraft acceptance task or an error dialog box will be displayed and the aircraft will automaticallly go into the CS_ACCEPTED_TASK_COMPLETE state

      Revision 1.4  2007/07/09 00:36:25  Aaron Yeung
      renamed aircraft accepted state blink items to use the word flash instead

      Revision 1.3  2007/07/08 17:07:35  Aaron Yeung
      Added XML configurable accept state blink params

      Revision 1.2  2007/07/08 04:55:26  Aaron Yeung
      Added start time attribute to aircrafts to allow them to enter the screen at a time after the trial has started

      Revision 1.1.1.2  2007/06/21 12:22:41  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.17  2006/09/24 00:28:03  seth
      fixed vectoring and rerouting

      Revision 1.16  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.15  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.14  2006/09/18 06:41:19  seth
      updating state calcutlations

      Revision 1.13  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.12  2006/09/14 07:28:06  seth
      inital implementation of sector controller state

      Revision 1.11  2006/09/12 05:43:43  seth
      aircraft route tool working implementation

      Revision 1.10  2006/09/11 06:13:44  seth
      working on route tool

      Revision 1.9  2006/09/07 06:46:21  seth
      reimplementing tools - history and short route probe

      Revision 1.8  2006/08/28 06:21:48  seth
      refactoring mvc

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

#ifndef __ATC_AIRCRAFT_DATA_HDR__
#define __ATC_AIRCRAFT_DATA_HDR__

#define AIRCRAFT_PATH_TOKENS std::deque< AircraftPathToken* >
#define STATE_QUEUE          std::deque< const AircraftState* >


#include "atc.h"
#include "atc_types.h"
#include "aircraftparam.h"
#include <deque>


namespace atc {

	//
	// atc::AircraftData
	//

	struct AircraftData {

		AircraftData( ) : call( 0 ), type( 0 ) {}

		AircraftData( const AircraftData &rhs )
			  : call( rhs.call ), 
				type( rhs.type ), 
				start_time(rhs.start_time), 
				accepted_state_flash_params(rhs.accepted_state_flash_params),
				user_input_state(rhs.user_input_state),
				coordinationRequestKey(rhs.coordinationRequestKey),
				conflict_trial_name(rhs.conflict_trial_name),
				conflict_acceptance_time(rhs.conflict_acceptance_time),
				conflict_time(rhs.conflict_time),
				conflict_aircraft(rhs.conflict_aircraft),
				m_reminderBoxParams(rhs.m_reminderBoxParams.release()),
                m_altitudeChangeEnableTime(rhs.m_altitudeChangeEnableTime),
                m_handoffKey(rhs.m_handoffKey)    
		{
        }

		const char*					call;
		const char*					type;
		unsigned int				start_time;
		AcceptedStateFlashParams	accepted_state_flash_params;
		UserInputState				user_input_state;
		char						coordinationRequestKey;
		std::string					conflict_trial_name;
		int							conflict_acceptance_time;
		int							conflict_time;
		std::string					conflict_aircraft;
        std::string                 m_handoffKey;
		std::auto_ptr<ReminderBoxParams> m_reminderBoxParams;
        long                        m_altitudeChangeEnableTime;
	};


	//
	//
	// atc::AircraftPathToken
	//

	struct AircraftPathToken {

		AircraftPathToken(
				double X0, double Y0, double Z0, double V0
			  , double X1, double Y1, double Z1, double V1
		)
			  : x0( X0 ), y0( Y0 ), z0( Z0 ), v0( V0 )
			  , x1( X1 ), y1( Y1 ), z1( Z1 ), v1( V1 )
		{}

		AircraftPathToken( const AircraftPathToken &rhs )
			  : x0( rhs.x0 ), y0( rhs.y0 ), z0( rhs.z0 ), v0( rhs.v0 )
			  , x1( rhs.x1 ), y1( rhs.y1 ), z1( rhs.z1 ), v1( rhs.v1 )
		{}

		~AircraftPathToken() {}

		double x0, y0, z0, v0;
		double x1, y1, z1, v1;
	};


	//
	// atc::AircraftPath
	//

	class AircraftPath {

	public:
		typedef AIRCRAFT_PATH_TOKENS::const_iterator const_iterator;

	public:
		AircraftPath();
		AircraftPath( const AircraftPath & );
		~AircraftPath();

	public:
		void push( const AircraftPathToken & );
		bool is_valid() const;
		void next();
		bool done();

		const AircraftPathToken* data() const;

		AIRCRAFT_PATH_TOKENS::const_iterator begin() const;
		AIRCRAFT_PATH_TOKENS::const_iterator   end() const;

		Point future_point( const Point &, double ) const;
		void reroute( const AircraftPathToken *, const Point &, const Point & );

	private:
		Point future_point( const_iterator, const Point &, double s ) const;

	private:
		AIRCRAFT_PATH_TOKENS _plan;		// future
		//AIRCRAFT_PATH_TOKENS _track;	// past

	};

	//
	// atc::AircraftPath typedefs
	//
	typedef AIRCRAFT_PATH_TOKENS AircraftPathTokens;


	//
	// atc::ControlState
	//
	enum ControlState {
			CS_NONE							// black            >15mins from entry
		  , CS_ANNOUNCED					// blue             > 2mins from entry
		  , CS_PROPOSED						// blinking_orange	> 0mins from entry
		  , CS_ACCEPTED_TASK_INCOMPLETE		// green			in sector/accepted but user has not completed acceptance task
		  , CS_ACCEPTED_TASK_COMPLETE		// green			in sector/accepted and user has completed acceptance task
		  , CS_OVEROUT						// orange			< 2mins from exit
		  , CS_PRE_HANDOFF					// blue				just exited sector
		  , CS_HANDOFF						// white			for 30secs after hand off
		  , CS_NOMORE						// black
	};


	//
	// atc::AircraftState
	//

	/*!
	 *  UNITS:
	 *
	 *   x,  y : nautical miles
	 *       z : feet
	 *  dx, dy : knots (nautical miles per hour
	 *      dz : feet per minute
	 * 
	 *  ddx, ddy, ddz : ?????
	 *
	 *    cvel : knots
	 *    calt : feet
	 */

	struct AircraftState {
		AircraftState( 
				long tt, ControlState cs
			  ,	double X, double Y, double Z
			  , double B, double V
			  , double A, double C, double T
		)
			  : tag( tt )
			  , control( cs )
			  , x( X ), y( Y ), z( Z )
			  , heading( B )
			  , velocity( V )
			  , acceleration( A )
			  , climb_rate( C )
			  , throttle( T )
		{}

		~AircraftState() {}

		long   tag;

		ControlState control;

		double x, y, z;
		double heading;
		double velocity;		// ground speed
		double acceleration;
		double climb_rate;
		double throttle;			// -1.0 < t < +1.0 throttle setting
	};


	//
	// atc::AircraftHistory
	//

	class AircraftHistory {

	public:
		AircraftHistory();
		~AircraftHistory();

	public:
		void add_event( const AircraftState * );

		typedef STATE_QUEUE::const_iterator const_iterator;

		AircraftHistory::const_iterator begin() const { 
			return _history.begin(); 
		}
		AircraftHistory::const_iterator end() const {
			return _history.end();
		}
		int size() const { return _history.size(); }

	private:
		STATE_QUEUE _history;
	};


};

#undef AIRCRAFT_PATH_TOKENS
#undef STATE_QUEUE

#endif

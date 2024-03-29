/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: clock.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2 $ )
      @date     : $Date: 2007/07/19 15:18:14 $
      @state    : $State: Exp $

      $Log: clock.cpp,v $
      Revision 1.2  2007/07/19 15:18:14  Aaron Yeung
      Added XML scriptable Clock that can control the length of time XML scripted widgets can appear onscreen

      Revision 1.1.1.2  2007/06/21 12:22:43  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.12  2007/02/28 00:48:12  seth
      implemented tool identifier

      Revision 1.11  2007/02/27 06:07:17  seth
      implemented clock

      Revision 1.10  2007/02/26 08:03:45  seth
      automatic handoff

      Revision 1.9  2006/11/07 03:05:44  seth
      added additional logging

      Revision 1.8  2006/11/01 04:53:36  seth
      created installer

      Revision 1.7  2006/10/20 04:49:23  seth
      reduced dimensions of workload popup question && initial implementation of experiment time (for inclusion in log file output)

      Revision 1.6  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.5  2006/09/11 06:13:44  seth
      working on route tool

      Revision 1.4  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.3  2006/08/27 13:01:19  seth
      progress ....*.....

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "clock.h"
#include "atc.h"

#include <qtimer.h>


using namespace atc;
using std::make_pair;
using std::string;


////////////////////////////////////////////////////////////////////////////////
//
// atc::Clock
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
Clock::Clock()
	  : _interval( 0 )
	  , _multiple( 1 )
	  , _timer   ( new QTimer( this ) )
	  , _time    ()
	  , _elapsed ( 0 )
	  , _seconds ( 0 )
	  , _stops   ( 1 )
{
	connect( _timer, SIGNAL( timeout() ), this, SLOT( tick_dispatch() ) );
}

/*!
 *
 */
Clock::~Clock() {}


//------------------------------------------------------------------------------
// [public] setters
//

/*!
 *
 */
void Clock::set_interval( int ms ) { _interval = ms; }

/*!
 *
 */
void Clock::set_multiple( int f ) { _multiple = f; }


//------------------------------------------------------------------------------
// [public] setters
//

/*!
 *
 */

long Clock::elapsed_time() {
	if ( is_running() ) _elapsed += _time.restart();
	return _elapsed;
}


//------------------------------------------------------------------------------
// [public] slots
//

/*!
 *
 */
void Clock::start() {
	if ( !is_running() ) {
		_stops = 0;

		_time.start();
		_timer->start( _interval );
		emit seconds_update( _seconds );
		emit started();

		KeyValues kvs;
		kvs.push_back( make_pair( string( "clock"    ), to_string( "start_request" ) ) );
		kvs.push_back( make_pair( string( "interval" ), to_string( _interval       ) ) );
		emit event_recorded( kvs );
	}
}

/*!
 *
 */
void Clock::stop() {
	if ( is_running() ) {
		_elapsed += _time.elapsed();
		emit seconds_update( _seconds );
		emit stopped();

		KeyValues kvs;
		kvs.push_back( make_pair( string( "clock"    ), to_string( "stop_request" ) ) );
		emit event_recorded( kvs );
	}
	++_stops;
}


//------------------------------------------------------------------------------
// [private]
//

/*!
 *
 */
bool Clock::is_running() { return 0 == _stops; }

//------------------------------------------------------------------------------
// [private] slots
//

/*!
 *
 */
void Clock::tick_dispatch() {
	if ( is_running() ) {
		_seconds += ( _multiple * _interval / ONE_THOUSAND );
		emit seconds_update( _seconds );
		emit tick( _multiple * _interval );
		emit tick();

		KeyValues kvs;
		kvs.push_back( make_pair( string( "clock" ), to_string( "tick" ) ) );
		emit event_recorded( kvs );
	}
	else {
		_timer->stop();

		KeyValues kvs;
		kvs.push_back( make_pair( string( "clock" ), to_string( "stop" ) ) );
		emit event_recorded( kvs );
	}
}

////////////////////////////////////////////////////////////////////////////////

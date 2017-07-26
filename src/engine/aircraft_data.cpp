/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: aircraft_data.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:41 $
      @state    : $State: Exp $

      $Log: aircraft_data.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:41  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.10  2006/09/24 00:28:03  seth
      fixed vectoring and rerouting

      Revision 1.9  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.8  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.7  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.6  2006/09/07 06:46:21  seth
      reimplementing tools - history and short route probe

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

#include "aircraft_data.h"
#include "atc.h"
#include <algorithm>

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
// atc::AircraftPath
//
//------------------------------------------------------------------------------
// construction/destruction
//

/*!
 *
 */
AircraftPath::AircraftPath() 
	  : _plan()
{}

/*!
 *
 *
 */
AircraftPath::AircraftPath( const AircraftPath &rhs ) 
	  : _plan()
{
	AircraftPath::const_iterator it = rhs.begin();
	for ( ; it != rhs.end(); ++ it )
		push( **it );
}

/*!
 *
 */
AircraftPath::~AircraftPath() {
	AircraftPath::const_iterator it = begin();
	for( ; it != end(); ++it )
		delete *it;
}


//------------------------------------------------------------------------------
// [public] api
//

/*!
 *
 */
void AircraftPath::push( const AircraftPathToken &t ) { 
	_plan.push_back( new AircraftPathToken( t ) ); 
}

/*!
 *
 */
bool AircraftPath::is_valid() const { 
	return !_plan.empty(); 
}


/*!
 *
 */
void AircraftPath::next() {
	//_track.push_back( _plan.back() );
	delete _plan.front();
	_plan.pop_front();
}

/*!
 *
 */
bool AircraftPath::done() { 
	return _plan.empty(); 
}

/*!
 *
 */
const AircraftPathToken* AircraftPath::data() const { 
	return _plan.front(); 
}

/*!
 *
 */
AircraftPath::const_iterator AircraftPath::begin() const { 
	return _plan.begin(); 
}

/*!
 *
 */
AircraftPath::const_iterator AircraftPath::end() const {
	return _plan.end();
}

/*!
 *
 */
Point AircraftPath::future_point( const Point &pt, double s ) const {
	return future_point( begin(), pt, s );
}


/*!
 *
 */
void AircraftPath::reroute( 
		const AircraftPathToken *t, const Point &p1, const Point &p0 
) {
	
	while ( !done() && t != data() )
		next();	
	
	AircraftPathToken *ct = _plan.front();
	ct->x0 = p1.first;
	ct->y0 = p1.second;

	_plan.push_front( new AircraftPathToken (
			p0.first, p0.second, NAN, NAN
		  , p1.first, p1.second, NAN, NAN
	) );
}


//------------------------------------------------------------------------------
// [private] implemetation
//

/*!
 *
 */
Point AircraftPath::future_point( 
		const_iterator ps, const Point &p0, double s 
) const {
	Point p1( (*ps)->x1, (*ps)->y1 );
	
	double dx = p1.first - p0.first;
	double dy = p1.second - p0.second;
	double length = _hypot( dx, dy );

	if ( s > length ) {
		++ps;
		if   ( ps == end() )  return p1;
		else                  return future_point( ps, p1, s - length );
	}
	else {
		double a = atan2( dy, dx );
		return Point( p0.first + s * cos( a ), p0.second + s * sin( a ) );
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// atc::AircraftHistory
//
//------------------------------------------------------------------------------
// 
//

/*!
 *
 */
AircraftHistory::AircraftHistory() : _history() {}

/*!
 *
 */
AircraftHistory::~AircraftHistory() {
	std::deque< const AircraftState * >::iterator it = _history.begin();
	for ( ; it != _history.end(); ++it ) {
		delete *it;
	}
}


//------------------------------------------------------------------------------
// 
//

/*!
 * Add Aircraft State to the History Log.
 *
 * State information is added to the top of the history log. Once the log 
 * reaches a certain size it is maintained at that size with each additional
 * history entry causing the earliest entry to be dropped off the bottom.
 */
void AircraftHistory::add_event( const AircraftState *state ) {
	_history.push_front( state );
//	if ( _history.size() > HISTORY_DOT_COUNT ) {
//		_history.pop_back(); // @todo: does this leak?
//	}
}


////////////////////////////////////////////////////////////////////////////////

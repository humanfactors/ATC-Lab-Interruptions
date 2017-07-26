/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: actools.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2 $ )
      @date     : $Date: 2007/07/08 06:17:40 $
      @state    : $State: Exp $

      $Log: actools.cpp,v $
      Revision 1.2  2007/07/08 06:17:40  Aaron Yeung
      Removed probe dots
      Made probe show future position in seconds

      Revision 1.1.1.2  2007/06/21 12:22:41  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.16  2007/02/07 02:51:56  seth
      1) fixed flashing aircraft to entering not exiting sector; 2) inhibited workload pop-up question; 3) implemented minute markers along short route probe

      Revision 1.15  2006/11/07 03:05:44  seth
      added additional logging

      Revision 1.14  2006/10/18 03:58:33  seth
      snap functionality for selection & fixed bug causing access violation once aircraft had reached destination

      Revision 1.13  2006/10/17 12:23:30  seth
      Altered selection priority (z) of ac-locator over actual sprite. Fixed requirement dialog to include distance

      Revision 1.12  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.11  2006/09/24 00:28:03  seth
      fixed vectoring and rerouting

      Revision 1.10  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.9  2006/09/20 06:44:01  seth
      vector selection box + some bug fix

      Revision 1.8  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.7  2006/09/14 12:24:34  seth
      reconciled aircraft tool class files

      Revision 1.6  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.5  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.4  2006/09/12 05:43:43  seth
      aircraft route tool working implementation

      Revision 1.3  2006/09/11 06:13:44  seth
      working on route tool

      Revision 1.2  2006/09/07 06:46:21  seth
      reimplementing tools - history and short route probe

      Revision 1.1  2006/09/06 13:09:24  seth
      refactoring translations into canvas

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "actools.h"
#include "atc.h"
#include "atc_types.h"
#include "aircraft_agent.h"
#include "aircraft_data.h"
#include "aircraftsprite.h"
#include "canvas.h"

#include <qcanvas.h>


using namespace atc;


////////////////////////////////////////////////////////////////////////////////
//
// atc::ACProbeTool
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
ACProbeTool::ACProbeTool( Canvas *c, AircraftSprite *s )
	  : QCanvasLine( c ), _sprite( s ), _canvas( c ), _idx( 0 ), _secs(0)
{
	QPen p = pen();
	p.setWidth( 3 );
	setPen( p );

	// create short route probe (minute) points
	/*int dots_size = SHORT_ROUTE_PROBE_SECS[ SHORT_ROUTE_PROBE_SIZE - 1 ];

	for ( int i = 0; i < dots_size; ++i ) {
		QCanvasEllipse *dot = new QCanvasEllipse( 
				HISTORY_DOT_DIAMETER, HISTORY_DOT_DIAMETER, c 
		); 
		//dot->setVisible( false );
		dot->setBrush( HISTORY_DOT_BRUSH );
		dot->setZ( AIRCRAFT_SPRITE_LAYER + 1 );
		dot->show();

		_dots.push_back( dot );
	}*/
}

/*!
 *
 */
ACProbeTool::~ACProbeTool() {}


//------------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
int ACProbeTool::value() const { return _secs; }

/*!
 *
 */
void ACProbeTool::deactivate() {
	_idx = 0;
	update();
}


/*!
 *
 */
void ACProbeTool::toggle() {
	++_idx %= SHORT_ROUTE_PROBE_SIZE;
	update();
}

/*!
 *
 */
void ACProbeTool::update() {
	// lookup length of probe (in minutes)
	_secs	= SHORT_ROUTE_PROBE_SECS[_idx];

	// canvas positions
	double x = _sprite->cx();
	double y = _sprite->cy();

	// calc x/y velocity components (minute)
	double angle = _sprite->a();
	double speed = _sprite->v();
	double dx    =  cos( angle );
	double dy    = -sin( angle );	// ??? but it works!!!
	double vx    =  _canvas->to_internal_length( dx * speed / SIXTY );
	double vy    =  _canvas->to_internal_length( dy * speed / SIXTY );
	double vx_secs = _canvas->to_internal_length(dx * speed / (SIXTY * SIXTY));
	double vy_secs = _canvas->to_internal_length(dy * speed)/ (SIXTY * SIXTY);

	// calculate positions for active second points
	/*for ( int i = 0; i < _secs ; ++i ) {
		x += vx_secs;
		y += vy_secs;
		_dots[ i ]->move( x, y ); 
	}*/

	// move inactive dots off screen
	/*for ( int j = _secs; j < _dots.size(); ++j ) {
		_dots[ j ]->move( 0, 0 );
	}*/

	// calc probe start/end points [ relative to (0,0) ]
	int radius = AIRCRAFT_SPRITE_DIAMETER / 2;
	double length = 
			_secs ? _canvas->to_internal_length( _secs * speed / (SIXTY * SIXTY))
		  :         radius
		  ;
	setPoints( radius * dx, radius * dy, length * dx, length * dy );

	// update
	_canvas->update();
}


//------------------------------------------------------------------------------
// [private] implementation
//


////////////////////////////////////////////////////////////////////////////////
//
// atc::ACHistoryTool
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
ACHistoryTool::ACHistoryTool( Canvas *c, AircraftSprite *s )
	  : _sprite( s ), _canvas( c )
{
	for ( int i = 0; i < HISTORY_DOT_COUNT; ++i ) {
		_dots.push_back( 
				new QCanvasEllipse( 
						HISTORY_DOT_DIAMETER
					  , HISTORY_DOT_DIAMETER
					  , c 
				) 
		);
	}
	set_visible( false );
	set_colour( HISTORY_DOT_BRUSH );
}

/*!
 *
 */
ACHistoryTool::~ACHistoryTool() {}


//------------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
void ACHistoryTool::set_colour( const QColor &colour ) {
	HistoryDots::iterator dot = _dots.begin();
	for( ; dot != _dots.end(); ++dot ) {
		(*dot)->setBrush( colour );
	}
	_canvas->update();
}

/*!
 *
 */
void ACHistoryTool::set_visible( bool state ) {
	if ( state == is_visible() ) return;

	HistoryDots::iterator dot = _dots.begin();
	for( ; dot != _dots.end(); ++dot ) {
		(*dot)->setVisible( state );
	}
	update();
}

/*!
 *
 */
bool ACHistoryTool::is_visible() const { return _dots[0]->isVisible(); }

/*!
 *
 */
void ACHistoryTool::toggle() { set_visible( ! is_visible() ); }

/*!
 *
 */
void ACHistoryTool::update() {
	const AircraftHistory &h = _sprite->history();

	if ( ! is_visible() || h.size() < 2 ) return;

	AircraftHistory::const_iterator hpt = h.begin();
	HistoryDots::iterator           dot = _dots.begin();

	(*dot)->move( _sprite->cx(), _sprite->cy() );
	++dot;

	long tt = (*hpt)->tag;

	while ( true ) {
		if ( dot == _dots.end() ) {	break; }

		if ( hpt == h.end() ) {
			// @todo blank out remaining dots
			for ( ; dot != _dots.end(); ++dot ) {
				(*dot)->move( 0, 0 );
			}
			break;
		}

		if ( ( tt - (*hpt)->tag ) >= HISTORY_DOT_PERIOD ) {
			tt = (*hpt)->tag;
			Point p( _canvas->to_internal_point( (*hpt)->x, (*hpt)->y ) );
			(*dot)->move( p.first, p.second );
			++dot;
		}

		++hpt;
	}
	_canvas->update();
}


//------------------------------------------------------------------------------
// [private] implementation
//


////////////////////////////////////////////////////////////////////////////////
//
// atc::ACRouteTool
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
ACRouteTool::ACRouteTool( Canvas *c, AircraftSprite *ac ) 
	  : _canvas( c ) 
	  , _aircraft( ac )
	  , _route()
	  , _track( new QCanvasLine( c ) )
	  , _shown(false)
	  , _paint()
{
	build();

	_track->setZ( AIRCRAFT_TOOL_LAYER );
	_track->setPoints( 0, 0, 0, 0 );
	_track->setPen( QPen( ROUTE_TOOL_COLOUR, 1, Qt::DotLine ) );
	_track->show();
}

/*!
 *
 */
ACRouteTool::~ACRouteTool() { 
	clear();
}


//------------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
bool ACRouteTool::is_active() const { return _shown; }

/*!
 * Set the active/visible state
 */
void ACRouteTool::set_active( bool active ) {
	if ( _shown != active ) {
		_shown = active;
		RouteSections::iterator sec = _route.begin();
		for ( ; sec < _route.end(); ++sec ) {
			set_visible( *sec, active );
		}
	}
}

/*!
 * Toggle the 'shown' state of route information
 */
void ACRouteTool::toggle() { set_active( !_shown ); }

/*!
 * Update the start point for route information
 */
void ACRouteTool::update() {

	if ( 0 == _route.size() ) return;

	RouteSection *s = _route.front();

	if ( s->path->_data != _aircraft->agent()->path().data() ) {
		_route.pop_front();
		delete s;
		update();
	}
	else {
		QPoint  ac( _aircraft->cx(), _aircraft->cy() );

		if ( _aircraft->agent()->is_vectoring() ) {
			QPoint p0( s->path->startPoint() );
			_track->setPoints( p0.x(), p0.y(), ac.x(), ac.y() );
		}
		else {
			QPoint p1( s->path->endPoint() );
			s->path->setPoints( ac.x(), ac.y(), p1.x(), p1.y() );
			set_time();
			_track->setPoints( 0, 0, 0, 0 );
		}
	}
}
	
/*!
 *
 */
void ACRouteTool::reroute( const AircraftPathToken *t, const QPoint &p ) {
	_aircraft->reroute( t, _canvas->to_user_point( p.x(), p.y() ) );
	clear();
	build();
	set_time();
	set_active( true );
}


//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 *
 */
void ACRouteTool::clear() {
	_shown = false;
	RouteSections::const_iterator s = _route.begin();
	for ( ; s < _route.end(); ++s )
		(*s)->destroy();
	_route.clear();
}

/*!
 *
 */
void ACRouteTool::build() {
	const AircraftPath &path = _aircraft->path();
	AircraftPath::const_iterator sec = path.begin();

	for( ; sec != path.end(); ++sec ) { 
		_route.push_back( create_section( *sec ) );
	}
}

/*!
 * Set the time interval for reaching waypoints
 */
void ACRouteTool::set_time() {
	double time = 0;

	RouteSections::iterator sec = _route.begin();
	for ( ; sec < _route.end(); ++sec ) {
		QPoint p0( (*sec)->path->startPoint() );
		QPoint p1( (*sec)->path->endPoint() );
		
		double vel = _canvas->to_internal_length( _aircraft->v() ) / SIXTY_SQR;

		double dx = p0.x() - p1.x();
		double dy = p0.y() - p1.y();
		double hy = _hypot( dx, dy );

		time += _hypot( p0.x() - p1.x(), p0.y() - p1.y() ) / vel;
		
		QString s;
	    s.sprintf( "%02d:%02d", int(time)/60, int(time)%60 );
		(*sec)->time->setText( s );
	}
}

/*!
 * Set the visibility of the canvas items making up particular route section
 */
void ACRouteTool::set_visible( RouteSection *s, bool v ) {
	s->path->setVisible( v );
	s->time->setVisible( v );
	s->back->setVisible( v );
}

/*!
 *
 */
ACRouteTool::RouteSection* ACRouteTool::create_section( 
		const AircraftPathToken *t
) {
	Point p0 = _canvas->to_internal_point( t->x0, t->y0 );
	Point p1 = _canvas->to_internal_point( t->x1, t->y1 );

	ACTRouteLine* fp = new ACTRouteLine( this, t );
	fp->setZ( AIRCRAFT_SPRITE_LAYER - 1.0 );
	fp->setPoints( p0.first, p0.second, p1.first, p1.second );
	fp->setPen( QPen( ROUTE_TOOL_COLOUR, 1 ) );

	QCanvasText* ft 
		  = new QCanvasText( "00:00", AIRCRAFT_SPRITE_FONT, _canvas );
	ft->setZ( AIRCRAFT_SPRITE_LAYER );
	ft->move( p1.first, p1.second );
	ft->setColor( ROUTE_TOOL_COLOUR );

	QCanvasRectangle* tb 
		  = new QCanvasRectangle( ft->boundingRect(), _canvas );
	tb->setZ( AIRCRAFT_SPRITE_LAYER - 1.0 );
	tb->setBrush( ROUTE_TOOL_BRUSH );
	tb->setPen( ROUTE_TOOL_COLOUR );

	return new RouteSection( fp, ft, tb );
}

////////////////////////////////////////////////////////////////////////////////
//
// atc::ACTRouteLine::RouteSection
//
//------------------------------------------------------------------------------
//
//

/*!
 *
 */
ACRouteTool::RouteSection::RouteSection( 
		ACTRouteLine* l, QCanvasText* t, QCanvasRectangle* r 
)
	  : path(l), time(t), back(r) 
{}

/*!
 *	Prevent copying
 *
ACRouteTool::RouteSection::RouteSection( const RouteSection &rhs )
	  : path(rhs.path), time(rhs.time), back(rhs.back) {}
 */

/*!
 *
 */
ACRouteTool::RouteSection::~RouteSection() {
	try {
		delete path;
		delete time;
		delete back;
	}
	catch( ...) {}
}

/*!
 *
 */
void ACRouteTool::RouteSection::destroy() const { delete this; }


////////////////////////////////////////////////////////////////////////////////
//
// atc::ACTRouteLine
//
//------------------------------------------------------------------------------
//
//

/*!
 *
 */
ACTRouteLine::ACTRouteLine( ACRouteTool *p, const AircraftPathToken *t ) 
	  : QCanvasLine( p->_canvas ), _parent( p ), _data( t ) 
{}

/*!
 *
 */
AircraftSprite* ACTRouteLine::aircraft() const { 
	return _parent->_aircraft; 
}

/*!
 *
 */
void ACTRouteLine::reroute( const QPoint &p ) {	
	_parent->reroute( _data, p );
}


////////////////////////////////////////////////////////////////////////////////

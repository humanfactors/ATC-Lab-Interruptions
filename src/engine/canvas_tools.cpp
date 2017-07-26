/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: canvas_tools.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2 $ )
      @date     : $Date: 2009/05/04 09:23:59 $
      @state    : $State: Exp $

      $Log: canvas_tools.cpp,v $
      Revision 1.2  2009/05/04 09:23:59  Aaron Yeung
      Added DivisionLine which allows a user to draw lines using XML. The user is able to specify a line's ID, start point (x1, y1), end point (x2, y2), colour, width, start time and end time.

      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.8  2006/10/09 04:59:46  seth
      bug fixes

      Revision 1.7  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.6  2006/09/20 06:44:01  seth
      vector selection box + some bug fix

      Revision 1.5  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.4  2006/09/13 05:38:49  seth
      full working implemtation of Range/Bearing tool

      Revision 1.3  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.2  2006/09/06 06:22:23  seth
      transformation problems

      Revision 1.1  2006/06/15 03:58:57  seth
      refactoring aircraft agents


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "canvas_tools.h"

#include "aircraft_agent.h"
#include "aircraft_data.h"
#include "aircraftsprite.h"
#include "atc.h"
#include "canvas.h"
#include "waypointsprite.h"

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
// atc::BearingRangeLine
//
//------------------------------------------------------------------------------
// Construction/Destruction
//

/*!
 *
 */
BearingRangeLine::BearingRangeLine( Canvas *canvas )
	:QObject( canvas )
	,_canvas( canvas )
	,_anchor( 0 )
	,_hook( 0 )
	,_line( 0 )
	,_X1( 0 )
	,_X2( 0 )
	,_range( 0 )
	,_bearing( 0 )
	,_time( 0 )
	,_box( 0 )
{
	int lsize = QFontMetrics( AIRCRAFT_SPRITE_FONT ).lineSpacing();

	_line = new BRLLineSprite( canvas );
	_line->setPen( BRL_TOOL_PAINT );
	_line->show();
	
	_range = new QCanvasText( "0000", AIRCRAFT_SPRITE_FONT, canvas );
	_range->setColor( BRL_TOOL_PAINT );
	_range->moveBy( 0, -3 * lsize );
	_range->setZ( _line->z() + 0.2 );
	_range->show();
	
	_bearing = new QCanvasText( "000", AIRCRAFT_SPRITE_FONT, canvas );
	_bearing->setColor( BRL_TOOL_PAINT );
	_bearing->moveBy( 0, -2 * lsize );
	_bearing->setZ( _line->z() + 0.2 );
	_bearing->show();
	
	_time = new QCanvasText( "00:00", AIRCRAFT_SPRITE_FONT, canvas );
	_time->setColor( BRL_TOOL_PAINT );
	_time->moveBy( 0, -lsize );
	_time->setZ( _line->z() + 0.2 );
	_time->show();

	QRegion box( _range->boundingRect() );
	//box += QRegion( _bearing->boundingRect() );
	box += QRegion( _time->boundingRect() );

	int xdim = box.boundingRect().width() / 5;

	_X1 = new BRLLineSprite( canvas );
	_X1->setPen( QPen( BRL_TOOL_PAINT, 3 ) );
	_X1->setZ( _line->z() + 0.15 );
	_X1->setPoints( 0, xdim, 0, -xdim );
	_X1->show();

	_X2 = new BRLLineSprite( canvas );
	_X2->setPen( QPen( BRL_TOOL_PAINT, 3 ) );
	_X2->setZ( _line->z() + 0.15 );
	_X2->setPoints( xdim, 0, -xdim, 0 );
	_X2->show();

	_box = new BRLDataSprite( this, box.boundingRect(), canvas );
	_box->setPen( BRL_TOOL_PAINT );
	_box->setBrush( Qt::NoBrush );
	_box->setZ( _line->z() + 0.1 );
	_box->show();
}

/*!
 * Destructor cleans up QCanvasItems.
 *
 * Although QCanvas would eventually clear up these items, they are not 
 * reused so would like to free up resources as early as possible. This 
 * leads to ownership issues when QCanvas cleans up first (result 
 * dangling pointers).
 */
BearingRangeLine::~BearingRangeLine() {
	try {
		if( _box    ) delete _box;
		if( _time   ) delete _time;
		if( _bearing) delete _bearing;
		if( _range  ) delete _range;
		if( _X2     ) delete _X2;
		if( _X1     ) delete _X1;
		if( _line   ) delete _line;
	}
	catch ( ... ) {
		// do nothing
	}
}

//------------------------------------------------------------------------------
// [public] interface
//

/*!
 * Attach start position to an aircraft (moving).
 */
void BearingRangeLine::anchor( AircraftSprite *s ) {
	_anchor = s;
	initialize( s->cx(), s->cy() );
}

/*!
 * Set the start position to waypoint location.
 */
void BearingRangeLine::anchor( WayPointSprite *wp ) {
	initialize( wp->x(), wp->y() );
}

/*!
 * Set the start position to arbitary point.
 */
void BearingRangeLine::anchor(const QPoint &pt ) {
	initialize( pt.x(), pt.y() );
}

/*!
 * Attach the end position to an aircraft (moving).
 */
void BearingRangeLine::hook( AircraftSprite *s ) {
	if ( s != _anchor )
		_hook = s;
	update_hook( s->cx(), s->cy() );
}

/*!
 * Set the end position to waypoint location.
 */
void BearingRangeLine::hook( WayPointSprite *wp ) {
	update_hook( wp->x(), wp->y() );
}

/*!
 * Set the end position to arbitary point.
 */
void BearingRangeLine::hook(const QPoint &pt ) {
	update_hook( pt.x(), pt.y() );
}

/*!
 *
 */
void BearingRangeLine::unhook() {
	if ( _hook ) {
		disconnect(
			_hook, SIGNAL( updated_position(double, double ) ),
			this, SLOT( update_hook( double, double ) )
		);
		_hook = 0;
	}
}

/*!
 *
 */
void BearingRangeLine::move_by( double dx, double dy ) {
	update_hook( dx, dy );
}

/*!
 *
 */
bool BearingRangeLine::hit( const QPoint &pt ) const {
	return _box->boundingRect().contains( pt );
}


//------------------------------------------------------------------------------
// [public slots]
//

/*!
 *
 */
void BearingRangeLine::update() {
	if ( _anchor )
		update_anchor( _anchor->cx(), _anchor->cy() );

	if ( _hook )
		update_hook( _hook->cx(), _hook->cy() );
}


//------------------------------------------------------------------------------
// [public slots]
//

/*!
 *
 */
void BearingRangeLine::update_anchor( double x, double y ) {
	QPoint end( _line->endPoint() );
	_line->setPoints( x, y, end.x(), end.y() );
	update_data();
}

/*!
 *
 */
void BearingRangeLine::update_hook( double x, double y ) {
	QPoint start( _line->startPoint() );
	QPoint end( _line->endPoint() );
	QPoint off( x - end.x(), y - end.y() );

	_line->setPoints( start.x(), start.y(), x, y );
	_X1->moveBy( off.x(), off.y() );
	_X2->moveBy( off.x(), off.y() );
	_range->moveBy( off.x(), off.y() );
	_bearing->moveBy( off.x(), off.y() );
	_time->moveBy( off.x(), off.y() );
	_box->moveBy( off.x(), off.y() );

	update_data();
}

/*!
 * Re-anchors the BRL when an aircraft is deactivated
 */
void BearingRangeLine::reanchor( bool anchor ) {
	if ( ! anchor ) {
		_anchor = 0;
		update_data();
	}
}

/*!
 * Re-hooks the BRL when an aircraft is deactivated
 */
void BearingRangeLine::rehook( bool hook ) {
	if ( ! hook ) {
		_hook = 0;
		update_data();
	}
}

//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 *
 */
void BearingRangeLine::initialize( double x, double y ) {
	_line->setPoints( x, y, x, y );
	_X1->moveBy( x, y );
	_X2->moveBy( x, y );
	_range->moveBy( x, y );
	_bearing->moveBy( x, y );
	_time->moveBy( x, y );
	_box->moveBy( x, y );
}


/*!
 *
 */
void BearingRangeLine::update_data() {
	QString s;

	QPoint start( _line->startPoint() );
	QPoint end( _line->endPoint() );

	double dx = end.x() - start.x();
	double dy = end.y() - start.y();
	double range = _hypot( dx, dy ); 
	double adj_r = _canvas->to_user_length( range );

	const char *fmt = ( adj_r < 100 ? "%04.1f" : "%04.0f" ); 
	_range->setText( s.sprintf( fmt, adj_r ) );

	double bearing = atan2( dy, dx ) * ONE_EIGHTY / PI;
	bearing += NINETY;
	if ( bearing < 0 ) bearing += THREE_SIXTY;
	_bearing->setText( s.sprintf( "%03d", int( bearing + ZERO_PT_FIVE ) ) );

	double time;
	if (
		!_anchor && !_hook ||
		 _anchor &&  _hook
	) {
		_time->setText( "--:--" );
	}
	else {
		time = adj_r / ( _anchor ? _anchor->v() : _hook->v() );
		time *= SIXTY_SQR;	// convert hours to seconds
		int min =      time / SIXTY;
		int sec = (int)time % SIXTY;
		_time->setText( s.sprintf( "%02d:%02d", min, sec ) );
	}	
}

////////////////////////////////////////////////////////////////////////////////
//
// atc::BRLDataSprite
//
//------------------------------------------------------------------------------
// Construction/Destruction
//

/*!
 *
 */
BRLDataSprite::BRLDataSprite( BearingRangeLine *l, const QRect &r, QCanvas *c ) 
	  : QCanvasRectangle( r, c )
	  , _brl( l )
{}

/*!
 *
 */
BRLDataSprite::~BRLDataSprite() { hide(); }


//------------------------------------------------------------------------------
// 
//

/*!
 *
 */
void BRLDataSprite::select() {
	Canvas *c = dynamic_cast< Canvas* >( canvas() );
	c->select_brl( _brl );
}

/*!
 *
 */
void BRLDataSprite::kill() {
	Canvas *c = dynamic_cast< Canvas* >( canvas() );
	c->remove_brl( _brl );
}


////////////////////////////////////////////////////////////////////////////////
//
// atc::VectorTool
//
//------------------------------------------------------------------------------
// Construction/Destruction
//

/*!
 *
 */
VectorTool::VectorTool( Canvas *c, AircraftSprite *s )
	  : _aircraft( s )
{
	_vector = new QCanvasLine( c );
	_vector->setPen( VECTOR_TOOL_PEN );
	_vector->setPoints( s->cx(), s->cy(), s->cx(), s->cy() );
	_vector->show();
}

/*!
 *
 */
VectorTool::~VectorTool() {
	delete _vector;
}

//------------------------------------------------------------------------------
// [public]
//

/*!
 *
 */
void VectorTool::update() {
	QPoint p = _vector->endPoint();
	_vector->setPoints( _aircraft->cx(), _aircraft->cy(), p.x(), p.y() );
}

/*!
 *
 */
void VectorTool::set_end( int x, int y ) {
	QPoint p = _vector->startPoint();
	_vector->setPoints( p.x(), p.y(), x, y);
}

/*!
 *
 */
void VectorTool::action() {
	QPoint p = _vector->endPoint();
	_aircraft->vector( p.x(), p.y() );
}

/*!
 *
 */
bool VectorTool::is_valid() const {
	QPoint p0 = _vector->startPoint();
	QPoint p1 = _vector->endPoint();

	return 
			_hypot( p0.x() - p1.x(), p0.y() - p1.y() ) 
		 >= VECTOR_TOOL_MIN_LENGTH;
}

DivisionLine::DivisionLine(	QCanvas*			a_canvas,
							const std::string&	a_id,
							const int&			a_x1,
							const int&			a_y1,
							const int&			a_x2,
							const int&			a_y2,
							const QColor&		a_color,
							const uint&			a_width)
	:	QCanvasLine(a_canvas),
		m_id(a_id)
{
    setPoints(a_x1, a_y1, a_x2, a_y2);
    setPen(QPen(a_color, a_width));
}
		
DivisionLine::~DivisionLine()
{
}

void 
DivisionLine::SetDisplayTime(	const int& a_startTime, 
								const int& a_endTime)
{
	m_displayTimes.push_back(std::make_pair<int, int>(a_startTime, a_endTime));
}

bool
DivisionLine::Display(const int& a_time)
{
	bool display = false;

	for (std::vector<DisplayTimeRange>::const_iterator it = m_displayTimes.begin();
			it != m_displayTimes.end(); it++)
	{
		if (it->second != atc::INVALID_VALUE)
		{
			if ((a_time >= it->first) && (a_time <= it->second))
			{
				display = true;
				break;
			}
		}
		else
		{
			if (a_time >= it->first)
			{
				display = true;
				break;
			}
		}
	}

	if (display)
	{
		show();
	}
	else
	{
		hide();
	}

	return display;
}
////////////////////////////////////////////////////////////////////////////////

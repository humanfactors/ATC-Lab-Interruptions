/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: scale.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:45 $
      @state    : $State: Exp $

      $Log: scale.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:45  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.6  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.5  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.4  2006/08/29 08:44:56  seth
      reimplementing mvc pattern

      Revision 1.3  2006/08/28 06:21:48  seth
      refactoring mvc

      Revision 1.2  2006/06/01 03:41:23  seth
      implemented CFL and speed selectors


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "scale.h"
#include "atc.h"
#include "canvas.h"

using namespace atc;

//------------------------------------------------------------------------------
// Construction/Destruction
//

/*!
 * Main constructor
 */
ScaleXHair::ScaleXHair(
		Canvas* canvas, 
		int xexent, 
		int yextent, 
		int interval, 
		int size
)
		:_canvas(canvas)
		,_x(0),_y(0)
		,_xextent(xexent), _xinterval(interval)
		,_yextent(yextent), _yinterval(interval)
		,_ticklen(size)
		,_xhair()
{
	build();
}

/*!
 * Destructor
 */
ScaleXHair::~ScaleXHair() {}


//------------------------------------------------------------------------------
// [public]
//

/*!
 *
QPointArray ScaleXHair::areaPoints() const {
	return 	QPointArray( QRect( 
			_x - _xextent - 1, _y - _yextent - 1
		  , 2 *_xextent + 2, 2 * _yextent + 2 
	) );
}
*/

/*!
 * Relocate
 */
void ScaleXHair::move_by( int dx, int dy ) {
	_x += dx; _y += dy;

	std::vector< QCanvasLine* >::const_iterator it;
	for ( it = _xhair.begin() ; it != _xhair.end(); ++it ) {
		(*it)->moveBy( dx, dy );
	}
}

//------------------------------------------------------------------------------
// [private]
//

/*!
 * Build the Scale Marker
 */
void ScaleXHair::build() {
	int tick;

	create_line( -_xextent, 0, _xextent, 0 );
	create_line( 0, -_yextent, 0, _yextent );

	for ( int i = _xinterval, n= 1; i <= _xextent; i += _xinterval, ++n ) {
		tick = _ticklen;
		if ( (n %  5) == 0 ) tick *= 2;
		if ( (n % 10) == 0 ) tick *= 2;

		create_line( i, -tick,  i, tick);
		create_line(-i, -tick, -i, tick);
	}

	for ( int j = _yinterval, m = 1; j <= _yextent; j += _yinterval, ++m ) {
		tick = _ticklen;
		if ( (m %  5) == 0 ) tick *= 2;
		if ( (m % 10) == 0 ) tick *= 2;

		create_line( -tick,  j, tick,  j );
		create_line( -tick, -j, tick, -j );
	}
}

/*!
 * Construct new scale cross hair component (QCanvasLine)
 */
void ScaleXHair::create_line( int xa, int ya, int xb, int yb ) {
	QCanvasLine *item = new ScaleLine( _canvas, this );
	item->setPen( SCALE_PEN );
	item->setPoints( xa, ya, xb, yb );
	item->show();

	_xhair.push_back( item );
}


////////////////////////////////////////////////////////////////////////////////


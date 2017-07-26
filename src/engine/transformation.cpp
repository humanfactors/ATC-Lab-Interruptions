/**
      @project  : ATC-Lab [engine]

      @file     : $RCSfile: transformation.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.4 $ )
      @date     : $Date: 2007/07/26 10:20:25 $
      @state    : $State: Exp $

      $Log: transformation.cpp,v $
      Revision 1.4  2007/07/26 10:20:25  Aaron Yeung
      Changed scaling to make map larger

      Revision 1.3  2007/07/08 04:53:03  Aaron Yeung
      Made airspace view smaller

      Revision 1.2  2007/07/05 15:04:19  Aaron Yeung
      Changed the map scale factor from 10 to 6 so that map sizes smaller than 100x75 won't appear so small

      Revision 1.1.1.2  2007/06/21 12:22:46  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.5  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.4  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.3  2006/09/13 05:38:49  seth
      full working implemtation of Range/Bearing tool

      Revision 1.2  2006/09/06 13:09:24  seth
      refactoring translations into canvas

      Revision 1.1  2006/06/07 05:50:14  seth
      refactor prior to implementation of performance data lookup


      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology
  
**/

#include "transformation.h"

using namespace atc;

///////////////////////////////////////////////////////////////////////////////
//
// Transformation
//
// Converts user measurements to internal representaiton. Currently assumes
// that x and y scalings are equal.
//
//-----------------------------------------------------------------------------
// construction/destruction
//

/*!
 *
 */
Transformation::Transformation() : _mtx() {}

/*!
 *
 */
Transformation::Transformation( const Transformation &rhs ) 
	  : _mtx( rhs._mtx ) 
{}

/*!
 *
 */
Transformation::~Transformation() {}


//-----------------------------------------------------------------------------
// [public] initialization
//

/*!
 * Initialize the transformation parameters.
 *
 * The internal representation of the region has its origin in the top left 
 * corner and is at least <code>hint</code> wide. The hint allows a minimum 
 * width to be specified which will allow QCanvas extents to be larger than 
 * the screen pixel count, which is useful since some acuracy is lost with 
 * QCanvas since it uses integers.
 *
 * @param user defines the region of interest in user coordinate system
 * @param hint defines a suggested minimum extent for internal representation
 *
 * @return internal representation
 *
 * @todo: check for aspect ratio problems
 */
const QRect Transformation::calculate( const QRect &region, int hint ) {
	int iw = region.width();
	int ih = region.height();
	while ( iw < hint ) {
		iw *= 6;
		ih *= 6;
	}
	QRect internal( 0, 0, iw, ih );
	init_matrix( region, internal );

	return internal;
}


//------------------------------------------------------------------------------
// [public] interface transformations
//

/*!
 * Convert user point to internal representation (double)
 */
std::pair<double, double> Transformation::point(double x, double y) const {
	double ix, iy;
	_mtx.map(x, y, &ix, &iy);
	return std::make_pair( ix, iy);
}


/*!
 * Convert user point to internal representation (integer)
 */
QPoint Transformation::qpoint( double mx, double my ) const {
	std::pair< double, double > pt = point( mx, my );
	return QPoint( (int)(pt.first + 0.5), (int)(pt.second + 0.5) );
}
		
/*!
 * Convert user point to internal representation (integer)
 */
QPoint Transformation::qpoint( const QPoint &pt ) const {
	return qpoint( (double)( pt.x() ), (double)( pt.y() ) );
}

/*!
 * Convert user length to internal
 */
double Transformation::length( double l ) const {
	return _mtx.m11() * l;
}

/*!
 * Convert height measurements
 *
 * @todo currently only 2D
 */
double Transformation::height( double h ) const {
	return h;
}

/*!
 * Convert point array
 */
QPointArray Transformation::qpoints( const QPointArray &pts ) const {
	QPointArray tpts( pts.size() );

	for ( int i = 0; i < pts.size(); ++i ) {
		tpts.setPoint( i, qpoint( pts[i].x(), pts[i].y() ) );
	}

	return tpts;
}


//------------------------------------------------------------------------------
// [public] interface inverse transformations
//

/*!
 * Convert internal point to user units
 */
std::pair<double, double> 
Transformation::inverse_point(double x, double y) const {
	Transformation t = invert();
	return t.point( x, y );
}

/*!
 * Convert internal point to user units
 */
QPoint
Transformation::inverse_qpoint(double x, double y) const {
	std::pair< double, double > pt = inverse_point( x, y );
	return QPoint( (int)(pt.first + 0.5), (int)(pt.second + 0.5) );
	;
}

/*!
 * Convert internal length to user units
 */
double Transformation::inverse_length( double l ) const {
	Transformation t = invert();
	return t.length( l );
}




//------------------------------------------------------------------------------
// [public] interface
//

/*!
 * Generate the inverse transformation
 */
const Transformation Transformation::invert() const {
	if ( ! _mtx.isInvertible() ) 
		throw new transformation_error ( "cannot get inverse transformation" );

	Transformation inv_t;
	inv_t._mtx = _mtx.invert();
	return inv_t;
}


//------------------------------------------------------------------------------
// [private]
//

/*!
 * Initinalize internal transformation matrix
 *
 * @param region area of interest in user space
 * @param internal representation of area
 */
void Transformation::init_matrix( const QRect &region, const QRect &internal ) {
	double sx = double(  internal.width()  ) / region.width();
	double sy = double( -internal.height() ) / region.height();

	double tx = -sx * region.left(); // + internal.left() [=0]
	double ty = -sy * region.top() + internal.bottom();

	_mtx.setMatrix( sx, 0, 0, sy, tx, ty );
}


///////////////////////////////////////////////////////////////////////////////
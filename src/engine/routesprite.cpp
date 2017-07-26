/**
 * @file:       atc_routesprite.cpp
 *
 *              ATC_Engine project definitions - c++ definitions.
 *
 *		Canvas item representing a designated flight route.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 13/07/04
 *
 * $Id: routesprite.cpp,v 1.1.1.2 2007/06/21 12:22:45 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "routesprite.h"
#include <qpainter.h>
#include "canvas.h"
#include "intersectionpoint.h"

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
//
//
//-----------------------------------------------------------------------------
//
//

/*!
 * Constructor
 *
 * Adds the sprite to the canvas. The canvas will clean up sprites associated
 * with it in this way ie they take responsibility.
 */
RouteSprite::RouteSprite( QCanvas *c )
	:QCanvasPolygonalItem( c )
{
	setPen( Qt::black );
}

/*!
 * Destructor
 */
RouteSprite::~RouteSprite() {
	hide();
}

//-----------------------------------------------------------------------------

/*!
 *
 */
void RouteSprite::set_points( QPointArray pa ) {
	if(!_vertices.isNull())
		throw routesprite_error(
			"cannot reinitialize route sprite"
		);

	if(pa.count() < 2)
		throw routesprite_error(
			"invalid initialization parameter"
		);

	_vertices = pa;
	_vertices.detach();	// explicit sharing stupid (apparently!)

	// @todo check for and remove coincident points

	reset_outline();

	//addToChunks();
}

//-----------------------------------------------------------------------------
// virtual
//

/*!
 *
 */
QPointArray RouteSprite::areaPoints()const{
	return _outline.copy();
}

/*!
 *
 */
void RouteSprite::drawShape(QPainter &p)
{
	p.drawPolyline(_vertices);
}

//-----------------------------------------------------------------------------
// private
//

/*!
 * Calculate the bounding region that contains the polyline.
 *
 * pre: _verticies.size() >= 2
 * pre: _verticies[n] != _verticies[n+1] for all n
 *
 */
void RouteSprite::reset_outline()
{
	int vertices_size = _vertices.count();
	int outline_size = 2 * vertices_size + 2;
	QPointArray area(outline_size);
	int insert = 0;

	int pw = pen().width() * 4/3 + 2;	// approx pw * sqrt(2)

	QPoint p1 = _vertices.at(0);
	QPoint p2 = _vertices.at(1);

	CanvasLine lineA = offset_line(p1, p2, pw);

	for(int i = 2; i < vertices_size; ++i){

		QPoint p3 = _vertices.at(i);

		CanvasLine lineB = offset_line(p2, p3, pw);
		IntersectionPoint intersect(
			lineA.first, lineA.second,
			lineB.first, lineB.second
		);

		area.setPoint(insert++, lineA.first);

		lineA = std::make_pair(
			intersect.coincident() ? lineB.first : intersect.point(),
			lineB.second
		);

		p1 = p2;
		p2 = p3;
	}

	area.setPoint(insert++, lineA.first);
	area.setPoint(insert++, lineA.second);

	// @todo set end points
	// @todo set reverse side
	



	_outline = area;
	_outline.detach();
}

/*!
 * Calculates the position of offset line
 */
CanvasLine RouteSprite::offset_line(
	const QPoint &p1, const QPoint &p2, int offset
){
	int dx = p2.x() - p1.x();
	int dy = p2.y() - p1.y();

	QPoint translation(0, 0);

	if(!dx && !dy)
		// pre condition check
		throw routesprite_error(
			"coincident points detected"
		);

	else if(abs(dy) >= abs(dx))
		translation.setX( dy > 0 ? -offset : offset);
	
	else 
		translation.setY( dx > 0 ? offset : -offset);

	QPoint tp1 = p1 + translation;
	QPoint tp2 = p2 + translation;

	return std::make_pair(tp1, tp2);
}


//-----------------------------------------------------------------------------
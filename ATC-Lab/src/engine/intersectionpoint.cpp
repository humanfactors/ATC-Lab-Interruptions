/**
 * @file:       atc_intersectionpoint.cpp
 *
 *              ATC_Engine project definitions - c++ definitions.
 *
 *		Utility for calculating the intersection point for two lines.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 14/07/04
 *
 * $Id: intersectionpoint.cpp,v 1.1.1.2 2007/06/21 12:22:43 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "intersectionpoint.h"
#include <qpoint.h>

//-----------------------------------------------------------------------------

/*!
 *
 */
atc::IntersectionPoint::IntersectionPoint(
	const QPoint &p1, const QPoint &p2,
	const QPoint &p3, const QPoint &p4
){
	_u_denominator =
		(p4.y() - p3.y()) * (p2.x() - p1.x()) -
		(p4.x() - p3.x()) * (p2.y() - p1.y());

	_ua_numerator = 
		(p4.x() - p3.x()) * (p1.y() - p3.y()) -
		(p4.y() - p3.y()) * (p1.x() - p3.x());

	_ub_numerator = 
		(p2.x() - p1.x()) * (p1.y() - p3.y()) -
		(p2.y() - p1.y()) * (p1.x() - p3.x());

	if(_u_denominator){
		double ua = (_ua_numerator / _u_denominator);
		_x = (int)( p1.x() + ua * (p2.x() - p1.x()) );
		_y = (int)( p1.y() + ua * (p2.y() - p1.y()) );
	}
}

/*!
 *
 */
atc::IntersectionPoint::~IntersectionPoint()
{}

//-----------------------------------------------------------------------------

/*!
 *
 */
bool atc::IntersectionPoint::parallel()const{
	return !_u_denominator;
}

/*!
 *
 */
bool atc::IntersectionPoint::coincident()const{
	return parallel() && !_ua_numerator && !_ub_numerator;
}

/*!
 *
 */
QPoint atc::IntersectionPoint::point()const
{
	if(parallel())
		throw atc::intersectionpoint_error(
			"lines are parallel"
		);

	return QPoint(_x, _y);
}

//-----------------------------------------------------------------------------
/**
 * @file:       atc_routearcsprite.cpp
 *
 *              ATC_Engine project definitions - c++ definitions.
 *
 *		Canvas item representing portion of designated flight route.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 15/07/04
 *
 * $Id: routearcsprite.cpp,v 1.1.1.2 2007/06/21 12:22:45 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "routearcsprite.h"
#include "canvas.h"

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
/*!
 * \class atc::RouteArcSprite atc_routearcsprite.h
 * \brief Represents a section of a designated flight route on the Canvas.
 *	
 * Initially all the sections of a route where going to be represented by a 
 * single sprite (atc::RouteSprite) for two reasons :
 *	1] a QCanvasLine based class in integer based
 *	2] want to be able to select all the sections of a route at once
 * Unfortunately QCanvas does not support a polyline and polygons are solid
 * shapes with no outline (due to limitations of QRegion upon which their 
 * implementation depends.
 *	An implementation based on CanvasLine is much easier to implement and
 * the reasons for using a polyline implementation are easily overcome:
 *	1] if the size of the canvas is bigger than the number of screen pixels
 *	   then no additional information is lost by rounding. Also Routes are 
 *	   static for display purposes only so high accuracy not required.
 *	2] by giving each section a pointer to _prev and _next sections a path
 *	   can be found.
 *
 *
 * \todo implement path ie _prev && _next
 */
//-----------------------------------------------------------------------------
//
//

/*!
 * Constructor
 *
 * Adds the sprite to the canvas. The canvas will clean up sprites associated
 * with it in this way ie they take responsibility.
 */
RouteArcSprite::RouteArcSprite( QCanvas *c )
	  : QCanvasLine( c )
{
	setPen(Qt::black);
	setZ(-5);
}

/*!
 * Destructor
 */
RouteArcSprite::~RouteArcSprite() {
	hide();
}

//-----------------------------------------------------------------------------

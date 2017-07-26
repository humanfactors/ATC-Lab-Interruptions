/**
 * ATC_Engine.
 * Canvas item representing the location of a waypoint.
 *
 * @author:     seth  
 * @email:      atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 08/07/04
 *
 * $Id: waypointsprite.cpp,v 1.1.1.2 2007/06/21 12:22:46 Aaron Yeung Exp $
 *
 * Copyright 2004 ARC Key Centre for 
 * Human Factors & Applied Cognitive Psychology
 */

#include "waypointsprite.h"
#include "canvas.h"
#include "logger.h"

using namespace atc;

///////////////////////////////////////////////////////////////////////////////
//
//
//
//-----------------------------------------------------------------------------
// Construction/Desturction
//

/*!
 * Constructor
 */
WayPointSprite::WayPointSprite(
	const std::string& name, QPoint pt, Canvas *canvas
)
	  : _point(0)
	  , _name(0)
	  , _visible(false)
{
	_point = new WPLocator(
			this, WAYPOINT_SPRITE_DIAMETER, WAYPOINT_SPRITE_DIAMETER, canvas
	);
	_point->setBrush( WAYPOINT_SPRITE_BRUSH );
	_point->setZ( WAYPOINT_SPRITE_LAYER );
	_point->move( pt.x(), pt.y() );

	QFontMetrics fm( WAYPOINT_SPRITE_FONT );

	_name = new WPText( name, WAYPOINT_SPRITE_FONT, canvas );
	_name->setColor( WAYPOINT_SPRITE_COLOR );
	_name->setZ( WAYPOINT_SPRITE_LAYER );
	_name->move(pt.x(), pt.y() - fm.lineSpacing());

	toggle();
}

/*!
 * Destructor
 */
WayPointSprite::~WayPointSprite() {}

//-----------------------------------------------------------------------------
// [public] interface
//

/*!
 * Test for selection of waypoint.
 */
bool WayPointSprite::hit(QPoint pt)const{
	return  
		_name->boundingRect().contains(pt) ||
		_point->boundingRect().contains(pt);
}

/*!
 * Toggle visible status
 */
void WayPointSprite::toggle() {
	_visible = !_visible;
	_point->setVisible(_visible);
	_name->setVisible(_visible);
}

//-----------------------------------------------------------------------------

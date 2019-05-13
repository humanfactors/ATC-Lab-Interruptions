/**
 * @file:       atc_sectorsprite.cpp
 *
 *              ATC_Engine project definitions - c++ definitions.
 *
 *		Canvas item representing sector [body].
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 15/07/04
 *
 * $Id: sectorsprite.cpp,v 1.1.1.2 2007/06/21 12:22:45 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "sectorsprite.h"
#include "atc.h"
#include "canvas.h"

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
// atc::SectorSprite
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
SectorSprite::SectorSprite( QCanvas *canvas )
	  : QCanvasPolygon(canvas)
{
	setBrush( SECTOR_BRUSH );
	setZ( SECTOR_AREA_Z );
}

/*!
 * Destructor
 */
SectorSprite::~SectorSprite() {
	hide();
}

////////////////////////////////////////////////////////////////////////////////
//
// atc::ActiveSectorSprite
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
ActiveSectorSprite::ActiveSectorSprite( QCanvas *c )
	  : SectorSprite( c )
{
	setBrush( SECTOR_ACTIVE_BRUSH );
	setZ( SECTOR_ACTIVE_AREA_Z );
}

/*!
 * Destructor
 */
ActiveSectorSprite::~ActiveSectorSprite() {
	hide();
}

//-----------------------------------------------------------------------------

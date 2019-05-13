/**
 * @file:       atc_sectorarcsprite.cpp
 *
 *              ATC_Engine project definitions - c++ definitions.
 *
 *		Canvas item representing portion of sector outline.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 15/07/04
 *
 * $Id: sectorarcsprite.cpp,v 1.1.1.2 2007/06/21 12:22:45 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "sectorarcsprite.h"
#include "atc.h"
#include "canvas.h"

//-----------------------------------------------------------------------------

/*!
 * \class atc::SectorArcSprite atc_sectorarcsprite.h
 * \brief Represents a section of a sector outline on the Canvas.
 */

//-----------------------------------------------------------------------------

/*!
 * Constructor
 *
 * Adds the sprite to the canvas. The canvas will clean up sprites associated
 * with it in this way ie they take responsibility.
 */
atc::SectorArcSprite::SectorArcSprite(QCanvas *canvas)
	:QCanvasLine(canvas)
{
	setPen( SECTOR_PEN );
	setZ( SECTOR_LINE_Z );
}

/*!
 * Destructor
 */
atc::SectorArcSprite::~SectorArcSprite()
{
	hide();
}

//-----------------------------------------------------------------------------

/**
 * @file:       atc_sectorsprite.h
 *
 *              ATC_Engine project definitions - c++ header.
 *
 *		Canvas item representing sector [body].
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 15/07/04
 *
 * $Id: sectorsprite.h,v 1.1.1.2 2007/06/21 12:22:45 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_SECTORSPRITE_HDR__
#define __ATC_SECTORSPRITE_HDR__


#include <qcanvas.h>
#include "canvas_types.h"
#include <stdexcept>


namespace atc{


	//
	// atc::sectorsprite_error
	//
	struct sectorsprite_error : public std::runtime_error{
		sectorsprite_error(const std::string& msg) 
			:std::runtime_error(msg)
		{}
	};


	//
	// atc::SectorSprite
	//
	class SectorSprite : public QCanvasPolygon {

	public: 
		SectorSprite( QCanvas * );
		~SectorSprite();

	public: // [virtual]
		int rtti()const { return CanvasItem::RTTI_SECTOR; }

	private:

	};

	
	//
	// atc::ActiveSectorSprite
	//
	class ActiveSectorSprite : public SectorSprite {

	public: 
		ActiveSectorSprite( QCanvas * );
		~ActiveSectorSprite();

	};

};

#endif

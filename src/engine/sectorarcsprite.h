/**
 * @file:       atc_sectorarcsprite.h
 *
 *              ATC_Engine project definitions - c++ header.
 *
 *		Canvas item representing portion of sector outline.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 15/07/04
 *
 * $Id: sectorarcsprite.h,v 1.1.1.2 2007/06/21 12:22:45 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_SECTORARCSPRITE_HDR__
#define __ATC_SECTORARCSPRITE_HDR__


#include <qcanvas.h>
#include "canvas_types.h"
#include <stdexcept>


namespace atc{


	/*!
	 * \struct sectorarcsprite_error
	 */
	struct sectorarcsprite_error : public std::runtime_error{
		sectorarcsprite_error(const std::string& msg) 
			:std::runtime_error(msg)
		{}
	};


	/*!
	 * \class atc::SectorArcSprite atc_sectorarcsprite.h
	 */
	class SectorArcSprite : public QCanvasLine 	{

	public: 
		SectorArcSprite( QCanvas * );
		~SectorArcSprite();

	public: // [virtual]
		int rtti()const { return CanvasItem::RTTI_SECTORARC; }

	private:

	};

};

#endif

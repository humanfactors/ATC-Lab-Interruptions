/**
 * @file:       atc_routearcsprite.h
 *
 *              ATC_Engine project definitions - c++ header.
 *
 *		Canvas item representing portion of designated flight route.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 15/07/04
 *
 * $Id: routearcsprite.h,v 1.1.1.2 2007/06/21 12:22:45 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_ROUTEARCSPRITE_HDR__
#define __ATC_ROUTEARCSPRITE_HDR__


#include <qcanvas.h>

#include "canvas_types.h"
#include <stdexcept>


namespace atc{


	/*!
	 * \struct routesprite_error
	 */
	struct routearcsprite_error : public std::runtime_error{
		routearcsprite_error(const std::string& msg) 
			:std::runtime_error(msg)
		{}
	};


	/*!
	 * \class atc::RouteSprite atc_routesprite.h
	 */
	class RouteArcSprite : public QCanvasLine {

	public: 
		
		RouteArcSprite( QCanvas * );
		~RouteArcSprite();

	public: // [virtual]

		virtual int rtti() const { return CanvasItem::RTTI_ROUTEARC; }

	private:

	};

};

#endif

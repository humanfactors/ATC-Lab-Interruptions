/**
 * @file:       atc_routesprite.h
 *
 *              ATC_Engine project definitions - c++ header.
 *
 *		Canvas item representing a designated flight route.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 13/07/04
 *
 * $Id: routesprite.h,v 1.1.1.2 2007/06/21 12:22:45 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_ROUTESPRITE_HDR__
#define __ATC_ROUTESPRITE_HDR__


#include <qcanvas.h>

#include "canvas_types.h"
#include <stdexcept>


namespace atc{


	/*!
	 * \struct routesprite_error
	 */
	struct routesprite_error : public std::runtime_error{
		routesprite_error(const std::string& msg) 
			:std::runtime_error(msg)
		{}
	};


	/*!
	 * \typedef CanvasLine
	 */
	typedef std::pair<QPoint, QPoint>CanvasLine;


	/*!
	 * \class atc::RouteSprite atc_routesprite.h
	 */
	class RouteSprite : public QCanvasPolygonalItem {

	public: 
		RouteSprite(QCanvas *);
		~RouteSprite();

		void set_points(QPointArray);

	public: // [virtual]
		int rtti() const { return CanvasItem::RTTI_ROUTE; }

		QPointArray areaPoints() const;
		void drawShape( QPainter & );

	private:
		void reset_outline();
		CanvasLine offset_line(	const QPoint &, const QPoint &, int );

	private:
		QPointArray _vertices;
		QPointArray _outline;

	};

};

#endif

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
 * $Id: waypointsprite.h,v 1.1.1.2 2007/06/21 12:22:46 Aaron Yeung Exp $
 *
 * Copyright 2004 ARC Key Centre for 
 * Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_WAYPOINTSPRITE_HDR__
#define __ATC_WAYPOINTSPRITE_HDR__


#include <qcanvas.h>
#include "canvas_types.h"
#include <string>


namespace atc{

	//
	// atc
	//
	class Canvas;


	//
	// atc::WayPointSprite
	//
	class WayPointSprite {

	public: 
		WayPointSprite( const std::string&, QPoint, atc::Canvas * );
		~WayPointSprite();

	public:
		bool hit(QPoint)const;

		double x()const{ return _point->x(); }
		double y()const{ return _point->y(); }

		void toggle();

	private:
		QCanvasEllipse *_point;
		QCanvasText    *_name;

		bool _visible;
	};


	//
	// atc::WPLocator
	//
	class WPLocator : public QCanvasEllipse {

	public:
		WPLocator( WayPointSprite *s, int w, int h, QCanvas *c  ) 
			  : QCanvasEllipse( w, h, c ), _waypoint( s ) {}
		~WPLocator() { hide(); }

	public: //virtual
		int rtti() const { return CanvasItem::RTTI_WPLOCATOR; }

	public:
		WayPointSprite* waypoint() { return _waypoint; }

	private:
		WayPointSprite *_waypoint;
	
	};


	//
	// atc::WPText
	//
	class WPText : public QCanvasText {

	public:
		WPText( const QString &s, QFont f, QCanvas *c ) 
			  : QCanvasText( s, f, c ) {} 
		~WPText() { hide(); }

	public: //virtual
		int rtti() const { return CanvasItem::RTTI_WPTEXT; }

	};


};

#endif

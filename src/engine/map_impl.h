/**
 * @file:       atc_map_impl.h
 *
 *              ATC_Engine
 *              Implementation of Map for static canvas objects
 *
 * @author:     seth  
 * @email:	    atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 15/06/04
 *
 * $Id: map_impl.h,v 1.2 2007/07/05 15:07:53 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_MAP_IMPL_HDR__
#define __ATC_MAP_IMPL_HDR__


#ifdef WIN32
	// identifier truncated in debug information
	#pragma warning(disable:4786)
	// decorated name length exceeded, name was truncated
	#pragma warning(disable:4503)
#endif


#include <map>
#include <string>
#include <utility>
#include <vector>

#include <qwmatrix.h>
#include <qpixmap.h>
#include <qpoint.h>

#include "map.h"
#include "area_definition.h"


namespace atc{

	//
	// atc::Location
	// 
	struct Location {
		Location( const char *id, double X, double Y, bool visible )
			: name( id ), x( X ), y( Y ), show( visible ) {}
		
		~Location() {}

		std::string name;
		double      x;
		double      y;
		bool        show;
	};


	/*
	 * \typedef LocationMap
	 * \typedef LocationMapIt
	 * \typedef LocationMapCIt
	 */
	typedef std::map< std::string, Location > LocationMap;


	/*
	 * \typedef RouteDef
	 * \typedef RouteDefCIt
	 * \typedef RouteMap
	 * \typedef RouteMapIt
	 * \typedef RouteMapCIt
	 */
	typedef std::vector< atc::Location > RouteDef;
	typedef std::map< std::string, atc::RouteDef * >RouteMap;


	/*!
	 *
	 */
	typedef std::map< std::string, atc::SectorDefinition > SectorMap;


	/*!
	 * \struct atc::map_error
	 */
	struct map_error : public std::runtime_error{
		map_error(const std::string& msg) : std::runtime_error(msg){}
	};


	/*
	 * \class atc::Canvas
	 */
	class Canvas;


	/*
	 * \class atc::MapImpl atc_map_impl.h
	 */
	class MapImpl : public Map
	{
	public:

		MapImpl();
		virtual ~MapImpl();

	public:	// Map Interface

		void set_region(double, double, double, double);

		bool add_location( const char *, double, double, bool );

		bool create_route(const char *);
		bool add_route_point(const char *, double, double);

		void create_sector( const char * );
		void set_sector_active( const char *uid );
		void add_sector_vertex( const char *, double, double );
		void add_sector_arc( const char *, double, double, double );

	public: // accessors

		int width()const{ return _Emax - _Emin; }
		int height()const{ return _Nmax - _Nmin; }

		QRect rect()const{ return QRect(_Emin, _Nmin, width(), height()); }

		const SectorMap& sectors()const{ return _sectormap; }
		const RouteMap& routes()const{ return _routemap; }
		const LocationMap& locations()const{ return _locationmap; }

		const char * active_sector() const { return _active_sector; }

	private:
	
		// signals
		// @todo QObject base for signals
		void warning(const char *){};

	private:
		
		bool point_on_map(double x, double y);
		void validate( const char *uid );
		
	private:

		double _Emin, _Emax;
		double _Nmin, _Nmax;

		LocationMap _locationmap;
		RouteMap    _routemap;
		SectorMap   _sectormap;

		const char *_active_sector;
	};

}

#endif

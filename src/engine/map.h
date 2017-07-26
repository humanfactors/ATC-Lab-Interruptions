/**
 * @file:       atc_map.h
 *
 *              ATC_Engine project definitions - c++ header .
 *
 *		External Interface for creation of Map Object.
 *              Contains geometry information and translations between 
 *              Real and Experiment Space.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 02:34:37 PM 03/05/04
 *
 * $Id: map.h,v 1.2 2007/07/05 15:07:53 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_MAP_HDR__
#define __ATC_MAP_HDR__


#include <qpixmap.h>


namespace atc{


	/*!
	 * \class atc::Map atc_map.h
	 * \brief Interface to set static features of the Engine's Canvas
	 */
	class Map
	{
	public:


		/*!
		 * Set the 'real space' region that is being modeled
		 */
		virtual void set_region(
			double Emin, double Emax, 
			double Nmin, double Nmax
		) = 0;

		/*!
		 * Add a waypoint to the Map.
		 * Way points are used in 'Route' descriptions. Depending
		 * upon the type of the way point, a different screen 
		 * representation will be used.
		 *
		 * \param uid is a unique id for refering to the waypoint.
		 */
		virtual bool add_location( const char *, double, double, bool ) = 0;

		/*!
		 * Create an entry for a Route with unique id.
		 *
		 * The path through the waypoints added to the route will
		 * pass through the waypoints in the order in which they were
		 * added. An aircraft may track either way along such a defined
		 * route.
		 * If no actual waypoints are added to the route a warning 
		 * message will be emitted when the map is validated.
		 * 
		 * \sa add_route_point()
		 */
		virtual bool create_route(const char *uid) = 0;

		/*!
		 * Add a waypoint to a route.
		 *
		 * The route must have already been created with a unique 
		 * identifier. The way point must already appear in the 
		 * waypoint list.
		 *
		 * \sa create_route(), add_waypoint()
		 */
		virtual bool add_route_point(
			const char *uid, 
			double x, double y
		) = 0;

		/*!
		 * Create an entry for a Sector with a unique identifier.
		 *
		 * The sector descriptors added to the sector will be assumed 
		 * to define a closed polygon. Each descriptor will add a new
		 * segment to the polygon in a clockwise direction and the last
		 * descriptor is assumed to link to the first (ie it is not 
		 * necessary to enter the starting point twice).
		 * If no actual sector descriptors are added to the sector a 
		 * warning message will be emitted when the map is validated.
		 *
		 * \sa add_sector_vertex(), add_sector_arc()
		 */
		virtual void create_sector( const char * ) = 0;

		/*!
		 *
		 */
		virtual void set_sector_active( const char *uid ) = 0;

		/*!
		 * Add a vertex to the sector.
		 *
		 * The sector must have already been created with a unique
		 * identifier.
		 *
		 * \sa create_sector(), add_sector_arc()
		 */
		virtual void add_sector_vertex(
			const char *uid, double x, double y
		) = 0;

		/*!
		 * Add an arc to the sector.
		 *
		 * The sector must have already been create with a unique
		 * identifer. If two arcs are added next to each other then
		 * the intersection point is assumed to be the point that 
		 * results in the shortest arc lengeth.
		 *
		 * \sa create_sector(), add_sector_vertex()
		 */
		virtual void add_sector_arc( 
				const char *uid, double x, double y, double r
		) = 0;

	};

}

#endif

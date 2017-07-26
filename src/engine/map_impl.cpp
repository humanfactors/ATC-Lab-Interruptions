/**
 * @file:       atc_map_impl.cpp
 *
 *              ATC_Engine project definitions - c++ header .
 *
 *		Private to atc_engine.
 *              Contains geometry information and translations between 
 *              Real and Experiment Space.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 15/06/04
 *
 * $Id: map_impl.cpp,v 1.2 2007/07/05 14:56:19 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "map_impl.h"
#include <cmath>
#include <sstream>
#include <utility>

using namespace atc;


//-----------------------------------------------------------------------------

/*!
 * \class MapImpl atc_map_impl.h
 * \brief
 *	
 * [Insert description here]
 *
 * \todo
 */

//-----------------------------------------------------------------------------

/*!
 * Default Constructor
 */
MapImpl::MapImpl()
	  : _Emin( 0 ), _Emax( 0 )
	  , _Nmin( 0 ), _Nmax( 0 )
	  , _locationmap()
	  , _routemap()
	  , _sectormap()
	  , _active_sector( 0 )
{}

/*!
 * Destructor
 */
MapImpl::~MapImpl() {
	SectorMap::const_iterator sec = _sectormap.begin();
	for ( ; sec != _sectormap.end(); ++sec ) {
		
	}

	RouteMap::const_iterator it = _routemap.begin();
	for ( ; it != _routemap.end(); ++it ) {
		delete it->second;
	}
}

//-----------------------------------------------------------------------------

//
// Map Interface [creational]
// (see atc_map.h for documentation)
//

void MapImpl::set_region(double Emin, double Emax, double Nmin, double Nmax)
{
	if(Emin >= Emax || Nmin >= Nmax)
		throw map_error("invalid map region");

	_Emin = Emin;
	_Emax = Emax;
	_Nmin = Nmin;
	_Nmax = Nmax;
}


/*!
 */
bool MapImpl::add_location( 
		const char *uid, double x, double y, bool visible 
) {
	try{
		if ( !strlen(uid) )
			throw map_error(
				"invalid unique waypoint identifier"
			);
		if ( _locationmap.count(uid) )
			throw map_error(
				std::string(uid) +
				" location unique identifier is allocated"
			);
		if ( !point_on_map(x, y) ) {
			std::ostringstream oss;
			oss 
				<< uid << "(" << x << ", " << y 
				<< ") location is off map";
		//	emit warning(oss.str());
		}
		Location pt( uid, x, y, visible );
		std::pair< LocationMap::iterator, bool > result 
			  = _locationmap.insert( std::make_pair( uid, pt ) );

		return result.second;

	}catch(map_error &e){
		Q_UNUSED(e);
	//	emit warning(e.what());
		return false;
	}
}

bool MapImpl::create_route(const char *uid)
{
	try{
		if(!strlen(uid))
			throw map_error(
				"invalid unique route identifier"
			);
		if(_routemap.count(uid))
			throw map_error(
				std::string(uid) + 
				" route identifier previously allocated"
			);
		std::pair< RouteMap::iterator, bool > result 
			  = _routemap.insert( std::make_pair( uid, new RouteDef ) );

		return result.second;

	}catch(map_error &e){
		Q_UNUSED(e);
	//	emit warning(e.what()):	
		return false;
	}
}

bool MapImpl::add_route_point( const char *id, double x, double y ) {
	try{
		if ( !_routemap.count(id) )
			throw map_error( std::string(id) + "route is not defined" );

		//LocationMapCIt it = _locationmap.find(loc);
		//if(it == _locationmap.end())
		//	throw map_error(
		//		std::string(loc) +
		//		" location is not defined"
		//	);
		_routemap[id]->push_back( Location( id, x, y, false ) );
		return true;
	}
	catch( map_error &e ) {
		Q_UNUSED(e);
	//	emit warning(e.what());
		return false;
	}
}

/*!
 *
 */
void MapImpl::create_sector( const char *uid ) {
	std::string id( uid );
	try {
		if ( !strlen( uid ) ) {
			throw map_error( "invalid unique sector identifier [ " + id + " ]" );
		}
		if ( _sectormap.count( uid ) ) {
			throw map_error( "sector identifier NOT unique [ " + id + " ]" );
		}

		std::pair< SectorMap::const_iterator, bool > result =
			_sectormap.insert( std::make_pair(
				id, SectorDefinition( uid )
			));

		if ( ! result.second ) {
			throw map_error( "cannot create sector [ " + id + " ]" );
		}
	}
	catch ( map_error &e ) {
	//	emit warning(e.what());
		throw e;
	}
}

/*!
 *
 */
void MapImpl::set_sector_active( const char *uid ) {
	validate( uid );

	if ( _active_sector ) 
			throw map_error( "only one active sector permitted" ); 

	_sectormap[uid].set_active();
	_active_sector = uid;
}

/*!
 *
 */
void MapImpl::add_sector_vertex( const char *id, double x, double y ) {
	validate( id );
	_sectormap[id].descriptors.push_back( new VertexDescriptor( x, y ) );
}

/*!
 *
 */
void MapImpl::add_sector_arc( const char *id, double x, double y, double r ) {
	validate( id );
	_sectormap[id].descriptors.push_back( new ArcDescriptor( x, y, r ) );
}

//-----------------------------------------------------------------------------

//
// private
//

/*!
 * Validate that the point (x,y) occurs within the map's defined region
 */
bool MapImpl::point_on_map(double east, double north){
	return !(
		east < _Emin || _Emax < east|| 
		north < _Nmin || _Nmax < north
	);
}

/*!
 * Confirm Sector has been defined
 */
void MapImpl::validate( const char *uid ) {
	try {
		if ( !_sectormap.count( uid ) ) {
			throw map_error( 
					"Sector is NOT defined [ " 
				  + std::string( uid ) + " ]!" 
			);
		}
	}
	catch ( map_error &e ) {
		throw e;
	}
}


//-----------------------------------------------------------------------------

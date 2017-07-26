/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: factory.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:49 $
      @state    : $State: Exp $

      $Log: factory.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:49  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.2  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.1  2006/08/15 11:48:46  seth
      aircraft agents initialized with performance data

      Revision 1.1  2006/08/14 06:12:13  seth
      implementing performance helper/wrapper factory


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "factory.h"
#include "exception.h"
#include "performance.h"


using namespace performance;

////////////////////////////////////////////////////////////////////////////////
//
// performance::Factory
//
//------------------------------------------------------------------------------
//
//

/*!
 *
 */
Factory * Factory::_instance = 0;

/*!
 *
 */
Factory::Factory( const std::auto_ptr< Data > data )
	  : _data( data )
{}
  
/*!
 *
 */
Factory::~Factory() {
	PerformanceMap::const_iterator it = _map.begin();
	for ( ; it != _map.end(); ++it ) {
		delete it->second;
	}
}
  
//------------------------------------------------------------------------------
//
//
  
/*!
 * Initialize the factory with the performance data
 *
 * The factory will take ownership of the performance data object.
 */
void Factory::initialize( const std::auto_ptr< Data > data ) {
	if ( _instance )
		throw performance_data_error( "factory already initialized" );

	_instance = new Factory( data );
}

void Factory::destroy() {
	if ( _instance ) delete _instance;
	_instance = 0;
}

/*!
 *
 */
const Performance * Factory::performance( const char *id ) {
	if ( ! _instance ) 
		throw performance_data_error( "factory not initialized" );

	PerformanceMap::iterator it = _instance->_map.find( id );
	if ( _instance->_map.end() != it ) {
		return it->second;
	}
	else {
		const PDT *type = _instance->_data->lookup_type( id );
		const PDO *data = _instance->_data->lookup_data( type->classRef );
		Performance* p = new Performance( type, data );
		_instance->_map[ id ] = p;
		return p;
	}
}

////////////////////////////////////////////////////////////////////////////////

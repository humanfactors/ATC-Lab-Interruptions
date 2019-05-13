/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: data.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2 $ )
      @date     : $Date: 2007/07/05 14:55:23 $
      @state    : $State: Exp $

      $Log: data.cpp,v $
      Revision 1.2  2007/07/05 14:55:23  Aaron Yeung
      Modified the method lookup_type to specify the ID when throwing a performance data error if the ID cannot be found

      Revision 1.1.1.2  2007/06/21 12:22:49  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.6  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.5  2006/09/04 04:47:10  seth
      fixed various problems with performance data + heading calcs

      Revision 1.4  2006/08/18 06:11:38  seth
      bug fixes

      Revision 1.3  2006/08/15 11:48:46  seth
      aircraft agents initialized with performance data

      Revision 1.2  2006/08/09 07:43:46  seth
      performance level data

      Revision 1.1  2006/08/07 06:22:46  seth
      implemtation of performance data



      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "data.h"
#include "exception.h"

#include <sstream>

using namespace performance;

////////////////////////////////////////////////////////////////////////////////
//
// performance::PDT
// 
//------------------------------------------------------------------------------
// [public] construction
//

PDT::PDT() {}

PDT::PDT( const PDT &rhs )
	  : type    ( rhs.type     )
	  ,	wtc     ( rhs.wtc      )
	  ,	syMSC   ( rhs.syMSC    )
	  ,	bnMSC   ( rhs.bnMSC    )
	  ,	classRef( rhs.classRef )
{}

PDT::~PDT() {}

////////////////////////////////////////////////////////////////////////////////
//
// performance::PDO
// 
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
PDO::PDO()
      : classNo( -1 )
      , lowHold( NAN )
      , midHold( NAN )
      , uppHold( NAN )
      , toSpeed( NAN )
      , toDuration( NAN )
      , toAltitude( NAN )
      , toDistance( NAN )
      , finSpeed( NAN )
      , xSection( NAN )
      , maxSpeed( NAN )
      , maxAltitude( NAN )
      , maxTurnSp( NAN )
      , is_mach( false )
      , is_jet( false )
      , stdTurnSp( NAN )
      , redTurn( NAN )
      , accTurn( NAN )
      , maxTurn( NAN )
      , redAccel( NAN )
      , accAccel( NAN )
      , maxAccel( NAN )
      , redDecel( NAN )
      , accDecel( NAN )
      , maxDecel( NAN )
      , redClimb( NAN )
      , accClimb( NAN )
      , maxClimb( NAN )
      , redDescend( NAN )
      , accDescend( NAN )
      , maxDescend( NAN )
      , levels( -1 )
      , altitude( new std::vector< double >() )
      , climbRate( new std::vector< double >() )
      , descendRate( new std::vector< double >() )
      , acceleration( new std::vector< double >() )
      , deceleration( new std::vector< double >() )
      , cruiseSpeed( new std::vector< double >() )
      , stallSpeed( new std::vector< double >() )
      , climbSpeed( new std::vector< double >() )
      , descendSpeed( new std::vector< double >() )
      , sidStar( L )
{}

/*!
 *
 */
PDO::PDO( const PDO &rhs )
      : classNo( rhs.classNo )
      , lowHold( rhs.lowHold )
      , midHold( rhs.midHold )
      , uppHold( rhs.uppHold )
      , toSpeed( rhs.toSpeed )
      , toDuration( rhs.toDuration )
      , toAltitude( rhs.toAltitude )
      , toDistance( rhs.toDistance )
      , finSpeed( rhs.finSpeed )
      , xSection( rhs.xSection )
      , maxSpeed( rhs.maxSpeed )
      , maxAltitude( rhs.maxAltitude )
      , maxTurnSp( rhs.maxTurnSp )
      , is_mach( rhs.is_mach )
      , is_jet( rhs.is_jet )
      , stdTurnSp( rhs.stdTurnSp )
      , redTurn( rhs.redTurn )
      , accTurn( rhs.accTurn )
      , maxTurn( rhs.maxTurn )
      , redAccel( rhs.redAccel )
      , accAccel( rhs.accAccel )
      , maxAccel( rhs.maxAccel )
      , redDecel( rhs.redDecel )
      , accDecel( rhs.accDecel )
      , maxDecel( rhs.maxDecel )
      , redClimb( rhs.redClimb )
      , accClimb( rhs.accClimb )
      , maxClimb( rhs.maxClimb )
      , redDescend( rhs.redDescend )
      , accDescend( rhs.accDescend )
      , maxDescend( rhs.maxDescend )
      , levels( rhs.levels )
      , altitude    ( new std::vector< double >( rhs.altitude->begin(),     rhs.altitude->end()     ) )
      , climbRate   ( new std::vector< double >( rhs.climbRate->begin(),    rhs.climbRate->end()    ) )
      , descendRate ( new std::vector< double >( rhs.descendRate->begin(),  rhs.descendRate->end()  ) )
      , acceleration( new std::vector< double >( rhs.acceleration->begin(), rhs.acceleration->end() ) )
      , deceleration( new std::vector< double >( rhs.deceleration->begin(), rhs.deceleration->end() ) )
      , cruiseSpeed ( new std::vector< double >( rhs.cruiseSpeed->begin(),  rhs.cruiseSpeed->end()  ) )
      , stallSpeed  ( new std::vector< double >( rhs.stallSpeed->begin(),   rhs.stallSpeed->end()   ) )
      , climbSpeed  ( new std::vector< double >( rhs.climbSpeed->begin(),   rhs.climbSpeed->end()   ) )
      , descendSpeed( new std::vector< double >( rhs.descendSpeed->begin(), rhs.descendSpeed->end() ) )
      , sidStar( rhs.sidStar )
{}


/*!
 *
 */
PDO::~PDO() {
	delete descendSpeed;
	delete climbSpeed;
	delete stallSpeed;
	delete cruiseSpeed;
	delete deceleration;
	delete acceleration;
	delete descendRate;
	delete climbRate;
	delete altitude;
}


////////////////////////////////////////////////////////////////////////////////
//
// performance::Data
// 
//------------------------------------------------------------------------------
// [public]
//

/*!
 *
 */
Data::Data() : _types(), _data() {}

/*!
 *
 */
Data::~Data() {}

//------------------------------------------------------------------------------
// [public] mutator interface
//

/*!
 *
 */
void Data::add_type( const PDT& ac_type ) {
	std::string key = ac_type.type;
	if ( 0 != _types.count( key ) ) {
		key.append( 
				L == ac_type.wtc ? "L"
			  : M == ac_type.wtc ? "M"
			  :                    "H"
		);

		if ( 0 != _types.count( key ) ) {
			throw performance_data_error( 
				std::string( "Type [" ) + ac_type.type + "] already exists" 
			);
		}
	}
	_types.insert( std::make_pair( key, ac_type ) );
}

/*!
 *
 */
void Data::add_performance( const PDO& data ) {
	if ( 0 != _data.count( data.classNo ) ) {
		std::ostringstream os;
		os 
			 << "Performance Data Object [ " 
			 << data.classNo 
			 << " ] already exists";
		throw performance_data_error( os.str() );
	}
	_data.insert( std::make_pair( data.classNo, data ) );
}

/*!
 *
 */
const PDT * Data::lookup_type( const std::string &id ) const {
	PDTMap::const_iterator it = _types.find( id );
	if ( _types.end() == it )
		throw performance_data_error( "illegal type id:" + id );
	return &it->second;
}

/*!
 *
 */
const PDO * Data::lookup_data( int id ) const {
	PDOMap::const_iterator it = _data.find( id );
	if ( _data.end() == it )
		throw performance_data_error( "illegal data id" );
	return &it->second;
}


//------------------------------------------------------------------------------

/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: builder.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:49 $
      @state    : $State: Exp $

      $Log: builder.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:49  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.7  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.6  2006/09/04 04:47:10  seth
      fixed various problems with performance data + heading calcs

      Revision 1.5  2006/08/24 04:12:51  seth
      seperating model from visuals

      Revision 1.4  2006/08/18 06:11:38  seth
      bug fixes

      Revision 1.3  2006/08/13 10:32:25  seth
      performance data structures built from file implemented

      Revision 1.2  2006/08/09 07:43:46  seth
      performance level data

      Revision 1.1  2006/08/07 06:22:46  seth
      implemtation of performance data



      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "builder.h"
#include "atmosphere.h"
#include "exception.h"
#include <sstream>

#ifdef WIN32
	#include <assert.h>
#endif

using namespace performance;

////////////////////////////////////////////////////////////////////////////////
//
// performance::Builder
//
//------------------------------------------------------------------------------
//
//

/*!
 *
 */
Builder::Builder() : _data( new Data ), _pdo( 0 ), _atmos( 0 ) {}

/*!
 *
 */
Builder::~Builder() {
	if ( _atmos ) delete _atmos;
	if ( _pdo   ) delete _pdo;
}


//------------------------------------------------------------------------------
//
//

/*!
 *
 */
void Builder::build_data( int id ) {
//	if ( _pdo ) delete _pdo;

	_pdo = new PDO();
	_pdo->classNo = id;

}

/*!
 *
 */
void Builder::build_type( 
		const std::string &id
	  , const std::string &wtc
	  , int syd, int bne
	  , int performance 
) {
	PDT ac_type;
	ac_type.type = std::string( id );
	ac_type.wtc = get_cat( wtc );
	ac_type.syMSC = syd;
	ac_type.bnMSC = bne;
	ac_type.classRef = performance;
	_data->add_type( ac_type );
}

/*!
 *
 */
void Builder::build_holding( double lower, double middle, double upper ) {
	_pdo->lowHold = lower;
	_pdo->midHold = middle;
	_pdo->uppHold = upper;
}

/*!
 *
 */
void Builder::build_takeoff( 
		double speed, double duration, double altitude, double distance 
) {
	_pdo->toSpeed = speed;
	_pdo->toDuration = duration;
	_pdo->toAltitude = altitude;
	_pdo->toDistance = distance / 10.0;
}

/*!
 *
 */
void Builder::build_landing( double speed ) {
	_pdo->finSpeed = speed;
}

/*!
 *
 */
void Builder::build_xsection( double xsect) {
	_pdo->xSection = xsect;
}

/*!
 *
 */
void Builder::build_maximums( 
		double speed, double altitude, double turn, bool mach, bool jet 
) {
	_pdo->maxSpeed = speed;
	_pdo->maxAltitude = altitude * 100.0;
	_pdo->maxTurnSp = turn / 10.0;
	_pdo->is_mach = mach;
	_pdo->is_jet = jet;
}

/*!
 *
 */
void Builder::build_turn_speed( double speed ) {
	_pdo->stdTurnSp = speed / 10.0;
}

/*!
 *
 */
void Builder::build_turn_rates( 
		double lower, double middle, double upper 
) {
	_pdo->redTurn = lower / 100.0;
	_pdo->accTurn = middle / 100.0;
	_pdo->maxTurn = upper / 100.0;
}

/*!
 *
 */
void Builder::build_acceleration( 
		double lower, double middle, double upper 
) {
	_pdo->redAccel = lower / 100.0;
	_pdo->accAccel = middle / 100.0;
	_pdo->maxAccel = upper / 100.0;
}

/*!
 *
 */
void Builder::build_decelerations( 
		double lower, double middle, double upper 
) {
	_pdo->redDecel = lower / 100.0;
	_pdo->accDecel = middle / 100.0;
	_pdo->maxDecel = upper / 100.0;
}

/*!
 *
 */
void Builder::build_climb_rates( 
		double lower, double middle, double upper 
) {
	_pdo->redClimb = lower / 100.0;
	_pdo->accClimb = middle / 100.0;
	_pdo->maxClimb = upper / 100.0;
}

/*!
 *
 */
void Builder::build_descend_rates( 
		double lower, double middle, double upper 
) {
	_pdo->redDescend = lower / 100.0;
	_pdo->accDescend = middle / 100.0;
	_pdo->maxDescend = upper / 100.0;
}

/*!
 *
 */
void Builder::build_level_count( int lvls) {
	_pdo->levels = lvls;
}

/*!
 *
 */
void Builder::build_altitude_levels( 
		const std::vector< double > &values 
) {
	construct_level_data( values, *_pdo->altitude );
	
	// convert 2 feet from 100's ft
	std::vector< double >::iterator val = _pdo->altitude->begin();
	for ( ; val != _pdo->altitude->end(); ++val ) {
		*val *= 100;
	}

	generate_atmospheric_params();
}

/*!
 *
 */
void Builder::build_climb_rate_levels( 
		const std::vector< double > &values 
) {
	construct_level_data( values, *_pdo->climbRate );
}

/*!
 *
 */
void Builder::build_descend_rate_levels( 
		const std::vector< double > &values 
) {
	construct_level_data( values, *_pdo->descendRate );
}

/*!
 *
 */
void Builder::build_acceleration_levels( 
		const std::vector< double > &values 
) {
	construct_level_data( values, *_pdo->acceleration );
}

/*!
 *
 */
void Builder::build_deceleration_levels( 
		const std::vector< double > &values 
) {
	construct_level_data( values, *_pdo->deceleration );
}

/*!
 *
 */
void Builder::build_cruising_speed_levels( 
		const std::vector< std::string > &values 
) {
	construct_level_velocities( values, *_pdo->cruiseSpeed );
}

/*!
 *
 */
void Builder::build_stall_speed_levels( 
		const std::vector< std::string > &values 
) {
	construct_level_velocities( values, *_pdo->stallSpeed );
}

/*!
 *
 */
void Builder::build_ascent_speed_levels( 
		const std::vector< std::string > &values 
) {
	construct_level_velocities( values, *_pdo->climbSpeed );
}

/*!
 *
 */
void Builder::build_descent_speed_levels( 
		const std::vector< std::string > &values 
) {
	construct_level_velocities( values, *_pdo->descendSpeed );
}

/*!
 *
 */
void Builder::build_sidstar( const std::string &sid ) {
	_pdo->sidStar = get_cat( sid );
}

/*!
 *
 */
void Builder::accept_performance() {
	// assert( false );
	// @todo: validation
	_data->add_performance( *_pdo );
	delete _pdo; 
	_pdo = 0;
}


//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 *
 */
LHM_CAT Builder::get_cat( const std::string &cat ) {
	return  ! cat.compare( "L" ) ? L 
		 :  ! cat.compare( "M" ) ? H
		 :                         M;
}

/*!
 *
 */
void Builder::construct_level_data( 
		const std::vector< double > &values, std::vector< double > &data 
) {
	assert( values.size() == _pdo->levels );
	std::vector< double >::const_iterator val = values.begin();
	for ( ; val != values.end(); ++val ) {
		data.push_back( *val );
	}
}

/*!
 *
 */
void Builder::construct_level_velocities( 
		const std::vector< std::string > &values, std::vector< double > &data 
) {
	for ( int lvl = 0; lvl < values.size(); ++lvl ) {
		std::istringstream is( values.at( lvl ) );
		char flg;
		double val;
		is >> flg >> val;
		data.push_back( convert2tas( lvl, flg, val ) );
	}
}

/*!
 *
 */
void Builder::generate_atmospheric_params() {
	assert( _pdo->levels > 0 );
	assert( _pdo->altitude->size() == _pdo->levels );

	if ( _atmos ) delete _atmos;

	_atmos = new Atmosphere( _pdo->levels, &(*_pdo->altitude)[0] );
}

/*!
 * Convert to true airspeed
 *
 * @param level needed for atmospheric parameter lookup
 * @param flag indicates either Mach or (N) corrected airspeed
 * @param value speed
 */
double Builder::convert2tas( int level, const char flag, double value ) {
	double retval;

	if ( 'N' == flag ) {
		retval = _atmos->cas2tas( level, value );
	}
	else if ( 'M' == flag ) {
		retval = _atmos->mach2tas( level, value / 100.0 );
	}
	else {
		std::ostringstream os;
		os
			 << "parser::invalid airspeed type [ " 
			 << flag << ":" << value 
			 << " ] at level [ "
			 << level << " ]";
		throw performance_data_error( os.str() );
	}

	return retval;
}

////////////////////////////////////////////////////////////////////////////////

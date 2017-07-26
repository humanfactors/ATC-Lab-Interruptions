/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: parser.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:50 $
      @state    : $State: Exp $

      $Log: parser.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:50  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.4  2006/08/24 04:12:51  seth
      seperating model from visuals

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

#include "parser.h"
#include "builder.h"
#include "exception.h"

#include <sstream>
#include <vector>

using namespace performance;

////////////////////////////////////////////////////////////////////////////////
//
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
Parser::Parser( std::istream &instream, Builder &builder ) 
	  : _instream( instream ), _builder( builder ) 
{}

/*!
 *
 */
Parser::~Parser() {}


//------------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
void Parser::parse() {

    std::string record;               // data record
	std::string tmpstr;               // temporary string storage
    InputState  state    = CLASS_NO;  // state machine 
    int         class_no = -1;        // performance date unique id
	int         lvls     =  0;        // caount of level bands

    // process input stream
    while ( std::getline( _instream, record ) ) {
    
        // skip blank lines
        if ( record.empty() ) continue;
    
        // process record
        std::istringstream is( record );

        switch ( state ) {

        case CLASS_NO: {
            is >> tmpstr >> class_no;
            _builder.build_data( class_no );
            state = AC_TYPE;
            break;
        }
        case AC_TYPE: {
			std::string ac_type;
            is >> ac_type;
            if ( ! ac_type.compare( "*" ) ) {
                state = HOLDING_SPEEDS;
            }
            else {
				std::string wtc;
				int syn;
				int bne;
                is >> wtc >> syn >> bne;
                _builder.build_type( 
					ac_type.c_str(), wtc.c_str(), syn, bne, class_no 
				);
            }
            break;
        }
        case HOLDING_SPEEDS: {
			process_range( is, &Builder::build_holding );
            state = TAKEOFF_PARAMS;
            break;
        }
        case TAKEOFF_PARAMS: {
			process_takeoff( is, &Builder::build_takeoff );
            state = LANDING_SPEED;
            break;
        }
        case LANDING_SPEED: {
			process_double( is, &Builder::build_landing );
            state = RADAR_XSECTION;
            break;
        }
        case RADAR_XSECTION: {
			process_double( is, &Builder::build_xsection );
            state = MAXIMUM_PARAMS;
            break;
        }
        case MAXIMUM_PARAMS: {
			double      speed, alt, turn;
			std::string mach, jet;
            is >> speed >> alt >> turn >> mach >> jet;
            _builder.build_maximums( speed, alt, turn, "Y" == mach, "Y" == jet );
            state = STD_TURN_SPEED;
            break;
        }
        case STD_TURN_SPEED: {
			process_double( is, &Builder::build_turn_speed );
            state = TURN_RATES;
            break;
        }
        case TURN_RATES: {
			process_range( is, &Builder::build_turn_rates );
            state = ACCELERATIONS;
            break;
        }
        case ACCELERATIONS: {
			process_range( is, &Builder::build_acceleration );
            state = DECELERATIONS;
            break;
        }
        case DECELERATIONS: {
			process_range( is, &Builder::build_decelerations );
            state = CLIMB_RATES;
            break;
        }
        case CLIMB_RATES: {
			process_range( is, &Builder::build_climb_rates );
            state = DESCEND_RATES;
            break;
        }
        case DESCEND_RATES: {
			process_range( is, &Builder::build_descend_rates );
            state = LEVELS;
            break;
        }
        case LEVELS: {
            is >> lvls;
            _builder.build_level_count( lvls );
            state = LVL_ALTITUDES;
            break;
        }
        case LVL_ALTITUDES: {
			process_doubles( is, lvls, &Builder::build_altitude_levels );
            state = LVL_CLIMB_RATES;
            break;
        }
        case LVL_CLIMB_RATES: {
			process_doubles( is, lvls, &Builder::build_climb_rate_levels );
            state = LVL_DESCEND_RATES;
            break;
        }
        case LVL_DESCEND_RATES: {
			process_doubles( is, lvls, &Builder::build_descend_rate_levels );
            state = LVL_ACCELERATIONS;
            break;
        }
        case LVL_ACCELERATIONS: {
			process_doubles( is, lvls, &Builder::build_acceleration_levels );
            state = LVL_DECERATIONS;
            break;
        }
        case LVL_DECERATIONS: {
			process_doubles( is, lvls, &Builder::build_deceleration_levels );
            state = LVL_CRUISING_SPEEDS;
            break;
        }
        case LVL_CRUISING_SPEEDS: {
			process_strings( is, lvls, &Builder::build_cruising_speed_levels );
            state = LVL_STALL_SPEEDS;
            break;
        }
        case LVL_STALL_SPEEDS: {
			process_strings( is, lvls, &Builder::build_stall_speed_levels );
            state = LVL_CLIMBING_SPEEDS;
            break;
        }
        case LVL_CLIMBING_SPEEDS: {
			process_strings( is, lvls, &Builder::build_ascent_speed_levels );
            state = LVL_DESCENDING_SPEEDS;
            break;
        }
        case LVL_DESCENDING_SPEEDS: {
			process_strings( is, lvls, &Builder::build_descent_speed_levels );
            state = SID_STAR;
            break;
        }
        case SID_STAR: {
			process_string( is, &Builder::build_sidstar );
			_builder.accept_performance();
            state = CLASS_NO;
            break;
        }
        }
    }
}

//------------------------------------------------------------------------------
// [private] implemenation
//

/*!
 *
 */
void Parser::process_double(
		std::istream &is
	  , void ( Builder::*buildfn )( double ) 
){
	double value;
    is >> value;
    ( _builder.*buildfn) ( value );
}

/*!
 *
 */
void Parser::process_doubles(
		std::istream &is
	  , const int count
	  , void (Builder::*buildfn)( const std::vector< double > & ) 
){
	std::vector< double > fields( count );
	for ( int i = 0; i < count; ++i ) {
		double val;
		is >> val;
		fields[ i ] = val;
	}
	( _builder.*buildfn )( fields );
}

/*!
 *
 */
void Parser::process_range( 
		std::istream &is
	  , void ( Builder::*buildfn )( double, double, double ) 
){
	double low, mean, high;
    is >> low >> mean >> high;
    ( _builder.*buildfn )( low, mean, high );
}

/*!
 *
 */
void Parser::process_string(
		std::istream &is
	  , void ( Builder::*buildfn )( const std::string & ) 
){
	std::string value;
    is >> value;
    ( _builder.*buildfn )( value.c_str() );
}

/*!
 *
 */ 
void Parser::process_strings(
		std::istream &is
	  , const int count
	  , void ( Builder::*buildfn )( const std::vector< std::string > & )
){
	std::vector< std::string >fields( count );
	for ( int i = 0; i < count; ++i ) {
		std::string strval;
		is >> strval;
		fields[ i ] = strval;
	}
	( _builder.*buildfn )( fields );
}

/*!
 *
 */
void Parser::process_takeoff(
		std::istream &is
	  , void ( Builder::*buildfn )( double, double, double, double ) 
){
	double speed, duration, altitude, distance;
    is >> speed >> duration >> altitude >> distance;
    ( _builder.*buildfn )( speed, duration, altitude, distance );
}


////////////////////////////////////////////////////////////////////////////////

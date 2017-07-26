/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: performance.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:50 $
      @state    : $State: Exp $

      $Log: performance.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:50  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.7  2006/09/18 12:41:07  seth
      refactored aircraft agent state updates - blink is causing access violation!

      Revision 1.6  2006/09/17 12:45:35  seth
      implementing solution mechanisms

      Revision 1.5  2006/09/04 04:47:10  seth
      fixed various problems with performance data + heading calcs

      Revision 1.4  2006/08/24 04:12:51  seth
      seperating model from visuals

      Revision 1.3  2006/08/21 06:21:43  seth
      fixing bad performance data cruise speed

      Revision 1.2  2006/08/18 06:11:38  seth
      bug fixes

      Revision 1.1  2006/08/15 11:48:46  seth
      aircraft agents initialized with performance data


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "performance.h"
#include "data.h"
#include "exception.h"

#include <sstream>

#ifdef WIN32
	#include <assert.h>
#endif


using namespace performance;

////////////////////////////////////////////////////////////////////////////////
//
// performance::performance
//
//------------------------------------------------------------------------------
//
//

/*!
 *
 */
Performance::Performance( const PDT *type, const PDO *data )
	  : _type( type ), _data( data ) {}

/*!
 *
 */
Performance::~Performance() {}


//------------------------------------------------------------------------------
//
//

/*!
 *
 */
double Performance::cruising_altitude() const {
	throw performance_data_error( 
		"data does not contain cruising altitude information" 
	);
}

/*!
 * Calculate the throttle setting from current cruise speed.
 *
 * The throttle setting is in the range -1.0 to +1.0 where a throttle setting
 * of +1.0 corresponds to the maximum cruise speed, a throttle setting of 0.0
 * corrensponds to the standard cruising speed and -1.0 corresponds to the
 * minimum cruise speed. The throttle is concidered to give linear performance
 * between 0.0 and +/- 1.0 but the +ve and negative parts may not have the same
 * slope.
 *
 * @param z the cuise velocity is altitude dependant
 * @param v the current cruise velocity
 */
double Performance::throttle_setting( double z, double v ) const {
	double min = cruise_speed_minimum( z );
	double std = cruise_speed_standard( z );
	double max = cruise_speed_maximum( z );

	assert( min <= std && std <= max );

	if ( v < min || max < v ) {
		std::ostringstream oss;
		oss 
			 << "invalid cruise speed: " 
			 << "Type( " << _type->type  << " ) "
			 << "@( " << z << " ) "
			 << "!( " << min << " <= " << v << " <= " << max << " )!" 
			 << std::ends;
		throw performance_data_error( oss.str() );
	}

	return 
			(int)( std ) == (int)( v ) ? 0.0 
		  :        std   <         v   ? ( v - std ) / ( max - std )
		  :     /* std   >         v */  ( v - std ) / ( std - min )
		  ;
}

/*!
 *
 */
double Performance::cruise_speed( double z, double t ) const {
	return adjusted_level_value( 
			z, t
		  , &Performance::cruise_speed_standard
		  , &Performance::cruise_speed_minimum
		  , &Performance::cruise_speed_maximum 
	);
}

/*!
 *
 */
double Performance::acceleration( double z, double t ) const {
	double a = acceleration_standard( z );
	return adjusted_value( 
			t, a
		  , reduced_acceleration_factor() * a
		  , maximum_acceleration_factor() * a
	);
}

/*!
 *
 */
double Performance::deceleration( double z, double t ) const {
	double a = deceleration_standard( z );
	return adjusted_value( 
			t, a
		  , reduced_deceleration_factor() * a
		  , maximum_deceleration_factor() * a
	);
}

/*!
 *
 */
double Performance::climb_speed( double z, double t  ) const {
	return adjusted_level_value( 
			z, t
		  , &Performance::climb_speed_standard
		  , &Performance::climb_speed_minimum
		  , &Performance::climb_speed_maximum 
	);
}

/*!
 * @todo reduced:standard:accelerated:maximum
 */
double Performance::climb_rate( double z, double t  ) const {
	return adjusted_level_value( 
			z, t
		  , &Performance::climb_rate_standard
		  , &Performance::climb_rate_reduced
		  , &Performance::climb_rate_maximum 
	);
}

/*!
 *
 */
double Performance::descent_speed( double z, double t  ) const {
	return adjusted_level_value( 
			z, t
		  , &Performance::descent_speed_standard
		  , &Performance::descent_speed_minimum
		  , &Performance::descent_speed_maximum 
	);
}

/*!
 * @todo reduced:standard:accelerated:maximum
 */
double Performance::descent_rate( double z, double t  ) const {
	return adjusted_level_value( 
			z, t
		  , &Performance::descent_rate_standard
		  , &Performance::descent_rate_reduced
		  , &Performance::descent_rate_maximum 
	);
}


//------------------------------------------------------------------------------
//
//

/*!
 *
 */
double Performance::ceiling() const {
	return _data->maxAltitude;
}

/*!
 *
 */
double Performance::reduced_acceleration_factor() const { 
	return _data->redAccel; 
}
double Performance::maximum_acceleration_factor() const { 
	return _data->maxAccel; 
}
double Performance::reduced_deceleration_factor() const { 
	return _data->redDecel; 
}
double Performance::maximum_deceleration_factor() const { 
	return _data->maxDecel; 
}


/*!
 *
 */
double Performance::acceleration_standard( double ft ) const {
	return calc_level_value( ft, *_data->altitude, *_data->acceleration );
}

/*!
 *
 */
double Performance::deceleration_standard( double ft ) const {
	return calc_level_value( ft, *_data->altitude, *_data->deceleration );
}



/*!
 *
 */
double Performance::climb_speed_minimum( double ft ) const {
	return cruise_speed_minimum( ft );
}

/*!
 *
 */
double Performance::climb_speed_standard( double ft ) const {
	return calc_level_value( ft, *_data->altitude, *_data->climbSpeed );
}

/*!
 *
 */
double Performance::climb_speed_maximum( double ft ) const {
	return cruise_speed_maximum( ft );
}

/*!
 *
 */
double Performance::cruise_speed_minimum( double ft ) const {
	return calc_level_value( ft, *_data->altitude, *_data->stallSpeed );
}

/*!
 *
 */
double Performance::cruise_speed_standard( double ft ) const {
	return calc_level_value( ft, *_data->altitude, *_data->cruiseSpeed );
}

/*!
 * Maximum cruising airspeed at an altitude
 *
 * @todo: this information is available in the original source data but was
 * @todo: not extracted and made available in this implementation of the 
 * @todo: performance data. The maximum speed implemented breaks the invariant
 * @todo: condition:
 * @todo:         min_speed <= standard_speed <= max_speed.
 * @todo: A work around has been implemented where: 
 * @todo:         |min_speed - standard_speed| == |max_speed - standard_speed|
 */
double Performance::cruise_speed_maximum( double ft ) const {
	// return _data->maxSpeed; - broken implementation

	double min = cruise_speed_minimum( ft );
	double std = cruise_speed_standard( ft );

	return std + ( std - min );
}

/*!
 *
 */
double Performance::descent_speed_minimum( double ft ) const {
	return cruise_speed_minimum( ft );
}

/*!
 *
 */
double Performance::descent_speed_standard( double ft ) const {
	return calc_level_value( ft, *_data->altitude, *_data->descendSpeed );
}
/*!
 *
 */
double Performance::descent_speed_maximum( double ft ) const {
	return cruise_speed_maximum( ft );
}

/*!
 *
 */
double Performance::climb_rate_reduced( double ft ) const {
	return _data->redClimb * climb_rate_standard( ft );
}
/*!
 *
 */
double Performance::climb_rate_standard( double ft ) const {
	return calc_level_value( ft, *_data->altitude, *_data->climbRate );
}

/*!
 *
 */
double Performance::climb_rate_accelerated( double ft ) const {
	return _data->accClimb * climb_rate_standard( ft );
}

/*!
 *
 */
double Performance::climb_rate_maximum( double ft ) const {
	return _data->maxClimb * climb_rate_standard( ft );
}

/*!
 *
 */
double Performance::descent_rate_reduced( double ft ) const {
	return _data->redDescend * descent_rate_standard( ft );
}

/*!
 *
 */
double Performance::descent_rate_standard( double ft ) const {
	return calc_level_value( ft, *_data->altitude, *_data->descendRate );
}

/*!
 *
 */
double Performance::descent_rate_accelerated( double ft ) const {
	return _data->accDescend * descent_rate_standard( ft );
}

/*!
 *
 */
double Performance::descent_rate_maximum( double ft ) const {
	return _data->maxDescend * descent_rate_standard( ft );
}


/*!
 *
 */
std::pair< double, double > 
Performance::time_distance( double ft0, double ft1 ) const {
    throw;
}

//------------------------------------------------------------------------------
// [private]
//

/*!
 * Retrieve altitude layer value
 *
 * @param z altitude in feet
 * @param alts altitude layer boundaries
 * @param vals values assigned to layer boundaries
 */
double Performance::calc_level_value( 
        double z, const LevelData &alts, const LevelData &vals 
) const {
    int lvl = 0;

    LevelData::const_iterator pos = alts.begin();
    for ( ; pos != alts.end(); ++pos ) {
		if ( z < alts[ lvl ] ) break;
        ++lvl;
    }

    if ( alts.begin() == pos ) {
        return vals.front();
    }
    else if ( alts.end() == pos ) {
        return vals.back();
    }
    else {
        double lAlt = alts.at( lvl - 1 );
        double hAlt = alts.at( lvl     );
        double lVal = vals.at( lvl - 1 );
        double hVal = vals.at( lvl     );

        double alpha = ( z - lAlt ) / ( hAlt - lAlt );

        return ( 1.0 - alpha ) * lVal + alpha * hVal;
    }
}

/*!
 *
 */
double Performance::adjusted_level_value( 
		double z, double t 
	  , double ( Performance::*standard_value )( double ) const
	  , double ( Performance::*minimum_value )( double ) const
	  , double ( Performance::*maximum_value )( double ) const
) const {
	double std = ((*this).*(standard_value))( z );
	if ( 0 == (int)(t * 1000) ) {
		return std;
	}
	else if ( t > 0.0 ) {
		return std + t * ( ((*this).*(maximum_value))( z ) - std );
	}
	else {
		return std - t * ( ((*this).*(minimum_value))( z ) - std );
	}
}


/*!
 *
 */
double Performance::adjusted_value( 
		double t, double std, double min, double max 
) const {
	if      ( 0 == (int)(t * 1000) ) { return std; }
	else if ( t > 0.0 )              { return std + t * ( max - std ); }
	else                             { return std - t * ( min - std ); }
}

////////////////////////////////////////////////////////////////////////////////

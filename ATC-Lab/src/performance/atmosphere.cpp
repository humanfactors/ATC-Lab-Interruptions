/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: atmosphere.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:49 $
      @state    : $State: Exp $

      $Log: atmosphere.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:49  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.1  2006/08/13 10:32:25  seth
      performance data structures built from file implemented


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "atmosphere.h"
#include <math.h>

using namespace performance;

////////////////////////////////////////////////////////////////////////////////
//
// performance::Atmosphere
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
Atmosphere::Atmosphere( int levels, const double *altitudes ) 
	  : _params(levels) 
{
	for ( int i = 0; i < levels; ++i ) {
		calc_param( altitudes[ i ], _params[ i ] );
	}
}

/*!
 *
 */
Atmosphere::~Atmosphere() {}


//------------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
double Atmosphere::cas2tas( int level, double cas ) const {
	return mach2tas( level, cas2mach( level, cas ) );
}

/*!
 *
 */
double Atmosphere::cas2mach( int level, double cas ) const {
	double retval;

    retval = pow( 1.0 + cas * cas / ( A0 * A0 * 5.0 ), 3.5 );
    retval = pow( (retval - 1.0) / _params[ level ].delta + 1.0, 2.0 / 7.0 );
	retval = sqrt( 5.0 * ( retval - 1.0 ) );
    
	return retval;
}

/*!
 *
 */
double Atmosphere::mach2tas( int level, double mach ) const {
	return mach * _params[ level ].a;
}

//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 *
 */
void Atmosphere::calc_param( double alt, AtmosParam &param ) {
	double t;   // temprature
	double p;	// pressure

    // Troposphere
    if ( alt <= 36152.0 ) {
		t = T0 - 3.56 * alt/1000.0;
		p = P0 * pow( t/T0, 5.256 );
    }
    // Stratosphere
	else if ( alt <= 82345.0) {
		t = 389.98;
		p = P0 * 0.2236 * exp( (36000.0 - alt) / (53.35 * 389.98) );
	}
	// Mesosphere (& Thermosphere ?)
    else {          
		t = 389.98 + 1.645 * (alt-82345.0)/1000.0;
		p = P0 * 0.02456 * pow( t/389.98, -11.388 );
	}
    
	param.alt = alt;
	param.t   = t;
	param.p   = p;

	param.a  = sqrt( GAMMA * R * t );       // in ft/sec
	param.a *= 3600.0 * MPERFT / MPERNM;	// in knots

	param.rho   = p / ( R * t );
	param.delta = p / P0;

}


////////////////////////////////////////////////////////////////////////////////

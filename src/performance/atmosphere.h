/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: atmosphere.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:49 $
      @state    : $State: Exp $

      $Log: atmosphere.h,v $
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

#ifndef __PERFORMANCE_ATMOSPHERE_HDR__
#define __PERFORMANCE_ATMOSPHERE_HDR__

#include "data.h"

namespace performance {

	//
	// performance::constants
	//

	const double MPERNM = 1852.0;        // meters / nautical mile
	const double MPERFT = 0.3048;        // meters / foot

	// Data for air ...

	const double R     = 1718.0;     // gas constant R, in ft2/sec2/degF
	const double GAMMA = 1.4;        // gamma, ratio of specific heats

	// Data for standard day @ sea level ...

	const double T0   = 518.67;         // absolute temperature, deg F
	const double P0   = 2116.2;         // pressure, lb/ft2
	const double RHO0 = 0.002378;       // density, slugs/ft3
	const double A0   = 661.7556275;    // speed of sound, Kt


	//
	// performance::AtmosParam
	//

	struct AtmosParam {

		double alt;     // altitude, in feet
		double t;       // temperature
		double p;       // pressure
		double rho;     // density
		double a;       // speed of sound
		double delta;   // p/p0

	};

    //
    // performance::Atmosphere
    //

    class Atmosphere {

    public:
        Atmosphere( int, const double * );
        ~Atmosphere();

    public:
		double cas2mach( int, double ) const;
		double  cas2tas( int, double ) const;
		double mach2tas( int, double ) const;

	private:
		void calc_param( double, AtmosParam & );

	private:
		std::vector< AtmosParam > _params;

	};


};
#endif

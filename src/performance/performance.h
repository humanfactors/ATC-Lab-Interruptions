/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: performance.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:50 $
      @state    : $State: Exp $

      $Log: performance.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:50  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.4  2006/09/18 12:41:07  seth
      refactored aircraft agent state updates - blink is causing access violation!

      Revision 1.3  2006/09/04 04:47:10  seth
      fixed various problems with performance data + heading calcs

      Revision 1.2  2006/08/21 06:21:44  seth
      fixing bad performance data cruise speed

      Revision 1.1  2006/08/15 11:48:46  seth
      aircraft agents initialized with performance data


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __PERFORMANCE_PERFORMANCE_HDR__
#define __PERFORMANCE_PERFORMANCE_HDR__

#ifdef WIN32
	#pragma warning( disable : 4786 )
#endif

#include <utility>
#include <vector>

namespace performance {

	//
	// performance::PDT
	// performance::PDO
	//
	struct PDT;
	struct PDO;


    //
    // performance::performance
    //
    class Performance {

    public:
        Performance( const PDT *, const PDO * );
        ~Performance();

    public:
		double cruising_altitude() const;

		double throttle_setting( double, double ) const;
		double cruise_speed    ( double, double ) const;
		double acceleration    ( double, double ) const;
		double deceleration    ( double, double ) const;
		double climb_speed     ( double, double ) const;
		double climb_rate      ( double, double ) const;
		double descent_speed   ( double, double ) const;
		double descent_rate    ( double, double ) const;

        double ceiling() const;

		double reduced_acceleration_factor() const;
		double maximum_acceleration_factor() const;
		double reduced_deceleration_factor() const;
		double maximum_deceleration_factor() const;

		double acceleration_standard   ( double ) const;
		double deceleration_standard   ( double ) const;
        double climb_speed_minimum     ( double ) const;
        double climb_speed_standard    ( double ) const;
        double climb_speed_maximum     ( double ) const;
        double cruise_speed_minimum    ( double ) const;
        double cruise_speed_standard   ( double ) const;
        double cruise_speed_maximum    ( double ) const;
        double descent_speed_minimum   ( double ) const;
        double descent_speed_standard  ( double ) const;
        double descent_speed_maximum   ( double ) const;
        double climb_rate_reduced      ( double ) const;
        double climb_rate_standard     ( double ) const;
        double climb_rate_accelerated  ( double ) const;
        double climb_rate_maximum      ( double ) const;
        double descent_rate_reduced    ( double ) const;
        double descent_rate_standard   ( double ) const;
        double descent_rate_accelerated( double ) const;
        double descent_rate_maximum    ( double ) const;

        std::pair< double, double > time_distance( double, double ) const;

	private:
		double calc_level_value( 
				double
			  , const std::vector< double > &
			  , const std::vector< double > & 
		) const;

		double adjusted_level_value( 
				double, double 
			  , double ( Performance::* )( double ) const
			  , double ( Performance::* )( double ) const
			  , double ( Performance::* )( double ) const
		) const;

		double adjusted_value( double, double, double, double ) const;


    private:
        const PDT *_type;
        const PDO *_data;
	};

};

#endif

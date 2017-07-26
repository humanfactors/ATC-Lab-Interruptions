/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: builder.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:49 $
      @state    : $State: Exp $

      $Log: builder.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:49  Aaron Yeung
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

#ifndef __PERFORMANCE_BUILER_HDR__
#define __PERFORMANCE_BUILER_HDR__

#pragma warning( disable : 4786 )

#include "data.h"
#include <memory>

namespace performance {

	//
	//
	//

	class Atmosphere;


    //
    // performance::Builder
    //

    class Builder {

    public:
        Builder();
        ~Builder();

    public:
		void build_data( int );

		void build_type( 
			const std::string &, const std::string &, int, int, int
		);

		void build_holding( double, double, double );
		void build_takeoff( double, double, double, double );
		void build_landing( double );
		void build_xsection( double );
		void build_maximums( double, double, double, bool, bool );
		void build_turn_speed( double );
		void build_turn_rates( double, double, double );
		void build_acceleration( double, double, double );
		void build_decelerations( double, double, double );
		void build_climb_rates( double, double, double );
		void build_descend_rates( double, double, double );

		void build_level_count( int );

		void build_altitude_levels( const std::vector< double > & );
		void build_climb_rate_levels( const std::vector< double > & );
		void build_descend_rate_levels( const std::vector< double > & );
		void build_acceleration_levels( const std::vector< double > & );
		void build_deceleration_levels( const std::vector< double > & );

		void build_cruising_speed_levels( const std::vector< std::string > & );
		void build_stall_speed_levels( const std::vector< std::string > & );
		void build_ascent_speed_levels( const std::vector< std::string > & );
		void build_descent_speed_levels( const std::vector< std::string > & );

		void build_sidstar( const std::string & );

		void accept_performance();

    public:
		const std::auto_ptr< Data > data() { return _data; }

	private:
		LHM_CAT get_cat( const std::string & );
		void construct_level_data( 
				const std::vector< double > &, std::vector< double > & 
		); 
		void construct_level_velocities( 
				const std::vector< std::string > &, std::vector< double > & 
		); 
		void generate_atmospheric_params();
		double convert2tas( int, const char, double );

    private:
		std::auto_ptr< Data >     _data;
		PDO                      *_pdo;		// current PDO
		Atmosphere               *_atmos;
    
	};


};
#endif

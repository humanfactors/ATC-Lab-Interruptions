/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: parser.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:50 $
      @state    : $State: Exp $

      $Log: parser.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:50  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.5  2006/08/24 04:12:51  seth
      seperating model from visuals

      Revision 1.4  2006/08/15 11:48:46  seth
      aircraft agents initialized with performance data

      Revision 1.3  2006/08/14 06:12:13  seth
      implementing performance helper/wrapper factory

      Revision 1.2  2006/08/09 07:43:46  seth
      performance level data

      Revision 1.1  2006/08/07 06:22:46  seth
      implemtation of performance data



      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __PERFORMANCE_DATA_PARSER_HDR__
#define __PERFORMANCE_DATA_PARSER_HDR__

#ifdef WIN32
	#pragma warning( disable : 4786 )
#endif


#include <iostream>
#include <vector>


namespace performance {

    //
    // performance::Builder
    //
    class Builder;

	//
	// performance::InputStatus
	//
	enum InputState {
		    CLASS_NO
		  , AC_TYPE
		  , HOLDING_SPEEDS
		  , TAKEOFF_PARAMS
		  , LANDING_SPEED
		  , RADAR_XSECTION
		  , MAXIMUM_PARAMS
		  , STD_TURN_SPEED
		  , TURN_RATES
		  , ACCELERATIONS
		  , DECELERATIONS
		  , CLIMB_RATES
		  , DESCEND_RATES
		  , LEVELS
		  , LVL_ALTITUDES
		  , LVL_CLIMB_RATES
		  , LVL_DESCEND_RATES
		  , LVL_ACCELERATIONS
		  , LVL_DECERATIONS
		  , LVL_CRUISING_SPEEDS
		  , LVL_STALL_SPEEDS
		  , LVL_CLIMBING_SPEEDS
		  , LVL_DESCENDING_SPEEDS
		  , SID_STAR
	};

    //
    // Parser
    //
    class Parser {

    public:
        Parser( std::istream &, Builder & );
		~Parser();

    public:
        void parse();

	private:
		// generic
		void Parser::process_double(
				std::istream &
			  , void (Builder::*)( double ) 
		);
		void process_doubles(
				std::istream &
			  , const int
			  , void (Builder::*)( const std::vector< double > & ) 
		);
		void process_range( 
				std::istream &
			  , void (Builder::*)( double, double, double ) 
		);
		void process_string(
				std::istream &
			  , void (Builder::*)( const std::string & ) 
		);
		void process_strings(
				std::istream &
			  , const int
			  , void (Builder::*)( const std::vector< std::string > & ) 
		);

		// specific
		void process_takeoff(
				std::istream &is
			  , void (Builder::*)( double, double, double, double ) 
		);

	private:
		std::istream &_instream;
		Builder      &_builder;

    };
        


};
#endif

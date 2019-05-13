/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: data.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:49 $
      @state    : $State: Exp $

      $Log: data.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:49  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.5  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.4  2006/08/31 06:15:11  seth
      implemented clock

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

#ifndef __PERFORMANCE_DATA_OBJECT_HDR__
#define __PERFORMANCE_DATA_OBJECT_HDR__

#pragma warning( disable : 4786 )

#include <limits>
#include <map>
#include <string>
#include <vector>

namespace performance {

	//
	//
	//
	const double NAN = std::numeric_limits<double>::quiet_NaN();


    //
    // wake turbulance category
    //

    enum LHM_CAT { L, M, H };


	//
	// performance::PDT
	//

	struct PDT {

		PDT();
		PDT( const PDT & );
		~PDT();

		std::string type;        // aircraft type
		LHM_CAT     wtc;         // wake turbulence category, L/M/H
		int         syMSC;       // Sydney MAESTRO speed class, 0-10
		int         bnMSC;       // Brisbane MAESTRO speed class, 0-10

		int         classRef;    // class number, 0-99

	};


	//
	// performance::PDO
	//

	struct PDO {

		PDO();
		PDO( const PDO & );
		~PDO();

		int    classNo;     // class number, 0-99

		double lowHold;     // low level (?<6000) holding speed, Kt true
		double midHold;     // mid level ...
		double uppHold;     // upper level (?>14000) ...

		double toSpeed;     // takeoff speed, Kt true
		double toDuration;  // takeoff duration, Sec
		double toAltitude;  // takeoff altitude, ft
		double toDistance;  // takeoff distance, NM

		double finSpeed;    // final landing speed, Kt true
		double xSection;    // (radar) cross section, m^2 0-1000

		double maxSpeed;    // maximum speed, Kt true
		double maxAltitude; // maximum altitude, ft
		double maxTurnSp;   // maximum turn speed, deg/Sec

        bool is_mach;		// is Mach capable
        bool is_jet;		// is jet

		double stdTurnSp;   // standard turn speed, deg/Sec

		double redTurn;     // reduced turn rate, ratio
		double accTurn;     // accelerated ...
		double maxTurn;     // maximum ...

		double redAccel;    // reduced accelaration, ratio
		double accAccel;    // accelerated ...
		double maxAccel;    // maximum ...

		double redDecel;    // reduced deceleration, ratio
		double accDecel;    // accelerated ...
		double maxDecel;    // maximum ...

		double redClimb;    // reduced climb rate, ratio
		double accClimb;    // accelerated ...
		double maxClimb;    // maximum ...

		double redDescend;  // reduced descend rate, ratio
		double accDescend;  // accelerated ...
		double maxDescend;  // maximum ...

		int    levels;      // number of performance levels, 1-10

		std::vector< double > *altitude;         // layer altitudes, ft

		std::vector< double > *climbRate;        // climb rate, ft/Min
		std::vector< double > *descendRate;      // descend rate, ft/Min
		std::vector< double > *acceleration;     // acceleration, Kt/Min
		std::vector< double > *deceleration;     // deceleration, Kt/Min

		std::vector< double > *cruiseSpeed;      // cruising speed, Kt true
		std::vector< double > *stallSpeed;       // stall speed, Kt true
		std::vector< double > *climbSpeed;       // speed climbing, Kt true
		std::vector< double > *descendSpeed;     // speed descending, Kt true 

		LHM_CAT  sidStar;          // SID/STAR category, H/M/L
    };

    //
    //
    //

    typedef std::vector< double > LevelData;
	//
	// performance::Data
	// 

	class Data {

	public:
		Data();
		~Data();

		friend class Writer;

	public:
		void add_type( const PDT& );
		void add_performance( const PDO& );

		const PDT * lookup_type( const std::string & ) const;
		const PDO * lookup_data( int ) const;

	private:
		std::map< std::string, PDT > _types;
		std::map<         int, PDO > _data;

	};

	//
	//
	//
	typedef const std::auto_ptr< Data >  DataCPtr;
	typedef std::map< std::string, PDT > PDTMap;
	typedef std::map< int, PDO >         PDOMap;


};

#endif


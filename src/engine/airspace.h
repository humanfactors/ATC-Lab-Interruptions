/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: airspace.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.5.6.1 $ )
      @date     : $Date: 2014/12/10 13:26:18 $
      @state    : $State: Exp $

      $Log: airspace.h,v $
      Revision 1.5.6.1  2014/12/10 13:26:18  Aaron Yeung
      ATC Version 2.4.5.8:

      - Implemented scriptable interruption task with configurable start time, end time, automatic aircraft acceptance,
      	    automatic aircraft handoff and the displaying of a full black screen
                *******************************************************************************************************************
                XML Element		Attributes  		Description
                *******************************************************************************************************************
      	  atc:interruption	atc:start		Trial time when the interruption begins
      				atc:end			Trial time when the interruption ends
      				atc:auto_handoff	Set if aircraft are automatically handed off during the interruption (true/false)
      				atc:auto_accept		Set if aircraft are automatically accepted during the interruption (true/false)
      				atc:show_blank_screen	Set if a full black screen is to be displayed to hide the aircraft simulation (true/false)
      				atc:external_program	Not implemented yet

      Revision 1.5  2007/08/08 14:47:52  Aaron Yeung
      Exclude points scoring for resolving an aircraft conflict when its conflicting pair has already been resolved

      Revision 1.4  2007/08/01 15:12:19  Aaron Yeung
      Added scoring for conflicts

      Revision 1.3  2007/07/09 12:41:30  Aaron Yeung
      Modified criteria for aircraft to go into proposed state. Originally, an aircraft would go into the proposed state (CS_PROPOSED) when it was 2 minutes away from the sector boundary. Modified so that aircraft go into the proposed state at a fixed distance (configurable via XML) from the sector boundary

      Revision 1.2  2007/07/08 04:54:28  Aaron Yeung
      Added UpdateAircraft clock event handler to update the active attribute of each aircraft depending on whether they are set to enter the screen at a certain time

      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.9  2006/11/02 06:48:53  seth
      improve arc resolution

      Revision 1.8  2006/10/26 05:59:52  seth
      working implementation of experiment time logging

      Revision 1.7  2006/10/25 04:49:58  seth
      fixing logging

      Revision 1.6  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.5  2006/09/14 07:28:06  seth
      inital implementation of sector controller state

      Revision 1.4  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.3  2006/08/29 08:44:56  seth
      reimplementing mvc pattern

      Revision 1.2  2006/08/28 06:21:48  seth
      refactoring mvc

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __ATC_AIRSPACE_HDR__
#define __ATC_AIRSPACE_HDR__


#include <qobject.h>
#include "atc_types.h"
#include "aircraft_data.h"
#include "lab_engine.h"
#include <qregion.h>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>


#define AIRCRAFT_MAP      std::map<        std::string, AircraftAgent * >
#define AIRCRAFT_MAP_PAIR std::pair< const std::string, AircraftAgent * >
#define QREGIONLIST	      std::vector< QRegion >



namespace atc {

	//
	// atc classes
	//

	class  AircraftAgent;
	struct AreaDefinition;


	//
	// atc::airspace_error
	//
	struct airspace_error : public std::runtime_error{
		airspace_error(const std::string& msg)
			:std::runtime_error(msg)
		{}
	};

    //
    // atc::Airspace
    //

    class Airspace : public QObject {
		Q_OBJECT

    public: // construction
        Airspace();
        ~Airspace();

	public: // builder interface
		void set_hdoms( double hdoms ) { _hdoms = hdoms; }
		void set_vdoms( double vdoms ) { _vdoms = vdoms; }
		void SetSectorEntryDistance(int distanceNM) { _sector_entry_dist = distanceNM; }
		void SetSectorExitDistance(int distanceNM)	{ _sector_exit_dist = distanceNM; }
        void SetAircraftAutoAccept(bool a_autoAccept);
        void SetAircraftAutoHandoff(bool a_autoHandoff);
		void add_aircraft( AircraftAgent * );
		void define_sector( const AreaDefinition & );
		void add_non_sector( const AreaDefinition & );

	public:
		void initialize();

		bool point_in_sector( const Point & ) const;
		int GetTimeToTravelToDistance(AircraftAgent* ac, double distance) const;
		int GetSectorEntryDistance() const { return _sector_entry_dist; }
		int GetSectorExitDistance()	const { return _sector_exit_dist; }

	public:
		AIRCRAFT_MAP::iterator begin() { return _aircraft.begin(); }
		AIRCRAFT_MAP::iterator   end() { return _aircraft.end();   }

    public slots:
		void advance( int );
		void UpdateAircraft(long seconds);
		void HandleAircraftConflictResolved(std::string conflict_aircraft1, 
											std::string conflict_aircraft2, 
											int acceptance_time, 
											int conflict_time, 
											int current_time);

	private:
		QPointArray& area_points( const AreaDefinition &, QPointArray & );
		double bearing( QPoint, QPoint ) const;  

	signals:
		void advanced();
		void event_recorded( KeyValues & );
		void AircraftConflictResolved(std::string, std::string, int, int, int);
		void AircraftConflictFalseAlarm();

	private:
		AIRCRAFT_MAP _aircraft;

		QRegion     _sector;
		QREGIONLIST _nonsector;

		double _hdoms;
		double _vdoms;
		int _sector_entry_dist;
		int _sector_exit_dist;

	//
	// iterators
	//

	public:
		typedef AIRCRAFT_MAP::iterator iterator;

	//
	// functors
	//

	private: 
		//
		// AdvancePosition
		//
		class AdvancePosition 
			  : public std::unary_function< AIRCRAFT_MAP_PAIR, void > {
		public:
			AdvancePosition( int ms ) : _time( ms ) {}
			void operator ()( AIRCRAFT_MAP_PAIR ) const; 
		private:
			const int _time;
		};

		//
		// DetectConflicts
		//
		class DetectConflicts 
			  : public std::unary_function< AIRCRAFT_MAP_PAIR, void > {
		public:
			DetectConflicts( AIRCRAFT_MAP *, double, double );
			void operator ()( AIRCRAFT_MAP_PAIR ) const;
		private:
			AIRCRAFT_MAP *_map;
			double _hdoms;
			double _vdoms;
		};

		friend class DetectConflicts;

		//
		// atc::Airspace::IsConflicted
		//
		class IsConflicted
			  : public std::unary_function< AIRCRAFT_MAP_PAIR, bool > {
		public:
			IsConflicted( AircraftAgent *, double, double );
			bool operator ()( AIRCRAFT_MAP_PAIR ) const;
		private:
			AircraftAgent *_ac;
			double         _hdoms;
			double         _vdoms;
		};

		//
		// atc::Airspace::InitialControlState
		//
		class InitialControlState
			  : public std::unary_function< AIRCRAFT_MAP_PAIR, void > {
		public:
			InitialControlState( const Airspace * as ) : _airspace( as ) {}
			void operator ()( AIRCRAFT_MAP_PAIR ) const;
		private:
			const Airspace *_airspace;
		};

		//
		// atc::Airspace::UpdateControlState
		//
		class UpdateControlState
			  : public std::unary_function< AIRCRAFT_MAP_PAIR, void > {
		public:
			UpdateControlState( const Airspace * as ) : _airspace( as ) {}
			void operator ()( AIRCRAFT_MAP_PAIR ) const;
		private:
			ControlState updated_control( AircraftAgent * ) const;
		private:
			const Airspace *_airspace;
		};

	};

	//
	// typedefs
	//
	typedef AIRCRAFT_MAP      ACAgentMap;
	typedef AIRCRAFT_MAP_PAIR ACAgentMapPair;

	typedef QREGIONLIST       QRegionList;


};

#undef AIRCRAFT_MAP
#undef AIRCRAFT_MAP_PAIR


#endif

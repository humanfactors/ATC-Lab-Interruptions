/**
 * @file:       atc_sky_impl.h
 *
 *              ATC_Engine project definitions - c++ declarations .
 *              Contains aircraft information for Real/Map Space.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:34:37 PM 06/24/04
 *
 * $Id: sky_impl.h,v 1.9.2.3 2014/12/02 13:48:59 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_SKY_IMPL_HDR__
#define __ATC_SKY_IMPL_HDR__


#ifdef WIN32
	// identifier truncated in debug information
	#pragma warning(disable:4786)
#endif


#include "sky.h"
#include "aircraftparam.h"
#include "area_definition.h"


namespace atc{

	/*!
	 *
	 */
	typedef std::map< std::string, atc::AreaDefinition > AreaHash;

	/*!
	 * \struct atc::sky_error
	 */
	struct sky_error : public std::runtime_error{
		sky_error(const std::string& msg) : std::runtime_error(msg){}
	};


	/*!
	 * \class atc::SkyImpl atc_sky_impl.h
	 */
	class SkyImpl : public atc::Sky
	{
	public:

		SkyImpl();
		virtual ~SkyImpl();

	public: // Sky Interface [creational]

		void create_aircraft(
			const char *,
			const char * = ""
		);

		void SetAircraftStartTime( 
			const char *id, 
			unsigned int start_time);

		void SetCoordinationRequestStateForAircraft(
			const char *id);

		void SetCoordinationRequestKeyForAircraft(
			const char* a_id, 
			char a_key);

		void SetAircraftAcceptStateFlashItem(
			const char *id,
			std::string item);

		void SetAircraftAcceptStateFlashColour(
			const char *id,
			std::string colour);

		void SetAircraftConflictDetailsTrial(
			std::string callsign, 
			std::string trial);

		void SetAircraftConflictDetailsAcceptanceTime(
			std::string callsign, 
			int time);

		void SetAircraftConflictDetailsConflictTime(
			std::string callsign, 
			int time);

		void SetAircraftConflictDetailsConflictAircraft(
			 std::string callsign,
			 std::string conflict_aircraft);

		void add_aircraft_pathdescriptor(
			const char *,
			double, double,
			double = DEFAULT, double = DEFAULT,
			double = DEFAULT, double = DEFAULT
		);

//		void add_aircraft_pathdescriptor(
//			const char *, 
//			double, double, double, double
//		);

		void set_aircraft_initialaltitude(const char *, double);
        
        void 
        SetAircraftAltitudeChangeEnableTime(const char *    a_aircraftID,
                                            long            a_enableTime);

        void
        SetAircraftHandoffKey(  const char*         a_aircraftID,
                                const std::string&  a_handoffKey);

		void set_aircraft_initialvelocity(const char *, double);

		void set_aircraft_closedpath(
			const char *, bool = true
		);

		void create_weather( const char * );
		void add_weather_vertex( const char *, double , double );
		void add_weather_arc( const char *, double , double , double );
		void add_weather_ellipse( 
				const char *, double , double , double, double, double
		);

		void
		AddReminderBoxToAircraft(	const char*			a_id,
									ReminderBoxParams	a_reminderBoxParams);

		void
		SetReminderBoxFlashColorForAircraft(	const char*	a_id,
												const std::string& a_flashColour);

		void
		SetReminderBoxFlashTimeForAircraft(	const char* a_id,
											unsigned int a_flashStartTime, 
											unsigned int a_flashEndTime);
		
        void
        SetReminderBoxDismissButtonEnableTime(  const char* a_id,
                                                int a_enableTime = INVALID_VALUE);

        void
        SetReminderBoxDismissButtonLabel(   const char* a_id,
                                            std::string a_label = "");
		void
		SetReminderBoxDisplayTimeForAircraft(	const char* a_id,
												unsigned int a_displayStartTime, 
												unsigned int a_displayEndTime);

        virtual void
        SetReminderBoxDismissButtonFontSize(   const char* a_id,
                                                unsigned int a_fontSize);

        virtual void
        SetReminderBoxDismissButtonWidth(   const char* a_id,
                                            unsigned int a_width);

        virtual void
        SetReminderBoxDismissButtonHeight(  const char* a_id,
                                            unsigned int a_height);

        virtual void
        SetReminderBoxDismissButtonPosX(    const char* a_id,
                                            unsigned int a_posX);

        virtual void
        SetReminderBoxDismissButtonPosY(    const char* a_id,
                                            unsigned int a_posY);

	public:

		const atc::AircraftParamMap& aircraft()const{ return _aircraft; }
		const atc::AreaHash& weather_systems() const { return _area_at; }

	private:

		atc::AircraftParamMapIt findAircraft(const char *);
		void validate( const char * );

	private:

		atc::AircraftParamMap _aircraft;
		atc::AreaHash _area_at;

	};
}

#endif


/**
 * @file:       atc_sky.h
 *
 *              ATC_Engine project definitions - c++ header .
 *              Contains aircraft information for Real Space.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:34:37 PM 03/10/04
 *
 * $Id: sky.h,v 1.9.2.3 2014/12/02 13:48:59 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_SKY_HDR__
#define __ATC_SKY_HDR__

#include "atc.h"
#include "ReminderBox.h"

namespace atc{

	/*!
	 * \class atc::Sky atc_sky.h
	 * \brief Interface for creation of variable objects to be modelled 
	 * by the engine.
	 */
	class Sky
	{
	public:

		/*!
		 * Create an aircraft in the sky.
		 *
		 * Each aircraft is created with unique identifier which is
		 * equivalent to its callsign. On successful creation the value
		 * of the callsign is returned.
		 *
		 * Each aircraft also has a type that may be used to lookup 
		 * properties such as climb/descent rates and acceleration/
		 * decelleration rates. If no type is assigned then default 
		 * data will be used.
		 *
		 * Each aircraft also has a launch time asscoiated with it. The 
		 * default is to have the aircraft active from the beginning of 
		 * the trial, but it is possible to delay this until later. 
		 *
		 * \todo A  possible future enhancement might be to have 
		 * aircraft that are launched before the trial begins (ie 
		 * negative launch times). This could be useful if an aircraft 
		 * starts at a waypoint off the map but is required to be on the
		 * map at the start of the trial or if an aircraft is between 
		 * waypoints at the start of the trial. It would save having to
		 * calculate the new waypoint position of actual start. (??)
		 *
		 * \sa set_aircraft_type() set_aircraft_launch()
		 */
		virtual void create_aircraft(
			const char *,
			const char * = ""
		) = 0;


		/*!
		 * Sets the launch time for the aircraft.
		 *
		 * \sa create_aircraft()
		 */
		virtual void SetAircraftStartTime( 
			const char *id, 
			unsigned int start_time) = 0;

		/*!
		 * Sets the user input state of the aircraft as USER_INPUT_STATE_COORDINATION_REQUEST.
		 *
		 * \sa create_aircraft()
		 */
		virtual void SetCoordinationRequestStateForAircraft(
			const char *id) = 0;

		/*!
		 * Sets the key that a user has to press for an aircraft when it is in the 
		 * coordination request mode
		 *
		 */
		virtual void SetCoordinationRequestKeyForAircraft(
			const char* a_id, 
			char a_key) = 0;

		/*!
		 * Sets the aircraft item that flashes when an aircraft goes into accepted state.
		 *
		 */
		virtual void SetAircraftAcceptStateFlashItem(
			const char *id,
			std::string item) = 0;

		/*!
		 * Sets the colour an aircraft item  flashes when an aircraft goes into accepted state.
		 *
		 */
		virtual void SetAircraftAcceptStateFlashColour(
			const char *id,
			std::string colour) = 0;

		/*!
		 * Sets the name of the trial when an aircraft conflicts.
		 */
		virtual void SetAircraftConflictDetailsTrial(
			std::string callsign, 
			std::string trial) = 0;

		/*!
		 * Sets the time when an aircraft is accepted
		 */
		virtual void SetAircraftConflictDetailsAcceptanceTime(
			std::string callsign, 
			int time) = 0;

		/*!
		 * Sets the time when an aircraft conflicts
		 */
		virtual void SetAircraftConflictDetailsConflictTime(
			std::string callsign, 
			int time) = 0;

		/*!
		 * Sets the aircraft callsign that will be in conflict
		 */
		virtual void SetAircraftConflictDetailsConflictAircraft(
			std::string callsign,
			std::string conflict_aircraft) = 0;

		/*!
		 * Add a flight path descriptor to an aircraft.
		 *
		 * The aircraft must have previously been created. To be valid 
		 * each aircraft must posses at least two path descriptors, a 
		 * from node and a to node. 
		 *
		 * Each descriptor consists of a location (x, y) a rate of 
		 * change in altitude (dz), a target altitude (tz), a rate of
		 * change in velocity (dv) and a target velocity (tv). The 
		 * flight bearing is calcuated from the position of the next
		 * descriptor.
		 *	If both the target and rate equal zero then no change is
		 * made to either current state of altitude or velocity (eg if 
		 * an aircraft is climbing at a fixed rate it will continue to 
		 * climb at that rate). If the rates are set to NaN then
		 * default rates are used from lookup tables (if supplied else
		 * generic default values which are instataneous). Instaneous
		 * changes may also be achieved be setting a new target and a 
		 * rate of zero.
		 *
		 * \todo currently we will only consider linear changes in 
		 * velocity and altitude.
		 *
		 * \sa set_aircraft_initialvelocity()
		 * \sa set_aircraft_initial_altitude()
		 */
		virtual void add_aircraft_pathdescriptor(
			const char *,
			double, double ,
			double = DEFAULT, double = DEFAULT,
			double = DEFAULT, double = DEFAULT
		) = 0;

		/*!
		 * Add a flight path descriptor to an aircraft.
		 *
		 * This is an overloaded function provided for convienience. 
		 * This form uses look up tables to find rates.
		 */
//		virtual void add_aircraft_pathdescriptor(
//			const char *,
//			double, double, double, double
//		) = 0;

		/*!
		 * Set the aircraft's initial altitude.
		 *
		 * The default behavoiour is to use a standard cruising altitude
		 * from a look up on aircraft type (or use a default value of 
		 * ??). This function provides non-standard fine control. It is 
		 * only necessary to set an initial altitude for aircraft that 
		 * are in an initial state of ascent/descent, otherwise the 
		 * desired result may be achieved using 
		 * add_aircraft_pathdescriptor(). Might also be simpler to use
		 * this function in experiments that do not use altitude.
		 *
		 * \sa add_aircraft_pathdescriptor()
		 */
		virtual void set_aircraft_initialaltitude(
			const char *,
			double
		) = 0;

        /*!
		 * Set the time when a participant can alter an aircraft's altitude.
         */
        virtual void
        SetAircraftAltitudeChangeEnableTime(    const char *    a_aircraftID,
                                                long            a_enableTime) = 0;

        /*!
		 * Set a key used to handoff an aircraft.
         */
        virtual void
        SetAircraftHandoffKey(  const char*         a_aircraftID,
                                const std::string&  a_handoffKey) = 0;

		/*!
		 * Set the aircrafts initial velocity.
		 *
		 * The default behaviour of an aircraft is to use a standard
		 * cruising speed from a lookup table (or default value of ??).
		 * This function provides finer control but is only realy 
		 * necessary for aircraft that are in an intial state of 
		 * slowing down/speeding up. It might also be simpler to use 
		 * this method in experiments that use constant velocity.
		 * or descent.
		 *
		 * \sa add_aircraft_pathdescriptor()
		 */
		virtual void set_aircraft_initialvelocity(
			const char *,
			double
		) = 0;

		/*!
		 * Close the loop of a flight path.
		 * 
		 * Once the aircraft reaches the last path descriptor it will 
		 * fly to the beginning again ie if loop set equal to true then
		 * the aircraft will effectively fly in circles. To add a new 
		 * descriptors after loop has been closed will reopen the loop.
		 */
		virtual void set_aircraft_closedpath(
			const char *, 
			bool = true
		) = 0;

		/*!
		 * Create a area of "weather"
		 */
		virtual void create_weather( const char * ) = 0;

		/*!
		 * Add a vertex to the weather blob.
		 */
		virtual void add_weather_vertex( const char *, double , double ) = 0;

		/*!
		 * Add an arc to the weather blob.
		 */
		virtual void add_weather_arc( 
				const char *, double , double , double
		) = 0;

		/*!
		 * Add an elliptical weather blob.
		 */
		virtual void add_weather_ellipse( 
				const char *, double , double , double, double, double
		) = 0;

		/*!
		 * Add a reminder box to an aircraft
		 */
		virtual void
		AddReminderBoxToAircraft(	const char*			a_id,
									ReminderBoxParams	a_reminderBoxParams) = 0;

		virtual void
		SetReminderBoxFlashColorForAircraft(	const char*	a_id,
												const std::string& a_flashColour) = 0;

		virtual void
		SetReminderBoxFlashTimeForAircraft(	const char* a_id,
											unsigned int a_flashStartTime, 
											unsigned int a_flashEndTime) = 0;

        virtual void
        SetReminderBoxDismissButtonEnableTime(  const char* a_id,
                                                int a_enableTime) = 0;

        virtual void
        SetReminderBoxDismissButtonLabel(   const char* a_id,
                                            std::string a_label) = 0;

		virtual void
		SetReminderBoxDisplayTimeForAircraft(	const char* a_id,
												unsigned int a_displayStartTime, 
												unsigned int a_displayEndTime) = 0;

        virtual void
        SetReminderBoxDismissButtonFontSize(    const char* a_id,
                                                unsigned int a_fontSize) = 0;

        virtual void
        SetReminderBoxDismissButtonWidth(   const char* a_id,
                                            unsigned int a_width) = 0;

        virtual void
        SetReminderBoxDismissButtonHeight(  const char* a_id,
                                            unsigned int a_height) = 0;

        virtual void
        SetReminderBoxDismissButtonPosX(    const char* a_id,
                                            unsigned int a_posX) = 0;

        virtual void
        SetReminderBoxDismissButtonPosY(    const char* a_id,
                                            unsigned int a_posY) = 0;
	};
}

#endif

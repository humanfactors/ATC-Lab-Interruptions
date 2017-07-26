/**
 * @file:       atc_aircraftparam.cpp
 *
 *              ATC-Lab: Engine component
 *              Aircraft representaion parameters
 *
 * @author:     seth  
 * @email:		atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 26/10/2004
 *
 * $Id: aircraftparam.h,v 1.12.2.2 2014/12/02 13:48:59 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_AIRCRAFTPARAM_HDR__
#define __ATC_AIRCRAFTPARAM_HDR__


#include "atc.h"
#include "ReminderBox.h"
#include <map>
#include <vector>


namespace atc{

	enum UserInputState
	{
		USER_INPUT_STATE_DEFAULT,
		USER_INPUT_STATE_COORDINATION_REQUEST
	};

	enum AcceptedStateFlashItem
	{
		FLASH_ITEM_NONE,
		FLASH_ITEM_SI,		// Set the speed information (SI) to blink
		FLASH_ITEM_FL,		// Set the flight level (FL) to blink
		FLASH_ITEM_CS		// Set the callsign (CS) to blink
	};

	struct AcceptedStateFlashParams
	{
		AcceptedStateFlashParams()
			: flash_colour(CS_PROPOSED_COLOUR), flash_item(FLASH_ITEM_NONE) {}

		AcceptedStateFlashParams(const AcceptedStateFlashParams& rhs)
		{
			flash_colour = rhs.flash_colour;
			flash_item = rhs.flash_item;
		};

		QColor flash_colour;
		AcceptedStateFlashItem flash_item;
	};

	/*!
	 * \struct atc::FlightPathDec
	 */
	struct FlightPathDesc {

		FlightPathDesc()
			: x(0), y(0), tz(0), dz(0), tv(0), dv(0)
		{}

		FlightPathDesc(
			double x, double y,
			double tz, double dz,
			double tv, double dv
		)
			: x(x), y(y), tz(tz), dz(dz), tv(tv), dv(dv)
		{}

		double x, y, tz, dz, tv, dv;

	};


	/*!
	 * \typedef atc::FlightPath
	 * \typedef atc::FlightPathIt
	 * \typedef atc::FlightPathCIt
	 */
	typedef std::vector<atc::FlightPathDesc> FlightPath;
	typedef atc::FlightPath::iterator FlightPathIt;
	typedef atc::FlightPath::const_iterator FlightPathCIt;


	/*!
	 * \struct atc::AircraftParam
	 */
	struct AircraftParam{

		AircraftParam( const char *id, const char *type )
			:callsign( id )
			,type( type )
			,oVelocity( NAN )
			,oAltitude( NAN )
			,flightpath( )
			,user_input_state(USER_INPUT_STATE_DEFAULT)
			,conflict_trial_name("")
			,conflict_acceptance_time(INVALID_VALUE)
			,conflict_time(INVALID_VALUE)
			,conflict_aircraft("")
			,m_reminderBoxParams(NULL)
            ,m_altitudeChangeEnableTime(INVALID_VALUE)
		{
			coordinationRequestKey = static_cast<char>(AC_ACCEPT_KEY_COORD_REQ_DEFAULT);
            char defaultKey = (char) AC_HANDOFF_KEY;
            m_handoffKey = &defaultKey;
		}

		AircraftParam( const AircraftParam& a_rhs )
			:callsign(a_rhs.callsign)
			,type(a_rhs.type)
			,oVelocity(a_rhs.oVelocity)
			,oAltitude(a_rhs.oAltitude)
			,flightpath(a_rhs.flightpath)
			,user_input_state(a_rhs.user_input_state)
			,conflict_trial_name(a_rhs.conflict_trial_name)
			,conflict_acceptance_time(a_rhs.conflict_acceptance_time)
			,conflict_time(a_rhs.conflict_time)
			,conflict_aircraft(a_rhs.conflict_aircraft)
			,m_reminderBoxParams(a_rhs.m_reminderBoxParams.release())
            ,m_altitudeChangeEnableTime(a_rhs.m_altitudeChangeEnableTime)
            ,m_handoffKey(a_rhs.m_handoffKey)
		{
		}

		const char*					callsign;
		const char*					type;
		double						oVelocity;
		double						oAltitude;
        long                        m_altitudeChangeEnableTime;
		unsigned int				start_time;
		UserInputState				user_input_state;
		char						coordinationRequestKey;
		FlightPath					flightpath;
		AcceptedStateFlashParams	accepted_state_flash_params;
		std::string					conflict_trial_name;
		int							conflict_acceptance_time;
		int							conflict_time;
		std::string					conflict_aircraft;
        std::string                 m_handoffKey;
		std::auto_ptr<ReminderBoxParams>	m_reminderBoxParams;
	};


	/*!
	 * \typedef AircraftParamMap
	 * \typedef AircraftParamMapIt
	 * \typedef AircraftParamMapCIt
	 */
	typedef std::map<std::string, atc::AircraftParam>AircraftParamMap;
	typedef AircraftParamMap::iterator AircraftParamMapIt;
	typedef AircraftParamMap::const_iterator AircraftParamMapCIt;


};

#endif

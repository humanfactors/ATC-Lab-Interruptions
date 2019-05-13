/**
 * @file:       atc_sky_impl.cpp
 *
 *              ATC_Engine project definitions - c++ definitions.
 *              Contains aircraft information for Real/Map Space.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:34:37 PM 07/07/04
 *
 * $Id: sky_impl.cpp,v 1.11.2.3 2014/12/02 13:48:59 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */


#include "sky_impl.h"
#include "performance/factory.h"
#include <string>

using namespace atc;

//------------------------------------------------------------------------------

/*!
 * Default Constructor
 */
SkyImpl::SkyImpl() {}

/*!
 * Destructor
 */
SkyImpl::~SkyImpl() {}

//------------------------------------------------------------------------------

//
// Sky Interface [creational]
// (see atc_sky.h for documentation)
//

void SkyImpl::create_aircraft( const char * id, const char * type ) {
	// @todo: 
	// validate performance data exists
	performance::Factory::performance( type );

	std::pair< AircraftParamMapIt, bool > result = _aircraft.insert(
		std::make_pair( id, AircraftParam( id, type ) )
	);
	if(!result.second)
		throw sky_error("error creating aircraft");
}

void
SkyImpl::AddReminderBoxToAircraft(const char*		a_id,
								  ReminderBoxParams	a_reminderBoxParams)
{
	atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams = 
		std::auto_ptr<ReminderBoxParams>(new ReminderBoxParams(a_reminderBoxParams));
}

void
SkyImpl::SetReminderBoxFlashColorForAircraft(	const char*	a_id,
												const std::string& a_flashColour)
{
	atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_flashColors.push_back(QColor(a_flashColour));
}

void
SkyImpl::SetReminderBoxFlashTimeForAircraft(	const char* a_id,
												unsigned int a_flashStartTime, 
												unsigned int a_flashEndTime)
{
	atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_flashPeriods.push_back(
							std::make_pair<unsigned int, unsigned int>
							(a_flashStartTime, a_flashEndTime));
}

void
SkyImpl::SetReminderBoxDismissButtonEnableTime( const char* a_id,
                                                int a_enableTime)
{
    atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_dismissButtonEnableTime = a_enableTime;
}

void
SkyImpl::SetReminderBoxDismissButtonLabel(const char* a_id,
                                          std::string a_label)
{
    atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_dismissButtonLabel = a_label;
}


void
SkyImpl::SetReminderBoxDisplayTimeForAircraft(	const char* a_id,
												unsigned int a_displayStartTime, 
												unsigned int a_displayEndTime)
{
	atc::AircraftParamMapIt it = findAircraft(a_id);


	it->second.m_reminderBoxParams->m_displayTimes.push_back(
							std::make_pair<int, int>
							(a_displayStartTime, a_displayEndTime));
}

void
SkyImpl::SetReminderBoxDismissButtonFontSize(   const char* a_id,
                                                unsigned int a_fontSize)
{
    atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_dismissButtonFontSize = a_fontSize;
}

void
SkyImpl::SetReminderBoxDismissButtonWidth(   const char* a_id,
                                            unsigned int a_width)
{
    atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_dismissButtonWidth = a_width;
}

void
SkyImpl::SetReminderBoxDismissButtonHeight(  const char* a_id,
                                            unsigned int a_height)
{
    atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_dismissButtonHeight = a_height;
}

void
SkyImpl::SetReminderBoxDismissButtonPosX(    const char* a_id,
                                            unsigned int a_posX)
{
    atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_dismissButtonPosX = a_posX;
}

void
SkyImpl::SetReminderBoxDismissButtonPosY(    const char* a_id,
                                            unsigned int a_posY)
{
    atc::AircraftParamMapIt it = findAircraft(a_id);

	it->second.m_reminderBoxParams->m_dismissButtonPosY = a_posY;
}

void SkyImpl::SetAircraftStartTime( const char *id, unsigned int start_time ) {
	atc::AircraftParamMapIt it = findAircraft(id);
	it->second.start_time = start_time;
}

void SkyImpl::SetCoordinationRequestStateForAircraft(const char *id)
{
	atc::AircraftParamMapIt it = findAircraft(id);
	it->second.user_input_state = USER_INPUT_STATE_COORDINATION_REQUEST;
}

void
SkyImpl::SetCoordinationRequestKeyForAircraft(const char* a_id, char a_key)
{
	atc::AircraftParamMapIt it = findAircraft(a_id);
	it->second.coordinationRequestKey = a_key;
}

void SkyImpl::SetAircraftAcceptStateFlashItem(const char *id, std::string item)
{
	atc::AircraftParamMapIt it = findAircraft(id);

	if (item == "si")
	{
		it->second.accepted_state_flash_params.flash_item = FLASH_ITEM_SI;
	}
	else if (item == "fl")
	{
		it->second.accepted_state_flash_params.flash_item = FLASH_ITEM_FL;
	}
	else if (item == "cs")
	{
		it->second.accepted_state_flash_params.flash_item = FLASH_ITEM_CS;
	}
	else if (item == "none")
	{
		it->second.accepted_state_flash_params.flash_item = FLASH_ITEM_NONE;
	}
	else
	{
		throw sky_error("SetAircraftAcceptStateFlashItem: error setting accept state flash item");
	}
}

void SkyImpl::SetAircraftAcceptStateFlashColour(const char *id, std::string colour)
{
	atc::AircraftParamMapIt it = findAircraft(id);

	if (colour == "red")
	{
		it->second.accepted_state_flash_params.flash_colour = Qt::red;
	}
	else if (colour == "blue")
	{
		it->second.accepted_state_flash_params.flash_colour = Qt::blue;
	}
	else
	{
		throw sky_error("SetAircraftAcceptStateFlashColour: error setting accept state flash colour");
	}
}

void SkyImpl::SetAircraftConflictDetailsTrial(
	std::string callsign, 
	std::string trial) 
{
	atc::AircraftParamMapIt it = findAircraft(callsign.c_str());

	it->second.conflict_trial_name = trial;
}

void SkyImpl::SetAircraftConflictDetailsAcceptanceTime(
	std::string callsign, 
	int time) 
{
	atc::AircraftParamMapIt it = findAircraft(callsign.c_str());

	it->second.conflict_acceptance_time = time;
}

void SkyImpl::SetAircraftConflictDetailsConflictTime(
	std::string callsign, 
	int time) 
{
	atc::AircraftParamMapIt it = findAircraft(callsign.c_str());

	it->second.conflict_time = time;
}

void SkyImpl::SetAircraftConflictDetailsConflictAircraft(
	 std::string callsign,
	 std::string conflict_aircraft)
{
	atc::AircraftParamMapIt it = findAircraft(callsign.c_str());

	it->second.conflict_aircraft = conflict_aircraft;
}

void SkyImpl::add_aircraft_pathdescriptor(
		const char *id,
		double x, double y,
		double tz, double dz,
		double tv, double dv
){
	AircraftParamMapIt it = findAircraft( id );
	FlightPath &rds = it->second.flightpath;
	rds.push_back( FlightPathDesc( x, y, tz, dz, tv, dv ) );
}

//void atc::SkyImpl::add_aircraft_pathdescriptor(
//	const char * id, 
//	double x, double y, double tz, double tv
//){
//	add_aircraft_pathdescriptor(
//		id, x, y, tz, atc::NaN, tv,atc::NaN
//	);
//}

void SkyImpl::set_aircraft_initialaltitude( const char * id, double z ) {
	AircraftParamMapIt it = findAircraft(id);
	it->second.oAltitude = z;
}

void 
SkyImpl::SetAircraftAltitudeChangeEnableTime(   const char * a_aircraftID,
                                                long a_enableTime)
{
    AircraftParamMapIt it = findAircraft(a_aircraftID);
	it->second.m_altitudeChangeEnableTime = a_enableTime;
}

void
SkyImpl::SetAircraftHandoffKey( const char*         a_aircraftID,
                                const std::string&  a_handoffKey)
{
    AircraftParamMapIt it = findAircraft(a_aircraftID);
	it->second.m_handoffKey = a_handoffKey;
}

void SkyImpl::set_aircraft_initialvelocity( const char * id, double v ) {
	AircraftParamMapIt it = findAircraft(id);
	it->second.oVelocity = v;
}

void SkyImpl::set_aircraft_closedpath( const char * id, bool loop ) {
	// \todo: closed loop path
//	atc::AircraftParamMapIt it = findAircraft(id);
//	it->second.loop = loop;
}

void SkyImpl::create_weather( const char *uid ) {
	std::string id( uid );
	try {
		if ( !strlen( uid ) ) {
			throw sky_error( "invalid unique area identifier [ " + id + " ]" );
		}
		if ( _area_at.count( uid ) ) {
			throw sky_error( "area identifier NOT unique [ " + id + " ]" );
		}

		std::pair< AreaHash::const_iterator, bool > result =
			_area_at.insert( std::make_pair(
				id, AreaDefinition( uid )
			));

		if ( ! result.second ) {
			throw sky_error( "cannot create area [ " + id + " ]" );
		}
	}
	catch ( sky_error &e ) {
		throw e;
	}
}

void SkyImpl::add_weather_vertex( const char *id, double x, double y ) {
	validate( id );
	_area_at[ id ].descriptors.push_back( new VertexDescriptor( x, y ) );
}

void SkyImpl::add_weather_arc( const char *id, double x, double y, double r ) {
	validate( id );
	_area_at[ id ].descriptors.push_back( new ArcDescriptor( x, y, r ) );
}

void SkyImpl::add_weather_ellipse( 
		const char *id
	  , double x, double y
	  , double w, double h
	  , double a
){
	validate( id );
	_area_at[ id ].descriptors
		.push_back( new EllipseDescriptor( x, y, w, h, a ) );
	;
}


//------------------------------------------------------------------------------

/*!
 */
AircraftParamMapIt SkyImpl::findAircraft( const char *id ) {
	atc::AircraftParamMapIt it = _aircraft.find(id);
	if(it == _aircraft.end())
		throw sky_error(
			std::string(id) + " aircraft does not exist"
		);
	return it;
}


/*!
 * Confirm Area has been declared
 */
void SkyImpl::validate( const char *id ) {
	try {
		if ( !_area_at.count( id ) ) {
			throw sky_error( 
					"Area is NOT defined [ " 
				  + std::string( id ) + " ]!" 
			);
		}
	}
	catch ( sky_error &e ) {
		throw e;
	}
}
//------------------------------------------------------------------------------

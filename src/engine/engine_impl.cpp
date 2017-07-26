/**
 * @file:       atc_engine_impl.cpp
 *
 *              ATC_Engine project implementations - c++ methods .
 *              An implementation of atc::Engine Interface.
 *
 * @author:     seth  
 * @email:	atc_support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $ 
 *              File created 12:24:10 PM 03/04/04
 *
 * $Id: engine_impl.cpp,v 1.25.2.11 2015/08/19 11:33:57 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */


#include "engine_impl.h"

#include "aircraft_agent.h"
#include "aircraft_data.h"
#include "aircraftsprite.h"
#include "airspace.h"
#include "canvas.h"
#include "clock.h"
#include "lab_engine.h"

#include "canvasview.h"
#include "listbox.h"
#include "transformation.h"

#include <qdatetime.h>
#include <assert.h>

#include "translator.h" // \todo: kill me!!

using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
// atc::EngineImpl
//
// todo: this class is to be refactored for Engine Building. The new Engine is 
// todo: LabEngine. 
//
//------------------------------------------------------------------------------
// initalization
//

/*!
 *
 */
EngineImpl::EngineImpl()
	  : _atclab( new LabEngine )
	  , _airspace( new Airspace() )
	  , _canvas  ( new Canvas()   )
	  , _clock   ( new Clock()    )
	  , _params( new ParamImpl() )
	  , _map( new MapImpl() )
	  , _sky( new SkyImpl() )
{}

/*!
 */
EngineImpl::~EngineImpl() {
	if ( _sky )           delete _sky;
	if ( _map )           delete _map;
	if ( _params )        delete _params;
}

//------------------------------------------------------------------------------
// [new]
//

/*!
 * Build ATC-Lab engine
 */
void EngineImpl::build() {

	process_map(); // initializes transformation - do first
	process_sky();
	process_scale();
	process_params();

	_atclab->set_airspace( _airspace );
	_atclab->set_canvas( _canvas );

	_clock->set_interval( _params->update_rate() );
	_clock->set_multiple( _params->update_multiplier() );

	connect(_clock.get(), SIGNAL(seconds_update(long)), 
						this, SLOT(HandleSecondsUpdate(long)));

	/*connect(_clock.get(), SIGNAL(seconds_update(long)),
						_canvas, SLOT(HandleSecondsUpdate(long)));*/

	_atclab->set_clock( _clock );

	connect(_canvas.get(), SIGNAL(FailedAcceptanceTask(const char*)),
			this, SIGNAL(FailedAcceptanceTask(const char*)));
	connect(_canvas.get(), SIGNAL(FailedHandoffTask(const char*)),
			this, SIGNAL(FailedHandoffTask(const char*)));
	connect(this, SIGNAL(UpdateScores(float, float, float)), 
			_canvas.get(), SLOT(HandleScoreUpdate(float, float, float)));
	connect(_airspace.get(), SIGNAL(AircraftConflictResolved(std::string, std::string, int, int, int)),
			this, SIGNAL(AircraftConflictResolved(std::string, std::string, int, int, int)));
	connect(_airspace.get(), SIGNAL(AircraftConflictFalseAlarm()),
			this, SIGNAL(AircraftConflictFalseAlarm()));
	connect(_canvas.get(), SIGNAL(Conflicted(std::pair<std::string, std::string>)),
			this, SIGNAL(Conflicted(std::pair<std::string, std::string>)));
}

void EngineImpl::TrialTime(unsigned int trial_time)
{
	_atclab->TrialTime(trial_time);
	connect(_atclab.get(), SIGNAL(TrialTimeout(long)), 
		this, SIGNAL(TrialTimeout(long)));
	connect(_atclab.get(), SIGNAL(TrialTimeout(long)),
		_canvas.get(), SLOT(HandleTrialTimeout(long)));
}

//------------------------------------------------------------------------------

/*!
 * Provide a reference to Parameters for initialization purposes.
 */
Param& EngineImpl::param(){
	return *_params;
}

/*!
 * Provide a reference to Map for initialization purposes.
 */
Map& EngineImpl::map(){
	return *_map;
}

/*!
 * Provide a reference to Sky for initialization purposes.
 */
Sky& EngineImpl::sky(){
	return *_sky;
}

/*!
 * Initialize the ATC Engine.
 *
 * Note: must process map first since it sets up conversion factors 
 * between map and canvas.
 *
 * pre-condition: the map & sky are already populated.
 */
void EngineImpl::init() {
	build();
}

//------------------------------------------------------------------------------
// [private] map
// 

/*!
 * Process parameters.
 */
void EngineImpl::process_params()
{
	_airspace->SetSectorEntryDistance(_params->SectorEntryDistance()); 
	_airspace->SetSectorExitDistance(_params->SectorExitDistance());
	_airspace->set_hdoms(_params->hdoms());
	_airspace->set_vdoms(_params->vdoms());
}

/*!
 */
void EngineImpl::process_map()
{
	//
	// set canvas region
	//
	_canvas->resize( _map->rect() );

	//
	// draw canvas map
	//

	process_sectors(_map->sectors());
	process_routes(_map->routes());
	process_locations(_map->locations());
}

/*!
 * Add sectors to canvas
 */
void EngineImpl::process_sectors( const SectorMap &sector_at ) {
	SectorMap::const_iterator asec;
	
	// do active sector first
	asec = sector_at.find( _map->active_sector() );
	_canvas->add_active_sector( asec->second );
	_airspace->define_sector( asec->second );

	// process remaining sectors
	SectorMap::const_iterator sec = sector_at.begin();
	for ( ; sec != sector_at.end(); ++sec ) {
		if ( asec == sec ) continue;
		_canvas->add_sector( sec->second );
		_airspace->add_non_sector( sec->second );
	}

	// define active sector for airspace
}

/*!
 *
 */
void EngineImpl::process_routes( const RouteMap& routes ) {
	RouteMap::const_iterator it = routes.begin();
	for( ; it != routes.end(); ++it){
		_canvas->add_route( *(it->second) );
	}
}

/*!
 */
void EngineImpl::process_locations( const LocationMap &locations ) {
	LocationMap::const_iterator it = locations.begin();
	for( ; it != locations.end(); ++it){
		Location pt = it->second;
		if ( pt.show ) _canvas->add_waypoint( pt.name, pt.x, pt.y );
	}
}

//------------------------------------------------------------------------------
// sky
//

void EngineImpl::process_sky() {
	// aircraft
	AircraftParamMap::const_iterator pit = _sky->aircraft().begin();
	for ( ; pit != _sky->aircraft().end(); ++pit ) {
		process_aircraft(pit->second);
	}

	// weather systems
	AreaHash::const_iterator wit = _sky->weather_systems().begin();
	for ( ; wit != _sky->weather_systems().end(); ++wit ) {
		_canvas->add_weather( wit->second );
	}

}

/*!
 * Aircraft builder
 *
 * 
 */
void EngineImpl::process_aircraft( const AircraftParam& aircraft ) {
	AircraftData data;
	AircraftPath path;

	data.call = aircraft.callsign;
	data.type = aircraft.type;
	data.start_time = aircraft.start_time;
	data.accepted_state_flash_params = aircraft.accepted_state_flash_params;
	data.user_input_state = aircraft.user_input_state;
	data.coordinationRequestKey = aircraft.coordinationRequestKey;
	data.conflict_trial_name = aircraft.conflict_trial_name;
	data.conflict_acceptance_time = aircraft.conflict_acceptance_time;
	data.conflict_time = aircraft.conflict_time;
	data.conflict_aircraft = aircraft.conflict_aircraft;
	data.m_reminderBoxParams = aircraft.m_reminderBoxParams;
    data.m_altitudeChangeEnableTime = aircraft.m_altitudeChangeEnableTime;
    data.m_handoffKey = aircraft.m_handoffKey;

	//
	// process flight path information
	//
	assert( 2 <= aircraft.flightpath.size() );

	FlightPath::const_iterator it = aircraft.flightpath.begin();
	
	const FlightPathDesc *pos0 = it;
	const FlightPathDesc *pos1;
	
	double z0 = aircraft.oAltitude;
	double v0 = aircraft.oVelocity;

	while ( ++it != aircraft.flightpath.end() ) {
		pos1 = it;

		path.push( AircraftPathToken( 
				pos0->x, pos0->y,       z0,       v0
			  ,	pos1->x, pos1->y, pos0->tz, pos0->tv 
		) );

		z0 = pos1->tz;
		v0 = pos1->tv;

		pos0 = pos1;
	}
	
	add_aircraft( data, path );
}

/*!
 * Add a new aircraft to the system
 *
 * @param data defines the aircraft
 * @param path describes its movement in space
 *
 * @todo: currently this is called from process_aircraft but the method is
 * @todo: public and should be called directly
 */
void EngineImpl::add_aircraft(
		const AircraftData &data
	  , const AircraftPath &path 
) {
	AircraftAgent *a = new AircraftAgent( _airspace.get(), data.call );
	a->set_data( data );
	a->set_path( path );
	_airspace->add_aircraft( a );
}

//------------------------------------------------------------------------------
// 
//

/*!
QPointArray& EngineImpl::area_points( 
		const AreaDefinition &area_def
	  , QPointArray &pts_ref 
) {
	// validate point array
	if ( ! pts_ref.isNull() ) {
		throw engine_error( "Cannot process area: point array is not empty!" );
	}
	
	// process area's descriptors
	AreaDefinition::const_iterator it = area_def.begin();
	for ( ; it != area_def.end(); ++it) {

		switch ( (*it)->rtti ) {
		
		case ( AreaDescriptor::RTTI_VERTEX ): {
			pts_ref.putPoints( pts_ref.size(), 1, (*it)->x, (*it)->y ); 
			break;
		}
		case ( AreaDescriptor::RTTI_ARC ): {
			ArcDescriptor *arc = static_cast< ArcDescriptor* >( *it );

			QPoint p0( (int)( arc->x ), (int)( arc->y ) );
			int r = (int)( arc->r );

			if ( 0 == pts_ref.size() ) {
				// assume 360 single area arc descriptor
				// @todo: assert( only area descriptor )
				pts_ref.makeEllipse( p0.x() - r, p0.y() - r , 2 * r, 2 * r );
			}
			else {
				++it;  // need next point also to limit arc

				assert( it != area_def.end() );

				QPoint pA( pts_ref.at( pts_ref.size() - 1 ) );
				QPoint pB( (*it)->x, (*it)->y );

				bool minor = true;  // TODO:??

				double bA = bearing(p0, pA);
				double bB = bearing(p0, pB);

				if ( (bB - bA > 180 && minor) || (bB - bA < 180 && !minor) ) {
					double tmp = bA;
					bA = bB;
					bB = tmp;
				}

				double angle = bB - bA; 

				QPointArray arcPts;
				arcPts.makeArc(
					p0.x() - r, p0.y() - r, 2 * r, 2 * r, 
					(floor)(bA * 16 + 0.5), (floor)(angle * 16 + 0.5)
				);

				// Note: do not include pA or pB in final pts.
				pts_ref.putPoints( pts_ref.size() - 1, arcPts.size(), arcPts );
			}			
			
			break;
		}
		case ( AreaDescriptor::RTTI_ELLIPSE ): {
			EllipseDescriptor *e 
				  = static_cast< EllipseDescriptor* >( *it );

			QPoint p0( e->x, e->y );
			int w = (int)( e->w );
			int h = (int)( e->h );
			int a =      ( e->a );

			// @todo: assert only one area descriptor (this)
			pts_ref.makeEllipse( p0.x() - w/2, p0.y() - h/2 , w, h );

			if ( a ) {
				QWMatrix matrix;
				matrix.rotate( -a );
				pts_ref.translate( -p0.x(), -p0.y() );
				pts_ref = matrix.map( pts_ref );
				pts_ref.translate(  p0.x(),  p0.y() );
			}
			
			break;
		}
		default:
			throw engine_error("Unknown area descriptor!");
		}
	}
	return pts_ref;
}
 */

//------------------------------------------------------------------------------
// [private] scale
// 

/*!
 * Add a scale marker to map (cross hair type)
 */
void EngineImpl::process_scale() { 

	_canvas->add_scale( 
			SCALE_X_EXT, SCALE_Y_EXT
		  , SCALE_INTERVAL, SCALE_TICK 
		  , SCALE_X_POS, SCALE_Y_POS 
	);
}

void
EngineImpl::SetReminderBoxFlashColor(const std::string&	a_flashColor)
{
	_canvas->SetReminderBoxFlashColor(a_flashColor);
}

void
EngineImpl::SetReminderBoxFlashTime(unsigned int a_flashStartTime, 
									unsigned int a_flashEndTime)
{
	_canvas->SetReminderBoxFlashTime(	a_flashStartTime, 
										a_flashEndTime);
}

void
EngineImpl::SetReminderBoxDisplayTime(int a_displayStartTime,
									  int a_displayEndTime)
{
	_canvas->SetReminderBoxDisplayTime(	a_displayStartTime, 
										a_displayEndTime);
}

void 
EngineImpl::SetCanvasView(CanvasView* a_canvasView)
{
    _canvas->SetCanvasView(a_canvasView);
}

void
EngineImpl::SetInfoBox(pact::TrialInfoBoxParams a_params)
{
    _canvas->CreateInfoBox(a_params);
}

void 
EngineImpl::ShowClock(bool a_show)
{
    _canvas->ShowClock(a_show);
}

void
EngineImpl::ProcessReminderBox(const ReminderBoxParams&	a_params)
{
	_canvas->AddReminderBox(a_params);
}

void 
EngineImpl::ProcessSoundFile(	const std::string&	a_fileName,
								const int&			a_startTime,
								const int&			a_endTime)
{
	_canvas->AddSoundFile(a_fileName, a_startTime, a_endTime);
}

void 
EngineImpl::SetSoundFilePlayTime(	const std::string&	a_fileName,
									const int&			a_startTime,
									const int&			a_endTime)
{
	_canvas->SetSoundFilePlayTime(a_fileName, a_startTime, a_endTime);
}

void 
EngineImpl::ProcessDivisionLine(const std::string&	a_id,
								const int&			a_x1,
								const int&			a_y1,
								const int&			a_x2,
								const int&			a_y2,
								const QColor&		a_color,
								const uint&			a_width)
{
	_canvas->AddDivisionLine (a_id, a_x1, a_y1, a_x2, a_y2, a_color, a_width);
}
		
void 
EngineImpl::SetDivisionLineDisplayTime (const std::string& a_id,
										const int& a_displayStartTime,
										const int& a_displayEndTime)
{
	_canvas->SetDivisionLineDisplayTime(a_id, a_displayStartTime, a_displayEndTime);
}

void 
EngineImpl::SetInterruptParams(pact::InterruptionParams a_params)
{
    _atclab->SetInterruptParams(a_params);
}

void 
EngineImpl::SetnBackTaskParams(pact::nBackTaskParams a_params)
{
    _atclab->SetnBackTaskParams(a_params);
}

void 
EngineImpl::SetNotifyMissedAcceptance(bool a_notify)
{
    _canvas->SetNotifyMissedAcceptance(a_notify);
}

void 
EngineImpl::SetNotifyMissedHandoff(bool a_notify)
{
    _canvas->SetNotifyMissedHandoff(a_notify);
}

void 
EngineImpl::SetDisableCalloutRotation(bool a_disable)
{
    _canvas->SetDisableCalloutRotation(a_disable);
}
//------------------------------------------------------------------------------

/*!
 * Start/Stop the engine 
 *
 * @todo: remove from builder
 */
void EngineImpl::start() 
{ 
	_atclab->start(); 
	_canvas->resume();
}

void EngineImpl::stop()  
{ 
	_atclab->stop(); 
	_canvas->pause();
}

/*!
 * Pauses the Animation of the Canvas. The Engine clock is still running so that
 * the pause interval will be included in time tags.
 *
 * @todo inline this function
void EngineImpl::pause(){
	_canvas->pause();
}
 */

/*!
 * If the Animation is in a paused state then resume will continue.
 *
 * @todo inline this function
void EngineImpl::resume(){
	_canvas->resume();
}
 */

/*!
 * Stops the updates of the canvas. The canvas will need to be reset before 
 * restarting.
void EngineImpl::stop(){
	_canvas->stop();
}
 */

/*!
void EngineImpl::reset(){
	_canvas->reset();
}
 */
/*!
void EngineImpl::clear(){
	_canvas->clear();
}
 */

//------------------------------------------------------------------------------

/*!
 * Creates a view of the Engine's Canvas (Document Object). The View's parent
 * is set so that when the parent goes out of scope the view is automatically
 * cleaned up.
 *
 * Currently returns a QWidget* - but not sure if additional functionality may
 * be required from higher up the inheritance tree?
 */
QWidget* EngineImpl::create_view( QWidget* parent, const char* name ) const {
/*
	if ( !_canvas )
		throw engine_error("engine not initialized");
	if ( !parent )
		throw engine_error(
			"must assign a valid parent object for view"
		);
*/

	QCanvas * cptr = _canvas.get();

	// create new view
	return new CanvasView( cptr, parent, name );
}

void EngineImpl::HandleSecondsUpdate(long seconds)
{
	int i = 0;

	for (std::map<unsigned int, bool>::iterator it = _time_events.begin();
			it != _time_events.end(); it++)
	{
		if ((!it->second) && (seconds >= it->first))
		{
			it->second = true;
			emit TimeEvent(i);
			break;
		}
		i++;
	}
}
//------------------------------------------------------------------------------

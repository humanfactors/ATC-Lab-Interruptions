/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: airspace.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.13.2.1 $ )
      @date     : $Date: 2014/12/10 13:26:18 $
      @state    : $State: Exp $

      $Log: airspace.cpp,v $
      Revision 1.13.2.1  2014/12/10 13:26:18  Aaron Yeung
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

      Revision 1.13  2009/04/27 13:37:44  Aaron Yeung
      Cosmetic cleanup

      Revision 1.12  2008/07/12 19:44:34  Aaron Yeung
      Added a pre handoff state to aircraft so that they flash blue when they exit the sector

      Revision 1.11  2008/06/30 11:07:50  Aaron Yeung
      Added aircraft handoff task

      Revision 1.10  2007/08/08 14:47:52  Aaron Yeung
      Exclude points scoring for resolving an aircraft conflict when its conflicting pair has already been resolved

      Revision 1.9  2007/08/01 15:14:37  Aaron Yeung
      Added scoring for conflicts

      Revision 1.8  2007/07/22 07:34:27  Aaron Yeung
      Added dialog box when pairs of aircraft conflict

      Revision 1.7  2007/07/15 06:27:39  Aaron Yeung
      Fixed Initial control states of aircraft

      Revision 1.6  2007/07/10 13:48:15  Aaron Yeung
      Added configurable values (hdom, vdom) to calculate aircraft conflicts

      Revision 1.5  2007/07/10 11:51:04  Aaron Yeung
      Fixed aircraft states

      Revision 1.4  2007/07/09 12:40:24  Aaron Yeung
      Modified criteria for aircraft to go into proposed state. Originally, an aircraft would go into the proposed state (CS_PROPOSED) when it was 2 minutes away from the sector boundary. Modified so that aircraft go into the proposed state at a fixed distance (configurable via XML) from the sector boundary

      Revision 1.3  2007/07/09 08:36:51  Aaron Yeung
      no message

      Revision 1.2  2007/07/08 04:48:02  Aaron Yeung
      Added UpdateAircraft timer handler to set an aircraft active x seconds (determined by XML config fille) after the trial has started

      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.10  2006/11/02 06:48:53  seth
      improve arc resolution

      Revision 1.9  2006/10/26 05:59:52  seth
      working implementation of experiment time logging

      Revision 1.8  2006/10/25 04:49:58  seth
      fixing logging

      Revision 1.7  2006/10/18 03:58:33  seth
      snap functionality for selection & fixed bug causing access violation once aircraft had reached destination

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

#include "airspace.h"
#include "Utilities.h"
#include "atc.h"
#include "aircraft_agent.h"
#include "aircraft_data.h"
#include "area_definition.h"
#include <algorithm>
#include <qpointarray.h>
#include <qwmatrix.h>

#ifdef WIN32
	#include <assert.h>
#endif


using namespace atc;

using std::deque;
using std::pair;
using std::string;


////////////////////////////////////////////////////////////////////////////////
//
// atc::Airspace
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
Airspace::Airspace()
	  : _aircraft()
	  , _sector()	  
	  , _hdoms( NAN )
	  , _vdoms( NAN )
{}

/*!
 *
 */
Airspace::~Airspace() {}


//------------------------------------------------------------------------------
// [public] setters
//

/*!
 * Add an aircraft to the airspace.
 *
 * The aircraft agent's destruction is handled by the QObject parent that was 
 * set at construction time. The parent may or may not be the airspace.
 */
void Airspace::add_aircraft( AircraftAgent *ac ) {
	_aircraft.insert( std::make_pair( ac->callsign(), ac ) );
	connect( 
		    ac,   SIGNAL( event_recorded( KeyValues & ) )
		  , this, SIGNAL( event_recorded( KeyValues & ) )
	);

	connect(ac, SIGNAL(AircraftConflictResolved(std::string, std::string, int, int, int)),
			this, SIGNAL(AircraftConflictResolved(std::string, std::string, int, int, int)));

	connect(ac, SIGNAL(AircraftConflictResolved(std::string, std::string, int, int, int)),
			this, SLOT(HandleAircraftConflictResolved(std::string, std::string, int, int, int)));

	connect(ac, SIGNAL(AircraftConflictFalseAlarm()),
			this, SIGNAL(AircraftConflictFalseAlarm()));
}

/*!
 *
 */
void Airspace::define_sector( const AreaDefinition &def ) {
	QPointArray pts;
	_sector = QRegion( area_points( def, pts ) );
}

/*!
 *
 */
void Airspace::add_non_sector( const AreaDefinition &def ) {
	QPointArray pts;
	_nonsector.push_back( QRegion( area_points( def, pts ) ) );
}

//------------------------------------------------------------------------------
// [public] initialization
//

/*!
 *
 */
void Airspace::initialize() {
	std::for_each( 
			_aircraft.begin(), _aircraft.end(), InitialControlState( this ) 
	);
}

/*!
 *
 */
bool Airspace::point_in_sector( const Point &p ) const {
	QPoint pos( p.first, p.second );

	QRegionList::const_iterator non = _nonsector.begin();
	for ( ; non != _nonsector.end(); ++non ) {
		if ( non->contains( pos ) ) {
			return false;
		}
	}

	return _sector.contains( pos );
}


//------------------------------------------------------------------------------
// [public slots]
//

/*!
 * Advance the airspace by time period
 *
 * @param ms time in milli-seconds
 */
void Airspace::advance( int ms ) {

	std::for_each( _aircraft.begin(), _aircraft.end(), AdvancePosition( ms ) );
	std::for_each( 
			_aircraft.begin(), _aircraft.end(), UpdateControlState( this ) 
	);
	std::for_each( 
			_aircraft.begin(), _aircraft.end()
		  , DetectConflicts( &_aircraft, _hdoms, _vdoms ) 
	);
	emit advanced();
}

void Airspace::UpdateAircraft(long seconds)
{
	for (std::map<std::string, AircraftAgent *>::iterator it = _aircraft.begin(); it != _aircraft.end(); it++)
	{
		it->second->UpdateAircraft(seconds);
	}
}

//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 *
 */
QPointArray& Airspace::area_points( 
		const AreaDefinition &area_def
	  , QPointArray &pts_ref 
) {
	// validate point array
	if ( ! pts_ref.isNull() ) {
		throw airspace_error( "Cannot process area: point array is not empty!" );
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
			throw airspace_error("Unknown area descriptor!");
		}
	}
	return pts_ref;
}

/*!
 * Calculate the bearing from _*canvas*_ point p1 to point p2 (degrees)
 *
 * The points are already converted to canvas space which has an upside down
 * geometry ie +ve y is down. Bearings are still however measured in clockwize
 * rotation from the y-axis;
 * 
 */
double Airspace::bearing(QPoint p1, QPoint p2)const 
{
	double dy = p2.y() - p1.y();
	double dx = p2.x() - p1.x();

	double a = 
		PI - 
		(
			dx != 0 
			? atan(dy / dx) 
			: (dy < 0 ? - PI/2 : PI/2) 
		);

	if ( dx >= 0 ) a += PI * ( dy < 0 ? -1 : 1);
	// slightly confusing because canvas space (like screen) is upside down

	return a * 180 / PI;
}


////////////////////////////////////////////////////////////////////////////////
//
// atc::Airspace::AdvancePosition
//
//------------------------------------------------------------------------------
// [public] interface
//

/*!
 * 
 */
void Airspace::AdvancePosition::operator ()( ACAgentMapPair pair ) const { 
	pair.second->advance_position( _time );
}


////////////////////////////////////////////////////////////////////////////////
//
// atc::Airspace::DetectConflicts
//
//------------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
Airspace::DetectConflicts::DetectConflicts( 
	    ACAgentMap *map, double hdoms, double vdoms
)
	  : _map( map ), _hdoms( hdoms), _vdoms( vdoms )
{}

/*!
 *
 */
void Airspace::DetectConflicts::operator ()( ACAgentMapPair pair ) const 
{
	AircraftAgent *ac = pair.second;
	ac->clear_conflict();
	
	if (_map->end() != std::find_if(_map->begin(), 
						_map->end(), 
						IsConflicted(ac, _hdoms, _vdoms))) 
	{
		ac->set_conflict();
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// atc::Airspace::IsConflicted
//
//------------------------------------------------------------------------------
//  atc::Canvas::IsConflicted
//

/*!
 *
 */
Airspace::IsConflicted::IsConflicted( 
		AircraftAgent *ac, double hdoms, double vdoms 
)
	  : _ac( ac ), _hdoms( hdoms ), _vdoms( vdoms ) 
{}

/*!
 *
 */
bool Airspace::IsConflicted::operator ()( ACAgentMapPair pair ) const {
	AircraftAgent *ac = pair.second;
	if ( ac == _ac ) return false;
	return ac->collides_with( _ac, _hdoms, _vdoms );
}

int Airspace::GetTimeToTravelToDistance(AircraftAgent* ac, double distance) const
{
	return (distance / (ac->speed() / (SIXTY_SQR * ONE_THOUSAND)));
}

void
Airspace::SetAircraftAutoAccept(bool a_autoAccept)
{
    for (std::map<std::string, AircraftAgent*>::iterator it=_aircraft.begin();
            it != _aircraft.end(); it++)
    {
        it->second->SetAutoAccept(a_autoAccept);
    }
}

void
Airspace::SetAircraftAutoHandoff(bool a_autoHandoff)
{
    for (std::map<std::string, AircraftAgent*>::iterator it=_aircraft.begin();
            it != _aircraft.end(); it++)
    {
        it->second->SetAutoHandoff(a_autoHandoff);
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// atc::Airspace::InitialControlState
//
//------------------------------------------------------------------------------
//  [public]
//

/*!
 *
 */
void Airspace::InitialControlState::operator ()( ACAgentMapPair pair ) const {
	AircraftAgent *ac = pair.second;

	bool p0 = _airspace->point_in_sector(ac->position() );
	bool p2 = _airspace->point_in_sector(ac->position(TWO_MINUTES));
	bool p5 = _airspace->point_in_sector(ac->position(FIVE_MINUTES));
	bool p10 = _airspace->point_in_sector(ac->position(TEN_MINUTES));
	bool p15 = _airspace->point_in_sector(ac->position(FIFTEEN_MINUTES));
	bool p20 = _airspace->point_in_sector(ac->position(TWENTY_MINUTES));
	bool point_at_entry_thresh  = _airspace->point_in_sector(ac->position(
											_airspace->GetTimeToTravelToDistance(ac, _airspace->GetSectorEntryDistance())));

	ControlState cs = 
			(!p0 && !p2 && !p5 && !p10 && !p15 && !p20 && !point_at_entry_thresh) ? CS_HANDOFF
		  : (!p0 && !point_at_entry_thresh && (p2 || p5 || p10 || p15 || p20)) ? CS_NONE
		  : (!p0 && point_at_entry_thresh) ? CS_PROPOSED
		  :                          CS_ACCEPTED_TASK_INCOMPLETE
	;
	ac->set_control( cs );
}

////////////////////////////////////////////////////////////////////////////////
//
// atc::Airspace::UpdateContolState
//
//------------------------------------------------------------------------------
//  [public]
//

/*!
 *
 */
void Airspace::UpdateControlState::operator ()( ACAgentMapPair pair ) const {
	AircraftAgent *ac = pair.second;
	if ( ac->is_active() )
		ac->set_control( updated_control( ac ) );
}

//------------------------------------------------------------------------------
//  [private]
//

/*!
 *
 */
ControlState 
Airspace::UpdateControlState::updated_control( AircraftAgent *ac ) const {
	bool p0  = _airspace->point_in_sector(ac->position());
	bool p2  = _airspace->point_in_sector(
				ac->position(_airspace->GetTimeToTravelToDistance(ac, _airspace->GetSectorEntryDistance())));
	bool pos_at_sector_exit_thresh = _airspace->point_in_sector( 
				ac->position(-1 * (_airspace->GetTimeToTravelToDistance(ac, _airspace->GetSectorExitDistance()))));
	bool p15 = _airspace->point_in_sector( ac->position( FIFTEEN_MINUTES ) );

	ControlState cs = ac->control();

	switch ( cs ) 
	{
	case CS_NONE:      
		//if (p15)
		//{
		//	return CS_ANNOUNCED;
		//}
		//break;
	case CS_ANNOUNCED: 
		if (p2) 
		{
			return CS_PROPOSED;
		}
		break;
	case CS_ACCEPTED_TASK_COMPLETE:	
	case CS_ACCEPTED_TASK_INCOMPLETE:
		if (!pos_at_sector_exit_thresh && !p2)
		{
			return CS_PRE_HANDOFF;
		}
		break;
	default:                            
		break;
	}

	return cs;
}

void Airspace::HandleAircraftConflictResolved(std::string conflict_aircraft1, 
												std::string conflict_aircraft2, 
												int acceptance_time, 
												int conflict_time, 
												int current_time)
{
	std::map<std::string, AircraftAgent*>::iterator it = _aircraft.find(conflict_aircraft1);

	if (it != _aircraft.end())
	{
		it->second->SetConflictResolved(true);
	}

	it = _aircraft.find(conflict_aircraft2);

	if (it != _aircraft.end())
	{
		it->second->SetConflictResolved(true);
	}
}
////////////////////////////////////////////////////////////////////////////////

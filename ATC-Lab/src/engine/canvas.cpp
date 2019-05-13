/**
 * @file:       atc_canvas.cpp
 *
 *              ATC_Engine project.
 *              Model component.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:34:37 PM 01/12/2003
 *
 * $Id: canvas.cpp,v 1.22.2.7 2015/08/19 11:33:56 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifdef _WIN32
	#include <assert.h>
#endif

#include "canvas.h"

#include "aircraft_agent.h"
#include "aircraftsprite.h"
#include "aircraft_data.h"
#include "canvas_tools.h"
#include "Utilities.h"
#include "aircraftparam.h"
#include "routearcsprite.h"
#include "scale.h"
#include "sectorarcsprite.h"
#include "sectorsprite.h"
#include "transformation.h"
#include "waypointsprite.h"
#include "ScoreLogger.h"
#include "ExcelLogger.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <qdialog.h>
#include <qpainter.h>
#include <winnt.h>
#include <iostream.h>
using namespace atc;

////////////////////////////////////////////////////////////////////////////////
//
// atc::Canvas
//
//------------------------------------------------------------------------------
// class constants
//

/*!
 * The Chunk Count defines how many blocks to break the canvas into.
 */
const int Canvas::CHUNK_COUNT = 50;

//------------------------------------------------------------------------------
// Constrution/Destruction
//

/*!
 * Default Constructor
 */
Canvas::Canvas(QObject *parent, const char *name)
	  : QCanvas   ( parent, name )
	  , _aircraft ()
	  , _2internal( 0 )
	  , _scale    ( 0 )
	  , _brl      ( 0 )
	  , _vector   ( 0 )
	  , m_reminderBox(NULL)
	  , m_currentTime(0)
      , m_canvasView(NULL)
      , m_interruptTask(NULL)
      , m_interruptParams()
      , m_nBackTask(NULL)
      , m_nBackTaskParams()
      , m_runMode(RUN_MODE_NORMAL)
      , m_notifyMissedAcceptance(true)
      , m_notifyMissedHandoff(true)
      , m_disableCalloutRotation(false)
      , m_info_box(NULL)
      , m_showClock(true)
{
	setBackgroundColor( SECTOR_BRUSH.color() );
}

/*!
 * Destructor
 */
Canvas::~Canvas() 
{
	if ( _vector      ) delete _vector;
	if ( _scale       ) delete _scale;

	WayPointVector::const_iterator wp = _waypoints.begin();
	for ( ; wp != _waypoints.end(); ++wp ) 
		delete *wp;
	
	if ( _2internal ) delete _2internal;
	
	ACSpriteMap::const_iterator s = _aircraft.begin();
	for ( ; s != _aircraft.end(); ++s )
		delete s->second;

	for (std::map<std::string, DivisionLine*>::iterator it = m_divisionLines.begin();
			it != m_divisionLines.end(); it++)
	{
		delete it->second;
	}

	for (std::vector<SoundFile*>::iterator it2 = m_soundFiles.begin();
			it2 != m_soundFiles.end(); it2++)
	{
		TCHAR* pFileName = new TCHAR[(*it2)->m_fileName.length()];

		int result = mbstowcs(pFileName, (*it2)->m_fileName.c_str(), -1);

		if (result > 0)
		{
			StopPlayingMediaFile(pFileName);
		}

		delete *it2;
	}
}

//------------------------------------------------------------------------------
// [public slots] virtual
//

/*!
 * Display the update counter with a specific format string.
 */
void Canvas::show_clock( ) {
	_clock     = new QCanvasText( "--:--:--",   CANVAS_FONT, this );
	_tool_name = new QCanvasText( " ---      ", CANVAS_FONT, this );

	QRect cr( _clock->boundingRect() );
	QRect tr( _tool_name->boundingRect() );
	int xcoord = this->width() / 2;
	int ycoord = this->height() - 2 * cr.height();

	_clock->setColor( Qt::black );
	_clock->move( xcoord - cr.width(), ycoord );
	_clock->setVisible(m_showClock);

	_tool_name->setColor( Qt::blue );
	_tool_name->move( xcoord + 10, ycoord );
	_tool_name->setVisible( true );

	QCanvasRectangle *rc = new QCanvasRectangle( _clock->boundingRect(), this );
	rc->setPen( QPen( Qt::black, 2 ) );

    if (m_showClock)
    {
	    rc->show();
    }

	QCanvasRectangle *rn = new QCanvasRectangle( _tool_name->boundingRect(), this );
	rn->setPen( QPen( Qt::black, 2 ) );
	rn->show();
}

void 
Canvas::SetCanvasView(CanvasView* a_canvasView)
{
    m_canvasView = a_canvasView;

    if (!m_interruptTask.get())
    {
        m_interruptTask = std::auto_ptr<InterruptTask>(new InterruptTask(a_canvasView, m_interruptParams));
    }

    if (!m_nBackTask.get())
    {
        m_nBackTask = std::auto_ptr<nBackTask>(new nBackTask(a_canvasView, m_nBackTaskParams));

        connect(m_nBackTask.get(), SIGNAL(event_recorded(KeyValues &)),
                this, SIGNAL(event_recorded(KeyValues &)));
    }

    ACSpriteMap::iterator ac =  _aircraft.begin();
	for ( ; ac != _aircraft.end(); ++ac ) 
    {
	    ac->second->SetCanvasView(a_canvasView);
	}
}

void
Canvas::SetReminderBoxFlashColor(const std::string& a_flashColor)
{
	m_reminderBox->SetFlashColor(a_flashColor);
}

void
Canvas::SetReminderBoxFlashTime(unsigned int		a_flashStartTime,
								unsigned int		a_flashEndTime)
{
	m_reminderBox->SetFlashTime(a_flashStartTime, a_flashEndTime);
}

void
Canvas::SetReminderBoxDisplayTime(	int a_displayStartTime,
									int a_displayEndTime)
{
	m_reminderBox->SetDisplayTime(a_displayStartTime, a_displayEndTime);
}

void
Canvas::AddReminderBox(const ReminderBoxParams& a_params)
{
	m_reminderBox = std::auto_ptr<ReminderBox>(new ReminderBox(this, a_params));
}

void 
Canvas::AddSoundFile(	const std::string&	a_fileName, 
						const int&			a_startTime, 
						const int&			a_endTime)
{
	bool found = false;

	for (std::vector<SoundFile*>::iterator it = m_soundFiles.begin();
			it != m_soundFiles.end(); it++)
	{
		if ((*it)->m_fileName == a_fileName)
		{
			found = true;

			(*it)->m_startTimes.insert(a_startTime);
			(*it)->m_endTimes.insert(a_endTime);
		}
	}

	if (!found)
	{
		SoundFile* sf = new SoundFile;
		sf->m_fileName = a_fileName;
		sf->m_startTimes.insert(a_startTime);
		sf->m_endTimes.insert(a_endTime);

		m_soundFiles.push_back(sf);
	}
}

bool 
Canvas::SetSoundFilePlayTime(	const std::string&	a_fileName, 
								const int&			a_startTime, 
								const int&			a_endTime)
{
	bool ret = false;

	for (std::vector<SoundFile*>::iterator it = m_soundFiles.begin();
			it != m_soundFiles.end(); it++)
	{
		if ((*it)->m_fileName == a_fileName)
		{
			(*it)->m_startTimes.insert(a_startTime);
			(*it)->m_endTimes.insert(a_endTime);
			ret = true;
			break;
		}
	}

	return ret;
}

void
Canvas::SetRunMode(RUN_MODE a_runMode)
{
    m_runMode = a_runMode;
}

void
Canvas::ShowInterruptScreen()
{
    ExcelLogging::ExcelLogger::instance()->LogInterruptionDetails(  "Interruption", 
                                                                    m_interruptParams.m_start,
                                                                    m_interruptParams.m_end,
                                                                    m_interruptParams.m_show_blank_screen,
                                                                    m_interruptParams.m_external_program,
                                                                    m_interruptParams.m_auto_accept,
                                                                    m_interruptParams.m_auto_handoff);

    KeyValues kvs;
    kvs.push_back(std::make_pair(std::string("interruption_type"),  std::string("Interruption")));
    kvs.push_back(std::make_pair(std::string("start_time" ),        to_string(m_interruptParams.m_start)));
	kvs.push_back(std::make_pair(std::string("end_time" ),          to_string(m_interruptParams.m_end)));
    kvs.push_back(std::make_pair(std::string("show_full_screen" ),  to_string(m_interruptParams.m_show_blank_screen)));
    kvs.push_back(std::make_pair(std::string("external_program" ),  to_string(m_interruptParams.m_external_program)));
    kvs.push_back(std::make_pair(std::string("auto_accept" ),       to_string(m_interruptParams.m_auto_accept)));
    kvs.push_back(std::make_pair(std::string("auto_handoff" ),      to_string(m_interruptParams.m_auto_handoff)));
    
    emit event_recorded(kvs);

    m_interruptTask->Start();
}

void
Canvas::HideInterruptScreen()
{
    m_interruptTask->Stop();
}

void 
Canvas::CreateInfoBox(pact::TrialInfoBoxParams a_params)
{
    if (!m_info_box.get())
    {
        m_info_box = std::auto_ptr<QCanvasText>(new QCanvasText(a_params.m_text.c_str(), 
                                                                QFont(a_params.m_font.c_str(), a_params.m_font_size), 
                                                                this));
        m_info_box->setText(a_params.m_text);
        m_info_box->setColor(Utils::Utilities::instance()->GetQColorFromString(a_params.m_font_colour));
	    m_info_box->move(a_params.m_x, a_params.m_y);

        if (a_params.m_border)
        {
            QCanvasRectangle* rn = new QCanvasRectangle(m_info_box->boundingRect(), this);
	        rn->setPen(QPen(Utils::Utilities::instance()->GetQColorFromString(a_params.m_font_colour), 2));
	        rn->show();
        }

	    m_info_box->setVisible( true );
    }
}

void
Canvas::ShowClock(bool a_show)
{
    m_showClock = a_show;
}

void
Canvas::SetInterruptParams(pact::InterruptionParams a_params)
{
    m_interruptParams = a_params;
}

void 
Canvas::SetnBackTaskParams(pact::nBackTaskParams a_params)
{
    m_nBackTaskParams = a_params;
}

void
Canvas::ShowNBackTask()
{
    ExcelLogging::ExcelLogger::instance()->LogInterruptionDetails(  "NBack", 
                                                                    m_nBackTaskParams.m_start,
                                                                    m_nBackTaskParams.m_end,
                                                                    m_nBackTaskParams.m_show_task,
                                                                    std::string(""),
                                                                    m_nBackTaskParams.m_auto_accept,
                                                                    m_nBackTaskParams.m_auto_handoff);

    KeyValues kvs;
    kvs.push_back(std::make_pair(std::string("interruption_type"),  std::string("NBack")));
    kvs.push_back(std::make_pair(std::string("start_time" ),        to_string(m_nBackTaskParams.m_start)));
	kvs.push_back(std::make_pair(std::string("end_time" ),          to_string(m_nBackTaskParams.m_end)));
    kvs.push_back(std::make_pair(std::string("show_full_screen" ),  to_string(m_nBackTaskParams.m_show_task)));
    kvs.push_back(std::make_pair(std::string("external_program" ),  std::string("")));
    kvs.push_back(std::make_pair(std::string("auto_accept" ),       to_string(m_nBackTaskParams.m_auto_accept)));
    kvs.push_back(std::make_pair(std::string("auto_handoff" ),      to_string(m_nBackTaskParams.m_auto_handoff)));
    
    emit event_recorded(kvs);

    m_nBackTask->Start();
}

void
Canvas::HideNBackTask()
{
    m_nBackTask->Stop();
}

void 
Canvas::SetNotifyMissedAcceptance(bool a_notify)
{
    m_notifyMissedAcceptance = a_notify;
}

void 
Canvas::SetNotifyMissedHandoff(bool a_notify)
{
    m_notifyMissedHandoff = a_notify;
}

void 
Canvas::SetDisableCalloutRotation(bool a_disable)
{
	m_disableCalloutRotation = a_disable;
}

void Canvas::show_score( ) 
{
	_total_score = new QCanvasText( "      ",   CANVAS_FONT, this );

	QRect sr(_total_score->boundingRect());
	int xcoord = this->width();
	int ycoord = this->height()/2 - 2 * sr.height();

	_total_score->setColor(Qt::black);
	_total_score->move(xcoord - (sr.width() + 50), ycoord);
	_total_score->setVisible(true);

	QCanvasRectangle *rc = new QCanvasRectangle(_total_score->boundingRect(), this );
	rc->setPen(QPen(Qt::black, 2));
	rc->show();

	float totalAcceptanceTaskScore	= 0.0;
	float totalConflictScore		= 0.0;
	float totalHandoffTaskScore	= 0.0;

	Score::ScoreLogger::instance()->GetCumulativeTotalScores(
			totalAcceptanceTaskScore, 
			totalConflictScore,
			totalHandoffTaskScore);

	std::stringstream total_score;
		
	total_score << (totalAcceptanceTaskScore + totalConflictScore + totalHandoffTaskScore);

	QString display_score = total_score.str();

	_total_score->setText(display_score);
}

bool Canvas::PlayMediaFile(LPCTSTR szFile)
{
	TCHAR szOpenCommandString[1000];
    TCHAR szPlayCommandString[1000];

    wsprintf(szOpenCommandString, TEXT("open \"%s\" type mpegvideo alias MediaFile"), szFile);
	wsprintf(szPlayCommandString, TEXT("play MediaFile"));
	
	if ((mciSendString(szOpenCommandString, NULL, 0, NULL) == ERROR_SUCCESS)  &&
        (mciSendString(szPlayCommandString, NULL, 0, NULL) == ERROR_SUCCESS))
	{
		return true;
	}

	return false;
}

bool
Canvas::StopPlayingMediaFile(LPCTSTR szFile)
{
	TCHAR szCommandString[1000];

    wsprintf(szCommandString, TEXT("open \"%s\" type mpegvideo alias MediaFile"), szFile);

	if ((mciSendString(TEXT("stop MediaFile"), NULL, 0, NULL) == ERROR_SUCCESS) &&
		(mciSendString(TEXT("close MediaFile"), NULL, 0, NULL) == ERROR_SUCCESS))
	{
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
// [public slots] virtual
//

/*!
 * Update clock
 */
void Canvas::set_clock( long seconds ) {
	m_currentTime = seconds;

	int s = seconds % SIXTY;
	int m = seconds / SIXTY;
	int h =       m / SIXTY;
		m =       m % SIXTY;

    if (m_showClock)
    {
	    _clock->setText( QString().sprintf( "%02d:%02d:%02d", h,m,s ) );

	    update();
    }

	for (std::map<std::string, DivisionLine*>::iterator it = m_divisionLines.begin();
			it != m_divisionLines.end(); it++)
	{
		it->second->Display(seconds);
	}

	for (std::vector<SoundFile*>::iterator it2 = m_soundFiles.begin();
			it2 != m_soundFiles.end(); it2++)
	{
		if ((*it2)->m_startTimes.find(seconds) != (*it2)->m_startTimes.end())
		{
			TCHAR* pFileName = new TCHAR[(*it2)->m_fileName.length()];

			int result = mbstowcs(pFileName, (*it2)->m_fileName.c_str(), -1);

			if (result > 0)
			{
				PlayMediaFile(pFileName);
			}
		}
		else if ((*it2)->m_endTimes.find(seconds) != (*it2)->m_endTimes.end())
		{
			TCHAR* pFileName = new TCHAR[(*it2)->m_fileName.length()];

			int result = mbstowcs(pFileName, (*it2)->m_fileName.c_str(), -1);

			if (result > 0)
			{
				StopPlayingMediaFile(pFileName);
			}
		}
	}
}

void Canvas::DisableReminderBox()
{
	if (m_reminderBox.get() != NULL)
	{
		m_reminderBox->Disable();
	}
}

ReminderBox*
Canvas::GetReminderBox()
{
	ReminderBox* ret = NULL;

	if (m_reminderBox.get() != NULL)
	{
		ret = m_reminderBox.get();
	}

	return ret;
}

/*!
 * stop clock
 */
void Canvas::stop_clock() {
	_clock->setColor( Qt::red );
	update();
}

/*!
 * start clock
 */
void Canvas::start_clock() {
	_clock->setColor( Qt::black );
	update();
}


/*!
 * set tool
 */
void Canvas::set_tool( QString name ) {
	_tool_name->setText( name );
	update();
}

void Canvas::HandleScoreUpdate(
	float total_acceptance_task_score, 
	float total_conflict_score,
	float a_totalHandoffScore)
{
	std::stringstream total_score;
		
	total_score << (total_acceptance_task_score + total_conflict_score + a_totalHandoffScore);

	QString display_score = total_score.str();
	_total_score->setText(display_score);
}

//------------------------------------------------------------------------------
// [public] canvas tools interface
//

//
// Bearing Range Line
//

/*!
 *
 */
void Canvas::add_brl( int x, int y ) {
	// @todo:
	_brl = new BearingRangeLine( this );
	_brls.push_back( _brl );
	_brl->anchor( QPoint( x, y ) );
}

/*!
 *
 */
void Canvas::add_brl( AircraftSprite *s ) {
	// @todo:
	_brl = new BearingRangeLine( this );
	_brls.push_back( _brl );
	_brl->anchor( s );
}

/*!
 *
 */
void Canvas::add_brl( WayPointSprite *s ) {
	_brl = new BearingRangeLine( this );
	_brls.push_back( _brl );
	_brl->anchor( s );
}

/*!
 *
 */
void Canvas::hook_brl( int x, int y ) {
	_brl->hook( QPoint( x, y ) );
}

/*!
 *
 */
void Canvas::hook_brl( AircraftSprite *s ) {
	_brl->hook( s );
}

/*!
 *
 */
void Canvas::hook_brl( WayPointSprite *s ) {
	_brl->hook( s );
}

/*!
 *
 */
void Canvas::select_brl( BearingRangeLine *brl ) {
	_brl = brl;
}

/*!
 *
 */
void Canvas::remove_brl( BearingRangeLine *brl ) {
	_brls.remove( brl );
	delete( brl );
	update();
}

//
// Scale
//

/*!
 * Add a scale indicator to the canvas
 */
void Canvas::add_scale(
	double x, double y, double xextent, double yextent, double interval, double tick
){
	_scale = new ScaleXHair( 
			this
		  , to_internal_length( xextent ), to_internal_length( yextent )
		  , to_internal_length( interval ), to_internal_length( tick ) 
	);
	_scale->move_by( to_internal_length( x ), to_internal_length( y ) );
}

/*!
 *
 */
void Canvas::move_scale( QPoint pt ) {
	if (_scale)
	{
		//_scale->move_by( pt.x(), pt.y() );
	}
}

//
// vector
//

/*!
 *
 */
VectorTool * Canvas::vector_tool() { return _vector; }

/*!
 *
 */
void Canvas::add_vector( AircraftSprite *s ) {
	_vector = new VectorTool( this, s );
}

/*!
 *
 */
void Canvas::move_vector( QPoint pt ) {
	_vector->set_end( pt.x(), pt.y() );
}

/*!
 *
 */
void Canvas::cancel_vector() {
	delete _vector;
	_vector = 0;
}


//------------------------------------------------------------------------------
// [public] transformation interface
//

/*!
 *
 */
double Canvas::to_internal_length( double len ) const {
	return _2internal->length( len );
}

/*!
 *
 */
std::pair< double, double > 
Canvas::to_internal_point( const double x, const double y ) const {
	return _2internal->point( x, y );
}

/*!
 *
 */
QPoint 
Canvas::to_internal_qpoint( const double x, const double y ) const {
	return _2internal->qpoint( x, y );
}

/*!
 *
 */
double Canvas::to_user_length( double len ) const {
	return _2internal->inverse_length( len );
}

/*!
 *
 */
std::pair< double, double > 
Canvas::to_user_point( const double x, const double y ) const {
	return _2internal->inverse_point( x, y );
}

/*!
 *
 */
QPoint 
Canvas::to_user_qpoint( const double x, const double y ) const {
	return _2internal->inverse_qpoint( x, y );
}

//------------------------------------------------------------------------------
// [public] initialization (map objects - integer)
//
/*!
 *
 */
void Canvas::add_active_sector( const AreaDefinition &def ) {
	QPointArray pts;
	build_sector( new ActiveSectorSprite( this ), area_points( def, pts ) );
}


/*!
 *
 */
void Canvas::add_sector( const AreaDefinition &def ) {
	QPointArray pts;
	build_sector( new SectorSprite( this ), area_points( def, pts ) );
}

/*!
 * add a weather system
 */
void Canvas::add_weather( const AreaDefinition& def ) {
	QPointArray pts;
	area_points( def, pts );

	QCanvasPolygon *poly = new QCanvasPolygon( this );
	poly->setPoints( pts );
	poly->setBrush( WEATHER_BRUSH );
	poly->setZ( WEATHER_Z );
	poly->show();
}

/*!
 */
void Canvas::add_waypoint( const std::string& name, double x, double y ) {
	_waypoints.push_back( 
		new WayPointSprite( name, _2internal->qpoint( x, y ), this ) 
	);
}

/*!
 */
void Canvas::add_route( const RouteList &pts ) {
	if ( 2 > pts.size() ) return;

	RouteList::const_iterator pt = pts.begin();
	QPoint p0 = _2internal->qpoint( pt->x, pt->y );
	QPoint p1;
	++pt;
	for( ; pt != pts.end(); ++pt ) {
		p1 = _2internal->qpoint( pt->x, pt->y );

		RouteArcSprite *s = new RouteArcSprite( this );
		s->setPoints( p0.x(), p0.y(), p1.x(), p1.y() );
		s->show();

		p0 = p1;
	}
}


//------------------------------------------------------------------------------
// [public] initialization (sky objects - double precision)
//

/*!
 * Add an aircraft to the canvas
 *
 * Creates a new aircraft sprite which is a controller for the aircraft agent.
 * A list of aircraft sprites is maintained for update purposes.
 */
void Canvas::add_aircraft( AircraftAgent *ac ) {
	AircraftSprite *s = new AircraftSprite( this, ac );
    s->SetDisableCalloutRotation(m_disableCalloutRotation);

	_aircraft.insert( std::make_pair( ac->callsign(), s ) );

	s->set_visible(ac->is_active());

	connect(ac, SIGNAL( updated()     ), s, SLOT( invalidate()    ) );
	connect(ac, SIGNAL( waypoint()    ), s, SLOT( next_waypoint() ) );
	connect(ac, SIGNAL( deactivated() ), s, SLOT( destroy()       ) );
	connect(ac, SIGNAL(FailedAcceptanceTask(const char*)),
			this, SIGNAL(FailedAcceptanceTask(const char*)));
	connect(ac, SIGNAL(FailedAcceptanceTask(const char*)),
			this,  SLOT(HandleAircraftFailedAcceptanceTask(const char*)));
	connect(ac, SIGNAL(FailedHandoffTask(const char*)),
			this, SIGNAL(FailedHandoffTask(const char*)));
	connect(ac, SIGNAL(FailedHandoffTask(const char*)),
			this, SLOT(HandleAircraftFailedHandoffTask(const char*)));
	connect(this, SIGNAL(pause_request()), ac, SLOT(HandlePauseRequest()));
	connect(this, SIGNAL(resume_request()), ac, SLOT(HandleResumeRequest()));
	connect(ac, SIGNAL(Conflicted(std::pair<std::string, std::string>)),
			this, SIGNAL(Conflicted(std::pair<std::string, std::string>)));
	/*
	connect(ac,	SIGNAL(Conflicted(std::pair<std::string, std::string>)), 
			this, SLOT(HandleAircrafConflict(std::pair<std::string, std::string>)));
	*/
}


//------------------------------------------------------------------------------
// public [slots]
//

/*!
 * Update all aircraft
 */
void Canvas::update_aircraft() {
	ACSpriteMap::iterator ac =  _aircraft.begin();
	for ( ; ac != _aircraft.end(); ++ac ) {
		ac->second->update(m_currentTime);
	}

	BRLList::iterator brl = _brls.begin();
	for ( ; brl != _brls.end(); ++brl ) {
		(*brl)->update();
	}

	if ( _vector )
		_vector->update();

	update();
}

void Canvas::HandleTrialTimeout(long seconds)
{
	emit TrialTimeout();
}

void Canvas::HandleAircraftFailedAcceptanceTask(const char* callsign)
{
    if ((m_runMode == RUN_MODE_NORMAL) && (m_notifyMissedAcceptance))
    {
	    std::string msg = "Failed to complete aircraft acceptance task for " + std::string(callsign);
	    std::auto_ptr<QDialog> error_dialog = Utils::Utilities::instance()->CreateDialogBox("Error", msg);
	
	    connect(this, SIGNAL(TrialTimeout()), error_dialog.get(), SLOT(reject()));

	    error_dialog->exec();
    }
}

void 
Canvas::HandleAircraftFailedHandoffTask(const char* callsign)
{
    if ((m_runMode == RUN_MODE_NORMAL) && (m_notifyMissedHandoff))
    {
	    std::string msg = "Failed to complete aircraft handoff task for " + std::string(callsign);
	    std::auto_ptr<QDialog> errorDialog = Utils::Utilities::instance()->CreateDialogBox("Error", msg);

	    connect(this, SIGNAL(TrialTimeout()), errorDialog.get(), SLOT(reject()));

	    errorDialog->exec();
    }
}

/*
void Canvas::HandleAircrafConflict(std::pair<std::string, std::string> conflicting_aircrafts)
{
	bool found = false;
	std::vector<std::pair<std::string, std::string>*>::iterator it;

	for (it = _conflict_aircraft.begin(); it != _conflict_aircraft.end(); it++)
	{
		if ((((*it)->first == conflicting_aircrafts.first) && ((*it)->second == conflicting_aircrafts.second)) ||
			(((*it)->first == conflicting_aircrafts.second) && ((*it)->second == conflicting_aircrafts.first))) 
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		std::pair<std::string, std::string>* new_pair = new std::pair<std::string, std::string>;
		new_pair->first = conflicting_aircrafts.first;
		new_pair->second = conflicting_aircrafts.second;

		_conflict_aircraft.push_back(new_pair);

		std::string msg = "Failed to Resolve Conflict";
		std::auto_ptr<QDialog> error_dialog = Utils::Utilities::instance()->CreateDialogBox("Error", msg);

		connect(this, SIGNAL(TrialTimeout()), error_dialog.get(), SLOT(reject()));

		error_dialog->exec();
		
		delete new_pair;

		for (std::vector<std::pair<std::string, std::string>*>::iterator it2 = _conflict_aircraft.begin();
				it2 != _conflict_aircraft.end(); it2++)
		{
			_conflict_aircraft.erase(it2);
			break;
		}
	}
}
*/

//------------------------------------------------------------------------------
// public: [virtual]
//

/*!
 * Resize and Retune the Canvas from zero sized initial state.
 */
void Canvas::resize( const QRect &map ) {
	if ( _2internal )
		throw canvas_error( "canvas previously initialized" );
		
	_2internal = new Transformation();
	QRect r = _2internal->calculate( map, CANVAS_SIZE_HINT );

	// automatically retune on resize 
	// aiming for approx CHUNK_COUNT chunks.

	int w = r.width();
	int h = r.height();

	int dim = (w > h ? w : h);
	int ln2 = int( log(dim / CHUNK_COUNT) / log(2) ) + 1;
	int chksz = (int)pow(2, ln2);
	retune(chksz);	// quick since no resize yet.

	QCanvas::resize(w, h);

	show_clock();
	show_score();

	//AddDivisionLine("line1", 0, 0, 2525, 1875);
	//AddDivisionLine("line1", 0, 937, 2525, 937);
	//SetDivisionLineDisplayTime("line1", 0, atc::INVALID_VALUE);
}


//------------------------------------------------------------------------------
// [private]
//

/*!
 *
 */
void Canvas::build_sector( SectorSprite *ss, const QPointArray &points ) {
	//
	// create sector
	//

	ss->setPoints( points );
	ss->show();

	//
	// process the canvas points to create sector outline
	//

	QPoint p1( points.at( points.size() - 1 ) );
	for ( int i = 0; i < points.size(); ++i ) {
		
		QPoint p2( points[i] );
		
		SectorArcSprite *sas = new SectorArcSprite( this );
		sas->setPoints( p1.x(), p1.y(), p2.x(), p2.y() );
		sas->show();

		p1 = p2;
	}
}

/*!
 *
 */
QPointArray& Canvas::area_points( 
		const AreaDefinition &area_def
	  , QPointArray &pts_ref 
) {
	// validate point array
	if ( ! pts_ref.isNull() ) {
		throw canvas_error( "Cannot process area: point array is not empty!" );
	}
	
	// process area's descriptors
	AreaDefinition::const_iterator it = area_def.begin();
	for ( ; it != area_def.end(); ++it) {

		switch ( (*it)->rtti ) {
		
		case ( AreaDescriptor::RTTI_VERTEX ): {
			QPoint p( _2internal->qpoint( (*it)->x, (*it)->y ) );
			pts_ref.putPoints( pts_ref.size(), 1, p.x(), p.y() ); 
			break;
		}
		case ( AreaDescriptor::RTTI_ARC ): {
			ArcDescriptor *arc = static_cast< ArcDescriptor* >( *it );

			QPoint p0 = _2internal->qpoint( arc->x, arc->y );
			int    r  = (int)( _2internal->length( arc->r ) );

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

			QPoint p0 = _2internal->qpoint( e->x, e->y );
			int    w  = (int)( _2internal->length( e->w ) );
			int    h  = (int)( _2internal->length( e->h ) );
			int    a  = ( e->a );

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
			throw canvas_error("Unknown area descriptor!");
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
double Canvas::bearing(QPoint p1, QPoint p2)const 
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

void
Canvas::DeselectAllAircraft()
{
	for (ACSpriteMap::iterator it = _aircraft.begin(); it != _aircraft.end(); it++)
	{
		it->second->select(false);
	}
}

AircraftSprite*
Canvas::SelectedAircraft()
{
	AircraftSprite* selectedAircraft = NULL;
	ACSpriteMap::iterator it = _aircraft.begin(); 
	
	while (it != _aircraft.end())
	{
		if (it->second->selected())
		{
			selectedAircraft = it->second;
			break;
		}

		it++;
	}

	return selectedAircraft;
}

void 
Canvas::AddDivisionLine (const std::string& a_id,
								const int&		a_x1,
								const int&		a_y1,
								const int&		a_x2,
								const int&		a_y2,
								const QColor&	a_color,
								const uint&		a_width)
{
	m_divisionLines[a_id] = new DivisionLine(	this,
												a_id,
												a_x1,
												a_y1,
												a_x2,
												a_y2,
												a_color,
												a_width);
}

bool 
Canvas::SetDivisionLineDisplayTime(	const std::string& a_id,
									const int& a_startTime,
									const int& a_endTime)
{
	bool ret = false;

	std::map<std::string, DivisionLine*>::iterator it = m_divisionLines.find(a_id);

	if (it != m_divisionLines.end())
	{
		m_divisionLines[a_id]->SetDisplayTime(a_startTime, a_endTime);
		ret = true;
	}

	return ret;
}
////////////////////////////////////////////////////////////////////////////////


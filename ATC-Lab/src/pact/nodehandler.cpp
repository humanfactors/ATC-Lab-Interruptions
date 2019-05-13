/**
 *
 * ATC-Lab: Presentation Module
 * XML document node handler
 *
 * @author:     seth  
 * @version:    $Name:  $
 *              File created 00:00:00 PM 07/12/04
 *
 * $Id: nodehandler.cpp,v 1.27.2.4 2014/12/06 08:13:23 Aaron Yeung Exp $
 *
 * @copyright:  2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "nodehandler.h"

#include "atc.h"

#include "engine.h"
#include "clock.h"
#include "instructiontask.h"
#include "questiontask.h"
#include "task.h"
#include "trialtask.h"
#include "taskwidget.h"
#include "ReminderBox.h"
#include "document.h"

#include <qapplication.h>	// allWidgets()
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qvariant.h> 
#include <qwidcoll.h>		// widget list classes

#ifdef _WIN32
	#include <assert.h>
#endif

using namespace pact;


//==============================================================================
// Instruction Node Handler
//

/*!
 */
void InstructionNodeHandler::operator ()(const xml::DocumentNode* node)
{
	InstructionTask* task = dynamic_cast<InstructionTask*>(_view);

	if("text" == node->type)
	{
		task->setText(node->content);
	}
	else if("keyEvent" == node->type)
	{
		task->addKey(QKeySequence(node->content));
	}
	else if (node->type == "buttonEvent")
	{
		task->SetButton(node->content);
	}
	else if (node->type == "timeEvent")
	{
		task->SetTimeOut(atoi(node->content.c_str()));
	}
	else if (node->type == "buttonTimeEvent")
	{
		task->SetButtonTimeEvent(atoi(node->content.c_str()));
	}
	else if (node->type == "textBox")
	{
		task->SetTextbox(node->content);
	}
	else if (node->type == "showCountdown")
	{
		task->SetShowCountDown(node->content == "true");
	}
	else if (node->type == "questionBox")
	{
		_doc->process_children(node, QuestionBoxNodeHandler(_doc, _view));
	}
	else
	{
		throw nodehandler_error(
			"illegal instruction node entry <" + node->type + ">"
		);
	}
}

//==============================================================================
// Question Node Handler
//

/*!
 */
void QuestionNodeHandler::operator ()(const xml::DocumentNode* node)
{
	QuestionTask* task = dynamic_cast<QuestionTask*>(_view);

	if("text" == node->type){
		task->setText(node->content);

	}else if("keyEvent" == node->type){
		task->addKey(QKeySequence(node->content));

	}else{
		throw nodehandler_error(
			"illegal instruction node entry <" + node->type + ">"
		);
	}
}

//==============================================================================
//
// Param Node Handler
//

/*!
 */
void ParamNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);
	atc::Param& param = task->engine()->param();

	if (node->type == "sectorEntryDistance")
	{
		param.SetSectorEntryDistance(_doc->getIntegerContent(node));
	}
	else if (node->type == "sectorExitDistance")
	{
		param.SetSectorExitDistance(_doc->getIntegerContent(node));
	}
	else if("update_rate" == node->type){
		param.set_update_rate(_doc->getIntegerContent(node));

	}else if("update_multiplier" == node->type){
		param.set_update_multiplier(_doc->getIntegerContent(node));

	}else if("counter_format" == node->type){
		param.set_counter_format(_doc->getStringContent(node).c_str());
	
	}else if("horizontal_doms" == node->type){
		param.set_hdoms(_doc->getDecimalContent(node));

	}else if("vertical_doms" == node->type){
		param.set_vdoms(_doc->getDecimalContent(node));

	}else{
		throw nodehandler_error(
			"illegal param node entry <" + node->type + ">"
		);
	}
}


//==============================================================================
//
// Map
//
//------------------------------------------------------------------------------
// Map Node Handler
//

/*!
 *
 */
void MapNodeHandler::operator ()( const xml::DocumentNode* node ) {
	TrialTask* task = dynamic_cast< TrialTask* >( _view );
	atc::Map& map = task->engine()->map();

	if ( "region" == node->type ) {
		double x = atof( _doc->getAttributeValue( node, "x"     ).c_str() );
		double y = atof( _doc->getAttributeValue( node, "y"     ).c_str() );
		double w = atof( _doc->getAttributeValue( node, "x_dim" ).c_str() );
		double h = atof( _doc->getAttributeValue( node, "y_dim" ).c_str() );

		map.set_region( x, x + w, y, y + h );
	}
	else if("location" == node->type){
		std::string uid = _doc->getAttributeValue( node, "idx" );
		map.add_location(
				_doc->getAttributeValue( node, "idx" ).c_str()
			  , atof( _doc->getAttributeValue( node, "x" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "y" ).c_str() )
			  , "on" == _doc->getAttributeValue( node, "visible" )
		);
	}
	else if("route" == node->type){
		_doc->process_children(
			node, 
			MapRouteNodeHandler(_doc, _view, node)
		);

	}else if("sector" == node->type){
		_doc->process_children(
			node, 
			MapSectorNodeHandler(_doc, _view, node)
		);

	}else{
		throw nodehandler_error(
			"illegal map node entry <" + node->type + ">"
		);
	}
}

//------------------------------------------------------------------------------
// Map Route Node Handler
//

/*!
 * Constructor
 */
MapRouteNodeHandler::MapRouteNodeHandler(
	const xml::Document* doc, 
	xml::DocumentView* view,
	const xml::DocumentNode* node
)
	:xml::NodeHandler(doc, view)
	,_routeID("")
{
	_routeID = _doc->getAttributeValue(node, "idx");

	TrialTask* task = dynamic_cast<TrialTask*>(_view);
	task->engine()->map().create_route(_routeID.c_str());
}

/*!
 * Destructor
 */
MapRouteNodeHandler::~MapRouteNodeHandler()
{}

/*!
 * Decode a map route
 */
void MapRouteNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);

	const xml::DocumentNode* point = 0;

	if("point" == node->type){
		point =  node;

	}else if("pointref" == node->type){
		point = _doc->getReferencedNode(
			_doc->getAttributeValue(node, "location")
		);

	}else{
		throw nodehandler_error(
			"illegal map route node entry <" + node->type + ">"
		);
	}

	task->engine()->map().add_route_point(
			_routeID.c_str()
		  , atof( _doc->getAttributeValue( point, "x" ).c_str() )
		  , atof( _doc->getAttributeValue( point, "y" ).c_str() )
	);
}

//------------------------------------------------------------------------------
// Map Sector Node Handler
//

/*!
 * Constructor
 */
MapSectorNodeHandler::MapSectorNodeHandler(
	const xml::Document* doc, 
	xml::DocumentView* view,
	const xml::DocumentNode* node
)
	:xml::NodeHandler(doc, view)
	,_sectorID("")
{
	_sectorID = _doc->getAttributeValue(node, "idx");
	
	bool is_active = "active" == _doc->getAttributeValue( node, "status" );

	TrialTask* task = dynamic_cast<TrialTask*>(_view);
	atc::Map& map = task->engine()->map();
	map.create_sector( _sectorID.c_str() );
	
	if ( is_active ) {
		map.set_sector_active( _sectorID.c_str() );
	}
}


/*!
 * Destructor
 */
MapSectorNodeHandler::~MapSectorNodeHandler() {}


/*!
 * Decode a map sector
 */
void MapSectorNodeHandler::operator()(
		const xml::DocumentNode* node
){
	TrialTask* task = dynamic_cast< TrialTask* >( _view );
	atc::Map& map = task->engine()->map();

	if ( "arc" == node->type ) {
		map.add_sector_arc(
				_sectorID.c_str()
			  , atof( _doc->getAttributeValue( node, "x" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "y" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "r" ).c_str() )
		);
	}
	else if ( "vertex" == node->type ) {
		map.add_sector_vertex(
				_sectorID.c_str()
			  , atof( _doc->getAttributeValue( node, "x" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "y" ).c_str() )
		);
	}
	else if ( "ellipse" == node->type ) {
		// @todo:
		throw nodehandler_error( "sector ellipse *NOT* implemented!" );
	}
	else if ( "remove" == node->type ) {
		// @todo: 
		throw nodehandler_error( "remove sector area *NOT* implemented!" );
	}
	else{
		throw nodehandler_error(
			"illegal map sector node entry <" + node->type + ">"
		);
	}
}

//==============================================================================
//
// Sky
//
//------------------------------------------------------------------------------
// Sky Node Handler
//

/*!
 */
void SkyNodeHandler::operator()( const xml::DocumentNode* node ) {
	TrialTask* task = dynamic_cast< TrialTask* > ( _view );

	if ( "aircraft" == node->type ) {
		task->engine()->sky().create_aircraft(
				_doc->getAttributeValue(node, "idx" ).c_str()
			  , _doc->getAttributeValue(node, "type").c_str()
		);
		_doc->process_children( 
				node, SkyAircraftNodeHandler( _doc, _view ) 
		);
	}
	else if ( "area" == node->type ) {
		_doc->process_children(
			node, AreaNodeHandler( _doc, _view, node )
		);
	}
	else if (node->type == "reminderBox")
	{
		TrialTask* task = dynamic_cast<TrialTask*>(_view);

		int displayTime = atc::INVALID_VALUE;
		atc::DisplayPeriods displayTimes;

		if (_doc->attributeExists(node, std::string("displayTime")))
		{
			displayTime = atoi(_doc->getAttributeValue(node, "displayTime").c_str());
			displayTimes.push_back(std::make_pair<int, int>(displayTime, atc::INVALID_VALUE));
		}

		atc::ReminderBoxParams params;

		params.m_text = _doc->getAttributeValue(node, "displayText");
		params.m_textSize =	atoi(_doc->getAttributeValue(node, "displayTextSize").c_str());
		params.m_BGColor = QColor(_doc->getAttributeValue(node, "bgColor"));
		params.m_width = atoi(_doc->getAttributeValue(node, "width").c_str());
		params.m_height = atoi(_doc->getAttributeValue(node, "height").c_str());
		params.m_posX = atof(_doc->getAttributeValue(node, "posX").c_str());
		params.m_posY = atof(_doc->getAttributeValue(node, "posY").c_str());
		params.m_displayTimes =	displayTimes;

		std::string attachedToAircraft = "";
		if (_doc->attributeExists(node, std::string("aircraft")))
		{
			attachedToAircraft = _doc->getAttributeValue(node, "aircraft");
		}

		if (attachedToAircraft != "")
		{
			task->engine()->sky().AddReminderBoxToAircraft(attachedToAircraft.c_str(), params);
		}
		else
		{
			// Reminder box displayed at a specific location on the screen
			task->engine()->ProcessReminderBox(params);
		}
			
		_doc->process_children(node, ReminderNodeHandler(_doc, _view, attachedToAircraft));

	}
	else if (node->type == "soundFile")
	{
		TrialTask* task = dynamic_cast<TrialTask*>(_view);

		std::string soundFile;

		if (_doc->attributeExists(node, std::string("fileName")))
		{
			soundFile = _doc->getAttributeValue(node, "fileName");
		}

		if (soundFile != "")
		{
			task->engine()->ProcessSoundFile(soundFile);
			_doc->process_children(node, SoundFileNodeHandler(soundFile, _doc, _view));
		}
	}
	else if (node->type == "divisionLine")
	{
		TrialTask* task = dynamic_cast<TrialTask*>(_view);

		std::string color = atc::DEFAULT_DIVISION_LINE_COLOR;
		if (_doc->attributeExists(node, ATTRIBUTE_TYPE_DIVISION_LINE_COLOR))
		{
			color = _doc->getAttributeValue(node, ATTRIBUTE_TYPE_DIVISION_LINE_COLOR);
		}

		unsigned int width = atc::DEFAULT_DIVISION_LINE_WIDTH;
		if (_doc->attributeExists(node, ATTRIBUTE_TYPE_DIVISION_LINE_WIDTH))
		{
			width = atoi(_doc->getAttributeValue(node, ATTRIBUTE_TYPE_DIVISION_LINE_WIDTH).c_str());
		}

		std::string id = _doc->getAttributeValue(node, ATTRIBUTE_TYPE_DIVISION_LINE_ID);
		task->engine()->ProcessDivisionLine(id,
											atoi(_doc->getAttributeValue(node, ATTRIBUTE_TYPE_DIVISION_LINE_X1).c_str()),
											atoi(_doc->getAttributeValue(node, ATTRIBUTE_TYPE_DIVISION_LINE_Y1).c_str()),
											atoi(_doc->getAttributeValue(node, ATTRIBUTE_TYPE_DIVISION_LINE_X2).c_str()),
											atoi(_doc->getAttributeValue(node, ATTRIBUTE_TYPE_DIVISION_LINE_Y2).c_str()),
											QColor(color),
											width);

		_doc->process_children(node, DivisionLineNodeHandler(id, _doc, _view));
	}
	else{
		throw nodehandler_error(
				"illegal sky node entry <" + node->type + ">"
		);
	}
}

//------------------------------------------------------------------------------
// Sky Aircraft Node Handler
//

/*!
 * Decode a sky Aircraft
 */
void SkyAircraftNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);
	atc::Sky& sky = task->engine()->sky();

	std::string planeID(_doc->getAttributeValue(node->parent, "idx"));

	if (node->type == NODE_TYPE_ALTITUDE)
	{
		sky.set_aircraft_initialaltitude(
			planeID.c_str(),
			_doc->getDecimalContent(node));
	}
    else if (node->type == NODE_TYPE_ALTITUDE_CHANGE_ENABLE_TIME)
	{
		sky.SetAircraftAltitudeChangeEnableTime(
			planeID.c_str(),
			_doc->getIntegerContent(node));
            
	}
	else if (node->type == NODE_TYPE_VELOCITY)
	{
		sky.set_aircraft_initialvelocity(
			planeID.c_str(),
			_doc->getDecimalContent(node));
	}
	else if (node->type == NODE_TYPE_FLIGHT_PATH)
	{
		_doc->process_children(
			node, 
			SkyAircraftFlightpathNodeHandler(_doc, _view));
	}
	else if (node->type == NODE_TYPE_CLOSED_PATH)
	{
		sky.set_aircraft_closedpath(planeID.c_str(), true);
	}
	else if (node->type == NODE_TYPE_START_TIME)
	{
		sky.SetAircraftStartTime(
			planeID.c_str(), 
			_doc->getIntegerContent(node));
	}
	else if (node->type == NODE_TYPE_AS_FLASH_PARAMS)
	{
		_doc->process_children(
			node,
			AircraftAcceptedStateFlashNodeHandler(_doc, _view));
	}
	else if (node->type == COORDINATION_REQUEST)
	{
		std::string key = "";

		if (_doc->attributeExists(node, "key"))
		{
			key = _doc->getAttributeValue(node, "key");
			
			if (key != "")
			{
				sky.SetCoordinationRequestKeyForAircraft(planeID.c_str(), toupper(key[0]));
			}
		}

		_doc->process_children(
			node,
			ShowCoordinationRequestNodeHandler(_doc, _view));
	}
    else if (node->type == NODE_TYPE_HANDOFF_KEY)
	{
        sky.SetAircraftHandoffKey(
			planeID.c_str(),
			_doc->getStringContent(node));
	}
	else if (node->type == NODE_TYPE_CONFLICT_DETAILS)
	{
		_doc->process_children(node, AircraftConflictDetailsNodeHandler(_doc, _view));
	}
	else
	{
		throw nodehandler_error("illegal sky aircraft node entry <" + node->type + ">");
	}
}

void ReminderNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);

	if (node->type == NODE_TYPE_REMINDER_BOX_FLASH_COLOR)
	{
		if (m_attachedToAircraft != "")
		{
			task->engine()->sky().SetReminderBoxFlashColorForAircraft(m_attachedToAircraft.c_str(), 
																		_doc->getStringContent(node));
		}
		else
		{
			task->engine()->SetReminderBoxFlashColor(_doc->getStringContent(node));
		}
	}
	else if (node->type == NODE_TYPE_REMINDER_BOX_FLASH_TIME)
	{
		unsigned int flashStartTime = 
			atoi(_doc->getAttributeValue(node, "startTime").c_str());
		unsigned int flashEndTime = 
			atoi(_doc->getAttributeValue(node, "endTime").c_str());

		if (m_attachedToAircraft != "")
		{
			task->engine()->sky().SetReminderBoxFlashTimeForAircraft(	m_attachedToAircraft.c_str(),
																		flashStartTime,
																		flashEndTime);
		}
		else
		{
			task->engine()->SetReminderBoxFlashTime(flashStartTime, 
													flashEndTime);
		}
	}
    else if (node->type == NODE_TYPE_REMINDER_BOX_DISMISS_BUTTON)
    {
        if (_doc->attributeExists(node, "enable_time"))
        {
            if (m_attachedToAircraft != "")
		    {
                task->engine()->sky().SetReminderBoxDismissButtonEnableTime(
                    m_attachedToAircraft.c_str(),
                    atoi(_doc->getAttributeValue(node, "enable_time").c_str()));
            }
        }

        if (_doc->attributeExists(node, "label"))
        {
            if (m_attachedToAircraft != "")
		    {
                task->engine()->sky().SetReminderBoxDismissButtonLabel(
                    m_attachedToAircraft.c_str(),
                    _doc->getAttributeValue(node, "label"));
            }
        }
    
        if (_doc->attributeExists(node, "fontSize"))
        {
            if (m_attachedToAircraft != "")
		    {
                task->engine()->sky().SetReminderBoxDismissButtonFontSize(
                    m_attachedToAircraft.c_str(),
                    atoi(_doc->getAttributeValue(node, "fontSize").c_str()));
            }
        }

        if (_doc->attributeExists(node, "width"))
        {
            if (m_attachedToAircraft != "")
		    {
                task->engine()->sky().SetReminderBoxDismissButtonWidth(
                    m_attachedToAircraft.c_str(),
                    atoi(_doc->getAttributeValue(node, "width").c_str()));
            }
        }

        if (_doc->attributeExists(node, "height"))
        {
            if (m_attachedToAircraft != "")
		    {
                task->engine()->sky().SetReminderBoxDismissButtonHeight(
                    m_attachedToAircraft.c_str(),
                    atoi(_doc->getAttributeValue(node, "height").c_str()));
            }
        }

        if (_doc->attributeExists(node, "posX"))
        {
            if (m_attachedToAircraft != "")
		    {
                task->engine()->sky().SetReminderBoxDismissButtonPosX(
                    m_attachedToAircraft.c_str(),
                    atoi(_doc->getAttributeValue(node, "posX").c_str()));
            }
        }

        if (_doc->attributeExists(node, "posY"))
        {
            if (m_attachedToAircraft != "")
		    {
                task->engine()->sky().SetReminderBoxDismissButtonPosY(
                    m_attachedToAircraft.c_str(),
                    atoi(_doc->getAttributeValue(node, "posY").c_str()));
            }
        }
        
    }
	else if (node->type == NODE_TYPE_REMINDER_BOX_DISPLAY_TIME)
	{
		int displayStartTime =
			atoi(_doc->getAttributeValue(node, "startTime").c_str());
		int displayEndTime =
			atoi(_doc->getAttributeValue(node, "endTime").c_str());

		if (m_attachedToAircraft != "")
		{
			task->engine()->sky().SetReminderBoxDisplayTimeForAircraft(	m_attachedToAircraft.c_str(),
																		displayStartTime,
																		displayEndTime);
		}
		else
		{
			task->engine()->SetReminderBoxDisplayTime(	displayStartTime, 
														displayEndTime);
		}
	}
	else
	{
		throw nodehandler_error("illegal reminder node entry <" + node->type + ">");
	}
}

void 
SoundFileNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);

	if (node->type == NODE_TYPE_SOUND_FILE_PLAY_TIME)
	{
		int startTime = atoi(_doc->getAttributeValue(node, "startTime").c_str());
		int endTime = atoi(_doc->getAttributeValue(node, "endTime").c_str());

		task->engine()->SetSoundFilePlayTime(m_fileName, startTime, endTime);
	}
	else
	{
		throw nodehandler_error("illegal reminder node entry <" + node->type + ">");
	}
}

void DivisionLineNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);

	if (node->type == NODE_TYPE_DIVISION_LINE_DISPLAY_TIME)
	{
		int startTime = 
			atoi(_doc->getAttributeValue(node, "startTime").c_str());
		int endTime = 
			atoi(_doc->getAttributeValue(node, "endTime").c_str());

		task->engine()->SetDivisionLineDisplayTime(m_id, startTime, endTime);
	}
	else
	{
		throw nodehandler_error("illegal reminder node entry <" + node->type + ">");
	}
}

void 
QuestionBoxNodeHandler::operator()(const xml::DocumentNode* node)
{
	InstructionTask* task = dynamic_cast<InstructionTask*>(_view);

	if (node->type == NODE_TYPE_QUESTION_BOX_ANSWER)
	{
		task->SetQuestionBoxAnswer(node->content);
	}
	else if (node->type == NODE_TYPE_QUESTION_BOX_INCORRECT_RESPONSE)
	{
		task->SetQuestionBoxIncorrectResponse(node->content);
	}
	else
	{
		throw nodehandler_error("illegal question box node entry <" + node->type + ">");
	}
}

//------------------------------------------------------------------------------
// Sky Aircraft FlightPath Node Handler
//

/*!
 * Decode a sky aircraft's flight path
 */
void SkyAircraftFlightpathNodeHandler::operator()(
	const xml::DocumentNode* node
){
	TrialTask* task = dynamic_cast<TrialTask*>(_view);

	if("point" == node->type){
		task->engine()->sky().add_aircraft_pathdescriptor(
				_doc->getAttributeValue(node->parent->parent, "idx").c_str()
			  , atof( _doc->getAttributeValue( node, "x" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "y" ).c_str() )
			  , _doc->exists(node, "altitude")
					? _doc->getDecimalContent(node, "altitude")	: atc::NAN
			  , _doc->exists(node, "ascent")
					? _doc->getDecimalContent(node, "ascent") : atc::NAN
			  , _doc->exists(node, "velocity")
					? _doc->getDecimalContent(node, "velocity") : atc::NAN
			  , _doc->exists(node, "acceleration")
					? _doc->getDecimalContent(node, "acceleration") : atc::NAN
		);

	}else{
		throw nodehandler_error(
			"illegal sky aircraft flightpath entry <" + node->type + ">"
		);
	}
}


void AircraftAcceptedStateFlashNodeHandler::operator()(
	const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);
	std::string planeID = _doc->getAttributeValue(node->parent->parent, "idx");

	if(node->type == FLASH_PARAM_FLASH_ITEM)
	{
		/*
		 * Disabled due to new aircraft acceptance procedure
		task->engine()->sky().SetAircraftAcceptStateFlashItem(
			planeID.c_str(),
			_doc->getStringContent(node));
		 */
	}
	else if (node->type == FLASH_PARAM_FLASH_COLOUR)
	{
	}
	else if (node->type == "flashColour")
	{
		task->engine()->sky().SetAircraftAcceptStateFlashColour(
			planeID.c_str(),
			_doc->getStringContent(node));
	}
	else
	{
		throw nodehandler_error(
			"illegal accepted state flash param entry <" + node->type + ">"
		);
	}
}

void
ShowCoordinationRequestNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);
	std::string planeID = _doc->getAttributeValue(node->parent->parent, "idx");

	if (node->type == COORDINATION_REQUEST_SHOW)
	{
		if (_doc->getStringContent(node) == "true")
		{
			task->engine()->sky().SetCoordinationRequestStateForAircraft(planeID.c_str());
		}
	}
	else
	{
		throw nodehandler_error("illegal coordination request details entry <" + node->type + ">");
	}
}

void AircraftConflictDetailsNodeHandler::operator()(
	const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);
	std::string planeID = _doc->getAttributeValue(node->parent->parent, "idx");

	if(node->type == CONFLICT_DETAILS_TRIAL)
	{
		task->engine()->sky().SetAircraftConflictDetailsTrial(
			planeID.c_str(),
			_doc->getStringContent(node));
	}
	else if(node->type == CONFLICT_DETAILS_ACCEPTANCE_TIME)
	{
		task->engine()->sky().SetAircraftConflictDetailsAcceptanceTime(
			planeID.c_str(),
			_doc->getIntegerContent(node));
	}
	else if (node->type == CONFLICT_DETAILS_CONFLICT_TIME)
	{
		task->engine()->sky().SetAircraftConflictDetailsConflictTime(
			planeID.c_str(),
			_doc->getIntegerContent(node));
	}
	else if (node->type == CONFLICT_DETAILS_CONFLICT_AIRCRAFT)
	{
		task->engine()->sky().SetAircraftConflictDetailsConflictAircraft(
			planeID.c_str(),
			_doc->getStringContent(node));
	}
	else
	{
		throw nodehandler_error(
			"illegal conflict details entry <" + node->type + ">"
		);
	}
}
//------------------------------------------------------------------------------
// Sky "area" node handler.
//

/*!
 * Constructor
 */
AreaNodeHandler::AreaNodeHandler(
	const xml::Document* doc, 
	xml::DocumentView* view,
	const xml::DocumentNode* node
)
	:xml::NodeHandler(doc, view)
	,_areaID( _doc->getAttributeValue( node, "idx" ) )
{
	TrialTask* task = dynamic_cast< TrialTask* >( _view );
	atc::Sky& sky = task->engine()->sky();
	sky.create_weather( _areaID.c_str() );
}


/*!
 * Destructor
 */
AreaNodeHandler::~AreaNodeHandler() {}


/*!
 *
 */
void AreaNodeHandler::operator()( const xml::DocumentNode* node ) {
	TrialTask* task = dynamic_cast< TrialTask* > ( _view );
	atc::Sky& sky = task->engine()->sky();

	if ( "arc" == node->type ) {
		sky.add_weather_arc(
				_areaID.c_str()
			  , atof( _doc->getAttributeValue( node, "x" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "y" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "r" ).c_str() )
		);
	}
	else if ( "vertex" == node->type ) {
		sky.add_weather_vertex(
				_areaID.c_str()
			  , atof( _doc->getAttributeValue( node, "x" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "y" ).c_str() )
		);
	}
	else if ( "ellipse" == node->type ) {
		sky.add_weather_ellipse(
				_areaID.c_str()
			  , atof( _doc->getAttributeValue( node, "x" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "y" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "w" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "h" ).c_str() )
			  , atof( _doc->getAttributeValue( node, "a" ).c_str() )
		);
	}
	else if ( "remove" == node->type ) {
		// @todo:
		throw nodehandler_error( "'remove' area *NOT* implemented!" );
	}
	else{
		throw nodehandler_error(
			"illegal map sector node entry <" + node->type + ">"
		);
	}
}

//==============================================================================
//
// User Interface
//
//------------------------------------------------------------------------------
// UI Node Handler
//

/*!
 */
void UINodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);

    
    if ("clock" == node->type)
    {
        bool show_clock = (_doc->getStringContent(node) == std::string("true"));
        task->ShowClock(show_clock);
    }
	else if("widget" == node->type){
		_doc->process_children(
			node, 
			UIWidgetNodeHandler(
				_doc, _view,
				_doc->getAttributeValue(node, "class"),
				_doc->getAttributeValue(node, "name"),
				task
			)
		);

	}else if("connection" == node->type){
		_doc->process_children(node, UIConnectionNodeHandler(_doc, _view));

	}
	else if (node->type == "timeEvent1")
	{
		task->SetTimeEvent(atoi(node->content.c_str()));
	}
	else if (node->type == "timeEvent2")
	{
		task->SetTimeEvent(atoi(node->content.c_str()));
	}
	else if (node->type == "timeEvent3")
	{
		task->SetTimeEvent(atoi(node->content.c_str()));
	}
	else if (node->type == "timeEvent4")
	{
		task->SetTimeEvent(atoi(node->content.c_str()));
	}
	else if (node->type == "QObject")
	{
		std::string type = _doc->getAttributeValue(node, "class");
		_doc->process_children(node, UIQObjectNodeHandler(_doc, _view, type));
	}
	else
	{
		throw nodehandler_error("illegal user interface entry <" + node->type + ">");
	}
}

//------------------------------------------------------------------------------
// UI Widget Node Handler
//

/*!
 * Constructor
 */
UIWidgetNodeHandler::UIWidgetNodeHandler(
	const xml::Document* doc, 
	xml::DocumentView* view,
	const std::string& type,
	const std::string& name,
	QWidget* parent
)
	:NodeHandler(doc, view)
	,_widget(0)
{
	create_widget(type, name, parent);
	add2layout(parent);
}

/*!
 * Destructor
 */
UIWidgetNodeHandler::~UIWidgetNodeHandler()
{}

/*!
 * Decode a User Interface Widget
 */
void UIWidgetNodeHandler::operator()(const xml::DocumentNode* node)
{
	if("property" == node->type){
		set_property(node);

	}else if("layout" == node->type){
		_doc->process_children(
			node, 
			UIWidgetLayoutNodeHandler(
				_doc, _view, 
				node,
				_widget
			)
		);

	}else{
		throw nodehandler_error(
			"illegal user interface widget entry <" + node->type + ">"
		);
	}
}

//
// [private] implementation
//

/*!
 * Factory method for creation of widgets for display during task. The 
 * destruction of new widgets will be handled by underlying Qt functionality 
 * via the parent QWidget.
 *
 * @param type string type identifier
 * @param name string used for lookup of new widget
 * @param parent QWidget object that will own new widget
 */
void UIWidgetNodeHandler::create_widget(
	const std::string& type,
	const std::string& name,
	QWidget* parent
){
	if("dialog" == type){
		_widget = new Dialog(parent, name.c_str());

	}else if("pushbutton" == type){
		_widget = new PushButton(parent, name.c_str());

	}else if("textline" == type){
		_widget = new QLineEdit(parent, name.c_str());

	}else if("frame" == type){
		_widget = new QFrame(parent, name.c_str());

	}else if("label" == type){
		_widget = new QLabel(parent, name.c_str());

	}else{
		throw nodehandler_error(
			std::string("illegal UI widget type<") + type + ">"
		);
	}
}

/*!
 * Add a NON top level widget to it's parent's layout
 */
void UIWidgetNodeHandler::add2layout(QWidget* parent)
{
	if(!_widget->isTopLevel()){
		parent->layout()->add(_widget);
	}
}

/*!
 *
 */
void UIWidgetNodeHandler::set_property(const xml::DocumentNode* node)
{
	std::string key(_doc->getAttributeValue(node, "name"));

	if("caption" == key || "text" == key){
		_widget->setProperty(
			key.c_str(), 
			_doc->getStringContent(node, "string").c_str()
		);

	}else if("font" == key){
		_widget->setProperty(
			key.c_str(),
			QFont(
				_doc->getStringContent(node, "font family"),
				_doc->getIntegerContent(node, "font pointsize")
			)
		);

//		_doc->descendantExists(node, "font weight")
//			? _doc->getIntegerContent(node, "font weight")
//			: QFont::Normal

	}else if("geometry" == key){
		_widget->setProperty(
			key.c_str(),
			QRect(
				_doc->getIntegerContent(node, "rect x"),
				_doc->getIntegerContent(node, "rect y"),
				_doc->getIntegerContent(node, "rect w"),
				_doc->getIntegerContent(node, "rect h")
			)
		);

	}else{
		throw nodehandler_error(
			"illegal property name <" + key + ">"
		);
	}
}

//------------------------------------------------------------------------------
// UI UI Widget Layout Node Handler
//

/*!
 * Constructor
 */
UIWidgetLayoutNodeHandler::UIWidgetLayoutNodeHandler(
	const xml::Document* doc, 
	xml::DocumentView* view,
	const xml::DocumentNode* node,
	QWidget* parent
)
	:NodeHandler(doc, view)
	,_layout(0)
	,_widget(parent)
{
	create_layout(
		_doc->getAttributeValue(node, "type"),
		atoi(_doc->getAttributeValue(node, "margin").c_str()),
		parent
	);
}

/*!
 * Destructor
 */
UIWidgetLayoutNodeHandler::~UIWidgetLayoutNodeHandler()
{}

/*!
 * Decode a User Interface Widget Layout
 */
void UIWidgetLayoutNodeHandler::operator()(const xml::DocumentNode* node)
{
	if("widget" == node->type){
		_doc->process_children(
			node,
			UIWidgetNodeHandler(
				_doc, _view,
				_doc->getAttributeValue(node, "class"),
				_doc->getAttributeValue(node, "name"),
				_widget
			)
		);

	}else if("stretch" == node->type){
		_layout->addStretch(_doc->getIntegerContent(node));

	}else if("space" == node->type){
		_layout->addSpacing(_doc->getIntegerContent(node));

	}else{
		throw nodehandler_error(
			"illegal user interface widget layout entry <" + node->type + ">"
		);
	}
}

//
// [private] implementation
//

/*!
 */
void UIWidgetLayoutNodeHandler::create_layout(
	const std::string& type,
	int margin,
	QWidget* parent
){
	if("hbox" == type){
		_layout = new QHBoxLayout(parent, margin);
	}else if("vbox" == type){
		_layout = new QVBoxLayout(parent, margin);
	}else{
		throw nodehandler_error(
			"illegal layout manager type <" + type + ">"
		);
	}
}

//------------------------------------------------------------------------------
// UI Connection Node Handler
//

/*!
 * Decode User Interface Connections
 */
void UIConnectionNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);

	if("signal" == node->type){
		_tx = 
			_doc->attributeExists(node, "tx")
			? widget_ptr(_doc->getAttributeValue(node, "tx"))
			: task;
		_sig = node->content;

	}else if("slot" == node->type){
		_rx = 
			_doc->attributeExists(node, "rx")
			? widget_ptr(_doc->getAttributeValue(node, "rx"))
			: task;
		_slot = node->content;

		connect();

	}else{
		throw nodehandler_error(
			"illegal user interface connection entry <" + node->type + ">"
		);
	}
}

/*!
 * Look up a widget using it's name and return a pointer to it.
 *
 * @param name widget's name
 * @return a pointer to the named widget (or NULL)
 */
QWidget* UIConnectionNodeHandler::widget_ptr(
	const std::string& name
)const{

	QWidgetList  *list = QApplication::allWidgets();
    QWidgetListIt it( *list );         // iterate over the widgets

    QWidget * w = 0;
    while ((w = it.current()) != 0){
		++it;
		if(name != w->name()) continue;

		QObject* parent = w;
		while(!parent->inherits("pact::TrialTask")){
			parent = parent->parent();
		}
		if(
			dynamic_cast<Task*>(parent) == 
			dynamic_cast<Task*>(_view) 
		) break;
	}
    delete list;                      // delete the list, not the widgets

	return w;
}

/*!
 */
void UIConnectionNodeHandler::connect()const
{
	assert(_tx && _sig.size());
	assert(_rx && _slot.size());

	QObject::connect(
		_tx, QString("2%1()").arg(_sig),
		_rx, QString("1%1()").arg(_slot)
	);

}

//------------------------------------------------------------------------------
// UI QObject Node Handler
//
UIQObjectNodeHandler::UIQObjectNodeHandler(
	const xml::Document* doc, 
	xml::DocumentView* view,
	std::string type)
		:	NodeHandler(doc, view), 
			_type(type),
			_object(0)
{
}

UIQObjectNodeHandler::~UIQObjectNodeHandler()
{
}

void UIQObjectNodeHandler::operator()(const xml::DocumentNode* node)
{
	TrialTask* task = dynamic_cast<TrialTask*>(_view);

	if (_type == "Clock")
	{
		if (node->type == "property")
		{
			std::string property(_doc->getAttributeValue(node, "name"));

			if (property == "interval")
			{
				task->SetUIClock(_doc->getIntegerContent(node, "integer") * 1000);
			}
			else
			{
				throw nodehandler_error("illegal property name <" + property + ">");
			}
		}
		else
		{
			throw nodehandler_error("illegal property name <" + node->type + ">");
		}
	}
	else if (_type == "QTimer")
	{
		QTimer ui_timer = new QTimer();

		if (node->type == "property")
		{
			std::string property(_doc->getAttributeValue(node, "name"));

			if (property == "timeout")
			{
				//TODO: Implement this
			}
			else
			{
				throw nodehandler_error("illegal property name <" + property + ">");
			}
		}
		else
		{
			throw nodehandler_error("illegal property name <" + node->type + ">");
		}
	}
	else
	{
		throw nodehandler_error(std::string("illegal UI QObject type <") + _type + ">");
	}
}



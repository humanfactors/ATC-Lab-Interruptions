/**
      @project  : ATC-Lab [engine]

      @file     : $RCSfile: canvasview.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.21.2.6 $ )
      @date     : $Date: 2015/02/23 13:34:42 $
      @state    : $State: Exp $

      $Log: canvasview.cpp,v $
      Revision 1.21.2.6  2015/02/23 13:34:42  Aaron Yeung
      - Added logging of interruption tasks to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element				Description							Values
                *******************************************************************************************************************
      	  interruption_type 			Type of interruption						"Interruption" or "NBack"
          	  start_time				Programmed start time of interruption
      	  end_time				Programmed end time of interruption
          	  show_full_screen			Whether the task was shown in full screen			0 = no, 1 = yes
          	  external_program			Name of external program that was run
          	  auto_accept				Whether aircraft were auto accepted during interruption		0 = no, 1 = yes
          	  auto_handoff				Whether aircraft were auto handed off during interruption	0 = no, 1 = yes

      	 - Added logging of interruption tasks to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                Field			Description							Values
                *******************************************************************************************************************
      	  Trial			Name of the trial that the interruption occurred in
      	  Interruption Type	Type of interruption						"Interruption" or "NBack"
          	  Start Time		Programmed start time of interruption
      	  End Time		Programmed end time of interruption
          	  Show Full Screen	Whether the task was shown in full screen			0 = no, 1 = yes
          	  External Program	Name of external program that was run
          	  Auto Accept		Whether aircraft were auto accepted during interruption		0 = no, 1 = yes
          	  Auto Handoff		Whether aircraft were auto handed off during interruption	0 = no, 1 = yes

      	 - Added additional field in the aircraft flash details section of the output .csv file called "Automatic" to indicate if an aircraft is
      	   automatically accepted or handed off. Values are 0 (not automatic) or 1 (automatic acceptance/handoff)

      	 - Added logging of reminder box dismiss button times to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  reminder_box		Tag to indicate that the reminder box dismiss button was pressed	Aircraft callsign that had the dismiss button pressed
      	  dismiss_button	Tag to indicate that the reminder box dismiss button was pressed	"pressed"
      	  clicked_time		Millisecond time that the dismiss button was pressed

      	- Added logging of reminder box dismiss button times to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                Field			Description								Values
                *******************************************************************************************************************
      	  Trial  		Name of the trial that the dismiss button was pressed
      	  Callsign		Aircraft callsign that had the dismiss button pressed
      	  Dismiss Time		Time in seconds into the trial that the dismiss button was pressed
      	  Dismiss Time HP	Time in milliseconds into the trial that the dismiss button was pressed

      	- Added calculation of Nback task results based on participant's inputs/non-inputs in the N back task. The are described as follows:
      	  ***********************************************************************************************************************
      	  Result		Description
      	  ***********************************************************************************************************************
      	  Hit			Participant correctly responds to a stimuli that was the same as the stimuli N times before
      	  Miss			Participant fails to respond to a stimuli that was the same as the stimuli N times before
      	  Correct rejection	Participant correct ignores the stimuli which is not the same as the stimulis N times before
      	  False alarm		Paricipant incorrectly responds to a stimuli that was not the same as the stimuli N times before

      	 - Added logging of N back task results to the output .xml file. The output XML logging are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  NBackTask_response	Stimuli order of the stimuli that was presented
      	  stimuli		The actual stimuli that was displayed to the participant
      	  response		The result of the paritcipant's response				hit, miss, false_alarm, correct_rejection

      	 - Added logging of N back task results to the output .csv file. The output fields are described below:
      	  *******************************************************************************************************************
                XML Element		Description								Values
                *******************************************************************************************************************
      	  Trial	  		Name of the trial that the N back response was generated in
      	  Stimuli Number	Stimuli order of the stimuli that was presented
      	  Stimuli		The actual stimuli that was displayed to the participant
      	  Response		The result of the paritcipant's response				hit, miss, false_alarm, correct_rejection

      	 - Added a count down timer for the blank interruption task. It can be enabled/disabled with the XML below:
      	  *******************************************************************************************************************
                XML Element		Attribute	Description							Values
                *******************************************************************************************************************
      	  atc:interruption	 atc:show_timer	Enable/disable countdown timer in the blank interruption task	'true' or 'false'

      	  Example:
      	  <atc:interruption atc:start='3' atc:end='21' atc:auto_handoff='true' atc:auto_accept='false' atc:show_blank_screen='true' atc:show_timer='true' atc:external_program='"calc.exe"'/>

      	  - Added ability to enable/disable error pop up dialogs when a paricipant misses an aircraft acceptance or handoff.
      	    This can be achieved with the XML below:
      	  ***********************************************************************************************************************************************
                XML Element	Attribute			Description									Values
                ***********************************************************************************************************************************************
      	  atc:trial	atc:notify_missed_acceptance	Enable/disable pop up error dialog when a paricipant misses an acceptance	'true' or 'false'
      			atc:notify_missed_handoff	Enable/disable pop up error dialog when a paricipant misses a handoff		'true' or 'false'

      	  Example:
      	  <atc:trial atc:idx='test2' atc:param='default' atc:map="map1" atc:sky="script2" atc:ui="ui001" atc:notify_missed_acceptance='false' atc:notify_missed_handoff='false'>

      Revision 1.21.2.5  2014/12/06 08:13:22  Aaron Yeung
      ATCLab v2.4.5.7
      - Fixed two issues:
      		1) Canvasview grabs all the keypress events which causes other widgets like the altitude selection
      		   tool to not be able to be escaped
      		2) When aircraft are set with a customized handoff key, the default handoff key 'h' is not usable
      		   for handoff

      Revision 1.21.2.4  2014/12/02 13:48:59  Aaron Yeung
      Implemented scriptable handoff key for individual aircraft - <atc:handoffkey>
      Default handoff key is still the 'h' key

      Revision 1.21.2.3  2014/12/02 12:06:36  Aaron Yeung
      Added scriptable altitude change time restriction to aircraft <atc:altitudeChangeEnableTime>

      Revision 1.21.2.2  2014/11/26 14:21:17  Aaron Yeung
      Removed superflous test code

      Revision 1.21.2.1  2012/03/14 20:17:55  Aaron Yeung
      Added millisecond timing to the Excel log file

      Revision 1.21  2010/04/18 02:17:51  Aaron Yeung
      Fixed problem where the task was taking focus over canvasview which prevented keypresses to aircrafts for acceptance and handoff etc. This was fixed by calling setFocus() on canvasview after task::ShowEvent (which called its own setFocus())

      Revision 1.20  2010/04/08 13:19:31  Aaron Yeung
      Initial attempt at adding reminder box to individual aircraft

      Revision 1.19  2009/04/29 13:55:35  Aaron Yeung
      Modified reminder box to stop flashing when it has not end display time or dissappear when it does have an end display time whenever a coordination request aircraft is accepted

      Revision 1.18  2009/04/27 13:38:18  Aaron Yeung
      Disabled selection of aircraft when they are green

      Revision 1.17  2009/04/23 12:54:42  Aaron Yeung
      Modified code to not allow a user to select an aircraft when it is black (aircraft state is CS_NONE or CS_NOMORE)

      Revision 1.16  2008/10/21 13:57:51  Aaron Yeung
      - Added functionality for experimenter to re-ask a question if the participant gives the wrong answer.
      - Added new xml element
      <questionBox>
              <atc:answer></atc:answer>
              <atc:incorrectResponse></atc:incorrectRespons>
      </questionBox>

      Revision 1.15  2008/07/29 11:58:02  Aaron Yeung
      Changed reminderbox to be able to display text on multiple lines
      Changed reminderbox to stop flashing when a coordination aircraft is accepted.
      Changed acceptance and handoff procedures to not penalise when the handoff key is pressed during acceptance task and when the acceptance key is pressed during the handoff task

      Revision 1.14  2008/07/12 19:53:54  Aaron Yeung
      Added prehandoff mode to aircraft such that they flash blue when exiting the sector

      Revision 1.13  2008/06/30 11:13:14  Aaron Yeung
      Added scoring for aircraft handoff task
      Modified aircraft acceptance task so that a user selects an aircraft then presses the 'a' key

      Revision 1.12  2008/05/13 17:00:32  Aaron Yeung
      Added functionality to:

      - Allow an aircraft's callsign to flash for an acceptance task
      - Add a minimum time to enable an ok button on an instruction form
      - Create reminder box

      Revision 1.11  2007/08/08 13:41:44  Aaron Yeung
      Disabled flight level selection for aircraft that are not in CS_PROPOSED, CS_ACCEPTED_TASK_INCOMPLETE, CS_ACCEPTED_TASK_COMPLETE states

      Revision 1.10  2007/07/31 14:49:05  Aaron Yeung
      Added Excel logging functionality

      Revision 1.9  2007/07/26 11:31:11  Aaron Yeung
      Disabled pressing of keys and clicking on aircraft when the engine is paused

      Revision 1.8  2007/07/26 10:22:05  Aaron Yeung
      Disallowed acceptance of aircraft that are not in the proposed state

      Revision 1.7  2007/07/25 14:44:33  Aaron Yeung
      Implemented scoring system

      Revision 1.6  2007/07/22 11:55:43  Aaron Yeung
      Added TrialTimeOut signal/slots to canvas to close any open dialog boxes

      Revision 1.5  2007/07/22 09:24:09  Aaron Yeung
      Disabled the following user tools:

      - Speed change
      - Level Requirement by distance
      - Vector
      - The bearing and range tool
      - Display route tool

      Disabled questionbox that normally appears after a FL change

      Revision 1.4  2007/07/16 10:54:53  Aaron Yeung
      Modified acceptance task so that a user can continue to try to press the correct key for an acceptance task until the time limit is exceeded. Previously, if the user pressed the wrong key, they would not be able to re-enter the correct key again

      Revision 1.3  2007/07/09 09:35:10  Aaron Yeung
      Added verification of key pressed for the accept state task

      Revision 1.2  2007/07/09 08:36:10  Aaron Yeung
      Added aircraft acceptance task where the user has to accept a aircraft in the CS_PROPOSED state, right clicking on either the speed information or flight level and then pressing a specfic key

      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.31  2007/02/28 00:48:12  seth
      implemented tool identifier

      Revision 1.30  2007/02/26 08:03:45  seth
      automatic handoff

      Revision 1.29  2007/02/07 02:51:56  seth
      1) fixed flashing aircraft to entering not exiting sector; 2) inhibited workload pop-up question; 3) implemented minute markers along short route probe

      Revision 1.28  2006/11/07 03:05:44  seth
      added additional logging

      Revision 1.27  2006/11/02 06:48:53  seth
      improve arc resolution

      Revision 1.26  2006/11/01 04:53:36  seth
      created installer

      Revision 1.25  2006/10/26 05:59:52  seth
      working implementation of experiment time logging

      Revision 1.24  2006/10/18 03:58:33  seth
      snap functionality for selection & fixed bug causing access violation once aircraft had reached destination

      Revision 1.23  2006/10/17 12:23:30  seth
      Altered selection priority (z) of ac-locator over actual sprite. Fixed requirement dialog to include distance

      Revision 1.22  2006/10/17 07:06:20  seth
      fixed popup question layouts

      Revision 1.21  2006/10/09 04:59:46  seth
      bug fixes

      Revision 1.20  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.19  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.18  2006/09/24 00:28:03  seth
      fixed vectoring and rerouting

      Revision 1.17  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.16  2006/09/20 06:44:01  seth
      vector selection box + some bug fix

      Revision 1.15  2006/09/18 06:41:19  seth
      updating state calcutlations

      Revision 1.14  2006/09/17 12:45:35  seth
      implementing solution mechanisms

      Revision 1.13  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.12  2006/09/14 07:28:06  seth
      inital implementation of sector controller state

      Revision 1.11  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.10  2006/09/13 05:38:49  seth
      full working implemtation of Range/Bearing tool

      Revision 1.9  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.8  2006/09/12 05:43:43  seth
      aircraft route tool working implementation

      Revision 1.7  2006/09/06 06:22:23  seth
      transformation problems

      Revision 1.6  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.5  2006/08/29 08:44:56  seth
      reimplementing mvc pattern

      Revision 1.4  2006/08/28 06:21:48  seth
      refactoring mvc

      Revision 1.3  2006/06/01 03:41:23  seth
      implemented CFL and speed selectors

      Revision 1.2  2006/05/25 04:15:25  seth
      implemented weather, few minor bug fixes, initial cfl/vel selection


      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology
  
**/

#include "canvasview.h"

#include "acsprite.h"
#include "actools.h"
#include "aircraft_agent.h"
#include "canvas.h"
#include "canvas_tools.h"
#include "listbox.h"
#include "popup_question.h"
#include "waypointsprite.h"

#include "logging/logger.h"
#include "logging/ExcelLogger.h"
#include "timer.h"

#include <iostream.h>
#include <qapplication.h>
#include <qdatetime.h>
#include <qlabel.h>
#include <qkeysequence.h>
#include <qtimer.h>
#include <qwmatrix.h>
#include <qvbox.h>
#include <qtextedit.h>
#include <qlayout.h>

#include <math.h>

using namespace atc;
using std::make_pair;
using std::string;


//------------------------------------------------------------------------------

/*!
 */
CanvasView::CanvasView( QCanvas* canvas, QWidget* parent, const char* name )
	  : QCanvasView( canvas, parent, name )
	  , _cursor()
	  , _key( 0 )
	  , _pointer( false )
	  , _scale( false )
	  , _brl( false )
	  , _vector( false )
	  , _shown_selector( 0 )
	  , _vel_select( new VelocitySelector( this ) )
	  , _cfl_select( new CFLSelector( this ) )
	  , _ang_select( new HeadingSelector( this ) )
	  , _requirement_dialog( new RequirementSelector( this ) )
	  , _workload_question( new WorkloadQuestion( this ) )
	  , _efficiency_question( new EfficiencyQuestion( this ) )
	  , _engine_state(ENGINE_STATE_STARTED)
	  , m_timeOfAcceptanceKeyPress(0)
{
	setVScrollBarMode( QScrollView::AlwaysOff );
	setHScrollBarMode( QScrollView::AlwaysOff );

	setFocusPolicy( QWidget::StrongFocus );
	viewport()->setMouseTracking( true );

	connect(this,   SIGNAL ( event_recorded( KeyValues & ))
		  , canvas, SIGNAL ( event_recorded( KeyValues & )));

    connect(_cfl_select, SIGNAL(Selected()), this, SLOT(RegainKeyboardFocus()));
    connect(_cfl_select, SIGNAL(Escaped()), this, SLOT(RegainKeyboardFocus()));
}

/*!
 */
CanvasView::~CanvasView() 
{
	if (_vel_select)
	{
		delete _vel_select;
	}

	if (_cfl_select)
	{
		delete _cfl_select;
	}

	if (_ang_select)
	{
		delete _ang_select;
	}

	if (_requirement_dialog)
	{
		delete _requirement_dialog;
	}

	if (_workload_question)
	{
		delete _workload_question;
	}
	
	if (_efficiency_question)
	{
		delete _efficiency_question;
	}
}


//------------------------------------------------------------------------------
// [public] slots:
//

/*!
 *
 */
Canvas * CanvasView::atc_canvas() const {
	return dynamic_cast< Canvas * >( canvas() );
}

//------------------------------------------------------------------------------
// 	[protected] virtual event handlers
//

/*!
 * Process mouse double click events.
 *
 * @param e QMouseEvent.
 */
void CanvasView::contentsMouseDoubleClickEvent( QMouseEvent* e ) {
    try
	{
		if (_engine_state == ENGINE_STATE_STARTED)
		{
			_cursor = inverseWorldMatrix().map( e->pos() );

			if ( QCanvasItem *item = find_item() )
					action_item( item );
			
			canvas()->update();
			
			//
			// Log event
			//

			Point up = atc_canvas()->to_user_point( _cursor.x(), _cursor.y() );

			KeyValues kvs;
			kvs.push_back( make_pair( string( "event" ), to_string( "mouse_double_click" ) ) );
			kvs.push_back( make_pair( string( "x"     ), to_string( up.first             ) ) );
			kvs.push_back( make_pair( string( "y"     ), to_string( up.second            ) ) );
			record_event( kvs );
		}
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling CanvasView::contentsMouseDoubleClickEvent");
	}
}

/*!
 * Procress mouse click events.
 *
 * @param e QMouseEvent.
 */
void CanvasView::contentsMousePressEvent( QMouseEvent* e ) {
	try
	{
		if (_engine_state == ENGINE_STATE_STARTED)
		{
			ButtonState button_state = e->button();
			
			_pointer = true;
			_cursor = inverseWorldMatrix().map( e->pos() );
			
			if ( QCanvasItem *item = find_item() )
					select_item(item, button_state);
			else
					select_key();

			canvas()->update();

			//
			// Log event
			//

			Point up = atc_canvas()->to_user_point( _cursor.x(), _cursor.y() );

			KeyValues kvs;
			kvs.push_back( make_pair( string( "event" ), to_string( "mouse_down" ) ) );
			kvs.push_back( make_pair( string( "x"     ), to_string( up.first     ) ) );
			kvs.push_back( make_pair( string( "y"     ), to_string( up.second    ) ) );
			record_event( kvs );
		}
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling CanvasView::contentsMousePressEvent");
	}
}

/*!
 * Process mouse movement events.
 *
 * @param e QMouseEvent.
 */
void CanvasView::contentsMouseMoveEvent( QMouseEvent* e ) {
	try
	{
		if (_engine_state == ENGINE_STATE_STARTED)
		{

			QPoint p = _cursor;
			_cursor  = inverseWorldMatrix().map( e->pos() );

			if ( _pointer && _scale )  {
				atc_canvas()->move_scale( _cursor - p );
				atc_canvas()->update();
				{
					KeyValues kvs;
					kvs.push_back( make_pair( string( "scale" ), string( "move_to" ) ) );
					kvs.push_back( make_pair( string( "x" ), to_string( _cursor.x() ) ) );
					kvs.push_back( make_pair( string( "y" ), to_string( _cursor.y() ) ) );
					record_event( kvs );
				}
			}
			else if ( _pointer && _brl ) {
				atc_canvas()->hook_brl( _cursor.x(), _cursor.y() );
				{
					KeyValues kvs;
					kvs.push_back( make_pair( string( "brl_tool" ), string( "move_to" ) ) );
					kvs.push_back( make_pair( string( "x" ), to_string( _cursor.x() ) ) );
					kvs.push_back( make_pair( string( "y" ), to_string( _cursor.y() ) ) );
					record_event( kvs );
				}
			}
			else if ( _pointer && _vector ) {
				atc_canvas()->move_vector( _cursor );
				{
					KeyValues kvs;
					kvs.push_back( make_pair( string( "vector_tool" ), string( "move_to" ) ) );
					kvs.push_back( make_pair( string( "x" ), to_string( _cursor.x() ) ) );
					kvs.push_back( make_pair( string( "y" ), to_string( _cursor.y() ) ) );
					record_event( kvs );
				}
			}
			canvas()->update();

			//
			// Log event
			//

			Point up = atc_canvas()->to_user_point( _cursor.x(), _cursor.y() );

			KeyValues kvs;
			kvs.push_back( make_pair( string( "event" ), to_string( "mouse_move" ) ) );
			kvs.push_back( make_pair( string( "x"     ), to_string( up.first     ) ) );
			kvs.push_back( make_pair( string( "y"     ), to_string( up.second    ) ) );
			record_event( kvs );
		}
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling CanvasView::contentsMouseMoveEvent");
	}
}

/*!
 * Process mouse release events.
 *
 * @param e QMouseEvent.
 */
void CanvasView::contentsMouseReleaseEvent( QMouseEvent* e ) {
    try
	{
		if (_engine_state == ENGINE_STATE_STARTED)
		{	
			_pointer = false;
			_cursor = inverseWorldMatrix().map( e->pos() );

			if ( QCanvasItem *item = find_item() )
					deselect_item( item );

			if ( _scale ) {
				_scale = false;
				{
					KeyValues kvs;
					kvs.push_back( make_pair( string( "scale" ), string( "end_move" ) ) );
					record_event( kvs );
				}
			}
			else if ( _brl ) {
				_brl = false;
				{
					KeyValues kvs;
					kvs.push_back( make_pair( string( "brl_tool" ), string( "end_move" ) ) );
					record_event( kvs );
				}
			}
			else if ( _vector ) {
				_vector = false;
				{
					KeyValues kvs;
					kvs.push_back( make_pair( string( "vector_tool" ), string( "end_move" ) ) );
					record_event( kvs );
				}
				
				VectorTool *v = atc_canvas()->vector_tool();
				if ( v->is_valid() ) {
					v->action();
					ask_efficiency();
				}
				else {
					show_selector( _ang_select, _cursor, v->aircraft()->agent() );
				}
				atc_canvas()->cancel_vector();
			}

			canvas()->update();

			//
			// Log event
			//

			Point up = atc_canvas()->to_user_point( _cursor.x(), _cursor.y() );

			KeyValues kvs;
			kvs.push_back( make_pair( string( "event" ), to_string( "mouse_up" ) ) );
			kvs.push_back( make_pair( string( "x"     ), to_string( up.first   ) ) );
			kvs.push_back( make_pair( string( "y"     ), to_string( up.second  ) ) );
			record_event( kvs );
		}
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling CanvasView::contentsMouseReleaseEvent");
	}
}

/*!
 * Process mouse wheel events (disable).
 *
 * @param e QMouseEvent.
 */
void CanvasView::contentsWheelEvent( QWheelEvent * ) {}

/*!
 * Process key press events - used for tool manipulation.
 *
 * @param e QKeyEvent.
 */
void 
CanvasView::keyPressEvent( QKeyEvent* e ) 
{
    try
	{
		if (_engine_state == ENGINE_STATE_STARTED)
		{	
			if (Qt::NoButton == e->state()) 
			{
				int keyPressed = e->key();
				std::string keyPressedStr;

				if (keyPressed == Qt::Key_F1)
				{
					keyPressedStr = "F1";
				}
                else if (keyPressed == Qt::Key_Up)
                {
                    keyPressedStr = "Up Arrow";
                }
                else if (keyPressed == Qt::Key_Down)
                {
                    keyPressedStr = "Down Arrow";
                }
                else if (keyPressed == Qt::Key_Left)
                {
                    keyPressedStr = "Left Arrow";
                }
                else if (keyPressed == Qt::Key_Right)
                {
                    keyPressedStr = "Right Arrow";
                }
				else
				{
					keyPressedStr = (char) keyPressed;
				}

				AircraftSprite* selectedAircraft = atc_canvas()->SelectedAircraft();
				if (selectedAircraft != NULL)
				{
					switch (selectedAircraft->agent()->control())
					{
					case CS_PROPOSED:
						ExcelLogging::ExcelLogger::instance()->LogAcceptanceKeysPressed(
							selectedAircraft->agent()->callsign(), 
							keyPressedStr,
							atc_canvas()->CurrentTime(),
                            Timer::instance()->elapsed_time());
						break;
					case CS_PRE_HANDOFF:
						ExcelLogging::ExcelLogger::instance()->LogHandoffKeysPressed(
							selectedAircraft->agent()->callsign(), 
							keyPressedStr,
							atc_canvas()->CurrentTime(),
                            Timer::instance()->elapsed_time());
						break;
					default:
						break;
					}

					if ((keyPressed == (int) AC_ACCEPT_KEY_DEFAULT) ||
						(keyPressed == selectedAircraft->agent()->CoordinationRequestKey()))
					{
						int acceptanceCount = 0;
											
						//ExcelLogging::ExcelLogger::instance()->LogFlashDetailsKeysPressed(selectedAircraft->agent()->callsign(), (std::string) e->text());

						switch (selectedAircraft->agent()->UserInputState())
						{
						case USER_INPUT_STATE_DEFAULT:
							if (keyPressed == (int) AC_ACCEPT_KEY_DEFAULT)
							{
								acceptanceCount = selectedAircraft->accept();
								if (acceptanceCount == 1)
								{
									ExcelLogging::ExcelLogger::instance()->LogAcceptanceError(
										selectedAircraft->agent()->callsign(), 
										ExcelLogging::TASK_ERROR_CORRECT);
									emit SucceededAcceptanceTask(selectedAircraft->agent()->callsign());
								}
							}
							break;
						case USER_INPUT_STATE_COORDINATION_REQUEST:
							if (keyPressed == (int) AC_ACCEPT_KEY_DEFAULT)
							{
								acceptanceCount = selectedAircraft->accept();

								if ((acceptanceCount == -1) || (acceptanceCount == 1))
								{
									ExcelLogging::ExcelLogger::instance()->LogAcceptanceError(
										selectedAircraft->agent()->callsign(), 
										ExcelLogging::TASK_ERROR_INCORRECT);
									
									if (selectedAircraft->HasReminderBox())
									{
										selectedAircraft->DisableReminderBox();
									}
									else
									{
										atc_canvas()->DisableReminderBox();
									}
									emit FailedAcceptanceTask(selectedAircraft->agent()->callsign());
								}
							}
							else if (keyPressed == selectedAircraft->agent()->CoordinationRequestKey())
							{
								acceptanceCount = selectedAircraft->accept();
								if (acceptanceCount == 1)
								{
									ExcelLogging::ExcelLogger::instance()->LogAcceptanceError(
										selectedAircraft->agent()->callsign(), 
										ExcelLogging::TASK_ERROR_CORRECT);

									if (selectedAircraft->HasReminderBox())
									{
										selectedAircraft->DisableReminderBox();
									}
									else
									{
										atc_canvas()->DisableReminderBox();
									}

									emit SucceededAcceptanceTask(selectedAircraft->agent()->callsign());
								}
							}
							break;
						default:
							break;
						}
					}
					else if (keyPressed == (int) atc_canvas()->SelectedAircraft()->agent()->HandoffKey() ||
                                keyPressed == AC_HANDOFF_KEY)
					{
						AircraftSprite* selectedAircraft = atc_canvas()->SelectedAircraft();
						if (selectedAircraft != NULL)
						{
							int handoffCount = selectedAircraft->handoff();

							if (handoffCount == 1)
							{
								ExcelLogging::ExcelLogger::instance()->LogHandoffError(
										selectedAircraft->agent()->callsign(), 
										ExcelLogging::TASK_ERROR_CORRECT);

								emit SucceededHandoffTask(selectedAircraft->agent()->callsign());
							}
						}
					}
				}

				set_tool_key(e->key());
			}
			e->ignore();
			
			//
			// Log event
			//
			KeyValues kvs;
			kvs.push_back( make_pair( string( "event" ), to_string( "key_press" ) ) );
			kvs.push_back( make_pair( string( "key"   ), to_string( QString( QKeySequence( e->key() ) ).latin1() ) ) );
			record_event( kvs );
		}
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling CanvasView::keyPressEvent");
	}
}

/*!
 * Virtual function to handle resizing of canvas view.
 *
 * @param e QResizeEvent defining resize event
 * 
 * @todo Set sprite drawing scale should be set for all sprites (class level) on
 * the basis of the view ie different views have different drawing scales. 
 * Sprites don't need independant drawing scales(??).
 */
void CanvasView::resizeEvent( QResizeEvent* e ) {
	try
	{
		QSize newsize =	e->size();
		
		QWMatrix wm(worldMatrix());
		wm.reset();
		wm.scale(
				double(newsize.width()) / double(canvas()->width())
			  ,	double(newsize.height()) / double(canvas()->height())
		);
		setWorldMatrix( wm );
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling CanvasView::resizeEvent");
	}
}

/*!
 *
 */
void CanvasView::showEvent( QShowEvent* e ) {
	try
	{
		// startTimer( ONE_MINUTE );		// disable ask_workload which happens on timeout
		setFocus();
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling CanvasView::showEvent");
	}
}

/*!
 *
 */
void CanvasView::timerEvent( QTimerEvent *e ) {
	try
	{
	//if ( _wltimer == e.timerId() ) {
		ask_workload();
	//}
	}
	catch (...)
	{
		logging::Logger::instance()->error("Error occurred while calling CanvasView::timerEvent");
	}
}


//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 * Find atc::CanvasItem with higest order Z at cursor position
 */
QCanvasItem* CanvasView::find_item() {
    QCanvasItem *i = 0;

	// snap rectangle
	QRect r( 
			_cursor.x() - SNAP_DIMENSION
		  , _cursor.y() - SNAP_DIMENSION
		  , 2*SNAP_DIMENSION, 2*SNAP_DIMENSION 
	);

	QCanvasItemList items = canvas()->collisions( r );
	QCanvasItemList::iterator item = items.begin();
	for ( ; item != items.end(); ++item ) {
		if ( CanvasItem::RTTI_ACTIVE <= (*item)->rtti() ) {
			i = *item;
			break;
		}
	}

	return i;
}

/*!
 *
 */
void CanvasView::action_item( QCanvasItem *item ) {

	switch ( item->rtti() ) {

	case CanvasItem::RTTI_LOCATOR:
		action_aircraft( dynamic_cast< ACLocator* >( item ) );
		break;

	case CanvasItem::RTTI_BRLDATA:
		action_brl( dynamic_cast< BRLDataSprite* >( item ) );
		break;
	
	default:
		;
	}

}

/*!
 *
 */
void CanvasView::action_aircraft( ACLocator *ac ) { ac->rotate_callout(); }

/*!
 *
 */
void CanvasView::action_brl( BRLDataSprite *s ) { 
	s->kill(); 
	{
		KeyValues kvs;
		kvs.push_back( make_pair( string( "brl_tool" ), string( "destroy" ) ) );
		record_event( kvs );
	}
}

void CanvasView::select_item(QCanvasItem *item, ButtonState button_state) 
{
	switch ( item->rtti() ) 
	{
	case CanvasItem::RTTI_CALLBLK:
		SelectCallsign(dynamic_cast<ACCallBlk*>(item), button_state);
		break;
	case CanvasItem::RTTI_WPLOCATOR:
		select_waypoint( dynamic_cast< WPLocator* >( item ) );
		break;
	case CanvasItem::RTTI_LOCATOR:
		select_aircraft( dynamic_cast< ACLocator* >( item ) );
		break;
	case CanvasItem::RTTI_ALTBLK:
		select_altitude(dynamic_cast< ACAltBlk* >( item ), button_state);
		break;
	case CanvasItem::RTTI_VELBLK:
		select_velocity(dynamic_cast< ACVelBlk* >( item ), button_state);
		break;
	case CanvasItem::RTTI_SCALE:
		_scale = true;
		{
			KeyValues kvs;
			kvs.push_back( make_pair( string( "scale" ), string( "start_move" ) ) );
			record_event( kvs );
		}
		break;
	case CanvasItem::RTTI_BRLDATA:
		select_brl( dynamic_cast< BRLDataSprite* >( item ) );
		break;
	default:
		break;
	}
}

/*!
 *
 */
void CanvasView::select_waypoint( WPLocator *s ) {
	switch ( _key )	{

	case AC_BRLINE_KEY:
		_brl = true;
		atc_canvas()->add_brl( s->waypoint() );
		{
			KeyValues kvs;
			kvs.push_back( make_pair( string( "brl_tool" ), string( "new" ) ) );
			kvs.push_back( make_pair( string( "anchor"   ), string( "waypoint" ) ) );
			record_event( kvs );
		}
		break;
	}
}

void 
CanvasView::select_aircraft( ACLocator *ac ) 
{
	// Do not allow an aircraft to be selected when in certain states
	if ((ac->sprite()->agent()->control() != CS_NONE) &&
		(ac->sprite()->agent()->control() != CS_NOMORE) &&
		(ac->sprite()->agent()->control() != CS_ACCEPTED_TASK_INCOMPLETE) &&
		(ac->sprite()->agent()->control() != CS_ACCEPTED_TASK_COMPLETE) &&
		(ac->sprite()->agent()->control() != CS_OVEROUT))
	{
		atc_canvas()->DeselectAllAircraft();
		ac->select(true);
		
        ExcelLogging::ExcelLogger::instance()->LogAircraftSelection(
			ac->sprite()->agent()->callsign(), 
			atc_canvas()->CurrentTime(),
            Timer::instance()->elapsed_time());
	}
}

/*!
 *
 */
void CanvasView::select_altitude(ACAltBlk *ac, ButtonState button_state) 
{
	AircraftAgent* agent;

	switch (button_state)
	{
	case LeftButton:
		agent = ac->sprite()->agent();
		
		if ((agent->control() == CS_PROPOSED) ||	
			(agent->control() == CS_ACCEPTED_TASK_INCOMPLETE) ||
			(agent->control() == CS_ACCEPTED_TASK_COMPLETE))
		{
            if (agent->CanChangeAltitude())
            {
			    show_selector( _cfl_select, _cursor, ac->sprite()->agent() );
            }
		}

		break;
	case RightButton:
		if (ac->sprite()->agent()->FlashParams().flash_item == FLASH_ITEM_FL)
		{
			ShowAircraftAcceptanceDialogBox(ac->sprite()->agent());
		}
		break;
	default:
		break;
	}
}

void
CanvasView::SelectCallsign(ACCallBlk* ac, ButtonState button_state)
{
	switch(button_state)
	{
	case LeftButton:
		break;
	case RightButton:
		if (ac->sprite()->agent()->FlashParams().flash_item == FLASH_ITEM_CS)
		{
			ShowAircraftAcceptanceDialogBox(ac->sprite()->agent());
		}
		break;
	}
}
/*!
 *
 */
void CanvasView::select_velocity(ACVelBlk *ac, ButtonState button_state) 
{
	switch (button_state)
	{
	case LeftButton:
		//show_selector( _vel_select, _cursor, ac->sprite()->agent());
		break;
	case RightButton:
		if (ac->sprite()->agent()->FlashParams().flash_item == FLASH_ITEM_SI)
		{
			ShowAircraftAcceptanceDialogBox(ac->sprite()->agent());
		}
		break;
	default:
		break;
	}
}

/*!
 *
 */
void CanvasView::select_brl( BRLDataSprite *s ) { 
	_brl = true;
	s->select(); 
	{
		KeyValues kvs;
		kvs.push_back( make_pair( string( "brl_tool" ), string( "rehook" ) ) );
		record_event( kvs );
	}
}

/*!
 *
 */
void CanvasView::select_key() {
	switch ( _key )	{
	case AC_BRLINE_KEY:
		/*
		_brl = true;
		atc_canvas()->add_brl( _cursor.x(), _cursor.y() );
		{
			KeyValues kvs;
			kvs.push_back( make_pair( string( "brl_tool" ), string( "new" ) ) );
			kvs.push_back( make_pair( string( "anchor"   ), string( "nothing" ) ) );
			record_event( kvs );
		}
		*/
		break;
	default:
		break;
	}
}

/*!
 *
 */
void CanvasView::deselect_item( QCanvasItem *item ) {
	switch ( item->rtti() ) {

	case CanvasItem::RTTI_WPLOCATOR:
		deselect_waypoint( dynamic_cast< WPLocator* >( item ) );
		break;

	case CanvasItem::RTTI_LOCATOR:
		deselect_aircraft( dynamic_cast< ACLocator* >( item ) );
		break;

	case CanvasItem::RTTI_AC_ROUTEPATH:
		deselect_routepath( dynamic_cast< ACTRouteLine* >( item ) );
		break;

	default:
		;
	}
}

/*!
 *
 */
void CanvasView::deselect_waypoint( WPLocator *s ) {
	if ( _brl ) {
		_brl = false;
		atc_canvas()->hook_brl( s->waypoint() );
		{
			KeyValues kvs;
			kvs.push_back( make_pair( string( "brl_tool" ), string( "hook" ) ) );
			kvs.push_back( make_pair( string( "anchor"   ), string( "waypoint" ) ) );
			record_event( kvs );
		}
	}
}

/*!
 *
 */
void CanvasView::deselect_aircraft( ACLocator *ac ) {
	if ( _brl ) {
		_brl = false;
		atc_canvas()->hook_brl( ac->sprite() );
		{
			KeyValues kvs;
			kvs.push_back( make_pair( string( "brl_tool" ), string( "hook" ) ) );
			kvs.push_back( make_pair( string( "anchor"   ), string( "aircraft" ) ) );
			kvs.push_back( make_pair( string( "callsign" ), string( ac->sprite()->agent()->callsign() ) ) );
			record_event( kvs );
		}
	}
}

/*!
 *
 */
void CanvasView::deselect_routepath( ACTRouteLine *l ) {
	if ( _vector ) {
		VectorTool *v = atc_canvas()->vector_tool();
		if ( v->aircraft() == l->aircraft() ) {
			_vector = false;
			{
				KeyValues kvs;
				kvs.push_back( make_pair( string( "vector_tool" ), string( "end_move" ) ) );
				record_event( kvs );
			}
			l->reroute( _cursor );
			atc_canvas()->cancel_vector();
			ask_efficiency();
		}
	}
}

/*!
 *
 */
void CanvasView::show_selector(ListBox *selector, const QPoint &p, AircraftAgent *ac) 
{
	if ( _shown_selector ) {
		_shown_selector->clear_aircraft();
		_shown_selector->hide();
	}
	_shown_selector = selector;

	_shown_selector->set_aircraft( ac );

	// calc cavas view location
	QPoint pt = worldMatrix().map( p );
	pt -= QPoint( 
			_shown_selector->width() / 5
		  , _shown_selector->height() / 2 
	);

	//
	// log requirement request
	//
	KeyValues request;
	request.push_back( make_pair( string( "solution-request" ), to_string( _shown_selector->type()        ) ) );
	request.push_back( make_pair( string( "callsign"         ), to_string( _shown_selector->aircraft_id() ) ) );
	record_event( request );

	//
	// show dialog
	// 
	_shown_selector->move( pt );
	int result = _shown_selector->exec();

	//
	// log requirement
	//
	KeyValues kvs;
	kvs.push_back( make_pair( string( "solution" ), to_string( _shown_selector->type()        ) ) );
	kvs.push_back( make_pair( string( "callsign" ), to_string( _shown_selector->aircraft_id() ) ) );
	kvs.push_back( make_pair( string( "value"    ), to_string( _shown_selector->value()       ) ) );
	kvs.push_back( make_pair( string( "result"   ), to_string( result                         ) ) );
	record_event( kvs );

	// 
	// CSAQ
	//
	if ( QDialog::Accepted == result )
		ask_efficiency();
}

void CanvasView::ShowAircraftAcceptanceDialogBox(AircraftAgent *agent)
{
	if (agent->control() == CS_ACCEPTED_TASK_INCOMPLETE)
	{
		AcceptanceStateDialogBox* accept_state_dialog = 
			new AcceptanceStateDialogBox(dynamic_cast<QWidget*>(this), "", agent);
			
		accept_state_dialog->connect(agent, SIGNAL(ProposedStateTimeout()),
											accept_state_dialog, SLOT(reject()));

		// calc cavas view location
		QPoint pt = worldMatrix().map(_cursor);
		pt -= QPoint(accept_state_dialog->width() / 5, 
						accept_state_dialog->height() / 2);

		// show dialog
		accept_state_dialog->move(pt);
		int result = accept_state_dialog->exec();

		if (result == 1)
		{
			agent->set_control(CS_ACCEPTED_TASK_COMPLETE);
			ExcelLogging::ExcelLogger::instance()->LogAcceptanceError(
				agent->callsign(), 
				ExcelLogging::TASK_ERROR_CORRECT);
			emit SucceededAcceptanceTask(agent->callsign());
		}
		else if (result == 0)
		{
			agent->set_control(CS_ACCEPTED_TASK_COMPLETE);
			ExcelLogging::ExcelLogger::instance()->LogAcceptanceError(
				agent->callsign(), 
				ExcelLogging::TASK_ERROR_INCORRECT);
			emit FailedAcceptanceTask(agent->callsign());
		}

		accept_state_dialog->disconnect(agent, SIGNAL(ProposedStateTimeout()),
										accept_state_dialog, SLOT(reject()));
		delete accept_state_dialog;
	}
}

/*!
 *
 */
void CanvasView::show_requirement( const QPoint &pt, AircraftAgent *ac ) {
	_requirement_dialog->set_aircraft( ac );
	_requirement_dialog->move( pt );
	int result = _requirement_dialog->exec();

	//
	// log requirement
	//
	KeyValues kvs;
	kvs.push_back( make_pair( string( "solution" ), to_string( "requirement"                      ) ) );
	kvs.push_back( make_pair( string( "callsign" ), to_string( _requirement_dialog->aircraft_id() ) ) );
	kvs.push_back( make_pair( string( "level"    ), to_string( _requirement_dialog->level()       ) ) );
	kvs.push_back( make_pair( string( "distance" ), to_string( _requirement_dialog->distance()    ) ) );
	kvs.push_back( make_pair( string( "result"   ), to_string( result                             ) ) );
	record_event( kvs );

	// 
	// CSAQ
	//
	if ( QDialog::Accepted == result )
		ask_efficiency();
}

/*!
 *
 */
void CanvasView::record_event( KeyValues &kvs ) {
	kvs.push_front( make_pair( string( "view" ), string( "experiment") ) );
	emit event_recorded( kvs );
}

void CanvasView::set_tool_key( int key ) {
	_key = key;

	QString tool =						//	"**********"
			//AC_ACCEPT_KEY_DEFAULT	== _key ?	" ACCEPT   " :
			//AC_HANDOFF_KEY		== _key ?	" HANDOFF  " :
			//AC_HISTORY_KEY		== _key ?	" History  " :
			//AC_SRPROBE_KEY		== _key ?	" SR-Probe " :
			//AC_ROUTE_KEY			== _key ?	" Route    " :
			//AC_VECTOR_KEY			== _key ?	" Vector   " :
			//AC_REQUIREMENT_KEY	== _key ?	" Require  " :
			//AC_BRLINE_KEY			== _key ?	" BR-Line  " :
											" ---      " ;

	atc_canvas()->set_tool( tool );
}

void
CanvasView::RegainKeyboardFocus()
{
    this->grabKeyboard();
}

//------------------------------------------------------------------------------
// [private slots]
//

/*!
 *
 */
void CanvasView::ask_workload() {
	{
		KeyValues kvs;
		kvs.push_back( make_pair( string( "ask_workload" ), to_string( "" ) ) );
		record_event( kvs );
	}

	atc_canvas()->pause();

	_workload_question->move( 
			( width()  - _workload_question->width()  ) / 2
		  , ( height() - _workload_question->height() ) / 2
	);

	QTime tmr;
	tmr.start();
	int w = _workload_question->exec();
	int t = tmr.elapsed();

	atc_canvas()->resume();

	{
		KeyValues kvs;
		kvs.push_back( make_pair( string( "workload"      ), to_string( w ) ) );
		kvs.push_back( make_pair( string( "response_time" ), to_string( t ) ) );
		record_event( kvs );
	}
}

/*!
 *
 */
void CanvasView::ask_efficiency() {
	/*
	{
		KeyValues kvs;
		kvs.push_back( make_pair( string( "ask_efficiency" ), to_string( "" ) ) );
		record_event( kvs );
	}

	atc_canvas()->pause();
	_efficiency_question->move( 
			( width()  - _efficiency_question->width()  ) / 2
		  , ( height() - _efficiency_question->height() ) / 2
	);

	QTime tmr;
	tmr.start();
	int e = _efficiency_question->exec();
	int t = tmr.elapsed();

	atc_canvas()->resume();

	{
		KeyValues kvs;
		kvs.push_back( make_pair( string( "efficiency"    ), to_string( e ) ) );
		kvs.push_back( make_pair( string( "response_time" ), to_string( t ) ) );
		record_event( kvs );
	}
	*/
}

void CanvasView::HandleTrialTaskStarted()
{
	_engine_state = ENGINE_STATE_STARTED;
	setFocus();
}

void CanvasView::HandleTrialTaskPaused()
{
	_engine_state = ENGINE_STATE_PAUSED;
}

////////////////////////////////////////////////////////////////////////////////

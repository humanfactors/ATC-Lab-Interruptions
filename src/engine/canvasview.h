/**
      @project  : ATC-Lab [engine]

      @file     : $RCSfile: canvasview.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.8.2.1 $ )
      @date     : $Date: 2014/12/06 08:13:23 $
      @state    : $State: Exp $

      $Log: canvasview.h,v $
      Revision 1.8.2.1  2014/12/06 08:13:23  Aaron Yeung
      ATCLab v2.4.5.7
      - Fixed two issues:
      		1) Canvasview grabs all the keypress events which causes other widgets like the altitude selection
      		   tool to not be able to be escaped
      		2) When aircraft are set with a customized handoff key, the default handoff key 'h' is not usable
      		   for handoff

      Revision 1.8  2008/10/21 13:57:51  Aaron Yeung
      - Added functionality for experimenter to re-ask a question if the participant gives the wrong answer.
      - Added new xml element
      <questionBox>
              <atc:answer></atc:answer>
              <atc:incorrectResponse></atc:incorrectRespons>
      </questionBox>

      Revision 1.7  2008/06/30 11:02:58  Aaron Yeung
      Added aircraft handoff task

      Revision 1.6  2008/05/13 17:00:32  Aaron Yeung
      Added functionality to:

      - Allow an aircraft's callsign to flash for an acceptance task
      - Add a minimum time to enable an ok button on an instruction form
      - Create reminder box

      Revision 1.5  2007/07/26 11:31:25  Aaron Yeung
      Disabled pressing of keys and clicking on aircraft when the engine is paused

      Revision 1.4  2007/07/25 14:44:33  Aaron Yeung
      Implemented scoring system

      Revision 1.3  2007/07/16 10:54:53  Aaron Yeung
      Modified acceptance task so that a user can continue to try to press the correct key for an acceptance task until the time limit is exceeded. Previously, if the user pressed the wrong key, they would not be able to re-enter the correct key again

      Revision 1.2  2007/07/09 08:36:10  Aaron Yeung
      Added aircraft acceptance task where the user has to accept a aircraft in the CS_PROPOSED state, right clicking on either the speed information or flight level and then pressing a specfic key

      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.17  2007/02/28 00:48:12  seth
      implemented tool identifier

      Revision 1.16  2006/10/26 05:59:52  seth
      working implementation of experiment time logging

      Revision 1.15  2006/10/09 04:59:46  seth
      bug fixes

      Revision 1.14  2006/09/27 06:13:12  seth
      popup questions, requirements and bug fixes

      Revision 1.13  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.12  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.11  2006/09/17 12:45:35  seth
      implementing solution mechanisms

      Revision 1.10  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.9  2006/09/13 05:38:49  seth
      full working implemtation of Range/Bearing tool

      Revision 1.8  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.7  2006/09/12 05:43:43  seth
      aircraft route tool working implementation

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

#ifndef __ATC_CANVASVIEW_HDR__ 
#define __ATC_CANVASVIEW_HDR__


#include <qcanvas.h>
#include "AcceptanceStateDialogBox.h"
#include "atc_types.h"


namespace atc{

	/*!
	 *
	 */
	class ACAltBlk;
	class ACCallBlk;
	class ACVelBlk;
	class ACLocator;
	class ACTRouteLine;
	class AircraftAgent;
	class BRLDataSprite;
	class Canvas;
	class CFLSelector;
	class EfficiencyQuestion;
	class HeadingSelector;
	class ListBox;
	class RequirementSelector;
	class VelocitySelector;
	class WorkloadQuestion;
	class WPLocator;

	enum EngineState
	{
		ENGINE_STATE_STARTED,
		ENGINE_STATE_PAUSED
	};

	/*!
	 *
	 */
	class CanvasView : public QCanvasView 
	{
		Q_OBJECT

		public:
			CanvasView( QCanvas *, QWidget *, const char * );
			~CanvasView();

			Canvas * atc_canvas() const;

		public slots:
			void HandleTrialTaskStarted();
			void HandleTrialTaskPaused();
            void RegainKeyboardFocus();

		protected: //virtual event handlers
			void contentsMouseDoubleClickEvent( QMouseEvent * );
			void contentsMousePressEvent      ( QMouseEvent * );
			void contentsMouseReleaseEvent    ( QMouseEvent * );
			void contentsMouseMoveEvent       ( QMouseEvent * );
			void contentsWheelEvent           ( QWheelEvent * );
			void keyPressEvent                ( QKeyEvent * );
			void resizeEvent                  ( QResizeEvent * );
			void showEvent                    ( QShowEvent * );
			void timerEvent                   ( QTimerEvent * );

		private:
			QCanvasItem* find_item();

			void action_item       ( QCanvasItem *   );
			void action_aircraft   ( ACLocator *     );
			void action_brl        ( BRLDataSprite * );
			void SelectCallsign	(ACCallBlk* ac, ButtonState button_state);
			void select_item       ( QCanvasItem *, ButtonState button_state);
			void select_waypoint   ( WPLocator *     );
			void select_aircraft   ( ACLocator *     );
			void select_altitude   ( ACAltBlk *, ButtonState button_state);
			void select_velocity   ( ACVelBlk *, ButtonState button_state);
			void select_brl        ( BRLDataSprite * ); 
			void deselect_item     ( QCanvasItem *   );
			void deselect_waypoint ( WPLocator *     );
			void deselect_aircraft ( ACLocator *     );
			void deselect_routepath( ACTRouteLine *  );
			void select_key();
			void show_selector( ListBox *, const QPoint &, AircraftAgent * );
			void ShowAircraftAcceptanceDialogBox(AircraftAgent *agent);
			void ShowAircraftAcceptanceTaskDialog();
			void show_requirement( const QPoint &, AircraftAgent * );
			void record_event( KeyValues & );
			void set_tool_key( int );

		private slots:
			void ask_workload();
			void ask_efficiency();

		signals:
			void event_recorded( KeyValues & );
			void FailedAcceptanceTask(const char*);
			void SucceededAcceptanceTask(const char*);
			void FailedHandoffTask(const char*);
			void SucceededHandoffTask(const char*);

		private:
			QPoint _cursor;
			int    _key;
			bool   _pointer;
			bool   _scale;
			bool   _brl;
			bool   _vector;
			EngineState _engine_state;

			ListBox          *_shown_selector;

			VelocitySelector *_vel_select;
			CFLSelector      *_cfl_select;
			HeadingSelector  *_ang_select;

			RequirementSelector *_requirement_dialog;

			WorkloadQuestion   *_workload_question;
			EfficiencyQuestion *_efficiency_question;
			unsigned int m_timeOfAcceptanceKeyPress;

	};

};


#endif

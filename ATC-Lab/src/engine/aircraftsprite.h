/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: aircraftsprite.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.8.2.2 $ )
      @date     : $Date: 2015/03/22 13:55:52 $
      @state    : $State: Exp $

      $Log: aircraftsprite.h,v $
      Revision 1.8.2.2  2015/03/22 13:55:52  Aaron Yeung
      - Added scriptable switch to enable/disable the ability to double click an aircraft and rotate it's details
      - Added ability for nback task initial display and stimuli to be presented in subseconds
      - Fixed bug in the instruction task that only prints every second instruction tasks textbox to the output CSV file

      Revision 1.8.2.1  2014/11/26 14:15:29  Aaron Yeung
      Added ability to store the canvas view widget in aircraft sprite to pass onto the reminder box to allow the reminder box dismiss button to be displayed

      Revision 1.8  2010/04/11 02:31:48  Aaron Yeung
      Added reminder boxes that can be attached to aircraft. These reminder boxes move with the aircraft that they are attached to

      Revision 1.7  2010/04/08 13:19:30  Aaron Yeung
      Initial attempt at adding reminder box to individual aircraft

      Revision 1.6  2008/07/12 19:53:28  Aaron Yeung
      Added prehandoff mode to aircraft such that they flash blue when exiting the sector

      Revision 1.5  2008/06/30 11:05:19  Aaron Yeung
      Added aircraft handoff task
      Modified aircraft acceptance task so that user only needs to select an aircraft and press the 'a' key

      Revision 1.4  2007/07/26 10:22:05  Aaron Yeung
      Disallowed acceptance of aircraft that are not in the proposed state

      Revision 1.3  2007/07/09 00:36:25  Aaron Yeung
      renamed aircraft accepted state blink items to use the word flash instead

      Revision 1.2  2007/07/08 17:05:53  Aaron Yeung
      Modified to blink accept state items when in accepted state

      Revision 1.1.1.2  2007/06/21 12:22:41  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.23  2006/11/07 03:05:44  seth
      added additional logging

      Revision 1.22  2006/10/25 04:49:58  seth
      fixing logging

      Revision 1.21  2006/10/18 03:58:33  seth
      snap functionality for selection & fixed bug causing access violation once aircraft had reached destination

      Revision 1.20  2006/09/24 00:28:03  seth
      fixed vectoring and rerouting

      Revision 1.19  2006/09/17 12:45:35  seth
      implementing solution mechanisms

      Revision 1.18  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.17  2006/09/14 12:24:34  seth
      reconciled aircraft tool class files

      Revision 1.16  2006/09/14 07:28:06  seth
      inital implementation of sector controller state

      Revision 1.15  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.14  2006/09/11 06:13:44  seth
      working on route tool

      Revision 1.13  2006/09/07 06:46:21  seth
      reimplementing tools - history and short route probe

      Revision 1.12  2006/09/06 13:09:24  seth
      refactoring translations into canvas

      Revision 1.11  2006/09/06 06:22:23  seth
      transformation problems

      Revision 1.10  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.9  2006/08/29 08:44:56  seth
      reimplementing mvc pattern

      Revision 1.8  2006/08/28 06:21:48  seth
      refactoring mvc

      Revision 1.7  2006/08/27 13:01:19  seth
      progress ....*.....

      Revision 1.6  2006/08/24 04:12:51  seth
      seperating model from visuals

      Revision 1.5  2006/08/07 06:22:46  seth
      implemtation of performance data

      Revision 1.4  2006/07/19 08:39:18  seth
      implementing aircraft agent

      Revision 1.3  2006/06/15 03:58:57  seth
      refactoring aircraft agents

      Revision 1.2  2006/06/07 05:50:14  seth
      refactor prior to implementation of performance data lookup


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/


#ifndef __ATC_AIRCRAFTSPRITE_HDR__
#define __ATC_AIRCRAFTSPRITE_HDR__


#ifdef WIN32
	#pragma warning( disable:4786 )
#endif

#include <qobject.h>
#include "atc_types.h"
#include "aircraftparam.h"
#include "aircraft_data.h"
#include "ReminderBox.h"
#include "canvasview.h"
#include <qcolor.h>
#include <functional>
#include <vector>

//
// Qt classes
//
class QCanvas;
class QCanvasEllipse;
class QCanvasItem;
class QCanvasLine;
class QCanvasText;
class QPoint;
class QTimer;


namespace atc{


	//
	// atc class declarations
	//

	class  AircraftAgent;
	class  AircraftHistory;
	class  AircraftPath;
	struct AircraftPathToken;

	class  ACLocator;
	class  ACCallOut;
	class  ACDataBlk;
	class  ACCallBlk;
	class  ACAltBlk;
	class  ACVelBlk;

	class  ACHistoryTool;
	class  ACProbeTool;
	class  ACRouteTool;

	class  Canvas;
	class  HistoryTool;
	class  ProbeTool;
	class  RouteTool;


	//
	// 
	//

	enum ActionType { 
			TOGGLE_ROUTE_TOOL
		  , TOGGLE_PROBE_TOOL
		  , TOGGLE_HISTORY_TOOL
	};


	/*!
	 * \class atc::AircraftSprite atc_aircraftsprite.h
	 */
	class AircraftSprite : public QObject {
		Q_OBJECT

	public:
		AircraftSprite( Canvas *, AircraftAgent * );
		~AircraftSprite();

	public:
		void update(long a_currentTime = INVALID_VALUE);

	public: // mutator interface
		void set_data( const char * );
		void set_call( const char * );
		void set_alt( double );
		void set_alt( double, double );
		void set_cfl( double );
		void set_vel( double );

		void move_to( double, double );
		void set_colour( const QColor & );
		void set_visible( bool );
		void set_flash( bool );
		void SetAcceptModeFlash(bool flash);
		void SetPreHandoffModeFlash(bool a_flash);
        void SetCanvasView(CanvasView* a_canvasView);
        void SetDisableCalloutRotation(bool a_disable = false);

		void rotate_callout();
		void toggle_route();
		void toggle_history();
		void toggle_probe();

		void vector( double, double );

		int accept();
		int handoff();
		void select(bool a_select = true);
		bool selected() const { return _selected; }

		void DisableReminderBox();

		bool HasReminderBox() const { return m_reminderBox.get() != NULL; }

	public:
		Canvas*        canvas() { return _canvas; }	// @todo:
		AircraftAgent* agent()  { return _aircraft; }

		double cx() const;
		double cy() const;

		double v() const;
		double a() const;

		const AircraftPath & path() const;
		const AircraftHistory & history() const;

		bool locator_selected( const QPoint & ) const;

		void reroute( const AircraftPathToken *, const Point & );

	public slots:
		void invalidate() { _update = true; }
		void next_waypoint();
		void destroy();

	private: // initialize
		void create_locator( );
		void create_callout( );
		void create_textblk( );
		void init_textblk( QCanvasText *, int , int );
		void create_vector( );
		void build_outline( QCanvas * );
		void add2sprite( QCanvasItem * );

		QColor color_lookup() const;
		bool   flash_lookup() const;

		void decode_altblk( double &, double & );

		Point convert2canvas( const double, const double ) const;

	private slots:
		void blink();
		void AcceptStateBlink();
		void PreHandoffStateBlink();

	signals:
		void event_recorded( KeyValues & );

	private:
		AircraftAgent  *_aircraft;

		Canvas         *_canvas;
        CanvasView*     m_canvasView;
		
		std::vector< QCanvasItem * > _sprite;
		
		ACLocator *_locator;
		ACCallOut *_callout;
		ACDataBlk *_datablk;
		ACCallBlk *_callblk;
		ACAltBlk  *_altblk;
		ACVelBlk  *_velblk;
		std::auto_ptr<ReminderBox> m_reminderBox;

		ACProbeTool    *_velvector;
		ACHistoryTool  *_history;
		ACRouteTool    *_route;


		double  _x, _y;		// canvas location
		bool    _update;
		bool    _visible;
		QColor  _colour;

		QTimer *_flash_timer;
		QTimer *_accept_state_flash_timer;
		std::auto_ptr<QTimer> m_preHandoffStateFlashTimer;

		bool    _blinkon;
		bool	_accept_state_blinkon;
		bool	_selected;
		bool	m_preHandoffStateBlinkOn;
        bool    m_disableCalloutRotation;

	private: // functors

		//
		// MoveBy
		//
		class MoveBy : public std::unary_function< QCanvasItem *, void > {
		public:
			MoveBy( const std::pair< double, double > pt )
				: _dx( pt.first ), _dy( pt.second ) {}; 
			MoveBy( const double dx, const double dy ) 
				  : _dx( dx ), _dy( dy ) {};
			void operator ()( QCanvasItem * ) const;
		private:
			const double _dx, _dy;
		};

		//
		// SetColour
		//
		class SetColour : public std::unary_function< QCanvasItem *, void > {
		public:
			SetColour(const QColor &paint , 
						ControlState state, 
						const AcceptedStateFlashParams& blink_params) 
				: _colour( paint ), _state(state), _blink_params(blink_params) {};
			void operator ()( QCanvasItem * ) const;
		private:
			const QColor &_colour;
			ControlState _state;
			const AcceptedStateFlashParams& _blink_params;
		};

		
		//
		// SetAcceptedStateColour
		//
		class SetAcceptedStateColour : public std::unary_function< QCanvasItem *, void > {
		public:
			SetAcceptedStateColour(const QColor &paint, 
									const AcceptedStateFlashParams& blink_params) 
									: _colour( paint ), _blink_params(blink_params) {};
			void operator ()( QCanvasItem * ) const;
		private:
			const QColor &_colour;
			const AcceptedStateFlashParams& _blink_params;
		};

		class SetPreHandoffStateColour : public std::unary_function<QCanvasItem*, void>
		{
		public:
			SetPreHandoffStateColour(const QColor &a_paint)
				: _colour(a_paint)
			{
			};

			void operator()(QCanvasItem*) const;
		private:
			const QColor& _colour;
		};

		//
		// SetVisible
		//
		class SetVisible : public std::unary_function< QCanvasItem *, void > {
		public:
			SetVisible( const bool status ) 
				: _status( status ) {};
			void operator ()( QCanvasItem * ) const;
		private:
			const bool _status;
		};

	};

};

#endif

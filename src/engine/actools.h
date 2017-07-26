/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: actools.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2 $ )
      @date     : $Date: 2007/07/08 06:18:28 $
      @state    : $State: Exp $

      $Log: actools.h,v $
      Revision 1.2  2007/07/08 06:18:28  Aaron Yeung
      Removed probe dots
      Made probe show future position in seconds

      Revision 1.1.1.2  2007/06/21 12:22:41  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.12  2007/02/07 02:51:56  seth
      1) fixed flashing aircraft to entering not exiting sector; 2) inhibited workload pop-up question; 3) implemented minute markers along short route probe

      Revision 1.11  2006/11/07 03:05:44  seth
      added additional logging

      Revision 1.10  2006/10/18 03:58:33  seth
      snap functionality for selection & fixed bug causing access violation once aircraft had reached destination

      Revision 1.9  2006/09/24 00:28:03  seth
      fixed vectoring and rerouting

      Revision 1.8  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.7  2006/09/20 06:44:01  seth
      vector selection box + some bug fix

      Revision 1.6  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.5  2006/09/14 12:24:34  seth
      reconciled aircraft tool class files

      Revision 1.4  2006/09/12 05:43:43  seth
      aircraft route tool working implementation

      Revision 1.3  2006/09/11 06:13:44  seth
      working on route tool

      Revision 1.2  2006/09/07 06:46:21  seth
      reimplementing tools - history and short route probe

      Revision 1.1  2006/09/06 13:09:24  seth
      refactoring translations into canvas

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __AIRCRAFT_TOOLS_HDR__
#define __AIRCRAFT_TOOLS_HDR__ 


#define POINT          std::pair< double, double >
#define HISTORY_DOTS   std::vector< QCanvasEllipse * >

#include "canvas_types.h"
#include <qcanvas.h>
#include <deque>
#include <vector>


//
// Qt classes
//
class QCanvasLine;
class QCanvasRectangle;
class QCanvasText;


namespace atc {

	//
	// atc
	//
	struct AircraftPathToken;
	class AircraftSprite;
	class Canvas;

    //
    // atc::ACProbeTool
    //

    class ACProbeTool : public QCanvasLine {

    public:
        ACProbeTool( Canvas *, AircraftSprite * );
        ~ACProbeTool();

    public:
		int  value() const;

		void deactivate();
		void toggle();
		void update();

	private:

	private:
		AircraftSprite *_sprite;
		Canvas         *_canvas;
		int             _idx;
		int				_secs;
		HISTORY_DOTS    _dots;
	};


    //
    // atc::ACHistoryTool
    //

    class ACHistoryTool {

    public:
        ACHistoryTool( Canvas *, AircraftSprite * );
        ~ACHistoryTool();

    public:
		void toggle();
		void update();

		void set_colour( const QColor & );
		void set_visible( bool );

		bool is_visible() const;

	private:

	private:
		AircraftSprite *_sprite;
		Canvas         *_canvas;
		HISTORY_DOTS    _dots;

	};


	//
	//
	//

	typedef HISTORY_DOTS HistoryDots;


	//
	// atc::
	//

	class ACTRouteLine;


    //
    // atc::ACRouteTool
    //
    class ACRouteTool {

	private:
		class RouteSection{

		public:
			RouteSection( ACTRouteLine *, QCanvasText *, QCanvasRectangle * );
			RouteSection( const RouteSection & );

			void destroy() const;

			friend class ACRouteTool;
			friend class ACTRouteLine;

		private:
			~RouteSection();

		private:
			ACTRouteLine     *path;
			QCanvasText      *time;
			QCanvasRectangle *back;
		};

    public:
        ACRouteTool( Canvas *, AircraftSprite * );
        ~ACRouteTool();

		friend class ACTRouteLine;

	public:
		bool is_active() const;

		void set_active( bool );

		void toggle();
		void update();

		void reroute( const AircraftPathToken *, const QPoint & );

	private:
		void clear();
		void build();

		void set_time();
		void set_visible( RouteSection *, bool );
		
		RouteSection* create_section( const AircraftPathToken * );

	private:
		AircraftSprite *_aircraft;
		Canvas         *_canvas;

		std::deque< RouteSection* >  _route;
		QCanvasLine	                *_track;

		bool   _shown;
		QColor _paint;

	public:
	    typedef std::deque< RouteSection* > RouteSections;

	};

	//
	// atc::ACTRouteLine
	//
	class ACTRouteLine : public QCanvasLine {

    public:
		ACTRouteLine( ACRouteTool *, const AircraftPathToken * );

		friend class ACRouteTool;

	public:
		AircraftSprite* aircraft() const;
		void reroute( const QPoint & );

    public: //virtual
		int rtti() const { return CanvasItem::RTTI_AC_ROUTEPATH; }

	private:
		ACRouteTool             *_parent;
		const AircraftPathToken *_data;

	};


};

#undef HISTORY_DOTS
#undef POINT

#endif

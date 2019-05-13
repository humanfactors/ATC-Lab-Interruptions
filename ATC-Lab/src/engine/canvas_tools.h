/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: canvas_tools.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.2 $ )
      @date     : $Date: 2009/05/04 09:23:59 $
      @state    : $State: Exp $

      $Log: canvas_tools.h,v $
      Revision 1.2  2009/05/04 09:23:59  Aaron Yeung
      Added DivisionLine which allows a user to draw lines using XML. The user is able to specify a line's ID, start point (x1, y1), end point (x2, y2), colour, width, start time and end time.

      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.8  2006/10/09 04:59:46  seth
      bug fixes

      Revision 1.7  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.6  2006/09/20 06:44:01  seth
      vector selection box + some bug fix

      Revision 1.5  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.4  2006/09/13 05:38:49  seth
      full working implemtation of Range/Bearing tool

      Revision 1.3  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.2  2006/09/12 05:43:43  seth
      aircraft route tool working implementation

      Revision 1.1  2006/06/15 03:58:57  seth
      refactoring aircraft agents


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __ATC_CANVAS_TOOLS_HDR__
#define __ATC_CANVAS_TOOLS_HDR__

#include <qcanvas.h>
#include <vector>
#include "atc_types.h"
#include "canvas_types.h"
#include "atc.h"

namespace atc {

	//
	// class declarations
	//
	class AircraftAgent;
	class AircraftSprite;
	class Canvas;
	class WayPointSprite;

	typedef std::pair<int, int> DisplayTimeRange;

	const std::string	DEFAULT_DIVISION_LINE_COLOR = "red";
	const uint			DEFAULT_DIVISION_LINE_WIDTH = 5;
	//
	// atc::BearingRangeLine
	//

	class BearingRangeLine : public QObject {
		Q_OBJECT

	public:
		BearingRangeLine( Canvas * );
		~BearingRangeLine();

	public:
		void anchor( AircraftSprite * );
		void anchor( WayPointSprite * );
		void anchor( const QPoint & );
		void hook( AircraftSprite * );
		void hook( WayPointSprite * );
		void hook( const QPoint & );
		void unhook();
		void move_by( double, double );
		bool hit( const QPoint & )const;

	public:
		void update();

	public slots:
		void update_anchor( double, double );
		void update_hook( double, double );
		void reanchor( bool );
		void rehook( bool );

	private:
		void initialize( double, double );
		void update_data();

	private:
		Canvas           *_canvas;
		AircraftSprite   *_anchor;
		AircraftSprite   *_hook;
		QCanvasLine      *_line;
		QCanvasLine      *_X1;
		QCanvasLine      *_X2;
		QCanvasText      *_range;
		QCanvasText      *_bearing;
		QCanvasText      *_time;
		QCanvasRectangle *_box;
	};


	//
	// atc::BRLLine
	//
	class BRLLineSprite : public QCanvasLine {

	public:
		BRLLineSprite( QCanvas *c ) : QCanvasLine( c ) {}
		~BRLLineSprite() { hide(); }

	public: //virtual
		int rtti() const { return CanvasItem::RTTI_BRLLINE; }
	
	};

	//
	// atc::BRLData
	//
	class BRLDataSprite : public QCanvasRectangle {

	public:
		BRLDataSprite( BearingRangeLine *, const QRect &, QCanvas * ); 
		~BRLDataSprite();

	public: //virtual
		int rtti() const { return CanvasItem::RTTI_BRLDATA; }

	public:
		void select();
		void kill();

	private:
		BearingRangeLine *_brl;
	
	};


	//
	// atc::VectorTool
	//
	class VectorTool {

	public:
		VectorTool( Canvas *, AircraftSprite * );
		~VectorTool();

	public:
		void update();
		void set_end( int, int );
		void action();

		bool is_valid() const;

		AircraftSprite *aircraft() const { return _aircraft; }

	private:
		AircraftSprite *_aircraft;
		QCanvasLine    *_vector;

	};

	class DivisionLine : public QCanvasLine
	{
	public:
		DivisionLine(	QCanvas*			a_canvas,
						const std::string&	a_id,
						const int&			a_x1,
						const int&			a_y1,
						const int&			a_x2,
						const int&			a_y2,
						const QColor&		a_color = QColor(DEFAULT_DIVISION_LINE_COLOR),
						const uint&			a_width = DEFAULT_DIVISION_LINE_WIDTH);
		~DivisionLine();

		void SetDisplayTime(const int& a_startTime, 
							const int& a_endTime = atc::INVALID_VALUE);
		bool Display(const int& a_time);


	private:
		std::string						m_id;
		std::vector<DisplayTimeRange>	m_displayTimes;
	};
};

#endif


/**

      @project  : ATC-Lab [engine]

      @file     : $RCSfile: scale.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:45 $
      @state    : $State: Exp $

      $Log: scale.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:45  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.4  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.3  2006/08/28 06:21:48  seth
      refactoring mvc

      Revision 1.2  2006/06/01 03:41:23  seth
      implemented CFL and speed selectors


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __ATC_SCALE_HDR__ 
#define __ATC_SCALE_HDR__

#ifdef WIN32
	#pragma warning(disable:4786) // 'identifier' truncation
#endif


#include "canvas_types.h"
#include <qcanvas.h>
#include <vector>


namespace atc{

	//
	// atc:: forward declarations
	//

	class Canvas;


	//
	// [class] atc::ScaleXHair
	//

	class ScaleXHair {

	public:
		ScaleXHair( Canvas *, int, int, int, int );
		~ScaleXHair();

	public:
		void show();
		void hide();

		void  move_by( int, int );

	private:
		void build();
		void create_line( int, int, int, int );

	private:
		Canvas *_canvas;
		int _x, _y;
		int _xextent, _xinterval;
		int _yextent, _yinterval;
		int _ticklen;
		std::vector< QCanvasLine * > _xhair;

	};

	//
	// atc::ScaleLine
	//
	class ScaleLine : public QCanvasLine {

    public:
        ScaleLine( QCanvas *c, ScaleXHair *x ) 
			  : QCanvasLine( c ), _scale( x ) {};
        ~ScaleLine() {};

    public:
		int   rtti() const { return CanvasItem::RTTI_SCALE; }
		//QPointArray areaPoints () const;

	private:
		ScaleXHair *_scale;

	};


};

#endif

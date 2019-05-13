/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: acsprite.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.3 $ )
      @date     : $Date: 2008/06/30 11:07:00 $
      @state    : $State: Exp $

      $Log: acsprite.h,v $
      Revision 1.3  2008/06/30 11:07:00  Aaron Yeung
      Added ability to select individual planes using the mouse

      Revision 1.2  2007/07/26 10:22:06  Aaron Yeung
      Disallowed acceptance of aircraft that are not in the proposed state

      Revision 1.1.1.2  2007/06/21 12:22:41  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.7  2006/09/14 07:28:06  seth
      inital implementation of sector controller state

      Revision 1.6  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.5  2006/09/06 06:22:23  seth
      transformation problems

      Revision 1.4  2006/08/31 06:15:11  seth
      implemented clock

      Revision 1.3  2006/08/28 06:21:48  seth
      refactoring mvc

      Revision 1.2  2006/08/27 13:01:19  seth
      progress ....*.....

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __ATC_ACSPRITE_HDR__
#define __ATC_ACSPRITE_HDR__


#include "aircraftsprite.h"
#include "canvas.h"
#include "canvas_types.h"


namespace atc {

    //
    // atc::ACSprite
    //
	class ACSprite {

	public:
		ACSprite( atc::AircraftSprite *sprite ) : _sprite( sprite ) {}
		~ACSprite() {};

	public:
		void history_tool() { _sprite->toggle_history(); }
		void route_tool()   { _sprite->toggle_route();   }
		void probe_tool()   { _sprite->toggle_probe();   }

		void rotate_callout() { _sprite->rotate_callout(); }

		bool accept()  { return _sprite->accept();  }
		void handoff() { _sprite->handoff(); }
		void select(bool a_select = true)  { _sprite->select(a_select); }
		AircraftSprite * sprite() { return _sprite; }
		
	protected:
		AircraftSprite *_sprite;
	};


    //
    // atc::ACLocator
    //
	class ACLocator : public QCanvasEllipse, public ACSprite {

    public:
        ACLocator( QCanvas *canvas, AircraftSprite *sprite ) 
			: QCanvasEllipse( canvas ), ACSprite( sprite ) {};
        ~ACLocator() {};

    public:
		int  rtti() const { return CanvasItem::RTTI_LOCATOR; }

	};


    //
    // atc::ACCallOut
    //
	class ACCallOut : public QCanvasLine, public ACSprite {

    public:
        ACCallOut( QCanvas *canvas, AircraftSprite *sprite ) 
			: QCanvasLine( canvas ), ACSprite( sprite ) {};
        ~ACCallOut() {};

    public:
		int rtti() const { return CanvasItem::RTTI_CALLOUT; }

	};


    //
    // atc::ACDataBlk
    //
	class ACDataBlk : public QCanvasText, public ACSprite {

    public:
        ACDataBlk( QCanvas *canvas, AircraftSprite *sprite ) 
			: QCanvasText( canvas ), ACSprite( sprite ) {};
        ~ACDataBlk() {};

    public:
		int rtti() const { return CanvasItem::RTTI_DATABLK; }

	};


    //
    // atc::ACCallBlk
    //
	class ACCallBlk : public QCanvasText, public ACSprite {

    public:
        ACCallBlk( QCanvas *canvas, AircraftSprite *sprite ) 
			: QCanvasText( canvas ), ACSprite( sprite ) {};
        ~ACCallBlk() {};

    public:
		int rtti() const { return CanvasItem::RTTI_CALLBLK; }

	};


    //
    // atc::ACAltBlk
    //
	class ACAltBlk : public QCanvasText, public ACSprite {

    public:
        ACAltBlk( QCanvas *canvas, AircraftSprite *sprite ) 
			: QCanvasText( canvas ), ACSprite( sprite ) {};
        ~ACAltBlk() {};

    public:
		int rtti() const { return CanvasItem::RTTI_ALTBLK; }

	};


    //
    // atc::ACVelBlk
    //
	class ACVelBlk : public QCanvasText, public ACSprite {

    public:
        ACVelBlk( QCanvas *canvas, AircraftSprite *sprite ) 
			: QCanvasText( canvas ), ACSprite( sprite ) {};
        ~ACVelBlk() {};

    public:
		int rtti() const { return CanvasItem::RTTI_VELBLK; }

	};


};

#endif

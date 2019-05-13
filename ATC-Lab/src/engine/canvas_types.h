/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: canvas_types.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:42 $
      @state    : $State: Exp $

      $Log: canvas_types.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.3  2006/09/21 11:06:41  seth
      implementing rerouting after vectoring - broken

      Revision 1.2  2006/09/13 05:38:49  seth
      full working implemtation of Range/Bearing tool

      Revision 1.1  2006/09/12 11:23:34  seth
      initial implemtation of Range/Bearing tool

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __ATC_CANVAS_TYPES_HDR__
#define __ATC_CANVAS_TYPES_HDR__


namespace atc {

    //
    // atc::
    //
	namespace CanvasItem {

		const int RTTI_INACTIVE = 1001;
		const int RTTI_ACTIVE   = 2001;
	
		enum RTTI_TYPE {
			    RTTI_SECTOR      = RTTI_INACTIVE
			  , RTTI_SECTORARC
			  , RTTI_ROUTE
			  , RTTI_ROUTEARC
			  , RTTI_WPTEXT
			  , RTTI_WPLOCATOR   = RTTI_ACTIVE
			  ,	RTTI_LOCATOR		// aircraft
			  ,	RTTI_CALLOUT
			  ,	RTTI_DATABLK
			  ,	RTTI_CALLBLK
			  ,	RTTI_ALTBLK
			  ,	RTTI_VELBLK
			  , RTTI_SCALE			// canvas tools
			  , RTTI_BRLLINE
			  , RTTI_BRLDATA
			  , RTTI_AC_ROUTEPATH	// aircraft tools
		};

	};
};

#endif

/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: writer.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:50 $
      @state    : $State: Exp $

      $Log: writer.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:50  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.2  2006/08/13 10:32:25  seth
      performance data structures built from file implemented

      Revision 1.1  2006/08/09 07:43:46  seth
      performance level data


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __PERFORMANCE_DATA_WRITER_HDR__
#define __PERFORMANCE_DATA_WRITER_HDR__

#include "data.h"

namespace performance {

    //
    // performance::Writer
    //

    class Writer {

    public:
        Writer();
        ~Writer();

    public:
		void write( DataCPtr & ) const;
	};


};
#endif

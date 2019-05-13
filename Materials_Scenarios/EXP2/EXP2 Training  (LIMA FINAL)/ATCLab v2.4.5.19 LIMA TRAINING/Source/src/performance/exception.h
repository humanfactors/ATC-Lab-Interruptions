/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: exception.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:49 $
      @state    : $State: Exp $

      $Log: exception.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:49  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.1  2006/08/07 06:22:46  seth
      implemtation of performance data



      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __PERFORMANCE_EXCEPTION_HDR__
#define __PERFORMANCE_EXCEPTION_HDR__

#include <stdexcept>

namespace performance {

    //
    // performance::error
    //
    struct performance_data_error : public std::runtime_error{
		performance_data_error( const std::string& msg ) 
			:std::runtime_error( msg ) {}
	};

};

#endif

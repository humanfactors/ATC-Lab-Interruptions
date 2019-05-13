/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: atc_types.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:42 $
      @state    : $State: Exp $

      $Log: atc_types.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.2  2006/10/26 05:59:52  seth
      working implementation of experiment time logging

      Revision 1.1  2006/09/16 10:02:55  seth
      implemented controller states plus colours - still bug in vectored aircraft

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __ATC_TYPES_HDR__
#define __ATC_TYPES_HDR__


#include <deque>
#include <sstream>
#include <string>
#include <utility>


namespace atc {

    //
    // type definitions
    //

	typedef std::pair< double, double > Point;

	typedef std::deque< std::pair< std::string, std::string > > KeyValues;
		 
	//
	// type conversions
	//

	template <class T>
	std::string to_string( const T &object ) {
		std::ostringstream os;
		os << object;
		return os.str();
	}



};

#endif

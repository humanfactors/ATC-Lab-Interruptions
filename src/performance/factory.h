/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: factory.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:50 $
      @state    : $State: Exp $

      $Log: factory.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:50  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.3  2006/09/20 01:43:23  seth
      fixed memory leaks

      Revision 1.2  2006/08/15 11:48:46  seth
      aircraft agents initialized with performance data

      Revision 1.1  2006/08/14 06:12:13  seth
      implementing performance helper/wrapper factory


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#ifndef __PERFORMANCE_FACTORY_HDR__
#define __PERFORMANCE_FACTORY_HDR__

#include "data.h"
#include <map>
#include <memory>
#include <utility>

namespace performance {

	//
	// performance::Perfomance
	//
	class Performance;

    //
    // performance::Factory
    //
    class Factory {
        
    public:
		static void initialize( const std::auto_ptr< Data > );
		static void destroy();
		static const Performance * performance( const char * );

    private:
        Factory( const std::auto_ptr< Data > );
        ~Factory();

    private:
		static Factory                        *_instance;
        std::auto_ptr< Data >                  _data;
		std::map< std::string, Performance * > _map;

	};

	//
	// performance::PerformanceMap
	//
	typedef std::map< std::string, Performance * > PerformanceMap;
    
};

#endif

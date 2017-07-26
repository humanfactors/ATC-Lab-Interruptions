/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: area_definition.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:42 $
      @state    : $State: Exp $

      $Log: area_definition.cpp,v $
      Revision 1.1.1.2  2007/06/21 12:22:42  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:11  Aaron Yeung
      no message

      Revision 1.1  2006/09/20 06:44:33  seth
      vector selection box + some bug fix

      Revision 1.1  2006/08/24 04:12:51  seth
      seperating model from visuals


      @email      : atc-support@psy.uq.edu.au
      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology

**/

#include "area_definition.h"


using namespace atc;


////////////////////////////////////////////////////////////////////////////////
//
// atc::AreaDefinition
//
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
AreaDefinition::AreaDefinition()                  : uid(),      descriptors() {}
AreaDefinition::AreaDefinition( const char *uid ) : uid( uid ), descriptors() {}

/*!
 *
 */
AreaDefinition::~AreaDefinition() {
	const_iterator it = begin();
	for ( ; it != end(); ++it )
		delete *it;
}


//------------------------------------------------------------------------------
// [public] interface
//


//------------------------------------------------------------------------------
// [private] implementation
//


////////////////////////////////////////////////////////////////////////////////

/**

      @project  : ATC-Lab [performance data]

      @file     : $RCSfile: writer.cpp,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:50 $
      @state    : $State: Exp $

      $Log: writer.cpp,v $
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

#include "writer.h"
#include "data.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace performance;

////////////////////////////////////////////////////////////////////////////////
//
// performance::Writer
// 
//------------------------------------------------------------------------------
// [public] construction
//

/*!
 *
 */
Writer::Writer() {}

/*!
 *
 */
Writer::~Writer() {}


//------------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
void Writer::write( DataCPtr &data ) const {
	
	std::cout << "****************************" << std::endl;
	std::cout << "*****  Aircraft Types  *****" << std::endl;
	std::cout << "****************************" << std::endl;

	std::map< std::string, PDT >::const_iterator tit;
	for ( tit = data->_types.begin(); tit != data->_types.end(); ++tit ) {
		
		PDT pdt = tit->second;

		std::cout 
			 << "TYPE: " << pdt.type 
			 <<    " : " << pdt.wtc 
			 <<    " : " << pdt.syMSC 
			 <<    " : " << pdt.bnMSC
			 <<    " : " << pdt.classRef
			 << std::endl;

	}

	std::cout << "******************************" << std::endl;
	std::cout << "*****  Performance Data  *****" << std::endl;
	std::cout << "******************************" << std::endl;

	std::map< int, PDO >::const_iterator dit;
	for ( dit = data->_data.begin(); dit != data->_data.end(); ++dit ) {
		
		PDO pdo = dit->second;

		std::cout 
			 << "PERORMANCE: " << pdo.classNo
			 <<          " : " << pdo.is_jet 
			 <<          " : " << pdo.xSection
			 <<          " : " << pdo.levels
			 <<          " : " << pdo.sidStar
			 << std::endl;

		std::vector< double >::const_iterator vit;

		std::cout << " ***** altitude >>>>> : ";
		for ( vit = pdo.altitude->begin() ; vit != pdo.altitude->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;

		std::cout << " ***** climb rate >>>>> : ";
		for ( vit = pdo.climbRate->begin() ; vit != pdo.climbRate->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;

		std::cout << " ***** descemt rate >>>>> : ";
		for ( vit = pdo.descendRate->begin() ; vit != pdo.descendRate->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;

		std::cout << " ***** acceleration >>>>> : ";
		for ( vit = pdo.acceleration->begin() ; vit != pdo.acceleration->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;

		std::cout << " ***** deceleration >>>>> : ";
		for ( vit = pdo.deceleration->begin() ; vit != pdo.deceleration->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;

		std::cout << " ***** cruise speed >>>>> : ";
		for ( vit = pdo.cruiseSpeed->begin() ; vit != pdo.cruiseSpeed->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;

		std::cout << " ***** stall speed >>>>> : ";
		for ( vit = pdo.stallSpeed->begin() ; vit != pdo.stallSpeed->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;

		std::cout << " ***** climb speed >>>>> : ";
		for ( vit = pdo.climbSpeed->begin() ; vit != pdo.climbSpeed->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;

		std::cout << " ***** descendSpeed >>>>> : ";
		for ( vit = pdo.descendSpeed->begin() ; vit != pdo.descendSpeed->end() ; ++vit )
			std::cout << *vit << " : ";
		std::cout << "<<<<<" << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////

/**
      @project  : ATC-Lab [engine]

      @file     : $RCSfile: transformation.h,v $
      @author   : $Author: Aaron Yeung $
      @version  : $Name:  $ ( $Revision: 1.1.1.2 $ )
      @date     : $Date: 2007/06/21 12:22:46 $
      @state    : $State: Exp $

      $Log: transformation.h,v $
      Revision 1.1.1.2  2007/06/21 12:22:46  Aaron Yeung
      no message

      Revision 1.1.1.1  2007/06/09 13:12:12  Aaron Yeung
      no message

      Revision 1.7  2006/09/25 06:06:17  seth
      implemented requirements

      Revision 1.6  2006/09/13 10:16:16  seth
      initial implemenation of vectoring functionality

      Revision 1.5  2006/09/13 05:38:49  seth
      full working implemtation of Range/Bearing tool

      Revision 1.4  2006/09/06 13:09:24  seth
      refactoring translations into canvas

      Revision 1.3  2006/09/06 06:22:23  seth
      transformation problems

      Revision 1.2  2006/07/19 08:39:18  seth
      implementing aircraft agent

      Revision 1.1  2006/06/07 05:50:14  seth
      refactor prior to implementation of performance data lookup


      @copyright  : 2006 ARC Key Center for 
                    Human Factors & Applied Cognitive Psycology
  
**/

#ifndef __ATC_TRANSFORMATION_HDR__
#define __ATC_TRANSFORMATION_HDR__

#define DBL_PAIR \
		std::pair< double, double >

#include <qwmatrix.h>
#include <stdexcept>

namespace atc {

	//
	// atc:transformation_error
	//

	struct transformation_error : public std::runtime_error {
		transformation_error( const std::string& msg ) 
			  : std::runtime_error( msg ) {}
	};


	//
	// atc::Transformation
	//

	class Transformation {

	public:
		Transformation();
		Transformation( const Transformation & );
		~Transformation();

	public:
		const QRect calculate( const QRect &, int );

	public:
		DBL_PAIR point ( double, double ) const;
		QPoint   qpoint( double, double ) const;
		QPoint   qpoint( const QPoint & ) const;
		double   length( double ) const;
		double   height( double ) const;

		QPointArray qpoints( const QPointArray & ) const;

		double   inverse_length( double ) const;
		DBL_PAIR inverse_point ( double, double ) const;
		QPoint   inverse_qpoint( double, double ) const;

	public:
		const Transformation invert() const;

	private:
		void init_matrix( const QRect &, const QRect & );

	private:
		QWMatrix _mtx;

	}; 

	typedef DBL_PAIR TPoint;

};

#undef DBL_PAIR
#endif
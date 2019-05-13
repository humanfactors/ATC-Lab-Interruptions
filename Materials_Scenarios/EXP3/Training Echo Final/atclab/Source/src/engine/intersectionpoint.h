/**
 * @file:       atc_intersectionpoint.h
 *
 *              ATC_Engine project definitions - c++ header.
 *
 *		Utility for calculating the intersection point for two lines.
 *
 * @author:     seth  
 * @email:	atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 06:44:37 PM 14/07/04
 *
 * $Id: intersectionpoint.h,v 1.1.1.2 2007/06/21 12:22:44 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_INTERSECTIONPOINT_HDR__
#define __ATC_INTERSECTIONPOINT_HDR__

#include <stdexcept>

class QPoint;

namespace atc{


	/*!
	 * \struct intersectionpoint_error
	 */
	struct intersectionpoint_error : public std::runtime_error{
		intersectionpoint_error(const std::string& msg) 
			:std::runtime_error(msg)
		{}
	};


	/*!
	 * \class atc::IntersectionPoint atc_intersectionpoint.h
	 */
	class IntersectionPoint  
	{
	public:

		IntersectionPoint(
			const QPoint &, const QPoint &, 
			const QPoint &, const QPoint &
		);
		virtual ~IntersectionPoint();

	public:

		bool coincident()const;
		bool parallel()const;

		QPoint point()const;

	private:

		int _u_denominator;
		int _ua_numerator;
		int _ub_numerator;

		int _x, _y;

	};

};

#endif

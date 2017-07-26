/**
 * ATC-Lab: Presentation Module
 * Translate experiment script units into map units
 *
 * @author:     seth  
 * @version:    $Name:  $
 *              File created 00:00:00 PM 15/12/04
 *
 * $Id: translator.cpp,v 1.1.1.2 2007/06/21 12:22:48 Aaron Yeung Exp $
 *
 * @copyright:  2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */


#include "translator.h"
#include "atc.h"
#include <math.h>
#include <sstream>

using namespace pact;


//==============================================================================
// pact::SimpleAeroNauticTranslator
//
//------------------------------------------------------------------------------
// [public] interface
//

/*!
 */
double SimpleAeroNauticTranslator::operator()(
	double value, VALUE_TYPE unit
)const{
	switch(unit){
	
	case(VVALUE): 
		return value / atc::SIXTY_SQR;					// knts => nm/sec
	
	case(DVVALUE): 
		return value / atc::SIXTY_SQR / atc::SIXTY;		// knts/min => nm/sec/sec

	case(ZVALUE):	
		return value / atc::NM2FT;						// ft => nm

	case(DZVALUE):	
		return value / atc::NM2FT / atc::SIXTY;			// ft/min => nm/sec

	case(XYVALUE):
	case(SVALUE):
		return value;									// nautical miles
	
	default: 
		throw pact::translator_error("illegal value type");
	}
}

//==============================================================================
// pact::AeroNauticTranslator
//
//------------------------------------------------------------------------------
// [public] interface
//

/*!
 */
double AeroNauticTranslator::operator()(
	double value, VALUE_TYPE unit
)const{
	switch(unit){
	case(XYVALUE): return translate_xy(value);
	case(ZVALUE): return translate_z(value);
	case(VVALUE): return translate_v(value);
	case(SVALUE): return translate_s(value);
	default: throw pact::translator_error("illegal value type");
	}
}

//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 * Translate X|Y values 
 *
 * @param dms Value represented as degrees, minutes and seconds
 * @retrun value converted to seconds
 */
double AeroNauticTranslator::translate_xy(double dms)const
{
	double n = fabs(dms);
	double f = modf(n, &n);

	div_t div_result;
	div_result = div( int(n), 100 );
	int s = div_result.rem;
	div_result = div( div_result.quot, 100 );
	int m = div_result.rem;
	int d = div_result.quot;
	
	double retValue = (dms < 0 ? -1 : 1) * (((d * 60) + m) * 60) + s + f;

	if( 
		retValue <= -180 * atc::SIXTY_SQR || 
		retValue >   180 * atc::SIXTY_SQR
	){
		std::ostringstream oss;
		oss << "illegal coordinate <" << dms << ">";
		throw pact::translator_error(oss.str());
	}

	return retValue;
}

/*!
 * Translate Z values
 * 
 * @param ft Value in feet
 * @retrun value left as feet
 */
double AeroNauticTranslator::translate_z(double ft)const{
	return ft;
}

/*!
 * Translate velocity values
 *
 * @param vel Velocity in knots (nautical miles per hour)
 * @return Velocity converted to angular seconds / temporal seconds
 */
double AeroNauticTranslator::translate_v(double vel)const{
	return translate_s(vel) / atc::SIXTY_SQR;
}

/*!
 * Translate distance values.
 *
 * Nautical miles are assumed to be equivalent to angular minutes
 *
 * @param dis Distance in nautical miles
 * @return Distance in angular seconds
 */
double AeroNauticTranslator::translate_s(double dist)const{
	return dist * atc::SIXTY;
}

//==============================================================================

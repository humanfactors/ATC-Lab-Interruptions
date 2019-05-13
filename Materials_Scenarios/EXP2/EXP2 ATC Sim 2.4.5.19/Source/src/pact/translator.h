/**
 * ATC-Lab: Presentation Module
 * Translate experiment script units into map units
 *
 * @author:     seth  
 * @version:    $Name:  $
 *              File created 00:00:00 PM 15/12/04
 *
 * $Id: translator.h,v 1.1.1.2 2007/06/21 12:22:48 Aaron Yeung Exp $
 *
 * @copyright:  2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */


#ifndef __PACT_TRANSLATOR_HDR__
#define __PACT_TRANSLATOR_HDR__


#include <stdexcept>


namespace pact{


	/*!
	 * \struct pact::translator_error
	 */
	struct translator_error : public std::runtime_error{
		translator_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};


	/*!
	 * \class pact::Translator
	 */
	class Translator
	{
	public:

		enum VALUE_TYPE{ XYVALUE, ZVALUE, DZVALUE, VVALUE, DVVALUE, SVALUE };

		virtual ~Translator(){};
		virtual double operator()(double val, VALUE_TYPE unit = XYVALUE)const = 0;

	};	


	/*!
	 *  \class pact::MetricTranslator
	 */
	class MetricTranslator : public Translator
	{
	public:
		
		MetricTranslator(){};
		virtual ~MetricTranslator(){};

	public:

		virtual double operator()(double val, VALUE_TYPE unit)const{
			return val;
		}

	};


	/*!
	 * \class pact::SimpleAeroNauticTranslator
	 */
	class SimpleAeroNauticTranslator : public Translator
	{
	public:
		
		SimpleAeroNauticTranslator(){};
		virtual ~SimpleAeroNauticTranslator(){};

	public:

		virtual double operator()(double val, VALUE_TYPE unit)const;

	};



	/*!
	 * \class pact::AeroNauticTranslator
	 */
	class AeroNauticTranslator : public Translator
	{
	public:

		AeroNauticTranslator(){};
		virtual ~AeroNauticTranslator(){};

	public:

		virtual double operator()(double, VALUE_TYPE unit)const;

	private:

		double translate_xy(double)const;
		double translate_z(double)const;
		double translate_v(double)const;
		double translate_s(double)const;

	};

	
};

#endif

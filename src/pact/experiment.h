/**
 * @file:       atc_experiment.h
 *
 * 		ATC-Lab
 *              
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: experiment.h,v 1.1.1.2 2007/06/21 12:22:46 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_EXPERIMENT_HDR__
#define __ATC_EXPERIMENT_HDR__

#ifdef WIN32
	// disable - decorated name length exceeded
	#pragma warning(disable:4503)
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif

#include "document.h"
#include <stdexcept>
#include <vector>

namespace pact{


	/*!
	 * \struct atc::experiment_error
	 */
	struct experiment_error : public std::runtime_error{
		experiment_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};


	/*!
	 * \class pact::Translator
	 */
	class Translator;


	/*!
	 * \class pact::Experiment atc_experiment.h
	 */
	class Experiment : public xml::Document	{

	public:
		typedef std::vector<xml::DocumentNode*>::const_iterator const_iterator;

	public:
		Experiment();
		~Experiment();

	public: // virtual
		virtual void createChild(const std::string&);
		virtual bool validate();

	public:
		const_iterator begin()const;
		const_iterator end()const;

	private:
		std::vector<xml::DocumentNode*> _presentation;
		Translator* _translator;
			
	};


};

#endif

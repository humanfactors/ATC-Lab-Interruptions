/**
 * @file:       atc_experiment.cpp
 *
 * 		ATC-Lab
 *              Document describing experiement
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: experiment.cpp,v 1.1.1.2 2007/06/21 12:22:46 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */


#include "experiment.h"
#include "translator.h"


#ifdef _WIN32
	#include <assert.h>
#endif

using namespace pact;

//------------------------------------------------------------------------------

/**
 * Default Constructor
 */
Experiment::Experiment()
	  : xml::Document()
	  , _presentation()
	  , _translator(0)
{}

/**
 * Destructor
 */
Experiment::~Experiment() {
	delete _translator;
}

//------------------------------------------------------------------------------
// public virtual
//

/**
 * Create child document node
 * The actual creation is delegated to Document parent class. If the new node
 * is a child of a 'phase' node (ie it's a task) then it is added to the 
 * presentaion queue.
 */
void Experiment::createChild(const std::string& type)
{
	Document::createChild(type); // sets _current to new node

	if(
		_current != _root &&
		_current->parent->type == "phase"
	){
		_presentation.push_back(_current);
	}
}

/**
 *
 */
bool Experiment::validate()
{
	if ( Document::validate() ) {

		std::string units = getStringContent(
			_root,
			"config units input"
		);

		if("+DDDMMSS" == units){
			_translator = new pact::AeroNauticTranslator;

		}else if("NM-FT" == units){
			_translator = new pact::SimpleAeroNauticTranslator;

		}else if("STANDARD" == units){
			_translator = new pact::MetricTranslator;

		}else{
			throw pact::experiment_error(
				"illegal unit schema <" + units + ">"
			);
		}
		return true;
	}

	return false;
}

/*!
 * 
 */
/*
double pact::Experiment::getDecimalContent(
	const xml::DocumentNode* node, 
	const std::string& path
)const{

	double val = Document::getDecimalContent(node, path);

	std::string p(path);
	if(path.empty()){
		p = node->type;
	}

	if(
		p.find("xcoord") != std::string::npos ||
		p.find("ycoord") != std::string::npos
	){
		return _translator->operator()(val, pact::Translator::XYVALUE);

	}else if(p.find("altitude") != std::string::npos){
		return _translator->operator()(val, pact::Translator::ZVALUE);

	}else if(p.find("ascent") != std::string::npos){
		return _translator->operator()(val, pact::Translator::DZVALUE);
	
	}else if(p.find("velocity") != std::string::npos){
		return _translator->operator()(val, pact::Translator::VVALUE);

	}else if(p.find("acceleration") != std::string::npos){
		return _translator->operator()(val, pact::Translator::DVVALUE);

	}else if(p.find("radius") != std::string::npos){
		return _translator->operator()(val, pact::Translator::SVALUE);

	}else{
		return val;
	}
}
*/

//------------------------------------------------------------------------------
// [public] iteration interface
//

/**
 * Get pointer to start of experiment
 */
Experiment::const_iterator Experiment::begin()const{
	return _presentation.begin();
}

/**
 * Get pointer to end of experiment
 */
Experiment::const_iterator Experiment::end()const{
	return _presentation.end();
}

//------------------------------------------------------------------------------

/**
 * @file:       atc_engine_factory.cpp
 *
 *              ATC_Engine project implementations - c++ methods .
 *              Factory methods for creation of atc::Engine* from Library .
 *
 * @author:     seth  
 * @email:	atc_support@humanfactors.uq.edu.au
 * 
 * @version:    $Name:  $
 *              File created 12:10:23 PM 03/04/04
 *
 * $Id: engine_factory.cpp,v 1.1.1.2 2007/06/21 12:22:43 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "engine_impl.h"

//------------------------------------------------------------------------------

extern "C" ATC_API atc::Engine* create_engine(){
	return new atc::EngineImpl;
}

extern "C" ATC_API void destroy_engine(atc::Engine* e){
	delete e;
}

//------------------------------------------------------------------------------

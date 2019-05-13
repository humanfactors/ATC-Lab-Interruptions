/**
 * @file:       xmlreader.cpp
 *
 *              XML Utilities
 *              Wrapper for Xerces-c SAX2 XML Reader.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: xmlreader.cpp,v 1.1.1.2 2007/06/21 12:22:49 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include "xmlhandler.h"
#include "xmlreader.h"
#include "xmlstring.h"

#include <memory>
#include <sstream>

//------------------------------------------------------------------------------

/*!
 */
xml::XMLReader::XMLReader()
	:_sax2parser(0)
{
	try{
		xercesc::XMLPlatformUtils::Initialize();
	}catch(const xercesc::XMLException& ex){
		std::ostringstream oss;
		oss << "initialization failed : " << ex.getMessage();
		throw xmlreader_error(oss.str());
	}

	_sax2parser = xercesc::XMLReaderFactory::createXMLReader();
		
	_sax2parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
	_sax2parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);
	_sax2parser->setFeature(xercesc::XMLUni::fgXercesSchema, true);
	_sax2parser->setFeature(
		xercesc::XMLUni::fgXercesSchemaFullChecking, true
	);

}

xml::XMLReader::~XMLReader()
{
	if(_sax2parser) delete _sax2parser;
	xercesc::XMLPlatformUtils::Terminate();
}
 
//------------------------------------------------------------------------------

/*!
 * Set the location for Schema Definition file
 */
//void XMLReader::setSchemaLocation(const char *path)
//{
//
//	parser->setProperty(
//		xercesc::XMLUni::fgXercesSchemaExternalSchemaLocation,
//		(void*)(std2xml_string(path).c_str())
//	);
//}

/*!
 * Set XML Handler
 */
void xml::XMLReader::setSAX2Handler(XMLHandler *handler)
{
	_sax2parser->setContentHandler(handler);
	_sax2parser->setErrorHandler(handler);
}

//------------------------------------------------------------------------------

/*!
 * Parse the XML file
 *
 * Generally exceptions should be caught and reported by the handler.
 */
void xml::XMLReader::parse(const char *xmlfile)
{
	try {
		_sax2parser->parse(xmlfile);

	}catch(const xercesc::XMLException &ex){
		std::ostringstream oss;
		oss
			<< "XML exception in " << ex.getSrcFile()
			<< " at line " << ex.getSrcLine()
			<< " : " << ex.getMessage();
		throw xmlreader_error(oss.str());

	}catch(const std::runtime_error &ex){
		std::ostringstream oss;
		oss << "RunTime error : " << ex.what();
		throw xmlreader_error(oss.str());

	}catch(...){
		std::ostringstream oss;
		oss << "unexpected error during parsing";
		throw xmlreader_error(oss.str());
	}
}

//------------------------------------------------------------------------------


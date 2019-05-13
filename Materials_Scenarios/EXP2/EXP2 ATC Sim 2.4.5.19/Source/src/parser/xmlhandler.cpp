/**
 * @file:       xmlhandler.cpp
 *
 * 		XML Utilities
 *              SAX2 xercesc::DefaultHandler extension.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: xmlhandler.cpp,v 1.1.1.2 2007/06/21 12:22:48 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "document.h"
#include "logger.h"
#include "xmlhandler.h"
#include "xmlstring.h"

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#ifdef WIN32
	#include <assert.h>
#endif

#include <sstream>


//------------------------------------------------------------------------------

/*!
 */
xml::XMLHandler::XMLHandler(xml::Document &doc, logging::Logger *log)
	:DefaultHandler()
	,_document(doc)
	,_logger(log)
	,_buffer("")
{}

//------------------------------------------------------------------------------

/*!
 */
xml::XMLHandler::~XMLHandler()
{}
		
//------------------------------------------------------------------------------

//
// SAX DocumentHandler interface
//

/*!
 * Processes XML start(opening) element tags.
 */
void xml::XMLHandler::startElement(
	const   XMLCh* const    uri,
	const   XMLCh* const    localname,
	const   XMLCh* const    qname,
	const   xercesc::Attributes&     attrs
){
	_document.createChild(xml2std_string(localname));

	for(int i = 0; i < attrs.getLength(); ++i){
		_document.addAttribute(
			xml2std_string(attrs.getLocalName(i)),
			xml2std_string(attrs.getValue(i))
		);
		if("ID" == xml2std_string(attrs.getType(i))){
			_document.setReference(
				xml2std_string(attrs.getValue(i))
			);
		}else if("IDREF" == xml2std_string(attrs.getType(i))){
			_document.setIDRef(
				xml2std_string(attrs.getValue(i))
			);
		}
	}
}

/*!
 * Processing of XML end (closing) element tags.
 */
void xml::XMLHandler::endElement(
	const   XMLCh* const    uri,
	const   XMLCh* const    localname,
	const   XMLCh* const    qname
){
	_document.setContent(_buffer);
	_document.ascendParent();
	_buffer = "";
}

/*!
 * Collects the content of XML tag.
 *
 * Each block of tag content data is appended to the end of the character 
 * buffer. The data will be processed once the closing tag is reached.
 *
 * \sa endElement()
 */
void xml::XMLHandler::characters(
	const XMLCh* const chars,
	const unsigned int length
){
	_buffer += xml2std_string(chars);
}

//------------------------------------------------------------------------------

//
// SAX ErrorHandler Interface
//

void xml::XMLHandler::warning(const xercesc::SAXParseException &ex){
	_logger->warning(error_text(ex).c_str());
	_document.setWarning();
}

void xml::XMLHandler::error(const xercesc::SAXParseException &ex){
	_logger->error(error_text(ex).c_str());
	_document.setError();
}

void xml::XMLHandler::fatalError(const xercesc::SAXParseException &ex){
	_logger->fatal(error_text(ex).c_str());
	_document.setFatal();
	throw xmlhandler_error(error_text(ex));
}

//------------------------------------------------------------------------------

//
// private implementation
//

/*!
 * Generate error message text for a SAX parsing exception
 */
std::string xml::XMLHandler::error_text(const xercesc::SAXParseException &ex)
{
	std::ostringstream oss;
	oss 
		<< "SAX parsing exception [" 
		<< xml2std_string(ex.getSystemId())
		<< "] (line=" 
		<< ex.getLineNumber() 
		<< " , char="
		<< ex.getColumnNumber() 
		<< ") " 
		<< xml2std_string(ex.getMessage());
	return oss.str();
}

//------------------------------------------------------------------------------

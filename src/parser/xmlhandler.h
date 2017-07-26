/**
 * @file:       xmlhandler.h
 *
 * 		XML Utilities
 *              SAX2 xercesc::DefaultHandler extension.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: xmlhandler.h,v 1.1.1.2 2007/06/21 12:22:48 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __XMLHANDLER_HDR__
#define __XMLHANDLER_HDR__

#ifdef WIN32
	// disable 'identifier' truncation warnings
	#pragma warning(disable:4786)
#endif

#include <xercesc/sax2/DefaultHandler.hpp>
#include <stdexcept>
#include <string>

namespace logging{

	/*!
	 */
	class Logger;

};

namespace xml{

	/*!
	 */
	class Document;


	/*!
	 * \struct atc::xmlhandler_error
	 */
	struct xmlhandler_error : public std::runtime_error{
		xmlhandler_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};


	/*!
	 * \class atc::XMLHandler atc_xmlhandler.h
	 */
	class XMLHandler : public xercesc::DefaultHandler
	{
	public:
	       
		XMLHandler(xml::Document&, logging::Logger*);
		virtual ~XMLHandler();

		//
		// SAX DocumentHandler interface
		//
	
		void startElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname,
			const   xercesc::Attributes&     attrs
		);

		void endElement(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname
		);

		void characters(
			const XMLCh* const chars,
			const unsigned int length
		);

		//
		// SAX ErrorHandler Interface
		//

		void warning(const xercesc::SAXParseException &);
		void error(const xercesc::SAXParseException &);
		void fatalError(const xercesc::SAXParseException &);

	private:

		std::string error_text(const xercesc::SAXParseException &);


	private:

		xml::Document &_document;
		logging::Logger* _logger;

		std::string _buffer;


	};
	
}; 

#endif 

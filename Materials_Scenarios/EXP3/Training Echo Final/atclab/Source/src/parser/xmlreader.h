/**
 * @file:       xmlreader.h
 *
 *              XML Utilities
 *              Wrapper for Xerces-c SAX2 XML Reader.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: xmlreader.h,v 1.1.1.2 2007/06/21 12:22:49 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __XMLREADER_HDR__
#define __XMLREADER_HDR__

#include <stdexcept>

namespace xercesc{

	/*!
	 */
	class SAX2XMLReader;

}

namespace xml{

	/*!
	 * \class XMLHandler
	 */
	class XMLHandler;


	/*!
	 * \struct xmlreader_error
	 */
	struct xmlreader_error : public std::runtime_error{
		xmlreader_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};

	
	/*!
	 * \class XMLReader xmlreader.h
	 */
	class XMLReader 
	{
	public:

		XMLReader();
		~XMLReader();

	public:

		void setSAX2Handler(XMLHandler *);
		//void setSchemaLocation(const char *);

		void parse(const char *);
		
	private:

		xercesc::SAX2XMLReader *_sax2parser;

	};
	
}; 

#endif 

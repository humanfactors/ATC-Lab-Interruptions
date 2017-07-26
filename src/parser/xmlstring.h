/**
 * @file:       xmlstring.h
 *
 *              XML Utilities
 *              Unicode String implementation for use with xceres SAX classes.
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/05/04
 *
 * $Id: xmlstring.h,v 1.1.1.2 2007/06/21 12:22:49 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __XMLSTRING_HDR__ 
#define __XMLSTRING_HDR__ 

#include <string>

#ifndef WIN32
namespace std{

	template<> 
	struct char_traits<XMLCh>
	{
		typedef XMLCh char_type;

		static void 
		assign(char_type& __c1, const char_type& __c2)
		{ __c1 = __c2; }

		static size_t
		length(const char_type* __s)
		{
			const char_type* __p = __s;
			while(*__p) ++__p;
			return(__p - __s);
		}

		static char_type*
		copy(char_type* __s1, const char_type* __s2, size_t __n)
		{
			return (char_type*)
				memcpy(__s1, __s2, __n * sizeof(char_type)); 
		}
		
	};

} 
#endif

namespace xml{
	
	typedef std::basic_string<XMLCh> xmlstring;
	
	std::string xml2std_string(const XMLCh* const);
	std::string xml2std_string(const xmlstring&);

	xmlstring std2xml_string(const char* const);
	xmlstring std2xml_string(const std::string&);


	inline std::string xml2std_string(const xmlstring& sxml){
		return xml2std_string(sxml.c_str());
	}
	
	inline std::string xml2std_string(const XMLCh* const sxml)
	{
		if(!sxml)return std::string();

		char* sptr = xercesc::XMLString::transcode(sxml);
		try{
			std::string rv(sptr);
			xercesc::XMLString::release(&sptr);
			return rv;
		}catch(...){
			xercesc::XMLString::release(&sptr);
			throw;
		}
	}
	
	inline xmlstring std2xml_string(const std::string& sstd){
		return std2xml_string(sstd.c_str());
	}
	
	inline xmlstring std2xml_string(const char* const sstd)
	{
		XMLCh* sptr = xercesc::XMLString::transcode(sstd);
		try{
			xmlstring rv(sptr);
			xercesc::XMLString::release(&sptr);
			return rv;
		}catch(...){
			xercesc::XMLString::release(&sptr);
			throw;
		}
	}
	
}

#endif 

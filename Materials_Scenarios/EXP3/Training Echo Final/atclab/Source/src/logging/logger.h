/**
 * @file:       logger.h
 *
 *              Logging Utilities
 *              Common Loggging Interface
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/08/04
 *
 * $Id: logger.h,v 1.2 2009/05/24 02:13:00 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __LOGGER_HDR__
#define __LOGGER_HDR__


#ifdef WIN32
	// disable - decorated name length exceeded
	#pragma warning(disable:4503)
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif

#include <sstream>
#include <stdexcept>
#include <vector>
#include <qfile.h>

namespace logging{


	/*!
	 * \struct logger_error
	 */
	struct logger_error : public std::runtime_error{
		logger_error(const std::string& msg) 
			:std::runtime_error(msg){}
	};

	/*!
	 *
	 */
	enum LogLevel { DATA, INFO, WARN, LOG_ERROR, FATAL };
	
	/*!
	 *
	 */
	typedef std::pair< std::string, std::string > KeyValue;
	typedef std::vector< KeyValue > KVPairs;
	typedef KVPairs::const_iterator KVPairsCIt;


	/*!
	 * \class Logger logger.h
	 */
	class Logger
	{
	public:

		static void set_name( const std::string & );

		static Logger* instance();
		static void destroy();

	private:

		Logger();
		Logger( const std::string & );
		~Logger();

	public:
		void open_record( LogLevel );
		void add_keyvalue( const std::string &, const std::string & );
		void add_keyvalue( const std::string &, const double &      );
		void close_record();

		void data( const char *, double );

		void info( const char * )const;
		void warning( const char * )const;
		void error( const char * )const;
		void fatal( const char * )const;

		void info( const KVPairs& )const;
		void warning( const KVPairs& )const;
		void error( const KVPairs& )const;
		void fatal( const KVPairs& )const;
		


	private:
		void init();

		std::string compose( const KVPairs& )const;

		void write( const char *, const char * )const;
		void write( const char * )const;
		
		const char * level2string( LogLevel );

	private:

		static Logger* _instance;

		mutable QFile _file;

		LogLevel              _level;
		std::string        _ostring;
		std::ostringstream _osstream;

	};

	
}; 

#endif 

/**
 * @file:       logger.cpp
 *
 *              Logging Utilities
 *              Common Loggging Interface
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/08/04
 *
 * $Id: logger.cpp,v 1.2 2009/05/24 02:13:00 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#include "logger.h"
#include <qdatetime.h>
#include <qstring.h>
#include <qtextstream.h>
#include <sstream>

using namespace logging;

////////////////////////////////////////////////////////////////////////////////
//
//
//
//------------------------------------------------------------------------------
// static
//

/*!
 * Logger instance, initially null.
 */
Logger* Logger::_instance = 0;

/*!
 *
 */
void Logger::set_name( const std::string &fn ) {
	if ( !_instance ) _instance = new Logger( fn );
}

/*!
 * Get the instance of the logger, creating if necessary.
 */
Logger* Logger::instance() {
	if(!_instance) _instance = new Logger;
	return _instance;
}

/*!
 * Destroy the instance of the logger, if exists.
 */
void Logger::destroy()
{
	if(_instance){
		delete(_instance);
		_instance = 0;
	}
}

//------------------------------------------------------------------------------
// Construction/Destruction
//

/*!
 * Protected default contructor
 */
Logger::Logger()
	: _file( "atc-lab.log" ) 
	, _level()
	, _ostring()
	, _osstream( _ostring )
{
	init();
}

/*!
 *
 */
Logger::Logger( const std::string &fn )
	: _file( fn )
	, _level()
	, _ostring()
	, _osstream( _ostring )

{
	init();
}

void Logger::init() {
	if ( !_file.open( IO_WriteOnly | IO_Append ) )
		throw logger_error( "unable to open log file" );

	info("<log>start</log>");
}

/*!
 * Protected Destructor
 */
Logger::~Logger()
{
	info("<log>end</log>");
	_file.close();
}

//------------------------------------------------------------------------------
// [public] interface
//

/*!
 *
 */
void Logger::info(const char *msg)const{
	write("info", msg);
}

/*!
 *
 */
void Logger::warning(const char *msg)const{
	write("warning", msg);
}

/*!
 *
 */
void Logger::error(const char *msg)const{
	write("error", msg);
}

/*!
 *
 */
void Logger::fatal(const char *msg)const{
	write("fatal", msg);
}

/*!
 *
 */
void Logger::info(const KVPairs& kvs)const{
	info(compose(kvs).c_str());
}

/*!
 *
 */
void Logger::warning(const KVPairs& kvs)const{
	warning(compose(kvs).c_str());
}

/*!
 *
 */
void Logger::error(const KVPairs& kvs)const{
	error(compose(kvs).c_str());
}

/*!
 *
 */
void Logger::fatal(const KVPairs& kvs)const{
	fatal(compose(kvs).c_str());
}
		

//------------------------------------------------------------------------------
// [private] implementation
//

/*!
 *
 */
std::string Logger::compose(const KVPairs& kvpairs)const
{
	std::ostringstream oss;
	KVPairsCIt kv = kvpairs.begin();
	for( ; kv != kvpairs.end(); ++kv){
		oss 
			<< "<" << kv->first << ">"
			<< kv->second
			<< "</" << kv->first << ">";
	}
	return oss.str();
}

/*!
 *
 */
void Logger::write(const char *tag, const char * msg)const{
	write(
		QString("<time>%1</time><%2>%3</%4>")
			.arg(QDateTime::currentDateTime().toString())
			.arg(tag)
			.arg(msg)
			.arg(tag)
			.latin1()
	);
}

/*!
 *
 */
void Logger::write(const char *msg)const
{
	//qDebug("[LOG] %s", msg);
	QTextStream os(&_file);
	os << msg << "\n";

}


//------------------------------------------------------------------------------
//
//

/*!
 *
 */
void Logger::open_record( LogLevel level ) {
	_level = level;
	_osstream 
		<< "<time>" 
		<< QDateTime::currentDateTime().toString().latin1() 
		<< "</time>"
		<< "<" << level2string( _level ) << ">";
}

/*!
 *
 */
void Logger::add_keyvalue( const std::string &key, const std::string &val ) {
	_osstream << "<" << key << ">" << val << "</" << key << ">";
}

/*!
 *
 */
void Logger::add_keyvalue( const std::string &key, const double &val ) {
	_osstream << "<" << key << ">" << val << "</" << key << ">";
}

/*!
 *
 */
void Logger::close_record() {
	_osstream << "</" << level2string( _level ) << ">" << std::ends;
	write( _osstream.str().c_str() );
	_osstream.str( "" );
}

/*!
 *
 */
void Logger::data( const char *key, double val ) {
	open_record( DATA );
	add_keyvalue( key, val );
	close_record();
}

//------------------------------------------------------------------------------
//
//

/*!
 *
 */
const char * Logger::level2string( LogLevel lvl ) {
	return 
		    logging::DATA  == lvl ? "data"
		  :	logging::INFO  == lvl ? "info"
		  : logging::WARN  == lvl ? "warn"
		  : logging::LOG_ERROR == lvl ? "errror"
		  : /* logging::FATAL */    "fatal"
	;
}

////////////////////////////////////////////////////////////////////////////////
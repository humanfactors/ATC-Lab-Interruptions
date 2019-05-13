/**
 * @file:       pact.cpp
 *
 * 		ATC-Lab
 * 		Presentaiton Module
 *
 * @author:     seth  
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 10/06/04
 *
 * $Id: pact.cpp,v 1.5.2.1 2015/03/23 13:41:47 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifdef WIN32
	#pragma warning(disable:4786)
#endif

//#include "vld.h"	// visual leak detector

#include "logging/logger.h"
#include "logging/ExcelLogger.h"
#include "parser/xmlhandler.h"
#include "parser/xmlreader.h"
#include "pact/experiment.h"
#include "pact/presenter.h"
#include "performance/builder.h"
#include "performance/exception.h"
#include "performance/factory.h"
#include "performance/parser.h"
#include "version.h"

#include <fstream>

#include <qapplication.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qdir.h>

int critical( const QString & );

int main( int argc, char **argv ) {

	//
	// create application instance
	//

	QApplication app( argc, argv );
	if ( 1 == app.argc() ) {
		QFileInfo fi( *argv );

		QString version = QString("ATCLab Version: %1").arg(ATC_VERSION);
		QString usage = QString("usage: %1 <experiment_script>").arg(fi.fileName());
		QString displayText = version + "\n\n" + usage;

		return critical(displayText);
	}

	QFile script( app.argv()[ app.argc() - 1 ] );
	if ( !script.exists() ) {
		return critical(
			QString( "cannot open experiment script '%1'" ).arg( script.name() )
		);
	}

	//
	// initialize logging
	// 
	QString logfile = QString( "%1.log" ).arg( script.name() );
	logging::Logger::set_name( logfile );
	
	QString excel_log_file = QString("%1.csv").arg(script.name());
	ExcelLogging::ExcelLogger::set_name(excel_log_file);
	
	//
	// load performance data
	//

	QFileInfo performance_file( QDir(app.applicationDirPath()), "acperf.txt" );
	//const char *performance_file = "acperf.txt";

	std::ifstream ifs( performance_file.absFilePath().local8Bit() );

	if ( ! ifs ) {
		return critical( QString
			( "cannot open performace data [%1]" ).arg( performance_file.absFilePath() ) 
		);
	}

	performance::Builder performance_builder;
	performance::Parser performance_parser( ifs, performance_builder );

	try {
		performance_parser.parse();
	}
	catch ( performance::performance_data_error &ex ) {
		return critical( QString
			( "performance parsing errors: %1" ).arg( ex.what() )
		);
	}

	performance::Factory::initialize( performance_builder.data() );

	//
	// load experiment script
	//

	pact::Experiment exp;
	xml::XMLHandler handler( exp, logging::Logger::instance() );
	xml::XMLReader experiment_parser;
	experiment_parser.setSAX2Handler( &handler );

	try	{
		experiment_parser.parse( app.argv()[ app.argc() - 1 ] );

		if ( ! exp.validate() ) {
			return critical(QString(
				"invalid experiment script (see log for details)"
			));
		}
	}
	catch( std::runtime_error& ex ) {
		return critical( ex.what() );
	}

	//
	// run experiment
	//
	int retValue = 0;

	try {
		pact::Presenter presenter;
		app.setMainWidget( &presenter );
		presenter.setExperiment( &exp );
		presenter.showFullScreen();
		presenter.start();

		retValue = app.exec();
	}
	catch( std::runtime_error& ex ) {
		return critical( ex.what() );
	}

	//
	// clean up
	//
    ExcelLogging::ExcelLogger::destroy();
	performance::Factory::destroy();

	logging::Logger::destroy();

	return retValue;
}

int critical( const QString &msg ) {
	return QMessageBox::critical(
			0
		  ,	"ATC-Lab Presentation Module"
		  ,	msg
		  ,	QMessageBox::Ok | QMessageBox::Default | QMessageBox::Escape
		  ,	QMessageBox::NoButton
		  ,	QMessageBox::NoButton
	);
}

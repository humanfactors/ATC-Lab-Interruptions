/**
 * @file:       atc.cpp
 *
 *              ATC-Lab: Engine component
 *              Standard header.
 *
 * @author:     seth  
 * @email:      atc-support@humanfactors.uq.edu.au
 *
 * @version:    $Name:  $
 *              File created 00:00:00 PM 28/10/2004
 *
 * $Id: atc.h,v 1.17.2.2 2015/03/22 13:55:52 Aaron Yeung Exp $
 *
 *              Copyright 2004 ARC Key Centre for 
 *              Human Factors & Applied Cognitive Psychology
 */

#ifndef __ATC_HDR__
#define __ATC_HDR__


#include <limits>
#include <qbrush.h>
#include <qcolor.h>
#include <qfont.h>
#include <qpen.h>


namespace atc{

	const int INVALID_VALUE = -1;
    const int MILLISECONDS_PER_SECOND = 1000;
	/*!
	 *
	 */
	const QString EMPTY_STRING;

	/*!
	 */
	const double NAN = std::numeric_limits<double>::quiet_NaN();

	/*!
	 */
	const double DEFAULT = NAN;

	/*!
	 */
	const float     PI = 3.14159265358979323846f;
	const float TWO_PI = 2 * PI;

	/*!
	 * Conversion Nautical Miles to Feet (nm * NM2FT = ft)
	 */
	const double NM2FT = 6076.11548556f;
	const double FT2NM = 1 / NM2FT;

	/*!
	 *
	 */
	const   int ZERO         =    0;
	const   int TEN          =   10;
	const   int SIXTY        =   60;
	const   int NINETY       =   90;
	const   int ONE_EIGHTY   =  180;
	const   int THREE_SIXTY  =  360;
	const   int ONE_THOUSAND = 1000;

	const   int SIXTY_SQR    = 3600;

	const float ZERO_PT_FIVE =    0.5;

	const int ONE_MINUTE      =  60000; // milliseconds
	const int TWO_MINUTES     = 120000; // milliseconds
	const int FIVE_MINUTES	  = 300000;
	const int TEN_MINUTES	  = 600000;
	const int FIFTEEN_MINUTES = 900000; // milliseconds
	const int TWENTY_MINUTES  = 1200000;

	const QColor ORANGE = QColor( 255, 128, 0 );

	/*!
	 *
	 */
	const int AC_ACCEPT_KEY_DEFAULT				= Qt::Key_A;
	const int AC_ACCEPT_KEY_COORD_REQ_DEFAULT	= Qt::Key_P;
	const int AC_HANDOFF_KEY					= Qt::Key_H;
	const int AC_HISTORY_KEY					= Qt::Key_H;
	const int AC_SRPROBE_KEY					= Qt::Key_P;
	const int AC_ROUTE_KEY						= Qt::Key_R;
	const int AC_VECTOR_KEY						= Qt::Key_V;
	const int AC_REQUIREMENT_KEY				= Qt::Key_L;
	const int AC_BRLINE_KEY						= Qt::Key_B;

	const int ATC_PAUSE_KEY				= Qt::Key_S;
	const int ATC_RESUME_KEY			= Qt::Key_A;

	/*!
	 *
	 */
	const int SNAP_DIMENSION = 5;

	/*!
	 *
	 */
	const int CLOCK_TICK_PERIOD			= 1000;
	const int CLOCK_TICK_XFACTOR		=    1;	// 10sec advance every 5seconds

	/*!
	 *
	 */
	const QString LISTBOX_FONT_FAMILY = "Courier";
	const int     LISTBOX_FONT_POINT  = 10;
	const int     LISTBOX_ITEMS_SHOWN = 11;

	/*!
	 *
	 */
	const int HEADING_FIELD_WIDTH = 3;

	/*!
	 *
	 */
	const int MIN_CFL         =     0;	
	const int MAX_CFL         = 50000; 
	const int CFL_INCREMENT   =  1000;
	const int CFL_FIELD_WIDTH =     5;

	
	/*!
	 *
	 */
	const int MIN_VELOCITY         =  50;	
	const int MAX_VELOCITY         = 550; 
	const int VELOCITY_INCREMENT   =  10;
	const int VELOCITY_FIELD_WIDTH =   3;


	/*!
	 */
	const QColor ACTIVE_SECTOR_PAINT   = Qt::white;
	const QColor INACTIVE_SECTOR_PAINT = Qt::lightGray;
	const QColor ROUTE_PATH_PAINT      = Qt::black;
	const QColor WEATHER_PAINT         = Qt::darkYellow;

	/*!
	 *
	 */
	const QColor BRL_TOOL_PAINT        = Qt::green;

	/*!
	 *
	 */
	const QPen VECTOR_TOOL_PEN        = QPen( Qt::green, 1 );
	const int  VECTOR_TOOL_MIN_LENGTH = 100;

	/*!
	 * Aircraft Requirements Tool
	 */
	const QColor REQTOOL_SPRITE_COLOUR   = Qt::green;
	const int    REQTOOL_SPRITE_DIAMETER = 30;
	const int    REQTOOL_SPRITE_LENGTH   = 80;
	const double REQTOOL_SPRITE_LAYER    = 100;

	/*!
	 * Scale
	 */
	const QPen SCALE_PEN        = QPen( Qt::yellow );
	const double  SCALE_X_EXT		= 2.5;	// Start X
	const double  SCALE_Y_EXT		= 25;	// Start Y
	const double  SCALE_INTERVAL	= 2.5;	// NM to each side of the scale
	const double  SCALE_TICK		= 0;
	const double  SCALE_X_POS		= 1;
	const double  SCALE_Y_POS		= 0;

	/*!
	 * Sector
	 */
	const QPen   SECTOR_PEN            = QPen  ( Qt::darkGray );
	const QBrush SECTOR_ACTIVE_BRUSH   = QBrush( Qt::lightGray );
	const QBrush SECTOR_BRUSH          = QBrush( Qt::gray );
	const double SECTOR_ACTIVE_AREA_Z  = -106;
	const double SECTOR_AREA_Z         = -105;
	const double SECTOR_LINE_Z         =   -9;

	/*!
	 */
	const QBrush WEATHER_BRUSH = QBrush( Qt::darkYellow );
	const double WEATHER_Z     = -100;

	/*!
	 *
	 */
	const QBrush WAYPOINT_SPRITE_BRUSH    = QBrush( Qt::black );
	const QColor WAYPOINT_SPRITE_COLOR    = Qt::blue;
	const double WAYPOINT_SPRITE_DIAMETER =  20;
	const double WAYPOINT_SPRITE_LAYER    =  -4;
	const QFont  WAYPOINT_SPRITE_FONT     
		  = QFont( "Courier New", 20, QFont::Normal );

	/*!
	 *
	 */
	const int CANVAS_SIZE_HINT = 1000; // @todo: check for overflow QCanvasEllipse::areaPoints()

	/*!
	 * Dimensions of Aircraft Sprite (Canvas Units)
	 */
	const QBrush AIRCRAFT_SPRITE_DEFAULT_BRUSH = Qt::NoBrush;
	const QBrush AIRCRAFT_SPRITE_SELECTED_BRUSH = Qt::SolidPattern;
	const double AIRCRAFT_SPRITE_DIAMETER      =  30;
	const double AIRCRAFT_SPRITE_CALLOUT       = 100;
	const double AIRCRAFT_SPRITE_PROBE         = 100;
	const double AIRCRAFT_LOCATOR_LAYER        = 105;
	const double AIRCRAFT_SPRITE_LAYER         = 100;
	const double AIRCRAFT_TOOL_LAYER           =  95;
	const QFont  AIRCRAFT_SPRITE_FONT          
		  = QFont( "Courier New", 25, QFont::Normal );

	const int    ACSPRITE_BLINK_PERIOD					= 250;	// milliseconds
	const QColor ACSPRITE_BLINK_OFF_COLOUR				= Qt::lightGray;
	const QColor ACSPRITE_PRE_HANDOFF_BLINK_OFF_COLOUR	= Qt::darkGray;
	/*!
	 *
	 */
	const QColor CS_NONE_COLOUR				= Qt::black;
	const QColor CS_ANNOUNCED_COLOUR		= Qt::blue;
	const QColor CS_PROPOSED_COLOUR			= ORANGE;			// blinking
	const QColor CS_ACCEPTED_COLOUR			= Qt::darkGreen;
	const QColor CS_OVEROUT_COLOUR			= Qt::darkGreen;
	const QColor CS_PRE_HANDOFF_COLOUR		= Qt::blue;
	const QColor CS_HANDOFF_COLOUR			= Qt::white;
	const QColor CS_NOMORE_COLOUR			= Qt::black;
	const QColor AIRCRAFT_CONFLICT_COLOR	= Qt::yellow;

	const int    CS_OVEROUT_PERIOD   = 30000; // milliseconds
	const int    CS_HANDOFF_PERIOD   = 30000; // milliseconds

	/*!
	 *
	 */
	const int    HISTORY_DOT_COUNT    =  11;
	const int    HISTORY_DOT_DIAMETER =	 10;
	const QColor HISTORY_DOT_BRUSH    = Qt::white;
	const long   HISTORY_DOT_PERIOD   = 60000;

	/*!
	 * Aircraft Short Route / Velocity Probe Tool
	 */
	const int SHORT_ROUTE_PROBE_SIZE   =   2;
	const int SHORT_ROUTE_PROBE_SECS[] = { 0, 10, 20, 35, 60, 120, 240 };

	const int AIRCRAFT_DEFAULT_PROBE_LEVEL = 1;

	/*!
	 *
	 */
	const QBrush ROUTE_TOOL_BRUSH  = Qt::NoBrush;
	const QColor ROUTE_TOOL_COLOUR = Qt::yellow;

	/*!
	 *
	 */
	const QFont  CANVAS_FONT          
		  = QFont( "Courier New", 50, QFont::Bold );

	const int DEFAULT_SECTOR_ENTRY_DIST = 6;
	const int DEFAULT_SECTOR_EXIT_DIST = 2;
};

#endif

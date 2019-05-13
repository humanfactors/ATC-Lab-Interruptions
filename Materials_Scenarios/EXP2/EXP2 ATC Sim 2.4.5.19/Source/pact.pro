#### +--
##
##  ATC-Lab Experiment Presentation Project File (qmake)
##
##  @author:    seth  
##  @email:     atc_support@humanfactors.uq.edu.au
##
##  @version:   $Name:  $ 
## 
##  @copyright: 2006 ARC Key Centre for Human Factors
##              and Applied Cognitive Psychology
##
##  $Id: pact.pro,v 1.1.1.2.6.1 2015/02/11 15:36:43 Aaron Yeung Exp $
##
#### +--

LANGUAGE = C++

## QT += qt3support 

CONFIG += qt 
CONFIG += exceptions 
CONFIG += rtti 
CONFIG += stl 
CONFIG += warn_on 
CONFIG += debug

win32{
    message( "Generating Visual C++ Project file ..." )
    TEMPLATE = vcapp
}

win32:debug{
    INCLUDEPATH += $(XERCESCROOT)/include
    LIBS        += -lxerces-c_2d -L$(XERCESCROOT)/lib
}

win32:debug{
    message("DEBUG intermediate directories")
    DESTDIR     = build/pact/debug
    UI_DIR      = build/pact/debug/uic
    MOC_DIR     = build/pact/debug/moc
    OBJECTS_DIR = build/pact/debug/obj
}

##
## --- ATC-Lab Src ---
##

INCLUDEPATH += src

##
## --- ATC-Lab Presentation Package ---
##

INCLUDEPATH += src/pact
VPATH       += src/pact

HEADERS += src/pact/experiment.h
HEADERS += src/pact/instructiontask.h
HEADERS += src/pact/nodehandler.h
HEADERS += src/pact/presenter.h
HEADERS += src/pact/questiontask.h
HEADERS += src/pact/task.h
HEADERS += src/pact/taskfactory.h
HEADERS += src/pact/taskwidget.h
HEADERS += src/pact/translator.h
HEADERS += src/pact/trialtask.h

SOURCES += src/pact/experiment.cpp
SOURCES += src/pact/instructiontask.cpp
SOURCES += src/pact/nodehandler.cpp
SOURCES += src/pact/pact.cpp
SOURCES += src/pact/presenter.cpp
SOURCES += src/pact/questiontask.cpp
SOURCES += src/pact/task.cpp
SOURCES += src/pact/taskfactory.cpp
SOURCES += src/pact/taskwidget.cpp
SOURCES += src/pact/translator.cpp
SOURCES += src/pact/trialtask.cpp
    

##
## --- XML parser ---
##

INCLUDEPATH += src/parser
VPATH       += src/parser 

HEADERS += src/parser/document.h
HEADERS += src/parser/xmlhandler.h
HEADERS += src/parser/xmlreader.h
HEADERS += src/parser/xmlstring.h

SOURCES += src/parser/document.cpp
SOURCES += src/parser/xmlhandler.cpp
SOURCES += src/parser/xmlreader.cpp


##
## --- ATC Engine ---
##

INCLUDEPATH += src/engine
VPATH       += src/engine

HEADERS += src/engine/acsprite.h
HEADERS += src/engine/actools.h
HEADERS += src/engine/aircraft_agent.h
HEADERS += src/engine/aircraft_data.h
HEADERS += src/engine/aircraftparam.h
HEADERS += src/engine/aircraftsprite.h
HEADERS += src/engine/airspace.h
HEADERS += src/engine/area_definition.h
HEADERS += src/engine/atc.h
HEADERS += src/engine/atc_types.h
HEADERS += src/engine/canvas.h
HEADERS += src/engine/canvas_tools.h
HEADERS += src/engine/canvas_types.h
HEADERS += src/engine/canvasview.h
HEADERS += src/engine/clock.h
HEADERS += src/engine/engine.h
HEADERS += src/engine/engine_builder.h
HEADERS += src/engine/engine_impl.h
HEADERS += src/engine/intersectionpoint.h
HEADERS += src/engine/lab_engine.h
HEADERS += src/engine/listbox.h
HEADERS += src/engine/map.h
HEADERS += src/engine/map_impl.h
HEADERS += src/engine/n_back_task.h
HEADERS += src/engine/param.h
HEADERS += src/engine/param_impl.h
HEADERS += src/engine/popup_question.h
HEADERS += src/engine/routearcsprite.h
HEADERS += src/engine/routesprite.h
HEADERS += src/engine/scale.h
HEADERS += src/engine/sectorarcsprite.h
HEADERS += src/engine/sectorsprite.h
HEADERS += src/engine/sky.h
HEADERS += src/engine/sky_impl.h
HEADERS += src/engine/transformation.h
HEADERS += src/engine/waypointsprite.h

SOURCES += src/engine/acsprite.cpp
SOURCES += src/engine/actools.cpp
SOURCES += src/engine/aircraft_agent.cpp
SOURCES += src/engine/aircraft_data.cpp
SOURCES += src/engine/aircraftsprite.cpp
SOURCES += src/engine/airspace.cpp
SOURCES += src/engine/area_definition.cpp
SOURCES += src/engine/canvas.cpp
SOURCES += src/engine/canvas_tools.cpp
SOURCES += src/engine/canvasview.cpp
SOURCES += src/engine/clock.cpp
SOURCES += src/engine/engine_builder.cpp
SOURCES += src/engine/engine_factory.cpp
SOURCES += src/engine/engine_impl.cpp
SOURCES += src/engine/intersectionpoint.cpp
SOURCES += src/engine/lab_engine.cpp
SOURCES += src/engine/listbox.cpp
SOURCES += src/engine/map_impl.cpp
SOURCES += src/engine/n_back_task.cpp
SOURCES += src/engine/param_impl.cpp
SOURCES += src/engine/popup_question.cpp
SOURCES += src/engine/routearcsprite.cpp
SOURCES += src/engine/routesprite.cpp
SOURCES += src/engine/scale.cpp
SOURCES += src/engine/sectorarcsprite.cpp
SOURCES += src/engine/sectorsprite.cpp
SOURCES += src/engine/sky_impl.cpp
SOURCES += src/engine/transformation.cpp
SOURCES += src/engine/waypointsprite.cpp


##
## --- ATC Logging ---
##

INCLUDEPATH += src/logging
VPATH       += src/logging

HEADERS += src/logging/logger.h

SOURCES += src/logging/logger.cpp


##
## --- ATC Performance ---
##

HEADERS += src/performance/atmosphere.h
HEADERS += src/performance/builder.h
HEADERS += src/performance/data.h
HEADERS += src/performance/exception.h
HEADERS += src/performance/factory.h
HEADERS += src/performance/parser.h
HEADERS += src/performance/performance.h
HEADERS += src/performance/writer.h

SOURCES += src/performance/atmosphere.cpp
SOURCES += src/performance/builder.cpp
SOURCES += src/performance/data.cpp
SOURCES += src/performance/factory.cpp
SOURCES += src/performance/parser.cpp
SOURCES += src/performance/performance.cpp
SOURCES += src/performance/writer.cpp

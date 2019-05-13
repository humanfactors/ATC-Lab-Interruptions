# Microsoft Developer Studio Project File - Name="pact" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=pact - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pact.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pact.mak" CFG="pact - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pact - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "pact - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pact - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "$(XERCESCROOT)\include" /I "src" /I "src\pact" /I "src\parser" /I "src\engine" /I "src\logging" /I "$(QTDIR)\include" /I "build\pact\debug\uic" /I "." /I "build\pact\debug\moc" /I "C:\Qt\3.3.6\mkspecs\win32-msvc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "QT_NO_DEBUG" /FR /FD -Zm200 /c
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "qt-mt336.lib" "qtmain.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "imm32.lib" "winmm.lib" "wsock32.lib" "winspool.lib" "xerces-c_2d.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "imm32.lib" "winmm.lib" "wsock32.lib" "winspool.lib" "opengl32.lib" "glu32.lib" "delayimp.lib" delayimp.lib /nologo /subsystem:windows /machine:IX86 /out:"build\pact\debug\pact.exe" /libpath:"$(QTDIR)\lib" /libpath:"$(XERCESCROOT)\lib" /DELAYLOAD:comdlg32.dll /DELAYLOAD:oleaut32.dll /DELAYLOAD:winmm.dll /DELAYLOAD:wsock32.dll /DELAYLOAD:winspool.dll

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "build/pact/debug"
# PROP BASE Intermediate_Dir "build/pact/debug/obj"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "build/pact/debug"
# PROP Intermediate_Dir "build/pact/debug/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /Od /I "$(XERCESCROOT)\include" /I "src" /I "src\pact" /I "src\parser" /I "src\engine" /I "src\logging" /I "$(QTDIR)\include" /I "build\pact\debug\uic" /I "." /I "build\pact\debug\moc" /I "C:\Qt\3.3.6\mkspecs\win32-msvc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FR /FD /GZ -Zm200 /c
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 "qt-mt336.lib" "qtmain.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "imm32.lib" "winmm.lib" "wsock32.lib" "winspool.lib" "xerces-c_2d.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "imm32.lib" "winmm.lib" "wsock32.lib" "winspool.lib" "opengl32.lib" "glu32.lib" "delayimp.lib" /nologo /subsystem:windows /debug /machine:IX86 /pdbtype:sept /libpath:"$(QTDIR)\lib" /libpath:"$(XERCESCROOT)\lib"

!ENDIF 

# Begin Target

# Name "pact - Win32 Release"
# Name "pact - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\engine\AcceptanceStateDialogBox.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\acsprite.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\actools.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\aircraft_agent.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\aircraft_data.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\aircraftsprite.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\airspace.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\area_definition.cpp
# End Source File
# Begin Source File

SOURCE=src\performance\atmosphere.cpp
# End Source File
# Begin Source File

SOURCE=src\performance\builder.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\canvas.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\canvas_tools.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\canvasview.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\clock.cpp
# End Source File
# Begin Source File

SOURCE=src\performance\data.cpp
# End Source File
# Begin Source File

SOURCE=src\parser\document.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\engine_builder.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\engine_factory.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\engine_impl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\logging\ExcelLogger.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\experiment.cpp
# End Source File
# Begin Source File

SOURCE=src\performance\factory.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\instructiontask.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\interrupt_task.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\intersectionpoint.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\lab_engine.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\listbox.cpp
# End Source File
# Begin Source File

SOURCE=src\logging\logger.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\map_impl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\n_back_task.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\nodehandler.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\pact.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\param_impl.cpp
# End Source File
# Begin Source File

SOURCE=src\performance\parser.cpp
# End Source File
# Begin Source File

SOURCE=src\performance\performance.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\popup_question.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\presenter.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\questiontask.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\ReminderBox.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\routearcsprite.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\routesprite.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\scale.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\ScoreLogger.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\sectorarcsprite.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\sectorsprite.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\sky_impl.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\task.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\taskfactory.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\taskwidget.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine\timer.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\transformation.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\translator.cpp
# End Source File
# Begin Source File

SOURCE=src\pact\trialtask.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\Utilities.cpp
# End Source File
# Begin Source File

SOURCE=src\engine\waypointsprite.cpp
# End Source File
# Begin Source File

SOURCE=src\performance\writer.cpp
# End Source File
# Begin Source File

SOURCE=src\parser\xmlhandler.cpp
# End Source File
# Begin Source File

SOURCE=src\parser\xmlreader.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\engine\AcceptanceStateDialogBox.h
# End Source File
# Begin Source File

SOURCE=src\engine\acsprite.h
# End Source File
# Begin Source File

SOURCE=src\engine\actools.h
# End Source File
# Begin Source File

SOURCE=src\engine\aircraft_agent.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__AIRCR=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\aircraft_agent.h...
InputPath=src\engine\aircraft_agent.h

"build\pact\debug\moc\moc_aircraft_agent.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\aircraft_agent.h -o build\pact\debug\moc\moc_aircraft_agent.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__AIRCR=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\aircraft_agent.h...
InputPath=src\engine\aircraft_agent.h

"build\pact\debug\moc\moc_aircraft_agent.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\aircraft_agent.h -o build\pact\debug\moc\moc_aircraft_agent.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\aircraft_data.h
# End Source File
# Begin Source File

SOURCE=src\engine\aircraftparam.h
# End Source File
# Begin Source File

SOURCE=src\engine\aircraftsprite.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__AIRCRA=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\aircraftsprite.h...
InputPath=src\engine\aircraftsprite.h

"build\pact\debug\moc\moc_aircraftsprite.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\aircraftsprite.h -o build\pact\debug\moc\moc_aircraftsprite.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__AIRCRA=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\aircraftsprite.h...
InputPath=src\engine\aircraftsprite.h

"build\pact\debug\moc\moc_aircraftsprite.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\aircraftsprite.h -o build\pact\debug\moc\moc_aircraftsprite.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\airspace.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__AIRSP=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\airspace.h...
InputPath=src\engine\airspace.h

"build\pact\debug\moc\moc_airspace.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\airspace.h -o build\pact\debug\moc\moc_airspace.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__AIRSP=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\airspace.h...
InputPath=src\engine\airspace.h

"build\pact\debug\moc\moc_airspace.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\airspace.h -o build\pact\debug\moc\moc_airspace.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\area_definition.h
# End Source File
# Begin Source File

SOURCE=src\engine\atc.h
# End Source File
# Begin Source File

SOURCE=src\engine\atc_types.h
# End Source File
# Begin Source File

SOURCE=src\performance\atmosphere.h
# End Source File
# Begin Source File

SOURCE=src\performance\builder.h
# End Source File
# Begin Source File

SOURCE=src\engine\canvas.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__CANVA=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\canvas.h...
InputPath=src\engine\canvas.h

"build\pact\debug\moc\moc_canvas.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\canvas.h -o build\pact\debug\moc\moc_canvas.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__CANVA=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\canvas.h...
InputPath=src\engine\canvas.h

"build\pact\debug\moc\moc_canvas.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\canvas.h -o build\pact\debug\moc\moc_canvas.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\canvas_tools.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__CANVAS=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\canvas_tools.h...
InputPath=src\engine\canvas_tools.h

"build\pact\debug\moc\moc_canvas_tools.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\canvas_tools.h -o build\pact\debug\moc\moc_canvas_tools.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__CANVAS=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\canvas_tools.h...
InputPath=src\engine\canvas_tools.h

"build\pact\debug\moc\moc_canvas_tools.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\canvas_tools.h -o build\pact\debug\moc\moc_canvas_tools.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\canvas_types.h
# End Source File
# Begin Source File

SOURCE=src\engine\canvasview.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__CANVASV=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\canvasview.h...
InputPath=src\engine\canvasview.h

"build\pact\debug\moc\moc_canvasview.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\canvasview.h -o build\pact\debug\moc\moc_canvasview.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__CANVASV=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\canvasview.h...
InputPath=src\engine\canvasview.h

"build\pact\debug\moc\moc_canvasview.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\canvasview.h -o build\pact\debug\moc\moc_canvasview.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\clock.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__CLOCK=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\clock.h...
InputPath=src\engine\clock.h

"build\pact\debug\moc\moc_clock.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\clock.h -o build\pact\debug\moc\moc_clock.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__CLOCK=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\clock.h...
InputPath=src\engine\clock.h

"build\pact\debug\moc\moc_clock.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\clock.h -o build\pact\debug\moc\moc_clock.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\performance\data.h
# End Source File
# Begin Source File

SOURCE=src\parser\document.h
# End Source File
# Begin Source File

SOURCE=src\engine\engine.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__ENGIN=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\engine.h...
InputPath=src\engine\engine.h

"build\pact\debug\moc\moc_engine.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\engine.h -o build\pact\debug\moc\moc_engine.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__ENGIN=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\engine.h...
InputPath=src\engine\engine.h

"build\pact\debug\moc\moc_engine.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\engine.h -o build\pact\debug\moc\moc_engine.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\engine_builder.h
# End Source File
# Begin Source File

SOURCE=src\engine\engine_impl.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__ENGINE=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\engine_impl.h...
InputPath=src\engine\engine_impl.h

"build\pact\debug\moc\moc_engine_impl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\engine_impl.h -o build\pact\debug\moc\moc_engine_impl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__ENGINE=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\engine_impl.h...
InputPath=src\engine\engine_impl.h

"build\pact\debug\moc\moc_engine_impl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\engine_impl.h -o build\pact\debug\moc\moc_engine_impl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\logging\ExcelLogger.h
# End Source File
# Begin Source File

SOURCE=src\performance\exception.h
# End Source File
# Begin Source File

SOURCE=src\pact\experiment.h
# End Source File
# Begin Source File

SOURCE=src\performance\factory.h
# End Source File
# Begin Source File

SOURCE=src\pact\instructiontask.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__INSTR=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\instructiontask.h...
InputPath=src\pact\instructiontask.h

"build\pact\debug\moc\moc_instructiontask.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\instructiontask.h -o build\pact\debug\moc\moc_instructiontask.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__INSTR=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\instructiontask.h...
InputPath=src\pact\instructiontask.h

"build\pact\debug\moc\moc_instructiontask.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\instructiontask.h -o build\pact\debug\moc\moc_instructiontask.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\engine\interrupt_task.h

!IF  "$(CFG)" == "pact - Win32 Release"

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

# Begin Custom Build - Moc'ing src\engine\interrupt_task...
InputPath=.\src\engine\interrupt_task.h

"build\pact\debug\moc\moc_interrupt_task.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\interrupt_task.h -o build\pact\debug\moc\moc_interrupt_task.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\intersectionpoint.h
# End Source File
# Begin Source File

SOURCE=src\engine\lab_engine.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__LAB_E=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\lab_engine.h...
InputPath=src\engine\lab_engine.h

"build\pact\debug\moc\moc_lab_engine.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\lab_engine.h -o build\pact\debug\moc\moc_lab_engine.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__LAB_E=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\lab_engine.h...
InputPath=src\engine\lab_engine.h

"build\pact\debug\moc\moc_lab_engine.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\lab_engine.h -o build\pact\debug\moc\moc_lab_engine.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\listbox.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__LISTB=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\listbox.h...
InputPath=src\engine\listbox.h

"build\pact\debug\moc\moc_listbox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\listbox.h -o build\pact\debug\moc\moc_listbox.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__LISTB=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\listbox.h...
InputPath=src\engine\listbox.h

"build\pact\debug\moc\moc_listbox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\listbox.h -o build\pact\debug\moc\moc_listbox.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\logging\logger.h
# End Source File
# Begin Source File

SOURCE=src\engine\map.h
# End Source File
# Begin Source File

SOURCE=src\engine\map_impl.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\n_back_task.h

!IF  "$(CFG)" == "pact - Win32 Release"

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__N_BAC="$(QTDIR)\bin\moc.exe"	
# Begin Custom Build - Moc'ing src\engine\n_back_task...
InputPath=.\src\engine\n_back_task.h

"build\pact\debug\moc\moc_n_back_task.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\n_back_task.h -o build\pact\debug\moc\moc_n_back_task.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\pact\nodehandler.h
# End Source File
# Begin Source File

SOURCE=src\engine\param.h
# End Source File
# Begin Source File

SOURCE=src\engine\param_impl.h
# End Source File
# Begin Source File

SOURCE=src\performance\parser.h
# End Source File
# Begin Source File

SOURCE=src\performance\performance.h
# End Source File
# Begin Source File

SOURCE=src\engine\popup_question.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__POPUP=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\popup_question.h...
InputPath=src\engine\popup_question.h

"build\pact\debug\moc\moc_popup_question.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\popup_question.h -o build\pact\debug\moc\moc_popup_question.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__POPUP=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\engine\popup_question.h...
InputPath=src\engine\popup_question.h

"build\pact\debug\moc\moc_popup_question.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\popup_question.h -o build\pact\debug\moc\moc_popup_question.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\pact\presenter.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__PRESE=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\presenter.h...
InputPath=src\pact\presenter.h

"build\pact\debug\moc\moc_presenter.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\presenter.h -o build\pact\debug\moc\moc_presenter.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__PRESE=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\presenter.h...
InputPath=src\pact\presenter.h

"build\pact\debug\moc\moc_presenter.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\presenter.h -o build\pact\debug\moc\moc_presenter.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\pact\questiontask.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__QUEST=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\questiontask.h...
InputPath=src\pact\questiontask.h

"build\pact\debug\moc\moc_questiontask.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\questiontask.h -o build\pact\debug\moc\moc_questiontask.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__QUEST=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\questiontask.h...
InputPath=src\pact\questiontask.h

"build\pact\debug\moc\moc_questiontask.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\questiontask.h -o build\pact\debug\moc\moc_questiontask.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\engine\ReminderBox.h

!IF  "$(CFG)" == "pact - Win32 Release"

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__REMIN="$(QTDIR)\bin\moc.exe"	
# Begin Custom Build - Moc'ing src\engine\ReminderBox...
InputPath=.\src\engine\ReminderBox.h

"build\pact\debug\moc\moc_ReminderBox.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\engine\ReminderBox.h -o build\pact\debug\moc\moc_ReminderBox.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\routearcsprite.h
# End Source File
# Begin Source File

SOURCE=src\engine\routesprite.h
# End Source File
# Begin Source File

SOURCE=src\engine\scale.h
# End Source File
# Begin Source File

SOURCE=.\src\engine\ScoreLogger.h
# End Source File
# Begin Source File

SOURCE=src\engine\sectorarcsprite.h
# End Source File
# Begin Source File

SOURCE=src\engine\sectorsprite.h
# End Source File
# Begin Source File

SOURCE=src\engine\sky.h
# End Source File
# Begin Source File

SOURCE=src\engine\sky_impl.h
# End Source File
# Begin Source File

SOURCE=src\pact\task.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__TASK_=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\task.h...
InputPath=src\pact\task.h

"build\pact\debug\moc\moc_task.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\task.h -o build\pact\debug\moc\moc_task.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__TASK_=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\task.h...
InputPath=src\pact\task.h

"build\pact\debug\moc\moc_task.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\task.h -o build\pact\debug\moc\moc_task.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\pact\taskfactory.h
# End Source File
# Begin Source File

SOURCE=src\pact\taskwidget.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__TASKW=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\taskwidget.h...
InputPath=src\pact\taskwidget.h

"build\pact\debug\moc\moc_taskwidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\taskwidget.h -o build\pact\debug\moc\moc_taskwidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__TASKW=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\taskwidget.h...
InputPath=src\pact\taskwidget.h

"build\pact\debug\moc\moc_taskwidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\taskwidget.h -o build\pact\debug\moc\moc_taskwidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\engine\timer.h
# End Source File
# Begin Source File

SOURCE=src\engine\transformation.h
# End Source File
# Begin Source File

SOURCE=src\pact\translator.h
# End Source File
# Begin Source File

SOURCE=src\pact\trialtask.h

!IF  "$(CFG)" == "pact - Win32 Release"

USERDEP__TRIAL=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\trialtask.h...
InputPath=src\pact\trialtask.h

"build\pact\debug\moc\moc_trialtask.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\trialtask.h -o build\pact\debug\moc\moc_trialtask.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "pact - Win32 Debug"

USERDEP__TRIAL=""$(QTDIR)\bin\moc.exe""	
# Begin Custom Build - Moc'ing src\pact\trialtask.h...
InputPath=src\pact\trialtask.h

"build\pact\debug\moc\moc_trialtask.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(QTDIR)\bin\moc src\pact\trialtask.h -o build\pact\debug\moc\moc_trialtask.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=src\engine\Utilities.h
# End Source File
# Begin Source File

SOURCE=.\src\pact\version.h
# End Source File
# Begin Source File

SOURCE=src\engine\waypointsprite.h
# End Source File
# Begin Source File

SOURCE=src\performance\writer.h
# End Source File
# Begin Source File

SOURCE=src\parser\xmlhandler.h
# End Source File
# Begin Source File

SOURCE=src\parser\xmlreader.h
# End Source File
# Begin Source File

SOURCE=src\parser\xmlstring.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=build\pact\debug\moc\moc_aircraft_agent.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_aircraftsprite.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_airspace.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_canvas.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_canvas_tools.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_canvasview.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_clock.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_engine.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_engine_impl.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_instructiontask.cpp
# End Source File
# Begin Source File

SOURCE=.\build\pact\debug\moc\moc_interrupt_task.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_lab_engine.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_listbox.cpp
# End Source File
# Begin Source File

SOURCE=.\build\pact\debug\moc\moc_n_back_task.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_popup_question.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_presenter.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_questiontask.cpp
# End Source File
# Begin Source File

SOURCE=.\build\pact\debug\moc\moc_ReminderBox.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_task.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_taskwidget.cpp
# End Source File
# Begin Source File

SOURCE=build\pact\debug\moc\moc_trialtask.cpp
# End Source File
# End Group
# End Target
# End Project

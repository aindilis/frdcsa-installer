# Microsoft Developer Studio Project File - Name="vampire" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=vampire - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vampire.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vampire.mak" CFG="vampire - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vampire - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "vampire - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vampire - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /I "..\..\BlodKorv" /I "..\..\Gematogen" /I "..\..\VampireKernel" /I "..\..\VampireKernel/PR_PARSER" /I "..\..\antlr" /I "..\antlr" /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "NO_DEBUG" /D DEBUG_PREPRO=0 /D DEBUG_TRACE_ALL=0 /FR /YX /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib ..\..\VampireKernel\VKLibrary_VSProject\Release\vkernel.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "vampire - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Gm /GX /ZI /Od /I "..\BlodKorv" /I "..\..\BlodKorv" /I "..\..\Gematogen" /I "..\..\VampireKernel" /I "..\..\VampireKernel/PR_PARSER" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEBUG_ALL" /FR /YX /FD /GZ /Zm300 /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib ..\..\VampireKernel\VKLibrary_VSProject\Debug\vkernel.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "vampire - Win32 Release"
# Name "vampire - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\ANTLRException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\assert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ASTFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ASTRefCount.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Atom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BaseAST.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BitSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\CharBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\CharScanner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clause.cpp
# End Source File
# Begin Source File

SOURCE=..\..\CommonAST.cpp
# End Source File
# Begin Source File

SOURCE=..\..\CommonToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Formula.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Inference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\InputBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Int.cpp
# End Source File
# Begin Source File

SOURCE=..\..\IntNameTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\KIFLexer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\KIFParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\LexerSharedInputState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Literal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\LLkParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Map.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Memory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Miniscope.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MismatchedCharException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MismatchedTokenException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NoViableAltException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NoViableAltForCharException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Options.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Output.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ParserSharedInputState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Problem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Query.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Random.cpp
# End Source File
# Begin Source File

SOURCE=..\..\RecognitionException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Refutation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Renaming.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Signature.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Statistics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\String.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Substitution.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SymCounter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Tabulate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Term.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Token.cpp
# End Source File
# Begin Source File

SOURCE=..\..\TokenBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\TokenStreamSelector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\TPTPLexer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\TPTPParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Tracer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Unit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\vampire.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Var.cpp
# End Source File
# Begin Source File

SOURCE=..\..\XMLLexer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\XMLParser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

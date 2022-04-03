# Microsoft Developer Studio Project File - Name="VKLibrary_VSProject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VKLibrary_VSProject - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VKLibrary_VSProject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VKLibrary_VSProject.mak" CFG="VKLibrary_VSProject - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VKLibrary_VSProject - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VKLibrary_VSProject - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VKLibrary_VSProject - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /Ob2 /I "..\..\BlodKorv" /I "..\..\Gematogen" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "NO_DEBUG" /YX /FD /Zm300 /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\vkernel.lib"

!ELSEIF  "$(CFG)" == "VKLibrary_VSProject - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Gm /GX /ZI /Od /I "..\..\BlodKorv" /I "..\..\Gematogen" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DEBUG_ALL" /FR /YX /FD /GZ /Zm300 /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\vkernel.lib"

!ENDIF 

# Begin Target

# Name "VKLibrary_VSProject - Win32 Release"
# Name "VKLibrary_VSProject - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\AutoFunctorPrecedence.cpp
# End Source File
# Begin Source File

SOURCE=..\BackDemodulation.cpp
# End Source File
# Begin Source File

SOURCE=..\BackParaCandidates.cpp
# End Source File
# Begin Source File

SOURCE=..\backward_superposition.cpp
# End Source File
# Begin Source File

SOURCE=..\BackwardSubsumption.cpp
# End Source File
# Begin Source File

SOURCE=..\binary_resolution.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\BitWord.cpp
# End Source File
# Begin Source File

SOURCE=..\BSIndex.cpp
# End Source File
# Begin Source File

SOURCE=..\BSMachine.cpp
# End Source File
# Begin Source File

SOURCE=..\BuiltInFloatingPointArithmetic.cpp
# End Source File
# Begin Source File

SOURCE=..\BuiltInTheory.cpp
# End Source File
# Begin Source File

SOURCE=..\BuiltInTheoryDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\ClassDesc.cpp
# End Source File
# Begin Source File

SOURCE=..\Clause.cpp
# End Source File
# Begin Source File

SOURCE=..\ClauseAssembler.cpp
# End Source File
# Begin Source File

SOURCE=..\ClauseBackground.cpp
# End Source File
# Begin Source File

SOURCE=..\ClauseNumIndex.cpp
# End Source File
# Begin Source File

SOURCE=..\ClauseQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\ComponentName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Debug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\DestructionMode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Exit.cpp
# End Source File
# Begin Source File

SOURCE=..\Flatterm.cpp
# End Source File
# Begin Source File

SOURCE=..\forward_subsumption.cpp
# End Source File
# Begin Source File

SOURCE=..\forward_superposition.cpp
# End Source File
# Begin Source File

SOURCE=..\ForwardDemodulation.cpp
# End Source File
# Begin Source File

SOURCE=..\fs_code.cpp
# End Source File
# Begin Source File

SOURCE=..\fs_command.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GlobalClock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GlobAlloc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GlobAlloc2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GlobalStopFlag.cpp
# End Source File
# Begin Source File

SOURCE=..\InferenceDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=..\InferenceTree.cpp
# End Source File
# Begin Source File

SOURCE=..\InstanceRetrievalForDemodulation.cpp
# End Source File
# Begin Source File

SOURCE=..\iu_command.cpp
# End Source File
# Begin Source File

SOURCE=..\iu_machine.cpp
# End Source File
# Begin Source File

SOURCE=..\kept.cpp
# End Source File
# Begin Source File

SOURCE=..\LiteralList.cpp
# End Source File
# Begin Source File

SOURCE=..\LiteralOrdering.cpp
# End Source File
# Begin Source File

SOURCE=..\LiteralSelection.cpp
# End Source File
# Begin Source File

SOURCE=..\LiteralSharing.cpp
# End Source File
# Begin Source File

SOURCE=..\LocalUnification.cpp
# End Source File
# Begin Source File

SOURCE=..\Logging.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Malloc.cpp
# End Source File
# Begin Source File

SOURCE=..\Master.cpp
# End Source File
# Begin Source File

SOURCE=..\Master3.cpp
# End Source File
# Begin Source File

SOURCE=..\NewClause.cpp
# End Source File
# Begin Source File

SOURCE=..\NonrecursiveKBOrdering.cpp
# End Source File
# Begin Source File

SOURCE=..\NonuniformConstantTermWeighting.cpp
# End Source File
# Begin Source File

SOURCE=..\old_fs_code.cpp
# End Source File
# Begin Source File

SOURCE=..\or_index.cpp
# End Source File
# Begin Source File

SOURCE=..\or_premise.cpp
# End Source File
# Begin Source File

SOURCE=..\PagedPassiveClauseStorage.cpp
# End Source File
# Begin Source File

SOURCE=..\prefix.cpp
# End Source File
# Begin Source File

SOURCE=..\premise_factoring.cpp
# End Source File
# Begin Source File

SOURCE=..\ProblemProfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\RuntimeError.cpp
# End Source File
# Begin Source File

SOURCE=..\sharing.cpp
# End Source File
# Begin Source File

SOURCE=..\Signature.cpp
# End Source File
# Begin Source File

SOURCE=..\SimplificationOrdering.cpp
# End Source File
# Begin Source File

SOURCE=..\Splitting.cpp
# End Source File
# Begin Source File

SOURCE=..\Splitting3.cpp
# End Source File
# Begin Source File

SOURCE=..\StandardKBOrdering.cpp
# End Source File
# Begin Source File

SOURCE=..\Tabulation.cpp
# End Source File
# Begin Source File

SOURCE=..\Term.cpp
# End Source File
# Begin Source File

SOURCE=..\TermWeighting.cpp
# End Source File
# Begin Source File

SOURCE=..\tmp_literal.cpp
# End Source File
# Begin Source File

SOURCE=..\TmpLitList.cpp
# End Source File
# Begin Source File

SOURCE=..\UniformTermWeighting.cpp
# End Source File
# Begin Source File

SOURCE=..\VampireKernel.cpp
# End Source File
# Begin Source File

SOURCE=..\VampireKernelInterceptMacros.cpp
# End Source File
# Begin Source File

SOURCE=..\variables.cpp
# End Source File
# Begin Source File

SOURCE=..\weight_formula.cpp
# End Source File
# Begin Source File

SOURCE=..\WeightPolynomial.cpp
# End Source File
# Begin Source File

SOURCE=..\WeightPolynomialSpecialisedComparisonCommand.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\BlodKorv\AllocationFailureHandling.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Allocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Array.hpp
# End Source File
# Begin Source File

SOURCE=..\AutoFunctorPrecedence.hpp
# End Source File
# Begin Source File

SOURCE=..\BackDemodulation.hpp
# End Source File
# Begin Source File

SOURCE=..\BackParaCandidates.hpp
# End Source File
# Begin Source File

SOURCE=..\backward_superposition.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\BackwardSubsumptionWithPathIndexingAndJoins.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\BitSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\BitString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\BitWord.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\BlodKorvDebugFlags.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\BufferedAllocationStatistics.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\CacheAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\ClassDesc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\CompactNodeSmallHeaderSkipList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Comparison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Debug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\debugMacros.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\DestructionMode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\EquivalenceRelation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Exit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\ExpandingMultiset.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\ExpandingStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\FIFO.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\ForwardMatchingIndex.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\ForwardMatchingIndexTree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GDiscTree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\Gematogen.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\GematogenDebugFlags.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GIncCountingSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GlobalClock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GlobAlloc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GlobAlloc2.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GlobalStopFlag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\GSortTree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\InfiniteStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\jargon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\LinearCombination.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\LinearPolynomial.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Malloc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Math.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Multiset.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\MultisetOfVariables.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\ObjDesc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\ObjectPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\OrdList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\PageQueueDisciplineStorage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\PathIndex.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\PathIndexForBackwardSubsumption.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\PathIndexingWithJoinsAndEarlyAndCompiledCleanUpForInstanceRetrieval.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\PathIndexingWithJoinsAndEarlyCleanUpForInstanceRetrieval.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Gematogen\PathIndexingWithJoinsForInstanceRetrieval.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Queue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\RuntimeError.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\SharedLinearPolynomial.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\SkipList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\SmallHeaderSkipList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Stack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\StaticCellArray.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\String.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Timer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\BlodKorv\Tuple.hpp
# End Source File
# End Group
# End Target
# End Project

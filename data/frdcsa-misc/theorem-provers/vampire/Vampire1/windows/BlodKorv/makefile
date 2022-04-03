# File:    makefile 
# Author:  Alexandre Riazanov (riazanov@cs.man.ac.uk)
# Created: Feb 8, 2000
# Purpose: makefile for BlodKorv library 
################################################################
CC= g++ #      	# UNIX GNU compiler 
#CC= /opt/gcc-2.8.1/bin/g++
OPT=                # no optimisation, default
CFLAGS = -Wall #-I/usr/include 
################################################################

Obj= BitWord.o ClassDesc.o CommandLine.o Debug.o DestructionMode.o Exit.o FormatText.o GlobalClock.o GlobAlloc.o GlobAlloc2.o GlobalStopFlag.o Malloc.o RuntimeError.o 

################################################################

default: ${Obj}

.SUFFIXES:
.SUFFIXES: .cpp .o

.cpp.o:
	${CC} ${OPT} ${CFLAGS} -c $*.cpp
	
remake: clean ${Obj}

clean: 
	rm -f *.o ; rm -f *~ ; rm -f *.bak ; rm -f *.tmp 

################################################################
# DO NOT DELETE

BitWord.o: BitWord.hpp ../Dummies/climits ../Dummies/cstddef jargon.hpp
BitWord.o: BlodKorvDebugFlags.hpp debugMacros.hpp Debug.hpp
BitWord.o: ../Dummies/iostream
ClassDesc.o: ClassDesc.hpp ../Dummies/cstdlib ../Dummies/climits
ClassDesc.o: ../Dummies/iostream jargon.hpp Debug.hpp BlodKorvDebugFlags.hpp
CommandLine.o: CommandLine.hpp ../Dummies/cstring ../Dummies/climits
CommandLine.o: ../Dummies/cfloat ../Dummies/iostream jargon.hpp
CommandLine.o: BlodKorvDebugFlags.hpp GList.hpp ../Dummies/cstddef
CommandLine.o: debugMacros.hpp Debug.hpp Malloc.hpp ../Dummies/cstdlib
CommandLine.o: ClassDesc.hpp ObjDesc.hpp DestructionMode.hpp FormatText.hpp
Debug.o: ../Dummies/iostream ../Dummies/csignal ../Dummies/climits Debug.hpp
Debug.o: jargon.hpp BlodKorvDebugFlags.hpp Exit.hpp ../Dummies/cstdlib
DestructionMode.o: DestructionMode.hpp
Exit.o: Exit.hpp ../Dummies/cstdlib ../Dummies/iostream DestructionMode.hpp
FormatText.o: FormatText.hpp ../Dummies/cstring jargon.hpp
FormatText.o: BlodKorvDebugFlags.hpp ClassDesc.hpp ../Dummies/cstdlib
FormatText.o: ../Dummies/climits ../Dummies/iostream Debug.hpp Malloc.hpp
FormatText.o: ObjDesc.hpp debugMacros.hpp GList.hpp ../Dummies/cstddef
FormatText.o: DestructionMode.hpp
GlobAlloc.o: GlobAlloc.hpp ../Dummies/cstdlib jargon.hpp
GlobAlloc.o: BlodKorvDebugFlags.hpp ClassDesc.hpp ../Dummies/climits
GlobAlloc.o: ../Dummies/iostream Debug.hpp ObjDesc.hpp
GlobAlloc.o: AllocationFailureHandling.hpp Exit.hpp RuntimeError.hpp
GlobAlloc.o: DestructionMode.hpp debugMacros.hpp Allocator.hpp
GlobAlloc.o: BufferedAllocationStatistics.hpp
GlobAlloc2.o: GlobAlloc2.hpp ../Dummies/cstdlib jargon.hpp
GlobAlloc2.o: BlodKorvDebugFlags.hpp ClassDesc.hpp ../Dummies/climits
GlobAlloc2.o: ../Dummies/iostream Debug.hpp ObjDesc.hpp
GlobAlloc2.o: AllocationFailureHandling.hpp Exit.hpp RuntimeError.hpp
GlobAlloc2.o: DestructionMode.hpp debugMacros.hpp Allocator.hpp
GlobAlloc2.o: BufferedAllocationStatistics.hpp
GlobalClock.o: jargon.hpp /usr/include/sys/time.h /usr/include/features.h
GlobalClock.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
GlobalClock.o: /usr/include/bits/types.h ../Dummies/stddef.h
GlobalClock.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sched.h
GlobalClock.o: /usr/include/time.h /usr/include/bits/time.h
GlobalClock.o: /usr/include/sys/select.h /usr/include/bits/select.h
GlobalClock.o: /usr/include/bits/sigset.h ../Dummies/csignal GlobalClock.hpp
GlobalClock.o: BlodKorvDebugFlags.hpp DestructionMode.hpp debugMacros.hpp
GlobalClock.o: Debug.hpp ../Dummies/iostream RuntimeError.hpp
GlobalStopFlag.o: GlobalStopFlag.hpp jargon.hpp BlodKorvDebugFlags.hpp
GlobalStopFlag.o: DestructionMode.hpp debugMacros.hpp Debug.hpp
GlobalStopFlag.o: ../Dummies/iostream
Malloc.o: Malloc.hpp ../Dummies/cstdlib jargon.hpp BlodKorvDebugFlags.hpp
Malloc.o: ClassDesc.hpp ../Dummies/climits ../Dummies/iostream Debug.hpp
Malloc.o: ObjDesc.hpp debugMacros.hpp
RuntimeError.o: ../Dummies/iostream RuntimeError.hpp jargon.hpp Exit.hpp
RuntimeError.o: ../Dummies/cstdlib

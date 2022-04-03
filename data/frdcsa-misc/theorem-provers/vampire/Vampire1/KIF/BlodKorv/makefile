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

Obj= BitWord.o ClassDesc.o CommandLine.o Debug.o DestructionMode.o Exit.o FormatText.o GlobalClock.o GlobAlloc.o GlobalStopFlag.o Label.o Malloc.o NonstandardInt.o RuntimeError.o 

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

arith_test.o: /usr/include/stdlib.h /usr/include/features.h
arith_test.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
arith_test.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
arith_test.o: Timer.h /usr/include/time.h /usr/include/bits/time.h
arith_test.o: /usr/include/bits/types.h jargon.h target.h compiler.h
arith_test.o: BlodKorvDebugFlags.h debugMacros.h Debug.h
BitWord.o: BitWord.h /usr/include/limits.h /usr/include/features.h
BitWord.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BitWord.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
BitWord.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h jargon.h
BitWord.o: target.h compiler.h BlodKorvDebugFlags.h debugMacros.h Debug.h
BitWord.o: /usr/include/time.h /usr/include/bits/time.h
BitWord.o: /usr/include/bits/types.h
ClassDesc.o: ClassDesc.h /usr/include/stdlib.h /usr/include/features.h
ClassDesc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ClassDesc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ClassDesc.o: /usr/include/limits.h
ClassDesc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ClassDesc.o: jargon.h target.h compiler.h Debug.h /usr/include/time.h
ClassDesc.o: /usr/include/bits/time.h /usr/include/bits/types.h
ClassDesc.o: BlodKorvDebugFlags.h
CommandLine.o: CommandLine.h /usr/include/string.h /usr/include/features.h
CommandLine.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
CommandLine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
CommandLine.o: /usr/include/limits.h
CommandLine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
CommandLine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
CommandLine.o: jargon.h target.h compiler.h BlodKorvDebugFlags.h GList.h
CommandLine.o: debugMacros.h Debug.h /usr/include/time.h
CommandLine.o: /usr/include/bits/time.h /usr/include/bits/types.h Malloc.h
CommandLine.o: /usr/include/stdlib.h ClassDesc.h ObjDesc.h StrToX.h
CommandLine.o: RuntimeError.h DestructionMode.h FormatText.h
Debug.o: /usr/include/signal.h /usr/include/features.h
Debug.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Debug.o: /usr/include/bits/sigset.h /usr/include/bits/types.h
Debug.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Debug.o: /usr/include/bits/signum.h /usr/include/limits.h
Debug.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h Debug.h
Debug.o: /usr/include/time.h /usr/include/bits/time.h jargon.h target.h
Debug.o: compiler.h BlodKorvDebugFlags.h Exit.h /usr/include/stdlib.h
DestructionMode.o: DestructionMode.h
Exit.o: Exit.h /usr/include/stdlib.h /usr/include/features.h
Exit.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Exit.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Exit.o: DestructionMode.h
FormatText.o: FormatText.h /usr/include/string.h /usr/include/features.h
FormatText.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
FormatText.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
FormatText.o: jargon.h target.h compiler.h BlodKorvDebugFlags.h ClassDesc.h
FormatText.o: /usr/include/stdlib.h /usr/include/limits.h
FormatText.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
FormatText.o: Debug.h /usr/include/time.h /usr/include/bits/time.h
FormatText.o: /usr/include/bits/types.h Malloc.h ObjDesc.h debugMacros.h
FormatText.o: GList.h DestructionMode.h
GlobalClock.o: /usr/include/sys/time.h /usr/include/features.h
GlobalClock.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
GlobalClock.o: /usr/include/bits/types.h
GlobalClock.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
GlobalClock.o: /usr/include/time.h /usr/include/bits/time.h
GlobalClock.o: /usr/include/sys/select.h /usr/include/bits/select.h
GlobalClock.o: /usr/include/bits/sigset.h /usr/include/signal.h
GlobalClock.o: /usr/include/bits/signum.h GlobalClock.h
GlobAlloc.o: GlobAlloc.h /usr/include/stdlib.h /usr/include/features.h
GlobAlloc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
GlobAlloc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
GlobAlloc.o: jargon.h target.h compiler.h BlodKorvDebugFlags.h ClassDesc.h
GlobAlloc.o: /usr/include/limits.h
GlobAlloc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h Debug.h
GlobAlloc.o: /usr/include/time.h /usr/include/bits/time.h
GlobAlloc.o: /usr/include/bits/types.h ObjDesc.h AllocationFailureHandling.h
GlobAlloc.o: Exit.h DestructionMode.h RuntimeError.h debugMacros.h
GlobAlloc.o: Allocator.h BufferedAllocationStatistics.h
GlobalStopFlag.o: GlobalStopFlag.h jargon.h target.h compiler.h
GlobalStopFlag.o: BlodKorvDebugFlags.h debugMacros.h Debug.h
GlobalStopFlag.o: /usr/include/time.h /usr/include/features.h
GlobalStopFlag.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
GlobalStopFlag.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
GlobalStopFlag.o: /usr/include/bits/time.h /usr/include/bits/types.h
Label.o: Label.h /usr/include/limits.h /usr/include/features.h
Label.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Label.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h jargon.h
Label.o: target.h compiler.h
Malloc.o: Malloc.h /usr/include/stdlib.h /usr/include/features.h
Malloc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Malloc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h jargon.h
Malloc.o: target.h compiler.h BlodKorvDebugFlags.h ClassDesc.h
Malloc.o: /usr/include/limits.h
Malloc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h Debug.h
Malloc.o: /usr/include/time.h /usr/include/bits/time.h
Malloc.o: /usr/include/bits/types.h ObjDesc.h debugMacros.h
NonstandardInt.o: NonstandardInt.h /usr/include/limits.h
NonstandardInt.o: /usr/include/features.h /usr/include/sys/cdefs.h
NonstandardInt.o: /usr/include/gnu/stubs.h
NonstandardInt.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
NonstandardInt.o: jargon.h target.h compiler.h BlodKorvDebugFlags.h
NonstandardInt.o: RuntimeError.h Comparison.h debugMacros.h Debug.h
NonstandardInt.o: /usr/include/time.h
NonstandardInt.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
NonstandardInt.o: /usr/include/bits/time.h /usr/include/bits/types.h
RuntimeError.o: RuntimeError.h jargon.h target.h compiler.h Exit.h
RuntimeError.o: /usr/include/stdlib.h /usr/include/features.h
RuntimeError.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
RuntimeError.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h

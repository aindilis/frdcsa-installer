# File:    makefile 
# Author:  Alexandre Riazanov, Andrei Voronkov (<surname>@cs.man.ac.uk)
# Created: who remembers
# Purpose: makefile for various clones of Vampire
################################################################
CC=g++
PPOPT=-DDEBUG_PREPRO=0 -DDEBUG_TRACE_ALL=0 -O4 -Wall# default options for compiling the preprocessor
#PPOPT=-O4 -Wall# default options for compiling the preprocessor
# VKOPT=-DDEBUG_ALL -DLARGE_TERMS # default options for compiling the kernel
VKOPT=-O4 -DNO_DEBUG -DHUGE_TERMS # default options for compiling the kernel

KIF_CURRENT_PATH=$(PWD)
ANTLR=antlr
KIF_CLASSPATH=$(KIF_CURRENT_PATH)/antlr
TPTP_CLASSPATH=$(KIF_CURRENT_PATH)/antlr
KIF_ANTLR_LOCAL=$(KIF_CLASSPATH)/local
KIF_INCLUDE_DIR=$(KIF_ANTLR_LOCAL)/include
KIF_LIB_DIR=$(KIF_ANTLR_LOCAL)/lib
KIF_CXXFLAGS=-I$(KIF_INCLUDE_DIR) -L$(KIF_LIB_DIR) -Wall -O4
KIF_ANTLR_SRC=$(KIF_CLASSPATH)/lib/cpp

MDOPT= -I/usr/include -I/usr/include/sys\
	-I$(KIF_INCLUDE_DIR) -L$(KIF_LIB_DIR) # default makedepend options
################################################################

PreproObj = Output.o\
            kif.o\
            Query.o\
            Signature.o\
            Term.o\
            Substitution.o\
            Var.o\
            SymCounter.o\
            Options.o\
            Unit.o\
            Problem.o\
            Tracer.o\
            Refutation.o\
            Statistics.o\
            Dump.o

	./XMLLexer.o\
	./XMLParser.o

TPTPObj = ./TPTPLexer.o\
	./TPTPParser.o

KernelObj = ./VampireKernel/vkernel.a

################################################################

default: vampire

KIFLexer.hpp KIFLexer.cpp KIFParser.hpp KIFParser.cpp: KIF.g 
	export CLASSPATH=$(KIF_CLASSPATH);java $(ANTLR).Tool KIF.g

TPTPLexer.hpp TPTPLexer.cpp TPTPParser.hpp TPTPParser.cpp: TPTP.g 
	export CLASSPATH=$(TPTP_CLASSPATH);java $(ANTLR).Tool TPTP.g

XMLLexer.hpp XMLLexer.cpp XMLParser.hpp XMLParser.cpp: XML.g 
	export CLASSPATH=$(KIF_CLASSPATH);java $(ANTLR).Tool XML.g

antlr:
	cd $(KIF_ANTLR_SRC);./configure --prefix=$(KIF_ANTLR_LOCAL);
	make -C $(KIF_ANTLR_SRC)
	make -C $(KIF_ANTLR_SRC) install

clean_antlr:
	make -C $(KIF_ANTLR_SRC) clean
	make -C $(KIF_ANTLR_SRC) distclean
	rm -rf $(KIF_LIB_DIR)
	rm -rf $(KIF_INCLUDE_DIR)

################################################################

make: clean vampire 

vampire: ${KIFObj} ${TPTPObj} ${PreproObj} kernel
	${CC} -static ${PreproObj} ${TPTPObj} ${PPOPT} ${VKOPT} -I./VampireKernel -I./BlodKorv ./VampireKernel/vkernel.a -L${KIF_LIB_DIR} ${KIFObj} -lantlr -o vampire
	strip vampire

kif: ${KIFObj} ${PreproObj} kernel
	${CC} -static ${PreproObj} ${PPOPT} ${VKOPT} -I./VampireKernel -I./BlodKorv ./VampireKernel/vkernel.a -L${KIF_LIB_DIR} ${KIFObj} -lantlr -o kif
	strip kif

vampireTPTP: ${PreproObj} ${TPTPObj} kernel kif.o query.o
	${CC} -static ${PreproObj} ${PPOPT} ${VKOPT} -I./VampireKernel -I./BlodKorv kif.o query.o ./VampireKernel/vkernel.a -L${KIF_LIB_DIR} ${KIFObj} -lantlr -o vampireTPTP
	strip vampireTPTP

prepro: ${PreproObj} 
	${CC} -static ${PPOPT} ${PreproObj} clausifier.cpp -o prepro

kernel:
	cd ./VampireKernel ; ${MAKE} vkernel.a CC=${CC} OPT='${VKOPT}'

.SUFFIXES:
.SUFFIXES: .cpp .o

.cpp.o:
	${CC} ${PPOPT} ${VKOPT} -I$(KIF_INCLUDE_DIR) -Wall -I./VampireKernel -I./BlodKorv -I./Gematogen -c $*.cpp

clean:
	rm -f KIFLexer.cpp KIFLexer.hpp KIFParser.cpp KIFParser.hpp KIFLexerTokenTypes.hpp KIFLexerTokenTypes.txt
	rm -f XMLLexer.cpp XMLLexer.hpp XMLParser.cpp XMLParser.hpp XMLLexerTokenTypes.hpp XMLLexerTokenTypes.txt
	rm -f *.o 
	cd ./VampireKernel ; ${MAKE} clean
	cd ./KIF ; ${MAKE} tidy

depend:
	makedepend ${MDOPT} -I./VampireKernel -I./BlodKorv -I./Gematogen *.cpp
	cd ./VampireKernel ; ${MAKE} depend

################################################################

# DO NOT DELETE

assert.o: assert.hpp /usr/include/stdlib.h /usr/include/features.h
assert.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
assert.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Tracer.hpp
Atom.o: Atom.hpp Term.hpp Signature.hpp Int.hpp /usr/include/limits.h
Atom.o: /usr/include/features.h /usr/include/sys/cdefs.h
Atom.o: /usr/include/gnu/stubs.h /usr/include/limits.h
Atom.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Atom.o: assert.hpp /usr/include/stdlib.h
Atom.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Tracer.hpp
Atom.o: Memory.hpp Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
Atom.o: ./BlodKorv/jargon.hpp
Clause.o: Clause.hpp Literal.hpp Atom.hpp Term.hpp Signature.hpp Int.hpp
Clause.o: /usr/include/limits.h /usr/include/features.h
Clause.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Clause.o: /usr/include/limits.h
Clause.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Clause.o: assert.hpp /usr/include/stdlib.h
Clause.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Tracer.hpp
Clause.o: Memory.hpp Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
Clause.o: ./BlodKorv/jargon.hpp Sort.hpp Renaming.hpp Map.hpp
DLGLexer.o: /usr/include/term.h /usr/include/ncurses/ncurses_dll.h
DLGLexer.o: /usr/include/termios.h /usr/include/features.h
DLGLexer.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
DLGLexer.o: /usr/include/bits/termios.h DLexer.cpp
Dump.o: assert.hpp /usr/include/stdlib.h /usr/include/features.h
Dump.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Dump.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Tracer.hpp
Dump.o: Dump.hpp
Formula.o: /usr/include/string.h /usr/include/features.h
Formula.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Formula.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Formula.o: Formula.hpp Atom.hpp Term.hpp Signature.hpp Int.hpp
Formula.o: /usr/include/limits.h /usr/include/limits.h
Formula.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Formula.o: assert.hpp /usr/include/stdlib.h Tracer.hpp Memory.hpp Var.hpp
Formula.o: Lst.hpp ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
Formula.o: Sort.hpp Substitution.hpp
IntNameTable.o: /usr/include/string.h /usr/include/features.h
IntNameTable.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
IntNameTable.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
IntNameTable.o: IntNameTable.hpp List.hpp assert.hpp /usr/include/stdlib.h
IntNameTable.o: Tracer.hpp Memory.hpp Pair.hpp
kif.o: /usr/include/string.h /usr/include/features.h /usr/include/sys/cdefs.h
kif.o: /usr/include/gnu/stubs.h
kif.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
kif.o: /usr/include/stdlib.h /usr/include/stdio.h /usr/include/bits/types.h
kif.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
kif.o: /usr/include/bits/wchar.h /usr/include/gconv.h
kif.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
kif.o: /usr/include/bits/stdio_lim.h /usr/include/signal.h
kif.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
kif.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/unistd.h
kif.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
kif.o: TPTPLexer.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/config.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/CommonToken.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/Token.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/RefCount.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/InputBuffer.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/CircularQueue.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/BitSet.hpp
kif.o: TPTPLexerTokenTypes.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/CharScanner.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStream.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/RecognitionException.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/ANTLRException.hpp
kif.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/LexerSharedInputState.hpp
kif.o: Unit.hpp Chain.hpp Memory.hpp assert.hpp Tracer.hpp Sort.hpp
kif.o: Clause.hpp Literal.hpp Atom.hpp Term.hpp Signature.hpp Int.hpp
kif.o: /usr/include/limits.h /usr/include/limits.h
kif.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
kif.o: Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
kif.o: ./BlodKorv/jargon.hpp Formula.hpp TPTPParser.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenBuffer.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/LLkParser.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/Parser.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/ASTFactory.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/AST.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/ASTRefCount.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/ASTArray.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/ASTPair.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/ParserSharedInputState.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/CharStreamException.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStreamRecognitionException.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStreamException.hpp
kif.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStreamSelector.hpp
kif.o: ./VampireKernel/Tabulation.hpp ./BlodKorv/GlobAlloc.hpp
kif.o: ./BlodKorv/BlodKorvDebugFlags.hpp ./BlodKorv/ClassDesc.hpp
kif.o: ./BlodKorv/Debug.hpp ./BlodKorv/ObjDesc.hpp
kif.o: ./BlodKorv/AllocationFailureHandling.hpp ./BlodKorv/Exit.hpp
kif.o: ./BlodKorv/RuntimeError.hpp ./BlodKorv/DestructionMode.hpp
kif.o: ./BlodKorv/debugMacros.hpp ./BlodKorv/Allocator.hpp
kif.o: ./BlodKorv/BufferedAllocationStatistics.hpp
kif.o: ./VampireKernel/VampireKernelDebugFlags.hpp
kif.o: ./VampireKernel/TermWeightType.hpp ./VampireKernel/Input.hpp
kif.o: ./BlodKorv/GList.hpp ./BlodKorv/Malloc.hpp ./BlodKorv/GlobalClock.hpp
kif.o: Options.hpp Output.hpp Renaming.hpp Map.hpp Problem.hpp
kif.o: IntNameTable.hpp Pair.hpp Refutation.hpp Statistics.hpp
ListSubstitution.o: ListSubstitution.hpp Term.hpp Signature.hpp Int.hpp
ListSubstitution.o: /usr/include/limits.h /usr/include/features.h
ListSubstitution.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ListSubstitution.o: /usr/include/limits.h
ListSubstitution.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ListSubstitution.o: List.hpp assert.hpp /usr/include/stdlib.h
ListSubstitution.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ListSubstitution.o: Tracer.hpp Memory.hpp Var.hpp Lst.hpp
ListSubstitution.o: ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
Literal.o: Literal.hpp Atom.hpp Term.hpp Signature.hpp Int.hpp
Literal.o: /usr/include/limits.h /usr/include/features.h
Literal.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Literal.o: /usr/include/limits.h
Literal.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Literal.o: assert.hpp /usr/include/stdlib.h
Literal.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Literal.o: Tracer.hpp Memory.hpp Var.hpp Lst.hpp
Literal.o: ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
Literal.o: Substitution.hpp
Map.o: Map.hpp assert.hpp /usr/include/stdlib.h /usr/include/features.h
Map.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Map.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Tracer.hpp
Memory.o: Memory.hpp /usr/include/stdlib.h /usr/include/features.h
Memory.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Memory.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h assert.hpp
Memory.o: Tracer.hpp
Options.o: /usr/include/string.h /usr/include/features.h
Options.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Options.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Options.o: /usr/include/stdlib.h /usr/include/limits.h /usr/include/limits.h
Options.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Options.o: assert.hpp Tracer.hpp Options.hpp Output.hpp Formula.hpp Atom.hpp
Options.o: Term.hpp Signature.hpp Int.hpp List.hpp Memory.hpp Var.hpp Lst.hpp
Options.o: ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
Output.o: Output.hpp Formula.hpp Atom.hpp Term.hpp Signature.hpp Int.hpp
Output.o: /usr/include/limits.h /usr/include/features.h
Output.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Output.o: /usr/include/limits.h
Output.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Output.o: assert.hpp /usr/include/stdlib.h
Output.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Tracer.hpp
Output.o: Memory.hpp Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
Output.o: ./BlodKorv/jargon.hpp Unit.hpp Chain.hpp Sort.hpp Clause.hpp
Output.o: Literal.hpp Refutation.hpp Map.hpp Problem.hpp IntNameTable.hpp
Output.o: Pair.hpp
primes.o: /usr/include/stdlib.h /usr/include/features.h
primes.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
primes.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Problem.o: /usr/include/string.h /usr/include/features.h
Problem.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Problem.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Problem.o: /usr/include/limits.h /usr/include/limits.h
Problem.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Problem.o: /usr/include/stdlib.h Problem.hpp Unit.hpp Chain.hpp Memory.hpp
Problem.o: assert.hpp Tracer.hpp Sort.hpp Clause.hpp Literal.hpp Atom.hpp
Problem.o: Term.hpp Signature.hpp Int.hpp List.hpp Var.hpp Lst.hpp
Problem.o: ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
Problem.o: Formula.hpp Options.hpp Output.hpp SymCounter.hpp
Query.o: /usr/include/stdlib.h /usr/include/features.h
Query.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Query.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Query.hpp
Query.o: Formula.hpp Atom.hpp Term.hpp Signature.hpp Int.hpp
Query.o: /usr/include/limits.h /usr/include/limits.h
Query.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Query.o: assert.hpp Tracer.hpp Memory.hpp Var.hpp Lst.hpp
Query.o: ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
Refutation.o: Refutation.hpp List.hpp assert.hpp /usr/include/stdlib.h
Refutation.o: /usr/include/features.h /usr/include/sys/cdefs.h
Refutation.o: /usr/include/gnu/stubs.h
Refutation.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Refutation.o: Tracer.hpp Memory.hpp Unit.hpp Chain.hpp Sort.hpp Clause.hpp
Refutation.o: Literal.hpp Atom.hpp Term.hpp Signature.hpp Int.hpp
Refutation.o: /usr/include/limits.h /usr/include/limits.h
Refutation.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Refutation.o: Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
Refutation.o: ./BlodKorv/jargon.hpp Formula.hpp Problem.hpp IntNameTable.hpp
Refutation.o: Pair.hpp Map.hpp
Signature.o: /usr/include/string.h /usr/include/features.h
Signature.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Signature.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Signature.o: /usr/include/stdio.h /usr/include/bits/types.h
Signature.o: /usr/include/libio.h /usr/include/_G_config.h
Signature.o: /usr/include/wchar.h /usr/include/bits/wchar.h
Signature.o: /usr/include/gconv.h
Signature.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
Signature.o: /usr/include/bits/stdio_lim.h Signature.hpp Int.hpp
Signature.o: /usr/include/limits.h /usr/include/limits.h
Signature.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Signature.o: List.hpp assert.hpp /usr/include/stdlib.h Tracer.hpp Memory.hpp
Signature.o: SymCounter.hpp Unit.hpp Chain.hpp Sort.hpp Clause.hpp
Signature.o: Literal.hpp Atom.hpp Term.hpp Var.hpp Lst.hpp
Signature.o: ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
Signature.o: Formula.hpp
Statistics.o: Statistics.hpp ./VampireKernel/VampireKernel.hpp
Statistics.o: ./BlodKorv/jargon.hpp Options.hpp Output.hpp Formula.hpp
Statistics.o: Atom.hpp Term.hpp Signature.hpp Int.hpp /usr/include/limits.h
Statistics.o: /usr/include/features.h /usr/include/sys/cdefs.h
Statistics.o: /usr/include/gnu/stubs.h /usr/include/limits.h
Statistics.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Statistics.o: List.hpp assert.hpp /usr/include/stdlib.h
Statistics.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Statistics.o: Tracer.hpp Memory.hpp Var.hpp Lst.hpp
Substitution.o: Substitution.hpp Term.hpp Signature.hpp Int.hpp
Substitution.o: /usr/include/limits.h /usr/include/features.h
Substitution.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Substitution.o: /usr/include/limits.h
Substitution.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Substitution.o: List.hpp assert.hpp /usr/include/stdlib.h
Substitution.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Substitution.o: Tracer.hpp Memory.hpp Var.hpp Lst.hpp
Substitution.o: ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
SymCounter.o: SymCounter.hpp Signature.hpp Int.hpp /usr/include/limits.h
SymCounter.o: /usr/include/features.h /usr/include/sys/cdefs.h
SymCounter.o: /usr/include/gnu/stubs.h /usr/include/limits.h
SymCounter.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
SymCounter.o: List.hpp assert.hpp /usr/include/stdlib.h
SymCounter.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
SymCounter.o: Tracer.hpp Memory.hpp Unit.hpp Chain.hpp Sort.hpp Clause.hpp
SymCounter.o: Literal.hpp Atom.hpp Term.hpp Var.hpp Lst.hpp
SymCounter.o: ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp
SymCounter.o: Formula.hpp Problem.hpp
Term.o: Term.hpp Signature.hpp Int.hpp /usr/include/limits.h
Term.o: /usr/include/features.h /usr/include/sys/cdefs.h
Term.o: /usr/include/gnu/stubs.h /usr/include/limits.h
Term.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Term.o: assert.hpp /usr/include/stdlib.h
Term.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Tracer.hpp
Term.o: Memory.hpp Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
Term.o: ./BlodKorv/jargon.hpp Sort.hpp Substitution.hpp
TPTPLexer.o: TPTPLexer.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/config.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/CommonToken.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/Token.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/RefCount.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/InputBuffer.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/CircularQueue.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/BitSet.hpp
TPTPLexer.o: TPTPLexerTokenTypes.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/CharScanner.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStream.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/RecognitionException.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/ANTLRException.hpp
TPTPLexer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/LexerSharedInputState.hpp
TPTPLexer.o: Unit.hpp Chain.hpp Memory.hpp /usr/include/stdlib.h
TPTPLexer.o: /usr/include/features.h /usr/include/sys/cdefs.h
TPTPLexer.o: /usr/include/gnu/stubs.h
TPTPLexer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
TPTPLexer.o: assert.hpp Tracer.hpp Sort.hpp Clause.hpp Literal.hpp Atom.hpp
TPTPLexer.o: Term.hpp Signature.hpp Int.hpp /usr/include/limits.h
TPTPLexer.o: /usr/include/limits.h
TPTPLexer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
TPTPLexer.o: List.hpp Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
TPTPLexer.o: ./BlodKorv/jargon.hpp Formula.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/CharBuffer.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStreamException.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStreamIOException.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStreamRecognitionException.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/CharStreamException.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/CharStreamIOException.hpp
TPTPLexer.o: /home/voronkov/Vampire/antlr/local/include/antlr/NoViableAltForCharException.hpp
TPTPParser.o: TPTPParser.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/config.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStream.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/Token.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/RefCount.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenBuffer.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/CircularQueue.hpp
TPTPParser.o: TPTPLexerTokenTypes.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/LLkParser.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/Parser.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/BitSet.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/RecognitionException.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/ANTLRException.hpp
TPTPParser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/ASTFactory.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/AST.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/ASTRefCount.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/ASTArray.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/ASTPair.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/ParserSharedInputState.hpp
TPTPParser.o: Unit.hpp Chain.hpp Memory.hpp /usr/include/stdlib.h
TPTPParser.o: /usr/include/features.h /usr/include/sys/cdefs.h
TPTPParser.o: /usr/include/gnu/stubs.h
TPTPParser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
TPTPParser.o: assert.hpp Tracer.hpp Sort.hpp Clause.hpp Literal.hpp Atom.hpp
TPTPParser.o: Term.hpp Signature.hpp Int.hpp /usr/include/limits.h
TPTPParser.o: /usr/include/limits.h
TPTPParser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
TPTPParser.o: List.hpp Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
TPTPParser.o: ./BlodKorv/jargon.hpp Formula.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/NoViableAltException.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/SemanticException.hpp
TPTPParser.o: IntNameTable.hpp Pair.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/TokenStreamSelector.hpp
TPTPParser.o: TPTPLexer.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/CommonToken.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/InputBuffer.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/CharScanner.hpp
TPTPParser.o: /home/voronkov/Vampire/antlr/local/include/antlr/LexerSharedInputState.hpp
TPTPParser.o: Options.hpp Output.hpp
Tracer.o: Tracer.hpp
Unit.o: /usr/include/string.h /usr/include/features.h
Unit.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Unit.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Unit.hpp
Unit.o: Chain.hpp Memory.hpp /usr/include/stdlib.h assert.hpp Tracer.hpp
Unit.o: Sort.hpp Clause.hpp Literal.hpp Atom.hpp Term.hpp Signature.hpp
Unit.o: Int.hpp /usr/include/limits.h /usr/include/limits.h
Unit.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Unit.o: Var.hpp Lst.hpp ./VampireKernel/VampireKernel.hpp
Unit.o: ./BlodKorv/jargon.hpp Formula.hpp
Var.o: Var.hpp Lst.hpp assert.hpp /usr/include/stdlib.h
Var.o: /usr/include/features.h /usr/include/sys/cdefs.h
Var.o: /usr/include/gnu/stubs.h
Var.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Tracer.hpp
Var.o: Substitution.hpp Term.hpp Signature.hpp Int.hpp /usr/include/limits.h
Var.o: /usr/include/limits.h
Var.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h List.hpp
Var.o: Memory.hpp ./VampireKernel/VampireKernel.hpp ./BlodKorv/jargon.hpp

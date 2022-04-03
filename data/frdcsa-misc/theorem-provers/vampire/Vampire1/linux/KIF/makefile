# File:    makefile 
# Author:  Alexandre Riazanov (riazanov@cs.man.ac.uk)
# Created: Jun 23, 2002
# Purpose: makefile for Vampire
################################################################
CC= gcc
PPOPT=-DDEBUG_PREPRO=1 -O4# default options for compiling the preprocessor
# PPOPT=-DDEBUG_PREPRO=0 -O4# default options for compiling the preprocessor
VKOPT=-DDEBUG_ALL -DLARGE_TERMS # default options for compiling the kernel
# VKOPT=-O4 -DNO_DEBUG -DLARGE_TERMS # default options for compiling the kernel

KIF_CURRENT_PATH=$(PWD)
KIF_CLASSPATH=$(KIF_CURRENT_PATH)/antlr-2.7.1
KIF_ANTLR_LOCAL=$(KIF_CLASSPATH)/local
KIF_INCLUDE_DIR=$(KIF_ANTLR_LOCAL)/include
KIF_LIB_DIR=$(KIF_ANTLR_LOCAL)/lib
KIF_CXXFLAGS=-I$(KIF_INCLUDE_DIR) -L$(KIF_LIB_DIR) -Wall -O4
KIF_ANTLR_SRC=$(KIF_CLASSPATH)/lib/cpp

MDOPT= -I/usr/include -I/usr/include/sys\
	-I$(KIF_INCLUDE_DIR) -L$(KIF_LIB_DIR) # default makedepend options
################################################################

PreproObj = atom.o\
            memory.o\
            signature.o\
            clause.o\
            options.o\
            sym_counter.o\
            term.o\
            formula.o\
            prepro.o\
            tracer.o\
            int.o\
            problem.o\
            unit.o\
            assert.o\
            renaming.o\
            substitution.o\
            int_name_table.o\
            property.o\
            literal.o

TPTPObj = AParser.o\
            ATokenBuffer.o\
            DLGLexer.o\
            parser.o\
            DLexerBase.o\
            TPTPParser.o

KIFObj = ./KIFLexer.o\
	./KIFParser.o\
	./XMLLexer.o\
	./XMLParser.o

KernelObj = ./VampireKernel/vkernel.a

################################################################

KIFLexer.hpp KIFLexer.cpp KIFParser.hpp KIFParser.cpp: KIF.g 
	export CLASSPATH=$(KIF_CLASSPATH);java antlr.Tool KIF.g

XMLLexer.hpp XMLLexer.cpp XMLParser.hpp XMLParser.cpp: XML.g 
	export CLASSPATH=$(KIF_CLASSPATH);java antlr.Tool XML.g

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

default: vampire

remake: clean vampire 

vampire: ${PreproObj} ${TPTPObj} kernel vampire.o schedule.o casc.o
	${CC} -static ${PreproObj} ${TPTPObj} ${PPOPT} ${VKOPT} -I./VampireKernel -I./BlodKorv vampire.o schedule.o casc.o ./VampireKernel/vkernel.a -o vampire
	strip vampire

kif: ${PreproObj} ${KIFObj} kernel kif.o query.o
	${CC} -static ${PreproObj} ${PPOPT} ${VKOPT} -I./VampireKernel -I./BlodKorv kif.o query.o ./VampireKernel/vkernel.a -L${KIF_LIB_DIR} ${KIFObj} -lantlr -o kif
	strip kif

prepro: ${PreproObj} 
	${CC} -static ${PPOPT} ${PreproObj} clausifier.cpp -o prepro

kernel:
	cd ./VampireKernel ; ${MAKE} vkernel.a OPT='${VKOPT}'

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

AParser.o: pcctscfg.h pccts_stdio.h /usr/include/stdio.h
AParser.o: /usr/include/features.h /usr/include/sys/cdefs.h
AParser.o: /usr/include/gnu/stubs.h
AParser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
AParser.o: /usr/include/bits/types.h /usr/include/libio.h
AParser.o: /usr/include/_G_config.h /usr/include/wchar.h
AParser.o: /usr/include/bits/wchar.h /usr/include/gconv.h
AParser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
AParser.o: /usr/include/bits/stdio_lim.h pccts_stdlib.h /usr/include/stdlib.h
AParser.o: pccts_stdarg.h pccts_string.h /usr/include/string.h AToken.h
AParser.o: ATokenBuffer.h ATokenStream.h AParser.h pccts_setjmp.h
AParser.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
AParser.o: /usr/include/bits/sigset.h ATokenBuffer.h
assert.o: assert.h /usr/include/stdlib.h /usr/include/features.h
assert.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
assert.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ATokenBuffer.o: pcctscfg.h pccts_stdio.h /usr/include/stdio.h
ATokenBuffer.o: /usr/include/features.h /usr/include/sys/cdefs.h
ATokenBuffer.o: /usr/include/gnu/stubs.h
ATokenBuffer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ATokenBuffer.o: /usr/include/bits/types.h /usr/include/libio.h
ATokenBuffer.o: /usr/include/_G_config.h /usr/include/wchar.h
ATokenBuffer.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ATokenBuffer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
ATokenBuffer.o: /usr/include/bits/stdio_lim.h pccts_stdlib.h
ATokenBuffer.o: /usr/include/stdlib.h ATokenBuffer.h ATokPtr.cpp ATokPtr.h
ATokPtr.o: pcctscfg.h pccts_stdio.h /usr/include/stdio.h
ATokPtr.o: /usr/include/features.h /usr/include/sys/cdefs.h
ATokPtr.o: /usr/include/gnu/stubs.h
ATokPtr.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ATokPtr.o: /usr/include/bits/types.h /usr/include/libio.h
ATokPtr.o: /usr/include/_G_config.h /usr/include/wchar.h
ATokPtr.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ATokPtr.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
ATokPtr.o: /usr/include/bits/stdio_lim.h pccts_stdlib.h /usr/include/stdlib.h
ATokPtr.o: ATokPtr.h
atom.o: tracer.h term.h typedefs.h list.h debug.h assert.h
atom.o: /usr/include/stdlib.h /usr/include/features.h
atom.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
atom.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
atom.o: int.h /usr/include/limits.h /usr/include/limits.h
atom.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h signature.h
atom.o: clist.h atom.h options.h
bittable.o: bittable.h assert.h /usr/include/stdlib.h /usr/include/features.h
bittable.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
bittable.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h debug.h
casc.o: /usr/include/string.h /usr/include/features.h
casc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
casc.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
casc.o: /usr/include/stdlib.h /usr/include/ctype.h /usr/include/bits/types.h
casc.o: /usr/include/endian.h /usr/include/bits/endian.h casc.h
clause.o: tracer.h clause.h typedefs.h list.h debug.h assert.h
clause.o: /usr/include/stdlib.h /usr/include/features.h
clause.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
clause.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
clause.o: literal.h atom.h int.h /usr/include/limits.h /usr/include/limits.h
clause.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h term.h
clause.o: signature.h clist.h sort.h
clausifier.o: /usr/include/string.h /usr/include/features.h
clausifier.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
clausifier.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
clausifier.o: /usr/include/stdlib.h /usr/include/stdio.h
clausifier.o: /usr/include/bits/types.h /usr/include/libio.h
clausifier.o: /usr/include/_G_config.h /usr/include/wchar.h
clausifier.o: /usr/include/bits/wchar.h /usr/include/gconv.h
clausifier.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
clausifier.o: /usr/include/bits/stdio_lim.h pcctscfg.h pccts_stdio.h
clausifier.o: pccts_stdlib.h tokens.h AParser.h pccts_setjmp.h
clausifier.o: /usr/include/setjmp.h /usr/include/bits/setjmp.h
clausifier.o: /usr/include/bits/sigset.h AToken.h pccts_string.h
clausifier.o: ATokenBuffer.h TPTPParser.h AParser.h term.h typedefs.h list.h
clausifier.o: debug.h assert.h memory.h int.h /usr/include/limits.h
clausifier.o: /usr/include/limits.h
clausifier.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
clausifier.o: signature.h clist.h literal.h atom.h clause.h formula.h
clausifier.o: tracer.h int_name_table.h pair.h unit.h chain.h sort.h
clausifier.o: problem.h DLGLexer.h DLexerBase.h ATokenStream.h options.h
DLexerBase.o: pcctscfg.h pccts_stdio.h /usr/include/stdio.h
DLexerBase.o: /usr/include/features.h /usr/include/sys/cdefs.h
DLexerBase.o: /usr/include/gnu/stubs.h
DLexerBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
DLexerBase.o: /usr/include/bits/types.h /usr/include/libio.h
DLexerBase.o: /usr/include/_G_config.h /usr/include/wchar.h
DLexerBase.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DLexerBase.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
DLexerBase.o: /usr/include/bits/stdio_lim.h pccts_stdlib.h
DLexerBase.o: /usr/include/stdlib.h DLexerBase.h AToken.h pccts_string.h
DLexerBase.o: /usr/include/string.h ATokenStream.h
DLGLexer.o: tokens.h AToken.h pcctscfg.h pccts_stdio.h /usr/include/stdio.h
DLGLexer.o: /usr/include/features.h /usr/include/sys/cdefs.h
DLGLexer.o: /usr/include/gnu/stubs.h
DLGLexer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
DLGLexer.o: /usr/include/bits/types.h /usr/include/libio.h
DLGLexer.o: /usr/include/_G_config.h /usr/include/wchar.h
DLGLexer.o: /usr/include/bits/wchar.h /usr/include/gconv.h
DLGLexer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
DLGLexer.o: /usr/include/bits/stdio_lim.h pccts_stdlib.h
DLGLexer.o: /usr/include/stdlib.h pccts_string.h /usr/include/string.h term.h
DLGLexer.o: typedefs.h list.h debug.h assert.h memory.h int.h
DLGLexer.o: /usr/include/limits.h /usr/include/limits.h
DLGLexer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
DLGLexer.o: signature.h clist.h literal.h atom.h clause.h formula.h tracer.h
DLGLexer.o: int_name_table.h pair.h unit.h chain.h sort.h problem.h AParser.h
DLGLexer.o: DLexerBase.h ATokenStream.h DLGLexer.h DLexerBase.h DLexer.cpp
formula.o: /usr/include/string.h /usr/include/features.h
formula.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
formula.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h term.h
formula.o: typedefs.h list.h debug.h assert.h /usr/include/stdlib.h memory.h
formula.o: int.h /usr/include/limits.h /usr/include/limits.h
formula.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
formula.o: signature.h clist.h atom.h tracer.h formula.h clause.h literal.h
formula.o: sort.h substitution.h pair.h
fsymbol.o: /usr/include/string.h /usr/include/features.h
fsymbol.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fsymbol.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
fsymbol.o: signature.h int.h /usr/include/limits.h /usr/include/limits.h
fsymbol.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h list.h
fsymbol.o: debug.h assert.h /usr/include/stdlib.h memory.h
int.o: int.h /usr/include/limits.h /usr/include/features.h
int.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
int.o: /usr/include/limits.h
int.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h list.h
int.o: debug.h assert.h /usr/include/stdlib.h
int.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
int_name_table.o: /usr/include/string.h /usr/include/features.h
int_name_table.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
int_name_table.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
int_name_table.o: int_name_table.h list.h debug.h assert.h
int_name_table.o: /usr/include/stdlib.h memory.h pair.h
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
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/RecognitionException.hpp
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/config.hpp
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/ANTLRException.hpp
kif.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/CharStreamException.hpp
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/TokenStreamRecognitionException.hpp
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/TokenStreamException.hpp
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/TokenStreamSelector.hpp
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/TokenStream.hpp
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/Token.hpp
kif.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/RefCount.hpp
kif.o: ./VampireKernel/VampireKernel.h ./BlodKorv/jargon.h
kif.o: ./BlodKorv/target.h ./BlodKorv/compiler.h ./BlodKorv/Exit.h
kif.o: ./BlodKorv/GlobAlloc.h ./BlodKorv/BlodKorvDebugFlags.h
kif.o: ./BlodKorv/ClassDesc.h /usr/include/limits.h /usr/include/limits.h
kif.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
kif.o: ./BlodKorv/Debug.h ./BlodKorv/ObjDesc.h
kif.o: ./BlodKorv/AllocationFailureHandling.h ./BlodKorv/DestructionMode.h
kif.o: ./BlodKorv/RuntimeError.h ./BlodKorv/debugMacros.h
kif.o: ./BlodKorv/Allocator.h ./BlodKorv/BufferedAllocationStatistics.h
kif.o: ./VampireKernel/Input.h ./VampireKernel/VampireKernelDebugFlags.h
kif.o: ./BlodKorv/GList.h ./BlodKorv/Malloc.h ./BlodKorv/GlobalClock.h
kif.o: options.h tracer.h assert.h renaming.h typedefs.h list.h debug.h
kif.o: memory.h term.h int.h signature.h clist.h pair.h problem.h unit.h
kif.o: chain.h sort.h clause.h literal.h atom.h formula.h int_name_table.h
kif.o: query.h
literal.o: int.h /usr/include/limits.h /usr/include/features.h
literal.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
literal.o: /usr/include/limits.h
literal.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h list.h
literal.o: debug.h assert.h /usr/include/stdlib.h
literal.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
literal.o: literal.h typedefs.h atom.h term.h signature.h clist.h
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/RecognitionException.hpp
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/config.hpp
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/ANTLRException.hpp
main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/exception
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/CharStreamException.hpp
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/TokenStreamRecognitionException.hpp
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/TokenStreamException.hpp
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/TokenStreamSelector.hpp
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/TokenStream.hpp
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/Token.hpp
main.o: /home/voronkov/KIF/antlr-2.7.1/local/include/antlr/RefCount.hpp
memory.o: memory.h debug.h /usr/include/stdlib.h /usr/include/features.h
memory.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
memory.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h assert.h
options.o: /usr/include/string.h /usr/include/features.h
options.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
options.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
options.o: /usr/include/stdlib.h /usr/include/limits.h /usr/include/limits.h
options.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h assert.h
options.o: options.h tracer.h
parser.o: pcctscfg.h pccts_stdio.h /usr/include/stdio.h
parser.o: /usr/include/features.h /usr/include/sys/cdefs.h
parser.o: /usr/include/gnu/stubs.h
parser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
parser.o: /usr/include/bits/types.h /usr/include/libio.h
parser.o: /usr/include/_G_config.h /usr/include/wchar.h
parser.o: /usr/include/bits/wchar.h /usr/include/gconv.h
parser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
parser.o: /usr/include/bits/stdio_lim.h pccts_stdlib.h /usr/include/stdlib.h
parser.o: tokens.h term.h typedefs.h list.h debug.h assert.h memory.h int.h
parser.o: /usr/include/limits.h /usr/include/limits.h
parser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
parser.o: signature.h clist.h literal.h atom.h clause.h formula.h tracer.h
parser.o: int_name_table.h pair.h unit.h /usr/include/string.h chain.h sort.h
parser.o: problem.h AParser.h TPTPParser.h DLexerBase.h AToken.h
parser.o: pccts_string.h ATokenStream.h ATokPtr.h DLexerBase.h DLGLexer.h
parser.o: DLexerBase.h
prepro.o: prepro.h problem.h typedefs.h list.h debug.h assert.h
prepro.o: /usr/include/stdlib.h /usr/include/features.h
prepro.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
prepro.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
prepro.o: unit.h /usr/include/string.h int.h /usr/include/limits.h
prepro.o: /usr/include/limits.h
prepro.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
prepro.o: signature.h chain.h sort.h clause.h literal.h atom.h term.h clist.h
prepro.o: formula.h tracer.h options.h
problem.o: /usr/include/string.h /usr/include/features.h
problem.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
problem.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
problem.o: /usr/include/limits.h /usr/include/limits.h
problem.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
problem.o: /usr/include/stdlib.h tracer.h clause.h typedefs.h list.h debug.h
problem.o: assert.h memory.h literal.h atom.h int.h term.h signature.h
problem.o: clist.h unit.h chain.h sort.h problem.h formula.h options.h
problem.o: sym_counter.h
property.o: property.h problem.h typedefs.h list.h debug.h assert.h
property.o: /usr/include/stdlib.h /usr/include/features.h
property.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
property.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
property.o: unit.h /usr/include/string.h int.h /usr/include/limits.h
property.o: /usr/include/limits.h
property.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
property.o: signature.h chain.h sort.h clause.h literal.h atom.h term.h
property.o: clist.h formula.h tracer.h
psymbol.o: /usr/include/string.h /usr/include/features.h
psymbol.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
psymbol.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
psymbol.o: signature.h int.h /usr/include/limits.h /usr/include/limits.h
psymbol.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h list.h
psymbol.o: debug.h assert.h /usr/include/stdlib.h memory.h
pure.o: pure.h debug.h clause.h typedefs.h list.h assert.h
pure.o: /usr/include/stdlib.h /usr/include/features.h
pure.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
pure.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
pure.o: literal.h atom.h int.h /usr/include/limits.h /usr/include/limits.h
pure.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h term.h
pure.o: signature.h clist.h
query.o: /usr/include/stdlib.h /usr/include/features.h
query.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
query.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h query.h
query.o: formula.h typedefs.h list.h debug.h assert.h memory.h term.h int.h
query.o: /usr/include/limits.h /usr/include/limits.h
query.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h signature.h
query.o: clist.h clause.h literal.h atom.h tracer.h
renaming.o: renaming.h typedefs.h list.h debug.h assert.h
renaming.o: /usr/include/stdlib.h /usr/include/features.h
renaming.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
renaming.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
renaming.o: tracer.h term.h int.h /usr/include/limits.h /usr/include/limits.h
renaming.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
renaming.o: signature.h clist.h pair.h
replace_code.o: replace_code.h typedefs.h list.h debug.h assert.h
replace_code.o: /usr/include/stdlib.h /usr/include/features.h
replace_code.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
replace_code.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
replace_code.o: memory.h term.h int.h /usr/include/limits.h
replace_code.o: /usr/include/limits.h
replace_code.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
replace_code.o: signature.h clist.h literal.h atom.h clause.h
resolvable.o: resolvable.h debug.h clause.h typedefs.h list.h assert.h
resolvable.o: /usr/include/stdlib.h /usr/include/features.h
resolvable.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
resolvable.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
resolvable.o: memory.h literal.h atom.h int.h /usr/include/limits.h
resolvable.o: /usr/include/limits.h
resolvable.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h term.h
resolvable.o: signature.h clist.h
schedule.o: ./VampireKernel/VampireKernel.h ./BlodKorv/jargon.h
schedule.o: ./BlodKorv/target.h ./BlodKorv/compiler.h ./BlodKorv/GlobAlloc.h
schedule.o: /usr/include/stdlib.h /usr/include/features.h
schedule.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
schedule.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
schedule.o: ./BlodKorv/BlodKorvDebugFlags.h ./BlodKorv/ClassDesc.h
schedule.o: /usr/include/limits.h /usr/include/limits.h
schedule.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
schedule.o: ./BlodKorv/Debug.h /usr/include/time.h /usr/include/bits/time.h
schedule.o: /usr/include/bits/types.h ./BlodKorv/ObjDesc.h
schedule.o: ./BlodKorv/AllocationFailureHandling.h ./BlodKorv/Exit.h
schedule.o: ./BlodKorv/DestructionMode.h ./BlodKorv/RuntimeError.h
schedule.o: ./BlodKorv/debugMacros.h ./BlodKorv/Allocator.h
schedule.o: ./BlodKorv/BufferedAllocationStatistics.h property.h problem.h
schedule.o: typedefs.h list.h debug.h assert.h memory.h unit.h
schedule.o: /usr/include/string.h int.h signature.h chain.h sort.h clause.h
schedule.o: literal.h atom.h term.h clist.h formula.h tracer.h options.h
schedule.o: schedule.h ./BlodKorv/GlobalClock.h vampire.h casc.h
signature.o: /usr/include/string.h /usr/include/features.h
signature.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
signature.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
signature.o: /usr/include/stdio.h /usr/include/bits/types.h
signature.o: /usr/include/libio.h /usr/include/_G_config.h
signature.o: /usr/include/wchar.h /usr/include/bits/wchar.h
signature.o: /usr/include/gconv.h
signature.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
signature.o: /usr/include/bits/stdio_lim.h tracer.h debug.h signature.h int.h
signature.o: /usr/include/limits.h /usr/include/limits.h
signature.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h list.h
signature.o: assert.h /usr/include/stdlib.h memory.h sym_counter.h atom.h
signature.o: typedefs.h term.h clist.h
strategy.o: /usr/include/string.h /usr/include/features.h
strategy.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
strategy.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
strategy.o: strategy.h list.h debug.h assert.h /usr/include/stdlib.h memory.h
strategy.o: property.h prepro.h options.h int.h /usr/include/limits.h
strategy.o: /usr/include/limits.h
strategy.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
string.o: /usr/include/string.h /usr/include/features.h
string.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
string.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
substitution.o: substitution.h typedefs.h list.h debug.h assert.h
substitution.o: /usr/include/stdlib.h /usr/include/features.h
substitution.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
substitution.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
substitution.o: memory.h tracer.h term.h int.h /usr/include/limits.h
substitution.o: /usr/include/limits.h
substitution.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
substitution.o: signature.h clist.h pair.h
sym_counter.o: formula.h typedefs.h list.h debug.h assert.h
sym_counter.o: /usr/include/stdlib.h /usr/include/features.h
sym_counter.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
sym_counter.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
sym_counter.o: memory.h term.h int.h /usr/include/limits.h
sym_counter.o: /usr/include/limits.h
sym_counter.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
sym_counter.o: signature.h clist.h clause.h literal.h atom.h tracer.h
sym_counter.o: sym_counter.h unit.h /usr/include/string.h chain.h sort.h
sym_counter.o: problem.h
term.o: /usr/include/string.h /usr/include/features.h
term.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
term.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h tracer.h
term.o: debug.h term.h typedefs.h list.h assert.h /usr/include/stdlib.h
term.o: memory.h int.h /usr/include/limits.h /usr/include/limits.h
term.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h signature.h
term.o: clist.h sort.h substitution.h pair.h
TPTPParser.o: pcctscfg.h pccts_stdio.h /usr/include/stdio.h
TPTPParser.o: /usr/include/features.h /usr/include/sys/cdefs.h
TPTPParser.o: /usr/include/gnu/stubs.h
TPTPParser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
TPTPParser.o: /usr/include/bits/types.h /usr/include/libio.h
TPTPParser.o: /usr/include/_G_config.h /usr/include/wchar.h
TPTPParser.o: /usr/include/bits/wchar.h /usr/include/gconv.h
TPTPParser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
TPTPParser.o: /usr/include/bits/stdio_lim.h pccts_stdlib.h
TPTPParser.o: /usr/include/stdlib.h tokens.h TPTPParser.h AParser.h term.h
TPTPParser.o: typedefs.h list.h debug.h assert.h memory.h int.h
TPTPParser.o: /usr/include/limits.h /usr/include/limits.h
TPTPParser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
TPTPParser.o: signature.h clist.h literal.h atom.h clause.h formula.h
TPTPParser.o: tracer.h int_name_table.h pair.h unit.h /usr/include/string.h
TPTPParser.o: chain.h sort.h problem.h
tracer.o: tracer.h
unit.o: /usr/include/string.h /usr/include/features.h
unit.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
unit.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h debug.h
unit.o: tracer.h unit.h typedefs.h list.h assert.h /usr/include/stdlib.h
unit.o: memory.h int.h /usr/include/limits.h /usr/include/limits.h
unit.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h signature.h
unit.o: chain.h sort.h clause.h literal.h atom.h term.h clist.h formula.h
vampire.o: /usr/include/string.h /usr/include/features.h
vampire.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
vampire.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
vampire.o: /usr/include/stdlib.h /usr/include/stdio.h
vampire.o: /usr/include/bits/types.h /usr/include/libio.h
vampire.o: /usr/include/_G_config.h /usr/include/wchar.h
vampire.o: /usr/include/bits/wchar.h /usr/include/gconv.h
vampire.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
vampire.o: /usr/include/bits/stdio_lim.h /usr/include/signal.h
vampire.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
vampire.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/unistd.h
vampire.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
vampire.o: pcctscfg.h pccts_stdio.h pccts_stdlib.h tokens.h AParser.h
vampire.o: TPTPParser.h term.h typedefs.h list.h debug.h assert.h memory.h
vampire.o: int.h /usr/include/limits.h /usr/include/limits.h
vampire.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
vampire.o: signature.h clist.h literal.h atom.h clause.h formula.h tracer.h
vampire.o: int_name_table.h pair.h unit.h chain.h sort.h problem.h DLGLexer.h
vampire.o: DLexerBase.h AToken.h pccts_string.h ATokenStream.h
vampire.o: ./VampireKernel/Tabulation.h ./BlodKorv/jargon.h
vampire.o: ./BlodKorv/target.h ./BlodKorv/compiler.h ./BlodKorv/GlobAlloc.h
vampire.o: ./BlodKorv/BlodKorvDebugFlags.h ./BlodKorv/ClassDesc.h
vampire.o: ./BlodKorv/Debug.h ./BlodKorv/ObjDesc.h
vampire.o: ./BlodKorv/AllocationFailureHandling.h ./BlodKorv/Exit.h
vampire.o: ./BlodKorv/DestructionMode.h ./BlodKorv/RuntimeError.h
vampire.o: ./BlodKorv/debugMacros.h ./BlodKorv/Allocator.h
vampire.o: ./BlodKorv/BufferedAllocationStatistics.h
vampire.o: ./VampireKernel/VampireKernelDebugFlags.h ./VampireKernel/Term.h
vampire.o: ./VampireKernel/VampireKernelConst.h ./BlodKorv/Comparison.h
vampire.o: ./BlodKorv/Stack.h ./BlodKorv/ExpandingStack.h ./BlodKorv/Array.h
vampire.o: ./BlodKorv/BitSet.h ./BlodKorv/BitWord.h ./BlodKorv/BitString.h
vampire.o: ./BlodKorv/SmallHeaderSkipList.h ./VampireKernel/TermWeightType.h
vampire.o: ./VampireKernel/VampireKernel.h ./VampireKernel/Signature.h
vampire.o: ./VampireKernel/SymbolInfo.h
vampire.o: ./VampireKernel/AutoFunctorPrecedence.h ./BlodKorv/Tuple.h
vampire.o: ./BlodKorv/GSortTree.h ./BlodKorv/GList.h ./VampireKernel/Clause.h
vampire.o: ./VampireKernel/ClauseBackground.h ./VampireKernel/LiteralList.h
vampire.o: ./VampireKernel/DefinedHeaderTable.h ./VampireKernel/Input.h
vampire.o: ./BlodKorv/Malloc.h ./BlodKorv/GlobalClock.h options.h renaming.h
vampire.o: property.h schedule.h
wizard.o: wizard.h property.h problem.h typedefs.h list.h debug.h assert.h
wizard.o: /usr/include/stdlib.h /usr/include/features.h
wizard.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
wizard.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h memory.h
wizard.o: unit.h /usr/include/string.h int.h /usr/include/limits.h
wizard.o: /usr/include/limits.h
wizard.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
wizard.o: signature.h chain.h sort.h clause.h literal.h atom.h term.h clist.h
wizard.o: formula.h tracer.h prepro.h strategy.h

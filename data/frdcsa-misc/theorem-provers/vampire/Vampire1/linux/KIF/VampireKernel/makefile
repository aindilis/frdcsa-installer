# File:    makefile 
# Author:  Alexandre Riazanov (riazanov@cs.man.ac.uk)
# Created: Feb 8, 2000
# Purpose: makefile for VampireKernel
################################################################

CC= g++ #      	# UNIX GNU compiler 
OPT=-O4 -DNO_DEBUG -DLARGE_TERMS# default compilation options
MDOPT=  # default makedepend options
CFLAGS = -Wall -IPR_PARSER -I../BlodKorv -I../Gematogen -static
######################################################################

Obj = AutoFunctorPrecedence.o\
      ClauseBackground.o\
      binary_resolution.o backward_superposition.o BackDemodulation.o BackParaCandidates.o BackwardSubsumption.o BSIndex.o BSMachine.o\
      BuiltInFloatingPointArithmetic.o BuiltInTheory.o BuiltInTheoryDispatcher.o\
      Clause.o ClauseNumIndex.o ClauseQueue.o ComponentName.o ClauseAssembler.o\
      forward_subsumption.o forward_superposition.o ForwardDemodulation.o fs_code.o fs_command.o\
      VampireKernelInterceptMacros.o\
      InferenceTree.o InferenceDispatcher.o InstanceRetrievalForDemodulation.o iu_command.o iu_machine.o\
      kept.o\
      LiteralList.o LiteralSelection.o LiteralSharing.o Logging.o\
      Master.o Master3.o\
      NewClause.o NonrecursiveKBOrdering.o NonuniformConstantTermWeighting.o\
      old_fs_code.o or_index.o or_premise.o\
      PagedPassiveClauseStorage.o prefix.o premise_factoring.o ProblemProfile.o\
      sharing.o Signature.o SimplificationOrdering.o Splitting.o Splitting3.o StandardKBOrdering.o\
      TermWeighting.o TmpLitList.o tmp_literal.o Term.o\
      UniformTermWeighting.o\
      VampireKernel.o variables.o\
      WeightPolynomial.o weight_formula.o\
      WeightPolynomialSpecialisedComparisonCommand.o

BlodKorvObj = ../BlodKorv/BitWord.o\
        ../BlodKorv/ClassDesc.o\
        ../BlodKorv/Debug.o\
        ../BlodKorv/DestructionMode.o\
        ../BlodKorv/Exit.o\
	../BlodKorv/FormatText.o\
        ../BlodKorv/GlobalClock.o\
        ../BlodKorv/GlobAlloc.o\
	../BlodKorv/GlobalStopFlag.o\
        ../BlodKorv/Label.o\
        ../BlodKorv/Malloc.o\
        ../BlodKorv/RuntimeError.o
PPObj = PR_PARSER/pr_parser.o
 
################################################################

default: standalone

vkernel.a: ${Obj} blodKorv
	ar -rs vkernel.a ${Obj} ${BlodKorvObj}

standalone: blodKorv vkernel.a prParser Standalone.o Parser.o Input.o
	${CC} ${OPT} ${CFLAGS} ../BlodKorv/CommandLine.o Standalone.o  ${PPObj}  Parser.o Input.o vkernel.a -o vkernel
	strip -s vkernel

.SUFFIXES:
.SUFFIXES: .cpp .o

.cpp.o:
	${CC} ${OPT} ${CFLAGS} -c $*.cpp


blodKorv:
	cd ../BlodKorv ; ${MAKE} OPT='${OPT}' ; cd ../VampireKernel
	 
	
prParser:
	cd PR_PARSER ; ${MAKE} ; cd ..

remake: clean standalone

clean: 
	rm -f *.o ; rm -f *~ ; rm -f *.bak ; rm -f *.tmp ; rm -f vkernel ; rm -f vkernel.a ; rm -f core
	cd ../BlodKorv ; ${MAKE} clean ; cd ../VampireKernel
	cd PR_PARSER ; ${MAKE} clean ; cd ..

depend:
	makedepend ${MDOPT} -I../BlodKorv -IPR_PARSER -I../Dummies -I../Gematogen *.cpp 
	cd ../BlodKorv;  makedepend ${MDOPT} -I../Dummies *.cpp ; cd ../VampireKernel
	

#############################################################################
# DO NOT DELETE

AutoFunctorPrecedence.o: AutoFunctorPrecedence.h ../BlodKorv/jargon.h
AutoFunctorPrecedence.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
AutoFunctorPrecedence.o: VampireKernelDebugFlags.h
AutoFunctorPrecedence.o: ../BlodKorv/BlodKorvDebugFlags.h
AutoFunctorPrecedence.o: VampireKernelConst.h /usr/include/limits.h
AutoFunctorPrecedence.o: /usr/include/features.h /usr/include/sys/cdefs.h
AutoFunctorPrecedence.o: /usr/include/gnu/stubs.h
AutoFunctorPrecedence.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
AutoFunctorPrecedence.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
AutoFunctorPrecedence.o: ../BlodKorv/Array.h /usr/include/stdlib.h
AutoFunctorPrecedence.o: ../BlodKorv/DestructionMode.h
AutoFunctorPrecedence.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
AutoFunctorPrecedence.o: /usr/include/time.h /usr/include/bits/time.h
AutoFunctorPrecedence.o: /usr/include/bits/types.h ../BlodKorv/GlobAlloc.h
AutoFunctorPrecedence.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
AutoFunctorPrecedence.o: ../BlodKorv/AllocationFailureHandling.h
AutoFunctorPrecedence.o: ../BlodKorv/Exit.h ../BlodKorv/RuntimeError.h
AutoFunctorPrecedence.o: ../BlodKorv/Allocator.h
AutoFunctorPrecedence.o: ../BlodKorv/BufferedAllocationStatistics.h
AutoFunctorPrecedence.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
AutoFunctorPrecedence.o: ../BlodKorv/GList.h
BackDemodulation.o: BackDemodulation.h ../BlodKorv/jargon.h
BackDemodulation.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
BackDemodulation.o: VampireKernelDebugFlags.h
BackDemodulation.o: ../BlodKorv/BlodKorvDebugFlags.h
BackDemodulation.o: ../BlodKorv/ExpandingMultiset.h
BackDemodulation.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
BackDemodulation.o: /usr/include/stdlib.h /usr/include/features.h
BackDemodulation.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BackDemodulation.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
BackDemodulation.o: ../BlodKorv/DestructionMode.h ../BlodKorv/debugMacros.h
BackDemodulation.o: ../BlodKorv/Debug.h /usr/include/time.h
BackDemodulation.o: /usr/include/bits/time.h /usr/include/bits/types.h Term.h
BackDemodulation.o: VampireKernelConst.h /usr/include/limits.h
BackDemodulation.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
BackDemodulation.o: ../BlodKorv/GlobAlloc.h ../BlodKorv/ClassDesc.h
BackDemodulation.o: ../BlodKorv/ObjDesc.h
BackDemodulation.o: ../BlodKorv/AllocationFailureHandling.h
BackDemodulation.o: ../BlodKorv/Exit.h ../BlodKorv/RuntimeError.h
BackDemodulation.o: ../BlodKorv/Allocator.h
BackDemodulation.o: ../BlodKorv/BufferedAllocationStatistics.h
BackDemodulation.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
BackDemodulation.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
BackDemodulation.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
BackDemodulation.o: TermWeightType.h VampireKernel.h Signature.h
BackDemodulation.o: /usr/include/string.h SymbolInfo.h
BackDemodulation.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
BackDemodulation.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h NewClause.h
BackDemodulation.o: prefix.h variables.h ../BlodKorv/MultisetOfVariables.h
BackDemodulation.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
BackDemodulation.o: ../BlodKorv/LinearCombination.h
BackDemodulation.o: ../BlodKorv/SharedLinearPolynomial.h
BackDemodulation.o: ../BlodKorv/GDiscTree.h OpenNewClause.h Flatterm.h
BackDemodulation.o: tmp_literal.h ../BlodKorv/Multiset.h TermWeighting.h
BackDemodulation.o: InstanceRetrievalForDemodulation.h
BackDemodulation.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
BackDemodulation.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
BackDemodulation.o: ../Gematogen/GematogenDebugFlags.h
BackDemodulation.o: ../Gematogen/OptimisedPathIndex.h
BackDemodulation.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
BackDemodulation.o: TmpLitList.h ClauseBackground.h ForwardDemodulation.h
BackDemodulation.o: VampireKernelInterceptMacros.h
BackDemodulation.o: ../Gematogen/ForwardMatchingIndexTree.h
BackDemodulation.o: ../BlodKorv/EquivalenceRelation.h
BackDemodulation.o: ../Gematogen/ForwardMatchingIndex.h
BackDemodulation.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h
BackDemodulation.o: ../BlodKorv/FIFO.h Clause.h LiteralList.h
BackDemodulation.o: DefinedHeaderTable.h PagedPassiveClauseStorage.h
BackDemodulation.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h
BackDemodulation.o: DivIntoComp.h ClauseNumIndex.h ../BlodKorv/SkipList.h
BackParaCandidates.o: BackParaCandidates.h ../BlodKorv/jargon.h
BackParaCandidates.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
BackParaCandidates.o: VampireKernelDebugFlags.h
BackParaCandidates.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
BackParaCandidates.o: /usr/include/limits.h /usr/include/features.h
BackParaCandidates.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BackParaCandidates.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
BackParaCandidates.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
BackParaCandidates.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
BackParaCandidates.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
BackParaCandidates.o: /usr/include/time.h /usr/include/bits/time.h
BackParaCandidates.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
BackParaCandidates.o: ../BlodKorv/AllocationFailureHandling.h
BackParaCandidates.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
BackParaCandidates.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
BackParaCandidates.o: ../BlodKorv/Allocator.h
BackParaCandidates.o: ../BlodKorv/BufferedAllocationStatistics.h
BackParaCandidates.o: ../BlodKorv/SkipList.h ../BlodKorv/GList.h
BackParaCandidates.o: ../BlodKorv/Tuple.h SimplificationOrdering.h
BackParaCandidates.o: ../BlodKorv/Comparison.h WeightPolynomial.h
BackParaCandidates.o: ../BlodKorv/LinearPolynomial.h
BackParaCandidates.o: ../BlodKorv/LinearCombination.h ../BlodKorv/Stack.h
BackParaCandidates.o: ../BlodKorv/SharedLinearPolynomial.h
BackParaCandidates.o: ../BlodKorv/GDiscTree.h TermWeightType.h
BackParaCandidates.o: VampireKernel.h Term.h ../BlodKorv/ExpandingStack.h
BackParaCandidates.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
BackParaCandidates.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
BackParaCandidates.o: ../BlodKorv/SmallHeaderSkipList.h Signature.h
BackParaCandidates.o: /usr/include/string.h SymbolInfo.h
BackParaCandidates.o: AutoFunctorPrecedence.h ../BlodKorv/GSortTree.h
BackParaCandidates.o: InstanceRetrievalForDemodulation.h
BackParaCandidates.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
BackParaCandidates.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
BackParaCandidates.o: ../Gematogen/GematogenDebugFlags.h
BackParaCandidates.o: ../Gematogen/OptimisedPathIndex.h
BackParaCandidates.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
BackwardSubsumption.o: BackwardSubsumption.h ../BlodKorv/jargon.h
BackwardSubsumption.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
BackwardSubsumption.o: VampireKernelDebugFlags.h
BackwardSubsumption.o: ../BlodKorv/BlodKorvDebugFlags.h
BackwardSubsumption.o: VampireKernelInterceptMacros.h ../BlodKorv/Exit.h
BackwardSubsumption.o: /usr/include/stdlib.h /usr/include/features.h
BackwardSubsumption.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BackwardSubsumption.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
BackwardSubsumption.o: Signature.h /usr/include/string.h VampireKernelConst.h
BackwardSubsumption.o: /usr/include/limits.h
BackwardSubsumption.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
BackwardSubsumption.o: ../BlodKorv/Array.h ../BlodKorv/DestructionMode.h
BackwardSubsumption.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
BackwardSubsumption.o: /usr/include/time.h /usr/include/bits/time.h
BackwardSubsumption.o: /usr/include/bits/types.h ../BlodKorv/GlobAlloc.h
BackwardSubsumption.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
BackwardSubsumption.o: ../BlodKorv/AllocationFailureHandling.h
BackwardSubsumption.o: ../BlodKorv/RuntimeError.h ../BlodKorv/Allocator.h
BackwardSubsumption.o: ../BlodKorv/BufferedAllocationStatistics.h
BackwardSubsumption.o: ../BlodKorv/Comparison.h SymbolInfo.h
BackwardSubsumption.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
BackwardSubsumption.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
BackwardSubsumption.o: TermWeightType.h VampireKernel.h BSIndex.h Term.h
BackwardSubsumption.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
BackwardSubsumption.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
BackwardSubsumption.o: ../BlodKorv/BitString.h
BackwardSubsumption.o: ../BlodKorv/SmallHeaderSkipList.h
BackwardSubsumption.o: ../Gematogen/PathIndex.h
BackwardSubsumption.o: ../Gematogen/GematogenDebugFlags.h BSMachine.h
BackwardSubsumption.o: ../BlodKorv/GlobalStopFlag.h
BackwardSubsumption.o: ../BlodKorv/GIncCountingSort.h Clause.h
BackwardSubsumption.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h
BackwardSubsumption.o: ../BlodKorv/StaticCellArray.h SubsumptionCheck.h
BackwardSubsumption.o: SimpleSubstitution.h ClauseNumIndex.h
BackwardSubsumption.o: ../BlodKorv/SkipList.h
backward_superposition.o: backward_superposition.h ../BlodKorv/jargon.h
backward_superposition.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
backward_superposition.o: VampireKernelDebugFlags.h
backward_superposition.o: ../BlodKorv/BlodKorvDebugFlags.h
backward_superposition.o: VampireKernelConst.h /usr/include/limits.h
backward_superposition.o: /usr/include/features.h /usr/include/sys/cdefs.h
backward_superposition.o: /usr/include/gnu/stubs.h
backward_superposition.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
backward_superposition.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
backward_superposition.o: ../BlodKorv/DestructionMode.h para_machine.h
backward_superposition.o: iu_command.h prefix.h Term.h
backward_superposition.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
backward_superposition.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
backward_superposition.o: /usr/include/time.h /usr/include/bits/time.h
backward_superposition.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
backward_superposition.o: ../BlodKorv/AllocationFailureHandling.h
backward_superposition.o: ../BlodKorv/Exit.h ../BlodKorv/RuntimeError.h
backward_superposition.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
backward_superposition.o: ../BlodKorv/BufferedAllocationStatistics.h
backward_superposition.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
backward_superposition.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
backward_superposition.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
backward_superposition.o: ../BlodKorv/BitString.h
backward_superposition.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
backward_superposition.o: VampireKernel.h Signature.h /usr/include/string.h
backward_superposition.o: SymbolInfo.h AutoFunctorPrecedence.h
backward_superposition.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
backward_superposition.o: ../BlodKorv/GList.h variables.h
backward_superposition.o: ../BlodKorv/MultisetOfVariables.h
backward_superposition.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
backward_superposition.o: ../BlodKorv/LinearCombination.h
backward_superposition.o: ../BlodKorv/SharedLinearPolynomial.h
backward_superposition.o: ../BlodKorv/GDiscTree.h iu_machine.h
backward_superposition.o: iu_translator.h or_index.h unification.h
backward_superposition.o: BackParaCandidates.h ../BlodKorv/SkipList.h
backward_superposition.o: SimplificationOrdering.h
backward_superposition.o: InstanceRetrievalForDemodulation.h
backward_superposition.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
backward_superposition.o: ../BlodKorv/GlobalStopFlag.h
backward_superposition.o: ../Gematogen/Gematogen.h
backward_superposition.o: ../Gematogen/GematogenDebugFlags.h
backward_superposition.o: ../Gematogen/OptimisedPathIndex.h
backward_superposition.o: ../BlodKorv/GIncCountingSort.h
backward_superposition.o: ../BlodKorv/Multiset.h NewClause.h OpenNewClause.h
backward_superposition.o: Flatterm.h tmp_literal.h TermWeighting.h
backward_superposition.o: TmpLitList.h ClauseBackground.h
backward_superposition.o: ForwardDemodulation.h
backward_superposition.o: VampireKernelInterceptMacros.h
backward_superposition.o: ../Gematogen/ForwardMatchingIndexTree.h
backward_superposition.o: ../BlodKorv/EquivalenceRelation.h
backward_superposition.o: ../Gematogen/ForwardMatchingIndex.h
backward_superposition.o: ../BlodKorv/ExpandingMultiset.h
backward_superposition.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h
backward_superposition.o: ../BlodKorv/FIFO.h Clause.h LiteralList.h
backward_superposition.o: DefinedHeaderTable.h PagedPassiveClauseStorage.h
backward_superposition.o: ../BlodKorv/PageQueueDisciplineStorage.h
backward_superposition.o: Splitting.h DivIntoComp.h weight_formula.h
binary_resolution.o: binary_resolution.h ../BlodKorv/jargon.h
binary_resolution.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
binary_resolution.o: VampireKernelDebugFlags.h
binary_resolution.o: ../BlodKorv/BlodKorvDebugFlags.h premise_factoring.h
binary_resolution.o: or_premise.h Clause.h
binary_resolution.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
binary_resolution.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
binary_resolution.o: /usr/include/features.h /usr/include/sys/cdefs.h
binary_resolution.o: /usr/include/gnu/stubs.h ../BlodKorv/ClassDesc.h
binary_resolution.o: /usr/include/limits.h
binary_resolution.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
binary_resolution.o: ../BlodKorv/Debug.h /usr/include/time.h
binary_resolution.o: /usr/include/bits/time.h /usr/include/bits/types.h
binary_resolution.o: ../BlodKorv/ObjDesc.h
binary_resolution.o: ../BlodKorv/AllocationFailureHandling.h
binary_resolution.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
binary_resolution.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
binary_resolution.o: ../BlodKorv/Allocator.h
binary_resolution.o: ../BlodKorv/BufferedAllocationStatistics.h
binary_resolution.o: ../BlodKorv/GList.h ../BlodKorv/BitWord.h Term.h
binary_resolution.o: VampireKernelConst.h ../BlodKorv/Comparison.h
binary_resolution.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
binary_resolution.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
binary_resolution.o: ../BlodKorv/BitString.h
binary_resolution.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
binary_resolution.o: VampireKernel.h Signature.h /usr/include/string.h
binary_resolution.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
binary_resolution.o: ../BlodKorv/GSortTree.h ClauseBackground.h LiteralList.h
binary_resolution.o: DefinedHeaderTable.h prefix.h variables.h
binary_resolution.o: ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
binary_resolution.o: ../BlodKorv/LinearPolynomial.h
binary_resolution.o: ../BlodKorv/LinearCombination.h
binary_resolution.o: ../BlodKorv/SharedLinearPolynomial.h
binary_resolution.o: ../BlodKorv/GDiscTree.h or_index.h unification.h
binary_resolution.o: iu_command.h BackParaCandidates.h ../BlodKorv/SkipList.h
binary_resolution.o: SimplificationOrdering.h
binary_resolution.o: InstanceRetrievalForDemodulation.h
binary_resolution.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
binary_resolution.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
binary_resolution.o: ../Gematogen/GematogenDebugFlags.h
binary_resolution.o: ../Gematogen/OptimisedPathIndex.h
binary_resolution.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
binary_resolution.o: NewClause.h OpenNewClause.h Flatterm.h tmp_literal.h
binary_resolution.o: TermWeighting.h TmpLitList.h ForwardDemodulation.h
binary_resolution.o: VampireKernelInterceptMacros.h
binary_resolution.o: ../Gematogen/ForwardMatchingIndexTree.h
binary_resolution.o: ../BlodKorv/EquivalenceRelation.h
binary_resolution.o: ../Gematogen/ForwardMatchingIndex.h
binary_resolution.o: ../BlodKorv/ExpandingMultiset.h
binary_resolution.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h
binary_resolution.o: ../BlodKorv/FIFO.h PagedPassiveClauseStorage.h
binary_resolution.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h
binary_resolution.o: DivIntoComp.h or_machine.h iu_machine.h iu_translator.h
binary_resolution.o: weight_formula.h
BSIndex.o: BSIndex.h ../BlodKorv/jargon.h ../BlodKorv/target.h
BSIndex.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
BSIndex.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
BSIndex.o: /usr/include/limits.h /usr/include/features.h
BSIndex.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BSIndex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
BSIndex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
BSIndex.o: ../BlodKorv/DestructionMode.h Term.h ../BlodKorv/GlobAlloc.h
BSIndex.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
BSIndex.o: /usr/include/time.h /usr/include/bits/time.h
BSIndex.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
BSIndex.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
BSIndex.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
BSIndex.o: ../BlodKorv/Allocator.h ../BlodKorv/BufferedAllocationStatistics.h
BSIndex.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
BSIndex.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
BSIndex.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
BSIndex.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h VampireKernel.h
BSIndex.o: Signature.h /usr/include/string.h SymbolInfo.h
BSIndex.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
BSIndex.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
BSIndex.o: ../Gematogen/PathIndex.h ../Gematogen/GematogenDebugFlags.h
BSMachine.o: BSMachine.h ../BlodKorv/jargon.h ../BlodKorv/target.h
BSMachine.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
BSMachine.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
BSMachine.o: /usr/include/limits.h /usr/include/features.h
BSMachine.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BSMachine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
BSMachine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
BSMachine.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
BSMachine.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h /usr/include/time.h
BSMachine.o: /usr/include/bits/time.h /usr/include/bits/types.h
BSMachine.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
BSMachine.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
BSMachine.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
BSMachine.o: ../BlodKorv/Allocator.h
BSMachine.o: ../BlodKorv/BufferedAllocationStatistics.h
BSMachine.o: ../BlodKorv/GlobalStopFlag.h ../BlodKorv/Array.h
BSMachine.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Stack.h
BSMachine.o: ../BlodKorv/ExpandingStack.h BSIndex.h Term.h
BSMachine.o: ../BlodKorv/Comparison.h ../BlodKorv/BitSet.h
BSMachine.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
BSMachine.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
BSMachine.o: VampireKernel.h Signature.h /usr/include/string.h SymbolInfo.h
BSMachine.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
BSMachine.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
BSMachine.o: ../Gematogen/PathIndex.h ../Gematogen/GematogenDebugFlags.h
BSMachine.o: Clause.h ClauseBackground.h LiteralList.h DefinedHeaderTable.h
BSMachine.o: ../BlodKorv/StaticCellArray.h
BuiltInFloatingPointArithmetic.o: /usr/include/string.h
BuiltInFloatingPointArithmetic.o: /usr/include/features.h
BuiltInFloatingPointArithmetic.o: /usr/include/sys/cdefs.h
BuiltInFloatingPointArithmetic.o: /usr/include/gnu/stubs.h
BuiltInFloatingPointArithmetic.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/GlobAlloc.h
BuiltInFloatingPointArithmetic.o: /usr/include/stdlib.h ../BlodKorv/jargon.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/BlodKorvDebugFlags.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/ClassDesc.h
BuiltInFloatingPointArithmetic.o: /usr/include/limits.h
BuiltInFloatingPointArithmetic.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/Debug.h /usr/include/time.h
BuiltInFloatingPointArithmetic.o: /usr/include/bits/time.h
BuiltInFloatingPointArithmetic.o: /usr/include/bits/types.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/ObjDesc.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/AllocationFailureHandling.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/Exit.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/DestructionMode.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/RuntimeError.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/debugMacros.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/Allocator.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/BufferedAllocationStatistics.h
BuiltInFloatingPointArithmetic.o: BuiltInFloatingPointArithmetic.h
BuiltInFloatingPointArithmetic.o: VampireKernelDebugFlags.h BuiltInTheory.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/Array.h Clause.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/GList.h ../BlodKorv/BitWord.h
BuiltInFloatingPointArithmetic.o: Term.h VampireKernelConst.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/Comparison.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/Stack.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/ExpandingStack.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/BitSet.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/BitString.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/SmallHeaderSkipList.h
BuiltInFloatingPointArithmetic.o: TermWeightType.h VampireKernel.h
BuiltInFloatingPointArithmetic.o: Signature.h SymbolInfo.h
BuiltInFloatingPointArithmetic.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/GSortTree.h ClauseBackground.h
BuiltInFloatingPointArithmetic.o: LiteralList.h DefinedHeaderTable.h
BuiltInFloatingPointArithmetic.o: Flatterm.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/MultisetOfVariables.h
BuiltInFloatingPointArithmetic.o: variables.h WeightPolynomial.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/LinearPolynomial.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/LinearCombination.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/SharedLinearPolynomial.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/GDiscTree.h tmp_literal.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/Multiset.h TermWeighting.h
BuiltInFloatingPointArithmetic.o: InstanceRetrievalForDemodulation.h
BuiltInFloatingPointArithmetic.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/GlobalStopFlag.h
BuiltInFloatingPointArithmetic.o: ../Gematogen/Gematogen.h
BuiltInFloatingPointArithmetic.o: ../Gematogen/GematogenDebugFlags.h
BuiltInFloatingPointArithmetic.o: ../Gematogen/OptimisedPathIndex.h
BuiltInFloatingPointArithmetic.o: ../BlodKorv/GIncCountingSort.h
BuiltInFloatingPointArithmetic.o: SimplificationOrdering.h
BuiltInTheory.o: ../BlodKorv/RuntimeError.h ../BlodKorv/jargon.h
BuiltInTheory.o: ../BlodKorv/target.h ../BlodKorv/compiler.h BuiltInTheory.h
BuiltInTheory.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
BuiltInTheory.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
BuiltInTheory.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
BuiltInTheory.o: /usr/include/time.h /usr/include/features.h
BuiltInTheory.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BuiltInTheory.o: /usr/include/bits/time.h /usr/include/bits/types.h
BuiltInTheoryDispatcher.o: /usr/include/string.h /usr/include/features.h
BuiltInTheoryDispatcher.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
BuiltInTheoryDispatcher.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
BuiltInTheoryDispatcher.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
BuiltInTheoryDispatcher.o: ../BlodKorv/jargon.h ../BlodKorv/target.h
BuiltInTheoryDispatcher.o: ../BlodKorv/compiler.h
BuiltInTheoryDispatcher.o: ../BlodKorv/BlodKorvDebugFlags.h
BuiltInTheoryDispatcher.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
BuiltInTheoryDispatcher.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
BuiltInTheoryDispatcher.o: ../BlodKorv/Debug.h /usr/include/time.h
BuiltInTheoryDispatcher.o: /usr/include/bits/time.h /usr/include/bits/types.h
BuiltInTheoryDispatcher.o: ../BlodKorv/ObjDesc.h
BuiltInTheoryDispatcher.o: ../BlodKorv/AllocationFailureHandling.h
BuiltInTheoryDispatcher.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
BuiltInTheoryDispatcher.o: ../BlodKorv/RuntimeError.h
BuiltInTheoryDispatcher.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
BuiltInTheoryDispatcher.o: ../BlodKorv/BufferedAllocationStatistics.h
BuiltInTheoryDispatcher.o: BuiltInTheoryDispatcher.h ../BlodKorv/GList.h
BuiltInTheoryDispatcher.o: VampireKernelDebugFlags.h Clause.h
BuiltInTheoryDispatcher.o: ../BlodKorv/BitWord.h Term.h VampireKernelConst.h
BuiltInTheoryDispatcher.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
BuiltInTheoryDispatcher.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
BuiltInTheoryDispatcher.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
BuiltInTheoryDispatcher.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
BuiltInTheoryDispatcher.o: VampireKernel.h Signature.h SymbolInfo.h
BuiltInTheoryDispatcher.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
BuiltInTheoryDispatcher.o: ../BlodKorv/GSortTree.h ClauseBackground.h
BuiltInTheoryDispatcher.o: LiteralList.h DefinedHeaderTable.h
BuiltInTheoryDispatcher.o: BuiltInFloatingPointArithmetic.h BuiltInTheory.h
BuiltInTheoryDispatcher.o: Flatterm.h ../BlodKorv/MultisetOfVariables.h
BuiltInTheoryDispatcher.o: variables.h WeightPolynomial.h
BuiltInTheoryDispatcher.o: ../BlodKorv/LinearPolynomial.h
BuiltInTheoryDispatcher.o: ../BlodKorv/LinearCombination.h
BuiltInTheoryDispatcher.o: ../BlodKorv/SharedLinearPolynomial.h
BuiltInTheoryDispatcher.o: ../BlodKorv/GDiscTree.h
ClauseAssembler.o: ClauseAssembler.h ../BlodKorv/jargon.h
ClauseAssembler.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
ClauseAssembler.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
ClauseAssembler.o: LiteralSharing.h ../BlodKorv/DestructionMode.h
ClauseAssembler.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
ClauseAssembler.o: /usr/include/features.h /usr/include/sys/cdefs.h
ClauseAssembler.o: /usr/include/gnu/stubs.h
ClauseAssembler.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ClauseAssembler.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
ClauseAssembler.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ClauseAssembler.o: ../BlodKorv/Debug.h /usr/include/time.h
ClauseAssembler.o: /usr/include/bits/time.h /usr/include/bits/types.h
ClauseAssembler.o: ../BlodKorv/ObjDesc.h
ClauseAssembler.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
ClauseAssembler.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
ClauseAssembler.o: ../BlodKorv/Allocator.h
ClauseAssembler.o: ../BlodKorv/BufferedAllocationStatistics.h
ClauseAssembler.o: ../BlodKorv/Array.h Term.h VampireKernelConst.h
ClauseAssembler.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
ClauseAssembler.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/BitSet.h
ClauseAssembler.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
ClauseAssembler.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
ClauseAssembler.o: VampireKernel.h Signature.h /usr/include/string.h
ClauseAssembler.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
ClauseAssembler.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h variables.h
ClauseAssembler.o: ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
ClauseAssembler.o: ../BlodKorv/LinearPolynomial.h
ClauseAssembler.o: ../BlodKorv/LinearCombination.h
ClauseAssembler.o: ../BlodKorv/SharedLinearPolynomial.h
ClauseAssembler.o: ../BlodKorv/GDiscTree.h LiteralList.h sharing.h
ClauseAssembler.o: VampireKernelInterceptMacros.h dtree.h LiteralSelection.h
ClauseAssembler.o: TmpLitList.h tmp_literal.h ../BlodKorv/Multiset.h
ClauseAssembler.o: TermWeighting.h Flatterm.h
ClauseAssembler.o: InstanceRetrievalForDemodulation.h
ClauseAssembler.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
ClauseAssembler.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
ClauseAssembler.o: ../Gematogen/GematogenDebugFlags.h
ClauseAssembler.o: ../Gematogen/OptimisedPathIndex.h
ClauseAssembler.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
ClauseAssembler.o: Clause.h ClauseBackground.h DefinedHeaderTable.h
ClauseAssembler.o: PagedPassiveClauseStorage.h
ClauseAssembler.o: ../BlodKorv/PageQueueDisciplineStorage.h
ClauseBackground.o: ClauseBackground.h ../BlodKorv/jargon.h
ClauseBackground.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
ClauseBackground.o: VampireKernelDebugFlags.h
ClauseBackground.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/ObjDesc.h
ClauseBackground.o: /usr/include/stdlib.h /usr/include/features.h
ClauseBackground.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ClauseBackground.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ClauseBackground.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
ClauseBackground.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ClauseBackground.o: ../BlodKorv/Debug.h /usr/include/time.h
ClauseBackground.o: /usr/include/bits/time.h /usr/include/bits/types.h
ClauseBackground.o: ../BlodKorv/GlobAlloc.h
ClauseBackground.o: ../BlodKorv/AllocationFailureHandling.h
ClauseBackground.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
ClauseBackground.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
ClauseBackground.o: ../BlodKorv/Allocator.h
ClauseBackground.o: ../BlodKorv/BufferedAllocationStatistics.h
ClauseBackground.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
ClauseBackground.o: ../BlodKorv/BitWord.h ../BlodKorv/GList.h
Clause.o: Clause.h /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Clause.o: ../BlodKorv/jargon.h ../BlodKorv/target.h ../BlodKorv/compiler.h
Clause.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
Clause.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
Clause.o: /usr/include/features.h /usr/include/sys/cdefs.h
Clause.o: /usr/include/gnu/stubs.h ../BlodKorv/ClassDesc.h
Clause.o: /usr/include/limits.h
Clause.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Clause.o: ../BlodKorv/Debug.h /usr/include/time.h /usr/include/bits/time.h
Clause.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
Clause.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
Clause.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
Clause.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
Clause.o: ../BlodKorv/BufferedAllocationStatistics.h ../BlodKorv/GList.h
Clause.o: ../BlodKorv/BitWord.h Term.h VampireKernelConst.h
Clause.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
Clause.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
Clause.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
Clause.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h VampireKernel.h
Clause.o: Signature.h /usr/include/string.h SymbolInfo.h
Clause.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
Clause.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h
Clause.o: VampireKernelInterceptMacros.h
ClauseNumIndex.o: ClauseNumIndex.h ../BlodKorv/jargon.h ../BlodKorv/target.h
ClauseNumIndex.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
ClauseNumIndex.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/SkipList.h
ClauseNumIndex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ClauseNumIndex.o: ../BlodKorv/DestructionMode.h ../BlodKorv/debugMacros.h
ClauseNumIndex.o: ../BlodKorv/Debug.h /usr/include/time.h
ClauseNumIndex.o: /usr/include/features.h /usr/include/sys/cdefs.h
ClauseNumIndex.o: /usr/include/gnu/stubs.h /usr/include/bits/time.h
ClauseNumIndex.o: /usr/include/bits/types.h ../BlodKorv/GlobAlloc.h
ClauseNumIndex.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
ClauseNumIndex.o: /usr/include/limits.h
ClauseNumIndex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ClauseNumIndex.o: ../BlodKorv/ObjDesc.h
ClauseNumIndex.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
ClauseNumIndex.o: ../BlodKorv/RuntimeError.h ../BlodKorv/Allocator.h
ClauseNumIndex.o: ../BlodKorv/BufferedAllocationStatistics.h Clause.h
ClauseNumIndex.o: ../BlodKorv/GList.h ../BlodKorv/BitWord.h Term.h
ClauseNumIndex.o: VampireKernelConst.h ../BlodKorv/Comparison.h
ClauseNumIndex.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
ClauseNumIndex.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
ClauseNumIndex.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
ClauseNumIndex.o: TermWeightType.h VampireKernel.h Signature.h
ClauseNumIndex.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
ClauseNumIndex.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
ClauseNumIndex.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h
ClauseQueue.o: ClauseQueue.h ../BlodKorv/jargon.h ../BlodKorv/target.h
ClauseQueue.o: ../BlodKorv/compiler.h ../BlodKorv/FIFO.h
ClauseQueue.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/GList.h
ClauseQueue.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ClauseQueue.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
ClauseQueue.o: /usr/include/time.h /usr/include/features.h
ClauseQueue.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ClauseQueue.o: /usr/include/bits/time.h /usr/include/bits/types.h
ClauseQueue.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
ClauseQueue.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
ClauseQueue.o: /usr/include/limits.h
ClauseQueue.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ClauseQueue.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
ClauseQueue.o: ../BlodKorv/Exit.h ../BlodKorv/RuntimeError.h
ClauseQueue.o: ../BlodKorv/Allocator.h
ClauseQueue.o: ../BlodKorv/BufferedAllocationStatistics.h Clause.h
ClauseQueue.o: VampireKernelDebugFlags.h ../BlodKorv/BitWord.h Term.h
ClauseQueue.o: VampireKernelConst.h ../BlodKorv/Comparison.h
ClauseQueue.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
ClauseQueue.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
ClauseQueue.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
ClauseQueue.o: TermWeightType.h VampireKernel.h Signature.h
ClauseQueue.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
ClauseQueue.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ClauseBackground.h
ClauseQueue.o: LiteralList.h DefinedHeaderTable.h
ComponentName.o: ComponentName.h ../BlodKorv/jargon.h ../BlodKorv/target.h
ComponentName.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
ComponentName.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
ComponentName.o: /usr/include/limits.h /usr/include/features.h
ComponentName.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ComponentName.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ComponentName.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ComponentName.o: fs_code.h ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
ComponentName.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
ComponentName.o: /usr/include/time.h /usr/include/bits/time.h
ComponentName.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
ComponentName.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
ComponentName.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
ComponentName.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
ComponentName.o: ../BlodKorv/BufferedAllocationStatistics.h
ComponentName.o: ../BlodKorv/MultisetOfVariables.h ../BlodKorv/Stack.h
ComponentName.o: ../BlodKorv/Comparison.h ../BlodKorv/EquivalenceRelation.h
ComponentName.o: fs_command.h Term.h ../BlodKorv/ExpandingStack.h
ComponentName.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
ComponentName.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
ComponentName.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
ComponentName.o: VampireKernel.h Signature.h /usr/include/string.h
ComponentName.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
ComponentName.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h old_fs_code.h
ComponentName.o: ../BlodKorv/Queue.h TmpLitList.h tmp_literal.h
ComponentName.o: ../BlodKorv/Multiset.h WeightPolynomial.h
ComponentName.o: ../BlodKorv/LinearPolynomial.h
ComponentName.o: ../BlodKorv/LinearCombination.h
ComponentName.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
ComponentName.o: TermWeighting.h Flatterm.h variables.h
ComponentName.o: InstanceRetrievalForDemodulation.h
ComponentName.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
ComponentName.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
ComponentName.o: ../Gematogen/GematogenDebugFlags.h
ComponentName.o: ../Gematogen/OptimisedPathIndex.h
ComponentName.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
ComponentName.o: ../BlodKorv/ObjectPool.h
ForwardDemodulation.o: ForwardDemodulation.h ../BlodKorv/jargon.h
ForwardDemodulation.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
ForwardDemodulation.o: VampireKernelDebugFlags.h
ForwardDemodulation.o: ../BlodKorv/BlodKorvDebugFlags.h
ForwardDemodulation.o: VampireKernelInterceptMacros.h ../BlodKorv/Exit.h
ForwardDemodulation.o: /usr/include/stdlib.h /usr/include/features.h
ForwardDemodulation.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ForwardDemodulation.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ForwardDemodulation.o: Signature.h /usr/include/string.h VampireKernelConst.h
ForwardDemodulation.o: /usr/include/limits.h
ForwardDemodulation.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ForwardDemodulation.o: ../BlodKorv/Array.h ../BlodKorv/DestructionMode.h
ForwardDemodulation.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
ForwardDemodulation.o: /usr/include/time.h /usr/include/bits/time.h
ForwardDemodulation.o: /usr/include/bits/types.h ../BlodKorv/GlobAlloc.h
ForwardDemodulation.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
ForwardDemodulation.o: ../BlodKorv/AllocationFailureHandling.h
ForwardDemodulation.o: ../BlodKorv/RuntimeError.h ../BlodKorv/Allocator.h
ForwardDemodulation.o: ../BlodKorv/BufferedAllocationStatistics.h
ForwardDemodulation.o: ../BlodKorv/Comparison.h SymbolInfo.h
ForwardDemodulation.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
ForwardDemodulation.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
ForwardDemodulation.o: TermWeightType.h VampireKernel.h
ForwardDemodulation.o: ../Gematogen/ForwardMatchingIndexTree.h
ForwardDemodulation.o: ../BlodKorv/GlobalStopFlag.h
ForwardDemodulation.o: ../Gematogen/GematogenDebugFlags.h ../BlodKorv/Stack.h
ForwardDemodulation.o: ../BlodKorv/EquivalenceRelation.h
ForwardDemodulation.o: ../BlodKorv/MultisetOfVariables.h
ForwardDemodulation.o: ../Gematogen/ForwardMatchingIndex.h
ForwardDemodulation.o: ../BlodKorv/ExpandingMultiset.h
ForwardDemodulation.o: ../BlodKorv/ExpandingStack.h Term.h
ForwardDemodulation.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
ForwardDemodulation.o: ../BlodKorv/BitString.h
ForwardDemodulation.o: ../BlodKorv/SmallHeaderSkipList.h Flatterm.h
ForwardDemodulation.o: variables.h WeightPolynomial.h
ForwardDemodulation.o: ../BlodKorv/LinearPolynomial.h
ForwardDemodulation.o: ../BlodKorv/LinearCombination.h
ForwardDemodulation.o: ../BlodKorv/SharedLinearPolynomial.h
ForwardDemodulation.o: ../BlodKorv/GDiscTree.h
ForwardDemodulation.o: ForwardMatchingSubstitutionCache.h
ForwardDemodulation.o: SimplificationOrdering.h
ForwardDemodulation.o: InstanceRetrievalForDemodulation.h
ForwardDemodulation.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
ForwardDemodulation.o: ../Gematogen/Gematogen.h
ForwardDemodulation.o: ../Gematogen/OptimisedPathIndex.h
ForwardDemodulation.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
ForwardDemodulation.o: Clause.h ClauseBackground.h LiteralList.h
ForwardDemodulation.o: DefinedHeaderTable.h
forward_subsumption.o: forward_subsumption.h ../BlodKorv/jargon.h
forward_subsumption.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
forward_subsumption.o: VampireKernelDebugFlags.h
forward_subsumption.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
forward_subsumption.o: /usr/include/limits.h /usr/include/features.h
forward_subsumption.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
forward_subsumption.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
forward_subsumption.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
forward_subsumption.o: fs_code.h ../BlodKorv/GlobAlloc.h
forward_subsumption.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
forward_subsumption.o: ../BlodKorv/Debug.h /usr/include/time.h
forward_subsumption.o: /usr/include/bits/time.h /usr/include/bits/types.h
forward_subsumption.o: ../BlodKorv/ObjDesc.h
forward_subsumption.o: ../BlodKorv/AllocationFailureHandling.h
forward_subsumption.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
forward_subsumption.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
forward_subsumption.o: ../BlodKorv/Allocator.h
forward_subsumption.o: ../BlodKorv/BufferedAllocationStatistics.h
forward_subsumption.o: ../BlodKorv/MultisetOfVariables.h ../BlodKorv/Stack.h
forward_subsumption.o: ../BlodKorv/Comparison.h
forward_subsumption.o: ../BlodKorv/EquivalenceRelation.h fs_command.h Term.h
forward_subsumption.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
forward_subsumption.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
forward_subsumption.o: ../BlodKorv/BitString.h
forward_subsumption.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
forward_subsumption.o: VampireKernel.h Signature.h /usr/include/string.h
forward_subsumption.o: SymbolInfo.h AutoFunctorPrecedence.h
forward_subsumption.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
forward_subsumption.o: ../BlodKorv/GList.h old_fs_code.h ../BlodKorv/Queue.h
forward_subsumption.o: TmpLitList.h tmp_literal.h ../BlodKorv/Multiset.h
forward_subsumption.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
forward_subsumption.o: ../BlodKorv/LinearCombination.h
forward_subsumption.o: ../BlodKorv/SharedLinearPolynomial.h
forward_subsumption.o: ../BlodKorv/GDiscTree.h TermWeighting.h Flatterm.h
forward_subsumption.o: variables.h InstanceRetrievalForDemodulation.h
forward_subsumption.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
forward_subsumption.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
forward_subsumption.o: ../Gematogen/GematogenDebugFlags.h
forward_subsumption.o: ../Gematogen/OptimisedPathIndex.h
forward_subsumption.o: ../BlodKorv/GIncCountingSort.h
forward_subsumption.o: SimplificationOrdering.h ../BlodKorv/ObjectPool.h
forward_subsumption.o: Clause.h ClauseBackground.h LiteralList.h
forward_subsumption.o: DefinedHeaderTable.h
forward_superposition.o: forward_superposition.h ../BlodKorv/jargon.h
forward_superposition.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
forward_superposition.o: VampireKernelDebugFlags.h
forward_superposition.o: ../BlodKorv/BlodKorvDebugFlags.h
forward_superposition.o: VampireKernelConst.h /usr/include/limits.h
forward_superposition.o: /usr/include/features.h /usr/include/sys/cdefs.h
forward_superposition.o: /usr/include/gnu/stubs.h
forward_superposition.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
forward_superposition.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
forward_superposition.o: ../BlodKorv/Stack.h ../BlodKorv/debugMacros.h
forward_superposition.o: ../BlodKorv/Debug.h /usr/include/time.h
forward_superposition.o: /usr/include/bits/time.h /usr/include/bits/types.h
forward_superposition.o: para_machine.h iu_command.h prefix.h Term.h
forward_superposition.o: ../BlodKorv/DestructionMode.h
forward_superposition.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
forward_superposition.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
forward_superposition.o: ../BlodKorv/AllocationFailureHandling.h
forward_superposition.o: ../BlodKorv/Exit.h ../BlodKorv/RuntimeError.h
forward_superposition.o: ../BlodKorv/Allocator.h
forward_superposition.o: ../BlodKorv/BufferedAllocationStatistics.h
forward_superposition.o: ../BlodKorv/Comparison.h
forward_superposition.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
forward_superposition.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
forward_superposition.o: ../BlodKorv/BitString.h
forward_superposition.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
forward_superposition.o: VampireKernel.h Signature.h /usr/include/string.h
forward_superposition.o: SymbolInfo.h AutoFunctorPrecedence.h
forward_superposition.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
forward_superposition.o: ../BlodKorv/GList.h variables.h
forward_superposition.o: ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
forward_superposition.o: ../BlodKorv/LinearPolynomial.h
forward_superposition.o: ../BlodKorv/LinearCombination.h
forward_superposition.o: ../BlodKorv/SharedLinearPolynomial.h
forward_superposition.o: ../BlodKorv/GDiscTree.h iu_machine.h iu_translator.h
forward_superposition.o: or_index.h unification.h BackParaCandidates.h
forward_superposition.o: ../BlodKorv/SkipList.h SimplificationOrdering.h
forward_superposition.o: InstanceRetrievalForDemodulation.h
forward_superposition.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
forward_superposition.o: ../BlodKorv/GlobalStopFlag.h
forward_superposition.o: ../Gematogen/Gematogen.h
forward_superposition.o: ../Gematogen/GematogenDebugFlags.h
forward_superposition.o: ../Gematogen/OptimisedPathIndex.h
forward_superposition.o: ../BlodKorv/GIncCountingSort.h
forward_superposition.o: ../BlodKorv/Multiset.h NewClause.h OpenNewClause.h
forward_superposition.o: Flatterm.h tmp_literal.h TermWeighting.h
forward_superposition.o: TmpLitList.h ClauseBackground.h
forward_superposition.o: ForwardDemodulation.h VampireKernelInterceptMacros.h
forward_superposition.o: ../Gematogen/ForwardMatchingIndexTree.h
forward_superposition.o: ../BlodKorv/EquivalenceRelation.h
forward_superposition.o: ../Gematogen/ForwardMatchingIndex.h
forward_superposition.o: ../BlodKorv/ExpandingMultiset.h
forward_superposition.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h
forward_superposition.o: ../BlodKorv/FIFO.h Clause.h LiteralList.h
forward_superposition.o: DefinedHeaderTable.h PagedPassiveClauseStorage.h
forward_superposition.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h
forward_superposition.o: DivIntoComp.h weight_formula.h
fs_code.o: fs_code.h ../BlodKorv/jargon.h ../BlodKorv/target.h
fs_code.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
fs_code.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
fs_code.o: /usr/include/limits.h /usr/include/features.h
fs_code.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fs_code.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
fs_code.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
fs_code.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
fs_code.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h /usr/include/time.h
fs_code.o: /usr/include/bits/time.h /usr/include/bits/types.h
fs_code.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
fs_code.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
fs_code.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
fs_code.o: ../BlodKorv/Allocator.h ../BlodKorv/BufferedAllocationStatistics.h
fs_code.o: ../BlodKorv/MultisetOfVariables.h ../BlodKorv/Stack.h
fs_code.o: ../BlodKorv/Comparison.h ../BlodKorv/EquivalenceRelation.h
fs_code.o: fs_command.h Term.h ../BlodKorv/ExpandingStack.h
fs_code.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
fs_code.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
fs_code.o: TermWeightType.h VampireKernel.h Signature.h /usr/include/string.h
fs_code.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
fs_code.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h old_fs_code.h
fs_code.o: ../BlodKorv/Queue.h TmpLitList.h tmp_literal.h
fs_code.o: ../BlodKorv/Multiset.h WeightPolynomial.h
fs_code.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
fs_code.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
fs_code.o: TermWeighting.h Flatterm.h variables.h
fs_code.o: InstanceRetrievalForDemodulation.h
fs_code.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
fs_code.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
fs_code.o: ../Gematogen/GematogenDebugFlags.h
fs_code.o: ../Gematogen/OptimisedPathIndex.h ../BlodKorv/GIncCountingSort.h
fs_code.o: SimplificationOrdering.h Clause.h ClauseBackground.h LiteralList.h
fs_code.o: DefinedHeaderTable.h
fs_command.o: fs_command.h ../BlodKorv/jargon.h ../BlodKorv/target.h
fs_command.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
fs_command.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
fs_command.o: /usr/include/limits.h /usr/include/features.h
fs_command.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fs_command.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
fs_command.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
fs_command.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
fs_command.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h /usr/include/time.h
fs_command.o: /usr/include/bits/time.h /usr/include/bits/types.h
fs_command.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
fs_command.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
fs_command.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
fs_command.o: ../BlodKorv/Allocator.h
fs_command.o: ../BlodKorv/BufferedAllocationStatistics.h Term.h
fs_command.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
fs_command.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
fs_command.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
fs_command.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
fs_command.o: TermWeightType.h VampireKernel.h Signature.h
fs_command.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
fs_command.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
InferenceDispatcher.o: InferenceDispatcher.h ../BlodKorv/jargon.h
InferenceDispatcher.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
InferenceDispatcher.o: VampireKernelDebugFlags.h
InferenceDispatcher.o: ../BlodKorv/BlodKorvDebugFlags.h or_index.h
InferenceDispatcher.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
InferenceDispatcher.o: /usr/include/features.h /usr/include/sys/cdefs.h
InferenceDispatcher.o: /usr/include/gnu/stubs.h
InferenceDispatcher.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
InferenceDispatcher.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
InferenceDispatcher.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
InferenceDispatcher.o: ../BlodKorv/Debug.h /usr/include/time.h
InferenceDispatcher.o: /usr/include/bits/time.h /usr/include/bits/types.h
InferenceDispatcher.o: ../BlodKorv/ObjDesc.h
InferenceDispatcher.o: ../BlodKorv/AllocationFailureHandling.h
InferenceDispatcher.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
InferenceDispatcher.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
InferenceDispatcher.o: ../BlodKorv/Allocator.h
InferenceDispatcher.o: ../BlodKorv/BufferedAllocationStatistics.h
InferenceDispatcher.o: ../BlodKorv/Array.h VampireKernelConst.h Term.h
InferenceDispatcher.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
InferenceDispatcher.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/BitSet.h
InferenceDispatcher.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
InferenceDispatcher.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
InferenceDispatcher.o: VampireKernel.h Signature.h /usr/include/string.h
InferenceDispatcher.o: SymbolInfo.h AutoFunctorPrecedence.h
InferenceDispatcher.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
InferenceDispatcher.o: ../BlodKorv/GList.h prefix.h variables.h
InferenceDispatcher.o: ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
InferenceDispatcher.o: ../BlodKorv/LinearPolynomial.h
InferenceDispatcher.o: ../BlodKorv/LinearCombination.h
InferenceDispatcher.o: ../BlodKorv/SharedLinearPolynomial.h
InferenceDispatcher.o: ../BlodKorv/GDiscTree.h unification.h iu_command.h
InferenceDispatcher.o: BackParaCandidates.h ../BlodKorv/SkipList.h
InferenceDispatcher.o: SimplificationOrdering.h
InferenceDispatcher.o: InstanceRetrievalForDemodulation.h
InferenceDispatcher.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
InferenceDispatcher.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
InferenceDispatcher.o: ../Gematogen/GematogenDebugFlags.h
InferenceDispatcher.o: ../Gematogen/OptimisedPathIndex.h
InferenceDispatcher.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
InferenceDispatcher.o: binary_resolution.h premise_factoring.h or_premise.h
InferenceDispatcher.o: Clause.h ClauseBackground.h LiteralList.h
InferenceDispatcher.o: DefinedHeaderTable.h NewClause.h OpenNewClause.h
InferenceDispatcher.o: Flatterm.h tmp_literal.h TermWeighting.h TmpLitList.h
InferenceDispatcher.o: ForwardDemodulation.h VampireKernelInterceptMacros.h
InferenceDispatcher.o: ../Gematogen/ForwardMatchingIndexTree.h
InferenceDispatcher.o: ../BlodKorv/EquivalenceRelation.h
InferenceDispatcher.o: ../Gematogen/ForwardMatchingIndex.h
InferenceDispatcher.o: ../BlodKorv/ExpandingMultiset.h
InferenceDispatcher.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h
InferenceDispatcher.o: ../BlodKorv/FIFO.h PagedPassiveClauseStorage.h
InferenceDispatcher.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h
InferenceDispatcher.o: DivIntoComp.h or_machine.h iu_machine.h
InferenceDispatcher.o: iu_translator.h weight_formula.h
InferenceDispatcher.o: forward_superposition.h para_machine.h eq_factoring.h
InferenceDispatcher.o: backward_superposition.h
InferenceTree.o: InferenceTree.h ../BlodKorv/jargon.h ../BlodKorv/target.h
InferenceTree.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
InferenceTree.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/GSortTree.h
InferenceTree.o: ../BlodKorv/GList.h
InferenceTree.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
InferenceTree.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
InferenceTree.o: /usr/include/time.h /usr/include/features.h
InferenceTree.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
InferenceTree.o: /usr/include/bits/time.h /usr/include/bits/types.h
InferenceTree.o: ../BlodKorv/DestructionMode.h Clause.h
InferenceTree.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
InferenceTree.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
InferenceTree.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
InferenceTree.o: ../BlodKorv/ObjDesc.h
InferenceTree.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
InferenceTree.o: ../BlodKorv/RuntimeError.h ../BlodKorv/Allocator.h
InferenceTree.o: ../BlodKorv/BufferedAllocationStatistics.h
InferenceTree.o: ../BlodKorv/BitWord.h Term.h VampireKernelConst.h
InferenceTree.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
InferenceTree.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
InferenceTree.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
InferenceTree.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
InferenceTree.o: VampireKernel.h Signature.h /usr/include/string.h
InferenceTree.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
InferenceTree.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h
Input.o: Input.h /usr/include/string.h /usr/include/features.h
Input.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Input.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Input.o: /usr/include/stdio.h /usr/include/bits/types.h /usr/include/libio.h
Input.o: /usr/include/_G_config.h /usr/include/wchar.h
Input.o: /usr/include/bits/wchar.h /usr/include/gconv.h
Input.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
Input.o: /usr/include/bits/stdio_lim.h ../BlodKorv/jargon.h
Input.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
Input.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
Input.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GList.h
Input.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h /usr/include/time.h
Input.o: /usr/include/bits/time.h ../BlodKorv/Malloc.h /usr/include/stdlib.h
Input.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
Input.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Input.o: ../BlodKorv/ObjDesc.h VampireKernel.h
InstanceRetrievalForDemodulation.o: InstanceRetrievalForDemodulation.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/jargon.h ../BlodKorv/target.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/compiler.h
InstanceRetrievalForDemodulation.o: VampireKernelConst.h
InstanceRetrievalForDemodulation.o: /usr/include/limits.h
InstanceRetrievalForDemodulation.o: /usr/include/features.h
InstanceRetrievalForDemodulation.o: /usr/include/sys/cdefs.h
InstanceRetrievalForDemodulation.o: /usr/include/gnu/stubs.h
InstanceRetrievalForDemodulation.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
InstanceRetrievalForDemodulation.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
InstanceRetrievalForDemodulation.o: VampireKernelDebugFlags.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/BlodKorvDebugFlags.h Term.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/DestructionMode.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/GlobAlloc.h
InstanceRetrievalForDemodulation.o: /usr/include/stdlib.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/ClassDesc.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/Debug.h /usr/include/time.h
InstanceRetrievalForDemodulation.o: /usr/include/bits/time.h
InstanceRetrievalForDemodulation.o: /usr/include/bits/types.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/ObjDesc.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/AllocationFailureHandling.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/Exit.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/RuntimeError.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/debugMacros.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/Allocator.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/BufferedAllocationStatistics.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/Comparison.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/Stack.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/ExpandingStack.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/BitWord.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/BitString.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/SmallHeaderSkipList.h
InstanceRetrievalForDemodulation.o: TermWeightType.h VampireKernel.h
InstanceRetrievalForDemodulation.o: Signature.h /usr/include/string.h
InstanceRetrievalForDemodulation.o: SymbolInfo.h AutoFunctorPrecedence.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/Tuple.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/GSortTree.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/GList.h
InstanceRetrievalForDemodulation.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/GlobalStopFlag.h
InstanceRetrievalForDemodulation.o: ../Gematogen/Gematogen.h
InstanceRetrievalForDemodulation.o: ../Gematogen/GematogenDebugFlags.h
InstanceRetrievalForDemodulation.o: ../Gematogen/OptimisedPathIndex.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/GIncCountingSort.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/Multiset.h
InstanceRetrievalForDemodulation.o: ../BlodKorv/LinearCombination.h
iu_command.o: iu_command.h ../BlodKorv/jargon.h ../BlodKorv/target.h
iu_command.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
iu_command.o: ../BlodKorv/BlodKorvDebugFlags.h prefix.h VampireKernelConst.h
iu_command.o: /usr/include/limits.h /usr/include/features.h
iu_command.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
iu_command.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
iu_command.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Term.h
iu_command.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
iu_command.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
iu_command.o: ../BlodKorv/Debug.h /usr/include/time.h
iu_command.o: /usr/include/bits/time.h /usr/include/bits/types.h
iu_command.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
iu_command.o: ../BlodKorv/Exit.h ../BlodKorv/RuntimeError.h
iu_command.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
iu_command.o: ../BlodKorv/BufferedAllocationStatistics.h
iu_command.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
iu_command.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
iu_command.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
iu_command.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
iu_command.o: TermWeightType.h VampireKernel.h Signature.h
iu_command.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
iu_command.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
iu_command.o: variables.h ../BlodKorv/MultisetOfVariables.h
iu_command.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
iu_command.o: ../BlodKorv/LinearCombination.h
iu_command.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
iu_machine.o: ../BlodKorv/GlobalStopFlag.h ../BlodKorv/jargon.h
iu_machine.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
iu_machine.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/debugMacros.h
iu_machine.o: ../BlodKorv/Debug.h /usr/include/time.h /usr/include/features.h
iu_machine.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
iu_machine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
iu_machine.o: /usr/include/bits/time.h /usr/include/bits/types.h iu_machine.h
iu_machine.o: VampireKernelDebugFlags.h VampireKernelConst.h
iu_machine.o: /usr/include/limits.h
iu_machine.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
iu_machine.o: iu_command.h prefix.h Term.h ../BlodKorv/DestructionMode.h
iu_machine.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
iu_machine.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
iu_machine.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
iu_machine.o: ../BlodKorv/RuntimeError.h ../BlodKorv/Allocator.h
iu_machine.o: ../BlodKorv/BufferedAllocationStatistics.h
iu_machine.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
iu_machine.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
iu_machine.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
iu_machine.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
iu_machine.o: TermWeightType.h VampireKernel.h Signature.h
iu_machine.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
iu_machine.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
iu_machine.o: variables.h ../BlodKorv/MultisetOfVariables.h
iu_machine.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
iu_machine.o: ../BlodKorv/LinearCombination.h
iu_machine.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
iu_machine.o: iu_translator.h or_index.h unification.h BackParaCandidates.h
iu_machine.o: ../BlodKorv/SkipList.h SimplificationOrdering.h
iu_machine.o: InstanceRetrievalForDemodulation.h
iu_machine.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
iu_machine.o: ../Gematogen/Gematogen.h ../Gematogen/GematogenDebugFlags.h
iu_machine.o: ../Gematogen/OptimisedPathIndex.h
iu_machine.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
kept.o: kept.h /usr/include/limits.h /usr/include/features.h
kept.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
kept.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
kept.o: ../BlodKorv/jargon.h ../BlodKorv/target.h ../BlodKorv/compiler.h
kept.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
kept.o: VampireKernelConst.h
kept.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Clause.h
kept.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
kept.o: ../BlodKorv/Debug.h /usr/include/time.h /usr/include/bits/time.h
kept.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
kept.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
kept.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
kept.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
kept.o: ../BlodKorv/BufferedAllocationStatistics.h ../BlodKorv/GList.h
kept.o: ../BlodKorv/BitWord.h Term.h ../BlodKorv/Comparison.h
kept.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
kept.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
kept.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h VampireKernel.h
kept.o: Signature.h /usr/include/string.h SymbolInfo.h
kept.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
kept.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h
LiteralList.o: LiteralList.h ../BlodKorv/jargon.h ../BlodKorv/target.h
LiteralList.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
LiteralList.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
LiteralList.o: /usr/include/limits.h /usr/include/features.h
LiteralList.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
LiteralList.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
LiteralList.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
LiteralList.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
LiteralList.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
LiteralList.o: /usr/include/time.h /usr/include/bits/time.h
LiteralList.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
LiteralList.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
LiteralList.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
LiteralList.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
LiteralList.o: ../BlodKorv/BufferedAllocationStatistics.h Term.h
LiteralList.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
LiteralList.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
LiteralList.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
LiteralList.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
LiteralList.o: TermWeightType.h VampireKernel.h Signature.h
LiteralList.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
LiteralList.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
LiteralList.o: ../BlodKorv/GList.h
LiteralSelection.o: VampireKernelDebugFlags.h
LiteralSelection.o: ../BlodKorv/BlodKorvDebugFlags.h LiteralSelection.h
LiteralSelection.o: ../BlodKorv/jargon.h ../BlodKorv/target.h
LiteralSelection.o: ../BlodKorv/compiler.h ../BlodKorv/RuntimeError.h
LiteralSelection.o: TmpLitList.h ../BlodKorv/GlobAlloc.h
LiteralSelection.o: /usr/include/stdlib.h /usr/include/features.h
LiteralSelection.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
LiteralSelection.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
LiteralSelection.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
LiteralSelection.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
LiteralSelection.o: ../BlodKorv/Debug.h /usr/include/time.h
LiteralSelection.o: /usr/include/bits/time.h /usr/include/bits/types.h
LiteralSelection.o: ../BlodKorv/ObjDesc.h
LiteralSelection.o: ../BlodKorv/AllocationFailureHandling.h
LiteralSelection.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
LiteralSelection.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
LiteralSelection.o: ../BlodKorv/BufferedAllocationStatistics.h
LiteralSelection.o: VampireKernelConst.h Term.h ../BlodKorv/Comparison.h
LiteralSelection.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
LiteralSelection.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
LiteralSelection.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
LiteralSelection.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
LiteralSelection.o: VampireKernel.h Signature.h /usr/include/string.h
LiteralSelection.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
LiteralSelection.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h tmp_literal.h
LiteralSelection.o: ../BlodKorv/Multiset.h WeightPolynomial.h
LiteralSelection.o: ../BlodKorv/LinearPolynomial.h
LiteralSelection.o: ../BlodKorv/LinearCombination.h
LiteralSelection.o: ../BlodKorv/SharedLinearPolynomial.h
LiteralSelection.o: ../BlodKorv/GDiscTree.h TermWeighting.h Flatterm.h
LiteralSelection.o: ../BlodKorv/MultisetOfVariables.h variables.h
LiteralSelection.o: InstanceRetrievalForDemodulation.h
LiteralSelection.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
LiteralSelection.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
LiteralSelection.o: ../Gematogen/GematogenDebugFlags.h
LiteralSelection.o: ../Gematogen/OptimisedPathIndex.h
LiteralSelection.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
LiteralSharing.o: LiteralSharing.h ../BlodKorv/jargon.h ../BlodKorv/target.h
LiteralSharing.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
LiteralSharing.o: ../BlodKorv/BlodKorvDebugFlags.h
LiteralSharing.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
LiteralSharing.o: /usr/include/stdlib.h /usr/include/features.h
LiteralSharing.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
LiteralSharing.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
LiteralSharing.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
LiteralSharing.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
LiteralSharing.o: ../BlodKorv/Debug.h /usr/include/time.h
LiteralSharing.o: /usr/include/bits/time.h /usr/include/bits/types.h
LiteralSharing.o: ../BlodKorv/ObjDesc.h
LiteralSharing.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
LiteralSharing.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
LiteralSharing.o: ../BlodKorv/Allocator.h
LiteralSharing.o: ../BlodKorv/BufferedAllocationStatistics.h
LiteralSharing.o: ../BlodKorv/Array.h Term.h VampireKernelConst.h
LiteralSharing.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
LiteralSharing.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/BitSet.h
LiteralSharing.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
LiteralSharing.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
LiteralSharing.o: VampireKernel.h Signature.h /usr/include/string.h
LiteralSharing.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
LiteralSharing.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h Flatterm.h
LiteralSharing.o: ../BlodKorv/MultisetOfVariables.h variables.h
LiteralSharing.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
LiteralSharing.o: ../BlodKorv/LinearCombination.h
LiteralSharing.o: ../BlodKorv/SharedLinearPolynomial.h
LiteralSharing.o: ../BlodKorv/GDiscTree.h dtree.h sharing.h
LiteralSharing.o: VampireKernelInterceptMacros.h
Logging.o: Logging.h ../BlodKorv/jargon.h ../BlodKorv/target.h
Logging.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
Logging.o: ../BlodKorv/BlodKorvDebugFlags.h Term.h
Logging.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Logging.o: VampireKernelConst.h /usr/include/limits.h /usr/include/features.h
Logging.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Logging.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Logging.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
Logging.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
Logging.o: /usr/include/time.h /usr/include/bits/time.h
Logging.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
Logging.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
Logging.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
Logging.o: ../BlodKorv/Allocator.h ../BlodKorv/BufferedAllocationStatistics.h
Logging.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
Logging.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
Logging.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
Logging.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h VampireKernel.h
Logging.o: Signature.h /usr/include/string.h SymbolInfo.h
Logging.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
Logging.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h Clause.h
Logging.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h Flatterm.h
Logging.o: ../BlodKorv/MultisetOfVariables.h variables.h WeightPolynomial.h
Logging.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
Logging.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
Master3.o: Master.h ../BlodKorv/jargon.h ../BlodKorv/target.h
Master3.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
Master3.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/GlobAlloc.h
Master3.o: /usr/include/stdlib.h /usr/include/features.h
Master3.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Master3.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Master3.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
Master3.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Master3.o: ../BlodKorv/Debug.h /usr/include/time.h /usr/include/bits/time.h
Master3.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
Master3.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
Master3.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
Master3.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
Master3.o: ../BlodKorv/BufferedAllocationStatistics.h
Master3.o: ../BlodKorv/GlobalStopFlag.h ../BlodKorv/GlobalClock.h
Master3.o: clause_set.h Clause.h ../BlodKorv/GList.h ../BlodKorv/BitWord.h
Master3.o: Term.h VampireKernelConst.h ../BlodKorv/Comparison.h
Master3.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
Master3.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
Master3.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h VampireKernel.h
Master3.o: Signature.h /usr/include/string.h SymbolInfo.h
Master3.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
Master3.o: ../BlodKorv/GSortTree.h ClauseBackground.h LiteralList.h
Master3.o: DefinedHeaderTable.h kept.h sharing.h
Master3.o: VampireKernelInterceptMacros.h ClauseAssembler.h LiteralSharing.h
Master3.o: variables.h ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
Master3.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
Master3.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
Master3.o: NewClause.h prefix.h OpenNewClause.h Flatterm.h tmp_literal.h
Master3.o: ../BlodKorv/Multiset.h TermWeighting.h
Master3.o: InstanceRetrievalForDemodulation.h
Master3.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
Master3.o: ../Gematogen/Gematogen.h ../Gematogen/GematogenDebugFlags.h
Master3.o: ../Gematogen/OptimisedPathIndex.h ../BlodKorv/GIncCountingSort.h
Master3.o: SimplificationOrdering.h TmpLitList.h ForwardDemodulation.h
Master3.o: ../Gematogen/ForwardMatchingIndexTree.h
Master3.o: ../BlodKorv/EquivalenceRelation.h
Master3.o: ../Gematogen/ForwardMatchingIndex.h
Master3.o: ../BlodKorv/ExpandingMultiset.h ForwardMatchingSubstitutionCache.h
Master3.o: ClauseQueue.h ../BlodKorv/FIFO.h PagedPassiveClauseStorage.h
Master3.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h DivIntoComp.h
Master3.o: ForwardSubsumptionFrontEnd.h forward_subsumption.h fs_code.h
Master3.o: fs_command.h old_fs_code.h ../BlodKorv/Queue.h
Master3.o: ../BlodKorv/ObjectPool.h ComponentName.h InferenceDispatcher.h
Master3.o: or_index.h unification.h iu_command.h BackParaCandidates.h
Master3.o: ../BlodKorv/SkipList.h binary_resolution.h premise_factoring.h
Master3.o: or_premise.h or_machine.h iu_machine.h iu_translator.h
Master3.o: weight_formula.h forward_superposition.h para_machine.h
Master3.o: eq_factoring.h backward_superposition.h BackwardSubsumption.h
Master3.o: BSIndex.h ../Gematogen/PathIndex.h BSMachine.h
Master3.o: ../BlodKorv/StaticCellArray.h SubsumptionCheck.h
Master3.o: SimpleSubstitution.h BackDemodulation.h ClauseNumIndex.h
Master3.o: LiteralSelection.h
Master.o: Master.h ../BlodKorv/jargon.h ../BlodKorv/target.h
Master.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
Master.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/GlobAlloc.h
Master.o: /usr/include/stdlib.h /usr/include/features.h
Master.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Master.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Master.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
Master.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Master.o: ../BlodKorv/Debug.h /usr/include/time.h /usr/include/bits/time.h
Master.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
Master.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
Master.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
Master.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
Master.o: ../BlodKorv/BufferedAllocationStatistics.h
Master.o: ../BlodKorv/GlobalStopFlag.h ../BlodKorv/GlobalClock.h clause_set.h
Master.o: Clause.h ../BlodKorv/GList.h ../BlodKorv/BitWord.h Term.h
Master.o: VampireKernelConst.h ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
Master.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
Master.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
Master.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h VampireKernel.h
Master.o: Signature.h /usr/include/string.h SymbolInfo.h
Master.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
Master.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h kept.h
Master.o: sharing.h VampireKernelInterceptMacros.h ClauseAssembler.h
Master.o: LiteralSharing.h variables.h ../BlodKorv/MultisetOfVariables.h
Master.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
Master.o: ../BlodKorv/LinearCombination.h
Master.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
Master.o: NewClause.h prefix.h OpenNewClause.h Flatterm.h tmp_literal.h
Master.o: ../BlodKorv/Multiset.h TermWeighting.h
Master.o: InstanceRetrievalForDemodulation.h
Master.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
Master.o: ../Gematogen/Gematogen.h ../Gematogen/GematogenDebugFlags.h
Master.o: ../Gematogen/OptimisedPathIndex.h ../BlodKorv/GIncCountingSort.h
Master.o: SimplificationOrdering.h TmpLitList.h ForwardDemodulation.h
Master.o: ../Gematogen/ForwardMatchingIndexTree.h
Master.o: ../BlodKorv/EquivalenceRelation.h
Master.o: ../Gematogen/ForwardMatchingIndex.h ../BlodKorv/ExpandingMultiset.h
Master.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h ../BlodKorv/FIFO.h
Master.o: PagedPassiveClauseStorage.h
Master.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h DivIntoComp.h
Master.o: ForwardSubsumptionFrontEnd.h forward_subsumption.h fs_code.h
Master.o: fs_command.h old_fs_code.h ../BlodKorv/Queue.h
Master.o: ../BlodKorv/ObjectPool.h ComponentName.h InferenceDispatcher.h
Master.o: or_index.h unification.h iu_command.h BackParaCandidates.h
Master.o: ../BlodKorv/SkipList.h binary_resolution.h premise_factoring.h
Master.o: or_premise.h or_machine.h iu_machine.h iu_translator.h
Master.o: weight_formula.h forward_superposition.h para_machine.h
Master.o: eq_factoring.h backward_superposition.h BackwardSubsumption.h
Master.o: BSIndex.h ../Gematogen/PathIndex.h BSMachine.h
Master.o: ../BlodKorv/StaticCellArray.h SubsumptionCheck.h
Master.o: SimpleSubstitution.h BackDemodulation.h ClauseNumIndex.h
Master.o: LiteralSelection.h BuiltInTheoryDispatcher.h
NewClause.o: NewClause.h ../BlodKorv/jargon.h ../BlodKorv/target.h
NewClause.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
NewClause.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
NewClause.o: /usr/include/limits.h /usr/include/features.h
NewClause.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
NewClause.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
NewClause.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Term.h
NewClause.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
NewClause.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
NewClause.o: ../BlodKorv/Debug.h /usr/include/time.h /usr/include/bits/time.h
NewClause.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
NewClause.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
NewClause.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
NewClause.o: ../BlodKorv/Allocator.h
NewClause.o: ../BlodKorv/BufferedAllocationStatistics.h
NewClause.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
NewClause.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
NewClause.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
NewClause.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
NewClause.o: TermWeightType.h VampireKernel.h Signature.h
NewClause.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
NewClause.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
NewClause.o: prefix.h variables.h ../BlodKorv/MultisetOfVariables.h
NewClause.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
NewClause.o: ../BlodKorv/LinearCombination.h
NewClause.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
NewClause.o: OpenNewClause.h Flatterm.h tmp_literal.h ../BlodKorv/Multiset.h
NewClause.o: TermWeighting.h InstanceRetrievalForDemodulation.h
NewClause.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
NewClause.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
NewClause.o: ../Gematogen/GematogenDebugFlags.h
NewClause.o: ../Gematogen/OptimisedPathIndex.h ../BlodKorv/GIncCountingSort.h
NewClause.o: SimplificationOrdering.h TmpLitList.h ClauseBackground.h
NewClause.o: ForwardDemodulation.h VampireKernelInterceptMacros.h
NewClause.o: ../Gematogen/ForwardMatchingIndexTree.h
NewClause.o: ../BlodKorv/EquivalenceRelation.h
NewClause.o: ../Gematogen/ForwardMatchingIndex.h
NewClause.o: ../BlodKorv/ExpandingMultiset.h
NewClause.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h
NewClause.o: ../BlodKorv/FIFO.h Clause.h LiteralList.h DefinedHeaderTable.h
NewClause.o: PagedPassiveClauseStorage.h
NewClause.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h
NewClause.o: DivIntoComp.h ForwardSubsumptionFrontEnd.h forward_subsumption.h
NewClause.o: fs_code.h fs_command.h old_fs_code.h ../BlodKorv/Queue.h
NewClause.o: ../BlodKorv/ObjectPool.h ClauseAssembler.h LiteralSharing.h
NewClause.o: BuiltInTheoryDispatcher.h
NonrecursiveKBOrdering.o: NonrecursiveKBOrdering.h ../BlodKorv/jargon.h
NonrecursiveKBOrdering.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
NonrecursiveKBOrdering.o: VampireKernelDebugFlags.h
NonrecursiveKBOrdering.o: ../BlodKorv/BlodKorvDebugFlags.h
NonrecursiveKBOrdering.o: SimplificationOrdering.h ../BlodKorv/Comparison.h
NonrecursiveKBOrdering.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
NonrecursiveKBOrdering.o: /usr/include/time.h /usr/include/features.h
NonrecursiveKBOrdering.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
NonrecursiveKBOrdering.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
NonrecursiveKBOrdering.o: /usr/include/bits/time.h /usr/include/bits/types.h
NonrecursiveKBOrdering.o: ../BlodKorv/RuntimeError.h WeightPolynomial.h
NonrecursiveKBOrdering.o: VampireKernelConst.h /usr/include/limits.h
NonrecursiveKBOrdering.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
NonrecursiveKBOrdering.o: ../BlodKorv/LinearPolynomial.h
NonrecursiveKBOrdering.o: ../BlodKorv/LinearCombination.h ../BlodKorv/Stack.h
NonrecursiveKBOrdering.o: ../BlodKorv/SharedLinearPolynomial.h
NonrecursiveKBOrdering.o: ../BlodKorv/GDiscTree.h
NonrecursiveKBOrdering.o: ../BlodKorv/DestructionMode.h
NonrecursiveKBOrdering.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
NonrecursiveKBOrdering.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
NonrecursiveKBOrdering.o: ../BlodKorv/AllocationFailureHandling.h
NonrecursiveKBOrdering.o: ../BlodKorv/Exit.h ../BlodKorv/Allocator.h
NonrecursiveKBOrdering.o: ../BlodKorv/BufferedAllocationStatistics.h
NonrecursiveKBOrdering.o: TermWeightType.h VampireKernel.h Term.h
NonrecursiveKBOrdering.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
NonrecursiveKBOrdering.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
NonrecursiveKBOrdering.o: ../BlodKorv/BitString.h
NonrecursiveKBOrdering.o: ../BlodKorv/SmallHeaderSkipList.h Signature.h
NonrecursiveKBOrdering.o: /usr/include/string.h SymbolInfo.h
NonrecursiveKBOrdering.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
NonrecursiveKBOrdering.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
NonrecursiveKBOrdering.o: InstanceRetrievalForDemodulation.h
NonrecursiveKBOrdering.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
NonrecursiveKBOrdering.o: ../BlodKorv/GlobalStopFlag.h
NonrecursiveKBOrdering.o: ../Gematogen/Gematogen.h
NonrecursiveKBOrdering.o: ../Gematogen/GematogenDebugFlags.h
NonrecursiveKBOrdering.o: ../Gematogen/OptimisedPathIndex.h
NonrecursiveKBOrdering.o: ../BlodKorv/GIncCountingSort.h
NonrecursiveKBOrdering.o: ../BlodKorv/Multiset.h Flatterm.h
NonrecursiveKBOrdering.o: ../BlodKorv/MultisetOfVariables.h variables.h
NonrecursiveKBOrdering.o: prefix.h
NonrecursiveKBOrdering.o: WeightPolynomialSpecialisedComparisonCommand.h
NonrecursiveKBOrdering.o: ForwardMatchingSubstitutionCache.h TermWeighting.h
NonuniformConstantTermWeighting.o: NonuniformConstantTermWeighting.h
NonuniformConstantTermWeighting.o: ../BlodKorv/jargon.h ../BlodKorv/target.h
NonuniformConstantTermWeighting.o: ../BlodKorv/compiler.h
NonuniformConstantTermWeighting.o: VampireKernelDebugFlags.h
NonuniformConstantTermWeighting.o: ../BlodKorv/BlodKorvDebugFlags.h
NonuniformConstantTermWeighting.o: TermWeighting.h WeightPolynomial.h
NonuniformConstantTermWeighting.o: VampireKernelConst.h /usr/include/limits.h
NonuniformConstantTermWeighting.o: /usr/include/features.h
NonuniformConstantTermWeighting.o: /usr/include/sys/cdefs.h
NonuniformConstantTermWeighting.o: /usr/include/gnu/stubs.h
NonuniformConstantTermWeighting.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
NonuniformConstantTermWeighting.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
NonuniformConstantTermWeighting.o: ../BlodKorv/RuntimeError.h
NonuniformConstantTermWeighting.o: ../BlodKorv/LinearPolynomial.h
NonuniformConstantTermWeighting.o: ../BlodKorv/LinearCombination.h
NonuniformConstantTermWeighting.o: ../BlodKorv/Stack.h
NonuniformConstantTermWeighting.o: ../BlodKorv/debugMacros.h
NonuniformConstantTermWeighting.o: ../BlodKorv/Debug.h /usr/include/time.h
NonuniformConstantTermWeighting.o: /usr/include/bits/time.h
NonuniformConstantTermWeighting.o: /usr/include/bits/types.h
NonuniformConstantTermWeighting.o: ../BlodKorv/Comparison.h
NonuniformConstantTermWeighting.o: ../BlodKorv/SharedLinearPolynomial.h
NonuniformConstantTermWeighting.o: ../BlodKorv/GDiscTree.h
NonuniformConstantTermWeighting.o: ../BlodKorv/DestructionMode.h
NonuniformConstantTermWeighting.o: ../BlodKorv/GlobAlloc.h
NonuniformConstantTermWeighting.o: /usr/include/stdlib.h
NonuniformConstantTermWeighting.o: ../BlodKorv/ClassDesc.h
NonuniformConstantTermWeighting.o: ../BlodKorv/ObjDesc.h
NonuniformConstantTermWeighting.o: ../BlodKorv/AllocationFailureHandling.h
NonuniformConstantTermWeighting.o: ../BlodKorv/Exit.h ../BlodKorv/Allocator.h
NonuniformConstantTermWeighting.o: ../BlodKorv/BufferedAllocationStatistics.h
NonuniformConstantTermWeighting.o: TermWeightType.h VampireKernel.h
NonuniformConstantTermWeighting.o: Flatterm.h Term.h
NonuniformConstantTermWeighting.o: ../BlodKorv/ExpandingStack.h
NonuniformConstantTermWeighting.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
NonuniformConstantTermWeighting.o: ../BlodKorv/BitWord.h
NonuniformConstantTermWeighting.o: ../BlodKorv/BitString.h
NonuniformConstantTermWeighting.o: ../BlodKorv/SmallHeaderSkipList.h
NonuniformConstantTermWeighting.o: Signature.h /usr/include/string.h
NonuniformConstantTermWeighting.o: SymbolInfo.h AutoFunctorPrecedence.h
NonuniformConstantTermWeighting.o: ../BlodKorv/Tuple.h
NonuniformConstantTermWeighting.o: ../BlodKorv/GSortTree.h
NonuniformConstantTermWeighting.o: ../BlodKorv/GList.h
NonuniformConstantTermWeighting.o: ../BlodKorv/MultisetOfVariables.h
NonuniformConstantTermWeighting.o: variables.h
NonuniformConstantTermWeighting.o: InstanceRetrievalForDemodulation.h
NonuniformConstantTermWeighting.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
NonuniformConstantTermWeighting.o: ../BlodKorv/GlobalStopFlag.h
NonuniformConstantTermWeighting.o: ../Gematogen/Gematogen.h
NonuniformConstantTermWeighting.o: ../Gematogen/GematogenDebugFlags.h
NonuniformConstantTermWeighting.o: ../Gematogen/OptimisedPathIndex.h
NonuniformConstantTermWeighting.o: ../BlodKorv/GIncCountingSort.h
NonuniformConstantTermWeighting.o: ../BlodKorv/Multiset.h prefix.h
NonuniformConstantTermWeighting.o: ForwardMatchingSubstitutionCache.h
OldBackwardSubsumption.o: OldBackwardSubsumption.h ../BlodKorv/jargon.h
OldBackwardSubsumption.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
OldBackwardSubsumption.o: VampireKernelDebugFlags.h
OldBackwardSubsumption.o: ../BlodKorv/BlodKorvDebugFlags.h
OldBackwardSubsumption.o: VampireKernelConst.h /usr/include/limits.h
OldBackwardSubsumption.o: /usr/include/features.h /usr/include/sys/cdefs.h
OldBackwardSubsumption.o: /usr/include/gnu/stubs.h
OldBackwardSubsumption.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
OldBackwardSubsumption.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
OldBackwardSubsumption.o: OldBSIndex.h ../BlodKorv/DestructionMode.h Term.h
OldBackwardSubsumption.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
OldBackwardSubsumption.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
OldBackwardSubsumption.o: /usr/include/time.h /usr/include/bits/time.h
OldBackwardSubsumption.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
OldBackwardSubsumption.o: ../BlodKorv/AllocationFailureHandling.h
OldBackwardSubsumption.o: ../BlodKorv/Exit.h ../BlodKorv/RuntimeError.h
OldBackwardSubsumption.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
OldBackwardSubsumption.o: ../BlodKorv/BufferedAllocationStatistics.h
OldBackwardSubsumption.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
OldBackwardSubsumption.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
OldBackwardSubsumption.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
OldBackwardSubsumption.o: ../BlodKorv/BitString.h
OldBackwardSubsumption.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
OldBackwardSubsumption.o: VampireKernel.h Signature.h /usr/include/string.h
OldBackwardSubsumption.o: SymbolInfo.h AutoFunctorPrecedence.h
OldBackwardSubsumption.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
OldBackwardSubsumption.o: ../BlodKorv/GList.h DiscTree.h OldBSMachine.h
OldBackwardSubsumption.o: Clause.h ClauseBackground.h LiteralList.h
OldBackwardSubsumption.o: DefinedHeaderTable.h ClauseNumIndex.h
OldBackwardSubsumption.o: ../BlodKorv/SkipList.h SubsumptionCheck.h
OldBackwardSubsumption.o: SimpleSubstitution.h
OldBSIndex.o: OldBSIndex.h ../BlodKorv/jargon.h ../BlodKorv/target.h
OldBSIndex.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
OldBSIndex.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
OldBSIndex.o: /usr/include/limits.h /usr/include/features.h
OldBSIndex.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
OldBSIndex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
OldBSIndex.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
OldBSIndex.o: ../BlodKorv/DestructionMode.h Term.h ../BlodKorv/GlobAlloc.h
OldBSIndex.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
OldBSIndex.o: ../BlodKorv/Debug.h /usr/include/time.h
OldBSIndex.o: /usr/include/bits/time.h /usr/include/bits/types.h
OldBSIndex.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
OldBSIndex.o: ../BlodKorv/Exit.h ../BlodKorv/RuntimeError.h
OldBSIndex.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
OldBSIndex.o: ../BlodKorv/BufferedAllocationStatistics.h
OldBSIndex.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
OldBSIndex.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
OldBSIndex.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
OldBSIndex.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
OldBSIndex.o: TermWeightType.h VampireKernel.h Signature.h
OldBSIndex.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
OldBSIndex.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
OldBSIndex.o: DiscTree.h
old_fs_code.o: old_fs_code.h ../BlodKorv/jargon.h ../BlodKorv/target.h
old_fs_code.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
old_fs_code.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
old_fs_code.o: /usr/include/limits.h /usr/include/features.h
old_fs_code.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
old_fs_code.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
old_fs_code.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
old_fs_code.o: ../BlodKorv/MultisetOfVariables.h ../BlodKorv/Stack.h
old_fs_code.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
old_fs_code.o: /usr/include/time.h /usr/include/bits/time.h
old_fs_code.o: /usr/include/bits/types.h ../BlodKorv/Comparison.h
old_fs_code.o: fs_command.h ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
old_fs_code.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
old_fs_code.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
old_fs_code.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
old_fs_code.o: ../BlodKorv/Allocator.h
old_fs_code.o: ../BlodKorv/BufferedAllocationStatistics.h Term.h
old_fs_code.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
old_fs_code.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
old_fs_code.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
old_fs_code.o: TermWeightType.h VampireKernel.h Signature.h
old_fs_code.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
old_fs_code.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
old_fs_code.o: ../BlodKorv/GList.h ../BlodKorv/Queue.h Clause.h
old_fs_code.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h
old_fs_code.o: Flatterm.h variables.h WeightPolynomial.h
old_fs_code.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
old_fs_code.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
old_fs_code.o: TmpLitList.h tmp_literal.h ../BlodKorv/Multiset.h
old_fs_code.o: TermWeighting.h InstanceRetrievalForDemodulation.h
old_fs_code.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
old_fs_code.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
old_fs_code.o: ../Gematogen/GematogenDebugFlags.h
old_fs_code.o: ../Gematogen/OptimisedPathIndex.h
old_fs_code.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
or_index.o: or_index.h ../BlodKorv/jargon.h ../BlodKorv/target.h
or_index.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
or_index.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/GlobAlloc.h
or_index.o: /usr/include/stdlib.h /usr/include/features.h
or_index.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
or_index.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
or_index.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
or_index.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
or_index.o: ../BlodKorv/Debug.h /usr/include/time.h /usr/include/bits/time.h
or_index.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
or_index.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
or_index.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
or_index.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
or_index.o: ../BlodKorv/BufferedAllocationStatistics.h ../BlodKorv/Array.h
or_index.o: VampireKernelConst.h Term.h ../BlodKorv/Comparison.h
or_index.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
or_index.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
or_index.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
or_index.o: TermWeightType.h VampireKernel.h Signature.h
or_index.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
or_index.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
or_index.o: prefix.h variables.h ../BlodKorv/MultisetOfVariables.h
or_index.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
or_index.o: ../BlodKorv/LinearCombination.h
or_index.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
or_index.o: unification.h iu_command.h BackParaCandidates.h
or_index.o: ../BlodKorv/SkipList.h SimplificationOrdering.h
or_index.o: InstanceRetrievalForDemodulation.h
or_index.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
or_index.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
or_index.o: ../Gematogen/GematogenDebugFlags.h
or_index.o: ../Gematogen/OptimisedPathIndex.h ../BlodKorv/GIncCountingSort.h
or_index.o: ../BlodKorv/Multiset.h Clause.h ClauseBackground.h LiteralList.h
or_index.o: DefinedHeaderTable.h
or_premise.o: or_premise.h ../BlodKorv/jargon.h ../BlodKorv/target.h
or_premise.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
or_premise.o: ../BlodKorv/BlodKorvDebugFlags.h Clause.h
or_premise.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
or_premise.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
or_premise.o: /usr/include/features.h /usr/include/sys/cdefs.h
or_premise.o: /usr/include/gnu/stubs.h ../BlodKorv/ClassDesc.h
or_premise.o: /usr/include/limits.h
or_premise.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
or_premise.o: ../BlodKorv/Debug.h /usr/include/time.h
or_premise.o: /usr/include/bits/time.h /usr/include/bits/types.h
or_premise.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
or_premise.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
or_premise.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
or_premise.o: ../BlodKorv/Allocator.h
or_premise.o: ../BlodKorv/BufferedAllocationStatistics.h ../BlodKorv/GList.h
or_premise.o: ../BlodKorv/BitWord.h Term.h VampireKernelConst.h
or_premise.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
or_premise.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
or_premise.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
or_premise.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
or_premise.o: VampireKernel.h Signature.h /usr/include/string.h SymbolInfo.h
or_premise.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
or_premise.o: ../BlodKorv/GSortTree.h ClauseBackground.h LiteralList.h
or_premise.o: DefinedHeaderTable.h prefix.h variables.h
or_premise.o: ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
or_premise.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
or_premise.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
or_premise.o: or_index.h unification.h iu_command.h BackParaCandidates.h
or_premise.o: ../BlodKorv/SkipList.h SimplificationOrdering.h
or_premise.o: InstanceRetrievalForDemodulation.h
or_premise.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
or_premise.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
or_premise.o: ../Gematogen/GematogenDebugFlags.h
or_premise.o: ../Gematogen/OptimisedPathIndex.h
or_premise.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
or_premise.o: NewClause.h OpenNewClause.h Flatterm.h tmp_literal.h
or_premise.o: TermWeighting.h TmpLitList.h ForwardDemodulation.h
or_premise.o: VampireKernelInterceptMacros.h
or_premise.o: ../Gematogen/ForwardMatchingIndexTree.h
or_premise.o: ../BlodKorv/EquivalenceRelation.h
or_premise.o: ../Gematogen/ForwardMatchingIndex.h
or_premise.o: ../BlodKorv/ExpandingMultiset.h
or_premise.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h
or_premise.o: ../BlodKorv/FIFO.h PagedPassiveClauseStorage.h
or_premise.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h
or_premise.o: DivIntoComp.h
PagedPassiveClauseStorage.o: PagedPassiveClauseStorage.h ../BlodKorv/jargon.h
PagedPassiveClauseStorage.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
PagedPassiveClauseStorage.o: VampireKernelDebugFlags.h
PagedPassiveClauseStorage.o: ../BlodKorv/BlodKorvDebugFlags.h
PagedPassiveClauseStorage.o: VampireKernelConst.h /usr/include/limits.h
PagedPassiveClauseStorage.o: /usr/include/features.h /usr/include/sys/cdefs.h
PagedPassiveClauseStorage.o: /usr/include/gnu/stubs.h
PagedPassiveClauseStorage.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
PagedPassiveClauseStorage.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
PagedPassiveClauseStorage.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
PagedPassiveClauseStorage.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
PagedPassiveClauseStorage.o: /usr/include/time.h /usr/include/bits/time.h
PagedPassiveClauseStorage.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
PagedPassiveClauseStorage.o: ../BlodKorv/AllocationFailureHandling.h
PagedPassiveClauseStorage.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
PagedPassiveClauseStorage.o: ../BlodKorv/RuntimeError.h
PagedPassiveClauseStorage.o: ../BlodKorv/debugMacros.h
PagedPassiveClauseStorage.o: ../BlodKorv/Allocator.h
PagedPassiveClauseStorage.o: ../BlodKorv/BufferedAllocationStatistics.h
PagedPassiveClauseStorage.o: ../BlodKorv/BitWord.h
PagedPassiveClauseStorage.o: ../BlodKorv/PageQueueDisciplineStorage.h Term.h
PagedPassiveClauseStorage.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
PagedPassiveClauseStorage.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
PagedPassiveClauseStorage.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
PagedPassiveClauseStorage.o: ../BlodKorv/SmallHeaderSkipList.h
PagedPassiveClauseStorage.o: TermWeightType.h VampireKernel.h Signature.h
PagedPassiveClauseStorage.o: /usr/include/string.h SymbolInfo.h
PagedPassiveClauseStorage.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
PagedPassiveClauseStorage.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
PagedPassiveClauseStorage.o: ClauseBackground.h Clause.h LiteralList.h
PagedPassiveClauseStorage.o: DefinedHeaderTable.h
Parser.o: /usr/include/stdlib.h /usr/include/features.h
Parser.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Parser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Parser.h
Parser.o: /usr/include/string.h ../BlodKorv/jargon.h ../BlodKorv/target.h
Parser.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
Parser.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/DestructionMode.h
Parser.o: ../BlodKorv/GList.h ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
Parser.o: /usr/include/time.h /usr/include/bits/time.h
Parser.o: /usr/include/bits/types.h ../BlodKorv/Malloc.h
Parser.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
Parser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Parser.o: ../BlodKorv/ObjDesc.h Input.h /usr/include/stdio.h
Parser.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
Parser.o: /usr/include/bits/wchar.h /usr/include/gconv.h
Parser.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
Parser.o: /usr/include/bits/stdio_lim.h VampireKernel.h
Parser.o: PR_PARSER/pr_abstract_syntax.h PR_PARSER/pr_parser.h
prefix.o: prefix.h ../BlodKorv/jargon.h ../BlodKorv/target.h
prefix.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
prefix.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
prefix.o: /usr/include/limits.h /usr/include/features.h
prefix.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
prefix.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
prefix.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h Term.h
prefix.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
prefix.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
prefix.o: /usr/include/time.h /usr/include/bits/time.h
prefix.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
prefix.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
prefix.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
prefix.o: ../BlodKorv/Allocator.h ../BlodKorv/BufferedAllocationStatistics.h
prefix.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
prefix.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
prefix.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
prefix.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h VampireKernel.h
prefix.o: Signature.h /usr/include/string.h SymbolInfo.h
prefix.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
prefix.o: ../BlodKorv/GList.h variables.h ../BlodKorv/MultisetOfVariables.h
prefix.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
prefix.o: ../BlodKorv/LinearCombination.h
prefix.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
premise_factoring.o: premise_factoring.h ../BlodKorv/jargon.h
premise_factoring.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
premise_factoring.o: VampireKernelDebugFlags.h
premise_factoring.o: ../BlodKorv/BlodKorvDebugFlags.h or_premise.h Clause.h
premise_factoring.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
premise_factoring.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
premise_factoring.o: /usr/include/features.h /usr/include/sys/cdefs.h
premise_factoring.o: /usr/include/gnu/stubs.h ../BlodKorv/ClassDesc.h
premise_factoring.o: /usr/include/limits.h
premise_factoring.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
premise_factoring.o: ../BlodKorv/Debug.h /usr/include/time.h
premise_factoring.o: /usr/include/bits/time.h /usr/include/bits/types.h
premise_factoring.o: ../BlodKorv/ObjDesc.h
premise_factoring.o: ../BlodKorv/AllocationFailureHandling.h
premise_factoring.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
premise_factoring.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
premise_factoring.o: ../BlodKorv/Allocator.h
premise_factoring.o: ../BlodKorv/BufferedAllocationStatistics.h
premise_factoring.o: ../BlodKorv/GList.h ../BlodKorv/BitWord.h Term.h
premise_factoring.o: VampireKernelConst.h ../BlodKorv/Comparison.h
premise_factoring.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
premise_factoring.o: ../BlodKorv/Array.h ../BlodKorv/BitSet.h
premise_factoring.o: ../BlodKorv/BitString.h
premise_factoring.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
premise_factoring.o: VampireKernel.h Signature.h /usr/include/string.h
premise_factoring.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
premise_factoring.o: ../BlodKorv/GSortTree.h ClauseBackground.h LiteralList.h
premise_factoring.o: DefinedHeaderTable.h prefix.h variables.h
premise_factoring.o: ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
premise_factoring.o: ../BlodKorv/LinearPolynomial.h
premise_factoring.o: ../BlodKorv/LinearCombination.h
premise_factoring.o: ../BlodKorv/SharedLinearPolynomial.h
premise_factoring.o: ../BlodKorv/GDiscTree.h or_index.h unification.h
premise_factoring.o: iu_command.h BackParaCandidates.h ../BlodKorv/SkipList.h
premise_factoring.o: SimplificationOrdering.h
premise_factoring.o: InstanceRetrievalForDemodulation.h
premise_factoring.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
premise_factoring.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
premise_factoring.o: ../Gematogen/GematogenDebugFlags.h
premise_factoring.o: ../Gematogen/OptimisedPathIndex.h
premise_factoring.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
premise_factoring.o: NewClause.h OpenNewClause.h Flatterm.h tmp_literal.h
premise_factoring.o: TermWeighting.h TmpLitList.h ForwardDemodulation.h
premise_factoring.o: VampireKernelInterceptMacros.h
premise_factoring.o: ../Gematogen/ForwardMatchingIndexTree.h
premise_factoring.o: ../BlodKorv/EquivalenceRelation.h
premise_factoring.o: ../Gematogen/ForwardMatchingIndex.h
premise_factoring.o: ../BlodKorv/ExpandingMultiset.h
premise_factoring.o: ForwardMatchingSubstitutionCache.h ClauseQueue.h
premise_factoring.o: ../BlodKorv/FIFO.h PagedPassiveClauseStorage.h
premise_factoring.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h
premise_factoring.o: DivIntoComp.h
ProblemProfile.o: ProblemProfile.h ../BlodKorv/jargon.h ../BlodKorv/target.h
ProblemProfile.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
ProblemProfile.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/GlobAlloc.h
ProblemProfile.o: /usr/include/stdlib.h /usr/include/features.h
ProblemProfile.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ProblemProfile.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
ProblemProfile.o: ../BlodKorv/ClassDesc.h /usr/include/limits.h
ProblemProfile.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
ProblemProfile.o: ../BlodKorv/Debug.h /usr/include/time.h
ProblemProfile.o: /usr/include/bits/time.h /usr/include/bits/types.h
ProblemProfile.o: ../BlodKorv/ObjDesc.h
ProblemProfile.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
ProblemProfile.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
ProblemProfile.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
ProblemProfile.o: ../BlodKorv/BufferedAllocationStatistics.h
ProblemProfile.o: ../BlodKorv/Array.h ClauseQueue.h ../BlodKorv/FIFO.h
ProblemProfile.o: ../BlodKorv/GList.h Clause.h ../BlodKorv/BitWord.h Term.h
ProblemProfile.o: VampireKernelConst.h ../BlodKorv/Comparison.h
ProblemProfile.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
ProblemProfile.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
ProblemProfile.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
ProblemProfile.o: VampireKernel.h Signature.h /usr/include/string.h
ProblemProfile.o: SymbolInfo.h AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
ProblemProfile.o: ../BlodKorv/GSortTree.h ClauseBackground.h LiteralList.h
ProblemProfile.o: DefinedHeaderTable.h
sharing.o: sharing.h /usr/include/limits.h /usr/include/features.h
sharing.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
sharing.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
sharing.o: ../BlodKorv/jargon.h ../BlodKorv/target.h ../BlodKorv/compiler.h
sharing.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
sharing.o: VampireKernelConst.h
sharing.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
sharing.o: ../BlodKorv/DestructionMode.h VampireKernelInterceptMacros.h
sharing.o: ../BlodKorv/Exit.h /usr/include/stdlib.h Signature.h
sharing.o: /usr/include/string.h ../BlodKorv/Array.h
sharing.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h /usr/include/time.h
sharing.o: /usr/include/bits/time.h /usr/include/bits/types.h
sharing.o: ../BlodKorv/GlobAlloc.h ../BlodKorv/ClassDesc.h
sharing.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
sharing.o: ../BlodKorv/RuntimeError.h ../BlodKorv/Allocator.h
sharing.o: ../BlodKorv/BufferedAllocationStatistics.h
sharing.o: ../BlodKorv/Comparison.h SymbolInfo.h AutoFunctorPrecedence.h
sharing.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
sharing.o: TermWeightType.h VampireKernel.h Term.h ../BlodKorv/Stack.h
sharing.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/BitSet.h
sharing.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
sharing.o: ../BlodKorv/SmallHeaderSkipList.h Flatterm.h
sharing.o: ../BlodKorv/MultisetOfVariables.h variables.h WeightPolynomial.h
sharing.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
sharing.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
Signature.o: /usr/include/stdlib.h /usr/include/features.h
Signature.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Signature.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Signature.o: Signature.h /usr/include/string.h ../BlodKorv/jargon.h
Signature.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
Signature.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
Signature.o: VampireKernelConst.h /usr/include/limits.h
Signature.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Signature.o: ../BlodKorv/Array.h ../BlodKorv/DestructionMode.h
Signature.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
Signature.o: /usr/include/time.h /usr/include/bits/time.h
Signature.o: /usr/include/bits/types.h ../BlodKorv/GlobAlloc.h
Signature.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
Signature.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
Signature.o: ../BlodKorv/RuntimeError.h ../BlodKorv/Allocator.h
Signature.o: ../BlodKorv/BufferedAllocationStatistics.h
Signature.o: ../BlodKorv/Comparison.h SymbolInfo.h AutoFunctorPrecedence.h
Signature.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
Signature.o: TermWeightType.h VampireKernel.h Tabulation.h Term.h
Signature.o: ../BlodKorv/Stack.h ../BlodKorv/ExpandingStack.h
Signature.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
Signature.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
Signature.o: Clause.h ClauseBackground.h LiteralList.h DefinedHeaderTable.h
SimplificationOrdering.o: SimplificationOrdering.h ../BlodKorv/jargon.h
SimplificationOrdering.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
SimplificationOrdering.o: VampireKernelDebugFlags.h
SimplificationOrdering.o: ../BlodKorv/BlodKorvDebugFlags.h
SimplificationOrdering.o: ../BlodKorv/Comparison.h ../BlodKorv/debugMacros.h
SimplificationOrdering.o: ../BlodKorv/Debug.h /usr/include/time.h
SimplificationOrdering.o: /usr/include/features.h /usr/include/sys/cdefs.h
SimplificationOrdering.o: /usr/include/gnu/stubs.h
SimplificationOrdering.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
SimplificationOrdering.o: /usr/include/bits/time.h /usr/include/bits/types.h
SimplificationOrdering.o: ../BlodKorv/RuntimeError.h WeightPolynomial.h
SimplificationOrdering.o: VampireKernelConst.h /usr/include/limits.h
SimplificationOrdering.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
SimplificationOrdering.o: ../BlodKorv/LinearPolynomial.h
SimplificationOrdering.o: ../BlodKorv/LinearCombination.h ../BlodKorv/Stack.h
SimplificationOrdering.o: ../BlodKorv/SharedLinearPolynomial.h
SimplificationOrdering.o: ../BlodKorv/GDiscTree.h
SimplificationOrdering.o: ../BlodKorv/DestructionMode.h
SimplificationOrdering.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
SimplificationOrdering.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
SimplificationOrdering.o: ../BlodKorv/AllocationFailureHandling.h
SimplificationOrdering.o: ../BlodKorv/Exit.h ../BlodKorv/Allocator.h
SimplificationOrdering.o: ../BlodKorv/BufferedAllocationStatistics.h
SimplificationOrdering.o: TermWeightType.h VampireKernel.h Term.h
SimplificationOrdering.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
SimplificationOrdering.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
SimplificationOrdering.o: ../BlodKorv/BitString.h
SimplificationOrdering.o: ../BlodKorv/SmallHeaderSkipList.h Signature.h
SimplificationOrdering.o: /usr/include/string.h SymbolInfo.h
SimplificationOrdering.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
SimplificationOrdering.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
SimplificationOrdering.o: InstanceRetrievalForDemodulation.h
SimplificationOrdering.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
SimplificationOrdering.o: ../BlodKorv/GlobalStopFlag.h
SimplificationOrdering.o: ../Gematogen/Gematogen.h
SimplificationOrdering.o: ../Gematogen/GematogenDebugFlags.h
SimplificationOrdering.o: ../Gematogen/OptimisedPathIndex.h
SimplificationOrdering.o: ../BlodKorv/GIncCountingSort.h
SimplificationOrdering.o: ../BlodKorv/Multiset.h NonrecursiveKBOrdering.h
SimplificationOrdering.o: Flatterm.h ../BlodKorv/MultisetOfVariables.h
SimplificationOrdering.o: variables.h prefix.h
SimplificationOrdering.o: WeightPolynomialSpecialisedComparisonCommand.h
SimplificationOrdering.o: ForwardMatchingSubstitutionCache.h
Splitting3.o: Splitting.h ../BlodKorv/jargon.h ../BlodKorv/target.h
Splitting3.o: ../BlodKorv/compiler.h ../BlodKorv/RuntimeError.h
Splitting3.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
Splitting3.o: VampireKernelConst.h /usr/include/limits.h
Splitting3.o: /usr/include/features.h /usr/include/sys/cdefs.h
Splitting3.o: /usr/include/gnu/stubs.h
Splitting3.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Splitting3.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Splitting3.o: ../BlodKorv/Array.h /usr/include/stdlib.h
Splitting3.o: ../BlodKorv/DestructionMode.h ../BlodKorv/debugMacros.h
Splitting3.o: ../BlodKorv/Debug.h /usr/include/time.h
Splitting3.o: /usr/include/bits/time.h /usr/include/bits/types.h
Splitting3.o: ../BlodKorv/ExpandingMultiset.h ../BlodKorv/ExpandingStack.h
Splitting3.o: ../BlodKorv/GlobAlloc.h ../BlodKorv/ClassDesc.h
Splitting3.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
Splitting3.o: ../BlodKorv/Exit.h ../BlodKorv/Allocator.h
Splitting3.o: ../BlodKorv/BufferedAllocationStatistics.h DivIntoComp.h
Splitting3.o: ../BlodKorv/Multiset.h ../BlodKorv/Stack.h
Splitting3.o: ../BlodKorv/Comparison.h ClauseQueue.h ../BlodKorv/FIFO.h
Splitting3.o: ../BlodKorv/GList.h Clause.h ../BlodKorv/BitWord.h Term.h
Splitting3.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
Splitting3.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
Splitting3.o: VampireKernel.h Signature.h /usr/include/string.h SymbolInfo.h
Splitting3.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
Splitting3.o: ../BlodKorv/GSortTree.h ClauseBackground.h LiteralList.h
Splitting3.o: DefinedHeaderTable.h TmpLitList.h tmp_literal.h
Splitting3.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
Splitting3.o: ../BlodKorv/LinearCombination.h
Splitting3.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
Splitting3.o: TermWeighting.h Flatterm.h ../BlodKorv/MultisetOfVariables.h
Splitting3.o: variables.h InstanceRetrievalForDemodulation.h
Splitting3.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
Splitting3.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
Splitting3.o: ../Gematogen/GematogenDebugFlags.h
Splitting3.o: ../Gematogen/OptimisedPathIndex.h
Splitting3.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
Splitting3.o: OpenNewClause.h ForwardSubsumptionFrontEnd.h
Splitting3.o: forward_subsumption.h fs_code.h
Splitting3.o: ../BlodKorv/EquivalenceRelation.h fs_command.h old_fs_code.h
Splitting3.o: ../BlodKorv/Queue.h ../BlodKorv/ObjectPool.h ComponentName.h
Splitting3.o: PagedPassiveClauseStorage.h
Splitting3.o: ../BlodKorv/PageQueueDisciplineStorage.h ClauseAssembler.h
Splitting3.o: LiteralSharing.h
Splitting.o: Splitting.h ../BlodKorv/jargon.h ../BlodKorv/target.h
Splitting.o: ../BlodKorv/compiler.h ../BlodKorv/RuntimeError.h
Splitting.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
Splitting.o: VampireKernelConst.h /usr/include/limits.h
Splitting.o: /usr/include/features.h /usr/include/sys/cdefs.h
Splitting.o: /usr/include/gnu/stubs.h
Splitting.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Splitting.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Splitting.o: ../BlodKorv/Array.h /usr/include/stdlib.h
Splitting.o: ../BlodKorv/DestructionMode.h ../BlodKorv/debugMacros.h
Splitting.o: ../BlodKorv/Debug.h /usr/include/time.h /usr/include/bits/time.h
Splitting.o: /usr/include/bits/types.h ../BlodKorv/ExpandingMultiset.h
Splitting.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/GlobAlloc.h
Splitting.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
Splitting.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
Splitting.o: ../BlodKorv/Allocator.h
Splitting.o: ../BlodKorv/BufferedAllocationStatistics.h DivIntoComp.h
Splitting.o: ../BlodKorv/Multiset.h ../BlodKorv/Stack.h
Splitting.o: ../BlodKorv/Comparison.h ClauseQueue.h ../BlodKorv/FIFO.h
Splitting.o: ../BlodKorv/GList.h Clause.h ../BlodKorv/BitWord.h Term.h
Splitting.o: ../BlodKorv/BitSet.h ../BlodKorv/BitString.h
Splitting.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h
Splitting.o: VampireKernel.h Signature.h /usr/include/string.h SymbolInfo.h
Splitting.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
Splitting.o: ../BlodKorv/GSortTree.h ClauseBackground.h LiteralList.h
Splitting.o: DefinedHeaderTable.h TmpLitList.h tmp_literal.h
Splitting.o: WeightPolynomial.h ../BlodKorv/LinearPolynomial.h
Splitting.o: ../BlodKorv/LinearCombination.h
Splitting.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
Splitting.o: TermWeighting.h Flatterm.h ../BlodKorv/MultisetOfVariables.h
Splitting.o: variables.h InstanceRetrievalForDemodulation.h
Splitting.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
Splitting.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
Splitting.o: ../Gematogen/GematogenDebugFlags.h
Splitting.o: ../Gematogen/OptimisedPathIndex.h ../BlodKorv/GIncCountingSort.h
Splitting.o: SimplificationOrdering.h OpenNewClause.h
Splitting.o: ForwardSubsumptionFrontEnd.h forward_subsumption.h fs_code.h
Splitting.o: ../BlodKorv/EquivalenceRelation.h fs_command.h old_fs_code.h
Splitting.o: ../BlodKorv/Queue.h ../BlodKorv/ObjectPool.h ComponentName.h
Splitting.o: PagedPassiveClauseStorage.h
Splitting.o: ../BlodKorv/PageQueueDisciplineStorage.h ClauseAssembler.h
Splitting.o: LiteralSharing.h
Standalone.o: /usr/include/time.h /usr/include/features.h
Standalone.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Standalone.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Standalone.o: /usr/include/bits/time.h /usr/include/bits/types.h
Standalone.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
Standalone.o: /usr/include/bits/confname.h /usr/include/limits.h
Standalone.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Standalone.o: /usr/include/signal.h /usr/include/bits/sigset.h
Standalone.o: /usr/include/bits/signum.h ../BlodKorv/jargon.h
Standalone.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
Standalone.o: ../BlodKorv/RuntimeError.h ../BlodKorv/GlobalClock.h
Standalone.o: ../BlodKorv/Exit.h /usr/include/stdlib.h
Standalone.o: ../BlodKorv/GlobAlloc.h ../BlodKorv/BlodKorvDebugFlags.h
Standalone.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
Standalone.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
Standalone.o: ../BlodKorv/DestructionMode.h ../BlodKorv/debugMacros.h
Standalone.o: ../BlodKorv/Allocator.h
Standalone.o: ../BlodKorv/BufferedAllocationStatistics.h
Standalone.o: VampireKernelDebugFlags.h VampireKernel.h
Standalone.o: ../BlodKorv/CommandLine.h /usr/include/string.h
Standalone.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
Standalone.o: ../BlodKorv/GList.h ../BlodKorv/Malloc.h Input.h
Standalone.o: /usr/include/stdio.h /usr/include/libio.h
Standalone.o: /usr/include/_G_config.h /usr/include/wchar.h
Standalone.o: /usr/include/bits/wchar.h /usr/include/gconv.h
Standalone.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
Standalone.o: /usr/include/bits/stdio_lim.h Parser.h
Standalone.o: PR_PARSER/pr_abstract_syntax.h PR_PARSER/pr_parser.h
Standalone.o: Tabulation.h Term.h VampireKernelConst.h
Standalone.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
Standalone.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
Standalone.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
Standalone.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
Standalone.o: TermWeightType.h Signature.h SymbolInfo.h
Standalone.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
Standalone.o: ../BlodKorv/GSortTree.h Clause.h ClauseBackground.h
Standalone.o: LiteralList.h DefinedHeaderTable.h
StandardKBOrdering.o: StandardKBOrdering.h ../BlodKorv/jargon.h
StandardKBOrdering.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
StandardKBOrdering.o: VampireKernelDebugFlags.h
StandardKBOrdering.o: ../BlodKorv/BlodKorvDebugFlags.h
StandardKBOrdering.o: SimplificationOrdering.h ../BlodKorv/Comparison.h
StandardKBOrdering.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
StandardKBOrdering.o: /usr/include/time.h /usr/include/features.h
StandardKBOrdering.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
StandardKBOrdering.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
StandardKBOrdering.o: /usr/include/bits/time.h /usr/include/bits/types.h
StandardKBOrdering.o: ../BlodKorv/RuntimeError.h WeightPolynomial.h
StandardKBOrdering.o: VampireKernelConst.h /usr/include/limits.h
StandardKBOrdering.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
StandardKBOrdering.o: ../BlodKorv/LinearPolynomial.h
StandardKBOrdering.o: ../BlodKorv/LinearCombination.h ../BlodKorv/Stack.h
StandardKBOrdering.o: ../BlodKorv/SharedLinearPolynomial.h
StandardKBOrdering.o: ../BlodKorv/GDiscTree.h ../BlodKorv/DestructionMode.h
StandardKBOrdering.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
StandardKBOrdering.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
StandardKBOrdering.o: ../BlodKorv/AllocationFailureHandling.h
StandardKBOrdering.o: ../BlodKorv/Exit.h ../BlodKorv/Allocator.h
StandardKBOrdering.o: ../BlodKorv/BufferedAllocationStatistics.h
StandardKBOrdering.o: TermWeightType.h VampireKernel.h Term.h
StandardKBOrdering.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
StandardKBOrdering.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
StandardKBOrdering.o: ../BlodKorv/BitString.h
StandardKBOrdering.o: ../BlodKorv/SmallHeaderSkipList.h Signature.h
StandardKBOrdering.o: /usr/include/string.h SymbolInfo.h
StandardKBOrdering.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
StandardKBOrdering.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
StandardKBOrdering.o: InstanceRetrievalForDemodulation.h
StandardKBOrdering.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
StandardKBOrdering.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
StandardKBOrdering.o: ../Gematogen/GematogenDebugFlags.h
StandardKBOrdering.o: ../Gematogen/OptimisedPathIndex.h
StandardKBOrdering.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
StandardKBOrdering.o: Flatterm.h ../BlodKorv/MultisetOfVariables.h
StandardKBOrdering.o: variables.h prefix.h ../BlodKorv/ObjectPool.h
StandardKBOrdering.o: WeightPolynomialSpecialisedComparisonCommand.h
StandardKBOrdering.o: ForwardMatchingSubstitutionCache.h TermWeighting.h
Term.o: Term.h /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
Term.o: ../BlodKorv/jargon.h ../BlodKorv/target.h ../BlodKorv/compiler.h
Term.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
Term.o: VampireKernelConst.h /usr/include/limits.h /usr/include/features.h
Term.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
Term.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
Term.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
Term.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
Term.o: /usr/include/time.h /usr/include/bits/time.h
Term.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
Term.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
Term.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
Term.o: ../BlodKorv/Allocator.h ../BlodKorv/BufferedAllocationStatistics.h
Term.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
Term.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h ../BlodKorv/BitSet.h
Term.o: ../BlodKorv/BitWord.h ../BlodKorv/BitString.h
Term.o: ../BlodKorv/SmallHeaderSkipList.h TermWeightType.h VampireKernel.h
Term.o: Signature.h /usr/include/string.h SymbolInfo.h
Term.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
Term.o: ../BlodKorv/GList.h
TermWeighting.o: TermWeighting.h WeightPolynomial.h ../BlodKorv/jargon.h
TermWeighting.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
TermWeighting.o: VampireKernelDebugFlags.h ../BlodKorv/BlodKorvDebugFlags.h
TermWeighting.o: VampireKernelConst.h /usr/include/limits.h
TermWeighting.o: /usr/include/features.h /usr/include/sys/cdefs.h
TermWeighting.o: /usr/include/gnu/stubs.h
TermWeighting.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
TermWeighting.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
TermWeighting.o: ../BlodKorv/RuntimeError.h ../BlodKorv/LinearPolynomial.h
TermWeighting.o: ../BlodKorv/LinearCombination.h ../BlodKorv/Stack.h
TermWeighting.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
TermWeighting.o: /usr/include/time.h /usr/include/bits/time.h
TermWeighting.o: /usr/include/bits/types.h ../BlodKorv/Comparison.h
TermWeighting.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
TermWeighting.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
TermWeighting.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
TermWeighting.o: ../BlodKorv/ObjDesc.h
TermWeighting.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
TermWeighting.o: ../BlodKorv/Allocator.h
TermWeighting.o: ../BlodKorv/BufferedAllocationStatistics.h TermWeightType.h
TermWeighting.o: VampireKernel.h Flatterm.h Term.h
TermWeighting.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
TermWeighting.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
TermWeighting.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
TermWeighting.o: Signature.h /usr/include/string.h SymbolInfo.h
TermWeighting.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
TermWeighting.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
TermWeighting.o: ../BlodKorv/MultisetOfVariables.h variables.h
TermWeighting.o: InstanceRetrievalForDemodulation.h
TermWeighting.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
TermWeighting.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
TermWeighting.o: ../Gematogen/GematogenDebugFlags.h
TermWeighting.o: ../Gematogen/OptimisedPathIndex.h
TermWeighting.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
tmp_literal.o: tmp_literal.h ../BlodKorv/jargon.h ../BlodKorv/target.h
tmp_literal.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
tmp_literal.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
tmp_literal.o: /usr/include/limits.h /usr/include/features.h
tmp_literal.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
tmp_literal.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
tmp_literal.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
tmp_literal.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
tmp_literal.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
tmp_literal.o: /usr/include/time.h /usr/include/bits/time.h
tmp_literal.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
tmp_literal.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
tmp_literal.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
tmp_literal.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
tmp_literal.o: ../BlodKorv/BufferedAllocationStatistics.h Term.h
tmp_literal.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
tmp_literal.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
tmp_literal.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
tmp_literal.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
tmp_literal.o: TermWeightType.h VampireKernel.h Signature.h
tmp_literal.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
tmp_literal.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
tmp_literal.o: ../BlodKorv/GList.h ../BlodKorv/Multiset.h WeightPolynomial.h
tmp_literal.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
tmp_literal.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
tmp_literal.o: TermWeighting.h Flatterm.h ../BlodKorv/MultisetOfVariables.h
tmp_literal.o: variables.h InstanceRetrievalForDemodulation.h
tmp_literal.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
tmp_literal.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
tmp_literal.o: ../Gematogen/GematogenDebugFlags.h
tmp_literal.o: ../Gematogen/OptimisedPathIndex.h
tmp_literal.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
TmpLitList.o: TmpLitList.h ../BlodKorv/jargon.h ../BlodKorv/target.h
TmpLitList.o: ../BlodKorv/compiler.h ../BlodKorv/GlobAlloc.h
TmpLitList.o: /usr/include/stdlib.h /usr/include/features.h
TmpLitList.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
TmpLitList.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
TmpLitList.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/ClassDesc.h
TmpLitList.o: /usr/include/limits.h
TmpLitList.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
TmpLitList.o: ../BlodKorv/Debug.h /usr/include/time.h
TmpLitList.o: /usr/include/bits/time.h /usr/include/bits/types.h
TmpLitList.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
TmpLitList.o: ../BlodKorv/Exit.h ../BlodKorv/DestructionMode.h
TmpLitList.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
TmpLitList.o: ../BlodKorv/Allocator.h
TmpLitList.o: ../BlodKorv/BufferedAllocationStatistics.h
TmpLitList.o: VampireKernelDebugFlags.h VampireKernelConst.h Term.h
TmpLitList.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
TmpLitList.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
TmpLitList.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
TmpLitList.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
TmpLitList.o: TermWeightType.h VampireKernel.h Signature.h
TmpLitList.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
TmpLitList.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
TmpLitList.o: tmp_literal.h ../BlodKorv/Multiset.h WeightPolynomial.h
TmpLitList.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
TmpLitList.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
TmpLitList.o: TermWeighting.h Flatterm.h ../BlodKorv/MultisetOfVariables.h
TmpLitList.o: variables.h InstanceRetrievalForDemodulation.h
TmpLitList.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
TmpLitList.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
TmpLitList.o: ../Gematogen/GematogenDebugFlags.h
TmpLitList.o: ../Gematogen/OptimisedPathIndex.h
TmpLitList.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
UniformTermWeighting.o: UniformTermWeighting.h ../BlodKorv/jargon.h
UniformTermWeighting.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
UniformTermWeighting.o: VampireKernelDebugFlags.h
UniformTermWeighting.o: ../BlodKorv/BlodKorvDebugFlags.h TermWeighting.h
UniformTermWeighting.o: WeightPolynomial.h VampireKernelConst.h
UniformTermWeighting.o: /usr/include/limits.h /usr/include/features.h
UniformTermWeighting.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
UniformTermWeighting.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
UniformTermWeighting.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
UniformTermWeighting.o: ../BlodKorv/RuntimeError.h
UniformTermWeighting.o: ../BlodKorv/LinearPolynomial.h
UniformTermWeighting.o: ../BlodKorv/LinearCombination.h ../BlodKorv/Stack.h
UniformTermWeighting.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
UniformTermWeighting.o: /usr/include/time.h /usr/include/bits/time.h
UniformTermWeighting.o: /usr/include/bits/types.h ../BlodKorv/Comparison.h
UniformTermWeighting.o: ../BlodKorv/SharedLinearPolynomial.h
UniformTermWeighting.o: ../BlodKorv/GDiscTree.h ../BlodKorv/DestructionMode.h
UniformTermWeighting.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
UniformTermWeighting.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
UniformTermWeighting.o: ../BlodKorv/AllocationFailureHandling.h
UniformTermWeighting.o: ../BlodKorv/Exit.h ../BlodKorv/Allocator.h
UniformTermWeighting.o: ../BlodKorv/BufferedAllocationStatistics.h
UniformTermWeighting.o: TermWeightType.h VampireKernel.h Flatterm.h Term.h
UniformTermWeighting.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
UniformTermWeighting.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
UniformTermWeighting.o: ../BlodKorv/BitString.h
UniformTermWeighting.o: ../BlodKorv/SmallHeaderSkipList.h Signature.h
UniformTermWeighting.o: /usr/include/string.h SymbolInfo.h
UniformTermWeighting.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
UniformTermWeighting.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h
UniformTermWeighting.o: ../BlodKorv/MultisetOfVariables.h variables.h
UniformTermWeighting.o: InstanceRetrievalForDemodulation.h
UniformTermWeighting.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
UniformTermWeighting.o: ../BlodKorv/GlobalStopFlag.h ../Gematogen/Gematogen.h
UniformTermWeighting.o: ../Gematogen/GematogenDebugFlags.h
UniformTermWeighting.o: ../Gematogen/OptimisedPathIndex.h
UniformTermWeighting.o: ../BlodKorv/GIncCountingSort.h ../BlodKorv/Multiset.h
UniformTermWeighting.o: prefix.h ForwardMatchingSubstitutionCache.h
VampireKernel.o: /usr/include/limits.h /usr/include/features.h
VampireKernel.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
VampireKernel.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
VampireKernel.o: /usr/include/stdio.h
VampireKernel.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
VampireKernel.o: /usr/include/bits/types.h /usr/include/libio.h
VampireKernel.o: /usr/include/_G_config.h /usr/include/wchar.h
VampireKernel.o: /usr/include/bits/wchar.h /usr/include/gconv.h
VampireKernel.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h
VampireKernel.o: /usr/include/bits/stdio_lim.h ../BlodKorv/GlobAlloc.h
VampireKernel.o: /usr/include/stdlib.h ../BlodKorv/jargon.h
VampireKernel.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
VampireKernel.o: ../BlodKorv/BlodKorvDebugFlags.h ../BlodKorv/ClassDesc.h
VampireKernel.o: ../BlodKorv/Debug.h /usr/include/time.h
VampireKernel.o: /usr/include/bits/time.h ../BlodKorv/ObjDesc.h
VampireKernel.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
VampireKernel.o: ../BlodKorv/DestructionMode.h ../BlodKorv/RuntimeError.h
VampireKernel.o: ../BlodKorv/debugMacros.h ../BlodKorv/Allocator.h
VampireKernel.o: ../BlodKorv/BufferedAllocationStatistics.h
VampireKernel.o: ../BlodKorv/GlobalClock.h ../BlodKorv/GlobalStopFlag.h
VampireKernel.o: VampireKernel.h Signature.h /usr/include/string.h
VampireKernel.o: VampireKernelDebugFlags.h VampireKernelConst.h
VampireKernel.o: ../BlodKorv/Array.h ../BlodKorv/Comparison.h SymbolInfo.h
VampireKernel.o: AutoFunctorPrecedence.h ../BlodKorv/Tuple.h
VampireKernel.o: ../BlodKorv/GSortTree.h ../BlodKorv/GList.h TermWeightType.h
VampireKernel.o: Clause.h ../BlodKorv/BitWord.h Term.h ../BlodKorv/Stack.h
VampireKernel.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/BitSet.h
VampireKernel.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
VampireKernel.o: ClauseBackground.h LiteralList.h DefinedHeaderTable.h
VampireKernel.o: ClauseQueue.h ../BlodKorv/FIFO.h ProblemProfile.h
VampireKernel.o: Tabulation.h Master.h clause_set.h kept.h sharing.h
VampireKernel.o: VampireKernelInterceptMacros.h ClauseAssembler.h
VampireKernel.o: LiteralSharing.h variables.h
VampireKernel.o: ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
VampireKernel.o: ../BlodKorv/LinearPolynomial.h
VampireKernel.o: ../BlodKorv/LinearCombination.h
VampireKernel.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
VampireKernel.o: NewClause.h prefix.h OpenNewClause.h Flatterm.h
VampireKernel.o: tmp_literal.h ../BlodKorv/Multiset.h TermWeighting.h
VampireKernel.o: InstanceRetrievalForDemodulation.h
VampireKernel.o: ../Gematogen/InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h
VampireKernel.o: ../Gematogen/Gematogen.h ../Gematogen/GematogenDebugFlags.h
VampireKernel.o: ../Gematogen/OptimisedPathIndex.h
VampireKernel.o: ../BlodKorv/GIncCountingSort.h SimplificationOrdering.h
VampireKernel.o: TmpLitList.h ForwardDemodulation.h
VampireKernel.o: ../Gematogen/ForwardMatchingIndexTree.h
VampireKernel.o: ../BlodKorv/EquivalenceRelation.h
VampireKernel.o: ../Gematogen/ForwardMatchingIndex.h
VampireKernel.o: ../BlodKorv/ExpandingMultiset.h
VampireKernel.o: ForwardMatchingSubstitutionCache.h
VampireKernel.o: PagedPassiveClauseStorage.h
VampireKernel.o: ../BlodKorv/PageQueueDisciplineStorage.h Splitting.h
VampireKernel.o: DivIntoComp.h ForwardSubsumptionFrontEnd.h
VampireKernel.o: forward_subsumption.h fs_code.h fs_command.h old_fs_code.h
VampireKernel.o: ../BlodKorv/Queue.h ../BlodKorv/ObjectPool.h ComponentName.h
VampireKernel.o: InferenceDispatcher.h or_index.h unification.h iu_command.h
VampireKernel.o: BackParaCandidates.h ../BlodKorv/SkipList.h
VampireKernel.o: binary_resolution.h premise_factoring.h or_premise.h
VampireKernel.o: or_machine.h iu_machine.h iu_translator.h weight_formula.h
VampireKernel.o: forward_superposition.h para_machine.h eq_factoring.h
VampireKernel.o: backward_superposition.h BackwardSubsumption.h BSIndex.h
VampireKernel.o: ../Gematogen/PathIndex.h BSMachine.h
VampireKernel.o: ../BlodKorv/StaticCellArray.h SubsumptionCheck.h
VampireKernel.o: SimpleSubstitution.h BackDemodulation.h ClauseNumIndex.h
VampireKernel.o: LiteralSelection.h UniformTermWeighting.h
VampireKernel.o: NonuniformConstantTermWeighting.h NonrecursiveKBOrdering.h
VampireKernel.o: WeightPolynomialSpecialisedComparisonCommand.h
VampireKernel.o: StandardKBOrdering.h InferenceTree.h
VampireKernel.o: BuiltInTheoryDispatcher.h
variables.o: variables.h ../BlodKorv/jargon.h ../BlodKorv/target.h
variables.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
variables.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
variables.o: /usr/include/limits.h /usr/include/features.h
variables.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
variables.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
variables.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
variables.o: ../BlodKorv/MultisetOfVariables.h ../BlodKorv/Stack.h
variables.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
variables.o: /usr/include/time.h /usr/include/bits/time.h
variables.o: /usr/include/bits/types.h ../BlodKorv/Comparison.h
variables.o: WeightPolynomial.h ../BlodKorv/RuntimeError.h
variables.o: ../BlodKorv/LinearPolynomial.h ../BlodKorv/LinearCombination.h
variables.o: ../BlodKorv/SharedLinearPolynomial.h ../BlodKorv/GDiscTree.h
variables.o: ../BlodKorv/DestructionMode.h ../BlodKorv/GlobAlloc.h
variables.o: /usr/include/stdlib.h ../BlodKorv/ClassDesc.h
variables.o: ../BlodKorv/ObjDesc.h ../BlodKorv/AllocationFailureHandling.h
variables.o: ../BlodKorv/Exit.h ../BlodKorv/Allocator.h
variables.o: ../BlodKorv/BufferedAllocationStatistics.h TermWeightType.h
variables.o: VampireKernel.h
weight_formula.o: weight_formula.h ../BlodKorv/jargon.h ../BlodKorv/target.h
weight_formula.o: ../BlodKorv/compiler.h VampireKernelDebugFlags.h
weight_formula.o: ../BlodKorv/BlodKorvDebugFlags.h prefix.h
weight_formula.o: VampireKernelConst.h /usr/include/limits.h
weight_formula.o: /usr/include/features.h /usr/include/sys/cdefs.h
weight_formula.o: /usr/include/gnu/stubs.h
weight_formula.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
weight_formula.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
weight_formula.o: Term.h ../BlodKorv/DestructionMode.h
weight_formula.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
weight_formula.o: ../BlodKorv/ClassDesc.h ../BlodKorv/Debug.h
weight_formula.o: /usr/include/time.h /usr/include/bits/time.h
weight_formula.o: /usr/include/bits/types.h ../BlodKorv/ObjDesc.h
weight_formula.o: ../BlodKorv/AllocationFailureHandling.h ../BlodKorv/Exit.h
weight_formula.o: ../BlodKorv/RuntimeError.h ../BlodKorv/debugMacros.h
weight_formula.o: ../BlodKorv/Allocator.h
weight_formula.o: ../BlodKorv/BufferedAllocationStatistics.h
weight_formula.o: ../BlodKorv/Comparison.h ../BlodKorv/Stack.h
weight_formula.o: ../BlodKorv/ExpandingStack.h ../BlodKorv/Array.h
weight_formula.o: ../BlodKorv/BitSet.h ../BlodKorv/BitWord.h
weight_formula.o: ../BlodKorv/BitString.h ../BlodKorv/SmallHeaderSkipList.h
weight_formula.o: TermWeightType.h VampireKernel.h Signature.h
weight_formula.o: /usr/include/string.h SymbolInfo.h AutoFunctorPrecedence.h
weight_formula.o: ../BlodKorv/Tuple.h ../BlodKorv/GSortTree.h
weight_formula.o: ../BlodKorv/GList.h variables.h
weight_formula.o: ../BlodKorv/MultisetOfVariables.h WeightPolynomial.h
weight_formula.o: ../BlodKorv/LinearPolynomial.h
weight_formula.o: ../BlodKorv/LinearCombination.h
weight_formula.o: ../BlodKorv/SharedLinearPolynomial.h
weight_formula.o: ../BlodKorv/GDiscTree.h
WeightPolynomial.o: WeightPolynomial.h ../BlodKorv/jargon.h
WeightPolynomial.o: ../BlodKorv/target.h ../BlodKorv/compiler.h
WeightPolynomial.o: VampireKernelDebugFlags.h
WeightPolynomial.o: ../BlodKorv/BlodKorvDebugFlags.h VampireKernelConst.h
WeightPolynomial.o: /usr/include/limits.h /usr/include/features.h
WeightPolynomial.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
WeightPolynomial.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
WeightPolynomial.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
WeightPolynomial.o: ../BlodKorv/RuntimeError.h ../BlodKorv/LinearPolynomial.h
WeightPolynomial.o: ../BlodKorv/LinearCombination.h ../BlodKorv/Stack.h
WeightPolynomial.o: ../BlodKorv/debugMacros.h ../BlodKorv/Debug.h
WeightPolynomial.o: /usr/include/time.h /usr/include/bits/time.h
WeightPolynomial.o: /usr/include/bits/types.h ../BlodKorv/Comparison.h
WeightPolynomial.o: ../BlodKorv/SharedLinearPolynomial.h
WeightPolynomial.o: ../BlodKorv/GDiscTree.h ../BlodKorv/DestructionMode.h
WeightPolynomial.o: ../BlodKorv/GlobAlloc.h /usr/include/stdlib.h
WeightPolynomial.o: ../BlodKorv/ClassDesc.h ../BlodKorv/ObjDesc.h
WeightPolynomial.o: ../BlodKorv/AllocationFailureHandling.h
WeightPolynomial.o: ../BlodKorv/Exit.h ../BlodKorv/Allocator.h
WeightPolynomial.o: ../BlodKorv/BufferedAllocationStatistics.h
WeightPolynomial.o: TermWeightType.h VampireKernel.h
WeightPolynomial.o: WeightPolynomialSpecialisedComparisonCommand.h
WeightPolynomialSpecialisedComparisonCommand.o: WeightPolynomialSpecialisedComparisonCommand.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/jargon.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/target.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/compiler.h
WeightPolynomialSpecialisedComparisonCommand.o: VampireKernelDebugFlags.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/BlodKorvDebugFlags.h
WeightPolynomialSpecialisedComparisonCommand.o: VampireKernelConst.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/include/limits.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/include/features.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/include/sys/cdefs.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/include/gnu/stubs.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h
WeightPolynomialSpecialisedComparisonCommand.o: WeightPolynomial.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/RuntimeError.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/LinearPolynomial.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/LinearCombination.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/Stack.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/debugMacros.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/Debug.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/include/time.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/include/bits/time.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/include/bits/types.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/Comparison.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/SharedLinearPolynomial.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/GDiscTree.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/DestructionMode.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/GlobAlloc.h
WeightPolynomialSpecialisedComparisonCommand.o: /usr/include/stdlib.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/ClassDesc.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/ObjDesc.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/AllocationFailureHandling.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/Exit.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/Allocator.h
WeightPolynomialSpecialisedComparisonCommand.o: ../BlodKorv/BufferedAllocationStatistics.h
WeightPolynomialSpecialisedComparisonCommand.o: TermWeightType.h
WeightPolynomialSpecialisedComparisonCommand.o: VampireKernel.h

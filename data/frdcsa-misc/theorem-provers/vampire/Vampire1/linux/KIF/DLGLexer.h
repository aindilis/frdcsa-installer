#ifndef DLGLexer_h
#define DLGLexer_h
/*
 * D L G L e x e r  C l a s s  D e f i n i t i o n
 *
 * Generated from: parser.dlg
 *
 * 1989-1998 by  Will Cohen, Terence Parr, and Hank Dietz
 * Purdue University Electrical Engineering
 * DLG Version 1.33MR18
 */


#include "DLexerBase.h"

class DLGLexer : public DLGLexerBase {
public:
public:
	static const int MAX_MODE;
	static const int DfaStates;
	static const int START;
	typedef unsigned char DfaState;

	DLGLexer(DLGInputStream *in,
		unsigned bufsize=2000)
		: DLGLexerBase(in, bufsize, 0)
	{
	;
	}
	void	  mode(int);
	ANTLRTokenType nextTokenType(void);
	void     advance(void);
protected:
	ANTLRTokenType act1();
	ANTLRTokenType act2();
	ANTLRTokenType act3();
	ANTLRTokenType act4();
	ANTLRTokenType act5();
	ANTLRTokenType act6();
	ANTLRTokenType act7();
	ANTLRTokenType act8();
	ANTLRTokenType act9();
	ANTLRTokenType act10();
	ANTLRTokenType act11();
	ANTLRTokenType act12();
	ANTLRTokenType act13();
	ANTLRTokenType act14();
	ANTLRTokenType act15();
	ANTLRTokenType act16();
	ANTLRTokenType act17();
	ANTLRTokenType act18();
	ANTLRTokenType act19();
	ANTLRTokenType act20();
	ANTLRTokenType act21();
	ANTLRTokenType act22();
	ANTLRTokenType act23();
	ANTLRTokenType act24();
	ANTLRTokenType act25();
	static DfaState st0[258];
	static DfaState st1[258];
	static DfaState st2[258];
	static DfaState st3[258];
	static DfaState st4[258];
	static DfaState st5[258];
	static DfaState st6[258];
	static DfaState st7[258];
	static DfaState st8[258];
	static DfaState st9[258];
	static DfaState st10[258];
	static DfaState st11[258];
	static DfaState st12[258];
	static DfaState st13[258];
	static DfaState st14[258];
	static DfaState st15[258];
	static DfaState st16[258];
	static DfaState st17[258];
	static DfaState st18[258];
	static DfaState st19[258];
	static DfaState st20[258];
	static DfaState st21[258];
	static DfaState st22[258];
	static DfaState st23[258];
	static DfaState st24[258];
	static DfaState st25[258];
	static DfaState st26[258];
	static DfaState st27[258];
	static DfaState st28[258];
	static DfaState st29[258];
	static DfaState st30[258];
	static DfaState st31[258];
	static DfaState st32[258];
	static DfaState st33[258];
	static DfaState st34[258];
	static DfaState st35[258];
	static DfaState st36[258];
	static DfaState st37[258];
	static DfaState st38[258];
	static DfaState st39[258];
	static DfaState st40[258];
	static DfaState st41[258];
	static DfaState st42[258];
	static DfaState st43[258];
	static DfaState st44[258];
	static DfaState st45[258];
	static DfaState st46[258];
	static DfaState st47[258];
	static DfaState st48[258];
	static DfaState st49[258];
	static DfaState st50[258];
	static DfaState st51[258];
	static DfaState st52[258];
	static DfaState st53[258];
	static DfaState *dfa[54];
	static DfaState dfa_base[];
	static unsigned char *b_class_no[];
	static DfaState accepts[55];
	static DLGChar alternatives[55];
	static ANTLRTokenType (DLGLexer::*actions[26])();
	static unsigned char shift0[257];
	int ZZSHIFT(int c) { return 1+c; }
//
// 133MR1 Deprecated feature to allow inclusion of user-defined code in DLG class header
//
#ifdef DLGLexerIncludeFile
#include DLGLexerIncludeFile
#endif
};
typedef ANTLRTokenType (DLGLexer::*PtrDLGLexerMemberFunc)();
#endif

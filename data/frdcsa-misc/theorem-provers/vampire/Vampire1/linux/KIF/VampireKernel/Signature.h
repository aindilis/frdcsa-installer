//
// File:         Signature.h
// Description:  Various signature related operations.  
// Created:      Apr 20, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      May 20, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               Bug fix in tabulation.
//====================================================
#ifndef SIGNATURE_H  
//=================================================
#define SIGNATURE_H
//#include <stddef.h>
#include <iostream.h>
#include <string.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE 
 #include "ClassDesc.h"
#endif
#include "VampireKernelConst.h"
#include "Array.h"
#include "GlobAlloc.h"
#include "Comparison.h"
#include "SymbolInfo.h"
#include "AutoFunctorPrecedence.h"
#include "TermWeightType.h"
//=================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIGNATURE
 #define DEBUG_NAMESPACE "Signature"
#endif 
#include "debugMacros.h"
//=================================================  
namespace VK
{

  using namespace BK;

class Tabulation;
class Clause;
class Signature
{
 public: 
  class InitStatic
    {
    public: 
      InitStatic()
	{ 
	  if (!_count) 
	    {
	      _count++;
	    };  
	};
      ~InitStatic() {};
    private:
      static long _count;   
    }; // class InitStatic 
  
 public:
  Signature();
  ~Signature() 
    {
      CALL("destructor ~Signature()");
      if (DestructionMode::isThorough()) destroy();
    };

  static Signature* current() { return _current; };
  static void setCurrent(Signature* sig) { _current = sig; };
 
  void destroy();

  // Registering new symbols: 

  void resetInputVariables(); 
  ulong inputVariableNumber(const char* varName);
  ulong inputSymbolNumber(bool predicate,const char* funName,ulong arity);
  ulong numericConstantNumber(const double& value);

  // Getting symbol properties: 

  bool isPredicate(ulong funNum) { return _isPredicate.sub(funNum); }; 
 

  bool weightIsSet(ulong funNum) 
    { 
      return (funNum < _weightIsSet.size()) && _weightIsSet[funNum];
    }; 
  ulong arity(ulong funNum) const
    {
      if (funNum >= _arity.size()) return 0UL;
      return _arity[funNum]; 
    };  
  bool isSkolem(ulong funNum) const 
    {
      if (funNum >= _isInputSymbol.size()) return false;
      return !_isInputSymbol[funNum]; 
    };   
  bool isCommutative(ulong funNum) 
    {
      if (funNum >= _isCommutative.size()) return false;
      return _isCommutative[funNum]; 
    };

  bool isNumericConstant(ulong funNum) const 
    {
      CALL("isNumericConstant(ulong funNum) const");
      return (funNum < _isNumericConstant.size()) &&
	_isNumericConstant[funNum];
    };

  const char* symbolName(ulong funNum) const
    { 
      CALL("symbolName(ulong funNum) const");
      ASSERT(!isNumericConstant(funNum));
      ASSERT(funNum < _symbolName.size());
      return _symbolName[funNum]; 
    };  
  

  const double& numericConstantValue(ulong funNum) const
    {
      CALL("numericConstantValue(ulong funNum) const");
      ASSERT(isNumericConstant(funNum));
      ASSERT(funNum < _numericConstantValue.size());
      return _numericConstantValue[funNum];
    };


  ostream& outputSymbol(ostream& str,ulong funNum)
    {
      if (isNumericConstant(funNum)) return str << numericConstantValue(funNum);
      return str << symbolName(funNum);
    };


  bool isAnswerPredicate(ulong num) const
    {
      return (num < _isAnswerPredicate.size()) &&
	_isAnswerPredicate[num];
    };

  // Setting/changing symbol properties:
   
  void makeCommutative(ulong funNum);
  void makeNoncommutative(ulong funNum);
  void setCommutativity(const SymbolInfo* commutativityTable,
                        const SymbolInfo* symmetryTable);

  
  void makeAnswerPredicate(ulong num)
    {
      CALL("makeAnswerPredicate(ulong num)");
      ASSERT((num < _isPredicate.size()) && _isPredicate[num]);
      _isAnswerPredicate.sub(num) = true;
    };
  // Operations on the signature as a whole:
  
  void endOfInput();
  
  bool answerPredicatesExist() const
    {
      return _isAnswerPredicate.size();
    };
  

  void setMaxlNumOfSkolemSymbols(long n) { _maxlNumOfSkolemSymbols = n; };
  ulong nextSymbolNumber() const { return _nextSymbolNumber; };

  bool canCreateNSkolemSymbols(ulong n) 
    {  
      return _numberOfSkolemSymbols  <= (long)(_maxlNumOfSkolemSymbols - n);  
    };

  bool createNewSkolemConstantNumber(ulong& res);
  
  bool createNewSkolemFunctionNumber(ulong arity,ulong& res);

  bool createNewSkolemPredicateNumber(ulong arity,ulong& res);
 
  const char* canonicalVariableName(ulong varNum)
    {
      CALL("canonicalVariableName(ulong varNum)");
      return _canonicalVariableName[varNum];
    };

  // Weights and precedences of symbols and headers
  
  TermWeightType autoWeight(ulong arity)
    {
      if (_symbolWeightByArity >= 0)
	{
	  return (TermWeightType)(1 + _symbolWeightByArity * arity);
	}
      else
	return 
	  (TermWeightType)(1 - _symbolWeightByArity*(VampireKernelConst::MaxSymbolArity - arity));

    };

  TermWeightType weightConstantPart(ulong funNum)
    {
      CALL("weightConstantPart(ulong funNum)");
      if (funNum >= _weightConstantPart.size()) return (TermWeightType)1;
      return _weightConstantPart[funNum]; 
    };

  void setSymbolWeightConstantPart(ulong funNum,const TermWeightType& weight)
    {
      _weightConstantPart.sub(funNum) = weight;
      _weightIsSet.sub(funNum) = true;
    };


  void setDependenceOfPrecedenceOnArity(long typeOfDependence)
    {
      _autoSymbolPrecedence.setDependenceOnArity(typeOfDependence);
    };

  void setDependenceOfWeightOnArity(long n)
    {
      _symbolWeightByArity = n;
    };

  void adjustHeaderPrecedenceForPositiveSelection()
    {
      _autoSymbolPrecedence.setInversePolarityFlag(true);
    };

  void makeHeaderPrecedenceKinky()
    {
      _autoSymbolPrecedence.setKinkyHeaderPrecedenceFlag(true);
    };


  void setInputSymbolPrecedence(ulong symNum,long inputPrecedence)
    {
      CALL("setPrecedence(ulong symNum,long inputPrecedence)");
      _autoSymbolPrecedence.assignFixed(symNum,inputPrecedence);
    };

  void assignMaximalPrecedence(ulong symNum)
    {
      _autoSymbolPrecedence.assignMaximalPrecedence(symNum);
    };

  static long maximalPossibleSymbolPrecedence()
    {
      return AutoFunctorPrecedence::maximalPrecedence();
    };



  bool precedenceIsReserved(long precedence,ulong& funNum)
    {
      return _autoSymbolPrecedence.precedenceIsReserved(precedence,funNum);
    };
  
  void assignMinimalPrecedence(ulong polarity,ulong predNum)
    {
      CALL("assignMinimalPrecedence(ulong polarity,ulong predNum)");
      ASSERT((polarity == PositivePolarity) || (polarity == NegativePolarity));     
      _minimalHeaderPrecedence--;
      setHeaderPrecedence(polarity,predNum,_minimalHeaderPrecedence);
    };

  void assignMaximalPrecedence(ulong polarity,ulong predNum)
    {
      CALL("assignMaximalPrecedence(ulong polarity,ulong predNum)");
      ASSERT((polarity == PositivePolarity) || (polarity == NegativePolarity));     
      _maximalHeaderPrecedence++;
      setHeaderPrecedence(polarity,predNum,_maximalHeaderPrecedence);
    };

  long symbolPrecedence(ulong funNum)
    {
      CALL("symbolPrecedence(ulong funNum)");
      if (funNum >= _positiveHeaderPrecedence.size()) return 0L;
      return _positiveHeaderPrecedence[funNum];
    };

  long headerPrecedence(ulong polarity,ulong predNum)
    {
      CALL("headerPrecedence(ulong polarity,ulong predNum)");
      ASSERT((polarity == PositivePolarity) || (polarity == NegativePolarity)); 
      if (polarity)
	{
	  if (predNum >= _negativeHeaderPrecedence.size()) return 0L;
	  return _negativeHeaderPrecedence[predNum];
	}
      else
	{
	  if (predNum >= _positiveHeaderPrecedence.size())  return 0L;
	  return _positiveHeaderPrecedence[predNum];
	};
    };
  
  Comparison compareHeaderPrecedences(ulong polarity1,ulong predNum1,ulong polarity2,ulong predNum2)
    {
      CALL("compareHeaderPrecedences(ulong polarity1,ulong predNum1,ulong polarity2,ulong predNum2)");
      ASSERT((polarity1 == PositivePolarity) || (polarity1 == NegativePolarity));
      ASSERT((polarity2 == PositivePolarity) || (polarity2 == NegativePolarity));  
      long precedenceDifference = headerPrecedence(polarity1,predNum1) - headerPrecedence(polarity2,predNum2);
      if (precedenceDifference)
	{ 
	  return (precedenceDifference > 0) ? Greater : Less; 
	}
      else 
	return Equal;
    };      
  ostream& output(ostream& str);

  void tabulate(Tabulation* tabulation);

  // Simple integrity check
  
#ifndef NO_DEBUG
  bool checkObj()
    {
      // check arrays: 
      if (!_inputVariableName.checkObj()) return false;
      if (!_symbolName.checkObj()) return false;
      if (!_isInputSymbol.checkObj()) return false;
      if (!_isPredicate.checkObj()) return false;
      if (!_isCommutative.checkObj()) return false;
      if (!_isAnswerPredicate.checkObj()) return false;      
      if (!_arity.checkObj()) return false;
      if (!_weightIsSet.checkObj()) return false;
      if (!_weightConstantPart.checkObj()) return false;
      if (!_positiveHeaderPrecedence.checkObj()) return false;  
      if (!_negativeHeaderPrecedence.checkObj()) return false;
      return true;
    }; // bool checkObj()    
#endif  

 public:
  static const ulong PositivePolarity = 0UL;
  static const ulong NegativePolarity = 1UL;
  static const long DefaultHeaderPrecedence = 0L; 
  static const long MinimalPossibleHeaderPrecedence = LONG_MIN/4; 
  static const long MaximalPossibleHeaderPrecedence = LONG_MAX/4;  

 private:
  typedef Array<GlobAlloc,char*,128UL,Signature> StrArray;
  typedef Array<GlobAlloc,double,128UL,Signature> DoubleArray;
  typedef Array<GlobAlloc,bool,128UL,Signature> BoolArray;
  typedef Array<GlobAlloc,ulong,128UL,Signature> ULongArray;
  typedef Array<GlobAlloc,long,128UL,Signature> LongArray;
  class WeightArrayId {};
  typedef Array<GlobAlloc,TermWeightType,128UL,WeightArrayId> WeightArray;
  class WeightPtrArrayId {};

 private: 

  void setHeaderPrecedence(ulong polarity,ulong predNum,long precedence)
    {
      CALL("setHeaderPrecedence(ulong polarity,ulong predNum,long precedence)");   
      ASSERT((polarity == PositivePolarity) || (polarity == NegativePolarity)); 
      if (polarity) { _negativeHeaderPrecedence.sub(predNum) = precedence; }
      else _positiveHeaderPrecedence.sub(predNum) = precedence;
    };
  bool reservedSymbolNumber(ulong funNum) 
    {
      return (funNum == VampireKernelConst::UnordEqNum) 
	|| (funNum == VampireKernelConst::OrdEqNum) 
	|| (funNum == VampireKernelConst::BuiltInTrueNum)
	|| (funNum == VampireKernelConst::ReservedPred0)  
	|| (funNum == VampireKernelConst::ReservedPred1)  
	|| (funNum == VampireKernelConst::ReservedPred2) 
	;                   
    };
  static void print(ulong num,char*& target)
    {
      if (!num) { target[0] = '0'; target++; return; }; 
      char lastDigit;
      switch (num % 10) 
	{   
	case 0UL: lastDigit = '0'; break;
	case 1UL: lastDigit = '1'; break;
	case 2UL: lastDigit = '2'; break;   
	case 3UL: lastDigit = '3'; break;
	case 4UL: lastDigit = '4'; break;
	case 5UL: lastDigit = '5'; break;   
	case 6UL: lastDigit = '6'; break;
	case 7UL: lastDigit = '7'; break;
	case 8UL: lastDigit = '8'; break;
	case 9UL: lastDigit = '9'; break;
#ifdef DEBUG_NAMESPACE
	default: ICP("ICP0"); return; 
#else
#ifdef _SUPPRESS_WARNINGS_
	default: return;  
#endif
#endif 
	};
      num = num / 10;     
      if (num) print(num,target);
      target[0] = lastDigit;
      target++;
    }; // void print(ulong num,char*& target) 
 
  bool isRegisteredSymbolName(const char* str);

  static const char* createCanonicalVariableName(ulong varNum);

 private:
  StrArray _inputVariableName;
  ulong _nextInputVariableNumber; 
  
  StrArray _symbolName;
  DoubleArray _numericConstantValue;
  BoolArray _isNumericConstant;
  BoolArray _isInputSymbol;
  BoolArray _isPredicate;
  BoolArray _isCommutative;
  ULongArray _arity;
  BoolArray _isAnswerPredicate;


  bool _endOfInputFlag;
  ulong _nextSymbolNumber;
 
  long _numberOfSkolemSymbols;
 
  long _maxlNumOfSkolemSymbols;

  ulong _nextSkolemConstantSuffix;
  ulong _nextSkolemFunctionSuffix;
  ulong _nextSkolemPredicateSuffix;  

  const char* _canonicalVariableName[VampireKernelConst::MaxNumOfVariables];
  
  BoolArray _weightIsSet;
  WeightArray _weightConstantPart;
  LongArray _positiveHeaderPrecedence;  
  LongArray _negativeHeaderPrecedence;
  long _minimalHeaderPrecedence;
  long _maximalHeaderPrecedence;

  AutoFunctorPrecedence _autoSymbolPrecedence;
  long _symbolWeightByArity;

  static Signature* _current;
}; // class Signature 


inline ostream& operator<<(ostream& str,Signature& sig) 
{ 
  return sig.output(str); 
};


static Signature::InitStatic signatureInitStatic;

}; // namespace VK


//==================================================
#endif

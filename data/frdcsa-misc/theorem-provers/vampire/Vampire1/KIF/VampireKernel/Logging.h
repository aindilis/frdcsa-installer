//
// File:         Logging.h
// Description:  Various utilities for logging data.
// Created:      Dec 05, 2000, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Jul 27, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               Standard stream-like ("<<") notation is used
//               in the interface instead of "output". 
//               All the stuff related to counting
//               the size of the output is 
//               reimplemented with ostream::tellp().
// Note:         Output of long functors and variables must comply with the
//               syntax definition for backward matching benchmarking.
//               
//====================================================
#ifndef LOGGING_H
//====================================================
#define LOGGING_H
#include <iostream.h>
#include <fstream.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"     
#include "Term.h"
#include "Clause.h"
#include "Flatterm.h"   
#include "variables.h"     
#include "Signature.h"    
#include "LiteralList.h"
#include "Clause.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_LOGGING
#define DEBUG_NAMESPACE "Logging"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
  using namespace BK;
class Logging
{
 public:
  enum Mode 
  {
    ForwardMatching,
    BackwardMatching,
    BackwardSubsumption
  };

  class NormalisedTerm
    {
    public: 
      NormalisedTerm(const TERM* term) : _term(term) {};
      ~NormalisedTerm() {};
    private: 
      const TERM* _term;
      friend class Logging;
    };
  
  class StdTerm
    {
    public:
      StdTerm(const TERM* term,Signature* sig) : _term(term), _sig(sig) {}; 
      StdTerm(const TERM* term,Signature& sig) : _term(term), _sig(&sig) {};  
      ~StdTerm() {};
    private: 
      const TERM* _term;
      Signature* _sig; 
      friend class Logging;
    };

  class StdClause
    {
    public:
      StdClause(const Clause* cl) : _clause(cl) {};
      ~StdClause() {};
    private:
      const Clause* _clause;
      friend class Logging;
    };
  
  class Integer32
    {    
    public:
      Integer32(ulong val) : _negative(false), _val(val) {};
      Integer32(long val)  
	{
	  if (val < 0)
	    {
	      _negative = true;
	      _val = (ulong)(-val);
	    }
	  else
	    {
	      _negative = false;
	      _val = (ulong)val;
	    };
	};
      template <typename Type>
	Integer32(Type* val) 
	{
	  if (!_basePointerSet) { _basePointer = (long*)val; _basePointerSet = true; };
	  long dist = ((long*)val - _basePointer)*sizeof(long);
	  if (dist < 0)
	    {
	      _negative = true;
	      dist = -dist;
	    }
          else
	    {
	      _negative = false;
	    };
	  _val = ((ulong)dist)/sizeof(Type);
          if (((ulong)dist) % sizeof(Type)) _val++;	  
	};
      ~Integer32() {};
    private:
      static long* _basePointer;
      static bool _basePointerSet;
      bool _negative;
      ulong _val;
      friend class Logging;
    };

 public:
  Logging(const Mode& md) : _stream(), _varRenaming(), _mode(md) {};
  ~Logging() {};
  void initStatic() {};
  bool open(const char* fileName) 
    { 
      _stream.open(fileName,ios::app);
      _initStreamPos = _stream.tellp();
      _blocked = false;
      return (bool)_stream;
    };
  void block() { _blocked = true; };
  void unblock() { _blocked = false; };
  bool isBlocked() const { return _blocked; };
  void flush() { if (!_blocked) _stream.flush(); };
  long counter()
    {
      return (long)(_stream.tellp() - _initStreamPos);
    };

  Logging& operator<<(const TERM* t)
    { 
      if (!_blocked) output(t);
      return *this; 
    };
  
  Logging& operator<<(const NormalisedTerm& term)
    {
      if (!_blocked) outputNormalised(term._term); 
      return *this;
    };
  Logging& operator<<(const StdTerm& term);

  Logging& operator<<(const StdClause& cl);

  Logging& operator<<(const Flatterm* t) 
    { 
      if (!_blocked) output(t); 
      return *this;
    }; 
  
  Logging& operator<<(const Clause* cl) 
    { 
      if (!_blocked) 
	{
	  if (cl->flag(Clause::FlagLiteralsShared))
	    {
	      output(cl);
	    }
	  else
	    {
	      outputUnshared(cl);
	    };
	};
      return *this; 
    }; 
   
  Logging& operator<<(const char* str)
    {
      if (!_blocked) _stream << str;  
      return *this;
    };
  
  Logging& operator<<(const char& c)
    {
      if (!_blocked) _stream << c;
      return *this;
    };
 
  Logging& operator<<(int num)
    {
      if (!_blocked) _stream << num;
      return *this;
    };

  
  Logging& operator<<(long num)
    {
      if (!_blocked) _stream << num;
      return *this;   
    };

  Logging& operator<<(ulong num)
    {
      if (!_blocked) _stream << num;
      return *this;
    };

  Logging& operator<<(float num)
    {
      if (!_blocked) _stream << num;
      return *this;
    };

  Logging& operator<<(const Integer32& num)
    { 
      if (!_blocked) output(num);
      return *this;
    };

  Logging& outputSignatureUpdate(Signature& sig,ulong& firstSym)
    {
      CALL("outputSignatureUpdate(Signature& sig,ulong& firstSym)");
      ASSERT(_mode == BackwardSubsumption);
      if (firstSym >= sig.nextSymbolNumber()) return *this;
      if (!_blocked)
	{
	  if (firstSym < VampireKernelConst::FirstNoneqFun)
	    {
	      if (firstSym <= VampireKernelConst::UnordEqNum)
		outputSymbolDeclaration(sig,VampireKernelConst::UnordEqNum);
	      firstSym = VampireKernelConst::FirstNoneqFun;
	    };
	  for (; 
	       firstSym < sig.nextSymbolNumber(); 
	       firstSym++)
	    { 
	      outputSymbolDeclaration(sig,firstSym);
	    };
	  
        };
      return *this;
    }; //  Logging& outputSignatureUpdate(Signature& sig,ulong& firstSym)

  Logging& operator<<(Signature& sig)
    {
      CALL("operator<<(Signature& sig)");
      if (!_blocked)
	{ 
	  switch (_mode)
	    {
	    case ForwardMatching:
	      for (ulong sym = VampireKernelConst::FirstNoneqFun; 
		   sym < sig.nextSymbolNumber(); 
		   sym++)
		{ 
		  outputFun(sym);
                  _stream << "/" << sig.arity(sym) << "\n";
		}; 
              _stream << "$\n";
              break;

	    case BackwardMatching:
	      outputFun(VampireKernelConst::UnordEqNum); 
	      _stream << "/" 
		      << sig.arity(VampireKernelConst::UnordEqNum)
		      << "/" << sig.symbolName(VampireKernelConst::UnordEqNum) 
		      << "/" 
                      << "          // equality"
	              << "\n";

	      // equality is symmetric:
	      outputCommutativityDeclaration(VampireKernelConst::UnordEqNum);
	      _stream << "          // equality is symmetric"
		      << "\n";
 
	      for (ulong sym = VampireKernelConst::FirstNoneqFun; 
		   sym < sig.nextSymbolNumber(); 
		   sym++)
		{ 
		  outputFun(sym);
                  _stream << "/" << sig.arity(sym) << "/";
		  sig.outputSymbol(_stream,sym) << "/\n";
		  if (sig.isCommutative(sym))
		    {
		      outputCommutativityDeclaration(sym); 
		      _stream << "          // ";
		      outputFun(sym);
                      _stream << " is commutative"
		              << "\n"; 
		    };
		};
              _stream << "$\n";
	      break;

	    case BackwardSubsumption:
	      ICP("ICP0");         
              break;
	    };
	};
      return *this;
    }; // Logging& operator<<(Signature& sig)

 private:
  Logging& outputFun(ulong fun);
  Logging& outputVar(ulong var);
  Logging& outputNormalisedVar(ulong var); 
  Logging& output(const TERM* t);
  Logging& outputUnshared(const TERM* t);
  Logging& outputNormalised(const TERM* t);
  Logging& output(const Flatterm* t); 
  Logging& output(const Clause* cl);
  Logging& outputUnshared(const Clause* cl);
  Logging& outputBase32(ulong num);
  void outputNonzeroBase32(ulong num);
  Logging& output(const Integer32& num);
  Logging& outputCommutativityDeclaration(ulong sym);
  Logging& outputSymbolDeclaration(Signature& sig,ulong sym);
 private:
  ofstream _stream;
  streampos _initStreamPos; 
  INT_VAR_RENAMING _varRenaming;
  bool _blocked;
  Mode _mode;
}; // class Logging

inline Logging& Logging::outputFun(ulong fun)
{
  CALL("Logging::outputFun(ulong fun)");
  ASSERT(!_blocked);
  switch (_mode)
    {
    case ForwardMatching:
      if (fun < 159) { _stream << (char)(97 + fun); }
      else 
	_stream << '&' << fun << '&';
      return *this;

    case BackwardMatching: 
      if (fun < 159) { _stream << (char)(97 + fun); }
      else 
	_stream << '&' << fun << '&';
      return *this;

    case BackwardSubsumption:
      if (fun == VampireKernelConst::OrdEqNum) fun = VampireKernelConst::UnordEqNum;
      if (fun <= 25) { _stream << (char)('a' + fun); }
      else
        {
          _stream << '&';
          outputBase32(fun);
          _stream << '&';
        };
      return *this;
    };

#ifdef DEBUG_NAMESPACE
  ICP("ICP0");
  return *this;
#else
#ifdef _SUPPRESS_WARNINGS_
  RuntimeError::report("Illegal control point reached in Logging::outputFun(ulong fun)");
  return *this;
#endif
#endif
}; // Logging& Logging::outputFun(ulong fun)

inline Logging& Logging::outputVar(ulong var)
{
  CALL("Logging::outputVar(ulong var)");
  ASSERT(!_blocked);
  switch (_mode)
    {
    case ForwardMatching:
      if (var < 10) { _stream << var; }
      else
        _stream << "%" << var << "%";
      return *this;

    case BackwardMatching: 
      if (var < 10) { _stream << var; }
      else
        _stream << "%" << var << "%";
      return *this;

    case BackwardSubsumption:
      if (var <= 25) { _stream << (char)('A' + var); }
      else
        {
	  _stream << '%';
	  outputBase32(var);
	  _stream << '%';
	};
      return *this;
    };

#ifdef DEBUG_NAMESPACE
  ICP("ICP0");
  return *this;
#else
#ifdef _SUPPRESS_WARNINGS_
  RuntimeError::report("Illegal control point reached in Logging::outputVar(ulong var)");
  return *this;
#endif
#endif
}; // Logging& Logging::outputVar(ulong var)

inline Logging& Logging::outputNormalisedVar(ulong var)
{
  return outputVar(_varRenaming.Rename(var));
}; // ostream&  Logging::outputNormalisedVar(ostream& str,ulong var)


inline Logging& Logging::output(const TERM* t)
{
  CALL("output(const TERM* t)");
  ASSERT(!_blocked);
  ASSERT(t->isComplex());
  TERM::Iterator iter(t);
  switch (_mode)
    {
    case ForwardMatching:
      // no break here
    case BackwardMatching:
      do
	{
	  if (iter.symbolRef().isVariable())     
	    {
	      outputVar(iter.symbolRef().var());
	    }
	  else
	    outputFun(iter.symbolRef().functor());
	}
      while (iter.Next());
      break;
    case BackwardSubsumption:
      outputFun(t->functor());
      if (t->arity())
	{
	  TERM::Arguments args(t);
	  for (ulong i = 0; i < t->arity(); i++)
	    {
	      const TERM* arg = args.nth(i);
	      if (arg->isVariable())
		{
		  outputVar(arg->var());
		}
	      else
		{
		  ASSERT(arg->IsReference());
                  if (arg->First()->arity())
		    {
		      _stream << '[';
		      output(Integer32(arg->First()));
		      _stream << ']';
		    }
		  else
		    {
		      outputFun(arg->First()->functor());
		    };
		};
	    };
	};
      break;
    };
  return *this; 
}; //  ostream& Logging::output(const TERM* t)


inline Logging& Logging::outputUnshared(const TERM* t)
{
  CALL("outputUnshared(const TERM* t)");
  ASSERT(!_blocked);
  ASSERT(t->isComplex());
  ASSERT(_mode == BackwardSubsumption);
  TERM::Iterator iter(t);
  do
    {
      if (iter.symbolRef().isVariable())     
	{
	  outputVar(iter.symbolRef().var());
	}
      else
	outputFun(iter.symbolRef().functor());
    }
  while (iter.Next());
  return *this; 
}; // Logging& Logging::outputUnshared(const TERM* t)


inline Logging& Logging::outputNormalised(const TERM* t)
{
  CALL("outputNormalised(const TERM* t)");
  ASSERT(!_blocked);
  _varRenaming.Reset();
  TERM::Iterator iter(t);
  do
    {
      if (iter.symbolRef().isVariable())     
	{
	  outputNormalisedVar(iter.symbolRef().var());
	}
      else
	outputFun(iter.symbolRef().functor());
    }
  while (iter.Next()); 
  return *this; 
}; //  ostream& Logging::outputNormalised(ostream& str,const TERM* t)

inline Logging& Logging::output(const Flatterm* t)
{
  CALL("output(const Flatterm* t)");
  ASSERT(!_blocked);
  _varRenaming.Reset();
  Flatterm::Iterator iter(t);
  do
    {
      if (iter.CurrPos()->isVariable())     
	{
	  outputVar(iter.CurrPos()->Var());
	}
      else
	outputFun(iter.CurrPos()->functor());
      iter.NextSym();
    }
  while (iter.NotEndOfTerm()); 
  return *this; 
}; 

inline Logging& Logging::output(const Clause* cl)
{
  CALL("output(const Clause* cl)");
  ASSERT(!_blocked);
  ASSERT(_mode == BackwardSubsumption);
  _stream << '{';
  for (LiteralList::Iterator iter(cl->LitList()); iter; iter.next())
    {
      const TERM* lit = iter.currentLiteral();
      if (lit->arity())
	{
	  _stream << '[';
	  output(Integer32(lit));
	  _stream << ']';
	}
      else
	{
	  _stream << (lit->Positive() ? "++" : "--");
          output(lit);
	};
    };
  _stream << '}';
  return *this; 
}; // Logging& Logging::output(const Clause* cl)


inline Logging& Logging::outputUnshared(const Clause* cl)
{
  CALL("outputUnshared(const Clause* cl)");
  ASSERT(!_blocked);
  ASSERT(_mode == BackwardSubsumption);
  _stream << '{';
  for (LiteralList::Iterator iter(cl->LitList()); iter; iter.next())
    {
      _stream << (iter.currentLiteral()->Positive() ? "++" : "--");
      outputUnshared(iter.currentLiteral());
    };
  _stream << '}';
  return *this; 
}; // Logging& Logging::outputUnshared(const Clause* cl)


inline Logging& Logging::outputCommutativityDeclaration(ulong sym)
{
  CALL("outputCommutativityDeclaration(ulong sym)");
  ASSERT(!_blocked);
  switch (_mode)
    {
    case ForwardMatching: break;
    case BackwardMatching:
      _stream << "\\commutativity\\";
      outputFun(sym);
      break;
    case BackwardSubsumption: 
      _stream << ". 'commutativity' {";
     outputFun(sym);
     _stream << "}";
     break;
    };
  return *this;
}; 

inline Logging& Logging::operator<<(const Logging::StdTerm& term)
{ 
  if (!_blocked) term._term->output(_stream,term._sig);
  return *this;
}; // Logging& Logging::operator<<(const Logging::StdTerm& term)

inline Logging& Logging::operator<<(const Logging::StdClause& cl)
{ 
  if (!_blocked) _stream << cl._clause; 
  return *this;
}; // Logging& Logging::operator<<(const Logging::StdClause& cl)



inline Logging& Logging::outputBase32(ulong num)
{ 
  CALL("outputBase32(ulong num)");
  ASSERT(!_blocked);
  if (!num) { _stream << '0'; }
  else
    outputNonzeroBase32(num);
  return *this;
}; // Logging& Logging::outputBase32(ulong num)

inline void Logging::outputNonzeroBase32(ulong num)
{
  CALL("outputNonzeroBase32(ulong num)");
  ASSERT(num);
  ASSERT(!_blocked);
  ulong lastDigitValue = num % 32;
  ulong rest = num / 32;
  if (rest) outputNonzeroBase32(rest);
  if (lastDigitValue <= 9) 
    {
      _stream << (char)('0' + lastDigitValue);
    }
  else
    {
      _stream << (char)(('A' - 10) + lastDigitValue);
    }; 
}; // void Logging::outputNonzeroBase32(ulong num)


inline Logging& Logging::output(const Integer32& num)
{
  CALL("output(const Integer32& num)");
  ASSERT(!_blocked);
  ASSERT(_mode == BackwardSubsumption);
  if (num._negative) _stream << '-';
  return outputBase32(num._val);
}; // Logging::output(const Integer32& num)


inline Logging& Logging::outputSymbolDeclaration(Signature& sig,ulong sym)
{
  CALL("outputSymbolDeclaration(ulong sym)");
  ASSERT(!_blocked);
  ASSERT(_mode == BackwardSubsumption);
  if (sym == VampireKernelConst::OrdEqNum) return *this;
  if (sym == VampireKernelConst::UnordEqNum)
    {
      _stream << "^ ";
      outputFun(VampireKernelConst::UnordEqNum); 
      _stream << " : 'predicate' "
	      << sig.arity(VampireKernelConst::UnordEqNum)
	      << " " << "'" << sig.symbolName(VampireKernelConst::UnordEqNum) << "'" 
	      << "    // equality"
	      << "\n";
      
      // equality is symmetric:
      outputCommutativityDeclaration(VampireKernelConst::UnordEqNum);
      _stream << "          // equality is symmetric"
	      << "\n";
      return *this;
    };
  
  // Nonequality symbol
  _stream << "^ ";
  outputFun(sym);
  _stream << " : " 
	  << (sig.isPredicate(sym) ? "'predicate' " : "'function' ")
	  << sig.arity(sym) << " "  << "'";
  sig.outputSymbol(_stream,sym) << "'" << "\n";
  if (sig.isCommutative(sym))
    {
      outputCommutativityDeclaration(sym); 
      _stream << "          // ";
      outputFun(sym);
      _stream << " is commutative"
	      << "\n"; 
    }; 
  
  return *this;
}; // Logging& Logging::outputSymbolDeclaration(Signature& sig,ulong sym)


}; // namespace VK


//====================================================
#undef DEBUG_NAMESPACE  
//====================================================
#endif

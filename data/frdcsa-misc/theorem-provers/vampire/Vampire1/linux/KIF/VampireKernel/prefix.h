#ifndef PREFIX_H
//==================================================================
#define PREFIX_H
#ifndef NO_DEBUG_VIS 
 #include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "Term.h"
#include "variables.h"
#include "Stack.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PREFIX
 #define DEBUG_NAMESPACE "PREF_SYM"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{

class PREF_SYM 
{
 private:
  TERM sym;
 public:
  PREF_SYM() {};
  PREF_SYM(const TERM& s) 
    {
      if (s.isVariable())
	{
	  MkVar(Unifier::current()->variableBase(s.var()));
	}
      else // functor
	{
	  sym = s;
	};
    };
  ~PREF_SYM() {};
  bool IsFunctor() const { return (bool)(sym.Flag()); };
  bool isVariable() const { return !(sym.Flag()); };
  const TERM& Func() const { return sym; };
  TERM& Func() { return sym; };
  Unifier::Variable* var() const { return (Unifier::Variable*)(sym.First()); };
  void MkFunc(const TERM& f) { sym = f; };
  void Inversepolarity() { sym.Inversepolarity(); };
  void MkVar(const Unifier::Variable* v) { sym.SetFirst((TERM*)v); };
  int operator==(const PREF_SYM& s) const 
  { return sym.Content() == s.Func().Content(); };
  int operator!=(const PREF_SYM& s) const 
  { return sym.Content() != s.Func().Content(); };
 public: // methods for debugging
#ifndef NO_DEBUG_VIS 
  ostream& output(ostream& str) const { return output(str,0); };
  ostream& output(ostream& str,unsigned var_offset) const
    {
      if (IsFunctor()) return str << Func();
      return (var() + var_offset)->output(str);
    };
#endif
}; // class PREF_SYM

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PREFIX
#define DEBUG_NAMESPACE "PREFIX_SYM"
#endif
#include "debugMacros.h"
//================================================= 
namespace VK
{
class PREFIX_SYM
{
 private: // structure
  PREF_SYM head;
  PREFIX_SYM* bracket;
 public: 
  PREFIX_SYM() {};
  ~PREFIX_SYM() {};
  PREF_SYM& Head() { return head; };
  const PREF_SYM& Head() const { return head; };
  void SetBracket(PREFIX_SYM* br) { bracket = br; };
  void ResetBracket() { bracket = 0; };
  PREFIX_SYM* Bracket() const { return bracket; };
  PREFIX_SYM* Arg1() { return this + 1; };
  PREFIX_SYM* Arg2() { return Arg1()->Bracket(); };
 public: // methods for debugging
#ifndef NO_DEBUG_VIS 
  ostream& output(ostream& str) const
    {
      return Head().output(str) << " [" << (ulong)Bracket() << ']'; 
    };
#endif
}; // class PREFIX_SYM
}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PREFIX
#define DEBUG_NAMESPACE "PREFIX"
#endif
#include "debugMacros.h"
//================================================= 

namespace VK
{
class PREFIX
{
 public:
  static long Weight(const PREFIX_SYM* word) { return (word->Bracket() - word); };
  static ulong WeightWithSubst(const PREFIX_SYM* word) 
    {
      CALL("WeightWithSubst(const PREFIX_SYM* word)");
      // word must represent a complex term here
      ASSERT(word->Head().IsFunctor());
      ulong res = 0; 
      ITER iter; 
      iter.Reset((PREFIX_SYM*)word);
      do
	{ 
	  res++;
	  iter.SkipSym();
	}
      while (iter.NotEmpty());
      return res;
    };  

  static bool GroundnessAndWeight(const PREFIX_SYM* word,ulong& weight)
    {
      CALL("GroundnessAndWeight(const PREFIX_SYM* word,ulong& weight)");
      // word must represent a complex term here
      ASSERT(word->Head().IsFunctor());
      weight = 0;
      ITER iter; 
      iter.Reset((PREFIX_SYM*)word);
      do
	{ 
	  if (iter.CurrVar()) return false;      
	  weight++;
	  iter.SkipSym();
	}
      while (iter.NotEmpty());
      return true;
    }; // bool GroundnessAndWeight(const PREFIX_SYM* word,ulong& weight)


 public:
  class SIMPLE_ITER
    {
    private:
      PREFIX_SYM* curr_pos;
      PREFIX_SYM* end_of_word;
    public:
      SIMPLE_ITER() {};
      SIMPLE_ITER(PREFIX_SYM*word) // word must represent a complex term here
	{
	  CALL("SIMPLE_ITER::constructor SIMPLE_ITER(PREFIX_SYM* word)");
	  Reset(word);
	};
      void Reset(PREFIX_SYM* word) // word must represent a complex term here
	{
	  CALL("SIMPLE_ITER::Reset(PREFIX_SYM* word)");
	  ASSERT(PREFIX::CorrectComplexTerm(word)); 
	  curr_pos = word;
	  end_of_word = word->Bracket();
	};
      void MemorizeState(SIMPLE_ITER& memo) const
	{
	  memo.curr_pos = curr_pos;
	  memo.end_of_word = end_of_word;
	};
      void RestoreState(const SIMPLE_ITER& memo)
	{
	  curr_pos = memo.curr_pos;
	  end_of_word = memo.end_of_word;
	}; 
      PREFIX_SYM* CurrPos() const { return curr_pos; };
      bool Empty() const { return (curr_pos == end_of_word); };
      bool NotEmpty() const { return (bool)(end_of_word-curr_pos); };
      void SkipSym()
	{
	  CALL("SIMPLE_ITER::SkipSym()");
	  curr_pos++; 
	  ASSERT(curr_pos <= end_of_word);
	};

      void SkipTerm()
	{
	  CALL("SIMPLE_ITER::SkipTerm()");
	  ASSERT((curr_pos < curr_pos->Bracket()) 
		 && (curr_pos->Bracket() - curr_pos <= (long)VampireKernelConst::MaxTermSize));
	  curr_pos = curr_pos->Bracket();
	  ASSERT(curr_pos);
	};
    }; // class PREFIX::SIMPLE_ITER
 public:
  class ITER : private SIMPLE_ITER
    {
    private:
      SIMPLE_ITER* next_continuation;
      SIMPLE_ITER continuation[VampireKernelConst::MaxTermDepth];
      Unifier::Variable* curr_var;
    private:
      void PopContinuation()
	{
	  next_continuation--;
	  SIMPLE_ITER::RestoreState(*next_continuation);
	};
      void PushContinuation()
	{
	  SIMPLE_ITER::MemorizeState(*next_continuation); 
	  next_continuation++;
	};
    public: 
      ITER() {};
      ITER(PREFIX_SYM* word) { Reset(word); };
      void Reset(PREFIX_SYM* word) // word must represent a complex term here
	{
	  CALL("ITER::Reset(PREFIX_SYM* word)");
	  SIMPLE_ITER::Reset(word);
	  ASSERT(word);
	  ASSERT(word->Head().IsFunctor());
	  ASSERT(word->Bracket());
	  //continuation_num = 0;
	  next_continuation = continuation;
	  curr_var = (Unifier::Variable*)0;
	};
      bool Empty() const
	{
	  CALL("ITER::Empty() const"); 
#ifdef DEBUG_NAMESPACE
	  if (SIMPLE_ITER::Empty()) 
	    {
	      ASSERT(next_continuation == continuation);
	      return true;
	    }
	  else { return false; };
#else
	  return SIMPLE_ITER::Empty(); 
#endif
	};
      bool NotEmpty() const { return SIMPLE_ITER::NotEmpty(); };
      Unifier::Variable* CurrVar() const { return curr_var; };
      PREFIX_SYM* CurrSubterm() const { return SIMPLE_ITER::CurrPos(); };
      void SkipSym() // can't be applied if the iterator is empty
	{
	  CALL("ITER::SkipSym()");
	  // SIMPLE_ITER can't be empty here
	  ASSERT(!(SIMPLE_ITER::Empty()));

	  SIMPLE_ITER::SkipSym();
	  if (SIMPLE_ITER::Empty()) 
	    {
	      if (next_continuation - continuation)
		{
		  PopContinuation();
		}
	      else
		{
		  return; // empty iterator now
		};
	    };
	  if (SIMPLE_ITER::CurrPos()->Head().isVariable())
	    {
	      curr_var = SIMPLE_ITER::CurrPos()->Head().var();
	      void* bind = curr_var->Binding();
	      if (bind) // bound variable
		{
		check_bind_type: 
		  if (Unifier::current()->isVariable(bind))
		    {
		      curr_var = (Unifier::Variable*)bind;
		      bind = curr_var->Binding();
		      if (bind) { goto check_bind_type; };
		    }
		  else // variable bound by a complex term
		    {
		      curr_var = (Unifier::Variable*)0;
		      SIMPLE_ITER::SkipSym();
		      if (SIMPLE_ITER::NotEmpty())
			{
			  PushContinuation();
			};
		      SIMPLE_ITER::Reset((PREFIX_SYM*)bind);
		    };
		};
	    }
	  else
	    {
	      curr_var = (Unifier::Variable*)0;
	    };
	}; // void SkipSym()  

      void SkipTerm() // can't be applied if the iterator is empty
	{
	  CALL("ITER::SkipTerm()");
	  // SIMPLE_ITER can't be empty here
	  ASSERT(!(SIMPLE_ITER::Empty()));
        
	  SIMPLE_ITER::SkipTerm(); // the only difference with void SkipSym() 
	  if (SIMPLE_ITER::Empty()) 
	    {
	      if (next_continuation - continuation)
		{
		  PopContinuation();
		}
	      else
		{
		  return; // empty iterator now
		};
	    };
	  if (SIMPLE_ITER::CurrPos()->Head().isVariable())
	    {
	      curr_var = SIMPLE_ITER::CurrPos()->Head().var();
	      void* bind = curr_var->Binding();
	      if (bind) // bound variable
		{
		check_bind_type: 
		  if (Unifier::current()->isVariable(bind))
		    {
		      curr_var = (Unifier::Variable*)bind;
		      bind = curr_var->Binding();
		      if (bind) { goto check_bind_type; };
		    }
		  else // variable bound by a complex term
		    {
		      curr_var = (Unifier::Variable*)0;
		      SIMPLE_ITER::SkipSym();
		      if (SIMPLE_ITER::NotEmpty())
			{
			  PushContinuation();
			};
		      SIMPLE_ITER::Reset((PREFIX_SYM*)bind);
		    };
		};
	    }
	  else
	    {
	      curr_var = (Unifier::Variable*)0;
	    };
	}; // void SkipTerm()
    }; // class PREFIX::ITER 
 public:
  class ITER_WITH_BACKTRACK : private SIMPLE_ITER
    {
    private:
      enum ACTION_TO_UNDO
      {
        NOTHING = 0,
        POP = 1, // = NOTHING + POP
        PUSH = 2, // = NOTHING + PUSH
        POP_AND_PUSH = 3 // = POP + PUSH
      };
    private:
      SIMPLE_ITER* next_continuation;
      SIMPLE_ITER continuation[VampireKernelConst::MaxTermDepth];
      Unifier::Variable* current_var[VampireKernelConst::MaxTermSize];
      Unifier::Variable** next_curr_var;
      Unifier::Variable* curr_var;
    private:
#ifdef DEBUG_NAMESPACE
      PREFIX_SYM* top_level;
#endif
       
    private:
      ACTION_TO_UNDO undo[VampireKernelConst::MaxTermSize];
      unsigned undo_num;
      SIMPLE_ITER state[VampireKernelConst::MaxTermSize];
      SIMPLE_ITER used_continuation[VampireKernelConst::MaxTermSize];
    private:
      void PushCurrVar() 
	{
	  CALL("ITER_WITH_BACKTRACK::PushCurrVar()");
	  *next_curr_var = curr_var;
	  next_curr_var++;
	  ASSERT(next_curr_var - current_var <= (long)VampireKernelConst::MaxTermSize);
	};   
      void PopCurrVar()
	{
	  CALL("ITER_WITH_BACKTRACK::PopCurrVar()");
	  next_curr_var--;
        
	  ASSERT(next_curr_var >= current_var);
        
	  curr_var = *next_curr_var;
	};   
      void PopContinuation()
	{
	  next_continuation--;
	  SIMPLE_ITER::RestoreState(*next_continuation);
	};
      void PushContinuation()
	{
	  SIMPLE_ITER::MemorizeState(*next_continuation); 
	  next_continuation++;
	};
    public:
#ifdef DEBUG_NAMESPACE
      PREFIX_SYM* TopLevel() { return top_level; };
#endif  
    public: 
      ITER_WITH_BACKTRACK() {};
      void Reset(PREFIX_SYM* word) // word must represent a complex term here
	{
	  CALL("ITER_WITH_BACKTRACK::Reset(PREFIX_SYM* word)");
	  ASSERT(word);
	  ASSERT(word->Head().IsFunctor());
	  ASSERT(word->Bracket());
        
	  SIMPLE_ITER::Reset(word);
        
#ifdef DEBUG_NAMESPACE
	  top_level = word;
#endif
        
	  next_continuation = continuation;
	  curr_var = (Unifier::Variable*)0;
	  next_curr_var = current_var;
	  undo_num = 0;
	};
      bool Empty() const { return SIMPLE_ITER::Empty(); };
      bool NotEmpty() const { return SIMPLE_ITER::NotEmpty(); };
      Unifier::Variable* CurrVar() const { return curr_var; };
      PREFIX_SYM* CurrSubterm() const { return SIMPLE_ITER::CurrPos(); };
      void SkipSym() // can't be applied if the iterator is empty
	{
	  CALL("ITER_WITH_BACKTRACK::SkipSym()");
	  // SIMPLE_ITER can't be empty here
	  ASSERT(!(SIMPLE_ITER::Empty()));
        
	  SIMPLE_ITER::MemorizeState(state[undo_num]);
	  PushCurrVar();
        
	  unsigned undo_action = NOTHING;
	  SIMPLE_ITER::SkipSym();
	  if (SIMPLE_ITER::Empty()) 
	    {
	      if (next_continuation - continuation)
		{
		  undo_action += POP;
		  PopContinuation();
		  SIMPLE_ITER::MemorizeState(used_continuation[undo_num]);
		}
	      else
		{
		  undo[undo_num] = (ACTION_TO_UNDO)undo_action;
		  undo_num++;
		  return; // empty iterator now
		};
	    };
	  if (SIMPLE_ITER::CurrPos()->Head().isVariable())
	    {
	      curr_var = SIMPLE_ITER::CurrPos()->Head().var();
	      void* bind = curr_var->Binding();
	      if (bind) // bound variable
		{
		check_bind_type: 
		  if (Unifier::current()->isVariable(bind))
		    {
		      curr_var = (Unifier::Variable*)bind;
		      bind = curr_var->Binding();
		      if (bind) { goto check_bind_type; };
		    }
		  else // variable bound by a complex term
		    {
		      curr_var = (Unifier::Variable*)0;
		      SIMPLE_ITER::SkipSym();
		      if (SIMPLE_ITER::NotEmpty())
			{
			  undo_action += PUSH;
			  PushContinuation();
			};
		      SIMPLE_ITER::Reset((PREFIX_SYM*)bind);
		    };
		};
	    }
	  else
	    {
	      curr_var = (Unifier::Variable*)0;
	    };
	  undo[undo_num] = (ACTION_TO_UNDO)undo_action;
	  undo_num++;
	}; // void SkipSym() 
       
      void SkipTerm() // can't be applied if the iterator is empty
	{
	  CALL("ITER_WITH_BACKTRACK::SkipTerm()");
	  // SIMPLE_ITER can't be empty here
	  ASSERT(!(SIMPLE_ITER::Empty()));
	  SIMPLE_ITER::MemorizeState(state[undo_num]);
	  PushCurrVar();
        
	  unsigned undo_action = NOTHING;
	  SIMPLE_ITER::SkipTerm(); // the only difference with void SkipSym() 
	  if (SIMPLE_ITER::Empty()) 
	    {
	      if (next_continuation - continuation)
		{
		  undo_action += POP;
		  PopContinuation();
		  SIMPLE_ITER::MemorizeState(used_continuation[undo_num]);
		}
	      else
		{
		  undo[undo_num] = (ACTION_TO_UNDO)undo_action;
		  undo_num++;
		  return; // empty iterator now
		};
	    };
	  if (SIMPLE_ITER::CurrPos()->Head().isVariable())
	    {
	      curr_var = SIMPLE_ITER::CurrPos()->Head().var();
	      void* bind = curr_var->Binding();
	      if (bind) // bound variable
		{
		check_bind_type: 
		  if (Unifier::current()->isVariable(bind))
		    {
		      curr_var = (Unifier::Variable*)bind;
		      bind = curr_var->Binding();
		      if (bind) { goto check_bind_type; };
		    }
		  else // variable bound by a complex term
		    {
		      curr_var = (Unifier::Variable*)0;
		      SIMPLE_ITER::SkipSym();
		      if (SIMPLE_ITER::NotEmpty())
			{
			  undo_action += PUSH;
			  PushContinuation();
			};
		      SIMPLE_ITER::Reset((PREFIX_SYM*)bind);
		    };
		};
	    }
	  else
	    {
	      curr_var = (Unifier::Variable*)0;
	    };
	  undo[undo_num] = (ACTION_TO_UNDO)undo_action;
	  undo_num++;
	}; // void SkipTerm() 
     
      void Backtrack()
	{
	  CALL("ITER_WITH_BACKTRACK::Backtrack()");
	  ASSERT(undo_num);
	  undo_num--;
	  SIMPLE_ITER::RestoreState(state[undo_num]);
	  PopCurrVar();
	  switch (undo[undo_num])
	    {
	    case NOTHING: break;
	    case POP:
	      next_continuation->RestoreState(used_continuation[undo_num]);
	      next_continuation++;
	      break;
	    case PUSH:
	      next_continuation--;
	      break;
	    case POP_AND_PUSH: 
	      (next_continuation-1)->RestoreState(used_continuation[undo_num]);
	      break;
#ifdef DEBUG_NAMESPACE
	    default: ICP("default"); 
#endif 
	    };
	}; // void Backtrack()
    }; // class PREFIX::ITER_WITH_BACKTRACK
 public: 
  static bool equal(PREFIX_SYM* word1,PREFIX_SYM* word2)
    {
      ITER iter1;
      ITER iter2;
      iter1.Reset(word1);
      iter2.Reset(word2);
      while (!(iter1.Empty()))
	{
	  if (iter1.CurrVar())
	    {
	      if (iter1.CurrVar() != iter2.CurrVar()) { return false; };
	    }
	  else
	    {
	      if ((iter2.CurrVar()) 
		  || (iter1.CurrSubterm()->Head() != iter2.CurrSubterm()->Head()))
		{ return false; };
	    };
       
	  iter1.SkipSym();
	  iter2.SkipSym();
	};
      return true;
    }; 
    
#ifndef NO_DEBUG
  static bool CorrectComplexTerm(const PREFIX_SYM* word)
    {
      CALL("CorrectComplexTerm(const PREFIX_SYM* word)");

      if (!word) 
	{
	  DMSG("PREFIX::CorrectComplexTerm(word)> !word\n");
	  return false; 
	};
      if (!word->Head().IsFunctor())
	{
	  DMSG("PREFIX::CorrectComplexTerm(word)> !word->Head().IsFunctor()\n");
	  return false;
	};
      if (!word->Bracket())
	{
	  DMSG("PREFIX::CorrectComplexTerm(word)> !word->Bracket()\n");
	  return false;
	};
      if (word->Bracket() <= word) 
	{
	  DMSG("PREFIX::CorrectComplexTerm(word)> word->Bracket() <= word\n");
	  return false;
	};

      
      PREFIX_SYM* end = word->Bracket();
      long holes = word->Head().Func().arity();
      word++;
      while (word < end)
	{
	  holes--;
	  if (holes < 0) 
	    {
	      DMSG("PREFIX::CorrectComplexTerm(word)> holes < 0\n"); 
	      return false; 
	    };
	  if (word->Head().IsFunctor())
	    {
	      holes += word->Head().Func().arity();
	    }
	  else
	    {
	      if (!Unifier::current()->isVariable(word->Head().var())) 
		{
		  DMSG("PREFIX::CorrectComplexTerm(word)> !Unifier::current()->isVariable(word->Head().var())\n"); 
		  return false;
		};
	    };
	  word++;
	};
      if (holes) 
	{
	  DMSG("PREFIX::CorrectComplexTerm(word)> holes\n"); 
	  return false;
	};
      return true;
    };
#endif

#ifndef NO_DEBUG_VIS 
  static ostream& outputWord(ostream& str,const PREFIX_SYM* word);
  static ostream& outputWordWithSubst(ostream& str,const PREFIX_SYM* word);
  static ostream& outputTerm(ostream& str,const PREFIX_SYM* word);
#endif
 public:
  class OPEN_WORD
    {
    private:
      PREFIX_SYM* begin;
      PREFIX_SYM* curr_pos;
    public:
      void Reset(PREFIX_SYM* memory) { begin = memory; curr_pos = memory; };
      PREFIX_SYM* Beginning() const { return begin; };
      PREFIX_SYM* CurrPos() const { return curr_pos; };
      void Write(const TERM f)
	{
	  curr_pos->Head().MkFunc(f);
	  if (f.arity())
	    {
	      curr_pos->ResetBracket();
	    }
	  else
	    {
	      curr_pos->SetBracket(curr_pos+1);
	    };
	  curr_pos++;
	};
      void Write(Unifier::Variable* const v)
	{
	  curr_pos->Head().MkVar(v);
	  curr_pos->SetBracket(curr_pos+1);
	  curr_pos++;
	};
      void Close() { begin->SetBracket(curr_pos); };
      void Backtrack(const unsigned n) 
	{
	  curr_pos -= n; 
	  for (PREFIX_SYM* p = begin; p<curr_pos; p++)
	    {
	      if (p->Bracket() > curr_pos) { p->ResetBracket(); };
	    };
	};
    }; // class PREFIX::OPEN_WORD
 public:
  static void CorrectBracket(PREFIX_SYM* word)
    {
      if (!(word->Bracket()))
	{
	  PREFIX_SYM* curr_pos = word+1; 
	  BracketCounter<VampireKernelConst::MaxTermDepth> brack_counter;
	  brack_counter.Open(word->Head().Func().arity());
	  do
	    {
	      if (curr_pos->Head().isVariable() || (!(curr_pos->Head().Func().arity())))
		{
		  brack_counter.Close();
		}
	      else { brack_counter.Open(curr_pos->Head().Func().arity()); };
	      curr_pos++;
	    }
	  while (brack_counter.IsOpen());
	  word->SetBracket(curr_pos);
	};
    }; // void CorrectBracket(PREFIX_SYM* word)
 public: 
  static void ResetCorruptedBrackets(PREFIX_SYM* word,
				     PREFIX_SYM* changed_suffix,
				     PREFIX_SYM* old_end_of_word,
				     PREFIX_SYM* new_end_of_word)
    {
      while (word < changed_suffix)
	{
	  if (word->Bracket() > changed_suffix)
	    {
	      if (word->Bracket() == old_end_of_word)
		{
		  word->SetBracket(new_end_of_word);
		}
	      else
		{
		  word->ResetBracket();
		};
	    };
	  word++;
	};
    }; // void ResetCorruptedBrackets(..........
}; // class PREFIX

}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PREFIX
#define DEBUG_NAMESPACE "(PREFIX)OCCUR_CHECK"
#endif
#include "debugMacros.h"
//=================================================  

namespace VK
{
class OCCUR_CHECK 
{
 public:
  static bool Occurs(Unifier::Variable* v,PREFIX_SYM* word)
    {
     CALL("Occurs(Unifier::Variable* v,PREFIX_SYM* word)");
     DOP(PREFIX_SYM* debug_top_level_word = word);
     static LARGE_VAR_COUNTER link_counter;
     link_counter.Reset();
      
#ifdef NO_DEBUG
     static Stack<void*,VampireKernelConst::MaxNumOfVariables * VampireKernelConst::MaxNumOfIndices > 
       _links;    

#else
     static Stack<void*,VampireKernelConst::MaxNumOfVariables * VampireKernelConst::MaxNumOfIndices > 
       _links("OCCUR_CHECK::Occurs(..):_links");    
      
#endif

     _links.reset();

		     //void* links[VampireKernelConst::MaxTmpTermSize];
		     //void** next_link = links;

     PREFIX::SIMPLE_ITER iter;
     void* link;
     
     check_top_level:
      iter.Reset(word);
      iter.SkipSym();
     
      while (!(iter.Empty()))
       {
        PREF_SYM sym  = iter.CurrPos()->Head();
        if (sym.isVariable())
         {
          link = sym.var()->Binding();
          if (link) // bound variable
           {
            if (!(link_counter.Register(sym.var())))
	      {
		_links.push(link);
		//*next_link = link;
		//next_link++;
	      };
           }
          else // free variable
           {
            if (sym.var() == v) 
             {
              ASSERT(OccursIneff(v,debug_top_level_word));
              return true; 
             };
           };
         };
        iter.SkipSym();
       }; 
     
      try_next_link:
       if (_links.empty())
       //if (next_link == links) 
        {
         ASSERT(!(OccursIneff(v,debug_top_level_word)));
         return false; 
        };
   
       link = _links.pop();
       //next_link--;
       //link = *next_link;
       check_link:
        if (Unifier::current()->isVariable(link))
         {
          if (((Unifier::Variable*)link)->Binding()) 
           {
            if (!(link_counter.Register((Unifier::Variable*)link)))
             {
              link = ((Unifier::Variable*)link)->Binding();
              goto check_link;
             }
            else
             {
              goto try_next_link;
             };
           }
          else // free var
           {
            if (((Unifier::Variable*)link) == v) 
             {
              ASSERT(OccursIneff(v,debug_top_level_word));
              return true; 
             };
            goto try_next_link;
           };
         }
        else // complex term
         {
          word = (PREFIX_SYM*)link;
          goto check_top_level;
         };
    }; // bool Occurs(Unifier::Variable* v,PREFIX_SYM* word)
    
   #ifndef NO_DEBUG
    // Inefficient but simple version: 
    static bool OccursIneff(Unifier::Variable* v,PREFIX_SYM* word) 
     // word must represent a complex term, v must be a nonnull pointer
     {
      CALL("OccursIneff(Unifier::Variable* v,PREFIX_SYM* word)");
      ASSERT(v);
      ASSERT(Unifier::current()->isVariable(v));
      ASSERT(PREFIX::CorrectComplexTerm(word));
      
      PREFIX::ITER iter;
      iter.Reset(word);
      next_sym:
       iter.SkipSym();
       if (iter.Empty())
        {
         return false;
        }
       else
        {
         if (iter.CurrVar() == v)
          {
           return true;
          }
         else
          {
           goto next_sym;
          };
        };
     }; // bool OccursIneff(Unifier::Variable* v,PREFIX_SYM* word) 
   #endif
 }; // class OCCUR_CHECK

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PREFIX
#define DEBUG_NAMESPACE "ST_TERM_WITH_SUBST"
#endif
#include "debugMacros.h"
//=================================================  
namespace VK
{


  class ST_TERM_WITH_SUBST
    {
    private:
      unsigned var_offset;
      PREFIX::ITER word;
      TERM::Iterator top;
      bool top_level;
      bool empty;
      PREF_SYM curr_sym;
      TERM dummyTerm[2];
    public:
      ST_TERM_WITH_SUBST() 
	{
	  dummyTerm[0].MakeComplex((TERM::Arity)1,(TERM::Functor)VampireKernelConst::ReservedPred1,TERM::PositivePolarity);	  
	};
      ~ST_TERM_WITH_SUBST() {};
      void Reset(ulong index,TERM* t) 
	// t must be a complex term here 
	{
	  CALL("Reset(ulong index,TERM* t)");
	  ASSERT(t->isComplex());
	  top.Reset(t);
	  var_offset = Unifier::variableOffset(index);
	  top_level = true;
	  empty = false;
	  curr_sym.MkFunc(top.Symbol());
	};
      void Reset(ulong index,TERM& t)
	// t must be either variable or reference
	{
	  CALL("Reset(ulong index,TERM& t)");
	  ASSERT(t.isVariable() || t.IsReference());
	  dummyTerm[1] = t;
	  Reset(index,dummyTerm);
	  SkipSym();
	  ASSERT(!Empty());
	}; 
      bool Empty() const { return empty; };
      const PREF_SYM& CurrSym() const { return curr_sym; };
      TERM* CurrPosInBase() const { return top.CurrPos(); }; 
   
      void SkipTerm()
	{
	  CALL("SkipTerm()");
	  if (top_level)
	    {
	      if ((empty = !top.SkipTerm())) return;
	    top_check_sym:
	      if (top.CurrentSymIsVar())
		{
		  bool free_var;
		  void* bind = (Unifier::current()->variableBase(top.Symbol().var()) + var_offset)->Unref(free_var);
		  if (free_var)
		    {
		      curr_sym.MkVar((Unifier::Variable*)bind);
		    }
		  else
		    {
		      top_level = false;
		      word.Reset((PREFIX_SYM*)bind);
		      curr_sym = word.CurrSubterm()->Head();
		    };
		}
	      else // functor
		{
		  curr_sym.MkFunc(top.Symbol());
		};
	    }
	  else // in the substitution
	    {
	      word.SkipTerm();
	      if (word.Empty())
		{
		  top_level = true;
		  if (!top.Next()) return;
		  goto top_check_sym;
		}
	      else
		{
		  if (word.CurrVar())
		    {
		      curr_sym.MkVar(word.CurrVar());
		    }
		  else
		    {
		      curr_sym = word.CurrSubterm()->Head();
		    };
		};
	    };
	};
    
     
      void SkipSym()
	{
	  CALL("SkipSym()");
	  bool free_var;
	  void* bind;
	  if (top_level)
	    {
	    top_next_sym:
	      if (top.Next())
		{
		  if (top.CurrentSymIsVar())
		    {
		      bind = (Unifier::current()->variableBase(top.Symbol().var()) + var_offset)->Unref(free_var);
		      if (free_var)
			{
			  curr_sym.MkVar((Unifier::Variable*)bind);
			}
		      else
			{
			  top_level = false;
			  word.Reset((PREFIX_SYM*)bind);
			  curr_sym = word.CurrSubterm()->Head();
			};
		    }
		  else // functor
		    {
		      curr_sym.MkFunc(top.Symbol());
		    };
		}
	      else { empty = true; };
	    }
	  else
	    {
	      word.SkipSym();
	      if (word.Empty())
		{
		  top_level = true;
		  goto top_next_sym;
		}
	      else
		{
		  if (word.CurrVar())
		    {
		      curr_sym.MkVar(word.CurrVar());
		    }
		  else
		    {
		      curr_sym = word.CurrSubterm()->Head();
		    };
		};
	    };
	};
 
      static bool equal(ulong index1,TERM* t1,ulong index2,TERM* t2)
	{
	  CALL("equal(ulong index1,TERM* t1,ulong index2,TERM* t2)");
	  ASSERT(t1->isComplex());
	  ASSERT(t2->isComplex());
	  ST_TERM_WITH_SUBST iter1;
	  ST_TERM_WITH_SUBST iter2;
	  iter1.Reset(index1,t1);
	  iter2.Reset(index2,t2);
	  do
	    {
	      if (iter1.CurrPosInBase() == iter2.CurrPosInBase()) { iter1.SkipTerm(); iter2.SkipTerm(); }
	      else  
		if (iter1.CurrSym() == iter2.CurrSym()) { iter1.SkipSym(); iter2.SkipSym(); } 
		else return false;             
	    }
	  while (!iter1.Empty());
	  return true;
	};

    }; // class ST_TERM_WITH_SUBST


  void StandardTermToPrefix(ulong index,TERM* t,PREFIX_SYM*& memory);

  inline void StandardTermToPrefix(ulong index,TERM t,PREFIX_SYM*& memory) 
    {
      if (t.isVariable())
	{
	  memory->Head().MkVar(Unifier::current()->variable(t.var(),index));
	  memory->SetBracket(memory + 1);
	  memory++;
	}
      else StandardTermToPrefix(index,t.First(),memory); 
    }; // void StandardTermToPrefix(ulong index,TERM t,PREFIX_SYM*& memory)


}; // namespace VK
  //==================================================================
#endif 
 
 
 
 
 
 
 

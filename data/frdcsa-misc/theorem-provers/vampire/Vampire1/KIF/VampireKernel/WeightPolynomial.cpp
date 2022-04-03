//
// File:         WeightPolynomial.cpp
// Description:  Operations on weights of terms.
// Created:      Apr 11, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      May 14, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               Some weight comparison commands changed. 
//===================================================================
#include "WeightPolynomial.h"
#include "WeightPolynomialSpecialisedComparisonCommand.h"
#include "TermWeightType.h"
//===================================================================
using namespace VK;

#ifdef DEBUG_ALLOC_OBJ_TYPE
#include "ClassDesc.h"

ClassDesc 
WeightPolynomial::Stored::_classDesc("WeightPolynomial::Stored",
				     WeightPolynomial::Stored::minSize(),
				     WeightPolynomial::Stored::maxSize());


ClassDesc 
WeightPolynomial::Sharing::DiscTree::Node::_classDesc("WeightPolynomial::Sharing::DiscTree::Node",
						      WeightPolynomial::Sharing::DiscTree::Node::minSize(),
						      WeightPolynomial::Sharing::DiscTree::Node::maxSize());
						      
#endif

WeightPolynomial::Sharing* WeightPolynomial::_sharing; // implicitely initialised to 0

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_WEIGHT_POLYNOMIAL
 #define DEBUG_NAMESPACE "WeightPolynomial::SpecialisedComparison"
#endif
#include "debugMacros.h"
//===================================================================



bool WeightPolynomial::SpecialisedComparison::compileGreaterOrEqual(const FunctionComparison& cat,
								    const WeightPolynomial& posPart,
								    const WeightPolynomial& negPart,
								    Command*& code,ulong maxCodeSize)
{
  CALL("compileGreaterOrEqual(const FunctionComparison& cat,..)");
  ASSERT(maxCodeSize);
  Command* start = code;
  const ulong* v;
  DOP(v = (const ulong*)0);
  switch (cat)
    {
    case FailureToCompare:
      code->setTag(Command::RetFailureToCompare);
      ++code;
      return true;
    case AlwaysLess:
      code->setTag(Command::RetFailureToCompare);
      ++code;
      return true;
    case AlwaysEqual:
      code->setTag(Command::RetAlwaysEqual);
      ++code;
      return true;
    case AlwaysGreater:
      code->setTag(Command::RetAlwaysGreater);
      ++code;
      return true;
      
    case CanBeLessOrEqual:
      {
	ASSERT((!negPart.isConstant()) && (!negPart.freeMember()));
	ASSERT(posPart.isConstant() && posPart.freeMember());	
	v = negPart.monomials().begin();

	// compile all the monomials except the last one
	while (v != negPart.monomials().last())
	  {
	    if ((code - start) + 1 == (long)maxCodeSize)
	      {
		code->setTag(Command::RetFailureToCompare);
		++code;
		return false;
	      };
	    code->setTag(Command::IfXGr1RetFail_IfNotGrndRetFail);
	    code->setVar(*v);
	    ++v;
	    ++code;
	  };
	
	// compile the last monomial
	ASSERT(v == negPart.monomials().last());
	if ((code - start) + 1 == (long)maxCodeSize)
	  {
	    code->setTag(Command::RetFailureToCompare);
	    ++code;
	    return false;
	  };
	code->setTag(Command::IfXGr1RetFail_IfGrndRetAlwEq_RetFail);
	code->setVar(*v);
	++code;
	return true;
      }; // case CanBeLessOrEqual

    case CanBeGreaterOrEqual:
      {
	ASSERT((!posPart.isConstant()) && (!posPart.freeMember()));
	ASSERT(negPart.isConstant() && negPart.freeMember());
	v = posPart.monomials().begin();

	if (posPart.numberOfVariables() == 1UL)
	  {
	    // compile the only monomial
	    code->setTag(Command::IfXGr1RetAlwGr_IfGrndRetAlwEq_RetCanBeGrOrEq);
	    code->setVar(*v);
	    ++code;
	    return true;	    
	  }
	else // posPart.numberOfVariables() > 1UL
 	  {
	    if ((code - start) + 1 == (long)maxCodeSize)
	      {
		code->setTag(Command::RetCanBeGreaterOrEqual);
		++code;
		return false;
	      };
	    // compile the first monomial
	    code->setTag(Command::IfXGr1RetAlwGr_GrndToGRND);
	    code->setVar(*v);
	    ++code;
	    
	    // compile the monomials between the first one and the last one
	    ++v;
	    while (v != posPart.monomials().last())
	      {
		if ((code - start) + 1 == (long)maxCodeSize)
		  {
		    code->setTag(Command::RetCanBeGreaterOrEqual);
		    ++code;
		    return false;
		  };
		code->setTag(Command::IfXGr1RetAlwGr_UpdateGRND);
		code->setVar(*v);
		++v;
		++code;
	      };

	    // compile the last monomial
	    ASSERT(v == posPart.monomials().last());
	    if ((code - start) + 1 == (long)maxCodeSize)
	      {
		code->setTag(Command::RetCanBeGreaterOrEqual);
		++code;
		return false;
	      };
	    code->setTag(Command::IfXGr1RetAlwGr_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq);
	    code->setVar(*v);
	    ++code;
	    return true;
	  };
      }; // case CanBeGreaterOrEqual

    case VolatileButNearlyAlwaysLess:
      {
	ASSERT((!negPart.isConstant()) && (!negPart.freeMember()));
	ASSERT(posPart.isConstant() && posPart.freeMember());	
	v = negPart.monomials().begin();
	if (negPart.numberOfVariables() == 1UL)
	  {
	    // compile the only monomial
	    if (negPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::IfNotGrndRetFail_IfXGrWRetFail_IfXLeWRetAlwGr_RetAlwEq);
	      }
	    else
	      {
		code->setTag(Command::IfNotGrndRetFail_IfCXGrWRetFail_IfCXLeWRetAlwGr_RetAlwEq);
		code->setCoefficient(negPart.coefficient(*v));
	      };
	    code->setVar(*v);
	    code->setWeight(posPart.freeMember());
	    ++code;
	    return true;	
	  }
	else // negPart.numberOfVariables() > 1UL
	  {
	    if ((code - start) + 1 == (long)maxCodeSize)
	      {
		code->setTag(Command::RetFailureToCompare);
		++code;
		return false;
	      };
	    // compile the first monomial
	    if (negPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::IfNotGrndRetFail_WMinusXToACC_IfACCLe0RetFail);
	      }
	    else
	      {
		code->setTag(Command::IfNotGrndRetFail_WMinusCXToACC_IfACCLe0RetFail);
		code->setCoefficient(negPart.coefficient(*v));
	      };
	    code->setVar(*v);
	    code->setWeight(posPart.freeMember() - (negPart.minimalValue() - negPart.coefficient(*v)));
	    ++code;

	    // compile the monomials between the first one and the last one

	  vol_but_nearly_alw_less_all_monomials_except_first:
	    ++v;
	    while (v != negPart.monomials().last())
	      {
		if ((code - start) + 1 == (long)maxCodeSize)
		  {
		    code->setTag(Command::RetFailureToCompare);
		    ++code;
		    return false;
		  };
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::IfNotGrndRetFail_ACCMinusXPlus1ToACC_IfACCLe0RetFail);
		  }
		else
		  {
		    code->setTag(Command::IfNotGrndRetFail_ACCMinusCXPlusCToACC_IfACCLe0RetFail);
		    code->setCoefficient(negPart.coefficient(*v));
		  };
		code->setVar(*v);
		++v;
		++code;
	      };

	    // compile the last monomial

	    ASSERT(v == negPart.monomials().last());
	    if ((code - start) + 1 == (long)maxCodeSize)
	      {
		code->setTag(Command::RetFailureToCompare);
		++code;
		return false;
	      };
	    
	    if (negPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::IfNotGrndRetFail_ACCMinusXPlus1ToACC_IfACCLe0RetFail_IfACCEq0RetAlwEq_RetAlwGr);
	      }
	    else
	      {
		code->setTag(Command::IfNotGrndRetFail_ACCMinusCXPlusCToACC_IfACCLe0RetFail_IfACCEq0RetAlwEq_RetAlwGr);
		code->setCoefficient(negPart.coefficient(*v));
	      };
	    code->setVar(*v);
	    ++code;
	    return true;
	  };
      }; // case VolatileButNearlyAlwaysLess


    case VolatileButNearlyAlwaysGreater:
      {
	ASSERT((!posPart.isConstant()) && (!posPart.freeMember()));
	ASSERT(negPart.isConstant() && negPart.freeMember());
	v = posPart.monomials().begin();
	if (posPart.numberOfVariables() == 1UL)
	  {
	    // compile the only monomial
	    if (posPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::IfXGrWRetAlwGr_IfXLeWRetFail_IfGrndRetAlwEq_RetCanBeGrOrEq);
	      }
	    else
	      {
		code->setTag(Command::IfCXGrWRetAlwGr_IfCXLeWRetFail_IfGrndRetAlwEq_RetCanBeGrOrEq);
		code->setCoefficient(posPart.coefficient(*v));
	      };
	    code->setVar(*(posPart.monomials().begin()));
	    code->setWeight(negPart.freeMember());
	    ++code;
	    return true;
	  }
	else // posPart.numberOfVariables() > 1UL
 	  {
	    if ((code - start) + 1 == (long)maxCodeSize)
	      {
		code->setTag(Command::RetFailureToCompare);
		++code;
		return false;
	      };
	    // compile the first monomial
	    if (posPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::WMinusXToACC_IfACCLe0RetAlwGr_GrndToGRND);
	      }
	    else
	      {
		code->setTag(Command::WMinusCXToACC_IfACCLe0RetAlwGr_GrndToGRND);
		code->setCoefficient(posPart.coefficient(*v));
	      };
	    code->setVar(*v);
	    code->setWeight(negPart.freeMember() - (posPart.minimalValue() - posPart.coefficient(*v)));
	    ++code;
	    
	    // compile the monomials between the first one and the last one
	  vol_but_nearly_alw_greater_all_monomials_except_first:
	    ++v;
	    while (v != posPart.monomials().last())
	      {
		if ((code - start) + 1 == (long)maxCodeSize)
		  {
		    code->setTag(Command::RetFailureToCompare);
		    ++code;
		    return false;
		  };

		if (posPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::ACCMinusXPlus1ToACC_IfACCLe0RetAlwGr_UpdateGRND);
		  }
		else
		  {
		    code->setTag(Command::ACCMinusCXPlusCToACC_IfACCLe0RetAlwGr_UpdateGRND);
		    code->setCoefficient(posPart.coefficient(*v));
		  };
		
		code->setVar(*v);
		++v;
		++code;
	      };

	    // compile the last monomial
	    ASSERT(v == posPart.monomials().last());
	    if ((code - start) + 1 == (long)maxCodeSize)
	      {
		code->setTag(Command::RetFailureToCompare);
		++code;
		return false;
	      };

	    if (posPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::IfXGrACCPlus1RetAlwGr_IfXLeACCPlus1RetFail_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq);
	      }
	    else
	      {
		code->setTag(Command::IfCXGrACCPlusCRetAlwGr_IfCXLeACCPlusCRetFail_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq);
		code->setCoefficient(posPart.coefficient(*v));
	      };
	    code->setVar(*v);
	    ++code;
	    return true;
	  };

      }; // case VolatileButNearlyAlwaysGreater

    case AbsolutelyVolatile: 
      {	
	ASSERT((!posPart.isConstant()) && (!negPart.isConstant()));
	ASSERT((!posPart.freeMember()) || (!negPart.freeMember()));
	Command* commandWithLabel;
	DOP(commandWithLabel = (Command*)0);

	// decide which part to compile first
	if (posPart.numberOfVariables() <= negPart.numberOfVariables())
	  {
	    // compile posPart first

	    if ((posPart.numberOfVariables() + 1 > maxCodeSize) ||
		((negPart.numberOfVariables() > 1UL) &&
		 (posPart.numberOfVariables() + 2 > maxCodeSize)))
	      {
		// not enough space
		code->setTag(Command::RetFailureToCompare);
		++code;
		return false;
	      };
	    

	    TermWeightType constPart = posPart.freeMember() - negPart.minimalValue();
	    v = posPart.monomials().begin();
	    if (posPart.numberOfVariables() == 1UL)
	      {
		// compile the only monomial together with the constant
		if (posPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::XPlusWToPOSINST_IfConstPOSINSAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL);
		  }
		else
		  {
		    code->setTag(Command::CXPlusWToPOSINST_IfConstPOSINSAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL);
		    code->setCoefficient(posPart.coefficient(*v));
		  };
		code->setWeight(constPart);
		code->setVar(*v);
		// in case there is not enough space for the code
		// optimised for ground posPart instances
		code->setLabel(code + 1); 
		
		commandWithLabel = code; 
		++code;
	      }
	    else // posPart.numberOfVariables() > 1UL
	      {		
		// compile the first monomial together with the constant
		if (posPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::XPlusWToPOSINST);
		  }
		else
		  {
		    code->setTag(Command::CXPlusWToPOSINST);
		    code->setCoefficient(posPart.coefficient(*v));
		  };
		code->setVar(*v);
		code->setWeight(constPart);
		++code;

		// compile the monomials between the first one and the last one
		      
		++v;
		while (v != posPart.monomials().last())
		  { 
		    if (posPart.coefficient(*v) == (TermWeightType)1)
		      {
			code->setTag(Command::POSINSTPlusXToPOSINST);
		      }
		    else
		      {			
			code->setTag(Command::POSINSTPlusCXToPOSINST);
			code->setCoefficient(posPart.coefficient(*v));
		      };

		    code->setVar(*v);
		    ++v;
		    ++code;
		  };  

		// compile the last monomial
		ASSERT(v == posPart.monomials().last());
		if (posPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::POSINSTPlusXToPOSINST_IfConstPOSINSTAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL);
		  }
		else
		  {			
		    code->setTag(Command::POSINSTPlusCXToPOSINST_IfConstPOSINSTAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL);
		    code->setCoefficient(posPart.coefficient(*v));
		  };
		code->setVar(*v);
		
		// in case there is not enough space for the code
		// optimised for ground posPart instances
		code->setLabel(code + 1); 
		
		commandWithLabel = code; 
		++code;
	      }; // else // posPart.numberOfVariables() > 1UL

	    ASSERT(code - start == (long)posPart.numberOfVariables());
	    // compile negPart now

	    // Assume that the positive part instance is nonconstant here
	    v = negPart.monomials().begin();

	    if (negPart.numberOfVariables() == 1UL)
	      {
		// compile the only monomial 
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::XMinus1ToNEGINSTRetCompForGrOrEqPOSINSTvNEGINST);
		  }
		else
		  {
		    code->setTag(Command::CXMinusCToNEGINSTRetCompForGrOrEqPOSINSTvNEGINST);
		    code->setCoefficient(negPart.coefficient(*v));
		  }; 
		code->setVar(*v);
		++code;
	      }
	    else // negPart.numberOfVariables() > 1UL
	      {
		// compile the first monomial 
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::XMinus1ToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail);
		  }
		else
		  {
		    code->setTag(Command::CXMinusCToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail);
		    code->setCoefficient(negPart.coefficient(*v));
		  };
		code->setVar(*v);
		++code;

		// compile the monomials between the first one and the last one
		++v;
		while (v != negPart.monomials().last())
		  { 
		    if (negPart.coefficient(*v) == (TermWeightType)1)
		      {
			code->setTag(Command::NEGINSTPlusXMinus1ToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail);
		      }
		    else
		      {			
			code->setTag(Command::NEGINSTPlusCXMinusCToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail);
			code->setCoefficient(negPart.coefficient(*v));
		      };

		    code->setVar(*v);
		    ++v;
		    ++code;
		  }; 

		// compile the last monomial  
		ASSERT(v == negPart.monomials().last());  
		
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::NEGINSTPlusXMinus1ToNEGINST_RetCompForGrOrEqPOSINSTvNEGINST);
		  }
		else
		  {			
		    code->setTag(Command::NEGINSTPlusCXMinusCToNEGINST_RetCompForGrOrEqPOSINSTvNEGINST);
		    code->setCoefficient(negPart.coefficient(*v));
		  };
		code->setVar(*v);
		++code;
	      }; // else // negPart.numberOfVariables() > 1UL

	    ASSERT(code - start == (long)(posPart.numberOfVariables() + negPart.numberOfVariables()));
	    ASSERT((code - 1)->isTerminal());

	    // Now assume that the positive part instance is constant and positive

	    if (((code - start) == (long)maxCodeSize) || 
		((negPart.numberOfVariables() > 1UL) && 
		 ((code - start) + 1 == (long)maxCodeSize)))
	      {
		return false;
	      };	    
	    commandWithLabel->setLabel(code);
	    v = negPart.monomials().begin();
	    if (negPart.numberOfVariables() == 1UL)
	      {
		// compile the only monomial
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::IfNotGrndRetFail_IfXMinus1GrPOSINSTRetFail_IfXMinus1LePOSINSTRetAlwGr_RetAlwEq);		    
		  }
		else
		  {
		    code->setTag(Command::IfNotGrndRetFail_IfCXMinusCGrPOSINSTRetFail_IfCXMinusCLePOSINSTRetAlwGr_RetAlwEq);
		    code->setCoefficient(negPart.coefficient(*v));
		  };
		code->setVar(*v);
		++code;
		return true;
	      }
	    else // negPart.numberOfVariables() > 1UL
	      {
		// compile the first monimial
		
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::IfNotGrndRetFail_POSINSTPlus1MinusXToACC_IfACCLe0RetFail);		    
		  }
		else
		  {
		    code->setTag(Command::IfNotGrndRetFail_POSINSTPlusCMinusCXToACC_IfACCLe0RetFail);
		    code->setCoefficient(negPart.coefficient(*v));
		  };
		code->setVar(*v);
		++code;
		goto vol_but_nearly_alw_less_all_monomials_except_first;		
	      };
	    ICP("ICP10");
	  }
	else // compile negPart first
	  {
	    if ((negPart.numberOfVariables() + 1 > maxCodeSize) ||
		((posPart.numberOfVariables() > 1UL) &&
		 (negPart.numberOfVariables() + 2 > maxCodeSize)))
	      {
		// not enough space
		code->setTag(Command::RetFailureToCompare);
		++code;
		return false;
	      };

	    
	    TermWeightType constPart = negPart.freeMember() - posPart.minimalValue();
	    v = negPart.monomials().begin();
	    if (negPart.numberOfVariables() == 1UL)
	      {
		// compile the only monomial together with the constant
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::XPlusWToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL);
		    code->setWeight(constPart);		     
		  }
		else 
		  {		
		    code->setTag(Command::CXPlusWToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL);
		    code->setWeight(constPart);
		    code->setCoefficient(negPart.coefficient(*v));
		  };
		code->setVar(*v);
		  
		// in case there is not enough space for the code
		// optimised for ground posPart instances
		code->setLabel(code + 1); 
		
		commandWithLabel = code; 
		++code;
	      }
	    else
	      {
		// compile the first monomial together with the constant
		
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::XPlusWToNEGINST);
		    code->setWeight(constPart);
		  }
		else 
		  {		   
		    code->setTag(Command::CXPlusWToNEGINST);
		    code->setWeight(constPart);
		    code->setCoefficient(negPart.coefficient(*v));
		  };
		code->setVar(*v);
		++code;

		// compile the monomials between the first one and the last one
		++v;
		while (v != negPart.monomials().last())
		  { 
		    if (negPart.coefficient(*v) == (TermWeightType)1)
		      {
			code->setTag(Command::NEGINSTPlusXToNEGINST);
		      }
		    else
		      {			
			code->setTag(Command::NEGINSTPlusCXToNEGINST);
			code->setCoefficient(negPart.coefficient(*v));
		      };
		    code->setVar(*v);
		    ++v;
		    ++code;
		  }; 

		// compile the last monomial
		ASSERT(v == negPart.monomials().last());  
 
		if (negPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::NEGINSTPlusXToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL);
		  }
		else
		  {			
		    code->setTag(Command::NEGINSTPlusCXToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL);
		    code->setCoefficient(negPart.coefficient(*v));
		  };
		code->setVar(*v);
		  
		// in case there is not enough space for the code
		// optimised for ground posPart instances
		code->setLabel(code + 1); 
		
		commandWithLabel = code; 
		++code;
	      };


	    ASSERT(code - start == (long)negPart.numberOfVariables());
	    // compile posPart now

	    // Assume that the negative part instance is nonconstant here
	    ASSERT(posPart.numberOfVariables() > 1UL); // otherwise posPart would be compiled first

	    // compile the first monomial
	    v = posPart.monomials().begin();
	    if (posPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::XMinus1ToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr);
	      }
	    else
	      {
		code->setTag(Command::CXMinusCToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr);
		code->setCoefficient(posPart.coefficient(*v));
	      };
	    
	    code->setVar(*v);
	    ++code;

	    // compile the monomials between the first one and the last one
	    ++v;
	    while (v != posPart.monomials().last())
	      { 
		if (posPart.coefficient(*v) == (TermWeightType)1)
		  {
		    code->setTag(Command::POSINSTPlusXMinus1ToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr);
		  }
		else
		  {			
		    code->setTag(Command::POSINSTPlusCXMinusCToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr);
		    code->setCoefficient(posPart.coefficient(*v));
		  };
		code->setVar(*v);
		++v;
		++code;
	      }; 
		
	    // compile the last monomial		
	    ASSERT(v == posPart.monomials().last());  
	    if (posPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::POSINSTPlusXMinus1ToPOSINST_RetCompForGrOrEqPOSINSTvNEGINST);
	      }
	    else
	      {			
		code->setTag(Command::POSINSTPlusCXMinusCToPOSINST_RetCompForGrOrEqPOSINSTvNEGINST);
		code->setCoefficient(posPart.coefficient(*v));
	      };
	    code->setVar(*v);
	    ++code; 
	    

	    ASSERT(code - start == (long)(posPart.numberOfVariables() + negPart.numberOfVariables()));
	    ASSERT((code - 1)->isTerminal());

	    // Now assume that the negative part instance is constant
	    ASSERT(posPart.numberOfVariables() > 1UL); // otherwise posPart would be compiled first

	    if ((code - start) + 1 >= (long)maxCodeSize)
	      {
		return false;
	      };	
	    commandWithLabel->setLabel(code);
	    
	    // compile the first monomial
	    v = posPart.monomials().begin();
	    if (posPart.coefficient(*v) == (TermWeightType)1)
	      {
		code->setTag(Command::NEGINSTPlus1MinusXToACC_IfACCLe0RetAlwGr_GrndToGRND);
	      }
	    else
	      {			
		code->setTag(Command::NEGINSTPlusCMinusCXToACC_IfACCLe0RetAlwGr_GrndToGRND);
		code->setCoefficient(posPart.coefficient(*v));
	      };
	    code->setVar(*v);
	    ++code; 
	    goto vol_but_nearly_alw_greater_all_monomials_except_first;
	  };
	ICP("ICP20");
      }; // case AbsolutelyVolatile
    };

  ICP("END");

#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif

}; // bool WeightPolynomial::SpecialisedComparison::compileGreaterOrEqual(const FunctionComparison& cat,..)



//===================================================================

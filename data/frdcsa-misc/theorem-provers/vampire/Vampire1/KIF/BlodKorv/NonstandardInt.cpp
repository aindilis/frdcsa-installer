//
// File:         NonstandardInt.cpp
// Description:  Integers with special values. 
// Created:      Jun 21, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//============================================================================
#include "NonstandardInt.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_ARRAY
 #define DEBUG_NAMESPACE "NonstandardInt"
#endif
#include "debugMacros.h"
//============================================================================
using namespace BK;

long NonstandardInt::subtract(long x,long y)
{
  CALL("subtract(long x,long y)");
  if (y == 0L) return x;
  if (x == 0L)
    {
      if (y == _undefined) return _undefined;
      return -y;
    };
  
  if ((x == _undefined) || (y == _undefined)) return _undefined;
    
  if (x <= _maxOrdinaryValue)
    {
      if (x >= _minOrdinaryValue)
	{
	  if (y <= _maxOrdinaryValue)
	    {
	      if (y >= _minOrdinaryValue)
		{
		  // both are ordinary values
		  if (x > 0L)
		    {
		      if (y > 0L)
			{
			  return x - y;
			}
		      else // y < 0L
			{
			  return _bigPositive;
			};
		    }
		  else // x < 0L
		    {
		      if (y > 0L)
			{
			  return _smallNegative;
			}
		      else // y < 0L
			{
			  return x - y;
			};
		    };
		}
	      else // y < _minOrdinaryValue
		{
		  ASSERT(y == _smallNegative);
		  if (x > 0L) return _bigPositive;
		  return _undefined;
		};
	    }
	  else // y > _maxOrdinaryValue
	    {
	      ASSERT(y == _bigPositive);
	      if (x < 0L) return _smallNegative;
	      return _undefined;
	    };
	}
      else // x < _minOrdinaryValue
	{
	  ASSERT(x == _smallNegative);
	  if (y > 0L) return _smallNegative;
	  return _undefined;
	};

    }
  else // x > _maxOrdinaryValue
    {
      ASSERT(x == _bigPositive);
      if (y < 0L) return _bigPositive;
      return _undefined;
    };
}; // long NonstandardInt::subtract(long x,long y)



long NonstandardInt::add(long x,long y)
{
  CALL("add(long x,long y)");
  if (y == 0L) return x;
  if (x == 0L) return y;
  if ((x == _undefined) || (y == _undefined)) return _undefined;
    
  if (x <= _maxOrdinaryValue)
    {
      if (x >= _minOrdinaryValue)
	{
	  if (y <= _maxOrdinaryValue)
	    {
	      if (y >= _minOrdinaryValue)
		{
		  // both are ordinary values
		  if (x > 0L)
		    {
		      if (y > 0L)
			{
			  return _bigPositive;
			}
		      else // y < 0L
			{
			  return x + y;
			};
		    }
		  else // x < 0L
		    {
		      if (y > 0L)
			{
			  return x + y;
			}
		      else // y < 0L
			{
			  return _smallNegative;
			};
		    };
		}
	      else // y < _minOrdinaryValue
		{
		  ASSERT(y == _smallNegative);
		  if (x < 0L) return _smallNegative;
		  return _undefined;
		};
	    }
	  else // y > _maxOrdinaryValue
	    {
	      ASSERT(y == _bigPositive);
	      if (x > 0L) return _bigPositive;
	      return _undefined;
	    };
	}
      else // x < _minOrdinaryValue
	{
	  ASSERT(x == _smallNegative);
	  if (y < 0L) return _smallNegative;
	  return _undefined;
	};
    }
  else // x > _maxOrdinaryValue
    {
      ASSERT(x == _bigPositive);
      if (y > 0L) return _bigPositive;
      return _undefined;
    };
}; // long NonstandardInt::add(long x,long y)




long NonstandardInt::multiply(long x,long y)
{
  CALL("multiply(long x,long y)");
  ASSERT((x > _maxSafeFactor) ||
	 (x < _minSafeFactor) ||
	 (y > _maxSafeFactor) ||
	 (y < _minSafeFactor));

  if ((x == 0L) || (y == 0L)) return 0L;
  if (x == 1L) return y;
  if (y == 1L) return x;
  if ((x == _undefined) || (y == _undefined)) return _undefined;
  if (x == -1L) return -y;
  if (y == -1L) return -x;
  
  if ((x > 0L) == (y > 0L)) return _bigPositive;
  return _smallNegative;
}; // long NonstandardInt::multiply(long x,long y)






//============================================================================


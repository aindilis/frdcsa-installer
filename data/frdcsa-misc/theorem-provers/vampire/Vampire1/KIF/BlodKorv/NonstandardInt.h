//
// File:         NonstandardInt.h
// Description:  Integers with special values. 
// Created:      Jun 21, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//============================================================================
#ifndef NONSTANDARD_INT_H
#define NONSTANDARD_INT_H
//============================================================================
#include <iostream.h>
#include <limits.h>
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "RuntimeError.h"
#include "Comparison.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_ARRAY
 #define DEBUG_NAMESPACE "NonstandardInt"
#endif
#include "debugMacros.h"
//============================================================================

namespace BK
{

  class NonstandardInt
    {
    public:
      NonstandardInt() {};
      ~NonstandardInt() {};
      NonstandardInt(const NonstandardInt& n) { _content = n._content; };
      explicit NonstandardInt(const long& n) 
	{
	  if (n > _maxOrdinaryValue)
	    {
	      _content = _bigPositive;
	    }
	  else
	    if (n < _minOrdinaryValue)
	      {
		_content = _smallNegative;
	      }
	    else
	      _content = n;
	}; // NonstandardInt(const long& n) 
      NonstandardInt& operator=(const NonstandardInt& n) 
      {
	_content = n._content;
	return *this;
      };
  
      long toLong() const
	{
	  if (isSpecialValue()) 
	    RuntimeError::report("NonstandardInt: a special value converted to long.");
	  return _content;
	};
      
      bool isOrdinary() const 
	{ 
	  return (_content <= _maxOrdinaryValue) &&
	    (_content >= _minOrdinaryValue);
	}; 
      
      bool isSpecialValue() const { return !isOrdinary(); };

      bool isPositiveInfinity() const
	{
	  return _content == _bigPositive;
	};
  
      static NonstandardInt bigPositive() 
	{ 
	  return NonstandardInt(_bigPositive,true); 
	};

      bool isNegativeInfinity() const
	{
	  return _content == _smallNegative;
	};
      
      static NonstandardInt smallNegative() 
	{ 
	  return NonstandardInt(_smallNegative,true); 
	};

      bool isUndefined() const
	{
	  return _content == _undefined;
	};
      
      static NonstandardInt undefined() 
	{ 
	  return NonstandardInt(_undefined,true); 
	};

      NonstandardInt operator-() const
	{
	  if (isUndefined()) return *this;
	  return NonstandardInt(-_content,true);
	};
      
      NonstandardInt operator-(const NonstandardInt& n) const
	{
	  CALL("operator-(const NonstandardInt& n) const");
	  // most frequent situation, must be fast
	  if ((_content <= _maxSafeSummand) &&
	      (_content >= _minSafeSummand) &&
	      (n._content <= _maxSafeSummand) &&
	      (n._content >= _minSafeSummand))
	    return NonstandardInt(_content - n._content,true);
	  return NonstandardInt(subtract(_content,n._content),true);
	}; // NonstandardInt operator-(const NonstandardInt& n) const

      NonstandardInt& operator-=(const NonstandardInt& n)
      {
	CALL("operator-=(const NonstandardInt& n)");	
	if ((_content <= _maxSafeSummand) &&
	    (_content >= _minSafeSummand) &&
	    (n._content <= _maxSafeSummand) &&
	    (n._content >= _minSafeSummand))
	  {
	    _content -= n._content;
	  }
	else
	  {
	    _content = subtract(_content,n._content);
	  };
	return *this;
      }; // NonstandardInt& operator-=(const NonstandardInt& n) 

      
      NonstandardInt operator+(const NonstandardInt& n) const
	{
	  CALL("operator+(const NonstandardInt& n) const");
	  // most frequent situation, must be fast
	  if ((_content <= _maxSafeSummand) &&
	      (_content >= _minSafeSummand) &&
	      (n._content <= _maxSafeSummand) &&
	      (n._content >= _minSafeSummand))
	    return NonstandardInt(_content + n._content,true);
	  return NonstandardInt(add(_content,n._content),true);
	}; // NonstandardInt operator+(const NonstandardInt& n) const


      NonstandardInt& operator+=(const NonstandardInt& n)
      {
	CALL("operator+=(const NonstandardInt& n)");	
	if ((_content <= _maxSafeSummand) &&
	    (_content >= _minSafeSummand) &&
	    (n._content <= _maxSafeSummand) &&
	    (n._content >= _minSafeSummand))
	  {
	    _content += n._content;
	  }
	else
	  {
	    _content = add(_content,n._content);
	  };
	return *this;
      }; // NonstandardInt& operator-=(const NonstandardInt& n)

      NonstandardInt operator*(const NonstandardInt& n) const
	{
	  CALL("operator*(const NonstandardInt& n) const");
	  // most frequent situation, must be fast
	  if ((_content <= _maxSafeFactor) &&
	      (_content >= _minSafeFactor) &&
	      (n._content <= _maxSafeFactor) &&
	      (n._content >= _minSafeFactor))
	    return NonstandardInt(_content * n._content,true);
	  return NonstandardInt(multiply(_content,n._content),true);
	}; // NonstandardInt operator*(const NonstandardInt& n) const
      

      NonstandardInt& operator*=(const NonstandardInt& n)
      {
	CALL("operator*=(const NonstandardInt& n)");	
	if ((_content <= _maxSafeFactor) &&
	    (_content >= _minSafeFactor) &&
	    (n._content <= _maxSafeFactor) &&
	    (n._content >= _minSafeFactor))
	  {
	    _content *= n._content;
	  }
	else
	  {
	    _content = multiply(_content,n._content);
	  };
	return *this;
      }; // NonstandardInt& operator-=(const NonstandardInt& n)


      Comparison compare(const NonstandardInt& n) const
	{
	  CALL("compare(const NonstandardInt& n) const");
	  if (isOrdinary())
	    {
	      if (n.isOrdinary())
		{
		  if (_content == n._content) return Equal;
		  if (_content < n._content) return Less;
		  return Greater;
		}
	      else
		{
		  ASSERT(_content != n._content);
		  if (_content < n._content)
		    {
		      ASSERT(n._content == _bigPositive);
		      return Less;
		    }
		  else // _content > n._content
		    { 
		      if (n._content == _undefined) return Incomparable;
		      ASSERT(n._content == _smallNegative);
		      return Greater;
		    };
		};
	    }
	  else // !this->isOrdinary()
	    {
	      if ((_content == n._content) || 
		  (_content == _undefined) || 
		  (n._content == _undefined)) 
		return Incomparable;
	      if (_content < n._content)
		{
		  ASSERT(_content == _smallNegative);
		  return Less;
		}
	      else // _content > n._content
		{
		  ASSERT(_content == _bigPositive);
		  return Greater;
		};	      
	    };
	}; // Comparison compare(const NonstandardInt& n) const



      ostream& output(ostream& str) const
	{	  
	  if (isOrdinary()) return str << _content;
	  if (isPositiveInfinity()) return str << "+inf";      
	  if (isNegativeInfinity()) return str << "-inf";
	  return str << "nan";
	};

    private:
      NonstandardInt(const long& n,bool) 
	{
	  _content = n;
	};

      bool operator==(const NonstandardInt&)
      {
	CALL("operator==(const NonstandardInt&)");
	ICP("ICP0");
	return false;
      };
      bool operator!=(const NonstandardInt&)
      {
	CALL("operator!=(const NonstandardInt&)");
	ICP("ICP0");
	return false;
      };
      bool operator>(const NonstandardInt&)
      {
	CALL("operator>(const NonstandardInt&)");
	ICP("ICP0");
	return false;
      };
      bool operator>=(const NonstandardInt&)
      {
	CALL("operator>=(const NonstandardInt&)");
	ICP("ICP0");
	return false;
      };
      bool operator<(const NonstandardInt&)
      {
	CALL("operator<(const NonstandardInt&)");
	ICP("ICP0");
	return false;
      };
      bool operator<=(const NonstandardInt&)
      {
	CALL("operator<=(const NonstandardInt&)");
	ICP("ICP0");
	return false;
      };

      static long subtract(long x,long y);
      static long add(long x,long y);
      static long multiply(long x,long y);

    private:
      static const long _bigPositive = 2147483647L; 
      static const long _smallNegative = -2147483647L;
      static const long _undefined = -2147483646L; 
      // _undefined < _bigPositive
      // _undefined > _maxOrdinaryValue
      static const long _maxOrdinaryValue = 2147483644L;
      static const long _minOrdinaryValue = -2147483644L;
      static const long _maxSafeSummand = 1073741822L;
      // _maxSafeSummand + _maxSafeSummand <= _maxOrdinaryValue
      static const long _minSafeSummand = -1073741822L;
      // _minSafeSummand + _minSafeSummand >= _minOrdinaryValue
      static const long _maxSafeFactor = 46340L;
      static const long _minSafeFactor = -46340L;
      // _maxSafeFactor * _maxSafeFactor <= _maxOrdinaryValue
      // _maxSafeFactor * _minSafeFactor >= _minOrdinaryValue
      // _minSafeFactor * _minSafeFactor <= _maxOrdinaryValue
 
    private:
      long _content;
    }; // class NonstandardInt

}; // namespace BK



inline
ostream& operator<<(ostream& str,const BK::NonstandardInt& n)
{
  return n.output(str);
}; 

//============================================================================
#endif




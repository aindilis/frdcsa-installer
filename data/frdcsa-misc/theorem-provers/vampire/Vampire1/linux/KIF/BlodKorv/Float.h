//
// File:         Float.h
// Description:  Floating point arithmetics with sane comparisons. 
// Created:      Jun 24, 2002, Alexandre Riazanov,riazanov@cs.man.ac.uk  
//============================================================================
#ifndef FLOAT_H
#define FLOAT_H 
//=============================================================================
#include <math.h>
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FLOAT
 #define DEBUG_NAMESPACE "Float"
#endif
#include "debugMacros.h"
//============================================================================

namespace BK 
{
  class Float
    {
    public:
      Float() : _content(0.0) {}; 
      ~Float() {};
      explicit Float(const float& n) : _content(n) {};
      explicit Float(const long& n) : _content((float)n) {};
      explicit Float(const ulong& n) : _content((float)n) {};
      explicit Float(const int& n) : _content((float)n) {};
      operator float() const { return _content; };
      operator bool() const { return fabsf(_content) > _epsilon; }; 
      bool operator==(const Float& n) const
      {
	return fabsf(_content - n._content) <= _epsilon;
      }; 
      bool operator!=(const Float& n) const
      {
	return !(*this == n);
      };
      bool operator>(const Float& n) const
      {
	return (_content - n._content) > _epsilon; 
      };
      bool operator>=(const Float& n) const
      {
	return !(*this < n);
      };

      bool operator<(const Float& n) const
      {
	return (n._content - _content) > _epsilon; 
      };      

      bool operator<=(const Float& n) const
      {
	return !(*this > n);
      };

      Float operator-() const 
	{
	  return Float(-_content);
	};


      Float operator-(const Float& n) const 
	{
	  return Float(_content - n._content);
	};

      Float& operator-=(const Float& n) 
	{
	  _content -= n._content;
	  return *this;
	};      

      Float operator+(const Float& n) const 
	{
	  return Float(_content + n._content);
	};

      Float& operator+=(const Float& n) 
	{
	  _content += n._content;
	  return *this;
	};

      Float& operator++()
	{
	  ++_content;
	  return *this;
	};

      Float operator*(const Float& n) const 
	{
	  return Float(_content * n._content);
	};

      Float& operator*=(const Float& n) 
	{
	  _content *= n._content;
	  return *this;
	};

      Float operator/(const Float& n) const 
	{
	  return Float(_content / n._content);
	};

    private:
      operator int() const 
	{ 
	  CALL("operator int() const");
	  ICP("ICP0");
	  return 0; 
	};
      operator long() const 
	{ 
	  CALL("operator long() const");
	  ICP("ICP0");
	  return 0L; 
	};
      operator unsigned long() const 
	{ 
	  CALL("operator unsigned long() const");
	  ICP("ICP0");
	  return 0UL; 
	};
    private:
      float _content;
      static const float _epsilon = 0.00000000001;
    }; // class Float

}; // namespace BK 


inline
ostream& operator<<(ostream& str,const BK::Float& n)
{
  return str << (float)n;
}; 



//============================================================================
#endif

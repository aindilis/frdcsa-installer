//
// File:         jargon.h
// Description:  Frequently used definitions.
// Created:      May 5, 2000, 20:20
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef JARGON_H
//============================================================================
#define JARGON_H
#include "target.h"
#include "compiler.h"
typedef unsigned long ulong;


namespace BK 
{

class DummyArgument
{
 public:
  DummyArgument() {};
  void touch() const {};
};

static DummyArgument dummyArgument;


inline
void init(long& x) {};
inline
void init(long& x,const long& y) 
{
  x = y;
};
inline
void init(long& x,const DummyArgument& da) 
{
#if (!defined NO_DEBUG) || (defined _SUPPRESS_WARNINGS_)
  da.touch();
#endif
  x = 0L;
};

inline
void destroy(long& x) {};
inline
void relocateFrom(long& x,long& y)
{
  x = y;
};

inline
void init(ulong& x) {};
inline
void init(ulong& x,const ulong& y) 
{
  x = y;
};
inline
void init(ulong& x,const DummyArgument& da) 
{
#if (!defined NO_DEBUG) || (defined _SUPPRESS_WARNINGS_)
  da.touch();
#endif
  x = 0UL;
};
inline
void destroy(ulong& x) {};
inline
void relocateFrom(ulong& x,ulong& y)
{
  x = y;
};

template <class T>
inline
void init(T*& x) {};

template <class T>
inline
void init(T*& x,const T*& y) 
{
  x = y;
};

template <class T>
inline
void init(T*& x,const DummyArgument& da) 
{
#if (!defined NO_DEBUG) || (defined _SUPPRESS_WARNINGS_)
  da.touch();
#endif
  x = (T*)0;
};

template <class T>
inline
void destroy(T*& x) {};

template <class T>
inline
void relocateFrom(T*& x,T*& y)
{
  x = y;
};


template <class T>
inline
void touch(const T& x) 
{
};


}; // namespace BK




//============================================================================
#endif

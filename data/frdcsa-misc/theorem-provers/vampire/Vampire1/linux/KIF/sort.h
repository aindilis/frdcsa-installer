// *************************************************************
//
//  Class Sort<C,n> for sorting
//  06/05/2002 
//
// *************************************************************


#ifndef __sort__
#  define __sort__


#include "debug.h"
#include "assert.h"
#include "memory.h"


// ******************* Class CList *******************************


template <class C>
class Sort
{
 public:

  inline
  Sort (int length) 
    :
    _elems ( new C[length] ),
    _size (length),
    _length (0)
    {
      if (! _elems) {
        NO_MEMORY;
      }
    }

  ~Sort () 
    {
      delete [] _elems;
    }

  inline int length () const 
    { return _length; }

  // note that this operator can only return values
  inline C operator [] (int n) const
    { 
      ASS(debug_sort, n < _length);
      return _elems[n]; 
    } // Sort::operator []

  inline void add (C c)
    {
      ASS(debug_sort, _length < _size);

      _elems[_length++] = c;
    } // Sort::add

  // remove duplicates from sorted list
  void removeDuplicates ()
    {
      int cur = 0;

      for (int next = 1; next < _length; next++) {
        if ( _elems[cur] != _elems[next] ) {
          cur++;
        }
      }

      // reset length
      _length = cur + 1;
    } // Sort::removeDuplicates

  inline void sort ()
    { sort (0,_length-1); }

  // sort using a comparison function compare on C
  inline void sortF ()
    { sortF (0,_length-1); }

  // sort using a global comparison function compare on C
  // inline void sortGF ()
  //  { sortGF (0,_length-1); }

  // check membership in the sorted list
  inline bool member (const C c) const
    { return member (c,0,_length-1); }

 protected:  // structure

  C* _elems;
  int _size;
  int _length;

  // Quicksort, copied from Cormen et.al's "Introduction to Algorithms"
  void sort ( int p,int r ) 
    {
      #if _debug_sort
        ASS(true, r < _length);
      #endif

      if (p < r) {
        int q = partition(p,r);
        sort(p,q);
        sort(q+1,r);    
      }
    } // Sort::sort

  // Quicksort, copied from Cormen et.al's "Introduction to Algorithms"
  void sortF ( int p,int r ) 
    {
      #if _debug_sort
        ASS(true, r < _length);
      #endif

      if (p < r) {
        int q = partitionF(p,r);
        sortF(p,q);
        sortF(q+1,r);    
      }
    } // Sort::sort

  // Quicksort, copied from Cormen et.al's "Introduction to Algorithms"
  void sortGF ( int p,int r ) 
    {
      #if _debug_sort
        ASS(true, r < _length);
      #endif

      if (p < r) {
        int q = partitionGF(p,r);
        sortGF(p,q);
        sortGF(q+1,r);    
      }
    } // Sort::sort

  int partition( int p,int r ) 
  {
    C x = _elems[p];
    int i = p-1;
    int j = r+1;

    for (;;) {
      do --j;
      while (x < _elems[j]);
      do ++i;
      while (_elems[i] < x);
      if (i < j) {
        // swap [i] and [j]
        C tmp = _elems[i];
        _elems[i] = _elems[j];
        _elems[j] = tmp;
      }
      else
        return j;
    }
  } // Sort::partitition

  int partitionF( int p,int r ) 
  {
    C x = _elems[p];
    int i = p-1;
    int j = r+1;

    for (;;) {
      do --j;
      while (x->isless(_elems[j]));

      do ++i;
      while (_elems[i]->isless(x));

      if (i < j) {
        // swap [i] and [j]
        C tmp = _elems[i];
        _elems[i] = _elems[j];
        _elems[j] = tmp;
      }
      else
        return j;
    }
  } // Sort::partititionF

  int partitionGF( int p,int r ) 
  {
    C x = _elems[p];
    int i = p-1;
    int j = r+1;

    for (;;) {
      do --j;
      while ( isless(_elems[j],x) );

      do ++i;
      while ( isless(x,_elems[i]) );

      if (i < j) {
        // swap [i] and [j]
        C tmp = _elems[i];
        _elems[i] = _elems[j];
        _elems[j] = tmp;
      }
      else
        return j;
    }
  } // Sort::partititionGF

  bool member (const C c, int fst, int lst) const
    { 
      for (;;) {
        if (fst > lst) {
          return false;
        }

        int mid = (fst + lst) / 2;
      
        if (_elems[mid] == c) {
          return true;
        }

        if (_elems[mid] < c) {
          lst = mid-1;
        }
        else { // _elems[mid] > c
          fst = mid+1;
        }
      }
    } // Sort::member
};  // class Sort


#endif



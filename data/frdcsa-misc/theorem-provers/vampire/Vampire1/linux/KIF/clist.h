// *************************************************************
//
//  Class CList<C> for implementing lists with reference counting
//  28/04/2002 Manchester, made from List
//
// *************************************************************


#ifndef __clist__
#  define __clist__


#include "debug.h"
#include "assert.h"
#include "memory.h"


// ******************* Class CList *******************************

template <class C,ClassID ID>
class CList
#   if debug_memory
    : public Memory <ID>
#   endif
{
 public:

  inline
  CList ( C head, CList* tail )
    :
    _head ( head ),
    _tail ( tail ),
    _count (0)
  {
    head->inc ();
    if (tail) {
      tail->inc ();
    }
  } // CList::CList

  // constructors
  inline
  CList ( C head )
    :
    _head (head),
    _tail (0),
    _count (0)
  {
    head->inc ();
  } // CList::CList

  // create empty list
  inline
  static CList* empty ()  
  {
    return 0;
  }

  // reference counting
  inline
  void inc () 
    { if (isEmpty()) {
        return;
      }
      _count++; 
    }

  inline
  void dec () 
    { if (isEmpty()) {
        return;
      }
      ASS(debug_clist,_count > 0); 
      _count--; 
      if (_count == 0) {
        destroy ();
      }
    }

  inline
  bool isEmpty () const
  {
    return this == 0;
  } // CList::isEmpty

  inline
  bool isNonEmpty () const
  {
    return this != 0;
  } // CList::isNonEmpty

  inline
  CList* tail () const
  {
    ASS ( debug_clist, this != 0 );

    return _tail;
  } // CList::tail

  inline
  void tail ( CList* tl )
  {
    ASS ( debug_clist, this != 0 );
    //  ASS ( debug_clist_check_validity, Memory::isValid (this, name ()) );

    tl->inc ();
    if ( _tail ) {
      _tail->dec ();
    }

    _tail = tl;
  } // list::tail

  inline
  C head () const
  {
    ASS ( debug_clist, this != 0 );
  //  ASS ( debug_clist_check_validity, Memory::isValid (this, name ()) );

    return _head;
  } // CList::head

  // second element of the list
  inline
  C second () const
  {
    ASS ( debug_clist, this != 0 &&
                         _tail != 0 );
  //  ASS ( debug_clist_check_validity, Memory::isValid (this, name ()) );

    return _tail->_head;
  } // CList::second

/*
  void head ( C head )
  {
    ASS ( debug_clist, this != 0 );
  //  ASS ( debug_clist_check_validity, Memory::isValid (this, name ()) );

    _head = head;
  } // CList::head
*/

  void destroy ()
  {
    ASS (debug_clist, _count == 0);
    
    head()->dec();
    tail()->dec();
    delete this;
  }

/*
  CList* copy () const
  {
    if ( isEmpty () )
      return empty ();

    CList* result = new CList;
    result->head ( head () );
    CList* previous = result;
    CList* rest = tail ();

    while ( ! rest->isEmpty () ) {
      CList* tmp = new CList;
      tmp->head ( rest->head () );
      previous->tail ( tmp );
      previous = tmp;
      rest = rest->tail ();
    }

    previous->tail ( empty () );

    return result;
  }  // CList::copy

  CList* append (CList* snd) 
  {
    if ( isEmpty () )
      return snd;

    CList* result = new CList;
    result->head ( head () );
    CList* previous = result;
    CList* rest = tail ();

    while ( ! rest->isEmpty () ) {
      CList* tmp = new CList;
      tmp->head ( rest->head () );
      previous->tail ( tmp );
      previous = tmp;
      rest = rest->tail ();
    }

    previous->tail ( snd );

    return result;
  }  // CList::append

  inline
  CList* cons ( C elem )
  {
    return new CList ( elem, this );
  } // CList::cons

  inline
  static C pop ( CList* &lst )
  {
    ASS ( debug_clist, lst != 0 );

    CList* tail = lst->tail ();
    C result = lst->head ();
    delete lst;
    lst = tail;

    return result;
  } // CList::pop

  // moves the top from from to to
  inline 
  static void move ( CList*& from, CList*& to )
  {
    CList* tail = from->tail ();
    from->tail ( to );
    to = from;
    from = tail;
  } // CList::move
*/

  inline
  static void push ( C elem, CList* &lst )
  {
    lst = new CList (elem, lst);
  } // CList::push


  // list concatenation, should be used with extreme care
  static CList* concat( CList* first, CList* second )
  {
    if ( first == 0 )
      return second;

    // an optimization, not really necessary
    if ( second == 0 )
      return first;

    CList* current = first;
    for (;;) {
      CList* next = current->tail ();
      if ( ! next ) {
        // if reference count for the last element is more than one
        // then something is wrong
        ASS (debug_clist, current->_count <= 1);
        // note that an assignment is used here since the counters don't change
        current->tail (second);
        return first;
      }
      current = next;
      next = next->tail ();
    }
  } // CList::concat


  // Manchester 12/05/2002
  CList* append ( CList* to )
  {
    if ( isEmpty() ) {
      return to;
    }

    return new CList ( head(), tail()->append(to) );
  } // CList::append

/*
  CList* reverse ()
  {
    if ( isEmpty () )
      return empty ();

    CList* result = empty ();
    CList* lst = this;

    while ( ! lst->isEmpty () ) {
      CList* tl = lst->tail ();
      lst->tail ( result );
      result = lst;
      lst = tl;
    }

    return result;
  } // CList::reverse
*/

  // length of the list
  int length () const
  {
    int len = 0;

    for ( const CList* lst = this; ! lst->isEmpty () ; lst = lst->tail() )
      len ++;

    return len;
  } // CList::length


/*
  // C is a member of the list
  bool member ( C elem )
  {
    for ( CList* lst = this; ! lst->isEmpty () ; lst = lst->tail() ) {
      if ( lst->head() == elem )
        return true;
    }

    return false;
  } // CList::member

  // removes first occurrence of elem from the list and deletes it
  CList* remove ( C elem )         
  {
    if ( isEmpty() )
      return this;

    if ( head() == elem ) {
      CList* result = tail ();
      delete this;
      return result;
    }

    if ( tail()->isEmpty() )
      return this;

    CList* current = this;
    CList* next = tail ();

    for (;;) {
      if ( next->head() == elem ) { // element found
        current->tail ( next->tail() );
        delete next;
        return this;
      }
      current = next;
      next = next->tail();
      if ( next->isEmpty() )
        return this;
    }
  } // CList::remove
  
  // Nth element, counting from 0
  C nth ( int N ) const 
  {     
    // Nth element, counting from 0
    ASS ( debug_clist, N >= 0 );

    const CList* l = this;

    while ( N != 0 ) {
      ASS ( debug_clist, l->isNonEmpty() );

      l = l->tail ();
      N-- ;
    }

    return l->head();
  } // nth

  // delete nth element and return it as the result
  static C deleteNth ( CList*& lst, int N ) 
  {     
    // Nth element, counting from 0
    ASS ( debug_clist, N >= 0 );

    C result;
    CList* l = lst;
    ASS ( debug_clist, lst->isNonEmpty() );

    if ( N == 0 ) {
      result = l->head();
      lst = l->tail();
      delete l;
      return result;
    }

    // N != 0
    CList* next = l->tail();

    while ( --N != 0 ) {
      l = next;
      next = next->tail ();
      ASS ( debug_clist, next->isNonEmpty() );
    }
    //  now next must be deleted
    result = next->head();
    l->tail (next->tail());
    delete next;

    return result;
  } // deleteNth

  // add last element
  CList* addLast ( C elem )
  {
    if ( ! this ) 
      return new CList (elem);

    // nonempty, trying to find the end
    CList* current;
    for ( current = this; current->_tail; current = current->_tail )
      ;

    current->tail ( new CList (elem) );

    return this;
  } // CList::addLast

  // split into two sublists, first of the length n
  CList* split ( int n, CList*& rest )
  {
    if ( ! this ) {
      ASS( debug_clist, n == 0 );
      rest = empty ();
      return empty ();
    }

    if ( n == 0 ) {
      rest = empty ();
      return this;
    }

    CList* nth = this;
    while ( --n > 0 ) {
      ASS( debug_clist, nth );
      nth = nth->_tail;
    }
    
    ASS( debug_clist, nth );
    rest = nth->_tail;
    nth->_tail = empty ();
    return this;
  } // CList::split

*/

  class Iterator {
   public:
    
    inline explicit 
    Iterator ( CList* l ) : _lst (l) {}  
    inline explicit 
    Iterator ( const CList* l ) : _lst (const_cast<CList*>(l)) {}  
    
    inline 
    C next () 
      { 
        C result = _lst->head(); 
        _lst = _lst->tail();
        return result;
      }
    
    inline 
    bool more () { return ! _lst->isEmpty(); }

    inline
    void reset ( CList* l ) { _lst = l; }

   private:
    CList* _lst;
  };

/*
  class DelIterator {
   public:
    
    inline 
    DelIterator ( CList*& l ) : 
      _lst ( l ),
      #ifdef debug_clist
        _prev ( 0 ), // required to check the double deletion ASS
      #endif
      _cur ( 0 )
      {}  
    
    inline 
    C next () 
      { 
        if ( _cur ) { // there was an element previously returned by next()
          _prev = _cur;
          _cur = _cur->tail();
          ASS( debug_clist, _cur );
        }
        else { // result must be the first element of the list
          _cur = _lst;
        }
        return _cur->head();
      }
    
    inline 
    bool more () 
      { 
        if ( _cur ) { // there was an element previously returned by next()
          return _cur->tail() != 0;
        }
        return ! _lst->isEmpty(); 
      }

    inline
    void del () 
      {
        // we can only delete the element returned by next
        ASS( debug_clist, _cur ); 
        // check that two delete requests in row did not occur
        ASS( debug_clist, _cur != _prev ); 

        if ( _cur == _lst ) { // the first element must be deleted
          _lst = _lst->tail ();
          delete _cur;
          _cur = 0;
          return;
        }

        // not the first element 
        _prev->tail (_cur->tail());
        delete _cur;
        _cur = _prev;
      }

    // 04/06/2001
    void restore ( C elem ) 
      {
        if ( _cur ) { // not the first element was deleted
          // we use that _cur and _prev point to the same element
          _cur = new CList (elem,_cur->tail());
          _prev->tail (_cur);
          return;
        }
        // the first element was deleted
        push (elem, _lst);
        // _cur == 0, making the state as if just one more and next were called
        _cur = _lst;
      }

   private:
    CList*& _lst;
    CList* _prev;
    CList* _cur; // _cur is the element returned by the last next, if 0, then no next was called
  };
*/

 protected:  // structure

  C _head;
  CList* _tail;
  long _count;
};  // class CList


#endif

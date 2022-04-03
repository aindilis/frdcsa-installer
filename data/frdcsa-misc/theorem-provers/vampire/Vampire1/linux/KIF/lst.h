// *************************************************************
//
//  Class Lst<C> for implementing lists with reference counting
//  28/04/2002 Manchester, made from List
//
// *************************************************************


#ifndef __clist__
#  define __clist__


#include "debug.h"
#include "assert.h"
#include "memory.h"


// ******************* Class Lst *******************************

template <class C>
class Lst
#   if debug_memory
    : public Memory <ID>
#   endif
{
 private:
  class Content;
 public:
  Lst () : _content (0) {}
  void operator = (Lst& rhs);
  bool operator == (Lst& rhs);

 private: // structure
  Content* _content;

class Content 
{
 public:
  inline
  Content ( C head, Lst* tail )
    :
    _head ( head ),
    _tail ( tail ),
    _count (0)
  {
    head->inc ();
    if (tail) {
      tail->inc ();
    }
  } // Lst::Lst

  // constructors
  inline
  Content ( C head )
    :
    _head (head),
    _tail (0),
    _count (0)
  {
    head->inc ();
  } // Lst::Lst

  // create empty list
  inline
  static Lst* empty ()  
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
  } // Lst::isEmpty

  inline
  bool isNonEmpty () const
  {
    return this != 0;
  } // Lst::isNonEmpty

  inline
  Lst* tail () const
  {
    ASS ( debug_clist, this != 0 );

    return _tail;
  } // Lst::tail

  inline
  void tail ( Lst* tl )
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
  } // Lst::head

  // second element of the list
  inline
  C second () const
  {
    ASS ( debug_clist, this != 0 &&
                         _tail != 0 );
  //  ASS ( debug_clist_check_validity, Memory::isValid (this, name ()) );

    return _tail->_head;
  } // Lst::second

/*
  void head ( C head )
  {
    ASS ( debug_clist, this != 0 );
  //  ASS ( debug_clist_check_validity, Memory::isValid (this, name ()) );

    _head = head;
  } // Lst::head
*/

  void destroy ()
  {
    ASS (debug_clist, _count == 0);
    
    head()->dec();
    tail()->dec();
    delete this;
  }

/*
  Lst* copy () const
  {
    if ( isEmpty () )
      return empty ();

    Lst* result = new Lst;
    result->head ( head () );
    Lst* previous = result;
    Lst* rest = tail ();

    while ( ! rest->isEmpty () ) {
      Lst* tmp = new Lst;
      tmp->head ( rest->head () );
      previous->tail ( tmp );
      previous = tmp;
      rest = rest->tail ();
    }

    previous->tail ( empty () );

    return result;
  }  // Lst::copy

  Lst* append (Lst* snd) 
  {
    if ( isEmpty () )
      return snd;

    Lst* result = new Lst;
    result->head ( head () );
    Lst* previous = result;
    Lst* rest = tail ();

    while ( ! rest->isEmpty () ) {
      Lst* tmp = new Lst;
      tmp->head ( rest->head () );
      previous->tail ( tmp );
      previous = tmp;
      rest = rest->tail ();
    }

    previous->tail ( snd );

    return result;
  }  // Lst::append

  inline
  Lst* cons ( C elem )
  {
    return new Lst ( elem, this );
  } // Lst::cons

  inline
  static C pop ( Lst* &lst )
  {
    ASS ( debug_clist, lst != 0 );

    Lst* tail = lst->tail ();
    C result = lst->head ();
    delete lst;
    lst = tail;

    return result;
  } // Lst::pop

  // moves the top from from to to
  inline 
  static void move ( Lst*& from, Lst*& to )
  {
    Lst* tail = from->tail ();
    from->tail ( to );
    to = from;
    from = tail;
  } // Lst::move
*/

  inline
  static void push ( C elem, Lst* &lst )
  {
    lst = new Lst (elem, lst);
  } // Lst::push


  // list concatenation, should be used with extreme care
  static Lst* concat( Lst* first, Lst* second )
  {
    if ( first == 0 )
      return second;

    // an optimization, not really necessary
    if ( second == 0 )
      return first;

    Lst* current = first;
    for (;;) {
      Lst* next = current->tail ();
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
  } // Lst::concat


  // Manchester 12/05/2002
  Lst* append ( Lst* to )
  {
    if ( isEmpty() ) {
      return to;
    }

    return new Lst ( head(), tail()->append(to) );
  } // Lst::append

/*
  Lst* reverse ()
  {
    if ( isEmpty () )
      return empty ();

    Lst* result = empty ();
    Lst* lst = this;

    while ( ! lst->isEmpty () ) {
      Lst* tl = lst->tail ();
      lst->tail ( result );
      result = lst;
      lst = tl;
    }

    return result;
  } // Lst::reverse
*/

  // length of the list
  int length () const
  {
    int len = 0;

    for ( const Lst* lst = this; ! lst->isEmpty () ; lst = lst->tail() )
      len ++;

    return len;
  } // Lst::length


/*
  // C is a member of the list
  bool member ( C elem )
  {
    for ( Lst* lst = this; ! lst->isEmpty () ; lst = lst->tail() ) {
      if ( lst->head() == elem )
        return true;
    }

    return false;
  } // Lst::member

  // removes first occurrence of elem from the list and deletes it
  Lst* remove ( C elem )         
  {
    if ( isEmpty() )
      return this;

    if ( head() == elem ) {
      Lst* result = tail ();
      delete this;
      return result;
    }

    if ( tail()->isEmpty() )
      return this;

    Lst* current = this;
    Lst* next = tail ();

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
  } // Lst::remove
  
  // Nth element, counting from 0
  C nth ( int N ) const 
  {     
    // Nth element, counting from 0
    ASS ( debug_clist, N >= 0 );

    const Lst* l = this;

    while ( N != 0 ) {
      ASS ( debug_clist, l->isNonEmpty() );

      l = l->tail ();
      N-- ;
    }

    return l->head();
  } // nth

  // delete nth element and return it as the result
  static C deleteNth ( Lst*& lst, int N ) 
  {     
    // Nth element, counting from 0
    ASS ( debug_clist, N >= 0 );

    C result;
    Lst* l = lst;
    ASS ( debug_clist, lst->isNonEmpty() );

    if ( N == 0 ) {
      result = l->head();
      lst = l->tail();
      delete l;
      return result;
    }

    // N != 0
    Lst* next = l->tail();

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
  Lst* addLast ( C elem )
  {
    if ( ! this ) 
      return new Lst (elem);

    // nonempty, trying to find the end
    Lst* current;
    for ( current = this; current->_tail; current = current->_tail )
      ;

    current->tail ( new Lst (elem) );

    return this;
  } // Lst::addLast

  // split into two sublists, first of the length n
  Lst* split ( int n, Lst*& rest )
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

    Lst* nth = this;
    while ( --n > 0 ) {
      ASS( debug_clist, nth );
      nth = nth->_tail;
    }
    
    ASS( debug_clist, nth );
    rest = nth->_tail;
    nth->_tail = empty ();
    return this;
  } // Lst::split

*/

  class Iterator {
   public:
    
    inline explicit 
    Iterator ( Lst* l ) : _lst (l) {}  
    inline explicit 
    Iterator ( const Lst* l ) : _lst (const_cast<Lst*>(l)) {}  
    
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
    void reset ( Lst* l ) { _lst = l; }

   private:
    Lst* _lst;
  };

/*
  class DelIterator {
   public:
    
    inline 
    DelIterator ( Lst*& l ) : 
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
          _cur = new Lst (elem,_cur->tail());
          _prev->tail (_cur);
          return;
        }
        // the first element was deleted
        push (elem, _lst);
        // _cur == 0, making the state as if just one more and next were called
        _cur = _lst;
      }

   private:
    Lst*& _lst;
    Lst* _prev;
    Lst* _cur; // _cur is the element returned by the last next, if 0, then no next was called
  };
*/

 protected:  // structure

  C _head;
  Lst* _tail;
  long _count;
};  // class Lst::Content

}; // class Lst


#endif

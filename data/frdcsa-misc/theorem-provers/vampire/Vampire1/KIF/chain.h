// *************************************************************
//
//  Class Chain<C> for implementing double-linked lists
//  03/05/2002 Manchester
//
// *************************************************************


#ifndef __chain__
#  define __chain__


#include "debug.h"
#include "assert.h"
#include "memory.h"
#include "sort.h"


// *******************Class Chain*******************************

template <class C,ClassID ID>
class Chain
#   if debug_memory
    : public Memory <ID>
#   endif
{
 public:
  class Link
#   if debug_memory
    : public Memory <ID>
#   endif
  {
   private:
    Link* _prev;
    Link* _next;
    C _elem;
   public:
    inline Link (C elem) 
      : _elem (elem) 
      {
        _elem->inc ();
      }

    inline Link ()  // for making the header to a chain only
      { 
        _prev = this;
        _next = this; 
        _elem = 0;
      }

    inline ~Link ()  // for making the header to a chain only
      { 
        TRACER( "Chain::Link::~Link" );

        if (_elem) {
          _elem->dec ();
        }
      }

    inline Link* prev () const { return _prev; }
    inline void prev ( Link* p ) { _prev = p; }
    inline Link* next () const { return _next; }
    inline void next ( Link* n ) { _next = n; }

    inline bool less (const Link* lnk) const 
      { return _elem->less (lnk->_elem); }

    void insertAfter (C elem)
      {
        Link* l = new Link (elem);
        l->_prev = this;
        l->_next = _next;
        _next->_prev = l;
        _next = l;
      }

    void insertBefore (C elem)
      {
        Link* l = new Link (elem);
        l->_next = this;
        l->_prev = _prev;
        _prev->_next = l;
        _prev = l;
      }

    void del ()
      {
        TRACER( "Chain::del" );
        ASS(debug_chain, _next != this && _prev != this);

        _prev->_next = _next;
        _next->_prev = _prev;
        delete this;
      }

    C content () const { return _elem; }
  }; // Class Chain::Link

  // I had to make this public since VC++ ignores my declaration of DelIterator as friend
  inline const Link* header () const { return &_header; }

  // Chain public members
  inline void addLast (C elem)   
    { _header.insertBefore (elem); }

  inline void addFirst (C elem)   
    { _header.insertAfter (elem); }

  inline void delLast ()    
    { _header.prev()->del(); }

  void delFirst ()    
    { _header.next()->del(); }

  inline bool isEmpty () const
    { return _header.prev() == &_header; }

  Link* first () const
    { ASS(debug_chain, ! isEmpty());
      return _header.next(); }

  C last () const
    { ASS(debug_chain, ! isEmpty());
      return _header.prev(); }

  void sort () // sort links using the less function on C
    {
      int len = length ();
      Sort<Link*> srt (len);

      for (Link* lnk = _header.next ();
           lnk != &_header;
           lnk = lnk->next () ) {
        srt.add (lnk);
      }

      // sort the chain using the comparison function less ()
      srt.sortF ();

      // sort now contains a sorted array of links
      // insert them one by one to the chain
      Link* last = &_header; // the last link
      for (int i = len - 1; i >= 0; i--) {
        Link* nxt = srt[i];
        // remove nxt from the chain
        nxt->prev()->next (nxt->next());
        nxt->next()->prev (nxt->prev());
        // insert before the last link
        nxt->next (last);
        nxt->prev (last->prev());
        nxt->prev()->next (nxt);
        last->prev (nxt);
        last = nxt;
      }
    }

  int length () const
    { int len = 0;
      Iterator links (*this);
      while (links.more()) {
        links.next ();
        len ++;
      }
      return len;
    } // length ()
      

  class Iterator {
   public: 
    inline explicit 
    Iterator (Chain& c) 
      : _header (c.header()),
        _current (_header->next())
        {}
    Iterator (const Chain& c) 
      : _header (c.header()),
        _current (_header->next())
        {}
    
    inline 
    bool more () 
      { return _header != _current; }

    inline 
    C next () 
      { ASS(debug_chain, more());

        C result = _current->content();
        _current = _current->next();

        return result;
      }
    
   private:
    const Link* _header;
    Link* _current;
  };

  class DelIterator {
   public:
    
    inline 
    DelIterator ( Chain& c ) : 
      _header ( c.header() ),
      _current ( _header->next() )
      {}  
    
    inline 
    bool more () 
      { return _header != _current; }

    inline 
    C next () 
      { 
        ASS(debug_chain, _current != _header);
        
        C result = _current->content();
        _current = _current->next();
        return result;
      }
    
    inline
    void del () 
      {
        ASS( debug_chain, _current->prev() != _header ); 

        _current->prev()->del();
      }

    // 07/05/2002 Manchester
    inline
    void replace (C elem) 
      {
        ASS( debug_chain, _current->prev() != _header ); 

        _current->prev()->del();
        _current->insertBefore (elem);
      }

   private:
    const Link* _header;
    Link* _current;
  };

 private:
  Link _header;

 friend class Iterator;
};  // class Chain


#endif



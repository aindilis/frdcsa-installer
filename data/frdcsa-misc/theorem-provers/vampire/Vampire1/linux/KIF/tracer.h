//
//  File tracer.h
//  01/05/2002 Manchester
//

#ifndef __tracer__
#  define __tracer__

#define debug_trace_all 0


class Tracer {
 public:
  Tracer (const char* fun);
  ~Tracer ();

 private:
  const char* _fun;
  static int _depth;
};


#if debug_trace_all
#  define TRACER(Fun) Tracer tmp (Fun)
#else
#  define TRACER(Fun) 
#endif

#endif

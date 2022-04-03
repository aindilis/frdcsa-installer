#ifndef OR_MACHINE_H 
//==========================================================================
#define OR_MACHINE_H
#include "jargon.h"
#include "iu_command.h"
#include "prefix.h"
#include "variables.h"
#include "or_index.h"
#include "iu_machine.h"
//==========================================================================

namespace VK
{

class OR_MACH : public IU_MACH
 {
  public:
   OR_MACH(OR_INDEX* resIndex) : _ordResIndex(resIndex) {};
   ~OR_MACH() {};
   void init(OR_INDEX* resIndex) { IU_MACH::init(); _ordResIndex = resIndex; };
   void assignResIndex(OR_INDEX* resIndex) { _ordResIndex = resIndex; };
   bool FindFirstLeaf()
    {
     return IU_MACH::FindFirstLeaf(*(_ordResIndex->Tree(Code()->Symbol().Func())));
    };
   void Destroy() 
   {
    IU_MACH::Destroy();
   };
   #ifndef NO_DEBUG
    bool clean() const 
    {
     return IU_MACH::clean();
    };
   #endif    
  private:
   OR_INDEX* _ordResIndex;
 }; // class OR_MACH : public IU_MACH

}; // namespace VK

//==========================================================================
#endif


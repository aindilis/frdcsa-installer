TYPES
{
 Pos  =  positions in the query
 NestPos = nest positions in the query
 MaxPos = positions in query terminated by constants
 VarPos = positions with variables  
 Fin(X) = finite subsets of X
 Seq(X) = sequences of elements from X
 Seq+(X) = sequences of elements from X 
 Var = variables 
}

FUNCTIONS
{
 varPositions : NestPos -> Seq(VarPos) 
              & varPositions(p) = all variable positions in the nest p  
              & p != q => sequences varPositions(p) and varPositions(q) 
                          do not contain common var. pos. 
  
 var : VarPos -> Var
     & var(p) = number of the variable in the position p      
    
}

INPUT
{
 primPos = [p_1,...,p_m] : Seq+(NestPos U MaxPos) = all primary positions in the query
                                                in certain order
                     & i != j => p_i != p_j
 secNests : Fin(NestPos) = all secondary nests in the query
}
                       
LOCAL VARIABLES 
{
 availVarPositions : Fin(VarPos) = {}  
 usedVarPositions : Fin(VarPos) = {}   
 substLoaded : Fin(Var) = {}
}
            
PROCEDURE
 
  /* Primary code */
 for i = 1..m
  {
   if (p_i : MaxPos)
    {
     write PrimaryMaxPos(p_i)
    }
   else
    {
     write PrimaryNestPos(p_i);
    
     // Write all immediately available Compare instruction
     for j = 1..k where [vpos_1,..,vpos_k] = varPositions(p_i)
      {
       if var(vpos_j) <- substLoaded
        {          
         write Compare(vpos_j);
         usedVarPositions := usedVarPositions U {vpos_j};
        };
      }; // for j = 1..k where [vpos_1,..,vpos_k] = varPositions(p_i)   
       
     // Write Compare instructions that can be enabled
     // by writing LoadSubst for some positions from availVarPositions
     for j = 1..k where [vpos_1,..,vpos_k] = varPositions(p_i)
      { 
       if (vpos_j <!- usedVarPositions)
        {
         if (some avp <- availVarPositions) 
             &
            (var(avp) = var(vpos_j))            
          { 
           write LoadSubst(avp);
           substLoaded := substLoaded U {var(avp)};
           availVarPositions := availVarPositions \ {avp};
           usedVarPositions := usedVarPositions U {avp};
           write Compare(vpos_j);
           usedVarPositions := usedVarPositions U {vpos_j};
           
           // Now some positions in [vpos_{j+1},..,vpos_k]             
           // can also be mapped into Compare instructions
                         
           for h = j+1..k
            { 
             if (var(vpos_h) = var(vpos_j)
              {
               INVARIANT var(vpos_h) <- substLoaded
               write Compare(vpos_h);             
               usedVarPositions := usedVarPositions U {vpos_h};
              };
            }; // for h = j+1..k
          }
         else 
          {
           // vpos_j must become available 
           availVarPositions := availVarPositions U {vpos_j};
          };
        }; // if (vpos_j <!- usedVarPositions)
      }; // for j = 1..k where [vpos_1,..,vpos_k] = varPositions(p_i)
    }; // if (pi : MaxPos) .. else ..
  }; // for i = 1..m
  
  /* Compile secondary nests */
 
  while secNests != {}
   { 
    // Select the most "promising" nest
    
    LOCAL FUNCTIONS
     {
      numOfImmediateComp : NestPos -> int 
       & numOfImmediateComp(nst) = |{vpos <- varPositions(nst)| var(vpos) <- substLoaded}| 
    
      numOfAvailableComp : NestPos -> int 
       & numOfAvailableComp(nst) = |{vpos <- varPositions(nst)| exists vp <- availVarPositions. var(vp) = var(vpos)}|
 
      numOfAllVarPos : NestPos -> int 
       & numOfAllVarPos(nst) = |varPositions(nst)|
     }

    let selectedNest <- secNests 
      & <numOfImmediateComp(selectedNest),numOfAvailableComp(selectedNest),numOfAllVarPos(selectedNest)> =
         max <numOfImmediateComp(nst),numOfAvailableComp(nst),numOfAllVarPos(nst)> | nst <- secNests;
    secNests := secNests \ {selectedNest}; 
       
    // Compile selectedNest
     
    write GetToTermId(selectedNest);
       
    // Write the immediately available Compare instructions
    
    for i = 1..k where [vpos_1,..,vpos_k] = varPositions(selectedNest)
     {
      if var(vpos_i) <- substLoaded
       { 
        write Compare(vpos_i)
        usedVarPositions := usedVarPositions U {vpos_i};   
       };
     };    
        
    // Write Compare instructions that can be enabled
    // by writing LoadSubst for some positions from availVarPositions
        
    for i = 1..k where [vpos_1,..,vpos_k] = varPositions(selectedNest)
     {
      if vpos_i <!- usedVarPositions
       {
        INVARIANT var(vpos_i) <!- substLoaded
           
        if (some avp <- availVarPositions) 
            &
           (var(avp) = var(vpos_i))            
         { 
          write LoadSubst(avp);
          substLoaded := substLoaded U {var(avp)};
          availVarPositions := availVarPositions \ {avp};
          usedVarPositions := usedVarPositions U {avp};
          
          write Compare(vpos_i);
          usedVarPositions := usedVarPositions U {vpos_i};
           
          // Now some positions in [vpos_{i+1},..,vpos_k]             
          // can also be mapped into Compare instructions
                         
          for h = i+1..k
           { 
            if var(vpos_h) = var(vpos_i)
             {
              INVARIANT var(vpos_h) <- substLoaded
              write Compare(vpos_h);             
              usedVarPositions := usedVarPositions U {vpos_h};
             };
           }; // for h = j+1..k
         }
        else 
         {
          // vpos_i must become available 
          availVarPositions := availVarPositions U {vpos_i};
         };
       };
     };
   }; // while secNests != {}

  /* Compile singleton variable positions */
  /* accumulated in availVarPositions */

  for vpos <- availVarPositions
   {
    INVARIANT var(vpos) <!- substLoaded 
    write LoadSubst(vpos);    
   }; // for vpos <- availVarPositions

END OF PROCEDURE

































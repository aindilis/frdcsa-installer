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
}

INPUT
{
 primPos = [p_1,...,p_m] : Seq+(NestPos U MaxPos) = all primary positions in the query
}
                       
LOCAL VARIABLES 
{
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
     };    
     

   }; // for i = 1..m

END OF PROCEDURE

































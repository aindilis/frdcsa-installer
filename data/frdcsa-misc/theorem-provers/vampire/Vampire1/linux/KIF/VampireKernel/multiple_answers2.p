


input_clause(cl1,axiom,
 [
	     ++p(a)
	     ]).

input_clause(cl2,axiom,
 [
	     --p(X),
	     ++p(f(X))
 ]).



input_clause(cl3,conjecture,
 [
	     --p(Y),
	     ++answer(Y)
	     ]).


	     answer(answer,1).

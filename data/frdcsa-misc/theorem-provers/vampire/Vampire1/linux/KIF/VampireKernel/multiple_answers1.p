


input_clause(cl1,axiom,
 [
	     ++p(a)
	     ]).

input_clause(dummy,axiom,
 [
	     ++dummy(1)
	     ]).




input_clause(cl2,axiom,
 [
	     --p(X),
	     ++p(f(X))
 ]).


input_clause(dummy,axiom,
 [
	     ++dummy(2)
	     ]).


input_clause(cl3,conjecture,
 [
	     --p(Y),
	     ++answer(Y)
	     ]).


	     answer(answer,1).


input_clause(dummy,axiom,
 [
	     ++dummy(3)
	     ]).



input_clause(cl4,axiom,
 [
	     ++q(a)
	     ]).

input_clause(dummy,axiom,
 [
	     ++dummy(4)
	     ]).


input_clause(cl5,conjecture,
 [
	     --q(X),
	     ++answer(X)
	     ]).


input_clause(dummy,axiom,
 [
	     ++dummy(5)
	     ]).


input_clause(cl6,axiom,
 [
	     ++r(a)
	     ]).


input_clause(dummy,axiom,
 [
	     ++dummy(6)
	     ]).

input_clause(cl7,conjecture,
 [
	     --r(X),
	     ++answer(X)
	     ]).


input_clause(dummy,axiom,
 [
	     --dummy(X),
	     ++dummy(plus(X,X))
	     ]).


input_clause(dummy,conjecture,
 [
	     --dummy(X),
	     --greater(X,20),
	     ++answer(X)
	     ]).



interpret(function,plus,2,floating_point_arithmetic,binary_plus).
interpret(predicate,greater,2,floating_point_arithmetic,greater).

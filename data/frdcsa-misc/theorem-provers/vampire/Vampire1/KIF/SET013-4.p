%--------------------------------------------------------------------------
% File     : SET013-4 : TPTP v2.4.1. Bugfixed v1.2.1.
% Domain   : Set Theory
% Problem  : The intersection of sets is commutative
% Version  : [BL+86] axioms : Reduced > Incomplete.
% English  : 

% Refs     : [BL+86] Boyer et al. (1986), Set Theory in First-Order Logic: 
% Source   : [ANL]
% Names    : inters.ver2.in [ANL]

% Status   : unsatisfiable
% Rating   : 0.27 v2.4.0, 0.12 v2.2.1, 0.14 v2.2.0, 0.20 v2.1.0, 0.75 v2.0.0
% Syntax   : Number of clauses    : 12 (2 non-Horn;4 unit;10 RR)
%            Number of literals   : 23 (6 equality)
%            Maximal clause size  : 3 (1 average)
%            Number of predicates : 3 (0 propositional; 1-2 arity)
%            Number of functors   : 8 (6 constant; 0-2 arity)
%            Number of variables  : 19 (4 singleton)
%            Maximal term depth   : 2 (1 average)

% Comments : 
%          : tptp2X -f tptp -t rm_equality:rstfp SET013-4.p 
% Bugfixes : v1.2.1 - Missing substitution axioms added.
%--------------------------------------------------------------------------
input_clause(a2,axiom,
    [--member(X,Y),
     ++little_set(X)]).

input_clause(extensionality1,axiom,
    [++little_set(f1(X,Y)),
     ++equal(X,Y)]).

input_clause(extensionality2,axiom,
    [++member(f1(X,Y),X),
     ++member(f1(X,Y),Y),
     ++equal(X,Y)]).

input_clause(extensionality3,axiom,
    [--member(f1(X,Y),X),
     --member(f1(X,Y),Y),
     ++equal(X,Y)]).

input_clause(intersection1,axiom,
    [--member(Z,intersection(X,Y)),
     ++member(Z,X)]).

input_clause(intersection2,axiom,
    [--member(Z,intersection(X,Y)),
     ++member(Z,Y)]).

input_clause(intersection3,axiom,
    [++member(Z,intersection(X,Y)),
     --member(Z,X),
     --member(Z,Y)]).

input_clause(empty_set,axiom,
    [--member(Z,empty_set)]).

input_clause(universal_set,axiom,
    [++member(Z,universal_set),
     --little_set(Z)]).

input_clause(intersection_of_a_and_b_is_c,hypothesis,
    [++equal(intersection(as,bs),cs)]).

input_clause(intersection_of_b_and_a_is_d,hypothesis,
    [++equal(intersection(bs,as),ds)]).

input_clause(prove_c_equals_d,conjecture,
    [--equal(cs,ds)]).
%--------------------------------------------------------------------------

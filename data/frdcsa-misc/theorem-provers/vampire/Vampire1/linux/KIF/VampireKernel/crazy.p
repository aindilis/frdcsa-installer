input_clause(ax1,axiom,
	[
		++equal(a(M),Y),
		++equal(a(inv_a(M,Y)),Y)
	]).

input_clause(ax2,axiom,
	[
	        --equal(a(M),Y),
	        --equal(a(inv_a(M,Y)),Y)
	 ]).
	         				
input_clause(ax1,axiom,
	[
		++equal(b(M),Y),
		++equal(b(inv_b(M,Y)),Y)
	]).

input_clause(ax2,axiom,
	[
	        --equal(b(M),Y),
	        --equal(b(inv_b(M,Y)),Y)
	 ]).
	         				
	         				
input_clause(ax2,axiom,
       [
	     --equal(M,s(M))
       ]).


input_clause(g,conjecture,
	[
		--equal(a(M),b(M))
	]).
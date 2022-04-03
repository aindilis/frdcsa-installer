input_clause(cl1,conjecture,
	     [
	     --equal(X,unary_minus(plus(Y,Z))),
	     --equal(Y,minus(A,Z)),
	     --equal(Z,unary_plus(multiply(2.1,4.7))),
	     --equal(A,divide(1.7,2.3)),
	     ++less_or_equal(Z,A),
	     ++answer(max(unary_minus(Z),divide(A,0)))
	     ]).

answer(answer).

interpret(plus,floating_point_arithmetic,binary_plus).
interpret(unary_plus,floating_point_arithmetic,unary_plus).
interpret(minus,floating_point_arithmetic,binary_minus).
interpret(unary_minus,floating_point_arithmetic,unary_minus).
interpret(multiply,floating_point_arithmetic,multiply).
interpret(divide,floating_point_arithmetic,divide).
interpret(max,floating_point_arithmetic,max2).
interpret(greater,floating_point_arithmetic,greater).
interpret(greater_or_equal,floating_point_arithmetic,greater_or_equal).
interpret(less,floating_point_arithmetic,less).
interpret(less_or_equal,floating_point_arithmetic,less_or_equal).

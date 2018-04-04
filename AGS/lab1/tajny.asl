// Agent tajny in project lab1.mas2j

/* Initial beliefs and rules */

/* Initial goals */

!neighbors.

/* Plans */

+!neighbors: .my_name(tajny1) <-
	+right(tajny2);
	.send(tajny2, tell, left);
	.send(tajny2, tell, number(2)).

+!neighbors: not .my_name(tajny1) <- true.


+left[source(A)] <- .print("On my left is: ", A).
+right(A) <- .print("On my right is: ", A).

+number(N): N<16 <-
	.concat("tajny", N+1, X);
	+right(X);
	.send(X, tell, left);
	.send(X, tell, number(N+1)).
	

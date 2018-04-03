// Agent node in project tree_sort.mas2j

/* Initial beliefs and rules */

/* Initial goals */


/* Plans */

+!init_node(N) <-
	+parent(math.floor(N/2));
	+child(N*2);
	+child(N*2+1).

+!init_leaf(N) <-
	+parent(math.floor(N/2));
	.random(R);
	+number(math.floor(R * 100));
	!print_number.

+!print_number: number(N) <- .print(N).

+!tell_parent: parent(P) & number(N) <-
	.send(P, tell, number(N)).

+!tell_parent <- .print("Can't tell anything").

+number[source(C)]: number(X) <-
	.print("Already have number").

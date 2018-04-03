// Agent manager in project tree_sort.mas2j

/* Initial beliefs and rules */

/* Initial goals */

!build_tree.
//!step.

/* Plans */

+!build_tree <-
	for( .range(I, 2, 7) ){
		.concat("node", I, N);
		.send(N, achieve, init_node(I));
	};
	for( .range(I, 8, 15) ){
		.concat("node", I, N);
		.send(N, achieve, init_leaf(I));
	}.

+!execute <-
	.broadcast(achieve, tell_parent).

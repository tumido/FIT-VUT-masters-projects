#pragma once
#include <sys/types.h>
#include <string>
#include <iostream>
#include <bitset>

#define BYTE 8

struct node {
	std::string label;
  u_int32_t count;
  std::bitset<BYTE> code;
	std::bitset<BYTE> mask;
	node *left;
	node *right;
};


class btree {
public:
	btree();
	btree(char letter, u_int32_t count);
	~btree();

	btree * join_tree(btree * tree);
	node * search(char letter);
	node * get_root();
	void print();
	void codify_nodes();

private:
  node * root;
	void destroy_tree(node * leaf);
	node * join_nodes(node * nodeA, node * nodeB);
	node * search(std::string label, node * leaf);
	void print(node * leaf);
	void codify_nodes(node * leaf);
};

#pragma once

#include <sys/types.h>
#include <string>
#include <iostream>
#include <bitset>

#define WORD 8
#define DWORD 16

struct node {
	std::string label;
  uint32_t count;
  std::bitset<DWORD> code;
	std::bitset<DWORD> mask;
	node *left;
	node *right;
};


class btree {
public:
	btree();
	btree(char letter, uint32_t count);
	~btree();

	btree * join_tree(btree * tree);
	node * search(char letter);
	node * search(std::bitset<DWORD> code, std::bitset<DWORD> mask);
	node * get_root();
	void print();
	void codify_nodes();
	void destroy_tree();

private:
  node * root;
	void destroy_tree(node * leaf);
	node * join_nodes(node * nodeA, node * nodeB);
	node * search(std::string label, node * leaf);
	node * search(std::bitset<DWORD> code, std::bitset<DWORD> mask, node * leaf);
	void print(node * leaf);
	void codify_nodes(node * leaf);
};

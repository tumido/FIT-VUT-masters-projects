#include "btree.h"

btree::btree() : root(NULL) {};
btree::btree(char letter, u_int32_t count) {
	std::string label; label += letter;
	root = new node{label, count, 0, 0, NULL, NULL};
}

btree::~btree() {
	destroy_tree(root);
}

node * btree::get_root() {
	return root;
}

void btree::destroy_tree(node * leaf){
	if (leaf == NULL) return;

	destroy_tree(leaf->left);
	destroy_tree(leaf->right);
	delete leaf;
}

btree * btree::join_tree(btree * tree) {
	node * other_root = tree->get_root();
	if (root->count < other_root->count)
		root = join_nodes(root, other_root);
	else if (root->count == other_root->count && root->label < other_root->label)
		root = join_nodes(root, other_root);
	else
		root = join_nodes(other_root, root);

	return this;
}

node * btree::join_nodes(node * nodeA, node * nodeB) {
	std::string label = nodeA->label + nodeB->label;
	u_int32_t count = nodeA->count + nodeB->count;
	return new node{label, count, 0, 0, nodeA, nodeB};
}

node * btree::search(std::string label, node *leaf) {
	if (leaf == NULL)
		return NULL;

	if (label == leaf->label)
		return leaf;

	if(leaf->left->label.find(label) != std::string::npos)
		return search(label, leaf->left);
	else
		return search(label, leaf->right);
}

node * btree::search(char letter) {
	std::string label; label += letter;
	return search(label, root);
}

node * btree::search(std::bitset<BYTE> code, std::bitset<BYTE> mask, node * leaf) {
	if (leaf == NULL)
		return NULL;

	if (mask.count() < leaf->mask.count())
		if ()
			return search(code, mask, leaf->left);
		else
			return search(code, mask, leaf->right);

	if (mask == leaf->mask && code == leaf->code)
		return leaf;

	return NULL;
}
node * btree::search(std::bitset<BYTE> code, std::bitset<BYTE> mask) {
	return search(code, mask, root);
}

void btree::codify_nodes() {
	root->code = 0;
	root->mask = 0;
	codify_nodes(root);
}

void btree::codify_nodes(node * leaf) {
	if (leaf->left != NULL) {
		leaf->left->code = (leaf->code << 1);
		leaf->left->mask = (leaf->mask << 1) | std::bitset<BYTE>(1);
		codify_nodes(leaf->left);
	}
	if (leaf->right != NULL) {
		leaf->right->code = (leaf->code << 1) | std::bitset<BYTE>(1);
		leaf->right->mask = (leaf->mask << 1) | std::bitset<BYTE>(1);
		codify_nodes(leaf->right);
	}
}

void btree::print() {
	print(root->left);
	std::cout << "Root \"";
	for (size_t i = 0; i < root->label.length(); i++) {
		char msg = root->label[i];
		if (msg >= 'a') printf("%c ", msg);
		else printf("%d ", msg);
	}
	std::cout << "\" (count: " << root->count;
	std::cout <<  ", mask: " << root->mask;
	std::cout << ", code: " << root->code << ")" << std::endl;
	print(root->right);
}

void btree::print(node * leaf) {
	if (leaf == NULL) return;
	print(leaf->left);
	std::cout << "Node \"";
	for (size_t i = 0; i < leaf->label.length(); i++) {
		char msg = leaf->label[i];
		if (msg >= 'a') printf("%c ", msg);
		else printf("%d ", msg);
	}
	std::cout << "\" (count: " << leaf->count;
	std::cout << ", mask: " << leaf->mask;
	std::cout << ", code: " << leaf->code << ")";

	if (leaf->left == NULL && leaf->right == NULL) std::cout << " <---";
	std::cout << std::endl;
	print(leaf->right);
}

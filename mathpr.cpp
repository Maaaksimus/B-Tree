#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>

#define t 1

using namespace std;

struct iNode
{
	iNode *parent;
	unsigned int data;
	vector<unsigned int> key;
	vector<iNode*> children;
	bool is_it_root;
};

class iTree
{
public:
	iTree();
	iNode *root;
	bool planted;

	~iTree();
	deleteNode(iNode *node);

	int addNewNode(iNode *root, unsigned int new_key, unsigned int new_data);
	int closerToLeavesAndAdd(iNode *branch_lvl, unsigned int check_key);
	int tryToAddOnLevel(iNode *level, unsigned int check_key, bool mode);
	void addToPosition(int pos, vector<unsigned int> *key, unsigned int new_key);
	iNode* kickOut(iNode *branch_lvl, unsigned int check_key);

	void printTree(iNode *node);

	void deleteKeyFromTree(unsigned int k);
	void deleteKey(iNode *node, unsigned int k);
	iNode* findNodeToDel(iNode *node, unsigned int k);
	bool leftLeave(iNode *par, int pos_of_key, int pos_as_a_child);
	bool rightLeave(iNode *par, int pos_of_key, int pos_as_a_child);
	int unitNodes(iNode *par, int pos);
};

iTree::iTree() {
	root = new iNode;
	planted = false;
}

iTree::~iTree() {
	if (root != nullptr) {
		deleteNode(root);
	}
}

iTree::deleteNode(iNode *node) {
	if (node != nullptr) {
		if (node->is_it_root == true) {
			cout << "Root keys " << node->key.size() << endl;
			cout << "Root children " << node->children.size() << endl;
		}
		for (int i = 0; i < node->children.size(); i ++) {
			cout << "diving" << endl;
			deleteNode(node->children[i]);
		}
		for (int j = 0; j < node->key.size(); j ++) {
			cout << node->key[j] << " ";
		}
		cout << endl;
		cout << "Root condition " << root->key.size() << endl;
		delete node;
		/*delete(node);
		cout << "deleted, ";
		if (node->is_it_root == true) {
			cout << "it's root, keys: ";
		} else {
			cout << "it's not root, keys: ";
		}
		cout << node->key.size() << endl;*/
	}
}

int iTree::addNewNode(iNode *root, unsigned int new_key, unsigned int new_data) {
	if (planted == false) {
		root->parent = nullptr;
		root->key.clear();
		root->children.clear();
		root->key.push_back(new_key);
		root->children.push_back(nullptr);
		root->children.push_back(nullptr);
		root->is_it_root = true;
		planted = true;
	} else {
		closerToLeavesAndAdd(root, new_key);
	}
}

int iTree::closerToLeavesAndAdd(iNode *branch_lvl, unsigned int check_key) {
	if (check_key > branch_lvl->key[branch_lvl->key.size() - 1]) {
		if (branch_lvl->children[branch_lvl->children.size() - 1] != nullptr) {
				closerToLeavesAndAdd(branch_lvl->children[branch_lvl->children.size() - 1], check_key);
			} else {
				int res_of_add;
				cout << "else" << endl;
				res_of_add = tryToAddOnLevel(branch_lvl, check_key, false);
				cout << "res " << res_of_add << endl;
				if (res_of_add == 0) {
					return 0;
				}
				if (res_of_add > 0) {
					return 1;
				} else {
					kickOut(branch_lvl, check_key);
				}
			}
	}
	for (int i = 0; i < branch_lvl->key.size(); i ++) {
		if (check_key == branch_lvl->key[i]) {return 0;}
		if (check_key < branch_lvl->key[i]) {
			if (branch_lvl->children[i] != nullptr) {
				return closerToLeavesAndAdd(branch_lvl->children[i], check_key);
			} else {
				int res_of_add;
				cout << "else" << endl;
				res_of_add = tryToAddOnLevel(branch_lvl, check_key, false);
				cout << "res " << res_of_add << endl;
				if (res_of_add == 0) {
					return 0;
				}
				if (res_of_add > 0) {
					return 1;
				} else {
					kickOut(branch_lvl, check_key);
					cout << "test mem 1 " << root->key[0] << endl;
					cout << "test mem 2 " << (root->children[0])->key[0] << endl;
					cout << "out" << endl;
					return 1;
				}
			}
		}
	}
	return 1;
}

int sgn(int ret, bool sw) {
	if (sw == false) {
		return ret + 1;
	}
	return ret*(-1) - 1;
}

int iTree::tryToAddOnLevel(iNode *level, unsigned int check_key, bool mode) {
	bool check_full = false;
	if (level->key.size() == 2*t + 1) {
		check_full = true;
	}
	cout << "bigest in keys " << level->key[level->key.size() - 1] << endl;
	cout << "new key " << check_key << endl;
	if (check_key > level->key[level->key.size() - 1]) {
		if (check_full == false) {
			level->key.push_back(check_key);
		}
		cout << "nice" << endl;
		if ((mode == false) && (check_full == false)) {
			level->children.push_back(nullptr);
		}
		return sgn(level->key.size() - 1, check_full);
	}
	if (check_key == level->key[level->key.size() - 1]) {
		return 0;
	}
	for (int i = 0; i < level->key.size(); i ++) {
		if (level->key[i] == check_key) {
			return 0;
		}
		if (check_key < level->key[i]) {
			if (check_full == false) {
				addToPosition(i, &(level->key), check_key);
			}
			if ((mode == false) && (check_full == false)) {
				level->children.push_back(nullptr);
			}
			return sgn(i, check_full);
		}
	}
	return -1;
}

void iTree::addToPosition(int pos, vector<unsigned int> *key, unsigned int new_key) {
	vector<unsigned int> v;
	for (int i = 0; i < pos; i ++) {
		v.push_back((*key)[i]);
	}
	v.push_back(new_key);
	for (int i = pos; i < (*key).size(); i ++) {
		v.push_back((*key)[i]);
	}
	(*key) = v;
}

iNode* iTree::kickOut(iNode *branch_lvl, unsigned int check_key) {
	int left_or_right = -1;
	int pos;
	if (check_key < branch_lvl->key[t]) {
		left_or_right = -1;
	} else {
		left_or_right = 1;
	}
	iNode *left = new iNode;
	iNode *right = new iNode;
	cout << "t 1" << endl;
	for (int i = 0; i < t; i ++) {
		left->key.push_back(branch_lvl->key[i]);
		left->children.push_back(branch_lvl->children[i]);
	}
	cout << "t 2" << endl;
	left->children.push_back(branch_lvl->children[t]);
	for (int i = t + 1; i < branch_lvl->children.size(); i ++) {
		if (i < branch_lvl->key.size()) {
			right->key.push_back(branch_lvl->key[i]);
		}
		right->children.push_back(branch_lvl->children[i]);
	}
	cout << "t 3" << endl;
	if (left_or_right == -1) {
		cout << "t 4" << endl;
		tryToAddOnLevel(left, check_key, 0);
		cout << "t 5" << endl;
	} else {
		cout << "t 6" << endl;
		tryToAddOnLevel(right, check_key, 0);
		cout << "t 7" << endl;
	}
	left->is_it_root = false;
	right->is_it_root = false;
	cout << "t 8" << endl;
	cout << "Left keys:" << endl;
	for (int i = 0; i < left->key.size(); i ++) {
		cout << left->key[i] << " ";
	}
	cout << endl;
	cout << "Right keys:" << endl;
	for (int i = 0; i < right->key.size(); i ++) {
		cout << right->key[i] << " ";
	}
	cout << endl;
	if (branch_lvl->is_it_root == true) {
		cout << "t 9" << endl;
		iNode *new_root = new iNode;
		new_root->key.push_back(branch_lvl->key[t]);
		new_root->children.push_back(left);
		new_root->children.push_back(right);
		left->parent = new_root;
		right->parent = new_root;
		new_root->parent = nullptr;
		new_root->is_it_root = true;
		delete branch_lvl;
		cout << "t 10" << endl;
		root = new_root;
		if (left_or_right == -1) {
				return left;
			} else {
				return right;
		}
	} else {
		cout << "t 11" << endl;
		pos = tryToAddOnLevel(branch_lvl->parent, branch_lvl->key[t], 1);
		cout << "t 12" << endl;
		if (pos > 0) {
			cout << "t 13" << endl;
			pos -= 1;
			cout << "pos " << pos << endl;
			left->parent = branch_lvl->parent;
			right->parent = branch_lvl->parent;
			vector<iNode*> v;
			for (int i = 0; i < pos; i ++) {
				v.push_back((branch_lvl->parent)->children[i]);
			}
			v.push_back(left);
			v.push_back(right);
			for (int i = pos + 1; i < (branch_lvl->parent)->children.size(); i ++) {
				v.push_back((branch_lvl->parent)->children[i]);
			}
			(branch_lvl->parent)->children = v;
			delete branch_lvl;
			if (left_or_right == -1) {
				return left;
			} else {
				return right;
			}
			cout << "t 14" << endl;
		} else {
			cout << "t 15" << endl;
			pos = pos*(-1) - 1;
			cout << "pos " << pos << endl;
			iNode *find_parent = new iNode;
			if ((branch_lvl->parent)->parent == nullptr) {
				cout << "What are you doing?" << endl;
			}
			cout << "t 16" << endl;
			find_parent = kickOut(branch_lvl->parent, branch_lvl->key[t]);
			cout << "t 17" << endl;
			left->parent = find_parent;
			right->parent = find_parent;
			cout << "t 18" << endl;
			vector<iNode*> v;
			for (int i = 0; i < pos; i ++) {
				v.push_back(find_parent->children[i]);
			}
			v.push_back(left);
			v.push_back(right);
			for (int i = pos + 1; i < branch_lvl->children.size(); i ++) {
				v.push_back(find_parent->children[i]);
			}
			cout << "t 19" << endl;
			find_parent->children = v;
			if (left_or_right == -1) {
				return left;
			} else {
				return right;
			}
			cout << "t 20" << endl;
			delete branch_lvl;
		}
	}
}

void iTree::deleteKeyFromTree(unsigned int k) {
	iNode *k_node = new iNode;
	k_node = findNodeToDel(root, k);
	deleteKey(k_node, k);
}

void iTree::deleteKey(iNode *node, unsigned int k) {
	int pos_of_key;
	for (int i = 0; i < node->key.size(); i ++) {
		if (node->key[i] == k) {pos_of_key = i;}
	}
	if (node->children[0] == nullptr) {
		if (node->key.size() > t) {
			node->key.erase(node->key.begin() + pos_of_key);
			node->children.pop_back();
		} else {
			int pos_as_a_child = -1;
			for (int i = 0; i < (node->parent)->key.size(); i ++) {
				if (k < (node->parent)->key[i]) {
					pos_as_a_child = i;
					break;
				}
			}
			bool check = false;
			int first, second;
			if (pos_as_a_child == 0) {
				check = rightLeave(node->parent, pos_of_key, pos_as_a_child);
				first = pos_as_a_child;
				second = pos_as_a_child + 1;
			} else if (pos_as_a_child == -1) {
				pos_as_a_child = (node->parent)->children.size() - 1;
				check = leftLeave(node->parent, pos_of_key, pos_as_a_child);
				first = pos_as_a_child - 1;
				second = pos_as_a_child;
			} else {
				check = leftLeave(node->parent, pos_of_key, pos_as_a_child);
				if (check != true) {
					check = rightLeave(node->parent, pos_of_key, pos_as_a_child);
				}
				first = pos_as_a_child;
				second = pos_as_a_child + 1;
			}
			if (check == false) {
				if (unitNodes(node, pos_of_key) == -1) {
					deleteKey(node, k);
				} else {
					deleteKey(node->children[pos_of_key], k);
				}
				iNode *parent = new iNode;
				parent = node->parent;
				delete parent->children[first];
				delete parent->children[second];
			}
		}
	} else {
		if (node->children[pos_of_key]->key.size() > t) {
			iNode *left_branch = new iNode;
			left_branch = node->children[pos_of_key];
			node->key[pos_of_key] = left_branch->key[left_branch->key.size() - 1];
			deleteKey(left_branch, left_branch->key[left_branch->key.size() - 1]);
		} else if (node->children[pos_of_key + 1]->key.size() > t) {
			iNode *right_branch = new iNode;
			right_branch = node->children[pos_of_key + 1];
			node->key[pos_of_key] = right_branch->key[0];
			deleteKey(right_branch, right_branch->key[0]);
		} else {
			switch (unitNodes(node, pos_of_key)) {
			case -1:
				deleteKey(node, k);
				break;
			case 0:
				for (int i = 0; i < (node->parent)->parent->children.size(); i ++) {

				}
			}
			if (unitNodes(node, pos_of_key) == -1) {
				deleteKey(node, k);
			} else {
				deleteKey(node->children[pos_of_key], k);
			}
		}
	}
}

iNode* iTree::findNodeToDel(iNode *node, unsigned int k) {
	for (int i = 0; i < node->key.size(); i ++) {
		if (k == node->key[i]) {return node;}
	}
	for (int i = 0; i < node->children.size(); i ++) {
		findNodeToDel(node->children[i], k);
	}
	return nullptr;
}

bool iTree::leftLeave(iNode *par, int pos_of_key, int pos_as_a_child) {
	if (par->children[pos_as_a_child - 1]->key.size() > t) {
		par->children[pos_as_a_child]->key.erase(par->children[pos_as_a_child]->key.begin() + pos_of_key);
		par->children[pos_as_a_child]->key.emplace(par->children[pos_as_a_child]->key.begin(), par->key[pos_as_a_child]);
		par->key[pos_as_a_child] = par->children[pos_as_a_child - 1]->key[par->children[pos_as_a_child - 1]->key.size() - 1];
		par->children[pos_as_a_child - 1]->key.pop_back();
		par->children[pos_as_a_child - 1]->children.pop_back();
		return true;
	}
	return false;
}

bool iTree::rightLeave(iNode *par, int pos_of_key, int pos_as_a_child) {
	if (par->children[pos_as_a_child + 1]->key.size() > t) {
		par->children[pos_as_a_child]->key.erase(node->key.begin() + pos_of_key);
		par->children[pos_as_a_child]->key.push_back(par->key[pos_as_a_child]);
		par->key[pos_as_a_child + 1] = par->children[pos_as_a_child + 1]->key[0];
		par->children[pos_as_a_child + 1]->key.erase(par->children[pos_as_a_child + 1]->key.begin());
		par->children[pos_as_a_child + 1]->children.pop_back();
		return true;
	}
	return false;
}

int iTree::unitNodes(iNode *par, int pos) {
	iNode *unity = new iNode;
	for (int i = 0; i < t; i ++) {
		unity->key.push_back(par->children[pos]->key[i]);
		unity->children.push_back(par->children[pos]->children[i]);
	}
	unity->children.push_back(par->children[pos]->children[t]);
	unity->key.push_back(par->key[pos]);
	for (int i = 0; i < t; i ++) {
		unity->key.push_back((par->children[pos + 1])->key[i]);
		unity->children.push_back(par->children[pos + 1]->children[i]);
	}
	unity->children.push_back(par->children[pos + 1]->children[t]);
	par->key.erase(par->key.begin() + pos);
	vector<iNode*> v;
	if ((par->is_it_root == true) && (par->key.size() == 0)) {
		unity->is_it_root = true;
		par = unity;
		return -1;
	} else if (par->key.size() == 0) {
		par->children.clear();
		par->children.push_back(unity);
		return 0;
	} else if ((par->key.size() < t) && (par->is_it_root == false)){
		for (int i = 0; i < (par->parent)->key.size(); i ++) {

		}
	} else {
		unity->is_it_root = false;
		for (int i = 0; i < par->children.size(); i ++) {
			if (i != pos) {
				v.push_back(par->children[i]);
			} else {
				v.push_back(unity);
				i ++;
			}
		}
		par->children = v;
		return 1;
	}
}

void iTree::printTree(iNode *node) {
	cout << "[";
	for (int i = 0; i < node->children.size(); i ++) {
		if (node->children[i] == nullptr) {
			if (i < node->key.size()) {
				cout << node->key[i] << " ";
			}
		} else {
			printTree(node->children[i]);
			if (i < node->key.size()) {
				cout << node->key[i] << " ";
			}
		}
	}
	cout << "]";
}

int main() {
	string com;
	iTree T;
	unsigned int key;
	do {
		cin >> com;
		if (com == "add") {
			cin >> key;
			T.addNewNode(T.root, key, 5);
			cout << "current size " << T.root->key.size() << endl;
			cout << "current children " << T.root->children.size() << endl;
		}
		if (com == "print") {
            T.printTree(T.root);
		}
		if (com == "del") {
			cin >> key;
			T.deleteKeyFromTree(key);
		}
		cout << "happy end" << endl;
	} while (com != "finish");
	/*do {
		cin >> key;
		T.addNewNode(T.root, key, 5);
		cout << "current size " << T.root->key.size() << endl;
		cout << "current children " << T.root->children.size() << endl;
	} while (key != 1);*/
	return 0;
}

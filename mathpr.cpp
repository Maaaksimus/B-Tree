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

    int deleteKeyFromTree(unsigned int k);
    void checkAfterDeleting(iNode* node, unsigned int del_k);
    void keyDelAndCheck(iNode* node, unsigned int k);
    iNode* findNodeToDel(iNode *node, unsigned int k);
    bool leftLeave(iNode *par, int pos);
    bool rightLeave(iNode *par, int pos);
    bool unitNodes(iNode* par, int pos);
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
		if (branch_lvl->children[i] != nullptr) {
			branch_lvl->children[i]->parent = left;
		}
	}
	cout << "t 2" << endl;
	left->children.push_back(branch_lvl->children[t]);
	if (branch_lvl->children[t] != nullptr) {
		branch_lvl->children[t]->parent = left;
	}
	for (int i = t + 1; i < branch_lvl->children.size(); i ++) {
		if (i < branch_lvl->key.size()) {
			right->key.push_back(branch_lvl->key[i]);
		}
		right->children.push_back(branch_lvl->children[i]);
		if (branch_lvl->children[i] != nullptr) {
			branch_lvl->children[i]->parent = right;
		}
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
			iNode* par = new iNode;
			par = branch_lvl->parent;
			left->parent = par;
			right->parent = par;
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
			//delete branch_lvl;
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

int iTree::deleteKeyFromTree(unsigned int k) {
	iNode *k_node = new iNode;
	cout << "d 1" << endl;
	k_node = findNodeToDel(root, k);
	cout << "d 2" << endl;
	if (k_node == nullptr) {
		cout << "No such key" << endl;
		return -1;
	}
	cout << "d 3" << endl;
	keyDelAndCheck(k_node, k);
	cout << "d 4" << endl;
	return 1;
}

void iTree::checkAfterDeleting(iNode* node, unsigned int del_k) {
    cout << "c -3" << endl;
    if (node->key.size() >= t) {
    	cout << "key size " << node->key.size() << endl;
    	return;
    }
    cout << "c -2" << endl;
    int pos_as_a_child = 0;
    cout << "del k " << del_k << endl;
    cout << "node key size" << node->key.size() << endl;
    cout << "node par kes size " << node->parent->key.size() << endl;
    cout << "size of node par child " << pos_as_a_child << endl;;
    for (int i = 0; i < node->parent->key.size(); i ++) {
    	cout << "Parent keys " << node->parent->key[i] << endl;
    	if (del_k > node->parent->key[i]) {
    		pos_as_a_child = i + 1;
    		cout << "pos as a child " << pos_as_a_child << endl;
    	}
    }
    cout << "c -1" << endl;
    bool check = false;
    if (pos_as_a_child == 0) {
    	cout << "c 0" << endl;
    	check = rightLeave(node->parent, pos_as_a_child);
    	cout << "c 1" << endl;
    } else if (pos_as_a_child == node->parent->key.size()) {
    	check = leftLeave(node->parent, pos_as_a_child);
    } else {
    	check = leftLeave(node->parent, pos_as_a_child);
		if (check == false) {
			check = rightLeave(node->parent, pos_as_a_child);
		}
    }
    cout << "rot check " << check << endl;
    if (check == false) {
        if (unitNodes(node->parent, pos_as_a_child) == false) {
            checkAfterDeleting(node->parent, del_k);
        } else {return;}
    }
}

void iTree::keyDelAndCheck(iNode* node, unsigned int k) {
    int pos_of_key;
    iNode* subst = new iNode;
    bool child = false;
    for (int i = 0; i < node->key.size(); i ++) {
        if (node->key[i] == k) {pos_of_key = i;}
    }
    if (node->children[0] == nullptr) {
        cout << "Have you been here?" << endl;
        child = true;
        node->key.erase(node->key.begin() + pos_of_key);
        node->children.pop_back();
    } else {
        subst = node->children[pos_of_key];
        while (subst->children[0] != nullptr) {
            subst = subst->children[subst->children.size() - 1];
        }
        node->key[pos_of_key] = subst->key[subst->key.size() - 1];
        subst->key.pop_back();
        subst->children.pop_back();
    }
    cout << "Really? " << child << endl;
    cout << node->parent->key.size() << endl;
    if (child == true) {
        checkAfterDeleting(node, k);
    } else {
        checkAfterDeleting(subst, node->key[pos_of_key]);
    }
}

iNode* iTree::findNodeToDel(iNode *node, unsigned int k) {
	if (node == nullptr) {return nullptr;}
	cout << "f 1" << endl;
	for (int i = 0; i < node->key.size(); i ++) {
		cout << "f 2 - " << node->key[i] << endl;
		if (k == node->key[i]) {
			cout << "I found! " << node->key[i] << endl;
			return node;
		}
	}
	cout << "f 3" << endl;
	for (int i = 0; i < node->children.size(); i ++) {
		cout << "f 4" << endl;
		iNode* deep_find = new iNode;
		deep_find = findNodeToDel(node->children[i], k);
		if (deep_find != nullptr) {return deep_find;}
		cout << "f 5" << endl;
	}
	return nullptr;
}

bool iTree::leftLeave(iNode *par, int pos) {
	cout << "welcome left rot" << endl;
    cout << "pos " << pos << endl;
    cout << par->children[pos - 1]->children.size() << endl;
    if (par->children[pos - 1]->key.size() > t) {

        //rotate keys
        par->children[pos]->key.emplace(par->children[pos]->key.begin(), par->key[pos - 1]);
        par->key[pos - 1] = par->children[pos - 1]->key[par->children[pos - 1]->key.size() - 1];
        par->children[pos - 1]->key.pop_back();

        //rotate children
        par->children[pos]->children.emplace(par->children[pos]->children.begin(), par->children[pos - 1]->children[par->children[pos - 1]->children.size() - 1]);
        if (par->children[pos - 1]->children[0] != nullptr) {
        	cout << "swap child left" << endl;
        	par->children[pos - 1]->children[par->children[pos - 1]->children.size() - 1]->parent = par->children[pos];
        }
        par->children[pos - 1]->children.pop_back();
        cout << "left good" << endl;
        return true;
    }
    return false;
}

bool iTree::rightLeave(iNode *par, int pos) {
    cout << "welcome right rot" << endl;
    if (par->children[pos + 1]->key.size() > t) {

        //rotate keys
        par->children[pos]->key.push_back(par->key[pos]);
        par->key[pos] = par->children[pos + 1]->key[0];
        par->children[pos + 1]->key.erase(par->children[pos + 1]->key.begin());

        cout << "rot key ok" << endl;

        //rotate children
        par->children[pos]->children.push_back(par->children[pos + 1]->children[0]);
        if (par->children[pos + 1]->children[0] != nullptr) {
        	cout << "swap child right" << endl;
        	par->children[pos + 1]->children[0]->parent = par->children[pos];
        }
        par->children[pos + 1]->children.erase(par->children[pos + 1]->children.begin());

        cout << "rot children ok" << endl;

        return true;
    }
    return false;
}

bool iTree::unitNodes(iNode* par, int pos) {
	iNode* unity = new iNode;
	unity->is_it_root = false;
	unity->parent = par;
	for (int i = 0; i < par->children[pos]->key.size(); i ++) {
		unity->key.push_back(par->children[pos]->key[i]);
		unity->children.push_back(par->children[pos]->children[i]);
		if (par->children[pos]->children[i] != nullptr) {
			par->children[pos]->children[i]->parent = unity;
		}
	}
	unity->children.push_back(par->children[pos]->children[par->children[pos]->children.size() - 1]);
	if (par->children[pos]->children[par->children[pos]->children.size() - 1] != nullptr) {
		par->children[pos]->children[par->children[pos]->children.size() - 1]->parent = unity;
	}
	unity->key.push_back(par->key[pos]);
	for (int i = 0; i < par->children[pos + 1]->key.size(); i ++) {
		unity->key.push_back(par->children[pos + 1]->key[i]);
		unity->children.push_back(par->children[pos + 1]->children[i]);
		if (par->children[pos + 1]->children[i] != nullptr) {
			par->children[pos + 1]->children[i]->parent = unity;
		}
	}
	unity->children.push_back(par->children[pos + 1]->children[par->children[pos + 1]->children.size() - 1]);
	if (par->children[pos + 1]->children[par->children[pos + 1]->children.size() - 1] != nullptr) {
		par->children[pos + 1]->children[par->children[pos + 1]->children.size() - 1]->parent = unity;
	}
	par->key.erase(par->key.begin() + pos);
	par->children.erase(par->children.begin() + pos);
	par->children.erase(par->children.begin() + pos);
	par->children.emplace(par->children.begin() + pos, unity);
	if ((par->is_it_root == true) && (par->key.size() == 0)) {
		unity->is_it_root = true;
		root = unity;
		unity->parent = nullptr;
		return true;
	}
	return false;
}

void iTree::printTree(iNode *node) {
	cout << "[";
	for (int i = 0; i < node->children.size(); i ++) {
		if (node->children[i] == nullptr) {
			if (i < node->key.size()) {
				cout << node->key[i] << " ";
				if (node->is_it_root == false) {cout << "(" << node->parent->key[0] << ") ";}
			}
		} else {
			printTree(node->children[i]);
			if (i < node->key.size()) {
				cout << node->key[i] << " ";
				if (node->is_it_root == false) {cout << "(" << node->parent->key[0] << ") ";}
			}
		}
	}
	cout << "]";
}

int main() {
	string com;
	iTree T;
	unsigned int key;
	ifstream input("f_r.txt");
	for (int i = 0; i < 15; i ++) {
		input >> key;
		T.addNewNode(T.root, key, 5);
	}
	input.close();
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

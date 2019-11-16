#include "TreeSet.h"

AVLNode* RotateLeft(AVLNode *root) {
	AVLNode *temp = NULL;
	temp = root->right;
	root->right = temp->left;
	temp->left = root;
	return temp;
}

AVLNode* RotateRight(AVLNode *root) {
	AVLNode* temp = NULL;
	temp = root->left;
	root->left = temp->right;
	temp->right = root;
	return temp;
}

AVLNode* add_LeftBal(AVLNode *root, bool &taller) {
	AVLNode *rightTree = NULL;
	AVLNode *leftTree = NULL;
	leftTree = root->left;
	if (leftTree->balance == 1)
	{
		root->balance = 0;
		leftTree->balance = 0;
		root = RotateRight(root);
		taller = false;
	}
	else
	{
		rightTree = leftTree->right;
		switch (rightTree->balance)
		{
		case 1:
			root->balance = -1;
			leftTree->balance = 0;
			break;
		case 0:
			root->balance = 0;
			leftTree->balance = 0;
			break;
		case -1:
			root->balance = 0;
			leftTree->balance = 1;
			break;
		}
		rightTree->balance = 0;
		root->left = RotateLeft(leftTree);
		root = RotateRight(root);
		taller = false;
	}
	return root;
}

AVLNode* add_RightBal(AVLNode *root, bool &taller) {
	AVLNode *rightTree = NULL;
	AVLNode *leftTree = NULL;
	rightTree = root->right;
	if (rightTree->balance == -1)
	{
		rightTree->balance = 0;
		root->balance = 0;
		root = RotateLeft(root);
		taller = false;
	}
	else
	{
		leftTree = rightTree->left;
		switch (leftTree->balance)
		{
		case -1:
			root->balance = 1;
			rightTree->balance = 0;
			break;
		case 0:
			root->balance = 0;
			rightTree->balance = 0;
			break;
		case 1:
			root->balance = 0;
			rightTree->balance = -1;
			break;
		}
		leftTree->balance = 0;
		root->right = RotateRight(rightTree);
		root = RotateLeft(root);
		taller = false;
	}
	return root;
}

AVLNode* rem_RightBal(AVLNode *root, bool &lower) {
	AVLNode *rightTree = NULL;
	AVLNode *leftTree = NULL;
	switch (root->balance)
	{
	case 1:
		root->balance = 0;
		break;
	case 0:
		root->balance = -1;
		lower = false;
		break;
	case -1:
		rightTree = root->right;
		if (rightTree->balance == 1)
		{
			leftTree = rightTree->left;
			switch (leftTree->balance)
			{
			case 1:
				root->balance = 0;
				rightTree->balance = -1;
				break;
			case 0:
				root->balance = 0;
				rightTree->balance = 0;
				break;
			case -1:
				root->balance = 1;
				rightTree->balance = 0;
				break;
			}
			leftTree->balance = 0;
			root->right = RotateRight(rightTree);
			root = RotateLeft(root);
		}
		else
		{
			switch (rightTree->balance)
			{
			case -1:
				root->balance = 0;
				rightTree->balance = 0;
				break;
			case 0:
				root->balance = -1;
				rightTree->balance = 1;
				lower = false;
				break;
			}
			root = RotateLeft(root);
		}
	}
	return root;
}

AVLNode* rem_LeftBal(AVLNode *root, bool &lower)
{
	AVLNode *rightTree = NULL;
	AVLNode *leftTree = NULL;
	switch (root->balance)
	{
	case -1:
		root->balance = 0;
		break;
	case 0:
		root->balance = 1;
		lower = false;
		break;
	case 1:
		leftTree = root->left;
		if (leftTree->balance == -1)
		{
			rightTree = leftTree->right;
			switch (rightTree->balance)
			{
			case 1:
				root->balance = -1;
				leftTree->balance = 0;
				break;
			case 0:
				root->balance = 0;
				leftTree->balance = 0;
				break;
			case -1:
				root->balance = 0;
				leftTree->balance = 1;
				break;
			}
			rightTree->balance = 0;
			root->left = RotateLeft(leftTree);
			root = RotateRight(root);
		}
		else
		{
			switch (leftTree->balance)
			{
			case 1:
				root->balance = 0;
				leftTree->balance = 0;
				break;
			case 0:
				root->balance = 1;
				leftTree->balance = -1;
				lower = false;
				break;
			}
			root = RotateRight(root);
		}
	}
	return root;
}

AVLNode* Adding(AVLNode *root, AVLNode *newNode, bool &taller, int &count) {
	if (root == NULL)
	{
		root = newNode;
		taller = true;
		count++;
		return root;
	}

	if (newNode->key == root->key)
	{
		taller = false;
		return root;
	}
	else if(newNode->key < root->key)
	{
		root->left = Adding(root->left, newNode, taller, count);
		if (taller == true)
		{
			if(root->balance == 1) 
				root = add_LeftBal(root, taller);
			else if(root->balance == 0) 
				root->balance = 1;
			else if(root->balance == -1) 
			{
				root->balance = 0; 
				taller = false; 
			}
		}
		return root;
	}
	else
	{
		root->right = Adding(root->right, newNode, taller, count);
		if (taller == true)
		{
			if (root->balance == 1) 
			{
				root->balance = 0; 
				taller = false; 
			}
			else if (root->balance == 0) 
				root->balance = -1;
			else if (root->balance == -1) 
				root = add_RightBal(root, taller);
		}
		return root;
	}
	return root;
}

AVLNode* Removing(AVLNode *root, int val, bool &lower, int &count) {
	AVLNode *toDelete = NULL;
	AVLNode *exchPtr = NULL;
	AVLNode *newRoot = NULL;

	if (root == NULL)
	{
		lower = false;
		return NULL;
	}

	if (val < root->key) 
	{
		root->left = Removing(root->left, val, lower, count);
		if (lower == true)
			root = rem_RightBal(root, lower);
	}
	else if (val > root->key)
	{
		root->right = Removing(root->right, val, lower, count);
		if (lower == true)
			root = rem_LeftBal(root, lower);
	}
	else
	{
		toDelete = root;
		if (root->right == NULL)
		{
			newRoot = root->left;
			lower = true;
			count--;
			delete toDelete; toDelete = NULL;
			return newRoot;
		}
		else
		{
			if (root->left == NULL)
			{
				newRoot = root->right;
				lower = true;
				count--;
				delete toDelete; toDelete = NULL;
				return newRoot;
			}
			else
			{
				exchPtr = root->left;
				while (exchPtr->right != NULL)
					exchPtr = exchPtr->right;
				root->key = exchPtr->key;
				root->left = Removing(root->left, exchPtr->key, lower, count);
				if (lower == true)
					root = rem_RightBal(root, lower);
			}
		}
	}
	return root;
}

void copyRestore(AVLNode *root, TreeSet* thisSet) 
{
		if (root != NULL)
		{
			copyRestore(root->left, thisSet);
			thisSet->add(root->key);
			copyRestore(root->right, thisSet);
		}
}

void subsetRestore(AVLNode *root, TreeSet *newSet, int from, int to)
{
	if (root != NULL)
	{
		subsetRestore(root->left, newSet, from, to);
		if ((root->key >= from) && (root->key < to)) newSet->add(root->key);
		subsetRestore(root->right, newSet, from, to);
	}
}


//TreeSet Implementations

TreeSet::TreeSet()
{
	root = NULL;
	count = 0;
}

TreeSet::~TreeSet()
{
	clear();
}

void TreeSet::clearRec(AVLNode* root) {
	if (root != NULL) {
		clearRec(root->left);
		clearRec(root->right);
		delete root;
	}
}

void TreeSet::clear() {
	clearRec(root);
	root = NULL;
	count = 0;
}

int TreeSet::add(int val) {
	bool taller;
	int pre_count = count;
	AVLNode *newNode = new AVLNode(val);
	root = Adding(root, newNode, taller, count);
	if (pre_count == count)
		return false;
	else 
		return true;
}

bool TreeSet::contains(int val) {
	AVLNode *temp = NULL;
	temp = root;
	while (temp != NULL)
	{
		if (val == temp->key)
			return true;
		else if (val < temp->key)
			temp = temp->left;
		else
			temp = temp->right;
	}
	return false;
}

void TreeSet::copy(const TreeSet& set) {
	clear();
	copyRestore(set.root, this);
}

int TreeSet::first() {
	if (root == NULL) {
		throw "NoSuchElementException";
	}
	AVLNode *temp = root;
	while (temp->left)
		temp = temp->left;
	return temp->key;
}

int TreeSet::last() {
	if (root == NULL) {
		throw "NoSuchElementException";
	}
	AVLNode *temp = root;
	while (temp->right)
		temp = temp->right;
	return temp->key;
}

void searchHigher(AVLNode *root, int val, int &getKey, bool &check, bool &middle)
{
	if (root == NULL)
	{
		middle = true;
	}
	else if (val < root->key)
	{
		searchHigher(root->left, val, getKey, check, middle);
		if (check == true)
		{
			getKey = root->key;
			check = false;
		}
		if (middle == true)
		{
			getKey = root->key;
			middle = false;
		}
	}
	else if (val > root->key)
		searchHigher(root->right, val, getKey, check, middle);
	else if (val == root->key)
	{
		if (root->right != NULL)
		{
			root = root->right;
			while (root->left)
				root = root->left;
			getKey = root->key;
		}
		else
			check = true;
	}
}

int TreeSet::higher(int val) {
	bool check = false;
	bool middle = false;
	int getKey = -1;
	searchHigher(root, val, getKey, check, middle);
	return getKey;
}

void searchLower(AVLNode *root, int val, int &getKey, bool &check, bool &middle)
{
	if (root == NULL)
	{
		middle = true;
	}
	else if (val < root->key)
		searchLower(root->left, val, getKey, check, middle);
	else if (val > root->key)
	{
		searchLower(root->right, val, getKey, check, middle);
		if (check == true)
		{
			getKey = root->key;
			check = false;
		}
		if (middle == true)
		{
			getKey = root->key;
			middle = false;
		}
	}
	else if (val == root->key)
	{
		if (root->left != NULL)
		{
			root = root->left;
			while (root->right)
				root = root->right;
			getKey = root->key;
		}
		else
			check = true;
	}
}

int TreeSet::lower(int val) {
	bool check = false;
	bool middle = false;
	int getKey = -1;
	searchLower(root, val, getKey, check, middle);
	return getKey;
}

int TreeSet::remove(int val) {
	bool lower;
	int pre_count = count;
	AVLNode *newRoot = NULL;
	newRoot = Removing(root, val, lower, count);
	if (pre_count != count)
	{
		root = newRoot;
		return true;
	}
	else
		return false;
}

TreeSet* TreeSet::subSet(int fromVal, int toVal) {
	TreeSet *newSet = new TreeSet();
	subsetRestore(root, newSet, fromVal, toVal);
	return newSet;
}

int TreeSet::size() {
	return count;
}
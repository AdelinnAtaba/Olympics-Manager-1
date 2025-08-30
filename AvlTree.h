#ifndef AVL_TREE
#define AVL_TREE


#include<iostream>
#include "vertice.h"

/*This header implements an AVL tree which supports those methods:
	-insert
	-delete
	-find
	-getters
	-inorder and reverse inorder traversal to store data into given arrays
	-coverting a sorted array to a tree
*/
template <class T>
class avlTree {
private:
	vertice<T>* root;
	vertice<T>* vert_with_min_val;
	vertice<T>* vert_with_max_val;
	int size;
	void insertVertice(vertice<T>* root, vertice<T>* added_vertice);
	void deleteAllVertices(vertice<T>* node);
	int  getBalanceFactor(const vertice<T>* root) const;
	vertice<T>* rotateLeft(vertice<T>* z);
	vertice<T>* rotateRight(vertice<T>* z);
	vertice<T>* vertexWithMinVal(vertice<T>* vertex) const;
	vertice<T>* deleteVertice(vertice<T>* root, const T& to_delete);
	static int getMax(int a, int b);
	vertice<T>* sortedArrayToTreeHelper(const T array[], int first, int last, int n);
	static vertice<T>* copyTree(avlTree<T>* dst, const avlTree<T>* src, vertice<T>* node);
	vertice<T>* findHelper(vertice<T>* root, const T& value);

public:
	avlTree() :
		root(nullptr),
		vert_with_min_val(nullptr),
		vert_with_max_val(nullptr),
		size(0)
	{};
	~avlTree();
	avlTree(const avlTree&) = default; // will point to the same root\tree
	avlTree& operator=(const avlTree&) = default;
	void insert(const T& value);
	vertice<T>* getRoot() const;
	T* findPtr(const T& value);
	void PrintInOrder(int num, int* curr_index, T* arr) const; // Left, Parent, Right
	void deleteVert(const T& to_delete);//giving the val is enough
	int getSize() const;
	static avlTree<T>* sortedArrayToTree(const T* array, int n, const avlTree<T>* to_cp);
	T* GetMaxPtr() const;
	static T* mergeSortedArr(const T* arr1, const T* arr2, int n1, int n2,int* len);
	T* GetMinPtr() const;
	T* GetSecondMax() const;
	T* GetSecondMin() const;
	T* GetThirdMax() const;
};


template <class T>
T* avlTree<T>::GetMaxPtr() const
{
	if (this == nullptr || root == nullptr)
		return nullptr;
	return &(vert_with_max_val->getValue());
}
template <class T>
T* avlTree<T>::GetMinPtr() const
{
	if (this == nullptr || root == nullptr)
		return nullptr;
	return &(vert_with_min_val->getValue());
}


template <class T>
T* avlTree<T>::GetSecondMin() const
{
	if (this == nullptr || root == nullptr || size < 2)
		return nullptr;
	if (vert_with_min_val->getRight())
		return &(vert_with_min_val->getRight()->getValue());
	return &(vert_with_min_val->getParent()->getValue());
}

template <class T>
T* avlTree<T>::GetSecondMax() const
{
	if (this == nullptr || root == nullptr || size < 2)
		return nullptr;
	if (vert_with_max_val->getLeft())
		return &(vert_with_max_val->getLeft()->getValue());
	return &(vert_with_max_val->getParent()->getValue());
}

template <class T>
T* avlTree<T>::GetThirdMax() const
{
	if (this == nullptr || root == nullptr || size < 3)
		return nullptr;
	vertice<T>* max = vert_with_max_val;
	vertice<T>* pLeft = max->getParent()->getLeft();
	if (max->getLeft())
		return &(max->getParent()->getValue());
	//max has no children
	else if (pLeft != nullptr)//his parent has a left child
	{
		if (pLeft->getRight())
			return &(pLeft->getRight()->getValue());
		else
			return &(pLeft->getValue());
	}
	return &(max->getParent()->getParent()->getValue());
}

template<class T>
avlTree<T>* avlTree<T>::sortedArrayToTree(const T* array, int n, const avlTree<T>* to_cp)
{
	avlTree<T>* tree = new avlTree<T>();
	if (to_cp == nullptr)
	{
		tree->root = tree->sortedArrayToTreeHelper(array, 0, n - 1, n);
	}
	else if (to_cp != nullptr)
	{
		tree->root = copyTree(tree, to_cp, to_cp->root);
	}
	tree->size = n;
	return tree;
}

template<class T>
vertice<T>* avlTree<T>::sortedArrayToTreeHelper(const T array[], int first, int last, int n)
{
	if (first > last)
		return nullptr;
	int mid = (first + last) / 2;
	vertice<T>* root(new vertice<T>(array[mid]));
	root->setLeft(this->sortedArrayToTreeHelper(array, first, mid - 1, n));
	if (root->getLeft() != nullptr) root->getLeft()->setParent(root);
	root->setRight(this->sortedArrayToTreeHelper(array, mid + 1, last, n));
	if (root->getRight() != nullptr) root->getRight()->setParent(root);
	root->setHeight(1 + getMax(root->getLeft()->getHeight(), root->getRight()->getHeight()));
	if (root->getValue() == array[0])
		this->vert_with_min_val = root;
	if (root->getValue() == array[n - 1])
		this->vert_with_max_val = root;
	return root;
}

template<class T >
int avlTree<T>::getSize() const
{
	if (this == nullptr || root == nullptr)
		return 0;
	return size;
}
template <class T>
int avlTree<T>::getMax(int a, int b)
{
	return (a >= b) ? a : b;
}

template <class T>
vertice<T>* avlTree<T>::vertexWithMinVal(vertice<T>* vertex) const
{
	vertice<T>* current = vertex;
	while (current->getLeft() != nullptr)
		current = current->getLeft();
	return current;
}

template <class T>
vertice<T>* avlTree<T>::deleteVertice(vertice<T>* root, const T& to_delete) {

	if (root == nullptr)
		return root;
	if (to_delete < root->getValue()) {
		root->setLeft(deleteVertice(root->getLeft(), to_delete));
		root->getLeft()->setParent(root);
	}
	else if (to_delete > root->getValue()) {
		root->setRight(deleteVertice(root->getRight(), to_delete));
		root->getRight()->setParent(root);
	}
	//this is the vertex to be deleted
	else {

		//case 1: v has 0 or 1 child
		if ((root->getLeft() == NULL) || (root->getRight() == NULL)) {
			vertice<T>* tmp = root->getLeft() ? root->getLeft() : root->getRight();
			if (tmp == nullptr)//no child
			{
				tmp = root;
				root = nullptr;
				delete tmp;
			}
			else //1 child
			{
				tmp->setParent(root->getParent());
				if (root == this->root)
				{
					this->root = tmp;
				}
				delete root;
				root = tmp;
			}
		}
		else //2 or more children
		{
			vertice<T>* tmp = vertexWithMinVal(root->getRight());
			T root_val = root->getValue();
			root->setValue(tmp->getValue());
			tmp->setValue(root_val);
			root->setRight(deleteVertice(root->getRight(), root_val));
		}
	}
	if (root == nullptr)
		return root;
	root->setHeight(getMax(root->getLeft()->getHeight(), root->getRight()->getHeight()) + 1);
	// AVL balancing algorithm
	int balance = getBalanceFactor(root);
	if (balance > 1) { // left tree unbalanced
		if (getBalanceFactor(root->getLeft()) < 0) // right child of left tree is the cause
			root->setLeft(rotateLeft(root->getLeft())); // double rotation required
		return rotateRight(root);
	}
	else if (balance < -1) { // right tree unbalanced
		if (getBalanceFactor(root->getRight()) > 0) // left child of right tree is the cause
			root->setRight(rotateRight(root->getRight()));
		return rotateLeft(root);
	}
	return root;
}

template <class T>
void avlTree<T>::deleteVert(const T& to_delete)
{
	if (findHelper(root, to_delete) == nullptr)
		throw Failure();

	this->root = deleteVertice(root, to_delete);
	vertice<T>* tmp = root;
	while (tmp->getRight() != nullptr)
		tmp = tmp->getRight();
	vert_with_max_val = tmp;
	vertice<T>* temp = root;
	while (temp->getLeft() != nullptr)
		temp = temp->getLeft();
	vert_with_min_val = temp;
	size--;
}

template <class T>
avlTree<T>::~avlTree() {
	if (root != nullptr) {
		deleteAllVertices(root);
	}
	root = nullptr;
	vert_with_max_val = nullptr;
	vert_with_min_val = nullptr;
}

template <class T>
void avlTree<T>::deleteAllVertices(vertice<T>* node) {
	if (node) {
		deleteAllVertices(node->getLeft());
		deleteAllVertices(node->getRight());
		delete node;
	}
}


template <class T>
void avlTree<T>::insert(const T& value) {
	if (this->findHelper(root, value) != nullptr)
	{
		throw Failure();
	}
	vertice<T>* new_node = new  vertice<T>(value);
	if (!new_node)
		throw std::bad_alloc();
	new_node->setHeight(0);
	if (this->root == nullptr)
	{
		root = new_node;
		vert_with_max_val = new_node;
		vert_with_min_val = new_node;
	}
	else
	{
		insertVertice(root, new_node);
		if (vert_with_max_val != nullptr && value > vert_with_max_val->getValue())
			vert_with_max_val = new_node;
		if (vert_with_min_val != nullptr && value < vert_with_min_val->getValue())
			vert_with_min_val = new_node;
	}
	size++;
	return;
}

template <class T>
void avlTree<T>::insertVertice(vertice<T>* root, vertice<T>* added_vertice) {

	if (added_vertice->getValue() < root->getValue())
	{
		if (root->getLeft())
			insertVertice(root->getLeft(), added_vertice);
		else {
			root->setLeft(added_vertice);
			added_vertice->setParent(root);
		}
	}
	else { //>
		if (root->getRight())
			insertVertice(root->getRight(), added_vertice);
		else {
			root->setRight(added_vertice);
			added_vertice->setParent(root);
		}
	}
	int updated_height = getMax(root->getLeft()->getHeight(), root->getRight()->getHeight()) + 1;
	root->setHeight(updated_height);
	// AVL balancing algorithm
	int balance = getBalanceFactor(root);
	if (balance > 1) { // left tree unbalanced
		if (getBalanceFactor(root->getLeft()) < 0) // right child of left tree is the cause
			rotateLeft(root->getLeft()); // double rotation required
		rotateRight(root);
	}
	else if (balance < -1) { // right tree unbalanced
		if (getBalanceFactor(root->getRight()) > 0) // left child of right tree is the cause
			rotateRight(root->getRight());
		rotateLeft(root);
	}
}


template <class T>
T* avlTree<T>::findPtr(const T& value)
{
	vertice<T>* vertice = findHelper(root, value);
	if (vertice == nullptr)
		return nullptr;
	return &(vertice->getValue());
}

template <class T>
vertice<T>* avlTree<T>::findHelper(vertice<T>* root, const T& value) {
	if (root) {
		if (root->getValue() == value)
			return root;
		else if (value < root->getValue())
			return findHelper(root->getLeft(), value);
		else
			return findHelper(root->getRight(), value);
	}
	return nullptr;
}



template <class T>
int  avlTree<T>::getBalanceFactor(const vertice<T>* root) const {
	int balance = 0;
	if (root) {
		balance = (root->getLeft())->getHeight() - (root->getRight())->getHeight();
	}
	return balance;
}

template <class T>
vertice<T>* avlTree<T>::rotateLeft(vertice<T>* z) {
	vertice<T>* y = z->getRight();
	z->setRight(y->getLeft());
	z->setHeight(getMax((z->getLeft())->getHeight(), (z->getRight())->getHeight()) + 1);
	y->setLeft(z);
	y->setHeight(getMax((y->getLeft())->getHeight(), (y->getRight())->getHeight()) + 1);
	z->getRight()->setParent(z);

	if (z->getParent() == NULL) {
		y->setParent(nullptr);
		z->setParent(y);
		this->root = y;
		return y;
	}
	else if (z->getParent()->getLeft() == z) {
		z->getParent()->setLeft(y);
	}
	else {
		z->getParent()->setRight(y);
	}
	z->getParent()->setHeight(getMax((z->getParent()->getLeft())->getHeight(), z->getParent()->getRight()->getHeight()) + 1);
	y->setParent(z->getParent());
	z->setParent(y);
	return y;
}



template <class T>
vertice<T>* avlTree<T>::rotateRight(vertice<T>* z) {

	vertice<T>* y = z->getLeft();
	z->setLeft(y->getRight());
	z->setHeight(getMax((z->getLeft())->getHeight(), (z->getRight())->getHeight()) + 1);
	y->setRight(z);
	y->setHeight(getMax((y->getLeft())->getHeight(), (y->getRight())->getHeight()) + 1);

	z->getLeft()->setParent(z);
	if (z->getParent() == NULL) {
		y->setParent(nullptr);
		z->setParent(y);
		this->root = y;
		return y;
	}
	else if (z->getParent()->getLeft() == z) {
		z->getParent()->setLeft(y);
	}
	else {
		z->getParent()->setRight(y);
	}
	z->getParent()->setHeight(getMax((z->getParent()->getLeft())->getHeight(), (z->getParent()->getRight())->getHeight()) + 1);
	y->setParent(z->getParent());
	z->setParent(y);
	return y;
}

template <class T>
vertice<T>* avlTree<T>::getRoot() const
{
	return root;
};




template <class T>
void avlTree<T>::PrintInOrder(int num, int* curr_index, T* arr) const
{
	bool left_subtree_done = false, first = true;
	vertice<T>* current = vert_with_min_val;
	while (current && *curr_index < num)
	{
		if (!left_subtree_done)
		{
			while (current->getLeft())
				current = current->getLeft();
		}
		arr[*curr_index] = current->getValue();
		(*curr_index)++;
		left_subtree_done = true;

		if (current->getRight())
		{
			left_subtree_done = false;
			current = current->getRight();
		}

		else if (current->getParent())
		{
			while (current->getParent() &&
				current == current->getParent()->getRight())
				current = current->getParent();
			if (!current->getParent())
				break;
			current = current->getParent();
		}
		else break;
	}
}



template <class T>
vertice<T>* avlTree<T>::copyTree(avlTree<T>* dst, const avlTree<T>* src, vertice<T>* node)
{
	if (node == nullptr)
		return nullptr;
	vertice<T>* temp = new vertice<T>(node->getValue());
	temp->setLeft(copyTree(dst, src, node->getLeft()));
	temp->getLeft()->setParent(temp);
	temp->setRight(copyTree(dst, src, node->getRight()));
	temp->getRight()->setParent(temp);
	temp->setHeight(1 + getMax(temp->getLeft()->getHeight(), temp->getRight()->getHeight()));
	if (temp->getValue() == src->vert_with_min_val->getValue())
		dst->vert_with_min_val = temp;
	if (temp->getValue() == src->vert_with_max_val->getValue())
		dst->vert_with_max_val = temp;
	return temp;
}

template<class T>
T* avlTree<T>::mergeSortedArr(const T* arr1, const T* arr2, int n1, int n2, int* len)
{
	if ((n1 + n2) == 0)
		return nullptr;
	T* tmp = new T[n1 + n2];
	int i = 0, j = 0, k = 0;
	while (i < n1 && j < n2)
	{
		if (arr1[i] < arr2[j])
			tmp[k++] = arr1[i++];
		else
			tmp[k++] = arr2[j++];
	}
	while (i < n1)
		tmp[k++] = arr1[i++];
	while (j < n2)
		tmp[k++] = arr2[j++];
	T* temp = new T[n1 + n2];
	int a = 0, counter = 0, n = n1 + n2;
	for (; a < n; a++)
	{
		temp[counter] = tmp[a];
		counter++;
		for (; a + 1 < n && tmp[a] == tmp[a + 1]; a++)
		{

		}
	}
	T* dst = new T[counter];
	for (int z = 0; z < counter; z++)
		dst[z] = temp[z];
	*len = counter;
	delete[] tmp;
	delete[]temp;
	return dst;
}

#endif
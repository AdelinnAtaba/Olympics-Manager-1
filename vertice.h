#ifndef VERTICE_H
#define VERTICE_H
#include<iostream>


/*This header implements a vertice of a tree, each vertice has a value stored in it, right and left son, parent and a height*/
class Failure {};



template <class T>
class vertice
{
private:
	T value;
	vertice* parent;
	vertice* right_son;
	vertice* left_son;
	int height;

public:
	vertice(const T& value) :
		value(value),
		parent(nullptr),
		right_son(nullptr),
		left_son(nullptr),
		height(0)
	{}

	~vertice() = default;

	T& getValue()
	{
		return this->value;
	}
	void setValue(const T& val)
	{
		if (this == nullptr)
		{
			return;
		}
		this->value = val;
	}
	void setLeft(vertice* left)
	{
		if (this == nullptr)
		{
			return;
		}
		this->left_son = left;
	}

	vertice* getLeft() const
	{
		if (this == nullptr)
		{
			return nullptr;
		}
		return this->left_son;
	}

	void setRight(vertice* right)
	{
		if (this == nullptr)
		{
			return;
		}
		this->right_son = right;
	}

	vertice* getRight() const
	{
		if (this == nullptr)
		{
			return nullptr;
		}
		return this->right_son;
	}

	void setParent(vertice* parent)
	{
		if (this == nullptr)
			return;
		this->parent = parent;
	}

	vertice* getParent() const
	{
		if (this == nullptr)
		{
			return nullptr;
		}
		return this->parent;
	}
	void setHeight(const int height)
	{
		if (this == nullptr)
		{
			return;
		}
		this->height = height;
		return;
	}
	int getHeight() const
	{
		if (this == nullptr)
			return -1;
		return this->height;
	}
};

#endif
#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void leftRotation(AVLNode<Key, Value>* z);
    void rightRotation(AVLNode<Key, Value>* z);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
  Value insertValue = new_item.second;

  if (this->root_ == nullptr) {
    this->root_ = new AVLNode<Key, Value>(new_item.first, insertValue, nullptr);
    this->root_->setBalance(0); 
  } 
  else {
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    bool beenInserted = false;

    while (!beenInserted) {
      if (new_item.first == current->getKey()) {
        current->setValue(insertValue);
        beenInserted = true;
      } 
      else if (new_item.first < current->getKey()) {
        if (current->getLeft()) {
          current = current->getLeft();
        } 
        else {
          current->setLeft(new AVLNode<Key, Value>(new_item.first, insertValue, current));
          current->updateBalance(-1); 
          beenInserted = true;
          current = current->getLeft();
          if (current->getParent()->getParent()) {
            if (current->getParent()->getBalance() != 0) {
              this->insertFix(current->getParent(), current);
            }
          }
        }
      } 
      else {
        if (current->getRight()) {
          current = current->getRight();
        } 
        else {
          current->setRight(new AVLNode<Key, Value>(new_item.first, insertValue, current));
          current->updateBalance(1); 
          beenInserted = true;
          current = current->getRight();

          if (current->getParent()->getParent()) {
            if (current->getParent()->getBalance() != 0) {
              this->insertFix(current->getParent(), current);
            }
          }
        }
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* node1, AVLNode<Key, Value>* node2)
{

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotation(AVLNode<Key, Value>* z)
{
  if(z == nullptr){
    return;
  }  
  AVLNode<Key, Value>* y = z->left; 
  if(y == nullptr){
    return; 
  }  
  z->left = y->right; 
  if(y->right != nullptr){
    y->right->parent = z; 
  }
  y->parent = z->parent; 
  if(z->parent == nullptr){
    this->root = y; 
  } 
  else if(z == z->parent->left){
    z->parent->left = y; 
  } 
  else{
    z->parent->right = y; 
  }
  y->right = z; 
  z->parent = y;
  /**
  bool rooted;
  if(z == this->root_){
    rooted = true;
  }
  else{
    rooted = false;
  }
  AVLNode<Key, Value>* leftChild = z->getLeft();
  leftChild->setParent(z->getParent());
  z->setLeft(leftChild->getRight());
  if(z->getLeft() != nullptr){
    z->getLeft()->setParent(z);
  }
  leftChild->setRight(z);
  if(z->getParent() != nullptr){
    if(z->getParent()->getLeft() == z){
      z->getParent()->setLeft(leftChild);
    }
    else{
      z->getParent()->setRight(leftChild);
    }
  }
  z->setParent(leftChild);
  if(rooted == true)
  {
    this->root_ = child;
  }
  */
} 

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotation(AVLNode<Key, Value>* z)
{
  if(z == nullptr){
    return; 
  }
  AVLNode<Key, Value>* y = z->right;
  if(y == nullptr){
    return; 
  }
  z->right = y->left; 
  if(y->left != nullptr){
    y->left->parent = z; 
  }
  y->parent = z->parent; 
  if(z->parent == nullptr){ 
    this->root_ = y;
  } 
  else if(z == z->parent->left){ 
    z->parent->left = y;
  } 
  else{ 
    z->parent->right = y;
  }

  y->left = z; 
  z->parent = y;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

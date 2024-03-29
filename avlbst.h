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
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, int8_t diff);
    static bool LeftChild(const AVLNode<Key, Value>* node);
};


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
  bool insertion = false;
  if(this->root_ == nullptr){
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    static_cast<AVLNode<Key, Value>*>(this->root_)->setBalance(0); 
  } 
  else{
    AVLNode<Key, Value>* start = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(insertion == false){
      if(new_item.first > start->getKey()){
        if(start->getRight() != nullptr) {
          start = start->getRight();
        } 
        else{
          start->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, start));
          start->setBalance(start->getBalance() + 1);
          start = start->getRight();
          if(start->getParent()->getParent() != nullptr){
            if(start->getParent()->getBalance() != 0){
              this->insertFix(start->getParent(), start);
            }
          }
          insertion = true;
        }
      } 
      else if(new_item.first < start->getKey()){
        if(start->getLeft() != nullptr){
          start = start->getLeft();
        } 
        else{
          start->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, start));
          start->setBalance(start->getBalance() - 1);
          start = start->getLeft();
          if(start->getParent()->getParent() != nullptr){
            if (start->getParent()->getBalance() != 0){
              this->insertFix(start->getParent(), start);
            }
          }
          insertion = true;
        }
      }
      else{
        start->setValue(new_item.second);
        insertion = true;
      } 
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
  if((p == nullptr) || (p->getParent() == nullptr)){
    return;
  }
  AVLNode<Key, Value>* g = p->getParent();
  if(LeftChild(p)){
    g->setBalance(g->getBalance() - 1);
    if(g->getBalance() == 0){
      return;
    }
    else if(g->getBalance() == -1){
      insertFix(g, p);
    }
    else if(g->getBalance() == -2){
      if(((n->getParent()->getRight() == n) && (p->getParent()->getRight() == p)) || ((n->getParent()->getLeft() == n) && (p->getParent()->getLeft() == p))){
        rightRotation(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      else{
        leftRotation(p);
        rightRotation(g);
        if(n->getBalance() == -1){
          p->setBalance(0);
          g->setBalance(1);
          n->setBalance(0);
        }
        else if(n->getBalance() == 0){
          p->setBalance(0);
          g->setBalance(0);
          n->setBalance(0);
        }
        else{
          p->setBalance(-1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
    }
  }
  else{
    g->setBalance(g->getBalance() + 1);
    if(g->getBalance() == 0){
      return;
    }
    else if(g->getBalance() == 1){
      insertFix(g, p);
    }
    else if(g->getBalance() == 2){
      if(((n->getParent()->getRight() == n) && (p->getParent()->getRight() == p)) || ((n->getParent()->getLeft() == n) && (p->getParent()->getLeft() == p))){
        leftRotation(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      else{
        rightRotation(p);
        leftRotation(g);
        if(n->getBalance() == 1){
          p->setBalance(0);
          g->setBalance(-1);
          n->setBalance(0);
        }
        else if(n->getBalance() == 0){
          p->setBalance(0);
          g->setBalance(0);
          n->setBalance(0);
        }
        else{
          p->setBalance(1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  // TODO
  AVLNode<Key, Value>* removing = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if(this->root_ == nullptr || removing == nullptr){
    return;
  }
  if(removing->getLeft() == nullptr && removing->getRight() == nullptr){
    bool rooted = false;
    if(removing == this->root_)
    {
      rooted = true;
    }
    AVLNode<Key, Value>* p = removing->getParent();
    int8_t diff = 0;
    if(p != nullptr){
      if (LeftChild(removing) == true){
        diff = 1;
        p->setLeft(nullptr);
      }
      else{
        diff = -1;
        p->setRight(nullptr);
      }
    }
    delete removing;
    if(rooted == true){
      this->root_ = nullptr;
    }
    removeFix(p, diff);
  }

  else if (removing->getLeft() != nullptr && removing->getRight() == nullptr){
    bool rooted = false;
    if(removing == this->root_){
      rooted = true;
    }
    AVLNode<Key, Value>* c = removing->getLeft();
    AVLNode<Key, Value>* p = removing->getParent();
    int8_t diff = 0;
    bool wasLeftChild = LeftChild(removing);
    nodeSwap(c, removing);
    if(p != nullptr){
      if(wasLeftChild == true){
        diff = 1;
        p->setLeft(c);
      }
      else{
        diff = -1;
        p->setRight(c);
      }
    }
    c->setParent(p);
    c->setLeft(removing->getLeft());
    c->setRight(removing->getRight());
    if (c->getLeft() != nullptr){
      c->getLeft()->setParent(p);
    }
    if(c->getRight() != nullptr){
      c->getRight()->setParent(p);
    }
    delete removing;
    if (rooted == true){
      this->root_ = c;
    }
    removeFix(p, diff);
    
  }
  else if(removing->getLeft() == nullptr && removing->getRight() != nullptr){
    bool rooted = false;
    if(removing == this->root_){
      rooted = true;
    }
    AVLNode<Key, Value>* c = removing->getRight();
    AVLNode<Key, Value>* p = removing->getParent();
    int8_t diff = 0;
    bool wasLeftChild = LeftChild(removing);
    nodeSwap(c, removing);
    if(p != nullptr){
      if(wasLeftChild){
        diff = 1;
        p->setLeft(c);
      }
      else{
        diff = -1;
        p->setRight(c);
      }
    }
    c->setParent(p);
    c->setLeft(removing->getLeft());
    c->setRight(removing->getRight());
    if (c->getLeft() != nullptr){
      c->getLeft()->setParent(p);
    }
    if(c->getRight() != nullptr){
      c->getRight()->setParent(p);
    }
    delete removing;
    if(rooted == true){
      this->root_ = c;
    }
    removeFix(p, diff);
  }

  else{
    AVLNode<Key, Value>* predecess = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(removing));
    nodeSwap(predecess, removing);
    if(removing->getLeft() == nullptr && removing->getRight() == nullptr){
      bool rooted = false;
      if(removing == this->root_){
        rooted = true;
      }
      AVLNode<Key, Value>* p = removing->getParent();
      int8_t diff = 0;
      if(p != nullptr){
        if (LeftChild(removing) == true){
          diff = 1;
          p->setLeft(nullptr);
        }
        else{
          diff = -1;
          p->setRight(nullptr);
        }
      }
      delete removing;
      if(rooted == true){
        this->root_ = nullptr;
      }
      removeFix(p, diff);
    }

    else if (removing->getLeft() && !removing->getRight()){
      bool rooted = false;
      if(removing == this->root_){
        rooted = true;
      }
      AVLNode<Key, Value>* c = removing->getLeft();
      AVLNode<Key, Value>* p = removing->getParent();
      int8_t diff = 0;
      bool wasLeftChild = LeftChild(removing);
      nodeSwap(c, removing);
      if(p != nullptr){
        if(wasLeftChild == true){
          diff = 1;
          p->setLeft(c);
        }
        else
        {
          diff = -1;
          p->setRight(c);
        }
      }
      c->setParent(p);
      c->setLeft(removing->getLeft());
      c->setRight(removing->getRight());
      if(c->getLeft() != nullptr){
        c->getLeft()->setParent(p);
      }
      if (c->getRight() != nullptr) {
        c->getRight()->setParent(p);
      }
      delete removing;
      if(rooted == true){
        this->root_ = c;
      }
      removeFix(p, diff);
    }

    else {
      bool rooted = false;
      if(removing == this->root_){
        rooted = true;
      }
      AVLNode<Key, Value>* c = removing->getRight();
      AVLNode<Key, Value>* p = removing->getParent();
      int8_t diff = 0;
      bool wasLeftChild = LeftChild(removing);
      nodeSwap(c, removing);
      if(p != nullptr){
        if (wasLeftChild == true){
          diff = 1;
          p->setLeft(c);
        }
        else{
          diff = -1;
          p->setRight(c);
        }
      }
      c->setParent(p);
      c->setLeft(removing->getLeft());
      c->setRight(removing->getRight());
      if (c->getLeft() != nullptr){
        c->getLeft()->setParent(p);
      }
      if (c->getRight() != nullptr){
        c->getRight()->setParent(p);
      }
      delete removing;
      if(rooted == true)
      {
        this->root_ = c;
      }
      removeFix(p, diff);
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
  if (n == nullptr){
    return;
  }
  int8_t ndiff = 0;
  if (LeftChild(n) == true){
    ndiff = 1;
  }
  else{
    ndiff = -1;
  }
  AVLNode<Key, Value>* p = n->getParent();

  if(diff == -1){
    if(n->getBalance() + diff == -2){
      AVLNode<Key, Value>* c = n->getLeft();
      if (c->getBalance() == -1){
        rightRotation(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if (c->getBalance() == 0){
        rightRotation(n);
        n->setBalance(-1);
        c->setBalance(1);
      }
      else{
        AVLNode<Key, Value>* g = c->getRight();
        leftRotation(c);
        rightRotation(n);
        if(g->getBalance() == 1){
          g->setBalance(0);
          n->setBalance(0);
          c->setBalance(-1);
        }
        else if(g->getBalance() == 0){
          g->setBalance(0);
          n->setBalance(0);
          c->setBalance(0);
        }
        else{
          g->setBalance(0);
          n->setBalance(1);
          c->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if(n->getBalance() + diff == -1){
      n->setBalance(-1);
    }
    else if (n->getBalance() + diff == 0){
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }

  else {
    if (n->getBalance() + diff == 1){
      n->setBalance(1);
    }
    else if(n->getBalance() + diff == 0){
      n->setBalance(0);
      removeFix(p, ndiff);
    }
    else{
      AVLNode<Key, Value>* c = n->getRight();
      if (c->getBalance() == 1){
        leftRotation(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(c->getBalance() == 0){
        leftRotation(n);
        n->setBalance(1);
        c->setBalance(-1);
      }
      else{
        AVLNode<Key, Value>* g = c->getLeft();
        rightRotation(c);
        leftRotation(n);
        if (g->getBalance() == -1){
          g->setBalance(0);
          n->setBalance(0);
          c->setBalance(1);
        }
        else if(g->getBalance() == 0){
          g->setBalance(0);
          n->setBalance(0);
          c->setBalance(0);
        }
        else{
          g->setBalance(0);
          n->setBalance(-1);
          c->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
  }
}


template<class Key, class Value>
bool AVLTree<Key, Value>::LeftChild(const AVLNode<Key, Value>* node)
{
  if(node == nullptr){
    return false;
  }
  else if (node->getParent() == nullptr) {
    return false;
  } 
  else if (node->getParent()->getLeft() == node){
    return true;
  }
  else{
    return false;
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rightRotation(AVLNode<Key, Value>* z)
{
  if(z == nullptr){
    return;
  }  
  AVLNode<Key, Value>* y = z->getLeft(); 
  y->setParent(z->getParent());
  z->setLeft(y->getRight());
  
  if(y == nullptr){
    return; 
  }  
  z->setLeft(y->getRight()); 
  if(y->getRight() != nullptr){
    y->getRight()->setParent(z); 
  }
  y->setParent(z->getParent()); 
  if(z->getParent() == nullptr){
    this->root_ = y; 
  } 
  else if(z == z->getParent()->getLeft()){
    z->getParent()->setLeft(y); 
  } 
  else{
    z->getParent()->setRight(y); 
  }
  y->setRight(z); 
  z->setParent(y);
} 

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotation(AVLNode<Key, Value>* z)
{
  if(z == nullptr){
    return; 
  }
  AVLNode<Key, Value>* y = z->getRight();
  if(y == nullptr){
    return; 
  }
  z->setRight(y->getLeft()); 
  if(y->getLeft() != nullptr){
    y->getLeft()->setParent(z); 
  }
  y->setParent(z->getParent()); 
  if(z->getParent() == nullptr){ 
    this->root_ = y;
  } 
  else if(z == z->getParent()->getLeft()){ 
    z->getParent()->setLeft(y);
  } 
  else{ 
    z->getParent()->setRight(y);
  }

  y->setLeft(z); 
  z->setParent(y);
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif
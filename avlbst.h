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
    virtual void insert (const std::pair<const Key, Value> &new_item) override; // TODO
    virtual void remove(const Key& key) override;  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight( AVLNode<Key,Value>* node);
    void rotateLeft( AVLNode<Key,Value>* node);
    void insertFix( AVLNode<Key,Value>* parent, AVLNode<Key, Value>* child);
    void removeFix( AVLNode<Key,Value>* parent, int8_t diff);


};


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(!this->root_){
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr); 
        return; 
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while(current != nullptr){
        if(new_item.first == current->getKey()){
            current->setValue(new_item.second); 
            return; 
        }
        parent = current;
        if(new_item.first < current->getKey()){
            current = current->getLeft();
        }
        else current = current->getRight();
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent); 
    if(new_item.first < parent->getKey()){
        parent->setLeft(newNode);
    }
    else{
        parent->setRight(newNode); 
    }
    
    if(parent->getBalance() == 0) {
      parent->setBalance((newNode == parent->getLeft()) ? -1 : 1);
      insertFix(parent, newNode); 
    }
    else{
      parent->setBalance(0); 
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
  if (parent == nullptr || parent->getParent() == nullptr){
    return; 
  } 

  AVLNode<Key, Value>* grandparent = parent->getParent();
  if(!grandparent) return; 

  if(parent == grandparent->getLeft()){
    grandparent->updateBalance(-1); 
    
    if(grandparent->getBalance() == 0) {
      return; 
    }

    else if(grandparent->getBalance() == -1) {
      insertFix(grandparent, parent);
    }

    else if(grandparent->getBalance() == -2){
      if (child == parent->getLeft()){
        rotateRight(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0); 
      }
      else {
        rotateLeft(parent);
        rotateRight(grandparent); 
        int8_t b = child->getBalance(); 
        if (b == -1) {parent->setBalance(0); grandparent->setBalance(1);}
        else if(b == 0) {parent->setBalance(0); grandparent->setBalance(0);}
        else {parent->setBalance(-1); grandparent->setBalance(0);}
        child->setBalance(0);
      }
    }
  }
  else{
    grandparent->updateBalance(1); 
    if(grandparent->getBalance() == 0){
      return; 
    }
    else if(grandparent->getBalance() == 1){
      insertFix(grandparent, parent); 
    }
    else if(grandparent->getBalance() == 2){
      if (child == parent->getRight()){
        rotateLeft(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0); 
      }
      else {
        rotateRight(parent);
        rotateLeft(grandparent); 
        int8_t b = child->getBalance(); 
        if (b == 1) {parent->setBalance(0); grandparent->setBalance(-1);}
        else if(b == 0) {parent->setBalance(0); grandparent->setBalance(0);}
        else {parent->setBalance(1); grandparent->setBalance(0);}
        child->setBalance(0);
      }
    }
  }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO    
    int8_t diff = 0; 
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(node == nullptr) return; 

    if(node->getLeft() && node->getRight()){
      AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*> (this->predecessor(node));
      nodeSwap(node, pred); 
    }

    AVLNode<Key, Value>* parent = node->getParent();
    if(node->getLeft()){
      if(parent){
        if(parent->getLeft() == node){
           parent->setLeft(node->getLeft());
           diff = 1; 
        }
        else{
           parent->setRight(node->getLeft()); 
           diff = -1; 
        }
      }
      else this->root_ = node->getLeft();
      node->getLeft()->setParent(parent);
      diff = (parent && parent->getLeft() == node) ? 1 : -1; 
    }
    else if(node->getRight()){
      if(parent){
        if(parent->getLeft() == node){
           parent->setLeft(node->getRight());
           diff = 1; 
        }
        else{
           parent->setRight(node->getRight());
           diff = -1; 
        }
      }
      else this->root_ = node->getRight();
      node->getRight()->setParent(parent);
      diff = (parent && parent->getLeft() == node) ? 1 : -1;
    }
    else{
      if(parent){
      if(parent->getLeft() == node) {
         parent->setLeft(nullptr); 
         diff = 1; 
         }
      else { 
        parent->setRight(nullptr); 
        diff = -1; } 
      }
      else this->root_ = nullptr;
    }
  delete node; 

  if(parent){
    removeFix(parent, diff); 
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* parent, int8_t diff)
{

  if(parent == nullptr){
    return; 
  }

  if(parent->getBalance() + diff == -2){
    if(parent->getLeft() && parent->getLeft()->getBalance() == -1){
      parent->setBalance(0);
      parent->getLeft()->setBalance(0);
      rotateRight(parent);
      if(parent->getParent()){
      int8_t nextDiff = (parent->getParent()->getLeft() == parent) ? 1 : -1;
      removeFix(parent->getParent(), nextDiff); 
    }
    return; 
    }
    else if(parent->getLeft() && parent->getLeft()->getBalance() == 0){
      parent->setBalance(-1);
      parent->getLeft()->setBalance(1);
      rotateRight(parent);
    }
    else if(parent->getLeft() && parent->getLeft()->getBalance() == 1){
      if(parent->getLeft() && parent->getLeft()->getRight() && parent->getLeft()->getRight()->getBalance() == 1){
        parent->setBalance(0);
        parent->getLeft()->setBalance(-1);
        parent->getLeft()->getRight()->setBalance(0);
      }
      else if(parent->getLeft()->getRight()->getBalance() == 0){
        parent->setBalance(0);
        parent->getLeft()->setBalance(0);
        parent->getLeft()->getRight()->setBalance(0);
      }
      else if(parent->getLeft()->getRight()->getBalance() == -1){
        parent->setBalance(1);
        parent->getLeft()->setBalance(0);
        parent->getLeft()->getRight()->setBalance(0);
      }
      else {
        parent->setBalance(0);
        parent->getLeft()->setBalance(-1);
        parent->getLeft()->getRight()->setBalance(0);
      }
    rotateLeft(parent->getLeft());
    rotateRight(parent);
    if(parent->getParent()){
      int8_t nextDiff = (parent->getParent()->getLeft() == parent) ? 1 : -1;
      removeFix(parent->getParent(), nextDiff); 
    }
    return; 
    }
  }



    if(parent->getBalance() + diff == 2){
    if(parent->getRight() && parent->getRight()->getBalance() == 1){
      parent->setBalance(0);
      parent->getRight()->setBalance(0);
      rotateLeft(parent);
      if(parent->getParent()){
        int8_t nextDiff = (parent->getParent()->getLeft() == parent) ? 1 : -1;
        removeFix(parent->getParent(), nextDiff); 
    }
    return; 

    }
    else if(parent->getRight() && parent->getRight()->getBalance() == 0){
      parent->setBalance(1);
      parent->getRight()->setBalance(-1);
      rotateLeft(parent);
    }
    else if(parent->getRight() && parent->getRight()->getBalance() == -1){
      if(parent->getRight()->getLeft() && parent->getRight()->getLeft()->getBalance() == -1){
        parent->setBalance(0);
        parent->getRight()->setBalance(1);
        parent->getRight()->getLeft()->setBalance(0);
      }
      else if(parent->getRight()->getLeft()->getBalance() == 0){
        parent->setBalance(0);
        parent->getRight()->setBalance(0);
        parent->getRight()->getLeft()->setBalance(0);
      }
      else if(parent->getRight()->getLeft()->getBalance() == 1){
        parent->setBalance(-1);
        parent->getRight()->setBalance(0);
        parent->getRight()->getLeft()->setBalance(0);
      }
    rotateRight(parent->getRight());
    rotateLeft(parent);
    if(parent->getParent()){
      int8_t nextDiff = (parent->getParent()->getLeft() == parent) ? 1 : -1;
      removeFix(parent->getParent(), nextDiff); 
    }
    return; 
 
    }
  }

  else if (parent->getBalance() + diff == 1){
    parent->setBalance(1); 
  }
  else if (parent->getBalance() + diff == -1){
    parent->setBalance(-1); 
  }
  else if (parent->getBalance() + diff == 0){
    parent->setBalance(0); 
  if(parent->getParent()){
    int8_t nextDiff = (parent->getParent()->getLeft() == parent) ? 1 : -1;
    removeFix(parent->getParent(), nextDiff); 
  }
  return; 
  }
}

    


template<class Key, class Value>
void AVLTree<Key, Value>:: rotateLeft(AVLNode<Key, Value>* node)
{
  AVLNode<Key, Value>* right = node->getRight(); 
  AVLNode<Key, Value>* parent = node->getParent(); 
  node->setRight(right->getLeft());
  if(right->getLeft()) right->getLeft()->setParent(node);
  right->setLeft(node);
  node->setParent(right); 

  if(!parent){ 
    this->root_ = right; 
    right->setParent(nullptr); 
  }
  else if(parent->getLeft() == node){
    parent->setLeft(right);
    right->setParent(parent); 
  }
  else{ 
    parent->setRight(right); 
    right->setParent(parent); 
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key, Value>* node)
{
  AVLNode<Key, Value>* left = node->getLeft(); 
  AVLNode<Key, Value>* parent = node->getParent(); 
 
  node->setLeft(left->getRight());
  if(left->getRight()) left->getRight()->setParent(node);
  left->setRight(node);
  node->setParent(left);

  if(!parent){
    this->root_ = left; 
    left->setParent(nullptr);
  }
  else if(parent->getLeft() == node){
    parent->setLeft(left);
    left->setParent(parent);
  }
  else{ 
    parent->setRight(left); 
    left->setParent(parent);
  }
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

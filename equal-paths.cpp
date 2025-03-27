#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream> 
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool checkEqualPath(Node* root, int depth, int &leafDepth){
  if(root == nullptr) return true; 
 
  if(root->left == nullptr && root->right == nullptr){
    if(leafDepth == -1){
      leafDepth = depth; 
    }
    return depth == leafDepth; 
  }
    bool leftOk = checkEqualPath(root->left, depth + 1, leafDepth); 
    bool rightOk = checkEqualPath(root->right, depth + 1, leafDepth); 

    return leftOk && rightOk; 
}

bool equalPaths(Node * root)
{
    // Add your code below
  int leafDepth = -1; 
  return checkEqualPath(root, 0, leafDepth); 
}


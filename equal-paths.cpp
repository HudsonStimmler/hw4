#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int helper(Node* root);

//helper function returns -1 if the paths to leafs are not equal
int helper(Node* root){
  //Executes if root has no children
  //No children means path length is 0
  if((root->left == nullptr) && (root->right == nullptr)){
    //Returns path length 0
    return 0; 
  }

  //Executes if root has only right child
  else if(root->left == nullptr){
    //Get path length for subtree rooted at right child
    int c = helper(root->right); 

    //Executes if the right child has unequal pathlengths
    //If right child does, so does parent so return -1
    if(c == -1){
      return -1;
    }

    //Executes if right child has equal path lengths
    //Return the path length rooted at right child + 1 because parent adds 1 length
    else{
      return 1 + c;
    } 
  }

  //Executes if root has only left child
  else if(root->right == nullptr){
    //Get path length for subtree rooted at left child
    int f = helper(root->left);
    
    //Executes if the left child has unequal pathlengths
    //If left child does, so does parent so return -1
    if(f == -1){
      return -1;
    }

    //Executes if left child has equal path lengths
    //Return the path length rooted at left child + 1 because parent adds 1 length
    else{
      return 1 + f;
    } 
  }

  //Executes if root has 2 children
  else{
    //Get path length rooted at left child
    int lLength = helper(root->left); 
    //Get path length rooted at right child 
    int rLength = helper(root->right);
    
    //Executes if lengths rooted at left child and right child are equal
    //Return the path length rooted at child + 1 because parent adds 1 length
    if (lLength == rLength){
      return 1 + rLength; 
    }

    //Executes if lengths rooted at left child and right child are not equal
    //Returns returns -1 because the paths to leafs are not equal
    else{
      return -1; 
    }
  }
}


bool equalPaths(Node * root)
{
  //If no root, there are no leafs
  //No leafs mean no paths so return true
  if (root == nullptr){
    return true; 
  }

  //If there is a root call helper function
  return (helper(root) > -1);
}


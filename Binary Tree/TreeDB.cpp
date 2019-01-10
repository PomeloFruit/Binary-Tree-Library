#include "TreeDB.h"
#include "TreeNode.h"
#include "DBentry.h"
#include <iostream>

// the default constructor, creates an empty database.
TreeDB :: TreeDB(){
    probesCount = 0;                                                                            //initialize the tree to null and probesCount 0 
    root = NULL;  
}

// the destructor, deletes all the entries in the database.
TreeDB :: ~TreeDB(){
    if(root == NULL) return;                                                                    //destructor calls on the empty function 
    
    empty(root);
}


bool TreeDB :: insert(DBentry* newEntry){                                                       //the insert function
    DBentry* found; 
    
    if(root == NULL){                                                                           //if the root is null, insert the node at the head 
        root = new TreeNode(newEntry); 
        return true; 
    } else {
        found = place(root, newEntry);                                                          //otherwise use the place function 
    }
    
    if(found == NULL){                                                                          //if place returns nothing, it means the entry already exists and insert is false 
        return false; 
    } else {
        return true;                                                                            //return true otherwise
    }
} 


DBentry* TreeDB :: find(string name){                                                           //the find function 
    probesCount = 0;                                                                            //reset probesCount 0 at the beginning
    
    TreeNode* found = search(root, name, probesCount);                                          //use the search function to find the key 
    
    if(found == NULL){                                                                          //if search found something return it, if it doesn't find anything return null
        return NULL;
    }else{
        
        return found->getEntry(); 
    }
}


bool TreeDB :: remove(string name){                                                             //the remove function - this function handles edge cases involving the deletion of the
    TreeNode* temp;                                                                             //root the general cases are handled by erase 
    TreeNode* max; 
    DBentry* copy; 

    if (root == NULL){                                                                         //if there is nothing in the list return false
        return false; 
    } else if(find(name) == NULL){                                                             //if item doesn't exist in the list, return false 
        return false;
    } else if (isKey(root, name) && isLeaf(root)){                                             //1st deletion case: root is the only node 
        delete root;
        root = NULL;                                                                           //delete root, set the pointer to null and return true; 
        return true;
    } else if (isKey(root, name)){                                                             //2nd deletion case: root has only 1 child 
        if(hasLNode(root)){                                                                    //has a left child 
            temp = root;                                                                       //set root as the left child and temp as root 
            root = root->getLeft();                                                            //delete temp 
            temp->setLeft(NULL); 
            delete temp;
            return true;
        } else if (hasRNode(root)){                                                            //root has a right child - same as above
            temp = root;
            root = root->getRight(); 
            temp->setRight(NULL);
            delete temp;  
            return true; 
        } else if (isKey(root, name)){                                                          //3rd deletion case: root has two children 
            max = maxNode(root->getLeft());                                                     //find the max value of the left subtree
            copy = new DBentry(max->getEntry()->getName(), max->getEntry()->getIPaddress(), max->getEntry()->getActive()); 
            max = new TreeNode(copy);                                                           //make a copy of max and erase max from the left subtree using erase 
            
            erase(root, max->getEntry()->getName()); 
            temp = max; 

            if(root->getLeft() == NULL){                                                        //if the left subtree was only 1 node big, set it to null to resolve dangling pointer
                temp->setLeft(NULL);
            }else{
                temp->setLeft(root->getLeft());                                                 //else, set the left of max to the left of root
            }

            if(root->getRight() == NULL){                                                       //same as above 
                temp->setRight(NULL); 
            }else{
                temp->setRight(root->getRight());                                               //set the right of max to the right of root
            }

            root->setLeft(NULL);                                                                //remove root from the tree and delete it
            root->setRight(NULL);
            delete root; 
            root = temp;                                                                        //make root point to temp 
            return true;
        }   
    } else {
        if(erase(root, name)){                                                                  //if root is not the only being deleted then use erase 
            return true; 
        } else {
            return false;                                                                       
        }
    }
 
}


void TreeDB :: clear(){                                                                     //clears the tree
    if(root == NULL) return; 
    empty(root); 
}


void TreeDB :: printProbes() const{                                                         //prints the current probe count
    cout << probesCount << endl; 
}


void TreeDB :: countActive () const{                                                        //only counts if there are things in the list to avoid seg faults
    if(this->root != NULL){
        cout << count(this->root) << endl; 
    }
}


ostream& operator<< (ostream& out, const TreeDB& rhs){                                      
    if(rhs.root != NULL){                                                                   //only prints if there are things in the list, to avoid seg faults 
        rhs.printTree(out, rhs.root); 
    }
} 


TreeNode* TreeDB :: search (TreeNode* dbNode, string name, int& probesCount){               //helper function for the find function  

    if(dbNode == NULL){                                                                     //if there are no nodes, return null
        return NULL; 
    } else if (dbNode->getEntry() == NULL){                                                 
        return NULL; 
    } else if (dbNode->getEntry()->getName().compare(name) == 0){                           //traverse the tree, adding 1 to probe count every time a new node 
        probesCount++;                                                                      //is accessed and then return the node only when it is found 
        return dbNode;                                                                      //otherwise return null 
    } else if (dbNode->getEntry()->getName().compare(name) < 0){
        probesCount++; 
        return search(dbNode->getRight(), name, probesCount); 
    } else {
        probesCount++;
        return search(dbNode->getLeft(), name, probesCount);
    }

}


DBentry* TreeDB :: place (TreeNode* dbNode, DBentry* newEntry){                             //helper function for the insert function 
    TreeNode* insert; 
    
    if(dbNode == NULL){                                                                     //if the given node is empty return null
        return NULL; 
    } else if (dbNode->getEntry() == NULL){
        return NULL; 
    }
    
    if(dbNode->getEntry()->getName().compare(newEntry->getName()) == 0){                    //if the entry already exists, return null
        
        return NULL; 
    } else if (dbNode->getEntry()->getName().compare(newEntry->getName()) < 0){             //traverse the tree to find the correct place to put the new entry
        if(dbNode->getRight() == NULL){                                                     
            insert = new TreeNode(newEntry);
            dbNode->setRight(insert);                                                       //insert the new entry using the insert pointer and then return the a pointer to new 
            return newEntry;                                                                //the new entry
        } else{
            return place(dbNode->getRight(), newEntry);
        }
    } else {
        if(dbNode->getLeft() == NULL){
            insert = new TreeNode(newEntry); 
            dbNode->setLeft(insert);
            return newEntry; 
        } else{
            return place(dbNode->getLeft(), newEntry); 
        }
    }
    
}


bool TreeDB :: erase (TreeNode* dbNode, string name){                                   //the helper function for the remove function
    TreeNode* key;                                                                      //this function assumes that there are at least 2 nodes in the BST
    TreeNode* prev;                                                                     //the remove function deals in the case where there is 1 node or the root node
                                                                                        //is the node you want to delete
    TreeNode* max;
    DBentry* copy; 
    
    if(isLeaf(dbNode)){                                                                 //edge case if the current node is a leaf node and you want to delete it 
        remove(dbNode->getEntry()->getName()); 
        return true; 
    }
    
    if(dbNode->getEntry()->getName().compare(name) < 0){                                // set key to the 2nd node and previous node to the first node 
        prev = dbNode; 
        key = dbNode->getRight(); 
    } else{
        prev = dbNode; 
        key = dbNode->getLeft(); 
    }
    
    while(key->getLeft() != NULL || key->getRight() != NULL){                           //find the node containing the key 
        if(key->getEntry()->getName().compare(name) == 0){
            break; 
        }
        else if(key->getEntry()->getName().compare(name) < 0){
            prev = key; 
            key = key->getRight(); 
        } else{
            prev = key;  
            key = key->getLeft(); 
        }
    }
    
    if(isLeaf(key) && isKey(key, name)){                                                            //first deletion case: if the key to be deleted is a leaf node 
        delete key;                                                                                 //delete the key, set previous left to null, and return true; 
        prev->setLeft(NULL); 
        return true; 
    } else if (isKey(key, name) && (key->getLeft() == NULL || key->getRight() == NULL)){            //second deletion case: if the key to be deleted has only one child 
        if(prev->getEntry()->getName() < key->getEntry()->getName()){                               //for the case where the key is on the right of previous
            if(hasLNode(key)){                                                                      //key has a left node 
                prev->setRight(key->getLeft());  
                key->setLeft(NULL);   
                delete key; 
                return true;                                                                        //delete the node and set the right of previous to the left node of key 
            } else if (hasRNode(key)){                                                              //key has a right node
                prev->setRight(key->getRight());
                key->setRight(NULL); 
                delete key; 
                return true;                                                                        //delete the node and set the right of previous to the right node of key 
            }
        } else if(prev->getEntry()->getName() > key->getEntry()->getName()){                        //for the case where the key is on the left of previous
            if(hasLNode(key)){                                                                      //key has a left node
                prev->setLeft(key->getLeft());  
                key->setLeft(NULL);   
                delete key;                                                                         //same as before
                return true; 
            } else if (hasRNode(key)){                                                              //key has a right node
                prev->setLeft(key->getRight());                 
                key->setRight(NULL);                                                                //same as before
                delete key; 
                return true; 
            }
        } 
    } else if (isKey(key, name)){                                                                   //third deletion case: if the key to be deleted has two children 
        if(prev->getEntry()->getName() < key->getEntry()->getName()){                               //if the key is on the right of previous
            max = maxNode(key->getLeft());                                                          //find the max of the left subtree of key 
            copy = new DBentry(max->getEntry()->getName(), max->getEntry()->getIPaddress(), max->getEntry()->getActive()); 
            max = new TreeNode(copy);                                                               //create a copy of the max value and then reuse this erase method to 
                                                                                                    //delete the max node from the left subtree 
            erase(key->getLeft(), max->getEntry()->getName()); 
            prev->setRight(max);                                                                    //set the right of previous to max and set max left and right to key left and 
            max->setLeft(key->getLeft());                                                           //key right 
            max->setRight(key->getRight()); 

            key->setLeft(NULL);                                                                     //set both sides of key to null and delete key 
            key->setRight(NULL);                                                                        
            delete key; 
            return true; 
        } else if (prev->getEntry()->getName() > key->getEntry()->getName()){                       //if the key is on the left of previous
            max = maxNode(key->getLeft());                                                          //same as above 
            copy = new DBentry(max->getEntry()->getName(), max->getEntry()->getIPaddress(), max->getEntry()->getActive()); 
            max = new TreeNode(copy); 
            
            erase(key->getLeft(), max->getEntry()->getName()); 
            prev->setLeft(max); 
            max->setLeft(key->getLeft()); 
            max->setRight(key->getRight()); 

            key->setLeft(NULL);
            key->setRight(NULL);
            delete key; 
            return true;
        }    
    } else {
        return false;                                                                               //if none of these three cases are met, return false; 
    }
}


bool TreeDB :: isLeaf (TreeNode* dbNode){
    if(dbNode->getLeft() == NULL && dbNode->getRight() == NULL){                // sees if the current node is a leaf node 
        return true;
    }else{
        return false; 
    }
}


bool TreeDB :: isKey (TreeNode* dbNode, string key){
    if(dbNode->getEntry()->getName().compare(key) == 0){                        //sees if the current node contains the key 
        return true;
    }
    return false; 
}


bool TreeDB :: hasLNode (TreeNode* dbNode){
    if (dbNode->getLeft() != NULL && dbNode->getRight() == NULL){               //sees if the current node has a left child only
        return true;
    } else {
        return false; 
    }
}


bool TreeDB :: hasRNode (TreeNode* dbNode){
    if (dbNode->getLeft() == NULL && dbNode->getRight() != NULL){               //sees if the current node has a right child only
        return true;
    } else {
        return false; 
    }
}


TreeNode* TreeDB :: maxNode (TreeNode* dbNode){
    if(dbNode->getRight() == NULL){                                             //traverse to the right most node of the tree to get max value of the BST
        return dbNode; 
    } 
    return maxNode(dbNode->getRight()); 
}


void TreeDB :: printTree (ostream& out, const TreeNode* dbNode) const{          //helper function for the "<<" operator overload
    if(dbNode == NULL) return; 
                                                                                
    printTree(out, dbNode->getLeft());                                          //traverse through the left tree 
    out << *(dbNode->getEntry());                                               //print out the values of the left nodes
    printTree(out, dbNode->getRight());                                         //then print out the values of the right nodes 
}


int TreeDB :: count (TreeNode* dbNode) const{   
    int counter; 
    
    if (dbNode == NULL){                                            //if the current node is null, return a value of 0
        return 0;
    }
    
    if(dbNode->getEntry()->getActive() == true){                    //if the current node is active add 1 to the count
        counter = 1; 
    }else{
        counter = 0; 
    }

    return counter + count(dbNode->getLeft()) + count(dbNode->getRight());  //return the value of the current node as well as the value of the left and right subtrees 
}


void TreeDB :: empty (TreeNode* dbNode){
    if(dbNode == NULL){                         //if the current node is null, return 
        return; 
    }
    
    empty(dbNode->getLeft());                   //recurse through all the nodes in the tree
    empty(dbNode->getRight());                  // and delete each node
    
    delete dbNode; 
}



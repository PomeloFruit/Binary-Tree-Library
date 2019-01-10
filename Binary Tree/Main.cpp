/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: dujia3
 *
 * Created on November 19, 2018, 3:12 PM
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include "DBentry.h"
#include "TreeDB.h"


using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    TreeDB* tree = new TreeDB();                                                //creating the tree/database
    DBentry* newEntry; 
    DBentry* entryFound; 
    
    string line;
    string command;

    cout << "> ";       
    getline(cin, line);  

    while (!cin.eof()) {                                                        //while not end of file, continue receiveing inputs 
        string name, state;
        unsigned int address; 
        bool result; 
        
        stringstream lineStream (line);
        lineStream >> command;                                                  //read the line of user input 
        
        if(command == "insert"){                                                //if the command is insert, take in the name, address and state 
            lineStream >> name; 
            lineStream >> address; 
            lineStream >> state; 
            
            if(state == "active"){                                              //if the state is active, create a new entry with status = true 
                newEntry = new DBentry(name, address, true); 
                result = tree->insert(newEntry); 
            }else{
                newEntry = new DBentry(name, address, false);                   //if the state is inactive, create a new entry with status = false
                result = tree->insert(newEntry); 
            }
            
            if(result){                                                         //insert was able to insert the entry, return success, otherwise print error 
                cout << "Success" << endl; 
            }else{
                delete newEntry;
                cout << "Error: entry already exists" << endl; 
            }
            
        }else if(command == "find" ){                                           //if the command is find, take in the name 
            lineStream >> name; 
            
            if(tree->find(name) == NULL){                                       //if the name is not found, print error
                cout << "Error: entry does not exist" << endl;                  
            }else{
                cout << *(tree->find(name));                                    //otherwise print out the found item 
            }
            
        }else if (command == "remove"){                                         //if the command is remove, take in the name 
            lineStream >> name; 
            
            result = tree->remove(name);                                        
            if(result){                                                         //if remove successfully removes the item, print success otherwise print an error
                cout << "Success" << endl; 
            }else{
                cout << "Error: entry does not exist" << endl; 
            }
            
        }else if (command == "printall"){                                       //if the command is printall, print the entire tree 
            cout << *tree; 

        }else if (command == "printprobes"){                                    //if the command is printprobes, take in the name 
            lineStream >> name;                                                 
            
            entryFound = tree->find(name);                                      //print the probe count if the name can be found 
            if(entryFound != NULL){
                tree->printProbes(); 
            }else{
                cout << "Error: entry does not exist" << endl; 
            }
            
        }else if (command == "removeall"){                                      //if the command is remove all
            delete tree;                                                        //delete the tree 
            tree = new TreeDB();                                                //make a new tree and output success 
            cout << "Success" << endl; 
        }else if (command == "countactive"){                                    //if command is countactive, output the currently active nodes 
            tree->countActive(); 
        }else if (command == "updatestatus"){                                   //if the command is updatestatus, take in name and state
            lineStream >> name; 
            lineStream >> state; 
            
            entryFound = tree->find(name);  
            if(entryFound != NULL){
                if(state == "active"){                                          //find the correct db entry and then change its status to true if active an false if inactive 
                    entryFound->setActive(true); 
                }else{
                    entryFound->setActive(false); 
                }
                cout << "Success" << endl; 
            }else{
                cout <<"Error: entry does not exist" << endl; 
            }
        }
        
        lineStream.clear();                                                                 
        lineStream.ignore(10000, '\n');
        
        cout << "> ";          
        getline(cin, line);   

    } 
    
    delete tree;                                                                //delete the tree and set the pointer to null once it is end of file 
    tree = NULL; 
    
    return 0;
}


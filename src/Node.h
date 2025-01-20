//
// Created by Tahira on 14.01.25.
//

#ifndef NODE_H
#define NODE_H
using namespace std;

struct Node{
    Node *prev=nullptr;
    string songTitle;
    string filePath;
    Node *next=nullptr;
};

#endif //NODE_H

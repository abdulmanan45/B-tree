#pragma once
#include<iostream>
#include<string>
#include<vector>
#include <typeinfo>

using namespace std;

template<typename T>
struct key_struct {
    T key;
    size_t file_num;
    key_struct() = default;
    key_struct(T key_val, size_t num) {
        key = key_val;
        file_num = num;
    }
    key_struct<T> insert(T key_val, size_t num) {
        key = key_val;
        file_num = num;

        return*this;
    }
};


template<typename T>
struct node
{
    key_struct<T>* keys;
    int degree;
    int n;    // number of keys in the node
    node<T>** child;
    bool leaf;  // is the node a leaf node?

    node(int deg, bool l=false) // initialize a node with a degree of tree
    {
        this->degree = deg;
        leaf = l;
        keys = new key_struct<T>[degree];
        child = new node<T>* [degree]{nullptr};
        n = 0;
    }
};



template<typename T>
class btree
{
public:
    node<T>* root;
    int m;                                  //degree of tree
    btree(int deg) {                        //initialise empty tree
        root = NULL;
        m = deg;
    }
    size_t search(T data, node<T>* root);
    size_t pointsearch(T data);
    node<T>* rangedsearch(T start, T end, node<T>* root, vector<size_t>& file_name_vec);
    void traverse(node<T>* root);

   
    void insert(T key, size_t num);
    node<T>* insert_func(key_struct<T> data, node<T>*& node);
    node<T>* splitNode(node<T>*& temp, key_struct<T>* new_key=nullptr);
    node<T>* insert_node(key_struct<T>, node<T>*& node);
    node<T>* add_split_node(node<T>*& n, node<T>*& split_node);

    
    node<T>* remove_node(T data, node<T>*& node, T seperator, int case_for_leaf);
    node<T>* remove_func(T data, node<T>*& root, T seperator, int case_for_leaf);
    void remove(T data);

    string keyname;
    void setkey(string key) {
        keyname = key;
    }
};
template<typename T>
size_t btree<T>::search(T data, node<T>* root)
{
    int no_of_keys = root->n;
    int i = 0;

    //search for appropriate subtree by checking if value of data is greater tahn the kay and incrementing if so
    while (i<no_of_keys && data>root->keys[i].key)
        i++;

    //we have traversed the node and found the subtree


    if (data == root->keys[i].key && i < no_of_keys)               //key is found
        return root->keys[i].file_num;
    else if (root->leaf)                                       //if the node is a leaf we terminate because there are no more children to traverse
        return 0;
    else                                                       //search in the child
        search(data, root->child[i]);

}
template<typename T>
size_t btree<T>::pointsearch(T data)
{
    return search(data, root);

}

template<typename T>
node<T>* btree<T>::rangedsearch(T start, T end, node<T>* root,vector<size_t>& file_name_vec)
{
    int i;
    for (i = 0; i < root->n; i++) {
        //first traverse first child if node is not leaf
        if (!root->leaf)
        {
            traverse(root->child[i]);
        }
        //print
        if (root->keys->key >= start && root->keys->key <= end)
            file_name_vec.push_back(root->keys->file_num);
    }
    //traverse children
    if (!root->leaf)
        traverse(root->child[i]);

}

//inorder traversal
template<typename T>
void btree<T>::traverse(node<T>* root)
{
    int i;
    for (i = 0; i < root->n; i++) {
        //first traverse first child if node is not leaf
        if (!root->leaf)
        {
            traverse(root->child[i]);
        }
        //print
        cout << root->keys[i].key << " "<<root->keys[i].file_num<<"  ";
    }
    //traverse children
    if(!root->leaf)
    traverse(root->child[i]);
}

//bubble sort
template<typename T>
inline void sort(key_struct<T>* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (arr[i].key > arr[j].key)
            {
                key_struct<T> temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}
template<typename T>
 node<T>* btree<T>::splitNode(node<T>*& temp, key_struct<T>* new_key)
{
     //for indvidual splits
     if (new_key != nullptr) {

         //sort all keys including new
         key_struct<T>* new_keys;
         new_keys = new key_struct<T>[m];
         for (int i = 0; i < m; i++)
         {
             new_keys[i] = temp->keys[i];
         }
         new_keys[m - 1] = *new_key;

         sort(new_keys, m);
         //set median
         int median = (m / 2);

         ////set median for keys
         //int median_for_keys = (temp->n / 2);

         //parent node of split node
         node<T>* split_parent = new node<T>(m);

         //children nodes of split node
         node<T>* split_child1 = new node<T>(m);
         node<T>* split_child2 = new node<T>(m);

         //set both children to be non leaf if node is not leaf node
         if (temp->leaf)
         {
             split_child1->leaf = true;
             split_child2->leaf = true;
         }

         //add children for 1st split child (all children before median)
         for (int i = 0; i <= median; i++)
             split_child1->child[i] = temp->child[i];

         //add keys for 1st split child (all keys before median)
         for (int i = 0; i < median; i++)
         {
             split_child1->keys[i] = new_keys[i];
             split_child1->n++;
         }

         //add children for 2nd split child (all children after median)
         for (int i = 0, j = median + 1; j < m+1; i++, j++)
             split_child2->child[i] = temp->child[j];

         //add keys for 2nd split child (all children after median)
         for (int i = 0, j = median + 1; j < m; i++, j++)
         {
             split_child2->keys[i] = new_keys[j];
             split_child2->n++;
         }

         //set parent node
         split_parent->n = 1;
         split_parent->leaf = false;
         split_parent->child[0] = split_child1;
         split_parent->child[1] = split_child2;
         split_parent->keys[0] = new_keys[median];

         //sent parent node containing key taht is to be sent
         return split_parent;
     }
     //for succesive splits
     else {
         key_struct<T>* new_keys;
         new_keys = new key_struct<T>[m];
         for (int i = 0; i < m; i++)
         {
             new_keys[i] = temp->keys[i];
         }
         

         sort(new_keys, m );
         //set median
         int median = m / 2;

         ////set median for keys
         //int median_for_keys = (temp->n / 2);

         //parent node of split node
         node<T>* split_parent = new node<T>(m);

         //children nodes of split node
         node<T>* split_child1 = new node<T>(m);
         node<T>* split_child2 = new node<T>(m);

         //set both children to be non leaf if node is not leaf node
         if (temp->leaf)
         {
             split_child1->leaf = true;
             split_child2->leaf = true;
         }

         //add children for 1st split child (all children before median)
         for (int i = 0; i <= median; i++)
             split_child1->child[i] = temp->child[i];

         //add keys for 1st split child (all keys before median)
         for (int i = 0; i < median; i++)
         {
             split_child1->keys[i] = new_keys[i];
             split_child1->n++;
         }

         //add children for 2nd split child (all children after median)
         for (int i = 0, j = median + 1; j < m+1 ; i++, j++)
             split_child2->child[i] = temp->child[j];

         //add keys for 2nd split child (all children after median)
         for (int i = 0, j = median + 1; j < m ; i++, j++)
         {
             split_child2->keys[i] = new_keys[j];
             split_child2->n++;
         }

         //set parent node
         split_parent->n = 1;
         split_parent->leaf = false;
         split_parent->child[0] = split_child1;
         split_parent->child[1] = split_child2;
         split_parent->keys[0] = new_keys[median];
         return split_parent;
     }
}

template<typename T>
node<T>* btree<T>::insert_node(key_struct<T> data, node<T>*& node)
{
    int i = (node->n) - 1;                           //no of keys
    //split if new key overflows
    if (node->n >= (this->m - 1))
        return splitNode(node, &data);

    //else simply add in appropriate place
    else
    {
        //clear space for new key
        while (i >= 0 && data.key < node->keys[i].key) {
            node->keys[i + 1] = node->keys[i];
            i -= 1;
        }
        //add new key
        node->keys[i + 1] = data;
        node->n++;
        //indicate no overflow
        return nullptr;
    }
}

template<typename T>
node<T>* btree<T>::add_split_node(node<T>*& node1, node<T>*& split_node) {
    
    //add new key from split nodes parent
    key_struct<T>* new_keys;
    new_keys = new key_struct<T>[node1->n+1];
    for (int i = 0; i < node1->n + 1; i++)
    {
        new_keys[i] = node1->keys[i];
    }
    new_keys[node1->n] = split_node->keys[0];
    sort(new_keys, node1->n + 1);                       //sort new keys
    node1->n++;

    //find position of added key
    int pos;
    for (int i = 0; i < node1->n; i++) {
        node1->keys[i] = new_keys[i];
        if (node1->keys[i].key == split_node->keys[0].key)
            pos = i;
    }

    //add first split child
    node<T>* temp; node<T>* temp2;
    node1->child[pos] = split_node->child[0];
    pos++;
    //-------------Add second split child------------- 
    //if pos for second child is last available child position
    if (pos == (node1->degree)) {
        node1->child[pos] = split_node->child[1];
    }
    // else shift children to the right to accomodate second split child
    else {
       
        for (int i = node1->n + 1; i >= pos; i--)
            node1->child[i + 1] = node1->child[i];

        node1->child[pos] = split_node->child[1];
    }

    //indicate no overflow
    return nullptr;
}

template<typename T>
node<T>* btree<T>::insert_func(key_struct<T> data, node<T>*& root)
{
    int no_of_keys = root->n;
    int i = 0;
    node<T>* temp;
    //search for appropriate subtree by checking if value of data is greater tahn the kay and incrementing if so
    while (i<no_of_keys && data.key>root->keys[i].key)
        i++;

    //we have traversed the node
    
    if (data.key == root->keys[i].key && i < no_of_keys)           //duplicate found
        return nullptr;
    else if (root->leaf)                                              //if the node is a leaf we terminate because there are no more children to traverse so we insert there
        return insert_node(data, root);
    else                                                       //search in the appropriate child
        temp=insert_func(data, root->child[i]);

    //if child was split but node wont overflow so just add key to node 
    if (temp && root->n < (this->m - 1)) {
        add_split_node(root, temp);
        //indicate no overflow
        return nullptr;
    }
    //if child was split but node did overflow so add key to node and split and return split_node indicating overflow
    else if (temp && root->n >= (this->m - 1)) {
        
        //add split key
        add_split_node(root, temp);
       
        //split node
        temp = splitNode(root);

        //return split node
        return temp;
    }
}

template<typename T>
void btree<T>::insert(T key,size_t num)
{
    key_struct<T> x;
    x.insert(key, num);
    node<T>* temp;
    //case for empty tree
    if (root == nullptr)
    {
        root = new node<T>(m);
        root->keys[0].key = x.key; 
        root->keys[0].file_num = x.file_num;
        root->n = 1; root->leaf = true;
    }
    //case for leaf root and overflow
    else if (root->leaf && (root->n >= (this->m - 1)))
    {
        root = splitNode(root,&x);
    }
    //call insert_fun
    else
        temp=insert_func(x, root);
   
    //root overflows
    if (root->n > (this->m - 1)) {
        root = new node<T>(m);
        root->n = 0;
        add_split_node(root, temp);
    }
}



template<typename T>
inline node<T>* btree<T>::remove_node(T data, node<T>*& node, T seperator, int case_for_leaf)
{
    int min = (m / 2);
    int pos = 0;
    //finding data to be removed
    for (int i = 0; i < node->n; i++) {
        if (node->keys[i].key == data)
            pos = i;
    }

    //case where node has more than min keys (simple removal)
    if (node->leaf && node->n > min) {
        if (pos == m - 1) {
            node->keys[pos].key = 0;
        }

        else {
            for (int i = pos; i < node->n; i++)
                node->keys[i] = node->keys[i + 1];
        }
        node->n--;
        return 0;
    }
    //case where node has less than min keys but either right or left sibling has enough keys
    else if (node->leaf && node->n <= min) {
        //left child has enough keys
        if (case_for_leaf == 2) {
            
            // deleting node and shift elements towards left
            for (int i = pos; i < node->n; i++)
                node->keys[i] = node->keys[i + 1];

            //shift elements towards right to make space for seperator
            for (int i = node->n - 2; i >= 0; i--)
                node->keys[i + 1] = node->keys[i];

            //add seperator
            node->keys[0] = seperator;

            return node;
        }

        //right child has enough keys
        else if (case_for_leaf == 1) {
             // deleting node and shift elements towards left
            for (int i = pos; i < node->n; i++)
                node->keys[i] = node->keys[i + 1];

            //replace the deleted node with seperator
            node->keys[node->n - 1] = seperator;

            return node;
        }
        else {

        }

    }

}


template<typename T>
node<T>* btree<T>::remove_func(T data, node<T>*& root, T seperator, int case_for_leaf)
{
    int min = (m / 2);
    int no_of_keys = root->n;
    int i = 0;
    node<T>* temp;
    //search for appropriate subtree by checking if value of data is greater tahn the kay and incrementing if so
    while (i<no_of_keys && data>root->keys[i].key)
        i++;

    //we have traversed the node and found the subtree

    if (data == root->keys[i].key && i < no_of_keys) {          //key is found
        return remove_node(data, root, seperator, case_for_leaf);
    }
    else if (root->leaf)                                              //if the node is a leaf we terminate because there are no more children to traverse
        return nullptr;
    else {                                                       //search in the child
        //premptively send whcih sibling has enough keys
        if (root->n >= min) {
            if (i + 1 <= root->n) {
                if (root->child[i + 1]->n > min)                     //right sibling has enough keys
                    case_for_leaf = 1;
                else if (root->child[i - 1]->n > min)                //left sibling has enough keys
                    case_for_leaf = 2;
            }
        }
        temp = remove_func(data, root->child[i], root->keys[i].key, case_for_leaf);           //call remove function

        //rebalance tree if needed
        if (temp) {
            //replace the separator with the first key of right sibling (case 1)
            if (i + 1 <= root->n) {
                if (root->child[i + 1]->n > (m / 2)) {
                    root->keys[i] = root->child[i + 1]->keys[0];

                    for (int j = 0; j < root->child[i + 1]->n; j++)
                        root->child[i + 1]->keys[j] = root->child[i + 1]->keys[j + 1];
                    root->child[i + 1]->n--;
                    return nullptr;                                        //indicate no rebalancing
                }
            }

            //replace the separator with the last key of left sibling (case 2)
            if (root->child[i]->n > (m / 2)) {
                root->keys[i] = root->child[i + 1]->keys[root->child[i + 1]->n - 1];

                root->child[i + 1]->n--;
                return nullptr;                                             //indicate no rebalancing                   
            }
        }
    }
}

template<typename T>
void btree<T>::remove(T data)
{
    remove_func(data, root, 0, 0);
}





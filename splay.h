#ifndef SPLAY_H_INCLUDED
#define SPLAY_H_INCLUDED
#include <iostream>

using namespace std;
/**
 * Generic Splay Trees
 * A lightweight generic implementation of Splay Trees.
 */

template<typename ndt>
class Node
{
    public:
    Node(const ndt& data) : data_(data),left_(nullptr),right_(nullptr),parent_(nullptr){}

    template<typename dt>
    friend class splay;

    private:
    ndt data_;
    Node<ndt> * left_;
    Node<ndt> * right_;
    Node<ndt> * parent_;
};

template<typename dt>
class splay
{
    public:

    enum traversal{inorder,rinorder};
    typedef Node<dt> node;

    //default constructor
    explicit splay() : size_(0),root_(nullptr),it_type_(inorder) {}

    //copy constructor
    splay(const splay &sp)
    {
        size_ = sp.size_;
        it_type_ = sp.it_type_;
        root_ = clone(sp.root_, nullptr);
    }

    //destructor
    ~splay()
	{
		destroy_splay_tree(root_);
	}

    class Iterator
    {
        public:

        //Iterator Constructor
        explicit Iterator(splay<dt> &outer, node* it_node = nullptr) : it_outer_(outer), it_node_(it_node){}

        //Iterator Class Public Methods

        bool operator==(const Iterator& rhs) const
        {
            return it_node_==rhs.it_node_;
        }
        bool operator!=(const Iterator& rhs) const
        {
            return !(*this==rhs);
        }

        Iterator& operator++() //pre increment
        {
            if(it_outer_.it_type_ == inorder)
            {
                set_inorder_successor(); //Sets value of it_node_ to inorder successor
            }
            else if(it_outer_.it_type_ == rinorder)
            {
                set_inorder_predecessor(); //Sets value of it_node_ to inorder predecessor
            }
            return *this;
        }

        Iterator operator++(int) //post increment
        {
            Iterator temp(*this); //Using default copy constructor
            return ++(*this);
            return temp;
        }

        Iterator& operator--() //pre increment
        {
            if(it_outer_.it_type_ == inorder)
            {
                set_inorder_predecessor(); //Sets value of it_node_ to inorder successor
            }
            else if(it_outer_.it_type_ == rinorder)
            {
                set_inorder_successor(); //Sets value of it_node_ to inorder predecessor
            }
            return *this;
        }

        Iterator operator--(int) //post increment
        {
            Iterator temp(*this); //Using default copy constructor
            return --(*this);
            return temp;
        }

        #if 1
        Iterator operator+(const int& rhs){
            int n = rhs;
            Iterator temp(*this);
            while(n--)
                ++temp;
            return temp;
        }

        Iterator operator-(const int& rhs){
            int n = rhs;
            Iterator temp(*this);
            while(n--)
                --temp;
            return temp;
        }

        int operator-(const Iterator& rhs){
            int cnt = 0;
            Iterator temp(*this);
            while(temp != rhs){
                ++cnt;
                --temp;
            }
            return cnt;
        }

        void operator+=(const int& rhs){
            int n = rhs;
            while(n--)
                ++*this;
        }

        void operator-=(const int& rhs){
            int n = rhs;
            while(n--)
                --*this;
        }
        #endif

        dt operator*()
        {
            return it_node_->data_;
        }

        private:
        node* it_node_; //The current node in the iterator.
        splay<dt> &it_outer_;

        //Iterator Class Private Methods

        /**
         * Setting it_node_ as the inorder successor
         *
         * If the current node has a non-null right child,
         * - Take a step down to the right
         * - Then run down to the left as far as possible
         *
         * If the current node has a null right child,
         * - move up the tree until we have moved over a left child link
         */
        void set_inorder_successor()
        {
            if(it_node_->right_)
		    {
			    it_node_ = it_node_->right_;
			    while(it_node_->left_)
                {
                    it_node_ = it_node_->left_;
                }
            }
		    else
		    {
                //Keep running loop till current node is left child of parent
                while (it_node_->parent_ != nullptr && it_node_ != it_node_->parent_->left_)
                {
                    it_node_ = it_node_->parent_;
                }

                // Parent is inorder successor
                it_node_ = it_node_->parent_;
            }
        }

        /**
         * Setting it_node_ as the inorder predecessor
         *
         * If the current node has a lst,
         * - max value of lst is predecessor
         *
         * Else if the current node has no lst,
         * - move up the tree till you find a node which is the right child of node
         */
        void set_inorder_predecessor()
        {
            if(it_node_->left_)
            {
                //there is a lst
                //predecessor is max value (right most value) in lst
                it_node_ = it_node_->left_;
                while(it_node_->right_)
                {
                    it_node_ = it_node_->right_;
                }
            }
            else
            {
                bool found = false;

                //while you don't reach the root node, keep traversing up
                while(it_node_->parent_)
                {
                    if(it_node_->parent_->right_ == it_node_) //if node turns out to be right child, then root is predecessor
                    {
                        it_node_ = it_node_->parent_;
                        found = true;
                        break;
                    }
                    else
                    {
                        it_node_ = it_node_->parent_;
                    }
                }
                if(!found)
                {
                    it_node_ = nullptr;
                }
            }
        }
    };

    //Splay Class Public Methods

    //Default traversal is inorder
    Iterator begin()
    {
        //Inorder
        //Begins with the left most leaf node
        it_type_ = inorder;

        if(root_ == nullptr)
        {
            return Iterator(*this,nullptr);
        }

        node *temp = root_;
        while(temp->left_ != nullptr)
        {
            temp = temp->left_;
        }

        return Iterator(*this,temp);
    }

    Iterator end()
    {
        return Iterator(*this,nullptr);
    }

    Iterator rbegin()
    {
        //reverse inorder
        it_type_ = rinorder;
        if(root_ == nullptr)
        {
            return Iterator(*this,nullptr);
        }
        node *temp = root_;

        //find rightmost root
        while(temp->right_ != nullptr)
        {
            temp = temp->right_;
        }

        return Iterator(*this,temp);
    }

    Iterator rend()
    {
        return Iterator(*this,nullptr);
    }

    void insert(const dt &data)
    {
        node *temp = new node(data);
        insert_node(temp);
    }

    Iterator find(const dt &data)
    {
        return Iterator(*this,search_node(root_,data));
    }

    unsigned int size()
    {
        return size_;
    }
    


    bool empty()
    {
        return (size_ == 0);
    }

    void erase(const dt &data)
    {
        delete_node(data);
    }

    void clear()
    {
        destroy_splay_tree(root_);
        root_ = nullptr;
        size_ = 0;
    }

    dt base(){
        return root_->data_;
    }

    /**
     * Comparision of two splay trees
     * Two splay trees are said to be the same if the position of every node is exactly
     * identical in both the trees.
     */
    bool operator==(const splay<dt>& rhs)
    {
        return identical(root_,rhs.root_);
    }


    private:

    // Splay Class Variables
    node *root_; //Root element of the splay tree
    unsigned int size_; //Size of the splay tree
    traversal it_type_;

    // Splay Class Private Methods
    // Helper functions

    bool identical(node* root_a, node* root_b){
		if((root_a==nullptr)&&(root_b==nullptr))
        {
			return true;
		}
        else if((root_a!=nullptr && root_b==nullptr)||(root_a==nullptr && root_b!=nullptr)){
			return false;
		}
        else if(root_a->data_==root_b->data_){
			return identical(root_a->left_, root_b->left_) && identical(root_a->right_, root_b->right_);
		}
        else{
			return false;
		}
	}

    node* clone(node* root,node* parent)
    {
        if(root == nullptr)
        {
           return nullptr;
        }
        node* temp = new node(root->data_);
        temp->parent_ = parent;
        temp->left_ = clone(root->left_,temp);
        temp->right_ = clone(root->right_,temp);
        return temp;
    }

    /**
     * Insert Node into Splay Tree
     *
     * Inserts node based on value (follows BST property).
     * Once inserted, element is splayed to the top.
     *
     * @param n : the node to be inserted into the tree
     * @return : nothing (void)
     */
   void insert_node(node *n)
   {
        node *prev = nullptr;
        node *curr = root_;

        //Find appropriate leaf node
        while(curr != nullptr)
        {
            prev = curr;

            if(n->data_ < curr->data_)
            {
                curr = curr->left_;
            }
            else
            {
                curr = curr->right_;
            }
        }

        // Assign Parent
        n->parent_ = prev;

        if(n->parent_ == nullptr)
        {
            //n turns out to be the root
            root_ = n;
        }
        else if(n->data_ < n->parent_->data_)
        {
            // make n as left child of parent.
             n->parent_->left_ = n;
        }
        else
        {
            // make n as right child of parent.
            n->parent_->right_ = n;
        }

        //Incremenet size
        ++size_;

        // Splay element to the top
        splayify(root_,n);
    }


    /**
     * Recursively Destroy entire splay tree
     * @param root : root of tree to be destroyed
     * @return : nothing (void)
     */
    void destroy_splay_tree(node *root)
    {
        if(root == nullptr)
        {
            size_ = 0;
            return;
        }
        destroy_splay_tree(root->left_);
        destroy_splay_tree(root->right_);
        delete root;
    }

    // Splay Class - Splay Tree Specific Private Methods

    /**
     * Zig : Right rotation
     * @param y : the node about which rotation is done
     * @return : nothing (void)
     */
    void zig(node* &root, node *y)
    {
        node *x = y->left_;
        y->left_ = x->right_;

        if(x->right_ != nullptr)
        {
            x->right_->parent_ = y;
        }
        x->parent_ = y->parent_;

        if(y->parent_ == nullptr)
        {
            //y is root
            root = x;
        }
        else if(y == y->parent_->right_)
        {
            y->parent_->right_ = x;
        }
        else
        {
            y->parent_->left_ = x;
        }
        x->right_ = y;
        y->parent_ = x;
    }


    /**
     * Zag : Left rotation
     * @param x : the node about which rotation is done
     * @return : nothing (void)
     */
    void zag(node* &root, node *x)
    {
        node *y = x->right_;
        x->right_ = y->left_;
        if(y->left_ != nullptr)
        {
            y->left_->parent_ = x;
        }
        y->parent_ = x->parent_;
        if(x->parent_ == nullptr)
        {
            //x is root
            root = y;
        }
        else if(x == x->parent_->left_)
        {
            x->parent_->left_ = y;
        }
        else
        {
            x->parent_->right_ = y;
        }
        y->left_ = x;
        x->parent_ = y;
    }

    /**
     * Brings an element to the root while keeping it a BST.
     * Continue zig or zag rotations, until the node to be splayed up doesn't become the root of the tree.
     * @param root : The current root of the tree being splayed.
     * @param n: Node to be splayed to the top of the tree.
     * @return nothing (void)
     *
     */
    void splayify(node* &root, node *n) {
        while(n->parent_ != nullptr) // i.e. until node is not the root
        {
            if(n->parent_ == root)
            {
                // has only parent and no grandparent
                if(n == n->parent_->left_)
                {
                    // node is left child
                    cout << "Zig called on  " << n->parent_->data_ << "\n"; 
                    zig(root, n->parent_);
                    
                }
                else
                {
                    // node is right child
                    cout << "Zag called on  " << n->parent_->data_ << "\n"; 
                    zag(root, n->parent_);
                    
                }
            }
            else {
                node *p = n->parent_; //parent
                node *g = p->parent_; //grandparent

                if(n->parent_->left_ == n && p->parent_->left_ == p)
                {
                    // Zig-Zig i.e. Left Left Case
                    cout << "\n";
                    cout << "Zig called on  " << g->data_ << "\n";
                    zig(root,g);
                 
                    cout << "Zig called on  " << p->data_ << "\n";
                    cout << "\n";
                    zig(root,p);
                    
                    
                    
                }
                else if(n->parent_->right_ == n && p->parent_->right_ == p)
                {
                    // Zag-Zag i.e. Right Right Case
                    cout << "\n";
                    cout << "Zag called on  " << g->data_ << "\n";
                    zag(root,g);
                    cout << "Zag called on  " << p->data_ << "\n";
                    cout << "\n";
                    zag(root,p);
                    
                }
                else if(n->parent_->right_ == n && p->parent_->left_ == p)
                {
                    // Zag-Zig i.e. Right Left Case
                    cout << "\n";
                    cout << "Zag called on  " << p->data_ << "\n";
                    zag(root,p);
                    cout << "Zig called on  " << g->data_ << "\n";
                    cout << "\n";
                    zig(root,g);
                    
                }
                else if(n->parent_->left_ == n && p->parent_->right_ == p)
                {
                    // Zig-Zag i.e. Left Right Case
                    cout << "\n";
                    cout << "Zig called on  " << p->data_ << "\n";
                    zig(root,p);
                    cout << "\n";
                    cout << "Zag called on  " << g->data_ << "\n";
                    zag(root,g);
              
                }
            }
        }
    }


     /**
     * Search for a node.
     *
     * Find node where element is found.
     *
     * @param root : The root of the tree/subtree being searched.
     * @param data : The value of the element being searched for
     * @return: the node where the element is found
     */
    node* search_node(node *n, const dt &data)
    {
        if(n==nullptr)
        {
            return nullptr;
        }
        if(data == n->data_) {
            splayify(root_,n);
            return n;
        }
        else if(data < n->data_)
        {
            //Element in lst
            return search_node(n->left_, data);
        }
        else if(data > n->data_)
        {
            //Element in rst
            return search_node(n->right_, data);
        }
        return nullptr;
    }



     /**
     * Delete a node.
     *
     * The steps involved for this are as follows,
     * - Search and find the node to be deleted.
     * - The search process spalys the node to the root
     * - Hence the element to be deleted is at the root.
     * - Now, delete the root element. We will have two non connected trees (lst,rst)
     * - Search for the largest element of the left subtree and splay it to the top
     * - Attach rst as right child of this element (being the largest element in the lst, it would not already have a right child when its the root)
     *
     * @param data : value of element to be deleted.
     * @return: no return (void)
     */
    void delete_node(const dt &data)
    {
        node* root = search_node(root_,data);

        if(root==nullptr)
        {
            return;
        }

        node* lst_root = root->left_;
        if(lst_root != nullptr)
        {
            lst_root->parent_=nullptr;
        }

        node* rst_root = root->right_;
        if(rst_root != nullptr)
        {
            rst_root->parent_=nullptr;
        }

        delete root;
        --size_;

        if(lst_root != nullptr) {
            //Find max (rightmost) value of lst.
            node* maxele = lst_root;
            while(maxele->right_ != nullptr)
            {
                maxele=maxele->right_;
            }
            //Splay this element to the root of lst.
            splayify(lst_root,maxele);
            lst_root->right_ = rst_root;
            root_ = lst_root;
        }
        else
        {
            //There is no lst. Make rst root as splay tree root.
            root_ = rst_root;
        }

    }

};
#endif // SPLAY_H_INCLUDED


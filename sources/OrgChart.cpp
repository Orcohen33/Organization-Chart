#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <bits/stdc++.h> // for queue
#include "OrgChart.hpp"
using namespace std;

namespace ariel
{
    // Declarations of functions.
    void pre_order(Node *node, vector<Node*> &result);
    void level_order(Node *node, vector<Node*> &result);
    void reverse_level_order(Node *node, vector<Node*> &result);
    // ------------------------------------ START ORGCHART ------------------------------------ //
    // Constructor
    OrgChart::OrgChart(){root = nullptr;}
    // Deep copy constructor
    OrgChart::OrgChart(const OrgChart &other)
    {
        this->root = nullptr;
        if (other.root != nullptr)
        {
            this->root = new Node(other.root->name, nullptr);
            this->root->children = other.root->children;
            for (size_t i = 0; i < this->root->children.size(); i++)
            {
                this->root->children[i] = new Node(*(other.root->children[i]));
                this->root->children[i]->parent = this->root;
            }
        }
    }

    // Superficial copying, with smart pointer
    // Use 'noexcept' to avoid memory leak
    OrgChart::OrgChart(OrgChart &&other) noexcept : root(other.root) {other.root = nullptr;}

    /**
     * @brief Add a root to the Org Chart:: Org Chart object
     *
     * @param name
     */
    OrgChart &OrgChart::add_root(const string &name)
    {   
        if (name.empty())
        {
            throw invalid_argument("Name cannot be empty");
        }
        if (this->root != nullptr)
        {
            // swap old_root to new_root and make all old_root childrens to new_root
            // then make childrens parent to be new_root
            // then make new_root parent to be nullptr
            Node *new_root = new Node(name, nullptr);
            Node *old_root = this->root;
            new_root->children = old_root->children;
            for (Node *child : new_root->children)
            {
                child->parent = new_root;
            }
            this->root = new_root;
            delete old_root;
        }
        else
        {
            this->root = new Node(name, nullptr);
        }
        return *this;
    }
    /**
     * @brief Add a sub to the Org Chart:: Org Chart object
     *
     * @param parent
     * @param child
     */
    OrgChart &OrgChart::add_sub(const string &parent, const string &child)
    {
        if (parent.empty() || child.empty())
        {
            throw invalid_argument("Name cannot be empty");
        }
        Node *parent_node = find_node(this->root, parent);
        if (parent_node == nullptr)
        {
            throw std::runtime_error("Parent does not exist");
        }
        parent_node->children.push_back(new Node(child, parent_node));
        return *this;
    }

    Iterator OrgChart::begin_level_order() const {if(this->root == nullptr){throw std::runtime_error("Chart is empty") ;} return Iterator(this->root, LevelOrder); }
    Iterator OrgChart::end_level_order() const {if(this->root == nullptr) {throw std::runtime_error("Chart is empty") ;} return Iterator(nullptr); }
    Iterator OrgChart::begin_reverse_order() const {if(this->root == nullptr) {throw std::runtime_error("Chart is empty") ;} return Iterator(this->root, ReverseLevelOrder); }
    Iterator OrgChart::reverse_order() const {if(this->root == nullptr) {throw std::runtime_error("Chart is empty") ;} return Iterator(nullptr); }
    Iterator OrgChart::begin_preorder() const {if(this->root == nullptr) {throw std::runtime_error("Chart is empty") ;} return Iterator(this->root, PreOrder); }
    Iterator OrgChart::end_preorder() const {if(this->root == nullptr) {throw std::runtime_error("Chart is empty") ;} return Iterator(nullptr); }
    Iterator OrgChart::begin() const {if(this->root == nullptr) {throw std::runtime_error("Chart is empty") ;} return Iterator(this->root, LevelOrder); }
    Iterator OrgChart::end() const {if(this->root == nullptr) {throw std::runtime_error("Chart is empty") ;} return Iterator(nullptr); }
    // ------------------------------------ END ORGCHART ------------------------------------ //

    // ---------------------------------START ITERATOR --------------------------------- //
    Iterator::Iterator(Node *node, IteratorType type)
    {
        this->node = node;
        this->index = 0;
        if (node != nullptr)
        {
            switch (type)
            {
            case LevelOrder:
                level_order(this->node, this->name_list);
                break;
            case ReverseLevelOrder:
                reverse_level_order(this->node, this->name_list);
                break;
            case PreOrder:
                pre_order(this->node, this->name_list);
                break;
            default:
                cout << "Error" << endl;
                exit(0);
            }
        }
    }
    Iterator &Iterator::operator++()
    {
        index++;
        if (index == this->name_list.size())
        {
            this->node = NULL;
        }
        return *this;
    }
    Iterator &Iterator::operator--()
    {
        index--;
        if (index == 0)
        {
            this->node = this->node->parent;
        }
        return *this;
    }
    string &Iterator::operator*() { return this->name_list[index]->name; }
    string *Iterator::operator->() { return &name_list[index]->name; };
    bool Iterator::operator==(const Iterator &other) const { return this->node == other.node; };
    bool Iterator::operator!=(const Iterator &other) const { return this->node != other.node; };
    // ---------------------------------END ITERATOR --------------------------------- //

    // ---------------------------------START HELP FUNCTIONS ---------------------------------
    /**
     * @brief Find a node in the Node:: Node object
     *
     * @param name
     * @return Node*
     */
    Node *OrgChart::find_node(Node *node, const string &name)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (node->name == name)
        {
            return node;
        }
        for (Node *child : node->children)
        {
            Node *found_node = find_node(child, name);
            if (found_node != nullptr)
            {
                return found_node;
            }
        }
        return nullptr;
    }
    
    /**
     * @brief Delete all nodes in OrgChart 
     * 
     * @param node 
     */
    void OrgChart::delete_chart(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }
        for (auto *child : node->children)
        {
            delete_chart(child);
        }
        delete node;
    }
    /**
     * @brief Enter nodes to vector 'result' by bfs algorithm
     * 
     * @param node 
     * @param result 
     */
    void level_order(Node *node, vector<Node*> &result)
    {
        if (node == nullptr)
        {
            return;
        }
        queue<Node *> q;
        q.push(node);
        while (!q.empty())
        {
            int n = q.size();
            while (n > 0)
            {
                Node *p = q.front();
                q.pop();
                result.push_back(p);
                for (auto *child : p->children)
                {
                    q.push(child);
                }
                n--;
            }
        }
    }
    /**
     * @brief Same as level order , but in reverse order
     * 
     * @param node 
     * @param result 
     */
    void reverse_level_order(Node *node, vector<Node*> &result)
    {
        if (node == nullptr)
        {
            return;
        }
        queue<Node *> q;
        q.push(node);
        while (!q.empty())
        {
            int n = q.size();
            while (n > 0)
            {
                Node *p = q.front();
                q.pop();
                result.push_back(p);
                if (!p->children.empty())
                {
                    // iterate from the last child to the first
                    for (uint i = p->children.size(); i > 0; i--)
                    {
                        q.push(p->children[(uint)(i - 1)]);
                    }
                }
                n--;
            }
        }
        std::reverse(result.begin(), result.end());
    }
    /**
     * @brief Used for preorder traversal
     * 
     * @param node 
     * @param result 
     */
    void pre_order(Node *node, vector<Node*> &result)
    {
        if (node == nullptr)
        {
            return;
        }
        result.push_back(node);
        for (auto *child : node->children)
        {
            pre_order(child, result);
        }
    }

    // ---------------------------------END HELP FUNCTIONS ---------------------------------
    // ---------------------Start Operators override -------------------------
    ostream &operator<<(std::ostream &os, const OrgChart &org)
    {
        Node *node = org.root;
        if (node == nullptr)
        {
            os << "Empty OrgChart" << endl;
            return os;
        }

        queue<Node *> q;
        q.push(node);
        while (!q.empty())
        {
            int n = q.size();

            // cout << "n: " << n << endl;
            while (n > 0)
            {
                Node *p = q.front();
                q.pop();
                os << p->name << "     ";
                for (auto *child : p->children)
                {
                    q.push(child);
                }
                n--;
            }
            os << endl;
        }
        return os;
    }
    // Operator =
    OrgChart &OrgChart::operator=(const OrgChart &other)
    {
        if (this != &other)
        {
            if (this->root != nullptr)
            {
                delete_chart(this->root);
            }
            this->root = nullptr;
            if (other.root != nullptr)
            {
                this->root = new Node(other.root->name, nullptr);
                this->root->children = other.root->children;
                for (size_t i = 0; i < this->root->children.size(); i++)
                {
                    this->root->children[i] = new Node(*(other.root->children[i]));
                    this->root->children[i]->parent = this->root;
                }
            }
        }
        return *this;
    }
    // Move assignment operator
    OrgChart &OrgChart::operator=(OrgChart &&other) noexcept
    {
        if (this != &other)
        {
            if (this->root != nullptr)
            {
                delete_chart(this->root);
            }
            this->root = other.root;
            other.root = nullptr;
        }
        return *this;
    }
    // ---------------------End Operators override ---------------------------
} // namespace ariel

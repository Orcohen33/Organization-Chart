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
    void pre_order(Node *node, vector<string> &result);
    void level_order(Node *node, vector<string> &result);
    void reverse_level_order(Node *node, vector<string> &result);
    // ------------------------------------ START ORGCHART ------------------------------------ //
    /**
     * @brief Construct a new Org Chart:: Org Chart object
     *
     */
    // OrgChart::OrgChart()
    // {
    //     this->root = NULL;
    // }
    /**
     * @brief Destroy the Org Chart:: Org Chart object
     *
     */
    // OrgChart::~OrgChart()
    // {
    //     // delete root and all his children and their childrens
    // }
    /**
     * @brief Add a root to the Org Chart:: Org Chart object
     *
     * @param name
     */
    OrgChart &OrgChart::add_root(const string &name)
    {
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
        Node *parent_node = find_node(this->root, parent);
        if (parent_node == nullptr)
        {
            throw std::runtime_error("Parent does not exist");
        }
        parent_node->children.push_back(new Node(child, parent_node));
        return *this;
    }

    Node *OrgChart::get_root()
    {
        return this->root;
    }

    Iterator OrgChart::begin_level_order() const { return Iterator(this->root, LevelOrder); }
    Iterator OrgChart::end_level_order() const { return Iterator(nullptr); }
    Iterator OrgChart::begin_reverse_order() const { return Iterator(this->root, ReverseLevelOrder); }
    Iterator OrgChart::reverse_order() const { return Iterator(nullptr); }
    Iterator OrgChart::begin_preorder() const { return Iterator(this->root, PreOrder); }
    Iterator OrgChart::end_preorder() const { return Iterator(nullptr); }
    Iterator OrgChart::begin() const { return Iterator(this->root, LevelOrder); }
    Iterator OrgChart::end() const { return Iterator(nullptr); }
    // ------------------------------------ END ORGCHART ------------------------------------ //

    // ---------------------------------START ITERATORS --------------------------------- //
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
    string &Iterator::operator*() { return this->name_list[index]; }
    string *Iterator::operator->() { return &name_list[index]; };
    bool Iterator::operator==(const Iterator &other) const { return this->node == other.node; };
    bool Iterator::operator!=(const Iterator &other) const { return this->node != other.node; };
    // ---------------------------------END ITERATORS --------------------------------- //

    // ---------------------------------START HELP FUNCTIONS ---------------------------------
    /**
     * @brief Find a node in the Org Chart:: Org Chart object
     * @param name
     *
     * @return Node*
     */

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
    void OrgChart::delete_chart(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }
        for (auto child : node->children)
        {
            delete_chart(child);
        }
        delete node;
    }
    void level_order(Node *node, vector<string> &result)
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
                result.push_back(p->name);
                for (auto child : p->children)
                {
                    q.push(child);
                }
                n--;
            }
        }
    }
    void reverse_level_order(Node *node, vector<string> &result)
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
                result.push_back(p->name);
                if (p->children.size() > 0)
                {
                    // iterate from the last child to the first
                    for (int i = p->children.size() - 1; i >= 0; i--)
                    {
                        q.push(p->children[(uint)i]);
                    }
                }
                n--;
            }
        }
        std::reverse(result.begin(), result.end());
    }
    void pre_order(Node *node, vector<string> &result)
    {
        if (node == nullptr)
        {
            return;
        }
        result.push_back(node->name);
        for (auto child : node->children)
        {
            pre_order(child, result);
        }
    }

    // ---------------------------------END HELP FUNCTIONS ---------------------------------
    // // ---------------------Start Operators override -------------------------
    ostream &operator<<(std::ostream &os, const OrgChart &org)
    {
        Node *node = org.root;
        if (node == nullptr)
        {
            os << "Empty OrgChart" << endl;
            return os;
        }
        /* TODO: Do spaces by indexing*/
        // auto itr = find(this->myGame.players_vec.begin(), this->myGame.players_vec.end(), this);
        // uint index = distance(this->myGame.players_vec.begin(), itr);

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
                for (auto child : p->children)
                {
                    q.push(child);
                }
                n--;
            }
            // int temp = q.size();
            // string temp = "--------|";
            // cout << "Temp: " << temp;
            os << endl;
        }
        return os;
    }
    /*
       CEO
       |--------|--------|
       CTO      CFO      COO
       |                 |
       VP_SW             VP_BI
    */
    // // ---------------------End Operators override -------------------------
} // namespace ariel

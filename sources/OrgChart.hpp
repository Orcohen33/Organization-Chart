#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

namespace ariel
{
    /**
     * @brief Struct for the node of the OrgChart
     */
    struct Node
    {
        string name;
        Node *parent;
        std::vector<Node *> children;
        Node(const string &name, Node *parent) : name(name), parent(parent){};
    };

    enum IteratorType
    {
        LevelOrder,
        ReverseLevelOrder,
        PreOrder,
        None
    };
    /**
     * @brief Class for the Iterator of the OrgChart
     */
    class Iterator
    {
    private:
        Node *node;
        vector<Node *> name_list;
        size_t index;

    public:
        Iterator(Node *node, IteratorType type = None);
        Node *get_root() const { return node; };
        Iterator &operator++();
        Iterator &operator--();
        string &operator*();
        string *operator->();
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
    };

    /**
     * @brief Class for the OrgChart
     */
    class OrgChart
    {
    public:
        OrgChart();
        // deep copy constructor
        OrgChart(const OrgChart &other);
        // Superficial copying, with smart pointer
        OrgChart(OrgChart &&other) noexcept; // Use 'noexcept' to avoid memory leak
        ~OrgChart() { delete_chart(root); };
        OrgChart &add_root(const string &name);
        OrgChart &add_sub(const string &parent, const string &child);
        // Node *get_root();
        Iterator begin_level_order() const;
        Iterator end_level_order() const;
        Iterator begin_reverse_order() const;
        Iterator reverse_order() const;
        Iterator begin_preorder() const;
        Iterator end_preorder() const;
        Iterator begin() const;
        Iterator end() const;

        OrgChart &operator=(const OrgChart &other);
        // Move assignment operator
        OrgChart &operator=(OrgChart &&other) noexcept;

    private:
        Node *root;
        Node *find_node(Node *node, const string &name);
        static void delete_chart(Node *node);
        friend ostream &operator<<(std::ostream &os, const OrgChart &org); // override << operator
        friend ostream &operator<<(ostream &os, const Node &node);         // override << operator
    };
} // namespace ariel
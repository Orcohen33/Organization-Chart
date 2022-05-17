#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <stdexcept>

#include "doctest.h"
#include "OrgChart.hpp"

using namespace std;
using namespace ariel;

TEST_CASE("Test OrgChart")
{
    SUBCASE("add_root function")
    {
        OrgChart org;
        org.add_root("root");
        Node *root = org.get_root();
        CHECK(root->name == "root");
        CHECK(root->parent == nullptr);
        CHECK(root->children.size() == 0);

        org.add_sub("root", "child");
        CHECK(root->children.size() == 1);

        // override root and make new root
        org.add_root("override_root");
        Node *root2 = org.get_root();
        CHECK_NE(root2->name, "root");
        CHECK(root2->name == "override_root");
        CHECK(root2->parent == nullptr);
        CHECK(root2->children.size() == 1); // 'override_root' has 1 child called 'child'
    }
    SUBCASE("add_sub function")
    {
        OrgChart org;
        org.add_root("root");
        org.add_sub("root", "child");
        Node *root = org.get_root();
        CHECK(root->name == "root");
        CHECK(root->children.size() == 1);
        CHECK(root->children[0]->name == "child");
        CHECK(root->children[0]->parent == root);
        CHECK(root->children[0]->children.size() == 0);

        org.add_sub("root", "child2");
        CHECK(root->children.size() == 2);
        CHECK(root->children[1]->name == "child2");
        CHECK(root->children[1]->parent == root);
        CHECK(root->children[1]->children.size() == 0);

        org.add_sub("child", "child3");
        CHECK(root->children[0]->children.size() == 1);
        CHECK(root->children[0]->children[0]->name == "child3");
        CHECK(root->children[0]->children[0]->parent == root->children[0]);
        CHECK(root->children[0]->children[0]->children.size() == 0);
    }
    SUBCASE("Iterators")
    {
        OrgChart org;
        org.add_root("root").add_sub("root", "child").add_sub("root", "child2").add_sub("child", "child3");
        /*
            root
            |-------|
          child   child2
            |
          child3
        */
        SUBCASE("Level order")
        {
            vector<string> expected = {"root", "child", "child2", "child3"};
            std::reverse(expected.begin(), expected.end());
            for (auto it = org.begin_level_order(); it != org.end_level_order(); ++it)
            {
                CHECK(*it == expected.back());
                expected.pop_back();
            }
        }
        SUBCASE("Reverse level order")
        {
            vector<string> expected = {"root", "child2", "child", "child3"};
            for (auto it = org.begin_reverse_order(); it != org.reverse_order(); ++it)
            {
                CHECK(*it == expected.back());
                expected.pop_back();
            }
        }
        SUBCASE("Pre order")
        {
            vector<string> expected = {"root", "child", "child3", "child2"};
            std::reverse(expected.begin(), expected.end());
            for (auto it = org.begin_preorder(); it != org.end_preorder(); ++it)
            {
                CHECK(*it == expected.back());
                expected.pop_back();
            }
        }
        SUBCASE("For loop")
        {
            vector<string> expected = {"root", "child", "child2", "child3"};
            std::reverse(expected.begin(), expected.end());
            for (string element : org)
            {
                CHECK(element == expected.back());
                expected.pop_back();
            }
        }
    }
}
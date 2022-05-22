#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
using namespace std;

#include "sources/OrgChart.hpp"
using namespace ariel;

string menu()
{
    return "--------------------Organization chart menu--------------------\n"
           "Press 1 to add root\n"
           "Press 2 to add sub\n"
           "Press 3 to print tree by level order\n"
           "Press 4 to print tree by reverse level order\n"
           "Press 5 to print tree by pre order\n"
           "Press 6 to for_each function from \'algorithm\' library\n"
           "Press 7 to print tree by operator \'<<\'\n"
           "Press 8 to create new chart\n"
           "Press 9 to exit";
}

int main()
{
    OrgChart organization;
    string command, name, parent, child, root;
    stringstream ss;

    while (true)
    {
        try
        {
            cout << menu() << endl;
            cin >> command;
            if (command == "1")
            {
                system("clear");
                cout << "Enter root name: ";
                cin >> root;
                organization.add_root(root);
                cout << root << " is the currect root right now.";
            }
            else if (command == "2")
            {
                system("clear");
                cout << "Enter parent name: ";
                cin >> parent;
                cout << "Enter child name: ";
                cin >> child;
                organization.add_sub(parent, child);
            }
            else if (command == "3")
            {
                system("clear");
                for (auto it = organization.begin_level_order(); it != organization.end_level_order(); ++it)
                {
                    cout << *it << " ";
                }
                
            }
            else if (command == "4")
            {
                system("clear");
                for (auto it = organization.begin_reverse_order(); it != organization.reverse_order(); ++it)
                {
                    cout << *it << " ";
                }

            }
            else if (command == "5")
            {
                system("clear");
                for (auto it = organization.begin_preorder(); it != organization.end_preorder(); ++it)
                {
                    cout << *it << " ";
                }
                
            }
            else if (command == "6")
            {
                system("clear");
                for_each(organization.begin(), organization.end(), [](string s)
                         { cout << s << " "; });
            }
            else if (command == "7")
            {
                system("clear");
                cout << organization;
            }
            else if (command == "8")
            {
                /*
                    Delete the current chart and create a new one called organization
                */
                system("clear");
                organization = OrgChart();
                cout << "New chart is created.";
            }
            else if (command == "9")
            {
                return 0;
            }

            cout << endl;
        }
        catch (...)
        {
            cout << "Exception: Invalid command." << endl;
        }
    }
    return 0;
}
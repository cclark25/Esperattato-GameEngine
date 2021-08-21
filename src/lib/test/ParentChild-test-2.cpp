#include "../ParentChild.h"
#include <vector>
#include <iostream>

class P;
class C;

typedef ParentChild<P, C> PCPair;

class P
{
public:
    std::vector<PCPair::Child *> children;
    int parentData = 187;
    P() {}
    ~P()
    {
        for (PCPair::Child *child : this->children)
        {
            delete child;
        }
    }
};

class C
{
public:
    PCPair::Parent parent;
    C(P &parent, int num) : parent(this)
    {
        this->childData = num;
        parent.children.push_back(new PCPair::Child(&parent, this->parent));
    }
    int childData = 0;
};

int main()
{

    P *p = new P();
    C *c1 = new C(*p, 1);
    C *c2 = new C(*p, 2);
    C *c3 = new C(*p, 3);

    if (!(c1->parent == c2->parent && c2->parent == c3->parent))
    {
        std::cerr << "Children's parents do not match!" << std::endl;
        throw 0;
    }
    else
    {
        std::cout << "Children's parents match as they are supposed to." << std::endl;
    }

    if ((*p->children[0]) == (*p->children[1]) || (*p->children[1]) == (*p->children[2]))
    {
        std::cerr << "Parent's children match when they are not supposed to!" << std::endl;
        throw 0;
    }
    else
    {
        std::cout << "Parent's children do not match, as they are supposed to." << std::endl;
    }

    for (PCPair::Child *pp : p->children)
    {
        PCPair::Child &po = *pp;
        std::cout << "Child data: " << po->childData << std::endl;
        std::cout << "Child data: " << (*po).childData << std::endl;
    }

    std::cout << "Parent data: " << c1->parent->parentData << std::endl;
    std::cout << "Parent data: " << (*c1->parent).parentData << std::endl;

    delete p;

    try
    {
        std::cout << "Parent data: " << c1->parent->parentData << std::endl;
    }
    catch (PCPair::RelativeDoesNotExist &e)
    {
        std::cout << "Accessing deleted parent through \"->\" failed as expected." << std::endl;
    }
    try
    {
        std::cout << "Parent data: " << (*c1->parent).parentData << std::endl;
    }
    catch (PCPair::RelativeDoesNotExist &e)
    {
        std::cout << "Accessing deleted parent through \"*\" failed as expected." << std::endl;
    }

    return 0;
}
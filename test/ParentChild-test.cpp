#include "../ParentChild.h"
#include <vector>
#include <iostream>

class P;
class C;

typedef ParentChild<P, C> PCPair;

class P
{
public:
    PCPair::Child parent;
    int parentData = 187;
    P() : parent(this) {}
};

class C
{
public:
    PCPair::Parent child;
    C(P &parent) : child(this, parent.parent) {}
    int childData = 54;
};

int main()
{

    P *p = new P();
    C *c = new C(*p);

    std::cout << "Child data: " << p->parent->childData << std::endl;
    std::cout << "Child data: " << (*p->parent).childData << std::endl;
    std::cout << "Parent data: " << c->child->parentData << std::endl;
    std::cout << "Parent data: " << (*c->child).parentData << std::endl;
    delete c;
    try
    {
        std::cout << "Child data: " << p->parent->childData << std::endl;
    }
    catch (PCPair::RelativeDoesNotExist &e)
    {
        std::cout << "Accessing deleted child through \"->\" failed as expected." << std::endl;
    }
    try
    {
        std::cout << "Child pointer: " << (*p->parent).childData << std::endl;
    }
    catch (PCPair::RelativeDoesNotExist &e)
    {
        std::cout << "Accessing deleted child through \"*\" failed as expected." << std::endl;
    }

    c = new C(*p);

    std::cout << "Child data: " << p->parent->childData << std::endl;
    std::cout << "Child data: " << (*p->parent).childData << std::endl;
    std::cout << "Parent data: " << c->child->parentData << std::endl;
    std::cout << "Parent data: " << (*c->child).parentData << std::endl;
    delete p;
    try
    {
        std::cout << "Parent data: " << c->child->parentData << std::endl;
    }
    catch (PCPair::RelativeDoesNotExist &e)
    {
        std::cout << "Accessing deleted parent through \"->\" failed as expected." << std::endl;
    }
    try
    {
        std::cout << "Parent pointer: " << (*c->child).parentData << std::endl;
    }
    catch (PCPair::RelativeDoesNotExist &e)
    {
        std::cout << "Accessing deleted parent through \"*\" failed as expected." << std::endl;
    }

    return 0;
}
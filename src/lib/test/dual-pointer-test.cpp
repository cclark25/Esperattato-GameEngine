#include "../dual-pointer.h"
#include <vector>

class P;
class C;

typedef ParentChild<P, C> PCPair;

class P
{
public:
    // std::vector<std::shared_ptr<ParentChild<Parent, Child>>> children;
    // void addChild(Child &child)
    // {
    //     // auto newChild = std::shared_ptr<ParentChild<Parent, Child>>(new ParentChild<Parent, Child>(this, child.parent));
    //     // children.push_back();
    // }
    PCPair::Parent parent;
    P() : parent(this) {}
};

class C
{
public:
    PCPair::Child child;
    // Child() : child(this) {}
    C(P &parent) : child(this, parent.parent) {}
};

int main()
{
    P p = P();
    C c = C(p);

    auto pp = &p;
    auto cp = &c;

    return 0;
}
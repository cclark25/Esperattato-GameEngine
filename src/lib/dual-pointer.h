#ifndef LIB_DUAL_PTR
#define LIB_DUAL_PTR
#include <memory>

template <class ParentClass, class ChildClass>
class ParentChild
{
private:
    struct coreData
    {
        ParentClass *parent = nullptr;
        ChildClass *child = nullptr;
    };
    std::shared_ptr<coreData> core;
    bool isParent = false;

public:
    class ParentChildError : public std::runtime_error
    {
    };
    class ExistingParentException : public ParentChildError
    {
    };
    class ExistingChildException : public ParentChildError
    {
    };

    class Child;
    class Parent
    {
    private:
        std::shared_ptr<coreData> core;
        Parent(const Parent &other) {}

    public:
        Parent(ParentClass *parentPtr)
        {
            this->core = std::shared_ptr<coreData>(new coreData());
            this->core->parent = parentPtr;
        }
        Parent(ParentClass *parentPtr, Child &child)
        {
            if (child.core->parent != nullptr)
            {
                throw ExistingParentException("ParentChild pairing already has a parent.");
            }
            this->core = child.core;
            this->core->parent = parentPtr;
        }

        friend class Child;
    };

    class Child
    {
    private:
        std::shared_ptr<coreData> core;
        Child(const Child &other) {}

    public:
        Child(ChildClass *childPtr)
        {
            this->core = std::shared_ptr<coreData>(new coreData());
            this->core->child = childPtr;
        }
        Child(ChildClass *childPtr, Parent &parent)
        {
            if (parent.core->child != nullptr)
            {
                // throw ExistingChildException("ParentChild pairing already has a child.");
            }
            this->core = parent.core;
            this->core->child = childPtr;
        }
        friend class Parent;
    };

    // ParentChild(ChildClass *child) : core(new coreData())
    // {
    //     this->core->child = child;
    //     this->isParent = false;
    // }
    // ParentChild(ParentClass *parent, DualPointer<ParentClass, ChildClass> &child) : core(child.core)
    // {
    //     this->core->parent = parent;
    //     this->isParent = true;
    // }
    // ParentChild(const DualPointer<ParentClass, ChildClass> &other)
    // {
    //     this->isParent = other.isParent;
    //     this->core = other.core;
    // }

    // ~ParentChild()
    // {
    //     if (this->isParent)
    //     {
    //         this->core->parent = nullptr;
    //     }
    //     else
    //     {
    //         this->core->child = nullptr;
    //     }
    // }
};

#endif
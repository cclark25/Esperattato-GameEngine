#ifndef LIB_DUAL_PTR
#define LIB_DUAL_PTR
#include <memory>
#include <exception>
#include <stdexcept>

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
    class ParentChildError : public std::logic_error
    {
        using std::logic_error::logic_error;
    };
    class RelativeDoesNotExist : public ParentChildError
    {
        using ParentChildError::ParentChildError;
    };
    class ExistingParentException : public ParentChildError
    {
        using ParentChildError::ParentChildError;
    };
    class ExistingChildException : public ParentChildError
    {
        using ParentChildError::ParentChildError;
    };

    template <class thisClass, class otherClass, bool inversed>
    class EntryPoint
    {
    private:
        std::shared_ptr<coreData> core;
        EntryPoint(const EntryPoint<thisClass, otherClass, inversed> &other)
        {
        }

    public:
        EntryPoint(thisClass *ptr)
        {
            this->core = std::shared_ptr<coreData>(new coreData());
            if (inversed)
            {
                this->core->child = (ChildClass *)(void *)ptr;
            }
            else
            {
                this->core->parent = (ParentClass *)(void *)ptr;
            }
        }
        EntryPoint(thisClass *parentPtr, EntryPoint<otherClass, thisClass, !inversed> &child)
        {
            void **pointer = nullptr;
            if (inversed)
            {
                pointer = (void **)&(child.core->child);
            }
            else
            {
                pointer = (void **)&(child.core->parent);
            }

            if (*pointer != nullptr)
            {
                throw ExistingParentException("ParentChild pairing already has a parent.");
            }
            this->core = child.core;
            *pointer = parentPtr;
        }
        ~EntryPoint()
        {
            void **pointer = nullptr;
            if (inversed)
            {
                pointer = (void **)&(this->core->child);
            }
            else
            {
                pointer = (void **)&(this->core->parent);
            }
            *pointer = nullptr;
        }

        otherClass &operator*() const
        {
            void **pointer = nullptr;
            if (inversed)
            {
                pointer = (void **)&(this->core->parent);
            }
            else
            {
                pointer = (void **)&(this->core->child);
            }

            if (*pointer == nullptr)
            {
                throw RelativeDoesNotExist("Relative is null and does not exist.");
            }
            return *((otherClass *)(*pointer));
        }

        otherClass *operator->() const
        {
            void **pointer = nullptr;
            if (inversed)
            {
                pointer = (void **)&(this->core->parent);
            }
            else
            {
                pointer = (void **)&(this->core->child);
            }

            if (*pointer == nullptr)
            {
                throw RelativeDoesNotExist("Relative is null and does not exist.");
            }

            return (otherClass *)(*pointer);
        }

        bool operator==(EntryPoint<thisClass, otherClass, inversed> &other) const {
            void **thisPointer = nullptr;
            if (inversed)
            {
                thisPointer = (void **)&(this->core->parent);
            }
            else
            {
                thisPointer = (void **)&(this->core->child);
            }

            void **otherPointer = nullptr;
            if (inversed)
            {
                otherPointer = (void **)&(other.core->parent);
            }
            else
            {
                otherPointer = (void **)&(other.core->child);
            }

            return *otherPointer == *thisPointer;
        }

        friend class EntryPoint<otherClass, thisClass, !inversed>;
    };

    typedef EntryPoint<ParentClass, ChildClass, false> Child;
    typedef EntryPoint<ChildClass, ParentClass, true> Parent;
};

#endif
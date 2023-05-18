#ifndef _BASE_DEF_STD_H_
#define _BASE_DEF_STD_H_
#include <string>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include <memory.h>
using namespace std;

class any;

template<class Type> Type any_cast(any&);
template<class Type> Type any_cast(const any&);
template<class Type> Type* any_cast(any*);
template<class Type> const Type* any_cast(const any*);

template<class Type> bool is_any_type(any&);
template<class Type> bool is_any_type(const any&);
template<class Type> bool is_any_type(any*);
template<class Type> bool is_any_type(const any*);


struct bad_any_cast : public std::bad_cast {};


struct placeholder {

    virtual placeholder* clone()  = 0;
    virtual const std::type_info& type()  = 0;
    virtual void* getValue() = 0;
    virtual void reset() = 0;
    //virtual ~placeholder() {}

};

template<class T>
struct concrete : public placeholder 
{
    concrete(T&& x) {value = x;}
    concrete(const T& x) { value =x;}
    placeholder* clone() {return (new concrete<T>(value));}
    const std::type_info& type()  { return typeid(T); }
    void* getValue() {return (void*)&value;};
    void reset()
    { 
      
    };
    T value;
};

class any
{
public: 


    template<class Type> friend 
        Type any_cast(any&);

    template<class Type> 
    friend Type any_cast(const any&);

    template<class Type> 
    friend Type* any_cast(any*);

    template<class Type> 
    friend const Type* any_cast(const any*);

    template<class T>
    any(const T& tvalue):ptr(NULL)
    {
        ptr = new concrete<T>(tvalue);
    }

    any():ptr(NULL)
    {

    }

	~any()
	{
		if (ptr != NULL)
		{
            delete ptr;
		}
        ptr = NULL;
	}

    void reset()
    {
        if (ptr !=NULL)
        {
            ptr->reset();
            delete ptr;
            ptr = NULL;
        }
    }

    any(any&& x) :ptr(NULL)
    {
        if (x.ptr != NULL)
        {
            ptr = x.ptr->clone();
        }
    }

    any(const any& x) :ptr(NULL)
    {
        if (x.ptr != NULL)
        {
            ptr = x.ptr->clone();
        }
    }

    const any& operator=(const any& other)
    {
        if(&other == this)
        {
            return *this;
        }
        reset();
        ptr = other.ptr->clone();
        return *this;
    }

    template<class T> any& operator=(const T& x) 
    {
        reset();
        ptr = new concrete<T>(x);
        return (*this);
    }


    any& clone(const any& rhs)
        {
            if (rhs.ptr)
            {
                ptr = rhs.ptr->clone();
            }
            return (*this);
        }

        any& clone(any& rhs) {
            if (rhs.ptr)
            {
                ptr = rhs.ptr->clone();

            }
            return (*this);
        }

        any& clone(any&& rhs) {
            if (rhs.ptr)
            {
                ptr = rhs.ptr->clone();

            }
            return (*this);
        }

        any& clone(const any&& rhs) {
            if (rhs.ptr)
            {
                ptr = rhs.ptr->clone();
            }
            return (*this);
        }

    const std::type_info& type() const
	{
        if (ptr != NULL)
        {
            return ptr->type();
        }
		return typeid(void);
	}
    const void* getValue() const
    {
        if (ptr != NULL)
        {
            return ptr->getValue();
        }
        return NULL;
    }
	
	bool empty() const
	{
		return ptr == NULL;
	}
private:
    placeholder* ptr;
};


template<class Type> 
Type any_cast(any& val) {
    if (val.type() != typeid(Type))
        throw bad_any_cast();
    return *(static_cast<Type*>((Type*)val.getValue()));
}

template<class Type> 
Type any_cast(const any& val) {
    if (val.type() != typeid(Type))
        throw bad_any_cast();
    return *(static_cast<Type*>((Type*)val.getValue()));
}

template<class Type> 
Type* any_cast(any* ptr) {
    if (ptr->type() != typeid(Type))
        throw bad_any_cast();
    return *(dynamic_cast<Type*>(ptr->getValue()));
}

template<class Type> 
const Type* any_cast(const any* ptr) {
    if (ptr->type() != typeid(Type))
        throw bad_any_cast();
    return *(dynamic_cast<Type*>(ptr->getValue()));
}

template<class Type> 
bool is_any_type(any& val) {
    return val.type() == typeid(Type);
}

template<class Type> 
bool is_any_type(const any& val) {
    return val.type() == typeid(Type);
}

template<class Type> 
bool is_any_type(any* ptr) {
    return ptr->type() == typeid(Type);
}

template<class Type> 
bool is_any_type(const any* ptr) {
    return ptr->type() == typeid(Type);
}

#endif

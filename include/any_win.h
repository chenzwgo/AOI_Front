#ifndef _RUNTIME_ANY_WIN_H_
#define _RUNTIME_ANY_WIN_H_

#include <exception>
#include <memory>
#include <typeinfo>
#include <type_traits>
    
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

class any {
public:

    template<class Type> friend 
    Type any_cast(any&);
    
    template<class Type> 
    friend Type any_cast(const any&);
    
    template<class Type> 
    friend Type* any_cast(any*);
    
    template<class Type> 
    friend const Type* any_cast(const any*);
    
    any() 
        : ptr(nullptr) 
        {}

    any(any&& x) 
        : ptr(std::move(x.ptr)) 
        {}
        
    any(const any& x) {
        if (x.ptr)
            ptr = x.ptr->clone();
    }
    
    template<class Type> any(const Type& x) 
        : ptr(new concrete<typename std::decay<const Type>::type>(x)) 
        {}

    any& operator=(any& rhs) {
        ptr = rhs.ptr->clone();
        return (*this);
    }

    any& operator=(const any& rhs) {
        ptr = rhs.ptr->clone();
        return (*this);
    }

    template<class T> any& operator=(T&& x) {
        if (strcmp(typeid(x).name() ,"class any")==0)
        {
            ptr = std::move(any(x).ptr);
            return (*this);
        }
        ptr.reset(new concrete<typename std::decay<T>::type>(typename std::decay<T>::type(x)));
        return (*this);
    }  

    template<class T> any& operator=(const T&& x) {
        if (strcmp(typeid(x).name() ,"class any")==0)
        {
            ptr = std::move(any(x).ptr);
            return (*this);
        }
        ptr.reset(new concrete<typename std::decay<T>::type>(typename std::decay<T>::type(x)));
        return (*this);
    }

	any& clone(const any& rhs)
	{
		if (rhs.ptr)
		{
			clear();
			ptr = rhs.ptr->clone();
		}
		return (*this);
	}

    any& clone(any& rhs) {
        if (rhs.ptr)
        {
            clear();
            ptr = rhs.ptr->clone();

        }
        return (*this);
    }

	any& clone(any&& rhs) {
		if (rhs.ptr)
		{
			clear();
			ptr = rhs.ptr->clone();

		}
		return (*this);
	}

    any& clone(const any&& rhs) {
        if (rhs.ptr)
        {
            clear();
            ptr = rhs.ptr->clone();

        }
        return (*this);
    }
	
    void clear() { 
        ptr.reset(nullptr); 
    }

    bool empty() const { 
        return ptr == nullptr; 
    }

    const std::type_info& type() const { 
        return (!empty()) 
            ? ptr->type() 
            : typeid(void); 
    }
    ~any()
    {
        clear();
    }
private:
    
    struct placeholder {

        virtual std::unique_ptr<placeholder> clone() const = 0;
        virtual const std::type_info& type() const = 0;
        //virtual ~placeholder() {}

    };
    
    template<class T>
    struct concrete : public placeholder {

        concrete(T&& x) 
            : value(std::move(x)) 
            {}

        concrete(const T& x) 
            : value(x) 
            {}

        virtual std::unique_ptr<placeholder> clone() const override {
            return std::unique_ptr<placeholder>(new concrete<T>(value));
        }

        virtual const std::type_info& type() const override { 
            return typeid(T); 
        }

        T value;

    };
    
    std::unique_ptr<placeholder> ptr;
    
};

template<class Type> 
Type any_cast(any& val) {
    if (val.ptr->type() != typeid(Type))
		throw bad_any_cast();
	return static_cast<any::concrete<Type>*>(val.ptr.get())->value;
}

template<class Type> 
Type any_cast(const any& val) {
	if (val.ptr->type() != typeid(Type))
		throw bad_any_cast();
	return static_cast<any::concrete<Type>*>(val.ptr.get())->value;
}

template<class Type> 
Type* any_cast(any* ptr) {
    return dynamic_cast<Type*>(ptr->ptr.get());
}

template<class Type> 
const Type* any_cast(const any* ptr) {
    return dynamic_cast<const Type*>(ptr->ptr.get());
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
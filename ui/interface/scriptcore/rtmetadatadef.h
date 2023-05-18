#ifndef _META_DATA_DEF_H_
#define _META_DATA_DEF_H_

#define R_METHOD(_method) #_method
#define R_METHOD_AND_ARGS(_method)  #_method
#define R_ARG(type, data) Argument<type >(#type, data)
#define R_RETURN_ARG(type, data) ReturnArgument<type >(#type, data)


#define ParArg      GenericArgument
#define ReturnArg   GenericReturnArgument


class  GenericArgument
{
public:
    inline GenericArgument(const char *aName = 0, const void *aData = 0)
        : _data(aData), _name(aName) {}
    inline void *data() const { return const_cast<void *>(_data); }
    inline const char *name() const { return _name; }

private:
    const void *_data;
    const char *_name;
};

class  GenericReturnArgument: public GenericArgument
{
public:
    inline GenericReturnArgument(const char *aName = 0, void *aData = 0)
        : GenericArgument(aName, aData)
        {}
};

template <class T>
class Argument: public GenericArgument
{
public:
    inline Argument(const char *aName, const T &aData)
        : GenericArgument(aName, static_cast<const void *>(&aData))
        {}
};
template <class T>
class Argument<T &>: public GenericArgument
{
public:
    inline Argument(const char *aName, T &aData)
        : GenericArgument(aName, static_cast<const void *>(&aData))
        {}
};


template <typename T>
class ReturnArgument: public GenericReturnArgument
{
public:
    inline ReturnArgument(const char *aName, T &aData)
        : GenericReturnArgument(aName, static_cast<void *>(&aData))
        {}
};
#endif

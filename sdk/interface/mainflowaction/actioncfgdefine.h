#ifndef ACTIONCFG_DEFINE_H
#define ACTIONCFG_DEFINE_H

#include <string>
using namespace std;

struct ACTION_PARAM
{
    virtual void setString(const string &data) = 0;
    virtual string toString() = 0;
};

#endif // ACTIONCFG_DEFINE_H

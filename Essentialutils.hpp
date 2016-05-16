#pragma once
#include <functional>
#include <valarray>
#include <array>
#include <typeindex>
#include <cstdarg>
#include <map>
#include <set>
#include <list>
#include <cstddef>
#include <stdexcept>
#include <cassert>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include <cstring>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <cxxabi.h>
#include <unordered_map>
#include <unordered_set>
using namespace std;
/**
 * @brief foreach macro (STL containers)
 */
#define foreach(var, container)			\
  for(auto var = (container).begin();		\
  var != (container).end();			\
  ++var)
/**
 * @brief each macro (custom containers)
 */
#define EACH(it,container)			\
  for(size_t it=0;		\
  (container).checkindexvalidity(it);			\
  ++it)
/**
 * @brief macro to implement default
 * special member functions
 */
#define SPECIALMEMBERSREFERENCE(C)\
  C(const C&) = default;\
  C(C&&) = default;\
  C& operator=(const C&) & = default;\
  C& operator=(C&&) & = default;\
  virtual ~C() = default;
/**
 * @brief macro to implement default
 * special member functions
 */
#define SPECIALMEMBERSVALUE(C)\
  C(const C&) = default;\
  C(C&&) = default;\
  C& operator=(const C&) & = default;\
  C& operator=(C&&) & = default;\
  ~C() = default;
/**
  * @brief Template member pointer (don't forget the argument types!)
  */
#define TEMPLATE_MEMBER(returntype,classname,functionpointername) returntype (classname::*functionpointername)
/**
 * @brief Declare member pointer (don't forget the arguments types!)
 */
#define DECLARE_MEMBER(returntype,classname,functionpointername) typedef TEMPLATE_MEMBER(returntype,classname,functionpointername)
/**
  * @brief Assign member pointer variable to a member
  */
#define ASSIGN_MEMBER(functionpointername,afunctionpointer,classname,amember) functionpointername afunctionpointer=&classname::amember
/**
 * @brief Invoke member pointer
 */
#define ACCESS_MEMBER(objectvariable,afunctionpointer) ((objectvariable).*(afunctionpointer))
/**
 * @brief Macro to invoke a static constructor or destructor of a class
 * to be inserted in a .cpp file
 * after initialization of any static data members
 * Signatures are "static void StaticConstructor()" and "static void StaticDestructor()"
 * when used, both constructor and destructor must be declared
 */
#define INVOKE_STATIC_CONSTRUCTOR(ClassName) \
  INVOKE_STATIC_CONSTRUCTOR_EXPANDED(ClassName)
template<typename ClassName>
struct StaticInvoker
{
    // Default Constructor:
    StaticInvoker()
    {
        // Call to the static constructor of ClassName:
        ClassName::StaticConstructor();
    }

    // Destructor:
    virtual ~StaticInvoker()
    {
        // Call to the static destructor of ClassName:
        ClassName::StaticDestructor();
    }
};
// Macro with expanded name:
#define INVOKE_STATIC_CONSTRUCTOR_EXPANDED(ClassName) \
  /* Single instance of this invoker class, so its constructor is called only once */ \
  StaticInvoker<ClassName> staticInvokerOf_##ClassName;

namespace Utils
{
/**
* @brief Address fortran-like arrays
*/
inline
int
address (int nrows, int irow, int icol)
{
    return irow + (icol - 1) * nrows;
}
/**
* @brief Performs a copy of a C array
*/
template<typename T>
void
copyarray (T* from, size_t dimfrom, T* to, size_t dimto)
{
    size_t dim = min (dimfrom, dimto);
    if (to&&from)
    {
        //  if(dim) memcpy( (void*)to, (void*)from, dim * sizeof(T) );
        for (size_t i = 0; i != dim; ++i)
            to[i] = from[i];
    }
}
/**
 * @brief Convert a Rvalue to Lvalue
 */
template <typename Value>
Value& lvalue (Value&& rvalue)
{
    return rvalue;
}

/**
* @brief Performs a dereference of a pointer
* @return Value stored by argument (which is a pointer)
*/
template<typename S>
S&
deref (S* arg)
{
    assert (arg);
    return *arg;
}

/**
* @brief Performs a reference of a variable
* @return Address stored by argument (which is a value)
*/
template<typename S>
S*
refer (S& arg)
{
    return &arg;
}

/**
* @brief Downcasts a pointer
*        Transforms a pointer to a more specific (derived) pointer
*/
template <typename Target, typename Source>
Target*
downcast (Source* x)
{
    assert (dynamic_cast<Target*> (x) == x);
    return static_cast<Target*> (x);
}

/**
* @brief Downcasts a reference
*        Transforms a reference to a more specific (derived) reference
*/
template <typename Target, typename Source>
Target&
downcast (Source& x)
{
    assert (dynamic_cast<Target*> (&x) == &x);
    return * (static_cast<Target*> (&x));
}

/**
* @brief Destroys a previously newed pointer
*/
template<typename T>
inline void
destroy (T*& p)
{
    if (p)
    {
        delete p;
        p = nullptr;
    }
}

/**
* @brief Destroys an array
*/
template<typename T>
inline void
destroyarray (T*& p)
{
    if (p)
    {
        delete [] p;
        p = 0;
    }
}

/**
* @brief Creates an array
*/
template<typename T>
inline void
createarray (T*& p,const size_t& size)
{
    destroyarray (p);
    if (size>0)p=new T[size];
}

/**
* @brief Returns the name of a type from template argument
*/
template<typename T>
string
namefromtype()
{
    int status;
    string temp (abi::__cxa_demangle (typeid (T).name(),0,0,&status));
    return temp;
}

/**
* @brief Returns the name of a type from argument
*/
template<typename T>
string
namefromtype (const T& type)
{
    int status;
    string temp (abi::__cxa_demangle (typeid (type).name(),0,0,&status));
    return temp;
}

/**
* @brief Invokes a Unix command in the terminal
*/
inline
int
invokecommand (const string& com)
{
    return system (com.c_str());
}

/**
* @brief The compare function using solely the < operator
*/
template<typename ValueType>
int
compare (const ValueType& first, const ValueType& second)
{
    if (first < second)return -1;
    if (second < first)return 1;
    return 0;
}

/**
* @brief Gets all files with a given extension
*        Using a temporary file to store the hits
*/
inline
vector<string>
getlistoffileswithextension (const string& ext,const string& temporary="__tmp__")
{
    vector<string> res;
    ifstream listoffiles;
    invokecommand (string ("ls *.")+ext+string (" > "+temporary));
    listoffiles.open (temporary);
    string name;
    for (;;)
    {
        listoffiles>>name;
        if (!listoffiles)break;
        res.push_back (name);
    }
    return res;
}
}

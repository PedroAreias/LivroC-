#pragma once
#include "Essentialutils.hpp"
#include "IOutils.hpp"
using namespace std;
namespace Utils
{
/**
 * @brief The IGetTypeNumber struct
 *        for entities that will be inserted in a graph manager
 */
struct IGetTypeNumber
{
public:
    virtual const size_t& obtaintypenumber() const=0;
    virtual ~IGetTypeNumber()
    {
    }
};
/**
* @brief Init interface
*        required by the factory class
*        corresponds to a two stage construction
*/
struct Init
{
protected:
    virtual void init() = 0;
public:
    virtual ~Init() { }
};
/**
  * @brief
  *
  *
  */
template<typename Value>
struct IComparable
{
    virtual ~IComparable() { }
    friend bool operator> (Value const &lhs, Value const &rhs)
    {
        return rhs < lhs;
    }
    friend bool operator== (Value const &lhs, Value const &rhs)
    {
        return ! (lhs < rhs || lhs > rhs);
    }
    friend bool operator!= (Value const &lhs, Value const &rhs)
    {
        return ! (rhs == lhs);
    }
    friend bool operator<= (Value const &lhs, Value const &rhs)
    {
        return ! (rhs < lhs);
    }
    friend bool operator>= (Value const &lhs, Value const &rhs)
    {
        return ! (lhs < rhs);
    }
};
/**
* @brief Serializable interface
*        creates a set of IO functions and operators
*        for the derived class
*        from 4 protected functions
*
*/
struct ISerializable
{
protected:
    /**
     * @brief Input from ASCII istream file
     * @param fich A istream already opened
     */
    virtual void inp (istream & inputfile) = 0;
    /**
     * @brief Output to ASCII istream file
     * @param fich A ostream already opened
     */
    virtual void out (ostream & outputfile) const = 0;
    /**
     * @brief Input from BINARY fstream file
     * @param fich A fstream already opened
     */
    virtual void read (fstream & inputbinaryfile) = 0;
    /**
     * @brief Output to BINARY fstream file
     * @param fich A fstream already opened
     */
    virtual void write (fstream & outputbinaryfile) const = 0;
public:

    virtual ~ISerializable() { }

    friend void reading (fstream& fich, ISerializable & quantity)
    {
        quantity.read (fich);
    }

    friend void writing (fstream& fich, const ISerializable & quantity)
    {
        quantity.write (fich);
    }

    friend istream& operator>> (istream& fich, ISerializable & quantity)
    {
        quantity.inp (fich);
        return fich;
    }

    friend ostream& operator<< (ostream& fich, const ISerializable & quantity)
    {
        quantity.out (fich);
        return fich;
    }
};

}

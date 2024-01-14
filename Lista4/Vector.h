#ifndef Vector_h
#define Vector_h
#include <iostream>
namespace cpplab
{


template<class C>
class Vector
{
public:
    using value_type = C;
    Vector();
    Vector(std::initializer_list<value_type>);
    Vector(value_type*,int);
    ~Vector();
    Vector(const Vector<C>&);
    Vector(Vector<C>&&);
    void printValues() const noexcept;
    void printSize() const noexcept;
    void push_back(value_type&&);
    void push_back(const value_type&);
    void pop_back();
    void resize(int);
    template <class T,class ... Types>
    void emplace_back(Types&&... );
    value_type& operator [](int);
    value_type& returnValue(int) const;
    Vector<value_type>& operator =(const Vector<value_type>&);
    Vector<value_type>& operator =(Vector<value_type>&&);
    const value_type& operator [](int) const;
    size_t size() const noexcept;
    bool check_if_empty() const noexcept;
 /*   auto operator *(const valid_arg auto& vec2)
    {
        
        if( vec2.size() != this->size())
            throw std::logic_error("Vectors size are not equal");
        else
        {
            auto result = vec2[0]*this->returnValue(0);
            for(int i=1;i<this->size();i++)
                result +=vec2[i]*this->returnValue(i);
            return result;
        }

    }
 */

    
private:
    value_type* values = nullptr;
    size_t vector_size = 0;
    void copy_values(int copy_size,C* newPointer);
};
}



#endif

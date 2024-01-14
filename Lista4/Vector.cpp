#include "Vector.h"
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <concepts>

template < class F>
concept valid_arg = (std::is_arithmetic_v<typename F::value_type> && std::is_same_v<F, std::vector<typename F::value_type>>) ||
(std::is_arithmetic_v<typename F::value_type> && std::is_same_v<F, cpplab::Vector<typename F::value_type>>);


auto operator *(const valid_arg auto& vec1,const valid_arg auto& vec2)
{
        if( vec1.size() != vec2.size())
            throw std::logic_error("Vectors sizes are not equal");
        
        if(vec1.size()==0 || vec2.size()==0)
            throw std::logic_error("Vectors sizes are equal 0");
        
    
        else
        {
            auto result = vec2[0]*vec1[0];
            for(int i=1;i<vec1.size();i++)
                result +=vec2[i]*vec1[i];
            return result;
        }

}

class Pixel{
public:
    Pixel(){};
    Pixel(int r, int g,int b) : r(r) , g(g) , b(b){};
    int r=0;
    int g=0;
    int b=0;
    friend std::ostream& operator<<(std::ostream& os, const Pixel& dt);
    
};
std::ostream& operator<<(std::ostream& os, const Pixel& p)
{
    os << p.r << ' ' << p.g << ' ' << p.b <<"\n";
    return os;
}


template<class C>
template <class T,class ... Types>
void cpplab::Vector<C>::emplace_back(Types&& ...types )
{
    this->push_back(C{std::forward<T>(types)...});
}

template<class C>
cpplab::Vector<C>::~Vector()
{
    if(values!=nullptr)
    delete [] values;
}

template<class C>
cpplab::Vector<C>::Vector() : vector_size(0), values(nullptr){};

template<class C>
cpplab::Vector<C>::Vector(std::initializer_list<C> arg_list) : vector_size(arg_list.size())
{
    values = new C[vector_size];
    int i=0;
    for(auto value : arg_list){
        values[i] = value;
        i++;
    }
}

template<class C>
cpplab::Vector<C>::Vector(C* pointer,int size) : vector_size(size)
{
    values = new C[size];
    for(int i=0;i<size;i++){
        values[i] = pointer[i] ;
    }
}

template <class C>
cpplab::Vector<C>::Vector(const Vector<C>& vec2)
{
    std::cout<<"Using lvalue copy constructor"<<"\n";
    if(this != &vec2)
    {
        this->vectorSize = vec2.vectorSize;
        this->values = new C[this->vectorSize];
        for(int i=0;i<this->vectorSize;i++)
            this->values[i] = vec2.returnValue(i);
    }
    
}
template <class C>
cpplab::Vector<C>::Vector(Vector<C>&& vec2)
{
    std::cout<<"Using rvalue copy constructor"<<"\n";
    if(this != &vec2)
    {
        if(!check_if_empty())
        {
            delete [] values;
            vector_size = 0;
        }
        this->vectorSize = vec2.vectorSize;
        this-> values = vec2.values;
        vec2.vectorSize=0;
        vec2.values = nullptr;
    }
}

    template<class C>
    void cpplab::Vector<C>::printValues() const noexcept
    {
        if(vector_size>0){
            for(int i=0;i<vector_size;i++)
                std::cout<<values[i]<<" ";
            std::cout<<"\n";
        }
        else
            std::cout<<"Vector size is 0"<<"\n";
    }

    template<class C>
    void cpplab::Vector<C>::printSize() const noexcept
    {
        std::cout<<vector_size<<"\n";
    }

    template<class C>
    size_t cpplab::Vector<C>::size() const noexcept
    {
        return vector_size;
    }

    template<class C>
    void cpplab::Vector<C>::push_back(C&& value)
    {
        std::cout<<"rvalue push back"<<"\n";
        resize(vector_size+1);
        values[vector_size-1] = value;
    }
template<class C>
void cpplab::Vector<C>::push_back(const C& value)
{
    std::cout<<"lvalue push back"<<"\n";
    resize(vector_size+1);
    values[vector_size-1] = value;
}

    template<class C>
    void cpplab::Vector<C>::pop_back()
    {
        if(vector_size<1)
            std::cout<<"Vector size is 0"<<"\n";
        else
        resize(vector_size-1);
    }
    
    template<class C>
    cpplab::Vector<C>& cpplab::Vector<C>::operator=(const Vector<value_type> & vec2)
    {
        std::cout<<"Using lvalue = operator"<<"\n";
        if(this != &vec2)
        {
            if(!check_if_empty())
            {
                delete [] this-> values;
                vector_size=0;
            }
            this->vectorSize = vec2.vectorSize;
            this->values = new C[this->vectorSize];
            for(int i=0;i<this->vectorSize;i++)
            this->values[i] = vec2.returnValue(i);
        }
        return *this;
    }

template<class C>
cpplab::Vector<C>& cpplab::Vector<C>::operator=(Vector<value_type>&& vec2)
{
    std::cout<<"Using rvalue = operator"<<"\n";
    if(this != &vec2)
    {
        if(!check_if_empty())
        {
            delete [] this-> values;
            vector_size=0;
        }
        this->vectorSize = vec2.vectorSize;
        this->values = vec2.values;
        vec2.values = nullptr;
        vec2.vectorSize=0;
    }
    return *this;
}
    
    
    template<class C>
    C& cpplab::Vector<C>::operator [](int index)
    {
            if(index> vector_size-1 || index<0)
                throw std::out_of_range("Given index is out of vector range");
            else
                return values[index];
    }

template<class C>
const  C& cpplab::Vector<C>::operator [](int index) const
{
        if(index> vector_size-1 || index<0)
            throw std::out_of_range("Given index is out of vector range");
        else
            return values[index];
}

    template<class C>
    void cpplab::Vector<C>::resize(int new_size)
    {
            C* temporaryVector = new C[new_size];
            if(!check_if_empty())
            {
                copy_values(new_size,temporaryVector);
                delete [] values;
            }
                values = temporaryVector;
                vector_size=new_size;
        
    }

    template<class C>
    void cpplab::Vector<C>::copy_values(int copy_size,C* newPointer)
    {
        std::cout<<copy_size<<"\n";
        for(int i=0;i<copy_size;i++)
        {
            newPointer[i] = values[i];
        }
    }

template<class C>
C& cpplab::Vector<C>::returnValue(int index) const
{
    
        if(index> vector_size-1 || index<0)
            throw std::out_of_range("Given index is out of vector range");
        else
            return values[index];
}

template<class C>
bool cpplab::Vector<C>::check_if_empty() const noexcept
{
    if(this->values == nullptr)
        return true;
    return false;
}

int main(int argc, const char * argv[]) {
   
 
    using namespace cpplab;
    {
        try{
            Vector<double> vec1{1.3333,2.5};
            std::vector<char>vec2{'F','F'} ;
            std::cout<<vec2*vec1<<"\n";
            std::cout<<vec1*vec2<<"\n";
            
            
        }
        catch(const std::out_of_range& er)
        {
            std::cout<<er.what()<<"\n";
        }

        catch(const std::logic_error& er)
        {
            std::cout<<er.what()<<"\n";
        }
        catch(...)
        {
            std::cerr<<"Unknown error"<<"\n";
        }
    }
    
    
       
}

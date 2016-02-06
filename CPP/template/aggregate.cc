#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>

template <typename T>
class Aggregate {

    // The intention is to define a class 
    // which can store any object

  public:
    Aggregate(const T& val);
    ~Aggregate();
    void set(const T& val);
    T* get();

    template <typename U>
    U* get()
    {
	return reinterpret_cast<U*>(d_data);
    }

    T* operator->() 
    {
	return get();
    }

    T& operator*() 
    {
	return *get();
    }

    template <typename U>
    struct rebind
    {
	typedef Aggregate<U> Other;
	/*
	U* get()
	{
	    return reinterpret_cast<U*>(d_data);
	}
	*/
    };

  private:
    char* d_data;
};

template <typename T> inline
Aggregate<T>::Aggregate(const T& val) : d_data(new char[sizeof(val)])
{
    set(val);
}

template <typename T> inline
Aggregate<T>::~Aggregate()
{
    delete []d_data;
}

template <typename T> inline
T* Aggregate<T>::get()
{
    return reinterpret_cast<T*>(d_data);
}

template <typename T> inline
void Aggregate<T>::set(const T& val)
{
    ::memcpy(d_data, &val, sizeof(val));
}

int main()
{
    Aggregate<double> aggInt(115111);
    std::cout << *(aggInt.get<std::string>()) << '\n';
    
    return EXIT_SUCCESS;
}

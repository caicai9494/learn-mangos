#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <cstring>
#include <typeinfo>

template <typename T>
T accu1(const T* from, const T* to) 
{
    T total = T();
    while (from != to) {
	total += *from;
	++from;
    }
    return total;
}

template <typename T>
class AccuT;

template <>
class AccuT<int> {
  public:
    typedef int AccuTrait;
        // type trait
    static AccuTrait zero()
    {
	return 0;
    }
};

template <>
class AccuT<char> {
  public:
    typedef int AccuTrait;
    static AccuTrait zero()
    {
	return 0;
    }
};

template <typename T>
typename AccuT<T>::AccuTrait accu2(const T* from, const T* to) 
{
    typedef typename AccuT<T>::AccuTrait AccuTrait;
    AccuTrait total = AccuT<T>::zero();
    while (from != to) {
	total += *from;
	++from;
    }
    return total;
}

template <typename T>
class SumPolicy {
  public:
    static void accu(T& ret, const T& val)
    {
	ret += val;
    }
};

template <typename T>
class MultPolicy {
  public:
    static void accu(T& ret, const T& val)
    {
	ret *= val;
    }
};

template <typename T, typename Policy = SumPolicy<T>, typename Trait = AccuT<T> >
typename Trait::AccuTrait accu3(const T* from, const T* to) 
{
    typedef typename Trait::AccuTrait AccuTrait;
    AccuTrait total = Trait::zero();
    while (from != to) {
	Policy::accu(total, *from);
	++from;
    }
    return total;
}

template <typename T, typename Policy = SumPolicy<T>, typename Trait = AccuT<T> >
typename Trait::AccuTrait accu4(const T* from, const T* to, typename Trait::AccuTrait ret) 
{
    typedef typename Trait::AccuTrait AccuTrait;
    AccuTrait total = ret;
    while (from != to) {
	Policy::accu(total, *from);
	++from;
    }
    return total;
}

int main()
{
    int arr[] = {1, 2, 3, 4};
    std::cout << accu1(arr, arr+4) << '\n';
    char str[] = "yes";
    std::cout << accu1(str, str+3) << '\n';

    std::cout << accu2(arr, arr+4) << '\n';
    std::cout << accu2(str, str+3) << '\n';

    // add policy
    std::cout << accu3(arr, arr+4) << '\n';

    // mult policy (1 initial value)
    std::cout << accu4<int, MultPolicy<int> >(arr, arr+4, 1) << '\n';

    // add policy (1 initial value)
    std::cout << accu4(arr, arr+4, 1) << '\n';
    return EXIT_SUCCESS;
}

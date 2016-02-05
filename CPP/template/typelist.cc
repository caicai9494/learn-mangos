
class TypeNull {};

template <typename HEAD, typename TAIL>
struct TypeList {
    typedef HEAD Head;
    typedef TAIL Tail;
};

#define TYPELIST_1(T1)  TypeList<T1, TypeNull>
#define TYPELIST_2(T1, T2)  TypeList<T1, TYPELIST_1(T2) >
#define TYPELIST_3(T1, T2, T3)  TypeList<T1, TYPELIST_2(T2, T3) >

int main()
{
    TYPELIST_3(int, float, double)::Head i = 5; 
    TYPELIST_3(int, float, double)::Tail::Head ii = 5.5; 
    TYPELIST_3(int, float, double)::Tail::Tail::Head iii = 5.55; 

    return 0;
}

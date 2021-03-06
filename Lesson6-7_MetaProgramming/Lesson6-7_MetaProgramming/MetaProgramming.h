#pragma once

/*Exercsise 2*/

template<int bin>
struct IsBinary
{
};

template<>
struct IsBinary<0>
{
	enum { value = 0 };
};

template<>
struct IsBinary<1>
{
	enum { value = 1 };
};

template <size_t N>
struct Binary
{
	static const size_t value = Binary <N / 10 >::value << 1 | IsBinary<N % 10>::value;
};

template <>
struct Binary <0>
{
	static const size_t value = 0;
};

/*Exercise 3.1*/

struct NullType {};

template <typename L, typename R>
struct TypeList
{
	typedef L First;
	typedef R Rest;
};

typedef TypeList<int, TypeList<std::string, TypeList<double, NullType> > > mixedTypes; //predefined types as macro.

#define TypeList_3(T1, T2, T3) TypeList<T1, TypeList<T2, TypeList<T3, NullType> > >  //generic typelist as definition, DOES NOT ACCEPT vector<int, allocator> OR ALIKE!
																					   // since this is read as two parameters: T1= "vector<int" and T2= "allocator>"
																					   // this is due to the preprocessor.
typedef TypeList_3(int, double, long) myTypes;

/*
Exercise 3.2.1
*/

template<typename A, typename B>
struct IsSame
{
	enum { value = false }; //std::is_same<A, B>::value
};

template<typename A>
struct IsSame<A ,A>
{
	enum{ value = true};
};


/*
Exercise 3.2.2
*/
typedef TypeList_3(int, char, long) TL;

template<typename TL, typename Comp>
struct Contains
{
	enum
	{
		value = std::is_same<typename TL::First, Comp>::value || Contains<typename TL::Rest, Comp>::value
	};
};

template<typename Comp>
struct Contains<NullType, Comp>
{
	enum{value = false};
};

/*
Exercise 3.2.3
*/

typedef TypeList_3(long, char, int) TL_ex323;

template<typename TL, int Index>
struct AtIndex
{
	typedef typename AtIndex< typename TL::Rest, Index - 1>::type type;
};

template<typename TL>
struct AtIndex<TL, 0>
{
	 typedef typename TL::First type;
};

/*
Exercise 3.2.4
*/

template<typename TL>
struct PrintIT
{
	PrintIT()
	{
		std::cout << typeid(typename TL::First).name() << ", ";
		PrintIT<typename TL::Rest>();
	}
};

template<>
struct PrintIT<NullType>
{
	PrintIT()
	{
	}
};

/*
Exercise 3.2.5
*/

typedef TypeList_3(long, char, int) TL_ex325;

//Solution from S�ren Hansen

//Method 1
template<typename TL, typename T> struct Remove;

template<typename First, typename Rest, typename T>
struct Remove<TypeList<First, Rest>, T>
{
	typedef TypeList<First, typename Remove<Rest, T>::type > type;
};

template<typename First, typename Rest>
struct Remove<TypeList<First, Rest>, First>
{
	typedef TypeList<typename Remove<Rest, First>::type::First, typename Remove<Rest, First>::type::Rest > type;
};

template<typename First>
struct Remove<TypeList<First, NullType>, First>
{
	typedef NullType type;
};


template<typename T>
struct Remove<NullType, T>
{
	typedef NullType type;
};

//Method 2

template<typename TL>
struct TLGetFirst
{
	typedef typename TL::First First;
};

template<>
struct TLGetFirst<NullType>
{
	typedef NullType First;
};


template<typename TL>
struct TLGetRest
{
	typedef typename TL::Rest Rest;
};

template<>
struct TLGetRest<NullType>
{
	typedef NullType Rest;
};



template<typename TL, typename T>
struct Erase
{
private:
	static const bool found = IsSame<typename TL::First, T>::value;
	static const bool lastElem = IsSame<typename TL::Rest, NullType>::value;
	typedef typename IfThenElse<typename TLGetFirst<typename TL::Rest>::First, typename TL::First, found>::Type First;
	typedef typename IfThenElse<typename TLGetRest<typename TL::Rest>::Rest, typename TL::Rest, found>::Type Rest;
public:
	typedef typename IfThenElse<NullType, TypeList<First, typename Erase<Rest, T>::Type >, found && lastElem>::Type Type;
};


template<typename T>
struct Erase<NullType, T>
{
	typedef NullType Type;
};


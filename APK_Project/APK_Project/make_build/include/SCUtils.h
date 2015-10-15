#ifndef SCUTILS_HPP
#define SCUTILS_HPP

namespace SCUtils
{
	enum LightStates
	{
		NORMALEXECUTION,
		/*
		* NorthSouth: Red
		* EastWest: Red
		*/
		INITIALRR,
		/*
		* NorthSouth: Green
		* EastWest: Red
		*/
		LS_1,
		/*
		* NorthSouth: Yellow
		* EastWest: Red
		*/
		LS_2,
		/*
		* NorthSouth: RedYellow
		* EastWest: Red
		*/
		LS_3,
		/*
		* NorthSouth: Red
		* EastWest: Green
		*/
		LS_4,
		/*
		* NorthSouth: Red
		* EastWest: Yellow
		*/
		LS_5,
		/*
		* NorthSouth: Red
		* EastWest: RedYellow
		*/
		LS_6
	};

	enum Direction
	{
		NORTH = 0,
		SOUTH,
		WEST,
		EAST
	};

	/*Metaprogramming methods*/

	template<typename T, typename D>
	struct Typelist
	{
		typedef T First;
		typedef D Rest;
	};

	struct NullType {};

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
		enum { value = false };
	};

}

#endif
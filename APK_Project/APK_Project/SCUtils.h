#pragma once

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
		North = 0,
		South,
		East,
		West
	};
}

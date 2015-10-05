struct LightState_1;
struct LightState_2;
struct LightState_3;
struct LightState_4;
struct LightState_5;
struct LightState_6;

/*
* NorthSouth: Red
* EastWest: Red
*/
struct InitialRR : sc::state<InitialRR, NormalExecution>
{
	//Entry
	InitialRR(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl;
		std::cout << "Initial state." << std::endl << "North-South: Red" << std::endl << "East-West: Red" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(wait_));

		//Nort-South have had green, thus its East-West's turn
		if (context<NormalExecution>().previous_state_ == LS_2 || context<NormalExecution>().previous_state_ == NORMALEXECUTION)
		{
			std::cout << " *EVENT* " << "PostEvent ToSL6"<< std::endl;
			post_event(EvToLS6());
		}
		else
		{
			std::cout << " *EVENT* " << "PostEvent ToSL3" << std::endl;
			post_event(EvToLS3());
		}
	}

	//Exit
	~InitialRR()
	{
		context<NormalExecution>().previous_state_ = INITIALRR;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS6, LightState_6>, sc::transition<EvToLS3, LightState_3> > reactions;
};

/*
* NorthSouth: Green
* EastWest: Red
*/
struct LightState_1 : sc::state<LightState_1, NormalExecution>
{
	//Entry
	LightState_1(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "LightState _1." << std::endl << "North-South: Green" << std::endl << "East-West: Red" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(wait_));

		std::cout << " *EVENT* " << "PostEvent ToSL2" << std::endl;
		post_event(EvToLS2());
	}

	//Exit
	~LightState_1()
	{
		context<NormalExecution>().previous_state_ = LS_1;
	}

	//Events
	typedef sc::transition<EvToLS2, LightState_2> reactions;
};

/*
* NorthSouth: Yellow
* EastWest: Red
*/
struct LightState_2 : sc::state<LightState_2, NormalExecution>
{
	//Entry
	LightState_2(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "LightState_2" << std::endl << "North-South: Yellow" << std::endl << "East-West: Red" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::seconds(wait_));

		std::cout << " *EVENT* " << "PostEvent EvToRR" << std::endl;
		post_event(EvToRR());
	}

	//Exit
	~LightState_2()
	{
		context<NormalExecution>().previous_state_ = LS_2;
	}

	//Events
	typedef sc::transition<EvToRR, InitialRR> reactions;
};

/*
* NorthSouth: RedYellow
* EastWest: Red
*/
struct LightState_3 : sc::state<LightState_3, NormalExecution>
{
	//Entry
	LightState_3(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "LightState_3" << std::endl << "North-South: RedYellow" << std::endl << "East-West: Red" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::seconds(wait_));

		std::cout << " *EVENT* " << "PostEvent ToSL1" << std::endl;
		post_event(EvToLS1());
	}

	//Exit
	~LightState_3()
	{
		context<NormalExecution>().previous_state_ = LS_3;
	}

	//Events
	typedef sc::transition<EvToLS1, LightState_1> reactions;
};

/*
* NorthSouth: Red
* EastWest: Green
*/
struct LightState_4 : sc::state<LightState_4, NormalExecution>
{
	//Entry
	LightState_4(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "LightState_4" << std::endl << "North-South: Red" << std::endl << "East-West: Green" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::seconds(wait_));

		std::cout << " *EVENT* " << "PostEvent ToSL5" << std::endl;
		post_event(EvToLS5());
	}

	//Exit
	~LightState_4()
	{
		context<NormalExecution>().previous_state_ = LS_4;
	}

	//Events
	typedef sc::transition<EvToLS5, LightState_5> reactions;
};

/*
* NorthSouth: Red
* EastWest: Yellow
*/
struct LightState_5 : sc::state<LightState_5, NormalExecution>
{
	//Entry
	LightState_5(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "LightState_5" << std::endl << "North-South: Red" << std::endl << "East-West: Yellow" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::seconds(wait_));

		std::cout << " *EVENT* " << "PostEvent EvToRR" << std::endl;
		post_event(EvToRR());
	}

	//Exit
	~LightState_5()
	{
		context<NormalExecution>().previous_state_ = LS_5;
	}

	//Events
	typedef sc::transition<EvToRR, InitialRR> reactions;
};

/*
* NorthSouth: Red
* EastWest: RedYellow
*/
struct LightState_6 : sc::state<LightState_6, NormalExecution>
{
	//Entry
	LightState_6(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "LightState_6" << std::endl << "North-South: Red" << std::endl << "East-West: RedYellow" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(wait_)); 

		std::cout << " *EVENT* " << "PostEvent ToSL4" << std::endl;
		post_event(EvToLS4());
	}

	//Exit
	~LightState_6()
	{
		context<NormalExecution>().previous_state_ = LS_6;
	}

	//Events
	typedef sc::transition<EvToLS4, LightState_4> reactions;
};


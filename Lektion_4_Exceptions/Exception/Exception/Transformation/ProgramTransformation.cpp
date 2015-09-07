// ProgramTransformation.cpp : Defines the entry point for the console application.
//

#include <iostream>
using namespace std;

#include "logfile.h"

int main()
{
	try
	{
		LogFile dummy;
		dummy.write("Writing from main");
	}
	catch (const LogFileException& ex)
	{
		cerr << ex.what();
	}
	
	try
	{
		LogFile logFile("Exceptions.log");
		logFile.write("Writing from main");
		logFile.write("Writing from main - again");
	}
	catch (const LogFileException& ex)
	{
		cerr << ex.what();
	}
	
	getchar();
}

/*
if( logFile.getState() != LogFile::es_OK )
{
switch( logFile.getState() )
{
case LogFile::es_OK:
break;
case LogFile::es_NOT_INITIALIZED:
cerr << "LogFile not initialized" << endl;
break;
case LogFile::es_COULD_NOT_OPEN_FILE:
cerr << "Could not open LogFile" << endl;
break;
case LogFile::es_FILENAME_IS_EMPTY:
cerr << "LogFile filename is empty" << endl;
break;
}
}
else if(!logFile.write( "Writing from main" ) ) //When readonly fails in internalwrite which returns false and sets state to error.
{
switch( logFile.getState() )
{
case LogFile::es_OK:
break;
case LogFile::es_NOT_INITIALIZED:
cerr << "LogFile not initialized" << endl;
break;
case LogFile::es_COULD_NOT_OPEN_FILE:
cerr << "Could not open LogFile" << endl;
break;
case LogFile::es_FILENAME_IS_EMPTY:
cerr << "LogFile filename is empty" << endl;
break;
}
}

if(logFile.getState() == LogFile::es_OK)
if( !logFile.write( "Writing from main - again" ) )
{
switch( logFile.getState() )
{
case LogFile::es_OK:
break;
case LogFile::es_NOT_INITIALIZED:
cerr << "LogFile not initialized" << endl;
break;
case LogFile::es_COULD_NOT_OPEN_FILE:
cerr << "Could not open LogFile" << endl;
break;
case LogFile::es_FILENAME_IS_EMPTY:
cerr << "LogFile filename is empty" << endl;
break;
}
}
*/
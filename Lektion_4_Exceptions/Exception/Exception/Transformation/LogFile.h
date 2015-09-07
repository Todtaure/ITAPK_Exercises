
#if !defined(_LOGFILE_H_)
#define _LOGFILE_H_

#include <string>
#include <stdexcept>

class LogFile
{
public:
	void write( const std::string& data );
	void init( const std::string& logfilename );    

	LogFile();
	explicit LogFile( const std::string& logfilename );
	~LogFile();

private:
	std::string logfilename_;
	void internalWrite( const std::string& data );
};

/*
Exception class
*/

class LogFileException : public std::runtime_error
{
public:
	enum EState { NOT_INITIALIZED, COULD_NOT_OPEN_FILE };

	explicit LogFileException(EState state, const std::string& str = "")
		: runtime_error(str), state_(state) {}

	EState getState() { return state_; }
private:
	EState state_;
};

#endif // !defined(AFX_LOGFILE_H__4A9D73D5_7588_11D3_A2E2_00104BB8D7A4__INCLUDED_)

/*
Using exceptions, it gets easier to call the code above, since you do not have to look at return values and
then the error codes. We simply put potentially error prone code into a try/catch and, when an exception occurs,
we write this to the console.
*/
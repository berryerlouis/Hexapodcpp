#pragma once



namespace Core {
struct CoreStatus
{
	static const int CORE_ERROR = ( -1 );
	static const int CORE_OK    = ( 0 );

	CoreStatus( int state ) : status( state )
	{
	}

	operator bool() const {
		return ( status == CORE_OK );
	}
private:
	int              status = CORE_ERROR;
};

class Utils
{
public:
};
}

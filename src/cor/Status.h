#pragma once



namespace Core {
enum CoreStatus
{
	CORE_ERROR = (-1),                                                                                                     //!< Generic Network Error
	CORE_OK    = (0),                                                                                                      //!< Network no error
};

class Utils
{
public:
	inline static bool CoreStatusToBool (const CoreStatus status)
	{
		return (status == CORE_OK);
	}

	inline static CoreStatus BoolToCoreStatus (const bool status)
	{
		return ( (status == true) ? CORE_OK : CORE_ERROR);
	}
};
}

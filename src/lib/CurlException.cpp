/**
 * @file
 * @brief Definition of CurlException methods.
 * @date 2017-01-07 [JFDR] Created.
 */

#include <ios>
#include <string>
#include <sstream>
#include "CurlException.hpp"

namespace AbcdEFramework
{
    namespace Web
    {
        using std::runtime_error;
        using std::string;
        using std::stringstream;

        CurlException::CurlException(const string& what_arg)
            :   runtime_error(what_arg),
                _errorCode((CURLcode)-1)
        {
        }

        CurlException::CurlException(const char* what_arg)
            :   runtime_error(what_arg),
                _errorCode((CURLcode)-1)
        {
        }

        CurlException::CurlException(const string& what_arg, CURLcode errorCode, const string& message)
            :   runtime_error(FormatErrorMessage(what_arg, errorCode, message)),
                _errorCode(errorCode)
        {
        }

        string CurlException::FormatErrorMessage(const string& what_arg, CURLcode errorCode, const string& message)
        {
            stringstream ss;
            ss << what_arg << " :: CURLcode=[" << errorCode << "]" << message;
            return ss.str();
        }
    } // namespace Web
} // namespace AbcdEFramework


/**
 * @file
 * @brief Declaration of the CurlException class.
 * @date 2017-01-07 [JFDR] Created.
 */
#if !defined CURL_EXCEPTION_67AC882F4E9645AC891475F9D4467B68
#define CURL_EXCEPTION_67AC882F4E9645AC891475F9D4467B68 1

#include <stdexcept>
#include <curl/curl.h>

namespace AbcdEFramework
{
    namespace Web
    {
        class CurlException : public std::runtime_error
        {
            private:
                CURLcode _errorCode; ///< The error code that was passed to the constructor.

            public:
                /**
                 * @brief Construct an object with an explanatory string.
                 * @param what_arg Description.
                 */
                explicit CurlException(const std::string& what_arg);

                /**
                 * @brief Construct an object with an explanatory string.
                 * @param what_arg Description.
                 */
                explicit CurlException(const char* what_arg);

                /**
                 * @brief Construct an object with an explanatory string and an error message.
                 * @param what_arg Description.
                 * @param errorCode Error code that was returned.
                 * @param message Error message that was returned by cURL.
                 */
                explicit CurlException(const std::string& what_arg, CURLcode errorCode, const std::string& message);

            public:
                /**
                 * @brief Get the error code that was passed to the constructor.
                 * @return The method returns an error code, or (CURLcode)-1 if a constructor was used that does
                 *      not accept an error code.
                 */
                inline CURLcode GetErrorCode() const;

            private:
                /**
                 * @brief Create an error message from the individual parameters passed to the constructor.
                 * @param what_arg Description.
                 * @param errorCode Error code that was returned.
                 * @param message Error message that was returned by cURL.
                 */
                static std::string FormatErrorMessage(const std::string& what_arg, CURLcode errorCode, const std::string& message);
        };

        inline CURLcode CurlException::GetErrorCode() const
        {
            return this->_errorCode;
        }
    } // namespace Web
} // namespace AbcdEFramework
#endif // CURL_EXCEPTION_67AC882F4E9645AC891475F9D4467B68

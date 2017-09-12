/**
 * @file
 * @brief Declaration of the CurlEasyWrapper class.
 * @date 2017-01-07 [JFDR] Created.
 */
#if !defined CURL_EASY_WRAPPER_67AC882F4E9645AC891475F9D4467B68
#define CURL_EASY_WRAPPER_67AC882F4E9645AC891475F9D4467B68 1

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>
#include <curl/curl.h>
#include "CurlException.hpp"

// Expands into a method declaration
#define AEF_BOOL_METHOD_DECL(METHOD_NAME, FLAG_DEFAULT) inline void METHOD_NAME(bool flag = FLAG_DEFAULT);

// Expands into a method body that calls SetOpt()
#define AEF_BOOL_METHOD_DEF(METHOD_NAME, CURL_OPT) inline void CurlEasyWrapper::METHOD_NAME(bool flag) { SetOpt(CURL_OPT, flag ? 1l : 0l); }


namespace AbcdEFramework
{
    namespace Web
    {
        class CurlSList;

        /**
         * @brief Wrapper around the cURL easy interface.
         */
        class CurlEasyWrapper
        {
            private:
                CURL* _curlHandle; ///< Handle to the ession.
                std::vector<char> _errorMsgBuffer; ///< Buffer where exception messages are dumped.
                std::vector<unsigned char> _receiveBuffer; ///< Buffer where data received from cURL is stored.
                std::string _agent; ///< Copy of the agent string;
                std::string _url; ///< Copy of the URL
                std::string _postData; ///< Copy of the data given to the \c PostFields() method.
                std::unique_ptr<CurlSList> _slistPtr; ///< Pointer to an instance of \c CurlSList.

            public:
                /**
                 * @brief Default constructor.
                 */
                CurlEasyWrapper();

                /**
                 * @brief Destructor.
                 */
                ~CurlEasyWrapper();

            public:
                /**
                 * @brief Clears the contents of the receive buffer.
                 */
                inline void ClearReceiveBuffer();

                /**
                 * @brief Execute the command that was constructed.
                 */
                void Execute();

                /**
                 * @brief Get a reference to the receive buffer.
                 * @return Returns a reference to the buffer where we write data that is received.
                 */
                inline const std::vector<unsigned char>& GetReceiveBuffer() const;

                /**
                 * @brief Move the data out of the receive buffer and return it to the caller.
                 * @return A vector that contains the data that was in the receive buffer.
                 */
                inline std::vector<unsigned char> MoveReceiveBufferData();

                /**
                 * @brief Reset the connection to its initial state.
                 */
                void Reset();

            public:
                /**
                 * @brief Operator to get a reference to the receive buffer;
                 */
                inline operator const std::vector<unsigned char>&() const;

            public:
                /**
                 * @brief Number of seconds to wait before a transfer times out.
                 * @param timeoutSeconds Number of seconds to wait before timing out. A zero value sets the timeout
                 *      value to indefinite.
                 */
                inline void TimeoutSeconds(long timeoutSeconds);

                /**
                 * @brief Number of milliseconds to wait before a transfer times out.
                 * @param timeoutMilliseconds Number of milliseconds to wait before timing out. A zero value sets the timeout
                 *      value to indefinite.
                 */
                inline void TimeoutMSeconds(long timeoutMilliseconds);

                /**
                 * @brief Report HTTP errors as failures.
                 * @param failOnErrors If set then HTTP errors are reported. If clear, no HTTP errors are reported.
                 */
                inline void FailOnHttpErrors(bool failOnErrors);

                /**
                 * @brief Set upload or download mode.
                 * @param upload If \c true then upload mode is selected. If \c false then download mode is selected.
                 * @remark The default mode is download.
                 */
                inline void Upload(bool upload = true);

                /**
                 * @brief Set the URL.
                 * @param url The url to use.
                 */
                inline void Url(const std::string& url);

                /**
                 * @brief Set the user agent string.
                 * @param agent The value of the user agent.
                 */
                inline void UserAgent(const std::string& agent);

                /**
                 * @brief Switch verbose output mode on or off.
                 * @param verboseMode If \c true we use verbose mode. If \c false verbose mode is disabled.
                 */
                inline void Verbose(bool verboseMode = true);

                /**
                 * @brief Reset custom HTTP headers.
                 */
                inline void ResetHttpHeader();

            public:
                /**
                 * @brief Data to be posted to the server.
                 * @param fieldData POST data.
                 */
                void PostFields(const std::string& fieldData);

                /**
                 * @brief Set custom HTTP headers.
                 * @param slistPtr Pointer to an \c CurlSList instance
                 */
                void HttpHeader(std::unique_ptr<CurlSList>& slistPtr);

                /**
                 * @brief Calls \c curl_easy_escape() to convert the input string to an escaped string.
                 */
                std::string Escape(const std::string& inputStr);

                /**
                 * @brief Set the size of the data in a POST
                 */
                 void PostFieldSize(size_t postSize);

            public:
                /**
                 * @brief Use POST to send data to a server.
                 */
                AEF_BOOL_METHOD_DECL(Post, true);

                AEF_BOOL_METHOD_DECL(NoBody, true);


            private:
                /**
                 * @brief Clears the \c _errorMsgBuffer.
                 */
                inline void ClearErrorMessageBuffer();

                /**
                 * @brief Callback method to write data received from cURL when executing a call.
                 */
                static size_t CurlWriteDataProc(void* contents, size_t size, size_t nmemb, void *userp);

                /**
                 * @brief Converts the contents of the \c _errorMsgBuffer to a string.
                 * @param errorCode The error code that was received in case a generic error message must be constructed.
                 * @return Returns a string object with the error message harvested from the \c _errorMsgBuffer
                 */
                std::string RetrieveErrorMessage(CURLcode errorCode) const;

                /**
                 * @brief Helper to set a cURL option.
                 * @param option Option identifier.
                 * @param optionData Data to set.
                 */
                void SetOpt(CURLoption option, const std::string& optionData);

                /**
                 * @brief Helper to set a cURL option.
                 * @param option Option identifier.
                 * @param optionData Data to set.
                 */
                void SetOpt(CURLoption option, bool optionData);

                /**
                 * @brief Helper to set a cURL option.
                 * @param option Option identifier.
                 * @param optionData Data to set.
                 */
                void SetOpt(CURLoption option, long optionData);

                /**
                 * @brief Helper to set a cURL option that requires a pointer to a data structure.
                 * @param option Option identifier.
                 * @param optionData Data to set.
                 */
                void SetOpt(CURLoption option, void* optionData);
        }; // class CurlEasyWrapper

        inline void CurlEasyWrapper::ClearErrorMessageBuffer()
        {
            _errorMsgBuffer[0] = '\0';
        }

        inline void CurlEasyWrapper::ClearReceiveBuffer()
        {
            this->_receiveBuffer.clear();
            this->_receiveBuffer.shrink_to_fit();
        }

        inline CurlEasyWrapper::operator const std::vector<unsigned char>&() const
        {
            return this->_receiveBuffer;
        }

        inline const std::vector<unsigned char>& CurlEasyWrapper::GetReceiveBuffer() const
        {
            return this->_receiveBuffer;
        }

        inline std::vector<unsigned char> CurlEasyWrapper::MoveReceiveBufferData()
        {
            return std::move(this->_receiveBuffer);
        }

        inline void CurlEasyWrapper::Url(const std::string& url)
        {
            this->_url = url;
            SetOpt(CURLOPT_URL, this->_url);
        }

        inline void CurlEasyWrapper::UserAgent(const std::string& agent)
        {
            this->_agent = agent;
            SetOpt(CURLOPT_USERAGENT, this->_agent);
        }

        inline void CurlEasyWrapper::Upload(bool upload)
        {
            SetOpt(CURLOPT_UPLOAD, upload);
        }

        inline void CurlEasyWrapper::TimeoutSeconds(long timeoutSeconds)
        {
            SetOpt(CURLOPT_TIMEOUT, timeoutSeconds);
        }

        inline void CurlEasyWrapper::TimeoutMSeconds(long timeoutMilliseconds)
        {
            SetOpt(CURLOPT_TIMEOUT_MS, timeoutMilliseconds);
        }

        inline void CurlEasyWrapper::FailOnHttpErrors(bool failOnErrors)
        {
            SetOpt(CURLOPT_FAILONERROR, failOnErrors ? 1l : 0l);
        }

        inline void CurlEasyWrapper::Verbose(bool verboseMode)
        {
            SetOpt(CURLOPT_VERBOSE, verboseMode ? 1l : 0l);
        }

        inline void CurlEasyWrapper::ResetHttpHeader()
        {
            SetOpt(CURLOPT_HTTPHEADER, nullptr);
        }

        AEF_BOOL_METHOD_DEF(Post, CURLOPT_POST)
        AEF_BOOL_METHOD_DEF(NoBody, CURLOPT_NOBODY)

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CurlSList
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Wrapper for a \c curl_slist linked list.
         */
        class CurlSList
        {
            private:
                curl_slist* mSList; ///< Pointer to the curl_slist.

            public:
                /**
                 * @brief Construct an empty curl_slist
                 */
                CurlSList();

                /**
                 * @brief Move constructor.
                 */
                CurlSList(CurlSList&& src);

                /**
                 * @brief Construct an slist with only a single entry.
                 */
                CurlSList(const std::string& listEntry);

                /**
                 * @brief Construct an slist with multiple entries.
                 * @param listEntries Collection of \c string objects to add to the container.

                 */
                CurlSList(std::initializer_list<std::string> listEntries);

                /**
                 * @brief Destructor.
                 */
                ~CurlSList() noexcept;

            public:
                /**
                 * @brief Return a pointer to the contained \c curl_slist.
                 */
                inline operator const curl_slist*() const;

                /**
                 * @brief Move assignment operator
                 */
                CurlSList& operator=(CurlSList&& src);

                /**
                 * @brief Add another string to the list.
                 */
                CurlSList& operator+=(const std::string& listEntry);

            public:
                /**
                 * @brief Append another item to the list.
                 */
                 void Append(const std::string& listEntry);

            private:
                /**
                 * @brief Copy constructor is deleted
                 */
                CurlSList(const CurlSList& src) = delete;

                /**
                 * @brief Copy assignment operator is deleted
                 */
                CurlSList& operator=(const CurlSList& src) = delete;
        };

        inline CurlSList::operator const curl_slist*() const
        {
            return this->mSList;
        }

    } // namespace Web

} // namespace AbcdEFramework

#endif // CURL_EASY_WRAPPER_67AC882F4E9645AC891475F9D4467B68

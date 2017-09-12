/**
 * @file
 * @brief Definition of the CurlEasyWrapper methods.
 * @date 2017-01-07 [JFDR] Created.
 * @date 2017-01-09 [JFDR] Must set the CURLOPT_NOSIGNAL option otherwise the program aborts with "longjmp causes uninitialized stack frame".
 */

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <string.h>
#include "CurlEasyWrapper.hpp"

#ifdef __GNUC__
    #define AEF_METHOD_NAME __PRETTY_FUNCTION__
#elif _MSC_VER
    #define AEF_METHOD_NAME __FUNCSIG__
#else
    #error "C++ compiler signature not recognised."
#endif

namespace AbcdEFramework
{
    namespace Web
    {
        using std::max;
        using std::min;
        using std::runtime_error;
        using std::string;
        using std::vector;

        CurlEasyWrapper::CurlEasyWrapper()
            :   _curlHandle(curl_easy_init())
        {
            if (nullptr == _curlHandle)
            {
                throw runtime_error(AEF_METHOD_NAME);
            }

            // Set the size of the error message buffer.
            this->_errorMsgBuffer.reserve((size_t)CURL_ERROR_SIZE);

            // Return code from calls to cURL
            CURLcode curlRes;

            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, CURLOPT_NOSIGNAL, 1)))
            {
                throw CurlException(string(AEF_METHOD_NAME) + "curl_easy_setopt(CURLOPT_NOSIGNAL)", curlRes, "");
            }

            // Point the error buffer to our local buffer so that we can get error messages.
            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, CURLOPT_ERRORBUFFER, this->_errorMsgBuffer.data())))
            {
                throw CurlException(string(AEF_METHOD_NAME) + "curl_easy_setopt(CURLOPT_ERRORBUFFER)", curlRes, "");
            }

            // Some servers don't like requests that are made without a user-agent, so we provide one.
            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0")))
            {
                throw CurlException(string(AEF_METHOD_NAME) + "curl_easy_setopt(CURLOPT_USERAGENT)", curlRes, RetrieveErrorMessage(curlRes));
            }

            // Change the default behaviour so that data will be written to our receive buffer.
            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, CURLOPT_WRITEFUNCTION, CurlEasyWrapper::CurlWriteDataProc)))
            {
                throw CurlException(string(AEF_METHOD_NAME) + "curl_easy_setopt(CURLOPT_WRITEFUNCTION)", curlRes, RetrieveErrorMessage(curlRes));
            }

            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, CURLOPT_WRITEDATA, (void*)&(this->_receiveBuffer))))
            {
                throw CurlException(string(AEF_METHOD_NAME) + "curl_easy_setopt(CURLOPT_WRITEDATA)", curlRes, RetrieveErrorMessage(curlRes));
            }
        }

        CurlEasyWrapper::~CurlEasyWrapper()
        {
            if (_curlHandle != nullptr)
            {
                curl_easy_cleanup(this->_curlHandle);
                this->_curlHandle = nullptr;
            }
        }

        void CurlEasyWrapper::PostFields(const std::string& fieldData)
        {
            this->_postData = fieldData;
            SetOpt(CURLOPT_POSTFIELDS, this->_postData);
        }

        void CurlEasyWrapper::HttpHeader(std::unique_ptr<CurlSList>& slistPtr)
        {
            this->_slistPtr = std::move(slistPtr);
            const curl_slist* listPtr = this->_slistPtr->operator const curl_slist*();
            SetOpt(CURLOPT_HTTPHEADER, listPtr);
        }

        string CurlEasyWrapper::Escape(const std::string& inputStr)
        {
            char* escapedStrPtr(curl_easy_escape(this->_curlHandle, inputStr.c_str(), (int)inputStr.size()));
            if (escapedStrPtr == nullptr)
            {
                throw CurlException(AEF_METHOD_NAME);
            }

            string result(escapedStrPtr);
            curl_free(escapedStrPtr);
            escapedStrPtr = nullptr;

            return std::move(result);
        }

        void CurlEasyWrapper::Execute()
        {
            CURLcode curlRes;
            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_perform(this->_curlHandle)))
            {
                throw CurlException(__PRETTY_FUNCTION__, curlRes, RetrieveErrorMessage(curlRes));
            }
        }

        void CurlEasyWrapper::Reset()
        {
            curl_easy_reset(this->_curlHandle);
            this->_slistPtr.release();
            this->_postData.clear();
        }

        std::string CurlEasyWrapper::RetrieveErrorMessage(CURLcode errorCode) const
        {
            size_t len = min(this->_errorMsgBuffer.size(), ::strlen(this->_errorMsgBuffer.data()));
            if (len > 0u)
            {
                return string(this->_errorMsgBuffer.data(), len);
            }

            return string(curl_easy_strerror(errorCode));
        }

        void CurlEasyWrapper::SetOpt(CURLoption option, const std::string& optionData)
        {
            // Return code from calls to cURL
            CURLcode curlRes;
            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, option, optionData.c_str())))
            {
                throw CurlException
                (
                    AEF_METHOD_NAME,
                    curlRes,
                    RetrieveErrorMessage(curlRes)
                );
            }
        }

        void CurlEasyWrapper::SetOpt(CURLoption option, bool optionData)
        {
            // Return code from calls to cURL
            CURLcode curlRes;
            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, option, optionData ? (long)1 : (long)0)))
            {
                throw CurlException
                (
                    AEF_METHOD_NAME,
                    curlRes,
                    RetrieveErrorMessage(curlRes)
                );
            }
        }

        void CurlEasyWrapper::SetOpt(CURLoption option, long optionData)
        {
            // Return code from calls to cURL
            CURLcode curlRes;
            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, option, optionData)))
            {
                throw CurlException
                (
                    AEF_METHOD_NAME,
                    curlRes,
                    RetrieveErrorMessage(curlRes)
                );
            }
        }

        void CurlEasyWrapper::SetOpt(CURLoption option, void* optionData)
        {
            // Return code from calls to cURL
            CURLcode curlRes;
            ClearErrorMessageBuffer();
            if (CURLE_OK != (curlRes = curl_easy_setopt(this->_curlHandle, option, optionData)))
            {
                throw CurlException
                (
                    AEF_METHOD_NAME,
                    curlRes,
                    RetrieveErrorMessage(curlRes)
                );
            }
        }

        size_t CurlEasyWrapper::CurlWriteDataProc(void* contents, size_t size, size_t nmemb, void *userp)
        {
            size_t processedSizeBytes = (size * nmemb);

            vector<unsigned char>* bufferPtr = reinterpret_cast<vector<unsigned char>*>(userp);
            unsigned char* sourcePtr = reinterpret_cast<unsigned char*>(contents);
            bufferPtr->insert(bufferPtr->end(), sourcePtr, sourcePtr + processedSizeBytes);

            return processedSizeBytes;
        }

        void CurlEasyWrapper::PostFieldSize(size_t postSize)
        {
            if (postSize <= ((size_t)2 * (size_t)(1024 * 1024 * 1024)))
            {
                SetOpt(CURLOPT_POSTFIELDSIZE, (long)postSize);
            }
            else
            {
                SetOpt(CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)postSize);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CurlSList
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CurlSList::CurlSList()
            :   mSList(curl_slist_append(mSList, nullptr))
        {
            // Check if the list was initialised
            if (mSList == nullptr)
            {
                throw CurlException(AEF_METHOD_NAME);
            }
        }

        CurlSList::CurlSList(CurlSList&& src)
            :   mSList(src.mSList)
        {
            src.mSList = nullptr;
        }

        CurlSList::CurlSList(const std::string& listEntry)
            :   mSList(nullptr)
        {
            // Attempt to add a new element
            if (nullptr == ::curl_slist_append(mSList, listEntry.c_str()))
            {
                // Failed. Free the list and raise an exception.
                curl_slist_free_all(mSList);
                mSList = nullptr;
                throw CurlException(AEF_METHOD_NAME);
            }
        }

        CurlSList::CurlSList(std::initializer_list<std::string> listEntries)
            :   mSList(nullptr)
        {
            //mSList = curl_slist_append(mSList, nullptr);
            // Check if the list was initialised
            //if (mSList == nullptr)
            //{
            //    throw CurlException(AEF_METHOD_NAME);
            //}

            // Attempt to add elements
            for (std::string element : listEntries)
            {
                if (nullptr == ::curl_slist_append(mSList, element.c_str()))
                {
                    // Failed. Free the list and raise an exception.
                    if (mSList != nullptr)
                    {
                        curl_slist_free_all(mSList);
                        mSList = nullptr;
                    }
                    throw CurlException(AEF_METHOD_NAME);
                }
            }
        }

        CurlSList::~CurlSList() noexcept
        {
            if (mSList != nullptr)
            {
                curl_slist_free_all(mSList);
                mSList = nullptr;
            }
        }

        CurlSList& CurlSList::operator=(CurlSList&& src)
        {
            // Delete any previous list.
            if (mSList != nullptr)
            {
                curl_slist_free_all(mSList);
                mSList = nullptr;
            }

            // Grab the list from our rival.
            mSList = src.mSList;
            src.mSList = nullptr;

            return *this;
        }

        CurlSList& CurlSList::operator+=(const std::string& listEntry)
        {
            if (nullptr == ::curl_slist_append(mSList, listEntry.c_str()))
            {
                throw CurlException(AEF_METHOD_NAME);
            }

            return *this;
        }

        void CurlSList::Append(const std::string& listEntry)
        {
            if (nullptr == ::curl_slist_append(mSList, listEntry.c_str()))
            {
                throw CurlException(AEF_METHOD_NAME);
            }
        }
    } // namespace Web
} // namespace AbcdEFramework

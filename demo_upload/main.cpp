#include <memory>
#include <cstring>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <json.hpp>
#include <CurlEasyWrapper.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::string;
using std::stringstream;
using std::unique_ptr;
using AbcdEFramework::Web::CurlException;
using AbcdEFramework::Web::CurlEasyWrapper;
using AbcdEFramework::Web::CurlSList;
using json = nlohmann::json;

int main (int argc, char* argv[])
{
    bool showHelp(false);
    if (argc != 3)
    {
        showHelp = true;
    }
    else
    {
        for (int argIndex = 1; argIndex < argc; ++argIndex)
        {
            if
            (
                (strcmp(argv[argIndex], "--help") == 0) ||
                (strcmp(argv[argIndex], "-h") == 0)
            )
            {
                showHelp = true;
                break;
            }
        }
    }

    if (showHelp)
    {
        cout << endl << "USAGE: " << argv[0] << " <dest-url> <number> " << endl;
        cout << "WHERE dest-url URL of destination server." << endl;
        cout << "      number Value to upload in the range [0..9999]" << endl << endl;
        return 0;
    }

    try
    {
        curl_global_init(CURL_GLOBAL_ALL);

        CurlEasyWrapper curl;

        unique_ptr<CurlSList> sListPtr
        (
            new CurlSList
            (
                {
                    "Accept: application/json",
                    "Content-Type: application/json",
                    "charsets: utf-8"
                }
            )
        );

        json uploadValue =
        {
            { "UploadValue", argv[2] }
        };

        stringstream ss;
        ss << "UploadValue=" << uploadValue;
        string jsonString(ss.str());

        curl.Url(argv[1]);
        curl.TimeoutSeconds(30);
        curl.Upload(false);
        curl.Post();
        curl.PostFields(jsonString);
        curl.PostFieldSize(jsonString.size());
        curl.HttpHeader(sListPtr);
        curl.Execute();
    }
    catch (CurlException& ex)
    {
        cerr << ex.what() << endl;
    }
    catch (exception& ex)
    {
        cerr << ex.what() << endl;
    }
    catch (...)
    {
        cerr << "An unknown exception was caught" << endl;
    }

    curl_global_cleanup();
    return 0;
}

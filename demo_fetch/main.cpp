#include <cstring>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <iostream>
#include <json.hpp>
#include <CurlEasyWrapper.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::string;
using AbcdEFramework::Web::CurlException;
using AbcdEFramework::Web::CurlEasyWrapper;
using json = nlohmann::json;

int main (int argc, char* argv[])
{
    bool showHelp = false;
    if (argc != 2)
    {
        showHelp = true;
    }
    else
    {
        if
        (
            (strcmp(argv[1], "--help") == 0) ||
            (strcmp(argv[1], "-h") == 0)
        )
        {
            showHelp = true;
        }
    }

    if (showHelp)
    {
        cout << endl << "USAGE: " << argv[0] << "<url>" << endl;
        cout << "WHERE url URL of the server from where to fetch the data." << endl << endl;
        return 0;
    }

    try
    {
        CurlEasyWrapper curl;
        curl.Upload(false);
        curl.Url(argv[1]);
        curl.Execute();
        string receivedData(curl.GetReceiveBuffer().begin(), curl.GetReceiveBuffer().end());

        json js = json::parse(receivedData);
        if (js["errorMessage"].is_null())
        {
            cout << "Previous value was [" << js["inputValue"] << "]" << endl;
        }
        else
        {
            cout << "Server reported an error: "  << js["errorMessage"] << endl;
        }
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

    return 0;
}

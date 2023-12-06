#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <fstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

#include <json/json.h>

#include "request.hpp"

using namespace std;
Json::Value game::HTTP::get(string url)
{
    try
    {
        curlpp::Cleanup myCleanup;
        {
            ostringstream os;
            list<string> headers;
            headers.push_back("User-Agent: UCSB-CS32-GALAGA");
            curlpp::options::Url myUrl(url);
            curlpp::options::HttpHeader myHeaders(headers);
            curlpp::options::WriteStream ws(&os);
            curlpp::Easy myRequest;
            myRequest.setOpt(myUrl);
            myRequest.setOpt(myHeaders);
            myRequest.setOpt(ws);
            myRequest.setOpt<curlpp::options::WriteStream>(&os);

            myRequest.perform();

            int code = curlpp::infos::ResponseCode::get(myRequest);
            if (code != 200)
            {
                cout << "[fatal] Request failed with a code of " << code << endl;
                exit(2);
            }

            Json::Value root;
            Json::Reader reader;
            bool parsingSuccessful = reader.parse(os.str(), root);
            if (!parsingSuccessful)
            {
                cout << "Error parsing the JSON RESP" << endl;
            }
            return root;
        }
    }
    catch (curlpp::RuntimeError &e)
    {
        cout << e.what() << endl;
    }
    catch (curlpp::LogicError &e)
    {
        cout << e.what() << endl;
    }
    return Json::Value();
}

Json::Value game::HTTP::post(string url, Json::Value data)
{
    std::cout << "POSTING" << std::endl;
    try
    {
        curlpp::Cleanup myCleanup;
        {
            ostringstream os;
            list<string> headers;
            string date_str = data.toStyledString();
            headers.push_back("User-Agent: UCSB-CS32-GALAGA");
            headers.push_back("Content-Type: application/json");
            curlpp::options::Url myUrl(url);
            curlpp::options::HttpHeader myHeaders(headers);
            curlpp::options::PostFields myPostFields(date_str);
            curlpp::options::PostFieldSize myPostFieldSize(date_str.length());
            curlpp::options::WriteStream ws(&os);
            curlpp::Easy myRequest;
            myRequest.setOpt(myUrl);
            myRequest.setOpt(myHeaders);
            myRequest.setOpt(myPostFields);
            myRequest.setOpt(myPostFieldSize);
            myRequest.setOpt(ws);
            myRequest.setOpt<curlpp::options::WriteStream>(&os);

            myRequest.perform();

            int code = curlpp::infos::ResponseCode::get(myRequest);
            if (code != 200)
            {
                cout << "[fatal] Request failed with a code of " << code << endl;
                exit(2);
            }

            Json::Value root;
            Json::Reader reader;
            bool parsingSuccessful = reader.parse(os.str(), root);
            if (!parsingSuccessful)
            {
                cout << "Error parsing the JSON RESP" << endl;
            }
            return root;
        }
    }
    catch (curlpp::RuntimeError &e)
    {
        cout << e.what() << endl;
    }
    catch (curlpp::LogicError &e)
    {
        cout << e.what() << endl;
    }
    return Json::Value();
}
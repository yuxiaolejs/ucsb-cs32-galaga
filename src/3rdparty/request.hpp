#ifndef REQ_H
#define REQ_H
#include <string>
#include <json/json.h>
namespace game::HTTP
{
    Json::Value get(std::string url);
    Json::Value post(std::string url, Json::Value data);
}
#endif
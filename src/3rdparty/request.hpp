#ifndef REQ_H
#define REQ_H
#include <string>
#include <json/json.h>
Json::Value request(std::string url, std::string token);
bool download(Json::Value item, std::string token);
#endif
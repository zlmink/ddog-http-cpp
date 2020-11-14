#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
//#include "DataDogClient.hpp"
#include <vector>
#include <map>
#include <string>

using namespace rapidjson;

struct Creator {
    std::string handle;
    uint64_t id;
    std::string name;
};

struct Notification {
    std::string handle;
    std::string name;
};


class DataDogMonitor {
    public:
        DataDogMonitor(Value& monitorJson);
        std::string status;
        std::vector<std::string> scopes;
        Creator creator;
        uint64_t overall_state_modified;
        std::vector<std::string> metrics;
        std::vector<Notification> notifications;
        uint64_t last_triggered_ts;
        std::string query;
        uint64_t id;
        std::string name;
        std::vector<std::string> tags;
        uint64_t org_id;
        //restricted roles
        std::string type;

        

    // private:
    //     DataDogClient * _client;

};
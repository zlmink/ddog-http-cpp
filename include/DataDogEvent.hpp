#pragma once
#include <stdio.h>
#include <stdlib.h>
//#include <uv.h>
//#include <curl/curl.h>
#include <cpr/cpr.h>
#include <cstdint>
//#include <cpr/ssl_options.h>
#include "DataDogMonitor.hpp"
#include "DataDogMetric.h"
#include <iostream>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class DataDogEvent {      
    public:
        DataDogEvent(Value& jsonData);
        uint64_t date_happened;
        std::string alert_type;
        bool is_aggregate;
        std::string title;
        std::string url;
        std::string text;
        std::vector<std::string> tags;
        std::vector<std::string> comments;
        std::string device_name;
        std::string priority;
        std::string source;
        std::string host;
        std::string resource;
        uint64_t id;

};
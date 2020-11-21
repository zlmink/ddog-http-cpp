#pragma once
#include <stdio.h>
#include <stdlib.h>
//#include <uv.h>
//#include <curl/curl.h>
#include <cpr/cpr.h>
#include <cstdint>
//#include <cpr/ssl_options.h>
#include "monitor.hpp"
#include "metric.hpp"
#include "event.hpp"
#include <iostream>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

namespace ddog_http {

class client {
    private:
      const char * apiKey;
      const char * appKey;
      
    public:
        client(const char* ddApiKey,const char* ddAppKey);
        void sendMetricSeries(metricSeries series,void(*cb)(bool,cpr::Response));
        void getMonitors(void(*cb)(bool,std::vector<monitor>,cpr::Response));
        void getEvents(uint64_t start_date, uint64_t end_date,void(*cb)(bool,std::vector<event>,cpr::Response));
        void checkApiKey();

};

}
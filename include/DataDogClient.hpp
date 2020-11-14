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
#include "DataDogEvent.hpp"
#include <iostream>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class DataDogClient {
    private:
      const char * apiKey;
      const char * appKey;
      
    public:
        DataDogClient(const char* ddApiKey,const char* ddAppKey);
        void sendMetricSeries(DataDogMetricSeries series,void(*cb)(bool,cpr::Response));
        void getMonitors(void(*cb)(bool,std::vector<DataDogMonitor>,cpr::Response));
        void getEvents(uint64_t start_date, uint64_t end_date,void(*cb)(bool,std::vector<DataDogEvent>,cpr::Response));
        void checkApiKey();

};
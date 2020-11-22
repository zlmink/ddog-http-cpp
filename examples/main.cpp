#include <curl/curl.h>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <sstream>
#include <unistd.h>

// #include "include/metric.hpp"
#include "ddog_http/client.hpp"

//namespace ddog_http = ddog_http;
//using namespace ddog_http

//------------------------------------------------------------------------------
uint64_t timeSinceEpochSec() {
  using namespace std::chrono;
  return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

std::string epochString(){
    uint64_t now = timeSinceEpochSec();
    std::ostringstream o;
    o << now;
    return o.str();
}


void testCb(){
    std::cout << "test callback" << std::endl;
}

//------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    //std::cout << epochString();
    std::string nowStr = epochString();
    //get datadog environment variables
    const char* ddApiKey = std::getenv("DD_API_KEY");
    const char* ddAppKey = std::getenv("DD_APPLICATION_KEY");

    if (ddApiKey == NULL){
        std::cerr << "Missing data dog api key";
        return EXIT_FAILURE;
    }

    if (ddAppKey == NULL){
        std::cerr << "Missing data dog application key";
        return EXIT_FAILURE;
    }

    // Check command line arguments.
    if(argc != 2)
    {
        std::cerr <<
            "Usage: ddog-cpp <body>\n" <<
            "Example:\n" <<
            "    http-client-async www.example.com 80 /\n" <<
            "    http-client-async www.example.com 80 / 1.0\n";
        return EXIT_FAILURE;
    }
    auto const body = argv[1];
    //int version = argc == 5 && !std::strcmp("1.0", argv[5]) ? 10 : 11;
    
    

    std::ostringstream bodyTest;
    ddog_http::point p (timeSinceEpochSec(),"5.465");
    std::vector<ddog_http::point> points;
    points.push_back(p);
    ddog_http::metric m ("myhost","cppTest",points);
    std::vector<ddog_http::metric> metrics;
    metrics.push_back(m);
    ddog_http::metricSeries series (metrics);

    ddog_http::client client(ddApiKey,ddAppKey);
    auto printMonitors = [](bool success, std::vector<ddog_http::monitor> Monitors,cpr::Response r){
        std::cout << r.text << std::endl;
    };
    auto printEvents = [](bool success, std::vector<ddog_http::event> Events,cpr::Response r){
        //std::cout << r.text << std::endl;
        if (Events.size() > 0){
            std::cout << Events[0].title << std::endl;
        }
    };

    auto printEventRes = [](bool success, cpr::Response r){
        std::cout << r.text << std::endl;
    };
    //client.sendMetricSeries(series,printRes);
    client.getMonitors(printMonitors);
    uint64_t now = timeSinceEpochSec();
    uint64_t before = timeSinceEpochSec() - (60 * 60 * 48);
    client.getEvents(before,now,printEvents);
    ddog_http::event e("Hello World","Testing 123");
    e.setAlertType("info");
    e.setPriority("normal");
    client.sendEvent(e,printEventRes);


    //client.checkApiKey();
    //system("pause");

}
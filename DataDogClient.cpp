#include "include/DataDogClient.hpp"
// #include <stdio.h>
// #include <stdlib.h>
// //#include <uv.h>
// //#include <curl/curl.h>
// #include <cpr/cpr.h>
// //#include <cpr/ssl_options.h>
// #include "DataDogMetric.h"
#include "include/DataDogMonitor.hpp"
#include "include/DataDogEvent.hpp"
#include <cstdint>
// #include <iostream>
// #include <sstream>
// #include "rapidjson/document.h"
// #include "rapidjson/writer.h"
// #include "rapidjson/stringbuffer.h"

using namespace rapidjson;


DataDogClient::DataDogClient(const char* ddApiKey,const char* ddAppKey){
    //loop = uv_default_loop();
    this->appKey = ddAppKey;
    this->apiKey = ddApiKey;  
}

void DataDogClient::sendMetricSeries(DataDogMetricSeries series,void(*cb)(bool,cpr::Response)){
    std::string data = series.getJsonStr();

    char uri[100] = "/api/v1/series?api_key=";
    std::strcat(uri,this->apiKey);
    std::ostringstream dd_url_stream;
    dd_url_stream << "https://api.datadoghq.com" << uri;
    std::string dd_url = dd_url_stream.str();

    auto parseResponse = [&cb](cpr::Response r){
        if (r.status_code == 202){
        cb(true,r);
        }else {
        cb(false,r);
        }
    };
    // cpr::SslOptions sslOpts = cpr::Ssl(
    // cpr::ssl::TLSv1{}, cpr::ssl::ALPN{false}, cpr::ssl::NPN{false}, cpr::ssl::CaPath{baseDirPath + "ca.cer"},
    // cpr::ssl::CertFile{baseDirPath + "/client.cer"}, cpr::ssl::KeyFile{baseDirPath + "client.key"},
    // cpr::ssl::VerifyPeer{false}, cpr::ssl::VerifyHost{false}, cpr::ssl::VerifyStatus{false});
    cpr::SslOptions sslOpts = cpr::Ssl(cpr::ssl::VerifyHost{true},cpr::ssl::VerifyPeer{true},cpr::ssl::ALPN{false}, cpr::ssl::NPN{false});
    //cpr::SslOptions sslOpts = cpr::Ssl();  
    auto future_response = cpr::PostCallback(parseResponse,cpr::Url{dd_url},
        cpr::Header{{"Accept","application/json"}},
        cpr::Header{{"Content-Type","application/json"}},
        cpr::Header{{"charset","utf-8"}},
        cpr::Body{data},cpr::VerifySsl(0));

    // if (future_response.wait_for(std::chrono::seconds(10)) == std::future_status::ready) {
    //     std::cout << future_response.get() << std::endl;
    // }
}

void DataDogClient::getMonitors(void(*cb)(bool,std::vector<DataDogMonitor>,cpr::Response)){
    char uri[100] = "/api/v1/monitor/search";
    
    std::ostringstream dd_url_stream;
    dd_url_stream << "https://api.datadoghq.com" << uri;
    std::string dd_url = dd_url_stream.str();

    auto parseResponse = [&cb](cpr::Response r){
        //std::cout << r.text << std::endl;
        std::vector<DataDogMonitor> Monitors;
        if (r.status_code == 200){
            Document doc;
            doc.Parse(r.text.c_str());
            Value& monitorsValue = doc["monitors"];
            assert(monitorsValue.IsArray());
            
            for (SizeType i=0;i<monitorsValue.Size();i++){
                Value& monitorVal = monitorsValue[i];
                DataDogMonitor Monitor(monitorVal);
                Monitors.push_back(Monitor);
            } 
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            doc.Accept(writer);
            std::cout << buffer.GetString() << std::endl;
            cb(true,Monitors,r);
        }else{
            cb(false,Monitors,r);
        }
    };

    auto future_response = cpr::GetCallback(parseResponse,cpr::Url{dd_url},
        cpr::Header{{"DD-APPLICATION-KEY",this->appKey},{"Content-Type","application/json"},{"DD-API-KEY",this->apiKey}},cpr::VerifySsl(0));
        
}

void DataDogClient::getEvents(uint64_t start_date, uint64_t end_date, void(*cb)(bool,std::vector<DataDogEvent>,cpr::Response)){
    char uri[100] = "/api/v1/events";
    
    std::ostringstream dd_url_stream;
    dd_url_stream << "https://api.datadoghq.com" << uri;
    std::string dd_url = dd_url_stream.str();

    auto parseResponse = [&cb](cpr::Response r){
        //std::cout << r.text << std::endl;
        std::vector<DataDogEvent> Events;
        if (r.status_code == 200){
            Document doc;
            doc.Parse(r.text.c_str());
            Value& eventsValue = doc["events"];
            assert(eventsValue.IsArray());
            
            for (SizeType i=0;i<eventsValue.Size();i++){
                Value& eventVal = eventsValue[i];
                assert(eventVal.IsObject());
                DataDogEvent Event(eventVal);
                Events.push_back(Event);
            } 
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            doc.Accept(writer);
            std::cout << buffer.GetString() << std::endl;
            cb(true,Events,r);
        }else{
            cb(false,Events,r);
        }
    };

    std::string start = std::to_string(start_date);
    std::string end = std::to_string(end_date);
    cpr::Parameters params = cpr::Parameters{{"start",start.c_str()},{"end",end.c_str()}};

    auto future_response = cpr::GetCallback(parseResponse,cpr::Url{dd_url},
        cpr::Header{{"DD-APPLICATION-KEY",this->appKey},{"Content-Type","application/json"},{"DD-API-KEY",this->apiKey}},
        params,
        cpr::VerifySsl(0));
       
};


void DataDogClient::checkApiKey(){
    char uri[100] = "/api/v1/validate";
    
    std::ostringstream dd_url_stream;
    dd_url_stream << "https://api.datadoghq.com" << uri;
    std::string dd_url = dd_url_stream.str();

    auto parseResponse = [](cpr::Response r){
        std::cout << r.text << std::endl;
        // if (r.status_code == 200){
        //   cb(true,r);
        // }else {
        //   cb(false,r);
        // }
    };

    auto future_response = cpr::GetCallback(parseResponse,cpr::Url{dd_url},
        cpr::Header{{"DD-API-KEY",this->apiKey}},cpr::VerifySsl(0));
};

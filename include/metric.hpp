#pragma once
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace rapidjson;

class point {
    public:
    uint64_t epoch;
    std::string value;
    point(uint64_t epoch, std::string value){
        this->epoch = epoch;
        this->value = value;
    }

};

class metric {
    public:
        metric (std::string host, std::string name, std::vector<point> );
        metric (std::string host, std::string name, std::vector<point>, std::string tags,std::string type);
        std::string getJsonStr(std::string host);
        Document getJsonDocument();
    private:
        std::string tags = "";
        std::string type = "";
        std::string host;
        std::string name;
        std::vector<point> points;
};

class metricSeries {
    public:
        metricSeries(std::vector<metric> metrics){
            this->metrics = metrics;
        }
        std::string getJsonStr(){
            Document root;
            root.SetObject();
            Document::AllocatorType& alloc = root.GetAllocator();
            Value series(kArrayType); 


            for (size_t i=0;i<this->metrics.size();++i) { 
                Document mD = this->metrics[i].getJsonDocument();
                series.PushBack(mD,alloc);
            }

            root.AddMember("series",series,alloc);
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            root.Accept(writer);

            //std::cout << buffer.GetString() << std::endl;
            // std::ostringstream ret;
            // ret << buffer.GetString();
            return std::string (buffer.GetString(), buffer.GetSize());

        }
    private:
        std::vector<metric> metrics;
};
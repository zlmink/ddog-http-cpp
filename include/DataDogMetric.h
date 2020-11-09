#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <vector>
#include <map>
#include <string>

using namespace rapidjson;


class DataDogPoint {
    public:
    uint64_t epoch;
    std::string value;
    DataDogPoint(uint64_t epoch, std::string value){
        this->epoch = epoch;
        this->value = value;
    }

};

class DataDogMetric {
    public:
        DataDogMetric (std::string host, std::string name, std::vector<DataDogPoint> );
        DataDogMetric (std::string host, std::string name, std::vector<DataDogPoint>, std::string tags,std::string type);
        std::string getJsonStr(std::string host);
        Document getJsonDocument();
    private:
        std::string tags = "";
        std::string type = "";
        std::string host;
        std::string name;
        std::vector<DataDogPoint> points;
};

class DataDogMetricSeries {
    public:
        DataDogMetricSeries(std::vector<DataDogMetric> metrics){
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
        std::vector<DataDogMetric> metrics;
};
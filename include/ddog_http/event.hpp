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
#include <iostream>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

namespace ddog_http {


// struct AlertType {
//     std::string none = "none";
//     std::string error = "error";
//     std::string warning = "warning";
//     std::string info = "info";
//     std::string success = "success";
//     std::string user_update = "user_update";
//     std::string recommendation = "recommendation";
//     std::string snapshot = "snapshot";
// };

// struct Priority {
//     std::string none = "none";
//     std::string normal = "normal";
//     std::string low = "low";
// };

struct child {
    uint64_t date_happened;
    std::string alert_type;
    uint64_t id;
};

class event {      

    public:
        event(Value& jsonData);
        event(std::string title, std::string text); //minimum requirement for an event
        uint64_t date_happened = 0;
        uint64_t related_event_id = 0;
        std::string alert_type = "none";
        bool is_aggregate;                          //event has children or not
        std::string title = "";
        std::string url = "";
        std::string text = "";
        std::vector<std::string> tags;
        std::vector<std::string> comments;
        std::string device_name = "";
        std::string priority = "none";
        std::string source = "";
        std::string host = "";
        std::string resource = "";
        std::string source_type_name = "";
        std::string aggregation_key = "";
        uint64_t id = 0;
        std::vector<child> children;               //children if the event is aggregated
        bool hasChildren();
        void setAlertType(std::string type){
            this->alert_type = type;
        };
        void setDate(uint64_t date){
            this->date_happened = date;
        };
        void setDeviceName(std::string name){
            this->device_name = name;
        };
        void setHost(std::string host){
            this->host = host;
        };
        void setPriority(std::string p){
            this->priority = p;
        };
        void setTags(std::vector<std::string> tags){
            this->tags = tags;
        };
        void setSourceTypeName(std::string name){
            this->source_type_name = name;
        };
        void setAggregationKey(std::string key){
            this->aggregation_key = key;
        };
        
        std::string getJsonStr(){
            Document root;
            root.SetObject();
            Document::AllocatorType& alloc = root.GetAllocator();
            Value series(kArrayType); 

            // if (!this->aggregation_key.empty()){

            // }

            if (this->alert_type.compare("none") != 0){
                Value alertType;
                alertType.SetString(this->alert_type.c_str(),this->alert_type.length(),alloc);
                root.AddMember("alert_type",alertType,alloc);
            }

            if (this->date_happened != 0){
                Value date;
                date.SetInt(this->date_happened);
                root.AddMember("date_happened",date,alloc);
            }

            if (this->device_name.compare("") != 0){
                Value name;
                name.SetString(this->device_name.c_str(),this->device_name.length(),alloc);
                root.AddMember("device_name",name,alloc);
            }

            if (this->host.compare("") != 0){
                Value h;
                h.SetString(this->host.c_str(),this->host.length(),alloc);
                root.AddMember("host",h,alloc);
            }

            if (this->priority.compare("none") != 0){
                Value p;
                p.SetString(this->priority.c_str(),this->priority.length(),alloc);
                root.AddMember("priority",p,alloc);
            }

            if (this->related_event_id != 0){
                Value id;
                id.SetInt(this->related_event_id);
                root.AddMember("related_event_id",id,alloc);
            }

            //array assignment
            // if (this->tags.size() > 0){
            //     Value tags;

            // }

            if (this->source_type_name.compare("") != 0){
                Value name;
                name.SetString(this->source_type_name.c_str(),this->source_type_name.length(),alloc);
                root.AddMember("source_type_name",name,alloc);
            }

            Value textVal;
            textVal.SetString(this->text.c_str(),this->text.length(),alloc);
            root.AddMember("text",textVal,alloc);

            Value titleVal;
            titleVal.SetString(this->title.c_str(),this->title.length(),alloc);
            root.AddMember("title",titleVal,alloc);
            
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            root.Accept(writer);

            //std::cout << buffer.GetString() << std::endl;
            // std::ostringstream ret;
            // ret << buffer.GetString();
            return std::string (buffer.GetString(), buffer.GetSize());

        };

};

}   //namespace
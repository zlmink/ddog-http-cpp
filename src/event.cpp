#include "ddog_http/event.hpp"
#include <stdexcept>
// #include "rapidjsonData/document.h"
// #include "rapidjsonData/writer.h"
// #include "rapidjsonData/stringbuffer.h"
// #include "client.hpp"
// #include <vector>
// #include <map>
// #include <string>
using namespace rapidjson;



namespace ddog_http {


uint64_t idToUint64(Value& json,std::string key){
    const char* k = key.c_str();;
    if (json[k].IsInt()){
        return json[k].GetInt();
    }else if (json[k].IsString()){
        return std::stoul(json[k].GetString());
    }else {
        throw std::invalid_argument("Value was not of integer type");
    }
}

bool event::hasChildren(){
    return this->is_aggregate;
}

event::event(Value& jsonData){
    if (!jsonData["date_happened"].IsNull()){
        this->date_happened = jsonData["date_happened"].GetInt();
    }
    this->alert_type = jsonData["alert_type"].GetString();
    if (!jsonData["is_aggregate"].IsNull()){
        this->is_aggregate = jsonData["is_aggregate"].GetBool();
    }

    if (this->is_aggregate){
        Value& childrenVal = jsonData["children"];
        for (SizeType i = 0;i<childrenVal.Size();i++){
            Value& childVal = childrenVal[i];
            child c {
                .date_happened=idToUint64(childVal,"date_happened"),
                .alert_type=childVal["alert_type"].GetString(),
                .id=idToUint64(childVal,"id")
            };
            this->children.push_back(c);
        }
    }

    this->title = jsonData["title"].GetString();
    this->url = jsonData["url"].GetString();
    this->text = jsonData["text"].GetString();

    Value& tags = jsonData["tags"];
    //assert(tags.IsArray());
    for (SizeType i = 0;i<tags.Size();i++){
        this->tags.push_back(tags[i].GetString());
    }

    if (!jsonData["comments"].IsNull()){
        Value& comments = jsonData["comments"];
        //assert(comments.IsArray());
        for (SizeType i = 0;i<comments.Size();i++){
            this->comments.push_back(comments[i].GetString());
        }
    }

    if (!jsonData["device_name"].IsNull()){
        this->device_name = jsonData["device_name"].GetString();
    }

    this->priority = jsonData["priority"].GetString();
    
    if (!jsonData["source"].IsNull()){
        this->source = jsonData["source"].GetString();
    }

    if (!jsonData["host"].IsNull()){
        this->host = jsonData["host"].GetString();
    }

    this->resource = jsonData["resource"].GetString();
    if (jsonData["id"].IsInt()){
        this->id = jsonData["id"].GetInt();
    }else if (jsonData["id"].IsString()){
        this->id = std::stoul(jsonData["id"].GetString());
    }
    

};

}
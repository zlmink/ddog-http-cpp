#include "ddog_http/monitor.hpp"
// #include "rapidjson/document.h"
// #include "rapidjson/writer.h"
// #include "rapidjson/stringbuffer.h"
// #include "client.hpp"
// #include <vector>
// #include <map>
// #include <string>
using namespace rapidjson;

namespace ddog_http {

monitor::monitor(Value& monitorJson){
    
    this->status = monitorJson["status"].GetString();
    Value& scopes = monitorJson["scopes"];
    for (SizeType i =0;i<scopes.Size();i++){
        //Value& scope = scopes[i];
        this->scopes.push_back(scopes[i].GetString());
    }
    Value& creator = monitorJson["creator"];
    this->creator = {.handle=creator["handle"].GetString(),
            .id=creator["id"].GetUint64(),
            .name=creator["name"].GetString()};
    this->overall_state_modified = monitorJson["overall_state_modified"].GetInt();
    //metrics
    Value& ms = monitorJson["metrics"];
    for (SizeType i=0;i<ms.Size();i++){
        this->metrics.push_back(ms[i].GetString());
    }
    //notifications
    Value& notes = monitorJson["notifications"];
    for (SizeType i=0;i<notes.Size();i++){
        Value& note = notes[i];
        this->notifications.push_back({.handle=note["handle"].GetString(),.name=note["name"].GetString()});
    }
    if (monitorJson["last_triggered_ts"].IsNull()!=true){
        this->last_triggered_ts = monitorJson["last_triggered_ts"].GetInt();
    }
    this->query = monitorJson["query"].GetString();
    this->id = monitorJson["id"].GetInt();
    this->name = monitorJson["name"].GetString();
    //tags
    this->org_id = monitorJson["org_id"].GetInt();
    this->type = monitorJson["type"].GetString();
    
}

}
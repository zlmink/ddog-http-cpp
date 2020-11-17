#include "include/metric.hpp"
// #include "include/rapidjson/document.h"
// #include "include/rapidjson/writer.h"
// #include "include/rapidjson/stringbuffer.h"
// #include <string>
// #include <iostream>
// #include <sstream>

// using namespace rapidjson;

metric::metric(std::string host, std::string name, std::vector<point> points){
    this->name = name;
    this->points = points;
    this->host = host;
}

metric::metric(std::string host, std::string name, std::vector<point> points,std::string tags = "",std::string type = ""){
    this->name = name;
    this->points = points;
    this->host = host;
    this->type = type;
    this->tags = tags;
}

Document metric::getJsonDocument(){
    Document d;
    d.SetObject();
    Document::AllocatorType& alloc = d.GetAllocator();
    Value h;
    h.SetString(host.c_str(),host.length(),alloc);
    Value metricName;
    metricName.SetString(this->name.c_str(),this->name.length(),alloc);

    d.AddMember("host",h,alloc);
    d.AddMember("metric",metricName,alloc);
    Value array(kArrayType);
    
    for (size_t i=0;i<this->points.size();++i) { 
        
        // Print the values 
        Value pArr(kArrayType);
        std::ostringstream e;
        e << this->points[i].epoch;
        Value epoch;
        epoch.SetString(e.str().c_str(),e.str().length(),alloc);
        Value val;
        val.SetString(this->points[i].value.c_str(),this->points[i].value.length(),alloc);
        pArr.PushBack(epoch,alloc).PushBack(val,alloc);
        array.PushBack(pArr,alloc);
        //std::cout << this->points[i].epoch << std::endl;
        
    }

    if (!this->tags.empty()){
        Value t;
        t.SetString(this->tags.c_str(),this->tags.length(),alloc);
        d.AddMember("tags",t,alloc);
    }

    if (!this->type.empty()){
        Value typ;
        typ.SetString(this->type.c_str(),this->type.length(),alloc);
        d.AddMember("type",typ,alloc);
    }

    d.AddMember("points",array,alloc);

    return d;
}

std::string metric::getJsonStr(std::string host){
    
    Document d = this->getJsonDocument();
    // Print the values 
    //std::cout << this->points[i].epoch << std::endl;
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    //std::cout << buffer.GetString() << std::endl;
    // std::ostringstream ret;
    // ret << buffer.GetString();
    return std::string (buffer.GetString(), buffer.GetSize());
}
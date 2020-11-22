ddog-http-cpp

This project enables access to the Datadog API using a lightweight C++ program.  It is asynchronous such that data can be collected and posted without needing to block the main loop of the integrating program.  The primary use case for this library is for IoT devices to post data to Datadog for analytics and alerting.  This library enables low power devices to interact with Datadog without requiring the Datadog agent.

from build folder
conan install .. --build=missing

TO DO
- [ ] - Validate a monitor 
- [ ] - Post monitors
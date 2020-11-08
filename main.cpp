// #include <boost/beast/core.hpp>
// #include <boost/beast/http.hpp>
// #include <boost/beast/version.hpp>
// #include <boost/asio/strand.hpp>
// #include <boost/lexical_cast.hpp>
// #include <boost/beast/ssl.hpp>
#include <curl/curl.h>
//#include <boost/certify/extensions.hpp>
//#include <boost/certify/https_verification.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <sstream>

#include "include/DataDogMetric.h"

#define NUMT 4

// namespace beast = boost::beast;         // from <boost/beast.hpp>
// namespace http = beast::http;           // from <boost/beast/http.hpp>
// namespace net = boost::asio;            // from <boost/asio.hpp>
// namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
// using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

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

// // Report a failure
// void
// fail(beast::error_code ec, char const* what)
// {
//     std::cerr << what << ": " << ec.message() << "\n";
// }

// // Performs an HTTP POST to the series endpoint and prints the response
// class session : public std::enable_shared_from_this<session>
// {
//     tcp::resolver resolver_;
//     beast::ssl_stream<beast::tcp_stream> stream_;
//     beast::flat_buffer buffer_; // (Must persist between reads)
//     http::request<http::string_body> req_;
//     http::response<http::string_body> res_;

//     public:
//         // Objects are constructed with a strand to
//         // ensure that handlers do not execute concurrently.
//         explicit
//         session(
//             net::any_io_executor ex,
//             ssl::context& ctx)
//         : resolver_(ex)
//         , stream_(ex, ctx)
//         {
//         }

//         // Start the asynchronous operation
//         void
//         run(
//             //char const* host,
//             //char const* port,
//             //char const* target,
//             char const* body,
//             char const* ddApiKey)
//             //char const* ddAppKey,
//             //int version)
//         {
//             char const* host = "api.datadoghq.com";
//             char const* port = "443";

//             // Set SNI Hostname (many hosts need this to handshake successfully)
//             if(! SSL_set_tlsext_host_name(stream_.native_handle(), host))
//             {
//                 beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
//                 std::cerr << ec.message() << "\n";
//                 return;
//             }
            
//             // Set up an HTTP GET request message
//             //req_.version(version);
//             req_.method(http::verb::post);
//             char uri[100] = "/api/v1/series?api_key=";
//             std::strcat(uri,ddApiKey);
//             printf("host %s\n",host);
//             printf("uri %s\n",uri);

//             req_.target(uri);
//             req_.set(http::field::host, host);
//             //req_.set("DD_API_KEY",ddApiKey);
//             //req_.set("DD_APPLICATION_KEY", ddAppKey);
//             req_.set(beast::http::field::content_type,"application/json");
//             req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
//             req_.body() = body;
//             req_.set(http::field::content_length, boost::lexical_cast<std::string>(strlen(body)));
//             req_.set(http::field::body, body);
//             req_.prepare_payload();

//             // Look up the domain name
//             resolver_.async_resolve(
//                 host,
//                 port,
//                 beast::bind_front_handler(
//                     &session::on_resolve,
//                     shared_from_this()));
//         }

//         void
//         on_resolve(
//             beast::error_code ec,
//             tcp::resolver::results_type results)
//         {
//             if(ec)
//                 return fail(ec, "resolve");

//              // Set a timeout on the operation
//             beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

//             // Make the connection on the IP address we get from a lookup
//             beast::get_lowest_layer(stream_).async_connect(
//                 results,
//                 beast::bind_front_handler(
//                     &session::on_connect,
//                     shared_from_this()));
//         }

//         void
//         on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
//         {
//             if(ec)
//                 return fail(ec, "connect");

//             // Set a timeout on the operation
//             //stream_.expires_after(std::chrono::seconds(30));

//             // Send the HTTP request to the remote host
//              // Perform the SSL handshake
//             stream_.async_handshake(
//                 ssl::stream_base::client,
//                 beast::bind_front_handler(
//                     &session::on_handshake,
//                     shared_from_this()));
//         }

//             void
//             on_handshake(beast::error_code ec)
//             {
//                 if(ec)
//                     return fail(ec, "handshake");

//                 // Set a timeout on the operation
//                 beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

//                 // Send the HTTP request to the remote host
//                 http::async_write(stream_, req_,
//                     beast::bind_front_handler(
//                         &session::on_write,
//                         shared_from_this()));
//             }

//         void
//         on_write(
//             beast::error_code ec,
//             std::size_t bytes_transferred)
//         {
//             boost::ignore_unused(bytes_transferred);

//             if(ec)
//                 return fail(ec, "write");
            
//             // Receive the HTTP response
//             http::async_read(stream_, buffer_, res_,
//                 beast::bind_front_handler(
//                     &session::on_read,
//                     shared_from_this()));
//         }

//         void
//         on_read(
//             beast::error_code ec,
//             std::size_t bytes_transferred)
//         {
//             boost::ignore_unused(bytes_transferred);

//         if(ec)
//             return fail(ec, "read");

//         // Write the message to standard out
//         std::cout << res_ << std::endl;

//         // Set a timeout on the operation
//         beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

//         // Gracefully close the stream
//         stream_.async_shutdown(
//             beast::bind_front_handler(
//                 &session::on_shutdown,
//                 shared_from_this()));
//         }

//         void
//         on_shutdown(beast::error_code ec)
//         {
//             if(ec == net::error::eof)
//             {
//                 // Rationale:
//                 // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
//                 ec = {};
//             }
//             if(ec)
//                 return fail(ec, "shutdown");

//             // If we get here then the connection is closed gracefully
//         }
// };

CURL * metricCurl(const char* ddApiKey,const char *jsonString)
{
	CURL *curl = NULL;
    // CURLM *multi_handle;
    // int still_running = 0;

	curl = curl_easy_init();
    struct curl_slist *headerList = NULL;
    //multi_handle = curl_multi_init();
	if (curl == NULL)
	{
		std::cout << "Failed to initialise curl" << std::endl;
		return NULL;
	}

    headerList = curl_slist_append(headerList, "Accept: application/json");
    headerList = curl_slist_append(headerList, "Content-Type: application/json");
    headerList = curl_slist_append(headerList, "charset: utf-8");

    char uri[100] = "/api/v1/series?api_key=";
    std::strcat(uri,ddApiKey);
	std::ostringstream dd_url_stream;
	dd_url_stream << "https://api.datadoghq.com" << uri;
	std::string dd_url = dd_url_stream.str();

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(curl, CURLOPT_URL, dd_url.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
	//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &DataDogStatsD::curlResponseWriteCallback);
	//curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (unsigned)strlen(jsonString));
	curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, jsonString);

    std::cout << "Sending data" << jsonString << std::endl;

	return curl;
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
    //auto const host = argv[1];
    //auto const port = argv[2];
    //auto const target = argv[3];
    auto const body = argv[1];
    //int version = argc == 5 && !std::strcmp("1.0", argv[5]) ? 10 : 11;
    
    
    // //---------- BOOST BEAST
    // // The io_context is required for all I/O
    // net::io_context ioc;

    // // The SSL context is required, and holds certificates
    // ssl::context ctx{ssl::context::tlsv12_client};

    // // This holds the root certificate used for verification
    // //load_root_certificates(ctx);

    // // Verify the remote server's certificate
    // ctx.set_verify_mode(ssl::context::verify_none );
    // //boost::certify::enable_native_https_server_verification(ctx);

    std::ostringstream bodyTest;
    DataDogPoint point (timeSinceEpochSec(),"5.465");
    std::vector<DataDogPoint> points;
    points.push_back(point);
    DataDogMetric metric ("myhost","cppTest",points);
    std::vector<DataDogMetric> metrics;
    metrics.push_back(metric);
    DataDogMetricSeries series (metrics);
    bodyTest << "{\"series\":[{\"host\":\"myhost\",\"metric\":\"cppTest\",\"points\":[[\"" << nowStr << "\",\"5.465\"]]}]}";
    //std::cout << bodyTest.str() << std::endl;
    // Launch the asynchronous operation
    //std::string content = bodyTest.str();
    //metric.getJsonStr("myhost");
    //eturn EXIT_USCCE
    //const char *cstr = bodyTest.str().c_str();
    const std::string json = series.getJsonStr();
    const char *cstr = json.c_str();
    // std::make_shared<session>(net::make_strand(ioc),ctx)->run(cstr, ddApiKey);


    // // Run the I/O service. The call will return when
    // // the get operation is complete.
    // ioc.run();
    // //---------------------------------------------------------
    CURLM *multi_handle;
    int still_running = 0;
    multi_handle = curl_multi_init();
    if (!multi_handle){return EXIT_FAILURE;};
    CURL *c = metricCurl(ddApiKey,cstr);
    curl_multi_add_handle(multi_handle, c);
    curl_multi_perform(multi_handle, &still_running);

    while(still_running){
        struct timeval timeout;
        int rc;
        CURLMcode mc;

        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcep;
        int maxfd = -1;

        long curl_timeo = -1;

        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        curl_multi_timeout(multi_handle, &curl_timeo);
        if(curl_timeo >= 0) {
            timeout.tv_sec = curl_timeo / 1000;
            if(timeout.tv_sec > 1)
            timeout.tv_sec = 1;
            else
            timeout.tv_usec = (curl_timeo % 1000) * 1000;
        }

        /* get file descriptors from the transfers */
        mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

        if(mc != CURLM_OK) {
            fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
            break;
        }

      /* On success the value of maxfd is guaranteed to be >= -1. We call
         select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
         no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
         to sleep 100ms, which is the minimum suggested value in the
         curl_multi_fdset() doc. */

      if(maxfd == -1) {
        #ifdef _WIN32
                Sleep(100);
                rc = 0;
        #else
                /* Portable sleep for platforms other than Windows. */
                struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
                rc = select(0, NULL, NULL, NULL, &wait);
        #endif
      }
      else {
        /* Note that on some platforms 'timeout' may be modified by select().
           If you need access to the original value save a copy beforehand. */
        rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
      }

      switch(rc) {
        case -1:
            /* select error */
            break;
        case 0:
        default:
            /* timeout or readable/writable sockets */
            printf("perform!\n");
            curl_multi_perform(multi_handle, &still_running);
            printf("running: %d!\n", still_running);
            break;
      }
       
}
    curl_multi_cleanup(multi_handle);

        /* always cleanup */
        curl_easy_cleanup(c);

        return EXIT_SUCCESS;    

}

    /* then cleanup the form */
    //curl_mime_free(form);

    /* free slist */
    //curl_slist_free_all(headerlist);
    //}

    //return EXIT_SUCCESS;
//}
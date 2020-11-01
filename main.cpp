#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/beast/ssl.hpp>
//#include <boost/certify/extensions.hpp>
//#include <boost/certify/https_verification.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//------------------------------------------------------------------------------

// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Performs an HTTP POST to the series endpoint and prints the response
class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver_;
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_; // (Must persist between reads)
    http::request<http::string_body> req_;
    http::response<http::string_body> res_;

    public:
        // Objects are constructed with a strand to
        // ensure that handlers do not execute concurrently.
        explicit
        session(
            net::any_io_executor ex,
            ssl::context& ctx)
        : resolver_(ex)
        , stream_(ex, ctx)
        {
        }

        // Start the asynchronous operation
        void
        run(
            //char const* host,
            //char const* port,
            //char const* target,
            char const* body,
            char const* ddApiKey)
            //char const* ddAppKey,
            //int version)
        {
            char const* host = "api.datadoghq.com";
            char const* port = "443";

            // Set SNI Hostname (many hosts need this to handshake successfully)
            if(! SSL_set_tlsext_host_name(stream_.native_handle(), host))
            {
                beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
                std::cerr << ec.message() << "\n";
                return;
            }
            
            // Set up an HTTP GET request message
            //req_.version(version);
            req_.method(http::verb::post);
            char uri[100] = "/api/v1/series?api_key=";
            std::strcat(uri,ddApiKey);
            printf("host %s\n",host);
            printf("uri %s\n",uri);

            req_.target(uri);
            req_.set(http::field::host, host);
            //req_.set("DD_API_KEY",ddApiKey);
            //req_.set("DD_APPLICATION_KEY", ddAppKey);
            req_.set(beast::http::field::content_type,"application/json");
            req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            req_.body() = "{\"series\":[{\"metric\":\"system.load.1\",\"points\":[[\"1604252717\",\"134.5\"]]}]}";
            req_.set(http::field::content_length, boost::lexical_cast<std::string>(strlen("{\"series\":[{\"metric\":\"system.load.1\",\"points\":[[\"1604252717\",\"134.5\"]]}]}")));
            req_.set(http::field::body, "{\"series\":[{\"metric\":\"system.load.1\",\"points\":[[\"1604252717\",\"134.5\"]]}]}");
            req_.prepare_payload();

            // Look up the domain name
            resolver_.async_resolve(
                host,
                port,
                beast::bind_front_handler(
                    &session::on_resolve,
                    shared_from_this()));
        }

        void
        on_resolve(
            beast::error_code ec,
            tcp::resolver::results_type results)
        {
            if(ec)
                return fail(ec, "resolve");

             // Set a timeout on the operation
            beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

            // Make the connection on the IP address we get from a lookup
            beast::get_lowest_layer(stream_).async_connect(
                results,
                beast::bind_front_handler(
                    &session::on_connect,
                    shared_from_this()));
        }

        void
        on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
        {
            if(ec)
                return fail(ec, "connect");

            // Set a timeout on the operation
            //stream_.expires_after(std::chrono::seconds(30));

            // Send the HTTP request to the remote host
             // Perform the SSL handshake
            stream_.async_handshake(
                ssl::stream_base::client,
                beast::bind_front_handler(
                    &session::on_handshake,
                    shared_from_this()));
        }

            void
            on_handshake(beast::error_code ec)
            {
                if(ec)
                    return fail(ec, "handshake");

                // Set a timeout on the operation
                beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

                // Send the HTTP request to the remote host
                http::async_write(stream_, req_,
                    beast::bind_front_handler(
                        &session::on_write,
                        shared_from_this()));
            }

        void
        on_write(
            beast::error_code ec,
            std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);

            if(ec)
                return fail(ec, "write");
            
            // Receive the HTTP response
            http::async_read(stream_, buffer_, res_,
                beast::bind_front_handler(
                    &session::on_read,
                    shared_from_this()));
        }

        void
        on_read(
            beast::error_code ec,
            std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "read");

        // Write the message to standard out
        std::cout << res_ << std::endl;

        // Set a timeout on the operation
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // Gracefully close the stream
        stream_.async_shutdown(
            beast::bind_front_handler(
                &session::on_shutdown,
                shared_from_this()));
        }

        void
        on_shutdown(beast::error_code ec)
        {
            if(ec == net::error::eof)
            {
                // Rationale:
                // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
                ec = {};
            }
            if(ec)
                return fail(ec, "shutdown");

            // If we get here then the connection is closed gracefully
        }
};

//------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    //get datadog environment variables
    const char* ddApiKey = std::getenv("DD_API_KEY");
    const char* ddAppKey = std::getenv("DD_APPLICATION_KEY");

    if (ddApiKey == NULL){
        std::cerr <<
            "Missing data dog api key";
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

    // The io_context is required for all I/O
    net::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::tlsv12_client};

    // This holds the root certificate used for verification
    //load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::context::verify_none );
    //boost::certify::enable_native_https_server_verification(ctx);

    // Launch the asynchronous operation
    std::make_shared<session>(net::make_strand(ioc),ctx)->run(body, ddApiKey);

    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();

    return EXIT_SUCCESS;
}
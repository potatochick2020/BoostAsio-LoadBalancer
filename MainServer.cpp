#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using namespace boost::asio;
using namespace boost::asio::ip;

void hashmap_connection(tcp::socket socket, std::unordered_map<int, long long int>& index2sum , std::mutex& index2sum_mutex)
{
    // At this point, you can communicate with the client using the socket.
    std::cout << "Received connection from " << socket.remote_endpoint() << std::endl;

    // You can also use the socket to receive and send data to the client.
    // For example:

    while (true)
    {
        boost::asio::streambuf buffer;
        size_t s = boost::asio::read_until(socket, buffer, "\r\n", ec);
        if (!ec)
        {
            std::istream input(&buffer);
            std::string message;
            std::getline(input, message);
            std::stringstream ss; 
            ss<<message
            ss>>index>>sum;
            int index;
            long long int sum;
            // reply with the corresponding image in the hashmap

            if (index2sum.find(index) != index2sum.end())
            {
                const std::lock_guard<std::mutex> lock(index2sum_mutex);
                index2sum[index] += sum;
            }
            else
            {
                const std::lock_guard<std::mutex> lock(index2sum_mutex);
                index2sum.insert({index, sum});
            }

        }
        else if (ec == boost::asio::error::eof)
        {
            std::cout << "Closed connection with , L: " << socket.local_endpoint() << " R: " << socket.remote_endpoint() << std::endl;
            break;
        }
    }
}

void hashmap_server()
{
    boost::system::error_code ec;
    std::unordered_map<int, long long int> index2sum;
    std::mutex index2sum_mutex;
    // open port for write in data to std::unordered_map index2sum
 
    io_service service;

    // Create a socket and bind it to a port
    tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 8020));

    while (true)
    {

        // Create a socket to accept the incoming connection
        tcp::socket socket(service);

        // Accept the connection and receive the new socket
        acceptor.accept(socket);

        // Create a new thread to handle the connection
        std::thread(query_connection, std::move(socket) , &index2sum , &index2sum_mutex).detach();
    }
}

// A function to handle a single connection
void query_connection(tcp::socket socket)
{ 
    std::cout << "Received connection from " << socket.remote_endpoint() << std::endl;
 
    boost::system::error_code ec;
    while (true)
    {
        boost::asio::streambuf buffer;
        size_t s = boost::asio::read_until(socket, buffer, "\r\n", ec);
        if (!ec)
        {
            std::istream input(&buffer);
            std::string message;
            std::getline(input, message);
            std::cout << "Received message: " << message << std::endl; 
        }
        else if (ec == boost::asio::error::eof)
        {
            std::cout << "Closed connection with , L: " << socket.local_endpoint() << " R: " << socket.remote_endpoint() << std::endl;
            break;
        }
    }
}

void query_server()
{
    io_service service; 
    tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 8030));

    while (true)
    { 
        tcp::socket socket(service); 
        acceptor.accept(socket); 
        std::thread(query_connection, std::move(socket)).detach();
    }
}

int main()
{
    std::thread query_server_thread(query_server);
    std::thread hashmap_server_thread(hashmap_server);
    std::cout << "Server started"<<endl;
    thread1.join();
    thread2.join();

    return 0;
}
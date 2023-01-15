#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <boost/system/error_code.hpp>  

void hashmap_connection(boost::asio::ip::tcp::socket socket, std::unordered_map<int, long long int>& index2sum , std::mutex& index2sum_mutex)
{
    // At this point, you can communicate with the client using the socket.
    std::cout << "Received connection from " << socket.remote_endpoint() << std::endl;

    // You can also use the socket to receive and send data to the client.
    // For example:

    while (true)
    {
        boost::asio::streambuf buffer;
        boost::system::error_code ec_index;
        boost::system::error_code ec_sum;
        int index;
        long long int sum;
        boost::asio::read(socket, boost::asio::buffer(&index , sizeof(int)), boost::asio::transfer_exactly(sizeof(int)),ec_index);
        boost::asio::read(socket, boost::asio::buffer(&sum , sizeof(long long int)), boost::asio::transfer_exactly(sizeof(long long int)),ec_sum);
        std::cout<<"received from worker, index : "<<index<<" , sum : "<<sum<<std::endl;
        if (!ec_index && !ec_sum)
        {  
            // reply with the corresponding image in the hashmap
            { 
                const std::lock_guard<std::mutex> lock(index2sum_mutex);
                if (index2sum.find(index) != index2sum.end())
                {
                    index2sum[index] += sum;
                }
                else
                { 
                    index2sum.insert({index, sum});
                }
            }  
        }
        else if (ec_index == boost::asio::error::eof || ec_sum == boost::asio::error::eof)
        {
            std::cout << "Closed connection with , L: " << socket.local_endpoint() << " R: " << socket.remote_endpoint() << std::endl;
            break;
        }
    }
}

void hashmap_server(std::unordered_map<int, long long int>& index2sum , std::mutex& index2sum_mutex)
{
    boost::system::error_code ec; 
    // open port for write in data to std::unordered_map index2sum
 
    boost::asio::io_service service;

    // Create a socket and bind it to a port
    boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),3310 ));

    while (true)
    {

        // Create a socket to accept the incoming connection
        boost::asio::ip::tcp::socket socket(service);

        // Accept the connection and receive the new socket
        acceptor.accept(socket);

        // Create a new thread to handle the connection
        std::thread(hashmap_connection, std::move(socket) , std::ref(index2sum) , std::ref(index2sum_mutex)).detach();
    }
}

// A function to handle a single connection
void query_connection(boost::asio::ip::tcp::socket socket, std::unordered_map<int, long long int>& index2sum , std::mutex& index2sum_mutex)
{ 
    std::cout << "Received connection from " << socket.remote_endpoint() << std::endl;
 
    boost::system::error_code ec;
    while (true)
    { 
        int index;
        boost::asio::read(socket, boost::asio::buffer(&index,sizeof(int)),boost::asio::transfer_exactly(sizeof(int)), ec);
        if (!ec)
        { 
            std::cout << "Received message: " << index << std::endl; 

            long long int reply = index2sum[index];   
            boost::asio::write(socket, boost::asio::buffer(&reply,sizeof(long long int)));
        }
        else if (ec == boost::asio::error::eof)
        {
            std::cout << "Closed connection with , L: " << socket.local_endpoint() << " R: " << socket.remote_endpoint() << std::endl;
            break;
        }
    }
}

void query_server(std::unordered_map<int, long long int>& index2sum , std::mutex& index2sum_mutex)
{
    boost::asio::io_service service; 
    boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3320));

    while (true)
    { 
        boost::asio::ip::tcp::socket socket(service); 
        acceptor.accept(socket); 
        std::thread(query_connection, std::move(socket),std::ref(index2sum),std::ref(index2sum_mutex)).detach();
    }
}

int main()
{ 
    std::unordered_map<int,long long int> index2sum;
    std::mutex index2sum_mutex;

    std::cout << "Server started"<<std::endl;
    std::thread query_server_thread(query_server,std::ref(index2sum),std::ref(index2sum_mutex));
    std::thread hashmap_server_thread(hashmap_server,std::ref(index2sum),std::ref(index2sum_mutex));
    
    query_server_thread.join();
    hashmap_server_thread.join();

    return 0;
}
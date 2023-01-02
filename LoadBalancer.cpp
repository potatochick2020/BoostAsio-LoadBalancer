#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <vector> 
#include <queue>
#include <thread>  

using namespace boost::asio;
using namespace boost::asio::ip;

class 

void insert_data(std::queue<std::string>& string_queue,std::mutex& string_queue_mutex,bool& end_read, std::queue<std::pair<int,std::vector<int>>>& data_queue,std::mutex& data_queue_mutex)
{
    while (end_read != true){
        {
            std::lock_guard<std::mutex> string_lock(string_queue_mutex);
            if (!string_queue.empty())std::string line = string_queue.pop();
        }
        int index;
        std::stringstream ss;
        ss<<line;
        ss>>index;
        std::vector<int> data;
        data.reserve(1000);
        int _i = 0;
        while (!ss.eof()){
            ss>>data[_i];
        }
        const std::lock_guard<std::mutex> data_lock(data_queue_mutex);
        data_queue.push({index,data[_i]});
    }
}

void reader(std::queue<std::string>& string_queue,std::mutex& string_queue_mutex,bool& end_read)
{
    std::ifstream file("datasheet-1.txt");
    std::string line;
    while (std::getline(file, line)) {  
        std::lock_guard<std::mutex> string_lock(string_queue_mutex);
        string_queue.push(line);
    } 
    end_read = true;
}
void get_worker_workload(){

        boost::uuids::uuid uuid = boost::uuids::random_generator()(); 
        //worker server get its uuid
        boost::asio::write(socket,boost::asio::buffer(uuid));
        {
            std::lock_guard<std::mutex> lock(uuid2socket_mutex);
            uuid2socket[uuid] = socket;
        }
        //worker server send a connected flag
        boost::asio:read(socket,boost::asio::buffer(uuid));
        
        //the worker server will send size of task queue every 1000ms
        boost::system::error_code ec;
        while (true)
        {
            boost::asio::streambuf buffer;
            size_t s = boost::asio::read(socket, buffer, ec);
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
void worker_listener(){
    io_service service; 
    tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 3310));

    while (true)
    { 
        tcp::socket socket(service); 
        acceptor.accept(socket); 
        
        std::thread(get_worker_workload, std::move(socket)).detach();
        uuid2socket.erase(uuid);
    }

}
void task_socket_listener(){
    io_service service; 
    tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 3320));

    while (true)
    { 
        tcp::socket socket(service); 
        acceptor.accept(socket); 
        
        std::thread(get_worker_workload, std::move(socket)).detach();
        uuid2socket.erase(uuid);
    }
}
int main()
{
    const auto processor_count = std::thread::hardware_concurrency();
    //one thread to read data 
    //remaining thread convert data from string to int index and vector<int> data
    std::queue<std::pair<int,std::vector<int>>> data_queue; 
    std::mutex data_queue_mutex;
    
    std::queue<std::string> string_queue; 
    std::mutex string_queue_mutex;
    bool end_read = false;

    std::thread reader_thread(reader,string_queue,string_queue_mutex,end_read);
    std::vector<std::thread> threads;
    for (int i = 0; i<processor_count-1;i++){
        threads.push_back(std::thread(insert_data,string_queue,string_queue_mutex,end_read,data_queue,data_queue_mutex));
    } 
    reader_thread.join();
    for (std::thread & thread : threads)
    { 
        if (thread.joinable()) thread.join();
    } 

    //one thread to open a port for receiving worker connection, open new thread once accept new connection
    //one thread to keep asking number of task for each worker every 1 second; (store in a min heap sorted by number of task and corresponding port/index in the port vector)

        //get task of all worker
        //it will distribute task to the worker 
            // if it is disconnected remove from the uuid2socket map
    

    unordered_map<boost::uuids::uuid,tcp::socket> uuid2socket; // uuid -> {worklistener socket, task data socket}
    std::mutex uuid2socket_mutex;
    map<boost::uuids::uuid,int> uuid2workload; 
    std::mutex uuid2workload_mutex;
    

    worker_listener(uuid2socket,uuid2socket_mutex,lowest_workload_worker_heap,lowest_workload_worker_heap_mutex);
    task_allocator(uuid2socket,uuid2socket_mutex,lowest_workload_worker_heap,lowest_workload_worker_heap_mutex);
    //find lowest task every 100 ms 

    while (!data_queue.empty()){
        while (uuid2socket.size()!=0){
            auto it = min_element(uuid2workload.begin(), uuid2workload.end(), [](const auto& l, const auto& r) { return l.second < r.second; });
            {
                std::lock_guard<std::mutex> lock(data_queue_mutex);
                auto task = data_queue.top();
                data_queue.pop();
            }
            boost::asio::write(it->second,task);
            //distribute task to work server
        }
    }

    return 0;
}

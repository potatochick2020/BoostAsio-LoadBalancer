#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/system/error_code.hpp> 
#include <iostream>
#include <algorithm>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <thread>
#include <unordered_map>
#include <boost/functional/hash.hpp>
  
void insert_data(std::queue<std::string> &string_queue, std::mutex &string_queue_mutex, std::atomic<bool> &end_read, std::queue<std::pair<int, std::vector<int>>> &data_queue, std::mutex &data_queue_mutex)
{
    {
        std::lock_guard<std::mutex> l(string_queue_mutex);
        std::cout<<"string_queue.size()"<<string_queue.size()<<" "<<end_read <<std::endl;

    }
    std::atomic<bool> end_string_queue = false;
    while (end_read == false || end_string_queue == false)
    {

        std::string line;
        {
            std::lock_guard<std::mutex> string_queue_lock(string_queue_mutex);
            if (!string_queue.empty())
            {
                line = string_queue.front();
                string_queue.pop();
            } else if (end_read == true) {
                end_string_queue = true;
                break;
            }
        }
        int index;
        std::stringstream ss;
        ss << line;
        ss >> index;
        std::vector<int> data(1000);
        for (int i = 0;i<1000;i++)
        {
            ss >> data[i];
        }
        {
            std::lock_guard<std::mutex> data_queue_lock(data_queue_mutex);
            data_queue.push({index, data});
            std::cout<<"data_queue.size() : "<<data_queue.size() <<" ,index : "<<index<<" , vc size :"<<data.size()<<std::endl;
        }
        
    }
    {
            std::lock_guard<std::mutex> data_queue_lock(data_queue_mutex); 
            std::cout<<"data_queue.size() : "<<data_queue.size()<<std::endl;
    }
}

void reader(std::queue<std::string> &string_queue, std::mutex &string_queue_mutex, std::atomic<bool> &end_read)
{
    std::ifstream file("datasheet-1.txt");
    std::string line;
    while (std::getline(file, line))
    {  
        std::lock_guard<std::mutex> string_lock(string_queue_mutex);
        string_queue.push(line); 
        std::cout<<"string_queue.size() : "<<string_queue.size()<<std::endl;
    }
    file.close();
    end_read = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
}

void get_worker_workload(boost::asio::ip::tcp::socket socket, const boost::uuids::uuid uuid, std::unordered_map<boost::uuids::uuid, size_t, boost::hash<boost::uuids::uuid>>& uuid2workload, std::mutex& uuid2workload_mutex)
{ 
    std::cout << "port 8820: Received connection from " << socket.remote_endpoint() << std::endl;
    // the worker server will send size of task queue every 1000ms
    boost::system::error_code ec;
    while (true)
    {
        boost::asio::streambuf buffer;
        size_t workload;
        boost::asio::read(socket, boost::asio::buffer(&workload, sizeof(size_t)), boost::asio::transfer_exactly(sizeof(size_t)),ec);
        std::cout<<"Receive workload : "<<workload<<std::endl;
        if (!ec)
        {
            std::lock_guard<std::mutex> uuid2workload_lock(uuid2workload_mutex);
            if (uuid2workload.find(uuid)!=uuid2workload.end()){ 
                uuid2workload[uuid] = workload;
            } else {
                uuid2workload.insert({uuid,workload});
            }
        }
        else if (ec == boost::asio::error::eof)
        {
            std::cout << "Closed connection with , L: " << socket.local_endpoint() << " R: " << socket.remote_endpoint() << std::endl;
            break;
        }
    }
}

void find_lowest_workload(boost::uuids::uuid& lowest_workload_uuid, std::mutex& lowest_workload_uuid_mutex,std::unordered_map<boost::uuids::uuid, size_t, boost::hash<boost::uuids::uuid>> uuid2workload,std::mutex& uuid2workload_mutex)
{
    // find lowest task every 100 ms
    while (true){
        std::lock_guard<std::mutex> uuid2workload_lock(uuid2workload_mutex); 
        
        while (uuid2workload.size() != 0)
        {
            auto it = std::min_element(uuid2workload.begin(), uuid2workload.end(), [](const auto &l, const auto &r)
                                       { return l.second < r.second; });
                                       
            std::lock_guard<std::mutex> lowest_workload_uuid_lock(lowest_workload_uuid_mutex);          
            lowest_workload_uuid = it->first;
        }
        // this thread sleep for 100ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }
}
 
void send_task_to_lowest_workload_worker(boost::uuids::uuid& lowest_workload_uuid, std::mutex& lowest_workload_uuid_mutex,std::unordered_map<boost::uuids::uuid,std::unique_ptr< boost::asio::ip::tcp::socket>, boost::hash<boost::uuids::uuid>>& uuid2socket,std::mutex& uuid2socket_mutex,std::queue<std::pair<int, std::vector<int>>>& data_queue ,std::mutex& data_queue_mutex)
{

    // find lowest task every 100 ms
    while (true){ 
        std::lock_guard<std::mutex> data_queue_lock(data_queue_mutex); 
        while (data_queue.size() != 0)
        { 
            int index;
            std::vector<int> task; 
            { 
                index = data_queue.front().first;
                task = data_queue.front().second;
                data_queue.pop();
            }
            { 
                std::lock_guard<std::mutex> lowest_workload_uuid_lock(lowest_workload_uuid_mutex);
                std::lock_guard<std::mutex> uuid2socket_lock(uuid2socket_mutex); 
                std::cout<<"start send index and task , uuid2socket size "<<uuid2socket.size() <<std::endl;
                std::cout<<"start index"<<index<<std::endl;
                boost::asio::write(*uuid2socket[lowest_workload_uuid], boost::asio::buffer(&index,sizeof(int)));
                std::cout<<"start task "<<task.size()<<std::endl;
                boost::asio::write(*uuid2socket[lowest_workload_uuid], boost::asio::buffer(&task,sizeof(std::vector<int>(1000))));
                 
            }
            // this thread sleep for 100ms 
        }
    }
}

// a main port 8820 which listen to worker initial connection, and move to the @get_work_workload function to get workload
void initial_worker_listener(std::unordered_map<boost::uuids::uuid, size_t, boost::hash<boost::uuids::uuid>>& uuid2workload, std::mutex& uuid2workload_mutex)
{
    boost::asio::io_context service;
    boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8820));
    
    while (true)
    {
        boost::asio::ip::tcp::socket socket(service);
        acceptor.accept(socket);
        std::cout<<"8820 connected"<<std::endl;
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        // worker server get its uuid
        boost::asio::write(socket, boost::asio::buffer(&uuid,sizeof(uuid)));
        
        std::thread(get_worker_workload,std::move(socket), std::ref(uuid), std::ref(uuid2workload),std::ref(uuid2workload_mutex)).detach();
    }
}

// a main port 8830 which listen to second worker connection, to send the worker task
void task_worker_listener(std::unordered_map<boost::uuids::uuid,std::unique_ptr< boost::asio::ip::tcp::socket>,boost::hash<boost::uuids::uuid>>& uuid2socket, std::mutex& uuid2socket_mutex)
{
    boost::asio::io_context service;
    boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8830));

    while (true)
    {
        boost::asio::ip::tcp::socket socket(service);
        acceptor.accept(socket); 
        std::cout << "port 8830: Received connection from " << socket.remote_endpoint() << std::endl;
        boost::uuids::uuid uuid;
        std::cout << "start reading uuid"<<std::endl;
        boost::asio::read(socket, boost::asio::buffer(&uuid , sizeof(boost::uuids::uuid)), boost::asio::transfer_exactly(sizeof(boost::uuids::uuid)));
        std::cout << "end reading uuid : "<<uuid<<std::endl; 
        {
            std::lock_guard<std::mutex> lock(uuid2socket_mutex);
            uuid2socket[uuid] = std::make_unique<boost::asio::ip::tcp::socket>(std::move(socket)); 
            std::cout<<"sizeeeeeeeeeeeeeeee "<<uuid2socket.size()<<std::endl;
        }  
    }
}

int main()
{
    const auto processor_count = std::thread::hardware_concurrency();
    std::atomic<bool> end_read = false;
    // scope to deal with data reading from file
     
        // string queue store the unserialize data,
        std::queue<std::string> string_queue;
        std::mutex string_queue_mutex;

        // data queue store the serialize data,
        std::queue<std::pair<int, std::vector<int>>> data_queue;
        std::mutex data_queue_mutex;
        std::cout<<"start reading data"<<std::endl;

        std::thread reader_thread(reader, std::ref(string_queue), std::ref(string_queue_mutex), std::ref(end_read));
        if (reader_thread.joinable()) reader_thread.join();

        // serialize data from string queue to data queue, converting string to int, long long int
        std::thread insert_data_thread(insert_data, std::ref(string_queue), std::ref(string_queue_mutex), std::ref(end_read), std::ref(data_queue), std::ref(data_queue_mutex));
        if (insert_data_thread.joinable()) insert_data_thread.join();
        
        
        std::cout<<"finish read and serialize data"<<std::endl;
    // one thread to open a port for receiving worker connection, open new thread once accept new connection
    // one thread to keep asking number of task for each worker every 1 second; (store in a min heap sorted by number of task and corresponding port/index in the port vector)

    // get task of all worker
    // it will distribute task to the worker
    //  if it is disconnected remove from the uuid2socket map

    std::unordered_map<boost::uuids::uuid,std::unique_ptr< boost::asio::ip::tcp::socket>, boost::hash<boost::uuids::uuid>> uuid2socket; // uuid -> {worklistener socket, task data socket}
    std::mutex uuid2socket_mutex;
    std::unordered_map<boost::uuids::uuid, size_t, boost::hash<boost::uuids::uuid>> uuid2workload;
    std::mutex uuid2workload_mutex;

    boost::uuids::uuid lowest_workload_uuid;
    std::mutex lowest_workload_uuid_mutex;

    ///  TODO: add the 2 global socket here
    // receive initial connection and get workload

    std::cout<<"1"<<std::endl;
    std::thread(initial_worker_listener, std::ref(uuid2workload),std::ref(uuid2workload_mutex)).detach();  
    // receive another connection and move the connection to uuid2socket map
    std::cout<<"2"<<std::endl;
    std::thread(task_worker_listener,std::ref(uuid2socket), std::ref(uuid2socket_mutex)).detach();  

    //check is there at least one working worker
    while (true){
        {
            std::lock_guard<std::mutex> uuid2socket_lock(uuid2socket_mutex);
            if (uuid2socket.size() > 0){
                std::cout<<"There are at least 1 connection, continue"<<std::endl;
                break;
            } else { 
                std::cout<<"No connection yet, retry after 2000 ms"<<std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
    }
    
    // thread to find out lowest task
    std::cout<<"3"<<std::endl;
    std::thread(find_lowest_workload,std::ref(lowest_workload_uuid), std::ref(lowest_workload_uuid_mutex),std::ref(uuid2workload),std::ref(uuid2workload_mutex)).detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000)); 
    // thread to send task to lowest workload
    {
        std::lock_guard<std::mutex> lowest_workload_uuid_lock(lowest_workload_uuid_mutex);
        std::lock_guard<std::mutex> data_queue_lock(data_queue_mutex);
        std::cout<<"4 "<<lowest_workload_uuid<<" dq.size : "<<data_queue.size()<<std::endl;
    }
    std::thread(send_task_to_lowest_workload_worker,std::ref(lowest_workload_uuid), std::ref(lowest_workload_uuid_mutex),std::ref(uuid2socket),std::ref(uuid2socket_mutex),std::ref(data_queue) ,std::ref(data_queue_mutex)).detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000)); 
    std::cout<<"5"<<std::endl;
    ///  TODO: close the 2 global socket here
     
}

// why need push socket to map? need to send the task back

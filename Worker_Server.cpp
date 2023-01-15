#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_io.hpp>     
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <chrono> 
#include <queue>
#include <numeric>
#include <iostream>

void task_listener(boost::uuids::uuid& uuid,std::queue<std::pair<int,std::vector<int>>>& task_queue, std::mutex& task_queue_mutex ){
    boost::asio::io_service task_listener_io;
    boost::asio::ip::tcp::socket task_listener_socket(task_listener_io); 
    task_listener_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8830)); 
    boost::asio::write(task_listener_socket,boost::asio::buffer(&uuid,sizeof(uuid))); 
    while (true){ // receive an end signal 
        int index;
        std::vector<int> values(1000);  
        std::cout<<"start read task"<<std::endl;
        boost::asio::read(task_listener_socket, boost::asio::buffer(&index, sizeof(int)), boost::asio::transfer_exactly(sizeof(int)));
        std::cout<<"end read index :"<<index<<std::endl;
        boost::asio::read(task_listener_socket, boost::asio::buffer(&values, sizeof(std::vector<int>(1000))), boost::asio::transfer_exactly(sizeof(std::vector<int>(1000))));
        std::cout<<"end read task :"<<values.size()<<std::endl;
        std::cout<<"end read task0 "<<std::endl;
        std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex); 
        std::cout<<"end read task1 "<<std::endl;
        task_queue.push({std::move(index),std::move(values)}); 
        std::cout<<"end read task2 "<<std::endl;
    }   
    std::cout<<"end task_listener"<<std::endl;
}

void connect_loadbalancer(std::queue<std::pair<int,std::vector<int>>>& task_queue,std::mutex& task_queue_mutex){
    boost::asio::io_service loadbalancer_io;
    boost::asio::ip::tcp::socket loadbalancer_socket(loadbalancer_io);
    //Initiate connection to loadbalancer
    loadbalancer_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8820)); 
    boost::uuids::uuid uuid;
    boost::asio::read(loadbalancer_socket,boost::asio::buffer(&uuid,sizeof(boost::uuids::uuid)),boost::asio::transfer_exactly(sizeof(boost::uuids::uuid)));
    std::thread(task_listener,std::ref(uuid),std::ref(task_queue),std::ref(task_queue_mutex)).detach();
  
    while (true){  
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //send queue size to port 8820
        size_t task_queue_size;
        {
            std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex);  
            //send queue size to port 3310
            task_queue_size = task_queue.size();
        } 
        boost::asio::write(loadbalancer_socket,boost::asio::buffer(&task_queue_size,sizeof(size_t)));
        /// TODO: send total number of task 
    }  
}

int main()
{
    
    std::queue<std::pair<int,std::vector<int>>> task_queue;
    std::mutex task_queue_mutex; 
    bool received_all_task_flag = false;
    bool processed_all_task_flag = false;
    bool sent_all_task_flag = false;

    //1.initiated connection
    boost::asio::io_service hashmap_io;
    boost::asio::ip::tcp::socket hashmap_socket(hashmap_io);
    hashmap_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3310));
   
    
    //3. initiate the connection to task_distributer port of the loadbalancer
    //4. receive task and put into the task queue

     
    std::thread(connect_loadbalancer,std::ref(task_queue),std::ref(task_queue_mutex)).detach();
   
    //while not receive all 
    while(true){
        {
           
            //lock task_queue mutex 
            {   
                task_queue_mutex.lock();
                 
                if (task_queue.size()>0){
                    auto [index, values] = task_queue.front();
                    task_queue.pop();
                    task_queue_mutex.unlock();

                    std::cout<<"non empty task_queue"<<std::endl;
                    //unlock mutex 
                    long long int sum = std::accumulate(values.begin(), values.end(), 0);

                    boost::asio::write(hashmap_socket,boost::asio::buffer(&index,sizeof(int)));

                    boost::asio::write(hashmap_socket,boost::asio::buffer(&sum,sizeof(long long int))); 
                } else {
                    task_queue_mutex.unlock();
                    std::cout<<"Empty task queue, retry after 500 ms"<<std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
                } 
            }
        } 
    } 
    /// TODO: close the connection to mainserver
    //one thread for listen to the loadbalancer and store in task queue
    //another thread for summing and send it to the mainserver.cpp  
} 
 
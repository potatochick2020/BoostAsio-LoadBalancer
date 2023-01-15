#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_io.hpp>     
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <chrono> 
#include <queue>
#include <numeric>
#include <iostream>
#include <iterator>

void task_listener(boost::uuids::uuid& uuid,std::queue<std::pair<int,std::vector<int>>>& task_queue, std::mutex& task_queue_mutex ){
    boost::asio::io_service task_listener_io;
    boost::asio::ip::tcp::socket task_listener_socket(task_listener_io); 
    task_listener_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8830)); 
    boost::asio::write(task_listener_socket,boost::asio::buffer(&uuid,sizeof(uuid))); 
    boost::system::error_code ec;
    while (true){ 
        int index;
        std::vector<int> values(1000);  
        //std::cout<<"start read index"<<std::endl;
        boost::asio::read(task_listener_socket, boost::asio::buffer(&index, sizeof(int)), boost::asio::transfer_exactly(sizeof(int)),ec);
        boost::asio::read(task_listener_socket, boost::asio::buffer(values),ec);
        
        std::cout<<"end read task "<<index<<" "<<values[0]<<" "<<values[1]<<std::endl;

        if (!ec){  
            {
                std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex);  
                task_queue.push({std::move(index),std::move(values)}); 
            }
        } else {
            std::cout<<"break!!!!!"<<std::endl;
            break;
        }
    }   
    std::cout<<"end task_listener"<<std::endl;
}

void send_workload(boost::asio::ip::tcp::socket loadbalancer_socket, boost::uuids::uuid& uuid,std::queue<std::pair<int,std::vector<int>>>& task_queue, std::mutex& task_queue_mutex ){

    while (true){  
        //send queue size to port 8820
        size_t task_queue_size;
        {
            std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex);  
            //send queue size to port 3310
            task_queue_size = task_queue.size();
        } 
        std::cout<<"SEND : task_queue_size "<<task_queue_size<<std::endl;
        boost::asio::write(loadbalancer_socket,boost::asio::buffer(&task_queue_size,sizeof(size_t)));
 
    }  
}
void connect_loadbalancer(std::queue<std::pair<int,std::vector<int>>>& task_queue,std::mutex& task_queue_mutex){
    boost::asio::io_service loadbalancer_io;
    boost::asio::ip::tcp::socket loadbalancer_socket(loadbalancer_io);
    //Initiate connection to loadbalancer
    loadbalancer_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8820)); 
    boost::uuids::uuid uuid;
    boost::asio::read(loadbalancer_socket,boost::asio::buffer(&uuid,sizeof(boost::uuids::uuid)),boost::asio::transfer_exactly(sizeof(boost::uuids::uuid)));
    std::thread task_listener_thread(task_listener,std::ref(uuid),std::ref(task_queue),std::ref(task_queue_mutex));
    std::thread send_workload_thread(send_workload,std::move(loadbalancer_socket),std::ref(uuid),std::ref(task_queue),std::ref(task_queue_mutex));
    task_listener_thread.join();
    send_workload_thread.join();
}

void connect_hashmap_server(std::queue<std::pair<int,std::vector<int>>>& task_queue,std::mutex& task_queue_mutex){

    boost::asio::io_service hashmap_io;
    boost::asio::ip::tcp::socket hashmap_socket(hashmap_io);
    hashmap_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3310));
    
    std::atomic<bool> temp = false;
    while (true){ 
            temp = false;
            //lock task_queue mutex 
            {   
                std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex);
                 
                if (!task_queue.empty()){  
                    int index = std::move(task_queue.front().first); 
                    std::vector<int> values = std::move(task_queue.front().second); 
                    task_queue.pop(); 

                    //unlock mutex 
                    boost::asio::write(hashmap_socket,boost::asio::buffer(&index,sizeof(int)));
                    std::cout<<"send index to hashmap : "<<index<<std::endl; 
                    long long int sum = 0;
                    for (auto value:values) { 
                        sum+=value; 
                    } 
                    boost::asio::write(hashmap_socket,boost::asio::buffer(&sum,sizeof(long long int))); 
                    std::cout<<"send sum to hashmap : "<<sum<<std::endl; 
                    std::cout<<"task queue size :"<<task_queue.size()<<std::endl;
                } else { 
                    temp = true; 
                } 
            }
            if (temp){ 
                std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
            }
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
   
    
    //3. initiate the connection to task_distributer port of the loadbalancer
    //4. receive task and put into the task queue

     
    std::thread connect_loadbalancer_thread(connect_loadbalancer,std::ref(task_queue),std::ref(task_queue_mutex));
    std::thread connect_hashmap_server_thread(connect_hashmap_server,std::ref(task_queue),std::ref(task_queue_mutex));
    connect_loadbalancer_thread.join();
    connect_hashmap_server_thread.join();
     
    //while not receive all 
    
    /// TODO: close the connection to mainserver
    //one thread for listen to the loadbalancer and store in task queue
    //another thread for summing and send it to the mainserver.cpp  
} 
 
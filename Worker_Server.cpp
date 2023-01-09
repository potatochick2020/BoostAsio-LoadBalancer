//worker_server will connect to the load balancer 
//load balancer will then ping a uuid to the worker_server
//the worker_server will then seperate into two thread 
    // one connect to the loadbalancer, send the uuid back to the load balancer, and wait for the load balancer to send query of task queue size to know the workload.
    // another one connect to the loadbalancer to get the task
    
#include <boost/uuid/uuid.hpp>            // uuid class 
#include <boost/uuid/uuid_io.hpp>     
#include <boost/asio.hpp>
#include <vector>
#include <chrono>
using boost::asio::ip::tcp;

//worker_server will connect to the load balancer 
//load balancer will then ping a uuid to the worker_server
//the worker_server will then seperate into two thread 
    // one connect to the loadbalancer, send the uuid back to the load balancer, and wait for the load balancer to send query of task queue size to know the workload.
    // another one connect to the loadbalancer to get the task
 
void process_task(){
    while(!received_all_task_flag){
        {
            //lock task_queue mutex
            auto size = task_queue.size();
            if (size>0){
                auto task = task_queue.top();
                task_queue.pop();
                //unlock mutex
                 
                long long int sum;
                std::accumulate(values.begin(), values.end(), 0);
                boost::asio::write(hashmap_socket, boost::asio::buffer({index,sum}));
            }
        
        }
        
        
    }
}
 
void add_task_to_task_queue(){
    while (){ // receive an end signal
        int index;
        std::vector<int> values(1000); 
        boost::asio::read(loadbalancer_socket, boost::asio::buffer({index,data}));  
        std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex);
        task_queue.push({index,values});
    }   
    received_all_task_flag = true;
}

void connect_loadbalancer(std::queue<pair<int,std::vector<int>>>& task_queue,std::mutex& task_queue_mutex){
    boost::asio::io_service loadbalancer_io;
    tcp::socket loadbalancer_socket(loadbalancer_io);
    //initiate connection to loadbalancer
    loadbalancer_socket.connect(tcp::endpoint({}, 3310)); 
    boost::uuids::uuid uuid; 
    boost::asio:read(socket,boost::asio::buffer(uuid));

    boost::asio::io_service task_listener_io;
    tcp::socket task_listener_socket(task_listener_io);



    std::thread add_task_to_task_queue_thread();
    while (received_all_task_flag == false){
        //sleep for 1000 ms 
        //send queue size to port 3310
    }
    add_task_to_task_queue_thread.join();

    
    while (){
        //sleep for 1000 ms
        std::this_thread::
        //send queue size to port 3310
    }

}

int main()
{
    
    std::queue<pair<int,std::vector<int>>> task_queue;
    std::mutex task_queue_mutex; 
    bool received_all_task_flag = false;
    bool processed_all_task_flag = false;
    bool sent_all_task_flag = false;

    boost::asio::io_service hashmap_io;
    tcp::socket hashmap_socket(hashmap_io);
    s.connect(tcp::endpoint({}, 8020));

    std::thread connect_loadbalancer_thread(add_task_to_task_queue,task_queue,task_queue_mutex);
    //while not eof 
    std::thread process_task_thread();
    process_task_thread.join();
    connect_loadbalancer.join();
    long long int sum;
    std::accumulate(values.begin(), values.end(), 0);
    boost::asio::write(hashmap_socket, boost::asio::buffer({idnex,sum}));
    
    //one thread for listen to the loadbalancer and store in task queue
    //another thread for summing and send it to the mainserver.cpp 




}

/*
  io_service service;

  // Create a socket and connect it to the server
  tcp::socket socket(service);
  socket.connect(tcp::endpoint(address::from_string("127.0.0.1"), 1234));

  // At this point, you can communicate with the server using the socket.
  std::cout << "Connected to server at " << socket.remote_endpoint() << std::endl;

  // You can also use the socket to send and receive data to/from the server.
  // For example:
  while (true){
    std::string mes; std::cin>>mes;
    std::cout<<mes;
    if (mes == "close"){
        socket.close();
        break;
    } 
    boost::asio::streambuf buffer;
    std::ostream output(&buffer);
    output << mes << "\r\n";
    boost::asio::write(socket, buffer);
    std::cout << "Sent message to server" << std::endl;
  } 
  */
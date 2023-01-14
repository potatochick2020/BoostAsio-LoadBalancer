#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_io.hpp>     
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <chrono> 
#include <queue>
#include <numeric>

void connect_loadbalancer(std::queue<std::pair<int,std::vector<int>>>& task_queue,std::mutex& task_queue_mutex){
    boost::asio::io_service loadbalancer_io;
    boost::asio::ip::tcp::socket loadbalancer_socket(loadbalancer_io);
    //Initiate connection to loadbalancer
    loadbalancer_socket.connect(boost::asio::ip::tcp::endpoint({}, 8820)); 
    

    boost::asio::io_service task_listener_io;
    boost::asio::ip::tcp::socket task_listener_socket(task_listener_io);
    task_listener_socket.connect(boost::asio::ip::tcp::endpoint({}, 8830)); 
    /// TODO: Send uuid to 8830 port 
    /// TODO: seperate below to two threads
    while (true){ // receive an end signal
        int index;
        std::vector<int> values(1000); 
        
        boost::asio::read(task_listener_socket, boost::asio::buffer(&index, sizeof(int)), boost::asio::transfer_exactly(sizeof(int)));
        boost::asio::read(task_listener_socket, boost::asio::buffer(&values, sizeof(std::vector<int>(1000))), boost::asio::transfer_exactly(sizeof(std::vector<int>(1000))));

        std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex); 
        task_queue.push({index,values}); 
    }   
    bool received_all_task_flag = true; 
    while (received_all_task_flag == false){ 
        //sleep for 1000 ms
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
    hashmap_socket.connect(boost::asio::ip::tcp::endpoint({}, 3310));
    //2. receive an uuid
    boost::uuids::uuid uuid; 
    boost::asio::read(hashmap_socket,boost::asio::buffer(uuid));
    //3. initiate the connection to task_distributer port of the loadbalancer
    //4. receive task and put into the task queue
    std::thread(connect_loadbalancer,task_queue,task_queue_mutex).detach();

    //while not receive all 
    while(!received_all_task_flag){
        {
            //lock task_queue mutex 
            {
                std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex);   
                if (task_queue.size()>0){
                    auto [index, values] = task_queue.front();
                    task_queue.pop();
                    //unlock mutex 
                    long long int sum = std::accumulate(values.begin(), values.end(), 0);

                    boost::asio::write(hashmap_socket,boost::asio::buffer(&index,sizeof(int)));

                    boost::asio::write(hashmap_socket,boost::asio::buffer(&sum,sizeof(long long int))); 
                } 
            }
        } 
    } 
    /// TODO: close the connection to mainserver
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
 
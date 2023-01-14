#include <boost/asio.hpp>
#include <iostream> 

int main()
{
    boost::asio::io_service service;

    // Create a socket and connect it to the server
     boost::asio::ip::tcp::socket socket(service);
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 3320));

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
        boost::asio::write(socket, boost::asio::buffer(mes));
        std::cout << "Sent message to server" << std::endl;
        
        boost::asio::streambuf streambuffer;
        boost::asio::read(socket, streambuffer,boost::asio::transfer_exactly(sizeof(long long int)));
        long long int response = 0;
        std::istream(&streambuffer) >> response;
        std::cout << "Receiver message from server"<<response << std::endl;
    } 
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


/*
Validation:
make sure sent message is an int  

close connection by sending a close signal '\n'
*/
#include <boost/asio.hpp>
#include <iostream>
using boost::asio::ip::tcp;

int main()
{
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
        boost::asio::write(socket, boost::asio::buffer(mes));
        std::cout << "Sent message to server" << std::endl;
        long long int response;
        boost::asio::read(socket, boost::asio::buffer(response));
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
#include <boost/asio.hpp>
#include <iostream>

int main()
{
  boost::asio::io_service service;

  // Create a socket and connect it to the server
  boost::asio::ip::tcp::socket socket(service);
  socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3320));

  // At this point, you can communicate with the server using the socket.
  std::cout << "Connected to server at " << socket.remote_endpoint() << std::endl;


  bool close_connection_flag = false;
  
  while (true)
  { 
    std::cout<<"Please input an integer: ";
    std::string mes = "";
    std::cin >> mes; 
    if (mes == "close")
    {
      socket.close();
      break;
    }
    else
    { 
      bool input_check_flag = true;
      for (int i = 0; i < mes.size(); i++)
      {
        if (!isdigit(mes[i]))
        {
          input_check_flag = false;
          std::cout<<"please input integer"<<std::endl;
          break;
        }
      } 
      if (input_check_flag)
      {
        int index_query = stoi(mes);
        boost::asio::write(socket, boost::asio::buffer(&index_query,sizeof(int)));
        std::cout << "Sent message to server: " <<index_query<< std::endl;

        long long int reply;
        boost::asio::read(socket, boost::asio::buffer(&reply, sizeof(long long int)), boost::asio::transfer_exactly(sizeof(long long int)));
        std::cout << "Receive message from server: " << reply << std::endl<<std::endl;
      }
    }
  }
}
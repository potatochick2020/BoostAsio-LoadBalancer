#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <iostream>

TEST(MainTest, TestConnection) {
    boost::asio::io_service service;
    boost::asio::ip::tcp::socket socket(service);
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3320));
    EXPECT_TRUE(socket.is_open());
    socket.close();
}

TEST(MainTest, TestInputCheck) {
    std::string mes = "abc";
    bool input_check_flag = true;
    for (int i = 0; i < mes.size(); i++)
    {
        if (!isdigit(mes[i]))
        {
            input_check_flag = false;
            break;
        }
    } 
    EXPECT_FALSE(input_check_flag);
}

TEST(MainTest, TestMessageSending) {
    boost::asio::io_service service;
    boost::asio::ip::tcp::socket socket(service);
    socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3320));
    int index_query = 5;
    boost::asio::write(socket, boost::asio::buffer(&index_query,sizeof(int)));
    long long int reply;
    boost::asio::read(socket, boost::asio::buffer(&reply, sizeof(long long int)), boost::asio::transfer_exactly(sizeof(long long int)));
    EXPECT_EQ(index_query, reply);
    socket.close();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include <gtest/gtest.h>
#include "file.hpp"

TEST(task_listener, test_task_listener_reads_tasks_from_socket) {
    boost::uuids::uuid uuid;
    std::queue<std::pair<int,std::vector<int>>> task_queue;
    std::mutex task_queue_mutex;
    std::thread task_listener_thread(task_listener,std::ref(uuid),std::ref(task_queue),std::ref(task_queue_mutex));

    // Create a mock socket that simulates incoming tasks
    // Add a few tasks to the socket
    std::vector<int> values1 = {1, 2, 3};
    std::vector<int> values2 = {4, 5, 6};
    std::vector<std::pair<int, std::vector<int>>> expected_tasks = {{1, values1}, {2, values2}};
    for (auto task : expected_tasks) {
        // Write the index and values to the socket
        int index = task.first;
        boost::asio::write(mock_socket, boost::asio::buffer(&index, sizeof(int)));
        boost::asio::write(mock_socket, boost::asio::buffer(task.second));
    }

    // Wait for the task listener thread to finish
    task_listener_thread.join();

    // Check that the task queue contains the expected tasks
    for (auto expected_task : expected_tasks) {
        std::pair<int,std::vector<int>> task;
        {
            std::lock_guard<std::mutex> task_queue_lock(task_queue_mutex);  
            task = task_queue.front();
            task_queue.pop();
        }
        EXPECT_EQ(task.first, expected_task.first);
        EXPECT_EQ(task.second, expected_task.second);
    }
}

TEST(TaskListenerTest, CorrectTaskIsAddedToQueue) {
    boost::uuids::uuid mock_uuid;
    std::queue<std::pair<int,std::vector<int>>> task_queue;
    std::mutex task_queue_mutex;
    // Add a mock task to the queue that the task listener should receive
    std::pair<int,std::vector<int>> mock_task = {0, {1, 2, 3, 4, 5}};
    task_listener(mock_uuid, task_queue, task_queue_mutex);
    // Verify that the task has been added to the queue
    EXPECT_EQ(task_queue.size(), 1);
    EXPECT_EQ(task_queue.front(), mock_task);
}

TEST(SendWorkloadTest, CorrectQueueSizeIsSent) {
    boost::asio::ip::tcp::socket mock_socket;
    boost::uuids::uuid mock_uuid;
    std::queue<std::pair<int,std::vector<int>>> task_queue;
    std::mutex task_queue_mutex;
    // Add some mock tasks to the queue
    task_queue.push({1, {1, 2, 3}});
    task_queue.push({2, {4, 5, 6}});
    // Create a spy to check the value sent to the load balancer
    auto spy = std::make_shared<testing::NiceMock<MockSocket>>();
    EXPECT_CALL(*spy, write(_, _))
        .WillOnce(testing::Invoke([](const void* data, size_t size) {
            EXPECT_EQ(*static_cast<const size_t*>(data), task_queue.size());
        }));
    send_workload(spy, mock_uuid, task_queue, task_queue_mutex);
}

TEST(ConnectLoadBalancerTest, UUIDReadFromSocketIsCorrect) {
// Set up a mock socket to return a specific UUID
std::string expected_uuid = "123e4567-e89b-12d3-a456-426655440000";
auto mock_socket = std::make_shared<MockSocket>();
EXPECT_CALL(*mock_socket, Read(_, _)).WillOnce(Return(expected_uuid));

// Create a load balancer instance
ConnectLoadBalancer lb(mock_socket);

// Read the UUID from the socket and check that it matches the expected value
std::string actual_uuid = lb.ReadUUIDFromSocket();
EXPECT_EQ(actual_uuid, expected_uuid);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
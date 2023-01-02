# BoostAsio-LoadBalancer
This is a education project for myself to learn boost asio, google test, and google bench mark.

A loadbalancer which distributed task to sum up multiple `vector<int>` and insert the index and corresponding sum to a `hash table`.

# Generate datasheet :  
- This will generate a txt file named `datasheet-{i}.txt` which `{i}` is an auto increment number starting from 1
- The first number in each row is the index of the map that will be inserted ranged from `0` to `65535`
- Followed by `1000` number which will be used to calculated as the sum to insert as the coressponding value of the index ranged from `-32768` to `32767`  


### Run the following code : 
```
python3 data.py
```

# Compile the program 
### Run the following code :  
(without make at the moment, will include in the future)
```
c++ -std="c++20" -I /usr/local/boost_1_81_0  Client.cpp -o client
c++ -std="c++20" -I /usr/local/boost_1_81_0  Server.cpp -o server
```

# Explanation of each file 
## client.cpp 
- a client program which connect to `mainserver.cpp ` for querying by providing the index and receive the value stored in the hashmap
## mainserver.cpp 
- Include 2 seperated server program 
    - A hashmap server which receive index and sum of data from `Worker_Server.cpp`, then store it to the hashmap
    - A query server which receive the index from `client.cpp` and return the the corresponding value in the hashmpa 
## loadbalancer.cpp 
- read in data from 
- send the serialized data to `worker_server.cpp`
## Worker_Server.cpp 
client.cpp -> for querying the hash map
mainserver.cpp -> for storing the hashmap and answer client.cpp query
worker_server.cpp -> sum up the vector and return a int index and long long int sum
loadbalancer.cpp -> read in the txt, send the vector and 



loadbalncer read in the data from datasheet, send an index ranged from , with a 10,000 int vector ranged from -32,768 to +32,767 (signed 16 bit) and send it to the server with the leasted load, server then return the index and the sum of vector to the mainserver.cpp.



# port

loadbalancer port 8010 (for worker connect to loadbalancer server)
hashmap server port 8020 (for worker connect to hadhmap server)

 



# To-Do
- Implement a logger
- use preprocessor to define the the port   
    - loadbalancer port
    - hashmap server mainserver port
    - query server port 

## client.cpp
- get the reply and print it 
- validation on valid index

## mainserver.cpp
- receive message from worker and put it into the hashmap
- write back feedback to query
## loadbalancer.cpp
- Least-loaded implementation
- find the lowest workload mainserver
## Worker_Server.cpp
- use read until ? 
- the while statement not yet finish 

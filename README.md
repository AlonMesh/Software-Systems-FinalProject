## Software-Systems Final Project
This project contains implementations of the sieve of Eratosthenes for prime number generation, as well as implementations of clients and servers to demonstrate network communication. You can run the sieve program independently to generate prime numbers, or you can use the clients and servers together to explore client-server interactions

### Build the Project

Run the following command to compile the project:
```bash
make all
```

### Running the Sieve Program

To run the sieve program:
```bash
./build/sieve n
```
Where `n` can be any number.

### Running the CLients

#### Running a single Client

To run the client:
```bash
./build/client 127.0.0.1 12345 42 1
```

Where:
* `127.0.0.1` is the IP address of the server
* `12345` is the port number where the server is listening for incoming connections
* `42` is the seed value for the random number generator used by the client
* `1` is the delay (in seconds) between requests

#### Running the Launcher (A group of clients)

To run a launcher that starts multiple clients:
```bash
./build/launcher 127.0.0.1 12345 5 1
```

Where:
* `127.0.0.1` is the IP address of the server
* `12345` is the port number where the server is listening for incoming connections
* `5` is the number of clients to be launched
* `1` is the delay (in seconds) between requests

### Running the Servers

#### Standard Server

To run the standard server:
```bash
./build/server 12345
```

#### Pipeline-Based Server

To run the pipeline-based server:
```bash
./build/pipeline 12345
```

#### Producer-Consumer-Based Server

To run the producer-consumer-based server:
```bash
./build/producer 12345
```

In each case, `12345` is the port number where the server will be listening for incoming connections. Adjust the port number as needed for your setup.
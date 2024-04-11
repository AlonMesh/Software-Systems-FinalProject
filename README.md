Run:

```bash
make all
```

-----

Then:
```bash
/build/sieve 12
```

Or:
```bash
./build/test
```

-----

To run the server:
```bash
./build/server 12345
```

To run the client:
```bash
./build/client 127.0.0.1 12345 42 1
```

Where:
* `127.0.0.1` is the IP address of the server
* `12345` is the port number where the server is listening for incoming connections
* `42` is the seed value for the random number generator used by the client
* `1` is the delay (in seconds) between requests

To run a launcher:
```bash
./build/launcher 127.0.0.1 12345 5 1
```

Where:
* `127.0.0.1` is the IP address of the server
* `12345` is the port number where the server is listening for incoming connections
* `5` is the number of clients
* `1` is the delay (in seconds) between requests


---------

```bash
make pipeline
```

```bash

```
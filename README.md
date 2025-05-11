# Rusty KV CLI

A command-line interface client for interacting with the Rusty Key-Value store
server.

## Building the Project

### Prerequisites

- CMake (version 3.10 or higher)
- C++ compiler with C++11 support
- Make

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/dtg-lucifer/rusty-kv-cli.git
   cd rusty-kv-cli
   ```

2. Create a build directory and navigate to it:
   ```bash
   mkdir build
   cd build
   ```

3. Generate the build files with CMake:
   ```bash
   cmake ..
   ```

4. Build the project with Make:
   ```bash
   make
   ```

5. The executable will be generated in the build directory.

## Usage

To connect to a server, you can use either individual command line arguments or
a connection URL:

### Using Individual Arguments

```bash
./rusty-kv-cli -h <host> -p <port> -U <username> -P <password>
```

Arguments:

- `-h`: Server hostname or IP address (default: 127.0.0.1)
- `-p`: Server port (default: 6379)
- `-U`: Username for authentication (optional)
- `-P`: Password for authentication (optional)

### Using Connection URL

```bash
./rusty-kv-cli -url kv://<username>:<password>@<host>:<port>
```

Examples:

```bash
# Connect to localhost with default port
./rusty-kv-cli -h 127.0.0.1 -p 6379

# Connect with username and password
./rusty-kv-cli -U admin -P secret -h 192.168.1.100 -p 6379

# Using connection URL
./rusty-kv-cli -url kv://admin:secret@192.168.1.100:6379
```

Once connected, you can enter commands at the prompt:

```
192.168.1.100:6379> SET mykey "Hello World"
OK
192.168.1.100:6379> GET mykey
"Hello World"
192.168.1.100:6379> exit
```

Type `exit` or `quit` to disconnect from the server and exit the program.

## Redis Command Examples

Here are some common Redis commands you can try:

```
> SET key value
> GET key
> DEL key
> EXISTS key
> INCR counter
> LPUSH mylist value
> LRANGE mylist 0 -1
> HSET myhash field value
> HGET myhash field
> PING
```

## Notes

- This is a simple client and doesn't implement the full RESP (Redis
  Serialization Protocol)
- Commands are sent as raw strings
- For more advanced features, consider using a full Redis client library

## License

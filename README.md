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

To connect to a server:

```bash
./tmp/cli <host> <port>
```

Once connected, you can enter commands at the prompt:

```
127.0.0.1:6379> SET mykey "Hello World"
OK
127.0.0.1:6379> GET mykey
"Hello World"
127.0.0.1:6379> exit
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

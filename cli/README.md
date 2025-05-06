# Redis CLI Client in C++

A simple command-line interface client for interacting with Redis servers.

## Overview

This Redis CLI client is a lightweight C++ application that allows you to
connect to a Redis server and execute Redis commands through an interactive
shell.

## Features

- Connect to any Redis server by specifying host and port
- Interactive command interface
- Simple and lightweight implementation
- Direct command transmission to Redis server
- Response display from server

## Building the Client

To build the Redis CLI client, you need a C++ compiler with C++11 support.

```bash
# Compile with g++
g++ -o redis-cli main.cpp -std=c++11

# Or with clang
clang++ -o redis-cli main.cpp -std=c++11
```

## Usage

```bash
# Connect to local Redis server (default: 127.0.0.1:6379)
./redis-cli

# Connect to specific host
./redis-cli 192.168.1.100

# Connect to specific host and port
./redis-cli 192.168.1.100 7000
```

Once connected, you will see a prompt where you can type Redis commands:

```
Redis client connected. Type commands, or 'exit' to quit.
> SET mykey "Hello, Redis!"
> GET mykey
> exit
```

To exit the client, type `exit` or `quit`.

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

[Insert your license information here]

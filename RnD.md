## High-Level Workflow

The workflow of this rusty-kv server follows this sequence:

1. **Server Initialization**
   - Sets up logging
   - Loads environment configuration
   - Binds to a network port

2. **Connection Acceptance Loop**
   - Continuously accepts client connections
   - Spawns a separate async task for each connection

3. **Command Processing**
   - Reads commands from clients
   - Processes commands through a command handler system
   - Sends responses back to clients

## Detailed Workflow

### 1. Server Initialization

```rust
#[tokio::main]
async fn main() {
  // Initialize logging
  SimpleLogger::new().init().unwrap();

  // Load environment variables
  dotenv().unwrap_or_else(|_| { /* error handling */ });

  // Configure server address
  let kv_host = std::env::var("KV_HOST").unwrap_or_else(|_| "127.0.0.1".to_string());
  let kv_port = std::env::var("KV_PORT").unwrap_or_else(|_| "6379".to_string());

  // Bind TCP listener
  let listener = TcpListener::bind(format!("{}:{}", kv_host, kv_port)).await.unwrap();
```

### 2. Connection Handling Loop

```rust
loop {
  let stream = listener.accept().await;
  match stream {
    Ok((stream, addr)) => {
      // Spawn a separate task for each connection
      tokio::spawn(async move {
        if let Err(e) = accept_connection(stream).await {
          error!("Error handling connection: {}", e);
        }
      });
      info!("Accepted a new connection from {}", addr);
    }
    // Error handling...
  }
}
```

### 3. Connection Processing

```rust
async fn accept_connection(mut socket: TcpStream) -> io::Result<()> {
  loop {
    // Read data
    let mut buffer = [0; 1024];
    match socket.read(&mut buffer).await {
      // Handle connection closed
      // Process command
      // Send response
    }
  }
}
```

### 4. Command Processing

```rust
// In the accept_connection function:
if let Some(command) = commands::get_command(&request) {
  command.execute(&mut socket).await?;
} else {
  socket.write_all(b"-ERR unknown command\r\n").await?;
}
```

## Design Architecture

The code uses the following design patterns:

1. **Command Pattern**: Commands are encapsulated as objects that implement the
   `Command` trait
2. **Factory Method**: The `get_command` function creates appropriate command
   objects
3. **Asynchronous Processing**: Uses Tokio for async I/O and task management

## Maintainability Tips

To make this Redis clone more maintainable as you add features:

1. **Command Organization**:
   - Keep each command in its own file (as already done with ping.rs and
     echo.rs)
   - Group related commands in subdirectories (e.g., `commands/string/`,
     `commands/hash/`)

2. **Error Handling**:
   - Create custom error types to better handle Redis-specific errors
   - Add proper error messages that match Redis error formats

3. **Protocol Implementation**:
   - Implement the RESP (Redis Serialization Protocol) properly
   - Create separate parser and serializer modules

4. **Data Storage**:
   - Separate data storage logic from command handling
   - Implement different storage engines (in-memory, persistent)

5. **Testing**:
   - Add unit tests for each command
   - Create integration tests that verify Redis compatibility

6. **Configuration**:
   - Move configuration to a dedicated module
   - Support Redis-compatible configuration options

7. **Performance**:
   - Add metrics and monitoring
   - Implement connection pooling
   - Consider using more efficient buffer handling

8. **Documentation**:
   - Document each command implementation
   - Document the Redis compatibility level

This project uses Rust's async features via Tokio to handle multiple connections
efficiently, which is a good foundation for building a high-performance Redis
clone.

dtg-lucifer: What are common Redis commands?

GitHub Copilot: # Common Redis Commands

Redis has a rich command set. Here are the most common Redis commands by
category:

## String Commands

- `SET key value` - Store a string value
- `GET key` - Retrieve a string value
- `DEL key` - Delete a key
- `EXISTS key` - Check if a key exists
- `INCR key` - Increment a numeric value
- `DECR key` - Decrement a numeric value
- `EXPIRE key seconds` - Set key expiration time

## List Commands

- `LPUSH key value` - Add to the left of a list
- `RPUSH key value` - Add to the right of a list
- `LPOP key` - Remove from the left of a list
- `RPOP key` - Remove from the right of a list
- `LRANGE key start stop` - Get a range of elements

## Hash Commands

- `HSET key field value` - Set a field in a hash
- `HGET key field` - Get a field from a hash
- `HGETALL key` - Get all fields and values in a hash
- `HDEL key field` - Delete a field from a hash

## Set Commands

- `SADD key member` - Add to a set
- `SMEMBERS key` - Get all members of a set
- `SREM key member` - Remove from a set
- `SISMEMBER key member` - Check if a value is in a set

## Sorted Set Commands

- `ZADD key score member` - Add to a sorted set
- `ZRANGE key start stop` - Get range of members by index

## Connection Commands

- `PING` - Test connection
- `AUTH password` - Authenticate client
- `ECHO message` - Echo the given string

## Server Commands

- `INFO` - Get server information
- `SELECT index` - Select database index
- `FLUSHDB` - Remove all keys from current database
- `FLUSHALL` - Remove all keys from all databases

Implementing these commands would give your Redis clone a good foundation of
functionality while you work toward full compatibility.

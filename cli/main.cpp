#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class RedisClient
{
private:
  int socket_fd;
  struct sockaddr_in server_addr;
  bool connected;

  static const int BUFFER_SIZE = 1024;

public:
  RedisClient() : socket_fd(-1), connected(false)
  {
    memset(&server_addr, 0, sizeof(server_addr));
  }

  ~RedisClient()
  {
    disconnect();
  }

  bool connect(const std::string &host, int port)
  {
    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
      std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
      return false;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0)
    {
      std::cerr << "Invalid address: " << strerror(errno) << std::endl;
      close(socket_fd);
      return false;
    }

    // Connect to server
    if (::connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
      std::cerr << "Connection failed: " << strerror(errno) << std::endl;
      close(socket_fd);
      return false;
    }

    connected = true;
    std::cout << "Connected to " << host << ":" << port << std::endl;
    return true;
  }

  void disconnect()
  {
    if (socket_fd >= 0)
    {
      close(socket_fd);
      socket_fd = -1;
      connected = false;
    }
  }

  bool isConnected() const
  {
    return connected;
  }

  bool sendCommand(const std::string &command)
  {
    if (!connected)
    {
      std::cerr << "Not connected to server" << std::endl;
      return false;
    }

    ssize_t bytes_sent = send(socket_fd, command.c_str(), command.length(), 0);
    if (bytes_sent < 0)
    {
      std::cerr << "Error sending command: " << strerror(errno) << std::endl;
      return false;
    }

    return true;
  }

  std::string receiveResponse()
  {
    if (!connected)
    {
      return "Not connected to server";
    }

    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_received < 0)
    {
      std::cerr << "Error receiving response: " << strerror(errno) << std::endl;
      return "Error receiving response";
    }

    return std::string(buffer);
  }
};

int main(int argc, char *argv[])
{
  std::string host = "127.0.0.1";
  int port = 6379;

  // Parse command line arguments
  if (argc >= 2)
  {
    host = argv[1];
  }
  if (argc >= 3)
  {
    port = std::stoi(argv[2]);
  }

  RedisClient client;

  if (!client.connect(host, port))
  {
    return 1;
  }

  std::string command;

  std::cout << "Redis client connected. Type commands, or 'exit' to quit.\n";

  while (true)
  {
    std::cout << "> ";
    std::getline(std::cin, command);

    if (command == "exit" || command == "quit")
    {
      break;
    }

    if (client.sendCommand(command))
    {
      std::string response = client.receiveResponse();
      std::cout << response;
    }
  }

  client.disconnect();
  std::cout << "Disconnected from server." << std::endl;

  return 0;
}

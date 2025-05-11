#include "client.hpp"

bool KvClient::connect(KvConnectionInfo& info) {
  // @INFO Create socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cerr << "Error creating socket: " << strerror(ECONNABORTED) << std::endl;
    return false;
  }

  // @INFO Set up server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(info.port);

  if (inet_pton(AF_INET, info.host.c_str(), &server_addr.sin_addr) <= 0) {
    std::cerr << "Invalid address: " << strerror(ECONNREFUSED) << std::endl;
    close(socket_fd);
    return false;
  }

  // @INFO Connect to server
  if (::connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Connection failed: " << strerror(ECONNABORTED) << std::endl;
    close(socket_fd);
    return false;
  }

  connected = true;
  std::cout << "Connected to " << info.host << ":" << info.port << std::endl;
  this->addr = info.host + ":" + std::to_string(info.port);
  return true;
}

void KvClient::disconnect() {
  if (socket_fd >= 0) {
    close(socket_fd);
    socket_fd = -1;
    connected = false;
    this->addr = "";
  }
}

bool KvClient::isConnected() const {
  return connected;
}

bool KvClient::sendCommand(const std::string& command) {
  if (!connected) {
    std::cerr << "Not connected to server" << std::endl;
    return false;
  }

  ssize_t bytes_sent = send(socket_fd, command.c_str(), command.length(), 0);
  if (bytes_sent < 0) {
    std::cerr << "Error sending command: " << strerror(errno) << std::endl;
    return false;
  }

  return true;
}

std::string KvClient::receiveResponse() {
  if (!connected) {
    std::cerr << "Not connected to server" << std::endl;
    return "Not connected to server";
  }

  char buffer[BUFFER_SIZE] = {0};
  ssize_t bytes_received = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);

  if (bytes_received < 0) {
    std::cerr << "Error receiving response: " << strerror(errno) << std::endl;
    return "Error receiving response";
  }

  return std::string(buffer);
}

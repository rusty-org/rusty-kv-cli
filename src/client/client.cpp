#include "client.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../utils/logger.hpp"
#include "../utils/utils.hpp"

// Constructor
KvClient::KvClient() : sock(-1), connected(false), authenticated(false), socket_fd(-1) {}

// Destructor
KvClient::~KvClient() {
  disconnect();
}

bool KvClient::connect(const std::string& host, int port) {
  // @INFO Create socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::string error_msg = "Socket creation failed: " + std::string(strerror(errno));
    Logger::error(error_msg);
    return false;
  }

  // Set connection info
  connectionInfo.host = host;
  connectionInfo.port = port;

  // @INFO Set up server address
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
    std::string error_msg = "Invalid address: " + std::string(strerror(ECONNREFUSED));
    Logger::error(error_msg);
    close(socket_fd);
    return false;
  }

  // @INFO Connect to server
  if (::connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::string error_msg = "Connection failed: " + std::string(strerror(errno));
    Logger::error(error_msg);
    close(socket_fd);
    return false;
  }

  connected = true;
  Logger::info("Connected to server at " + host + ":" + std::to_string(port));
  this->addr = host + ":" + std::to_string(port);
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

bool KvClient::isAuthenticated() const {
  return authenticated;
}

void KvClient::setAuthenticated(bool __authenticated) {
  authenticated = __authenticated;
}

void KvClient::setConnectionInfo(const KvConnectionInfo& info) {
  connectionInfo = info;
}

void KvClient::setConnectionInfoFromAuthCommand(const std::string& authCommand) {
  // Parse the AUTH command to extract username and password
  std::vector<std::string> args = cmd::split(authCommand, ' ');
  if (args.size() > 1) {
    connectionInfo.setUser(args.size() > 1 ? args[1] : "");
    connectionInfo.setPassword(args.size() > 2 ? args[2] : "");
    connectionInfo.requireAuth = true;
  }
}

const KvConnectionInfo* KvClient::getConnectionInfo() const {
  return &connectionInfo;
}

std::string KvClient::getAddr() const {
  return connectionInfo.user.empty() ? addr : connectionInfo.user + "@" + addr;
}

bool KvClient::sendCommand(const std::string& command) {
  if (!connected) {
    Logger::error("Not connected to server");
    return false;
  }

  ssize_t bytes_sent = send(socket_fd, command.c_str(), command.length(), 0);
  if (bytes_sent < 0) {
    std::string error_msg = "Error sending command: " + std::string(strerror(errno));
    Logger::error(error_msg);
    return false;
  }

  return true;
}

std::string KvClient::receiveResponse() {
  if (!connected) {
    Logger::error("Not connected to server");
    return "Not connected to server";
  }

  char buffer[BUFFER_SIZE] = {0};
  ssize_t bytes_received = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);

  if (bytes_received < 0) {
    std::string err_msg = "Error receiving response: " + std::string(strerror(errno));
    Logger::error(err_msg);
    return "Error receiving response";
  }

  return std::string(buffer);
}

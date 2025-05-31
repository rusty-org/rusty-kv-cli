/**
 * @file client.cpp
 * @brief KvClient method implementations.
 */

#include "include/client.hpp"

#include "include/logger.hpp"
#include "include/utils.hpp"

// Constructor
KvClient::KvClient() : sock(-1), connected(false), authenticated(false), BUFFER_SIZE(1024), socket_fd(-1) {}

// Destructor ensures cleanup
KvClient::~KvClient() {
  disconnect();
}

/**
 * @brief Create TCP socket and connect to server.
 *
 * @param host Server address.
 * @param port Server port.
 * @return True if successful, false on error.
 */
bool KvClient::connect(const std::string& host, int port) {
  // @INFO Create socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::string error_msg = "Socket creation failed: " + std::string(strerror(ECONNABORTED));
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

/**
 * @brief Close the socket and update connection state.
 */
void KvClient::disconnect() {
  if (socket_fd >= 0) {
    close(socket_fd);
    socket_fd = -1;
    connected = false;
    this->addr = "";
  }
}

/** @copydoc KvClient::isConnected */
bool KvClient::isConnected() const {
  return connected;
}

/** @copydoc KvClient::isAuthenticated */
bool KvClient::isAuthenticated() const {
  return authenticated;
}

/** @brief Set authentication flag. */
void KvClient::setAuthenticated(bool authenticated) {
  this->authenticated = authenticated;
}

/** @brief Store connection parameters. */
void KvClient::setConnectionInfo(const KvConnectionInfo& info) {
  connectionInfo = info;
}

/**
 * @brief Extracts user and password from raw AUTH command.
 *
 * @param authCommand Raw "AUTH user pass" string.
 */
void KvClient::setConnectionInfoFromAuthCommand(const std::string& authCommand) {
  // Parse the AUTH command to extract username and password
  std::vector<std::string> args = cmd::split(authCommand, ' ');
  if (args.size() > 1) {
    connectionInfo.setUser(args.size() > 1 ? args[1] : "");
    connectionInfo.setPassword(args.size() > 2 ? args[2] : "");
    connectionInfo.requireAuth = true;
  }
}

/** @copydoc KvClient::getConnectionInfo */
const KvConnectionInfo* KvClient::getConnectionInfo() const {
  return &connectionInfo;
}

/**
 * @brief Returns prompt address, prefixed by user if set.
 *
 * @return String for CLI prompt.
 */
std::string KvClient::getAddr() const {
  return connectionInfo.user.empty() ? addr : connectionInfo.user + "@" + addr;
}

/**
 * @brief Send a RESP-formatted command over TCP.
 *
 * @param command RESP string.
 * @return True if send succeeded.
 */
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

/**
 * @brief Receive a single response chunk (up to BUFFER_SIZE-1).
 *
 * @return Response string or error message.
 */
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

  return std::string(buffer, bytes_received);
}

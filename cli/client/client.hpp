#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "../include/include.hpp"

class KvClient {
 private:
  int socket_fd;
  bool connected;
  struct sockaddr_in server_addr;
  std::string addr;

  static const int BUFFER_SIZE = 1024;

 public:
  KvClient() : socket_fd(-1), connected(false) {
    memset(&server_addr, 0, sizeof(server_addr));
    addr = "";
  }

  ~KvClient() { disconnect(); }

  // Connecting and disconnecting to the server
  void disconnect();
  bool connect(const std::string& host, int port);
  bool isConnected() const;

  // Sending and receiving data
  std::string receiveResponse();
  bool sendCommand(const std::string& command);

  // Getter for addr
  std::string getAddr() const { return addr; }
};

#endif  // _CLIENT_HPP_

#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "include/include.hpp"

struct KvConnectionInfo {
  std::string user;
  std::string password;
  std::string host;
  std::string url;
  int port;
};

class KvClient {
 private:
  int socket_fd{-1};
  bool connected{false};
  bool authenticated{false};
  std::string addr{""};
  struct sockaddr_in server_addr;
  struct KvConnectionInfo connection_info{
      .user = "",
      .password = "",
      .host = "",
      .url = "",
      .port = 0,
  };

  static const int BUFFER_SIZE = 1024;

 public:
  KvClient() { memset(&server_addr, 0, sizeof(server_addr)); }

  ~KvClient() { disconnect(); }

  // Connecting and disconnecting to the server
  void disconnect();
  bool connect(KvConnectionInfo& __info);
  bool isConnected() const;
  bool isAuthenticated() const;
  void setAuthenticated(bool __authenticated);
  void setConnectionInfo(KvConnectionInfo& __info);
  KvConnectionInfo getConnectionInfo();

  // Sending and receiving data
  std::string receiveResponse();
  bool sendCommand(const std::string& __command);

  std::string getAddr() const;
};

#endif  // _CLIENT_HPP_

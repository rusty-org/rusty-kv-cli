#ifndef _CLI_CLIENT_HPP_
#define _CLI_CLIENT_HPP_

#include "include/include.hpp"

// Define KvConnectionInfo before KvClient
class KvConnectionInfo {
 public:
  std::string host;
  int port;
  std::string user;
  std::string password;
  std::string url;  // <--- add this
  bool requireAuth;

  // Constructor with defaults
  KvConnectionInfo()
      : host("127.0.0.1"),
        port(6379),
        user(""),  // initialize user/password too
        password(""),
        url(""),  // initialize url
        requireAuth(false) {}
  void setUser(const std::string& u) { user = u; }
  void setPassword(const std::string& p) { password = p; }
};

class KvClient {
 private:
  std::string addr;
  int sock;
  bool connected;
  bool authenticated;
  KvConnectionInfo connectionInfo;
  int BUFFER_SIZE = 1024;  // Default buffer size for communication
  int socket_fd;           // File descriptor for the socket

 public:
  // Constructor
  KvClient();

  // Destructor
  ~KvClient();

  // Connection methods
  bool connect(const std::string& host, int port);
  void disconnect();

  // Getters
  bool isAuthenticated() const;
  bool isConnected() const;
  std::string getAddr() const;
  const KvConnectionInfo* getConnectionInfo() const;

  // Setters
  void setAuthenticated(bool auth);
  void setConnectionInfo(const KvConnectionInfo& info);
  void setConnectionInfoFromAuthCommand(const std::string& authCommand);

  // Communication
  bool sendCommand(const std::string& command);
  std::string receiveResponse();
};

#include "../utils/utils.hpp"  // Include utils after class definitions

#endif  // _CLI_CLIENT_HPP_

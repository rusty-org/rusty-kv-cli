/**
 * @file client.hpp
 * @brief KvConnectionInfo and KvClient class declarations.
 */

#ifndef _CLI_CLIENT_HPP_
#define _CLI_CLIENT_HPP_

#include "include.hpp"

/**
 * @class KvConnectionInfo
 * @brief Stores host, port, credentials, and URL for a client.
 */
class KvConnectionInfo {
 public:
  std::string host;     /**< Server hostname or IP */
  int port;             /**< Server port */
  std::string user;     /**< Username for AUTH */
  std::string password; /**< Password for AUTH */
  std::string url;      /**< Full connection URI */
  bool requireAuth;     /**< Indicates if AUTH is required */

  /**
   * @brief Default constructor initializes defaults.
   */
  KvConnectionInfo()
      : host("127.0.0.1"),
        port(6379),
        user(""),  // initialize user/password too
        password(""),
        url(""),  // initialize url
        requireAuth(false) {}

  /** @brief Set username. */
  void setUser(const std::string& user) { this->user = user; }

  /** @brief Set password. */
  void setPassword(const std::string& password) { this->password = password; }
};

/**
 * @class KvClient
 * @brief TCP client that communicates with the KV server via RESP.
 */
class KvClient {
 private:
  std::string addr;                /**< Address string for prompts */
  int sock;                        /**< Socket descriptor (unused) */
  bool connected;                  /**< Connection state flag */
  bool authenticated;              /**< AUTH state flag */
  KvConnectionInfo connectionInfo; /**< Connection parameters */
  int BUFFER_SIZE;                 /**< Size of receive buffer */
  int socket_fd;                   /**< Active socket file descriptor */

 public:
  /** @brief Default constructor. */
  KvClient();

  /** @brief Destructor; ensures disconnection. */
  ~KvClient();

  /** @name Connection methods */
  //@{
  bool connect(const std::string& host, int port);
  void disconnect();
  //@}

  /** @name State accessors */
  //@{
  bool isAuthenticated() const;
  bool isConnected() const;
  std::string getAddr() const;
  const KvConnectionInfo* getConnectionInfo() const;
  //@}

  /** @name State mutators */
  //@{
  void setAuthenticated(bool auth);
  void setConnectionInfo(const KvConnectionInfo& info);
  void setConnectionInfoFromAuthCommand(const std::string& authCommand);
  //@}

  /** @name Communication */
  //@{
  bool sendCommand(const std::string& command);
  std::string receiveResponse();
  //@}
};

#include "utils.hpp"  // CLI utilities

#endif  // _CLI_CLIENT_HPP_

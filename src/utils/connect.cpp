/**
 * @file connect.cpp
 * @brief Implements network::connect_to_client to parse args and open socket.
 */

#include "argument.hpp"
#include "client/client.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace network {

/**
 * @brief Parses CLI args, configures KvClient, and establishes socket.
 *
 * Exits on fatal errors (missing args or connection failure).
 *
 * @param argc CLI argument count.
 * @param argv CLI argument values.
 * @return Configured and connected KvClient.
 */
KvClient connect_to_client(int argc, char* argv[]) {
  // --------------------------------------------------
  // @INFO Parse the command line arguments
  // --------------------------------------------------
  KvClient client;
  KvConnectionInfo connection_info;

  arg::parse(argc, argv, connection_info);

  client.setConnectionInfo(connection_info);

  Logger::info("Connecting to " + connection_info.url);

  if (connection_info.user.empty() && connection_info.password.empty()) {
    client.setAuthenticated(false);
  } else if (!connection_info.user.empty() && !connection_info.password.empty()) {
    client.setAuthenticated(true);
  } else {
    Logger::error("Both user and password must be provided.");
    exit(1);
  }

  // --------------------------------------------------
  // @INFO Connect to the server
  // --------------------------------------------------
  if (!client.connect(connection_info.host, connection_info.port)) {
    Logger::error("Failed to connect to the server at " + connection_info.url);
    exit(1);
  }

  // @INFO Check if the connection was successful
  if (!client.isConnected()) {
    Logger::error("Failed to connect to the server.");
    exit(1);
  }

  return client;
}
}  // namespace network

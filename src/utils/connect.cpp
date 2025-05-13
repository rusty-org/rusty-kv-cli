#include "argument.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace network {
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
  if (!client.connect(connection_info)) {
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

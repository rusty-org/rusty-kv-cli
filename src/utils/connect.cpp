#include "argument.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace network {
KvClient connect_to_client(int argc, char* argv[]) {
  // --------------------------------------------------
  // @INFO Parse the command line arguments
  // --------------------------------------------------
  KvConnectionInfo connection_info;
  arg::parse(argc, argv, connection_info);

  Logger::info("Connecting to " + connection_info.url);

  // --------------------------------------------------
  // @INFO Connect to the server
  // --------------------------------------------------
  KvClient client;
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

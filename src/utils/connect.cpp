#include "argument.hpp"
#include "utils.hpp"

namespace network {
KvClient connect_to_client(int argc, char* argv[]) {
  // --------------------------------------------------
  // @INFO Parse the command line arguments
  // --------------------------------------------------
  KvConnectionInfo connection_info;
  arg::parse(argc, argv, connection_info);

  std::cout << "Connecting to " << connection_info.url << std::endl;

  // --------------------------------------------------
  // @INFO Connect to the server
  // --------------------------------------------------
  KvClient client;
  if (!client.connect(connection_info)) {
    std::cerr << "Failed to connect to the server at " << connection_info.url << std::endl;
    exit(1);
  }

  // @INFO Check if the connection was successful
  if (!client.isConnected()) {
    std::cerr << "Failed to connect to the server." << std::endl;
    exit(1);
  }

  return client;
}
}  // namespace network

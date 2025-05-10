#include "../client/client.hpp"

KvClient connect_to_client(int argc, char* argv[]) {
  // @INFO Default values for the server
  std::string kv_host = "127.0.0.1";
  int kv_port = 6379;

  // @INFO Parse the command line arguments
  if (argc >= 2) {
    kv_host = argv[1];
  }

  if (argc >= 3) {
    kv_port = std::stoi(argv[2]);
  }

  KvClient client;
  if (!client.connect(kv_host, kv_port)) {
    std::cerr << "Failed to connect to the server at " << kv_host << ":"
              << kv_port << std::endl;
    exit(1);
  }

  // @INFO Check if the connection was successful
  if (!client.isConnected()) {
    std::cerr << "Failed to connect to the server." << std::endl;
    exit(1);
  }

  return client;
}

#include "utils.hpp"

namespace arg {
void parse(int argc, char* argv[], KvConnectionInfo& info) {
  // Set default values
  info.host = "127.0.0.1";
  info.port = 6379;

  // ---------------------------------------------------
  // @INFO If the user does not provide any arguments,
  // ---------------------------------------------------
  if (argc < 2) {
    std::cerr << "Example Usage: " << std::endl;
    std::cerr << "\t" << argv[0] << " -p <port> -h <host> -U <user> -P <password>" << std::endl;
    std::cerr << "Or: " << std::endl;
    std::cerr << "\t" << argv[0] << " -url kv://<user>:<password>@<host>:<port>" << std::endl;
    exit(1);
  }

  // ---------------------------------------------------
  // @INFO Parse the command line arguments
  // ---------------------------------------------------
  for (int arg = 1; arg < argc; ++arg) {
    if (strcmp(argv[arg], "-p") == 0) {
      if (arg + 1 < argc) {
        info.port = std::stoi(argv[arg + 1]);
        ++arg;  // Skip the next argument
      } else {
        std::cerr << "Error: Port number not provided after -p" << std::endl;
        exit(1);
      }
    } else if (strcmp(argv[arg], "-h") == 0) {
      if (arg + 1 < argc) {
        info.host = argv[arg + 1];
        ++arg;  // Skip the next argument
      } else {
        std::cerr << "Error: Host name not provided after -h" << std::endl;
        exit(1);
      }
    } else if (strcmp(argv[arg], "-U") == 0) {
      if (arg + 1 < argc) {
        info.user = argv[arg + 1];
        ++arg;  // Skip the next argument
      } else {
        std::cerr << "Error: User name not provided after -U" << std::endl;
        exit(1);
      }
    } else if (strcmp(argv[arg], "-P") == 0) {
      if (arg + 1 < argc) {
        info.password = argv[arg + 1];
        ++arg;  // Skip the next argument
      } else {
        std::cerr << "Error: Password not provided after -P" << std::endl;
        exit(1);
      }
    } else if (strcmp(argv[arg], "-url") == 0) {
      if (arg + 1 < argc) {
        if (!network::parse_connection_uri(argv[arg + 1], info)) {
          std::cerr << "Error: Invalid connection URI format" << std::endl;
          std::cerr << "Format: kv://<user>:<password>@<host>:<port>" << std::endl;
          exit(1);
        }
        ++arg;  // Skip the next argument
      } else {
        std::cerr << "Error: Connection URI not provided after -url" << std::endl;
        exit(1);
      }
    }
  }

  // Set the URL string for display purposes if it wasn't set via -url
  if (info.url.empty()) {
    std::string credentials = "";
    if (!info.user.empty()) {
      credentials = info.user;
      if (!info.password.empty()) {
        credentials += ":" + info.password;
      }
      credentials += "@";
    }
    info.url = "kv://" + credentials + info.host + ":" + std::to_string(info.port);
  }
}
}  // namespace arg

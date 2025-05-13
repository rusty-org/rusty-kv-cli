#include "logger.hpp"
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
    Logger::error("No arguments provided. Please provide the connection details.");
    Logger::error("Example Usage: ");
    Logger::error("\t" + std::string(argv[0]) + " -p <port> -h <host> -U <user> -P <password>");
    Logger::error("Or: ");
    Logger::error("\t" + std::string(argv[0]) + " -url kv://<user>:<password>@<host>:<port>");
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
        Logger::error("Error: Port number not provided after -p");
        exit(1);
      }
    } else if (strcmp(argv[arg], "-h") == 0) {
      if (arg + 1 < argc) {
        info.host = argv[arg + 1];
        ++arg;  // Skip the next argument
      } else {
        Logger::error("Error: Host name not provided after -h");
        exit(1);
      }
    } else if (strcmp(argv[arg], "-U") == 0) {
      if (arg + 1 < argc) {
        info.user = argv[arg + 1];
        ++arg;  // Skip the next argument
      } else {
        Logger::error("Error: User name not provided after -U");
        exit(1);
      }
    } else if (strcmp(argv[arg], "-P") == 0) {
      if (arg + 1 < argc) {
        info.password = argv[arg + 1];
        ++arg;  // Skip the next argument
      } else {
        Logger::error("Error: Password not provided after -P");
        exit(1);
      }
    } else if (strcmp(argv[arg], "-url") == 0) {
      if (arg + 1 < argc) {
        if (!network::parse_connection_uri(argv[arg + 1], info)) {
          Logger::error("Error: Invalid connection URI format");
          Logger::error("Format: kv://<user>:<password>@<host>:<port>");
          Logger::error("Example: kv://user:password@127.0.0.1:6379");
          exit(1);
        }
        ++arg;  // Skip the next argument
      } else {
        Logger::error("Error: Connection URI not provided after -url");
        exit(1);
      }
    }
  }

  // ---------------------------------------------------
  // @INFO Set the URL string for display purposes if it wasn't set via -url
  // ---------------------------------------------------
  if (info.url.empty()) {
    std::string credentials = "";

    if (!info.user.empty()) {
      credentials = info.user;
      if (!info.password.empty()) {
        credentials += ":" + info.password;
      }
      credentials += "@";
    }

    // @INFO convert localhost to an IPv4 address
    if (info.host == "localhost") {
      info.host = "127.0.0.1";
    }

    info.url = "kv://" + credentials + info.host + ":" + std::to_string(info.port);
  }
}
}  // namespace arg

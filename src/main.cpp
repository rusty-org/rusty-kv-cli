#include "client/client.hpp"
#include "include/include.hpp"
#include "utils/logger.hpp"
#include "utils/resp_encoder.hpp"
#include "utils/utils.hpp"

int main(int argc, char* argv[]) {
  // --------------------------------------------------
  //  @INFO Connect and initialize the client
  // --------------------------------------------------
  KvClient client = network::connect_to_client(argc, argv);
  KvConnectionInfo connection_info = client.getConnectionInfo();
  bool authenticated = client.isAuthenticated();

  if (authenticated) {
    // --------------------------------------------------
    // @INFO Authenticate the client
    // --------------------------------------------------
    std::string auth_command = resp::encode_command("AUTH", {connection_info.user, connection_info.password});
    if (client.sendCommand(auth_command)) {
      std::string response = client.receiveResponse();
      if (response == resp::encode_simple_string("OK")) {
        Logger::success("Authentication successful.");
      } else {
        Logger::error("Authentication failed: " + response);
        client.disconnect();
        return 1;
      }
    } else {
      Logger::error("Failed to send authentication command.");
      client.disconnect();
      return 1;
    }
  } else {
    Logger::warn("Starting an unauthenticated session.");
  }

  // @INFO buffer to store the commands for each iteration
  std::string input;

  while (true) {
    Logger::client(client.getAddr() + "> ");
    std::getline(std::cin, input);

    // @INFO transform the command to lowercase
    std::string cmd = cmd::command_to_lowercase(input);
    if (cmd.empty()) continue;
    if (cmd == "exit" || cmd == "quit") break;

    // @NOTE Encode the command using RESP protocol
    std::string resp_command = resp::encode_raw_command(input);

    // @INFO Send the command to the server
    if (resp_command.empty()) {
      Logger::error("Failed to encode command: " + input);
      continue;
    }
    if (client.sendCommand(resp_command)) {
      std::string response = client.receiveResponse();
      std::cout << response;
    }
  }

  client.disconnect();
  Logger::warn("Disconnecting from server...");

  return 0;
}

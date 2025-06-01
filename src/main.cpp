/**
 * @file main.cpp
 * @brief Entry point for the Rusty KV CLI application.
 *
 * This module establishes a connection to the key–value store server,
 * handles optional authentication, enters a REPL for user commands
 * (encode→send→receive), and performs a graceful shutdown.
 */

#include "include/client.hpp"
#include "include/include.hpp"
#include "include/logger.hpp"
#include "include/resp.hpp"
#include "include/utils.hpp"

/**
 * @brief Initialize and connect the KvClient.
 *
 * Parses command-line arguments and connects to the server,
 * returning a configured client instance.
 *
 * @param argc Number of CLI arguments.
 * @param argv Array of argument strings.
 * @return Exit code (0 = success, non-zero = error).
 */
int main(int argc, char* argv[]) {
  // --------------------------------------------------
  //  @INFO Connect and initialize the client
  // --------------------------------------------------
  KvClient client = network::connect_to_client(argc, argv);
  // Fix: Use reference instead of pointer
  const KvConnectionInfo* connection_info = client.getConnectionInfo();

  /// @section Authentication
  /// If credentials were provided, send an AUTH command
  /// and verify the server’s response.
  if (client.isAuthenticated()) {
    // --------------------------------------------------
    // @INFO Authenticate the client
    // --------------------------------------------------
    // Create a vector of arguments for the AUTH command
    std::vector<std::string> auth_args = {connection_info->user, connection_info->password};

    std::string auth_command = resp::encode_command("AUTH", auth_args);
    if (client.sendCommand(auth_command)) {
      std::string response = client.receiveResponse();
      std::string decoded_response = resp::decode(response);
      if (response == resp::encode_simple_string("OK")) {
        Logger::success("Authentication successful.");
      } else {
        Logger::error("Authentication failed");
        std::cerr << decoded_response << std::endl;
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

  /// @section REPL Loop
  /// Main interactive loop: read user input, parse and normalize it,
  /// handle special AUTH re-authentication, or encode & send any other command.
  // @INFO buffer to store the commands for each iteration
  std::string input;

  while (true) {
    // Prompt for input
    std::cout << client.getAddr() + "> ";
    std::getline(std::cin, input);

    /// Normalize the command to lowercase for parsing.
    std::string cmd = cmd::command_to_lowercase(input);
    if (cmd.empty()) continue;
    if (cmd == "exit" || cmd == "quit") break;

    /// Handle in-loop AUTH command to reset credentials.
    std::vector<std::string> args = cmd::split(cmd, ' ');
    if (!args.empty() && args.at(0) == "auth") {
      Logger::warn("Re-authenticating...");

      // Extract username and password from args
      std::vector<std::string> auth_args;
      if (args.size() > 1) auth_args.push_back(args[1]);
      if (args.size() > 2) auth_args.push_back(args[2]);

      // Use the fixed encode_command function directly
      std::string auth_command = resp::encode_command("AUTH", auth_args);

      if (client.sendCommand(auth_command)) {
        std::string response = client.receiveResponse();
        std::string decoded_response = resp::decode(response);
        if (response == resp::encode_simple_string("OK")) {
          Logger::success("Re-authentication successful.");

          // Create a new connection info object instead of modifying the const one
          KvConnectionInfo newInfo = *connection_info;  // Copy the existing info
          newInfo.setUser(args.size() > 1 ? args[1] : "");
          newInfo.setPassword(args.size() > 2 ? args[2] : "");

          client.setAuthenticated(true);
          // Set the updated connection info
          client.setConnectionInfo(newInfo);

          // Update our local pointer to the new connection info
          connection_info = client.getConnectionInfo();
        } else {
          Logger::error("Re-authentication failed");
          std::cerr << decoded_response << std::endl;
          continue;
        }
      } else {
        Logger::error("Failed to send re-authentication command.");
        continue;
      }

      input.clear();

      // Skip the rest of the loop to avoid sending the AUTH command again
      continue;
    }

    // Encode the command using RESP protocol and send.
    // Parse the command and arguments with type awareness
    // Split by space but respect quotes and arrays
    std::vector<std::string> tokens = resp::tokenize(input);

    if (tokens.empty()) {
      continue;
    }

    std::string command_name = tokens[0];

    // Extract args, skipping the command name
    std::vector<std::string> new_args(tokens.begin() + 1, tokens.end());

    // Use our enhanced command encoder that applies type detection
    std::string resp_command = resp::encode(input);

    // @INFO Send the command to the server
    if (resp_command.empty()) {
      Logger::error("Failed to encode command: " + input);
      continue;
    }
    if (client.sendCommand(resp_command)) {
      std::string response = client.receiveResponse();
      std::string decoded_response = resp::decode(response);
      if (response.empty()) {
        Logger::error("Received empty response from server.");
        continue;
      }
      std::cout << decoded_response << std::endl;
    }
  }

  /// @section Cleanup
  /// Disconnects the client and exit, logging the shutdown.
  client.disconnect();
  Logger::warn("Disconnecting from server...");

  return 0;
}

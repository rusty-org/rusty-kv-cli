#ifndef _CLI_UTILS_HPP_
#define _CLI_UTILS_HPP_

#include "../client/client.hpp"
#include "../include/include.hpp"

// This function connects to the server and returns a KvClient object
// It takes command line arguments to specify the host and port
// If no arguments are provided, it defaults to "127.0.0.1" and port 6379
KvClient connect_to_client(int argc, char* argv[]);

namespace cmd {
// This function takes a command string and converts it to lowercase
// It uses ICU library for Unicode support
// It returns the lowercase command as a string
// If the command is empty, it returns an empty string and prints an error
// message It uses the UnicodeString class from the ICU library to handle
// Unicode strings
std::string command_to_lowercase(std::string& input);
}  // namespace cmd

#endif  // _CLI_UTILS_HPP_

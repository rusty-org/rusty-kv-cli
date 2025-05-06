#ifndef _CLI_UTILS_HPP_
#define _CLI_UTILS_HPP_

#include "include.hpp"
#include "../client/client.hpp"

// This function connects to the server and returns a KvClient object
// It takes command line arguments to specify the host and port
// If no arguments are provided, it defaults to "127.0.0.1" and port 6379
KvClient connect_to_client(int argc, char *argv[]);

// This function gets a command from the user and converts it to lowercase
// It returns the command as an icu::UnicodeString object
// The command is read from standard input
// If the command is empty, it prompts the user again
icu::UnicodeString command_to_lowercase(std::string &command);

#endif // _CLI_UTILS_HPP_

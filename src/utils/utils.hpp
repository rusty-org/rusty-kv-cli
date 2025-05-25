/**
 * @file utils.hpp
 * @brief Utility function declarations for networking and command parsing.
 */

#ifndef _CLI_UTILS_HPP_
#define _CLI_UTILS_HPP_

#include "include/include.hpp"

// Forward declarations
class KvClient;
class KvConnectionInfo;

namespace network {
/**
 * @brief Establishes a connection to the KV server.
 *
 * Parses CLI args (host, port, credentials) and returns a configured client.
 *
 * @param argc Number of arguments.
 * @param argv Array of argument strings.
 * @return KvClient instance connected (or configured) to the server.
 */
KvClient connect_to_client(int argc, char* argv[]);

/**
 * @brief Parses a connection URI into its components.
 *
 * Supported formats:
 *   - kv://user:password@host:port
 *   - kv://host:port
 *   - kv://user:password@host
 *
 * @param uri   URI string to parse.
 * @param info  Output connection info struct to populate.
 * @return True on successful parse; false otherwise.
 */
bool parse_connection_uri(const std::string& uri, KvConnectionInfo& info);
}  // namespace network

namespace cmd {
/**
 * @brief Converts a UTF-8 command string to lowercase.
 *
 * Uses ICU library for full Unicode support.
 *
 * @param input Command string (modified by trim/toLower).
 * @return Lowercased, trimmed command. Empty if input empty.
 */
std::string command_to_lowercase(std::string& input);

/**
 * @brief Splits a string into tokens by a separator character.
 *
 * Iterates over the string, extracting substrings between separators.
 *
 * @param s          Input string to split.
 * @param separator  Character used as delimiter.
 * @return Vector of substrings.
 */
std::vector<std::string> split(const std::string& s, char separator);
}  // namespace cmd

#endif  // _CLI_UTILS_HPP_

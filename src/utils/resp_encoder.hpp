/**
 * @file resp_encoder.hpp
 * @brief RESP protocol encoder interface for commands and data types.
 */

#ifndef _RESP_ENCODER_HPP_
#define _RESP_ENCODER_HPP_

#include "include/include.hpp"

namespace resp {

/**
 * @brief Encodes a command and its arguments as a RESP array.
 *
 * @param cmd  Command string (e.g., "SET").
 * @param args Vector of argument strings.
 * @return RESP-formatted string.
 */
std::string encode_command(const std::string& cmd, const std::vector<std::string>& args);

/**
 * @brief Tokenizes a raw command line and encodes as RESP.
 *
 * @param raw_cmd Raw input string (may contain quotes).
 * @return RESP-formatted array or empty on error.
 */
std::string encode_raw_command(const std::string& raw_cmd);

/** @name Helpers for basic RESP types */
//@{
std::string encode_bulk_string(const std::string& str);
std::string encode_simple_string(const std::string& str);
std::string encode_error(const std::string& str);
std::string encode_array(const std::vector<std::string>& elements);
std::vector<std::string> tokenize(const std::string& input);
//@}

}  // namespace resp

#endif  // _RESP_ENCODER_HPP_

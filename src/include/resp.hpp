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

/** @name Helper method to decode the incoming resp encoded messages from the server
 *  it will make the cli more human readable and more easy to use
 *
 *  # SYNTAX
 *  (type) value
 *
 *  @example
 *  ```
 *  # INCOMING
 *  $4\r\nHell\r\n
 *
 *  # DECODED
 *  (string) Hell
 *
 *  # INCOMING
 *  #t\r\n
 *
 *  # DECODED
 *  (boolean) true
 *
 *  # INCOMING
 *  :155\r\n
 *
 *  # DECODED
 *  (integer) 155
 *
 *  # INCOMING
 *  *2\r\n$5\r\nHello\r\n$5\r\nWorld\r\n
 *
 *  # DECODED
 *  (array) ["Hello", "World"]
 *  ```
 */
//@{
std::string decode(const std::string& str);                //< Main function to call other helper methods
std::string decode_bulk_string(const std::string& str);    //< Decode single bulk string
std::string decode_simple_string(const std::string& str);  //< Decode simple string
std::string decode_array(const std::string& str);          //< Decode array of values
std::string decode_error(const std::string& str);          //< Decode error
std::string decode_integer(const std::string& str);        //< Decode integer values
std::string decode_boolean(const std::string& str);        //< Decode boolean values
//@}

}  // namespace resp

#endif  // _RESP_ENCODER_HPP_

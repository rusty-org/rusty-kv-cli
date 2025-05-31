/**
 * @file resp.hpp
 * @brief RESP protocol encoder/decoder interface.
 */

#ifndef _RESP_HPP_
#define _RESP_HPP_

#include "include/include.hpp"

namespace resp {

/** @name Encoding functions */
//@{
std::string encode(const std::string& str);
std::string encode_bulk_string(const std::string& str);
std::string encode_simple_string(const std::string& str);
std::string encode_error(const std::string& str);
std::string encode_integer(int64_t num);
std::string encode_boolean(bool value);
std::string encode_array(const std::vector<std::string>& elements);
std::string encode_command(const std::string& cmd, const std::vector<std::string>& args);
std::string encode_raw_command(const std::string& raw_cmd);
//@}

/** @name Type detection helpers */
//@{
bool is_integer(const std::string& str);
bool is_boolean(const std::string& str);
bool is_array(const std::string& str);
std::vector<std::string> parse_array(const std::string& str);
std::vector<std::string> tokenize(const std::string& input);
std::string encode_token(const std::string& token);
//@}

/** @name Decoding functions */
//@{
std::string decode(const std::string& str);
std::string decode_simple_string(const std::string& str);
std::string decode_error(const std::string& str);
std::string decode_integer(const std::string& str);
std::string decode_bulk_string(const std::string& str);
std::string decode_array(const std::string& str);
std::string decode_boolean(const std::string& str);
//@}

}  // namespace resp

#endif  // _RESP_HPP_

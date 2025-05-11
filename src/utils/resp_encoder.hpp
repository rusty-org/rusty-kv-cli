#ifndef _RESP_ENCODER_HPP_
#define _RESP_ENCODER_HPP_

#include "include/include.hpp"

namespace resp {

// Encode a command and its arguments into the RESP protocol format
std::string encode_command(const std::string& cmd, const std::vector<std::string>& args);

// Parse a raw command string into command and args, then encode it
std::string encode_raw_command(const std::string& raw_cmd);

// Helper functions
std::string encode_bulk_string(const std::string& str);
std::string encode_array(const std::vector<std::string>& elements);
std::vector<std::string> tokenize(const std::string& input);

}  // namespace resp

#endif  // _RESP_ENCODER_HPP_

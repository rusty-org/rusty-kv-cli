/**
 * @file command.cpp
 * @brief Implements cmd::command_to_lowercase and cmd::split.
 */

#include "utils.hpp"

namespace cmd {

/**
 * @brief Convert and trim input to lowercase using ICU.
 *
 * @param input UTF-8 string to normalize.
 * @return Lowercase, trimmed version (or empty if input empty).
 */
std::string command_to_lowercase(std::string& input) {
  const icu::UnicodeString uni_command(input.c_str(), "UTF-8");
  if (uni_command.isEmpty()) return "";

  icu::UnicodeString command_lower = std::move(uni_command);
  std::string cmd;
  command_lower = command_lower.toLower().trim();
  command_lower.toUTF8String(cmd);

  return cmd;
}

/**
 * @brief Split a string by a single-character separator.
 *
 * @param s         Input string.
 * @param separator Delimiter character.
 * @return Vector of substrings.
 */
std::vector<std::string> split(const std::string& s, char separator) {
  std::vector<std::string> output;
  std::string::size_type prev_pos = 0, pos = 0;

  while ((pos = s.find(separator, pos)) != std::string::npos) {
    std::string substring(s.substr(prev_pos, pos - prev_pos));
    output.push_back(substring);
    prev_pos = ++pos;
  }

  output.push_back(s.substr(prev_pos, pos - prev_pos));  // Last word
  return output;
}

}  // namespace cmd

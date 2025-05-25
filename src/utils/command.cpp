#include "utils.hpp"

namespace cmd {
std::string command_to_lowercase(std::string& input) {
  const icu::UnicodeString uni_command(input.c_str(), "UTF-8");
  if (uni_command.isEmpty()) return "";

  icu::UnicodeString command_lower = std::move(uni_command);
  std::string cmd;
  command_lower = command_lower.toLower().trim();
  command_lower.toUTF8String(cmd);

  return cmd;
}

std::vector<std::string> split(const std::string& s, char seperator) {
  std::vector<std::string> output;
  std::string::size_type prev_pos = 0, pos = 0;

  while ((pos = s.find(seperator, pos)) != std::string::npos) {
    std::string substring(s.substr(prev_pos, pos - prev_pos));
    output.push_back(substring);
    prev_pos = ++pos;
  }

  output.push_back(s.substr(prev_pos, pos - prev_pos));  // Last word
  return output;
}
}  // namespace cmd

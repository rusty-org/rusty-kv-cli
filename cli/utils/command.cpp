#include "utils.hpp"

std::string command_to_lowercase(std::string &input)
{
  const icu::UnicodeString uni_command(input.c_str(), "UTF-8");
  if (uni_command.isEmpty())
    return "";

  icu::UnicodeString command_lower = std::move(uni_command);
  std::string cmd;
  command_lower = command_lower.toLower().trim();
  command_lower.toUTF8String(cmd);

  return cmd;
}

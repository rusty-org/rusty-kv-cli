/**
 * @file resp_encoder.cpp
 * @brief RESP protocol encoder implementations.
 */

#include "include/logger.hpp"
#include "include/resp.hpp"

namespace resp {

/**
 * @brief Encode a bulk string: `$<len>\r\n<data>\r\n`.
 */
std::string encode_bulk_string(const std::string& str) {
  std::ostringstream oss;
  oss << "$" << str.length() << "\r\n" << str << "\r\n";
  return oss.str();
}

/**
 * @brief Encode a simple string: `+<str>\r\n`.
 */
std::string encode_simple_string(const std::string& str) {
  std::ostringstream oss;
  oss << "+" << str << "\r\n";
  return oss.str();
}

/**
 * @brief Encode an error: `-<message>\r\n`.
 */
std::string encode_error(const std::string& str) {
  std::ostringstream oss;
  oss << "-" << str << "\r\n";
  return oss.str();
}

/**
 * @brief Encode an integer: `:<num>\r\n`.
 */
std::string encode_integer(int64_t num) {
  std::ostringstream oss;
  oss << ":" << num << "\r\n";
  return oss.str();
}

/**
 * @brief Encode a boolean: `#t\r\n` or `#f\r\n`.
 */
std::string encode_boolean(bool value) {
  std::ostringstream oss;
  oss << "#" << (value ? "t" : "f") << "\r\n";
  return oss.str();
}

/**
 * @brief Encode an array of bulk strings.
 *
 * Formats as `*<count>\r\n` followed by each element.
 */
std::string encode_array(const std::vector<std::string>& elements) {
  std::ostringstream oss;
  oss << "*" << elements.size() << "\r\n";
  for (const auto& element : elements) {
    oss << encode_bulk_string(element);
  }
  return oss.str();
}

/**
 * @brief Check if a string is a valid integer.
 * @param str String to check
 * @return true if string is a valid integer
 */
bool is_integer(const std::string& str) {
  if (str.empty()) return false;

  size_t start = 0;
  if (str[0] == '-' || str[0] == '+') {
    if (str.length() == 1) return false;  // Just a sign
    start = 1;
  }

  return str.find_first_not_of("0123456789", start) == std::string::npos;
}

/**
 * @brief Check if a string is a valid boolean ("true" or "false").
 * @param str String to check
 * @return true if string is a valid boolean representation
 */
bool is_boolean(const std::string& str) {
  return str == "true" || str == "false";
}

/**
 * @brief Check if a string represents an array (starts with '[' and ends with ']').
 * @param str String to check
 * @return true if string appears to be an array
 */
bool is_array(const std::string& str) {
  return !str.empty() && str.front() == '[' && str.back() == ']';
}

/**
 * @brief Parse a string array representation into vector of strings.
 *
 * Handles format like: [item1, item2, "quoted item"]
 *
 * @param str String containing array in format [element1, element2, ...]
 * @return Vector of array elements
 */
std::vector<std::string> parse_array(const std::string& str) {
  std::vector<std::string> elements;

  // Remove brackets and split by comma
  if (str.size() < 2) return elements;

  std::string content = str.substr(1, str.size() - 2);

  // Handle empty array
  if (content.empty()) return elements;

  // Parse elements considering quotes
  std::string element;
  bool in_quotes = false;

  for (size_t i = 0; i < content.size(); ++i) {
    char c = content[i];

    if (c == '"') {
      in_quotes = !in_quotes;
    } else if (c == ',' && !in_quotes) {
      // End of element
      element = std::string(element.begin(), element.end());
      element.erase(0, element.find_first_not_of(" \t"));
      element.erase(element.find_last_not_of(" \t") + 1);
      elements.push_back(element);
      element.clear();
    } else {
      element += c;
    }
  }

  // Add the last element if any
  if (!element.empty()) {
    element.erase(0, element.find_first_not_of(" \t"));
    element.erase(element.find_last_not_of(" \t") + 1);
    elements.push_back(element);
  }

  return elements;
}

/**
 * @brief Encode a string based on its type.
 *
 * Handles special cases for integers, booleans, arrays, and bulk strings.
 *
 * @param str The string to encode
 * @return RESP-encoded string
 */
std::string encode(const std::string& str) {
  std::stringstream ssin(str);
  std::vector<std::string> command_vec;

  while (ssin.good() && !ssin.eof()) {
    std::string token;
    ssin >> token;

    // Check for empty tokens
    if (token.empty()) continue;

    // Encode the token based on its type
    command_vec.push_back(encode_token(token));
  }

  return encode_array(command_vec);
}

/**
 * @brief Split raw input into tokens, respecting quoted substrings.
 *
 * Handles unclosed quotes by collecting remainder as one token.
 */
std::vector<std::string> tokenize(const std::string& input) {
  std::vector<std::string> tokens;
  std::istringstream iss(input);
  std::string token;

  bool in_quotes = false;
  std::string quoted_token;

  while (iss >> token) {
    // Check if token starts with a quote
    if (!in_quotes && token.front() == '"') {
      in_quotes = true;
      quoted_token = token.substr(1);  // Remove leading quote

      // If it also ends with a quote, it's a complete quoted token
      if (token.length() > 1 && token.back() == '"') {
        in_quotes = false;
        quoted_token = quoted_token.substr(0, quoted_token.length() - 1);  // Remove trailing quote
        tokens.push_back(quoted_token);
      }
    }
    // Check if token ends with a quote
    else if (in_quotes && token.back() == '"') {
      in_quotes = false;
      quoted_token += " " + token.substr(0, token.length() - 1);  // Remove trailing quote
      tokens.push_back(quoted_token);
    }
    // If we're in the middle of a quoted string
    else if (in_quotes) {
      quoted_token += " " + token;
    }
    // Check if token is an array
    else if (token.front() == '[' && token.back() != ']') {
      // Start of array that spans multiple tokens
      in_quotes = true;  // Not actually quotes, but we'll reuse the mechanism
      quoted_token = token;
    } else if (in_quotes && token.back() == ']') {
      // End of multi-token array
      in_quotes = false;
      quoted_token += " " + token;
      tokens.push_back(quoted_token);
    }
    // Regular token, not part of a quoted string or array
    else {
      tokens.push_back(token);
    }
  }

  // If we're still in quotes after processing all tokens,
  // add what we have (handles unclosed quotes)
  if (in_quotes) {
    tokens.push_back(quoted_token);
  }

  return tokens;
}

/**
 * @brief Encode a single token with the appropriate RESP data type.
 *
 * Attempts to detect the data type and encode accordingly.
 * Note: For command arguments, even numeric values must be sent as bulk strings
 * according to the RESP protocol specification for commands.
 *
 * @param token The token to encode
 * @return RESP-encoded string for the token
 */
std::string encode_token(const std::string& token) {
  // Try to detect the data type
  if (is_integer(token)) {
    try {
      // Parse as integer for validation, but still encode as bulk string
      // as required by RESP protocol for command arguments
      int64_t num = std::stoll(token);
      // Log for debugging (optional)
      return encode_integer(num);  // Encode as integer
    } catch (const std::exception& e) {
      // Fallback to string if overflow occurs
      return encode_bulk_string(token);
    }
  } else if (is_boolean(token)) {
    return encode_boolean(token == "true");  // Encode as boolean
  } else if (is_array(token)) {
    // Parse the array and encode each element
    auto elements = parse_array(token);
    std::vector<std::string> encoded_elements;
    for (const auto& elem : elements) {
      encoded_elements.push_back(elem);
    }
    return encode_array(encoded_elements);  // Encode as array
  } else {
    // Default to string
    return encode_bulk_string(token);
  }
}

/**
 * @brief Combine command and args, then call `encode_array`.
 *
 * Detects argument types and performs appropriate encoding for each.
 *
 * @param cmd Command name (e.g., "SET")
 * @param args Vector of argument strings
 * @return RESP-encoded array command
 */
std::string encode_command(const std::string& cmd, const std::vector<std::string>& args) {
  std::vector<std::string> all_elements;
  all_elements.push_back(cmd);

  // Process each argument with type detection
  for (const auto& arg : args) {
    // For command arguments, we should always use bulk strings
    // as required by the RESP protocol for command arguments
    all_elements.push_back(arg);
  }

  return encode_array(all_elements);
}

/**
 * @brief Parse raw command tokens and encode via `encode_command`.
 *
 * Intelligently detects data types in arguments.
 */
std::string encode_raw_command(const std::string& raw_cmd) {
  std::vector<std::string> tokens = tokenize(raw_cmd);

  if (tokens.empty()) {
    return "";
  }

  std::string cmd = tokens[0];
  std::vector<std::string> args(tokens.begin() + 1, tokens.end());

  return encode_command(cmd, args);
}

}  // namespace resp

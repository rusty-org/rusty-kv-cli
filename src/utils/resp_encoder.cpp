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
 * @brief Encode a string based on its type.
 *
 * Processes input character-by-character to properly handle arrays and quoted strings.
 *
 * @param str The string to encode
 * @return RESP-encoded string
 */
std::string encode(const std::string& str) {
  std::vector<std::string> tokens = tokenize(str);
  std::vector<std::string> encoded_tokens;

  for (const auto& token : tokens) {
    encoded_tokens.push_back(encode_token(token));
  }

  return encode_array(encoded_tokens);
}

/**
 * @brief Split raw input into tokens, respecting quoted substrings and arrays.
 *
 * Processes character-by-character to properly handle complex structures.
 */
std::vector<std::string> tokenize(const std::string& input) {
  std::vector<std::string> tokens;
  std::string token;
  bool in_quotes = false;
  bool in_array = false;
  int array_depth = 0;
  bool in_token = false;

  for (size_t i = 0; i < input.length(); ++i) {
    char c = input[i];

    // Handle quoted strings
    if (c == '"') {
      if (!in_quotes) {
        // Starting a quoted string
        if (!token.empty() && !in_array) {
          // If we have a token already and we're starting quotes, save current token
          tokens.push_back(token);
          token.clear();
        }
        in_quotes = true;
        token += c;  // Keep the quotes in the token
      } else {
        // Ending a quoted string
        token += c;
        if (!in_array) {
          tokens.push_back(token);
          token.clear();
          in_quotes = false;
        } else {
          in_quotes = false;
        }
      }
      continue;
    }

    // Handle arrays
    if (c == '[' && !in_quotes) {
      if (!token.empty() && array_depth == 0) {
        // If we have a token already and starting a new array, save current token
        tokens.push_back(token);
        token.clear();
      }
      in_array = true;
      array_depth++;
      token += c;
      continue;
    }

    if (c == ']' && !in_quotes && in_array) {
      token += c;
      array_depth--;
      if (array_depth == 0) {
        in_array = false;
        tokens.push_back(token);
        token.clear();
      }
      continue;
    }

    // Handle whitespace
    if (std::isspace(c) && !in_quotes && !in_array) {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
      continue;
    }

    // Add character to current token
    token += c;
    in_token = true;
  }

  // Add any remaining token
  if (!token.empty()) {
    tokens.push_back(token);
  }

  return tokens;
}

/**
 * @brief Encode a single token with the appropriate RESP data type.
 *
 * Detects the data type based on string content and encodes accordingly.
 *
 * @param token The token to encode
 * @return RESP-encoded string for the token
 */
std::string encode_token(const std::string& token) {
  // Handle quoted strings (remove quotes and encode as bulk string)
  if (token.length() >= 2 && token.front() == '"' && token.back() == '"') {
    return encode_bulk_string(token.substr(1, token.length() - 2));
  }
  // Try to detect the data type
  else if (is_integer(token)) {
    try {
      int64_t num = std::stoll(token);
      return encode_integer(num);
    } catch (const std::exception& e) {
      return encode_bulk_string(token);
    }
  } else if (is_boolean(token)) {
    return encode_boolean(token == "true");
  } else if (is_array(token)) {
    // Parse the array and encode each element
    auto elements = parse_array(token);
    std::vector<std::string> encoded_elements;
    for (const auto& elem : elements) {
      // For elements inside array, we should detect their type
      if (is_integer(elem)) {
        try {
          int64_t num = std::stoll(elem);
          encoded_elements.push_back(std::to_string(num));
        } catch (const std::exception&) {
          encoded_elements.push_back(elem);
        }
      } else if (elem.length() >= 2 && elem.front() == '"' && elem.back() == '"') {
        // Handle quoted strings in arrays
        encoded_elements.push_back(elem.substr(1, elem.length() - 2));
      } else {
        encoded_elements.push_back(elem);
      }
    }
    return encode_array(encoded_elements);
  } else {
    // Default to string
    return encode_bulk_string(token);
  }
}

/**
 * @brief Parse a string array representation into vector of strings.
 *
 * Character-by-character parsing to properly handle nested structures.
 *
 * @param str String containing array in format [element1, element2, ...]
 * @return Vector of array elements
 */
std::vector<std::string> parse_array(const std::string& str) {
  std::vector<std::string> elements;

  // Remove brackets
  if (str.size() < 2 || str.front() != '[' || str.back() != ']') return elements;

  std::string content = str.substr(1, str.size() - 2);

  // Handle empty array
  if (content.empty()) return elements;

  std::string element;
  bool in_quotes = false;
  bool in_nested_array = false;
  int nested_depth = 0;

  for (size_t i = 0; i < content.length(); ++i) {
    char c = content[i];

    // Handle quoted strings
    if (c == '"') {
      in_quotes = !in_quotes;
      element += c;
      continue;
    }

    // Handle nested arrays
    if (c == '[' && !in_quotes) {
      in_nested_array = true;
      nested_depth++;
      element += c;
      continue;
    }

    if (c == ']' && !in_quotes && in_nested_array) {
      nested_depth--;
      element += c;
      if (nested_depth == 0) in_nested_array = false;
      continue;
    }

    // Handle element separator
    if (c == ',' && !in_quotes && !in_nested_array) {
      // Trim whitespace
      element.erase(0, element.find_first_not_of(" \t"));
      size_t last = element.find_last_not_of(" \t");
      if (last != std::string::npos) element.erase(last + 1);

      elements.push_back(element);
      element.clear();
      continue;
    }

    // Add character to current element
    element += c;
  }

  // Add final element if any
  if (!element.empty()) {
    // Trim whitespace
    element.erase(0, element.find_first_not_of(" \t"));
    size_t last = element.find_last_not_of(" \t");
    if (last != std::string::npos) element.erase(last + 1);

    elements.push_back(element);
  }

  return elements;
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
  all_elements.insert(all_elements.end(), args.begin(), args.end());
  return encode_array(all_elements);
}

/**
 * @brief Parse raw command and encode as RESP command.
 *
 * Uses character-by-character parsing for accurate token detection.
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

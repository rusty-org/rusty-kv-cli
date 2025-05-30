/**
 * @file resp_decoder.cpp
 * @brief RESP protocol decoder implementations.
 *
 * Provides functionality to convert RESP wire protocol responses from
 * the KV server into human-readable output for the CLI interface.
 * Works in conjunction with resp_encoder.cpp to provide the full RESP
 * protocol implementation.
 */

#include "include/include.hpp"
#include "include/resp.hpp"

namespace resp {
/**
 * @brief Decodes a RESP simple string: `+<str>\r\n`.
 *
 * Formats output as "(string) value".
 *
 * @param str The RESP-encoded simple string starting with '+'.
 * @return Human-readable formatted string.
 */
std::string decode_simple_string(const std::string& str) {
  size_t end_pos = str.find("\r\n");
  if (end_pos == std::string::npos || str.size() < 2) {
    return "(string) (invalid input)";
  }
  std::ostringstream oss;
  oss << "(string) " << str.substr(1, end_pos - 1);
  return oss.str();
}

/**
 * @brief Decodes a RESP integer: `:<num>\r\n`.
 *
 * Formats output as "(integer) value".
 *
 * @param str The RESP-encoded integer starting with ':'.
 * @return Human-readable formatted string.
 */
std::string decode_integer(const std::string& str) {
  size_t end_pos = str.find("\r\n");
  if (end_pos == std::string::npos || str.size() < 2) {
    return "(integer) (invalid input)";
  }
  std::ostringstream oss;
  oss << "(integer) " << str.substr(1, end_pos - 1);
  return oss.str();
}

/**
 * @brief Decodes a RESP boolean: `#t\r\n` or `#f\r\n`.
 *
 * Formats output as "(boolean) true" or "(boolean) false".
 *
 * @param str The RESP-encoded boolean starting with '#'.
 * @return Human-readable formatted string.
 */
std::string decode_boolean(const std::string& str) {
  if (str.size() < 2) {
    return "(boolean) (invalid input)";
  }
  std::ostringstream oss;
  char val = str[1];
  oss << "(boolean) " << ((val == 't') ? "true" : "false");
  return oss.str();
}

/**
 * @brief Decodes a RESP bulk string: `$<len>\r\n<data>\r\n`.
 *
 * Handles null bulk strings ($-1) and formats output as "(string) value".
 *
 * @param str The RESP-encoded bulk string starting with '$'.
 * @return Human-readable formatted string.
 */
std::string decode_bulk_string(const std::string& str) {
  size_t len_end = str.find("\r\n", 1);
  if (len_end == std::string::npos || str.size() < 2) {
    return "(string) (invalid input)";
  }

  int len = std::stoi(str.substr(1, len_end - 1));
  if (len == -1) return "(string) null";  // null bulk string

  size_t val_start = len_end + 2;
  if (val_start + len > str.size()) {
    return "(string) (incomplete)";
  }

  std::string val = str.substr(val_start, len);
  std::ostringstream oss;
  oss << "(string) " << val;
  return oss.str();
}

/**
 * @brief Decodes a RESP array: `*<count>\r\n` followed by elements.
 *
 * Recursively parses array elements and formats as "(array) [elem1, elem2, ...]".
 * Currently focuses on bulk string array elements for the CLI use case.
 *
 * @param str The RESP-encoded array starting with '*'.
 * @return Human-readable formatted string.
 */
std::string decode_array(const std::string& str) {
  std::ostringstream oss;
  oss << "(array) [";

  size_t pos = 1;
  size_t len_end = str.find("\r\n", pos);
  if (len_end == std::string::npos) return "(array) (invalid input)";

  int count = std::stoi(str.substr(pos, len_end - pos));
  pos = len_end + 2;

  for (int i = 0; i < count; ++i) {
    if (i > 0) oss << ", ";

    if (pos >= str.size()) {
      oss << "(incomplete)";
      break;
    }

    char type = str[pos];
    if (type == '$') {
      size_t bulk_len_end = str.find("\r\n", pos + 1);
      if (bulk_len_end == std::string::npos) {
        oss << "(invalid)";
        break;
      }

      int bulk_len = std::stoi(str.substr(pos + 1, bulk_len_end - (pos + 1)));
      size_t val_start = bulk_len_end + 2;

      if (bulk_len == -1) {
        oss << "null";
        pos = val_start + 2;
        continue;
      }

      if (val_start + bulk_len > str.size()) {
        oss << "(incomplete)";
        break;
      }

      std::string val = str.substr(val_start, bulk_len);
      oss << "\"" << val << "\"";
      pos = val_start + bulk_len + 2;  // skip value + \r\n
    } else {
      oss << "(unknown)";
      break;
    }
  }

  oss << "]";
  return oss.str();
}

/**
 * @brief Decodes a RESP error: `-<message>\r\n`.
 *
 * Formats output as "(error) message".
 *
 * @param str The RESP-encoded error starting with '-'.
 * @return Human-readable formatted string.
 */
std::string decode_error(const std::string& str) {
  size_t end_pos = str.find("\r\n");
  if (end_pos == std::string::npos || str.size() < 2) {
    return "(error) (invalid input)";
  }

  std::string message = str.substr(1, end_pos - 1);
  std::ostringstream oss;
  oss << "(error) " << message;
  return oss.str();
}

/**
 * @brief Main decoder function that dispatches based on RESP type.
 *
 * Examines the first character to determine the RESP data type and
 * calls the appropriate decoder. Used in the client REPL to format
 * server responses for display.
 *
 * @param str The RESP-encoded response string.
 * @return Human-readable formatted response.
 */
std::string decode(const std::string& str) {
  if (str.empty()) return "(null)";

  char prefix = str[0];
  switch (prefix) {
    case '+':
      return decode_simple_string(str);
    case '-':
      return decode_error(str);
    case ':':
      return decode_integer(str);
    case '$':
      return decode_bulk_string(str);
    case '*':
      return decode_array(str);
    case '#':
      return decode_boolean(str);
    default:
      return "(unknown)";
  }
}

}  // namespace resp

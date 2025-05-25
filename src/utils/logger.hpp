/**
 * @file logger.hpp
 * @brief ANSI-colored logging utilities with severity levels.
 */

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <iostream>
#include <string>

namespace Logger {

/**
 * @enum Level
 * @brief Log severity levels.
 */
enum class Level { INFO, WARN, ERROR, SUCCESS, CLIENT };

// ANSI escape codes
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";

/**
 * @brief Core logging function.
 *
 * Applies color/style, tag, and prints to stdout or stderr.
 *
 * @param level   Severity level.
 * @param message Message to log.
 */
inline void log(Level level, const std::string& message) {
  std::string tag;
  std::string color;
  bool new_line = true;

  switch (level) {
    case Level::INFO:
      tag = "[INFO]   ";
      color = BLUE;
      break;
    case Level::WARN:
      tag = "[WARN]   ";
      color = YELLOW;
      break;
    case Level::ERROR:
      tag = "[ERROR]  ";
      color = RED;
      break;
    case Level::SUCCESS:
      tag = "[SUCCESS]";
      color = GREEN;
      break;
    case Level::CLIENT:
      tag = "[CLIENT] ";
      color = BLUE;
      new_line = false;
      break;
  }

  if (new_line) {
    std::cout << BOLD << color << tag << RESET << " " << message << std::endl;
  } else {
    std::cout << BOLD << color << tag << RESET << " " << message;
  }
}

/**
 * @brief Shortcut for INFO level.
 */
inline void info(const std::string& message) {
  log(Level::INFO, message);
}

/**
 * @brief Shortcut for WARN level.
 */
inline void warn(const std::string& message) {
  log(Level::WARN, message);
}

/**
 * @brief Shortcut for ERROR level.
 */
inline void error(const std::string& message) {
  log(Level::ERROR, message);
}

/**
 * @brief Shortcut for SUCCESS level.
 */
inline void success(const std::string& message) {
  log(Level::SUCCESS, message);
}

/**
 * @brief Client prompt style (no newline).
 */
inline void client(const std::string& message) {
  log(Level::CLIENT, message);
}

}  // namespace Logger

#endif  // _LOGGER_HPP_

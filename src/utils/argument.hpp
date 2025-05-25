/**
 * @file argument.hpp
 * @brief CLI argument parsing interface.
 */

#ifndef _ARGUMENT_HPP_
#define _ARGUMENT_HPP_

#include "utils.hpp"

namespace arg {

/**
 * @brief Parses command-line options into KvConnectionInfo.
 *
 * Supports -p, -h, -U, -P, and -url flags.
 *
 * Exits on missing required values or invalid URI.
 *
 * @param argc Number of CLI args.
 * @param argv Array of arg strings.
 * @param info  Output connection info to populate.
 */
void parse(int argc, char* argv[], KvConnectionInfo& info);

}  // namespace arg

#endif  // _ARGUMENT_HPP_

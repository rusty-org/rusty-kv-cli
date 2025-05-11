#ifndef _ARGUMENT_HPP_
#define _ARGUMENT_HPP_

#include "utils.hpp"

namespace arg {
// Function to parse command line arguments
// and encode the given connection info
void parse(int argc, char* argv[], KvConnectionInfo& info);
}  // namespace arg

#endif  // _ARGUMENT_HPP_

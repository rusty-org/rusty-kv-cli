#include "client/client.hpp"
#include "utils.hpp"

namespace network {
bool parse_connection_uri(const std::string& uri, KvConnectionInfo& info) {
  // @INFO Regular expression to match the connection URI
  std::regex uri_regex(R"(kv://(?:(\w+):(\w+)@)?([^:]+):(\d+))");
  std::smatch match;

  if (std::regex_match(uri, match, uri_regex)) {
    if (match.size() == 5) {
      info.user = match[1].str();
      info.password = match[2].str();

      // @INFO convert localhost to an actual IPv4 address
      if (match[3].str() == "localhost") {
        info.host = "127.0.0.1";
      } else {
        info.host = match[3].str();
      }

      info.port = std::stoi(match[4].str());
      return true;
    }
  }

  return false;
}
}  // namespace network

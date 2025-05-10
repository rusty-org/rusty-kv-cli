#include "client/client.hpp"
#include "include/include.hpp"
#include "utils/resp_encoder.hpp"
#include "utils/utils.hpp"

int main(int argc, char* argv[]) {
  KvClient client = connect_to_client(argc, argv);

  std::string input;

  while (true) {
    std::cout << client.getAddr() << "> ";
    std::getline(std::cin, input);

    std::string cmd = cmd::command_to_lowercase(input);
    if (cmd.empty()) continue;
    if (cmd == "exit" || cmd == "quit") break;

    // @NOTE Encode the command using RESP protocol
    std::string resp_command = resp::encode_raw_command(input);

    if (client.sendCommand(resp_command)) {
      std::string response = client.receiveResponse();
      std::cout << response;
    }
  }

  client.disconnect();
  std::cout << "Disconnected from server." << std::endl;

  return 0;
}

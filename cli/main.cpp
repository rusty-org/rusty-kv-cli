#include "include/include.hpp"
#include "client/client.hpp"
#include "utils/utils.hpp"

int main(int argc, char *argv[])
{
  KvClient client = connect_to_client(argc, argv);

  std::string input;

  while (true)
  {
    std::cout << client.getAddr() << "> ";
    std::getline(std::cin, input);

    std::string cmd = command_to_lowercase(input);
    if (cmd.empty())
      continue;
    if (cmd == "exit" || cmd == "quit")
      break;

    if (client.sendCommand(cmd))
    {
      std::string response = client.receiveResponse();
      std::cout << response;
    }
  }

  client.disconnect();
  std::cout << "Disconnected from server." << std::endl;

  return 0;
}

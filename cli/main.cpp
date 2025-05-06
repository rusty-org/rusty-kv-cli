#include "utils/include.hpp"
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

    const icu::UnicodeString uni_command(input.c_str(), "UTF-8");
    if (uni_command.isEmpty())
    {
      continue;
    }

    icu::UnicodeString command_lower = std::move(uni_command);
    command_lower = command_lower.toLower().trim();
    std::string cmd;
    command_lower.toUTF8String(cmd);
    if (cmd == "exit" || cmd == "quit")
    {
      break;
    }

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

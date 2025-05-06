pub mod echo;
pub mod ping;
pub mod lib;
pub mod help;

use lib::CommandsList;

// Parse the request to identify which command is being requested
fn parse_command(request: &str) -> Option<(String, &str)> {
  let parts: Vec<&str> = request.splitn(2, ' ').collect();
  let command = parts[0].to_uppercase();
  let args = parts.get(1).unwrap_or(&"");

  Some((command, args))
}

// Get a command instance from the request string
pub fn get_command(request: &str) -> Option<CommandsList> {
  if let Some((cmd_type, args)) = parse_command(request) {
    CommandsList::from_command_type(&cmd_type, args)
  } else {
    None
  }
}

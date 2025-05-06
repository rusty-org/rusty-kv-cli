pub mod echo;
pub mod executor;
pub mod help;
pub mod lib;
pub mod ping;
pub mod set;

use lib::CommandsList;

// Parse the request to identify which command is being requested
fn parse_command_parts(request: &str) -> Option<(String, &str)> {
  let parts: Vec<&str> = request.splitn(2, ' ').collect();
  if parts.is_empty() {
    return None;
  }

  let command = parts[0].to_uppercase();
  let args = parts.get(1).unwrap_or(&"");

  Some((command, args))
}

// Get a command instance from the request string
#[allow(dead_code)]
pub fn get_command_instance(request: &str) -> Option<CommandsList> {
  if let Some((cmd_type, args)) = parse_command_parts(request) {
    CommandsList::from_command_type(&cmd_type, args)
  } else {
    None
  }
}

// Used by the RESP protocol handler
#[allow(dead_code)]
pub fn tokenize_command(input: &str) -> Vec<String> {
  input.split_whitespace().map(|s| s.to_string()).collect()
}

#[allow(dead_code)]
pub fn extract_command_name(parts: &[String]) -> Option<String> {
  parts.first().cloned()
}

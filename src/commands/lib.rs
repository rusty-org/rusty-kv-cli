use crate::resp::value::Value;
use anyhow::Result;

use super::{echo::EchoCommand, help::HelpCommand, ping::PingCommand, set::SetCommand};

pub enum CommandsList {
  Ping(PingCommand),
  Echo(EchoCommand),
  Help(HelpCommand),
  Set(SetCommand),
}

impl CommandsList {
  // Create a command instance from command type and arguments
  pub fn from_command_type(cmd_type: &str, args: &str) -> Option<Self> {
    match cmd_type {
      "PING" => Some(CommandsList::Ping(PingCommand::new())),
      "ECHO" => {
        if !args.is_empty() {
          Some(CommandsList::Echo(EchoCommand::new(args.to_string())))
        } else {
          None
        }
      }
      "SET" => {
        if !args.is_empty() {
          Some(CommandsList::Set(SetCommand::new(args.to_string())))
        } else {
          None
        }
      }
      "HELP" => Some(CommandsList::Help(HelpCommand::new())),
      _ => None,
    }
  }

  // Method to execute command with string args - for backward compatibility
  pub async fn execute_with_args(&self, args: Vec<String>) -> Result<Value> {
    match self {
      CommandsList::Ping(cmd) => cmd.execute(args),
      CommandsList::Echo(cmd) => cmd.execute(args),
      CommandsList::Help(cmd) => cmd.execute(args),
      CommandsList::Set(cmd) => cmd.execute(args),
    }
  }
}

// Updated Command trait to match how it's used with RESP values
pub trait Command {
  fn execute(&self, args: Vec<String>) -> Result<Value>;

  fn _matches(&self, _request: &str) -> bool {
    // Default implementation for matching
    false
  }
}

// Implement Command for the enum
impl Command for CommandsList {
  fn execute(&self, args: Vec<String>) -> Result<Value> {
    match self {
      CommandsList::Ping(cmd) => cmd.execute(args),
      CommandsList::Echo(cmd) => cmd.execute(args),
      CommandsList::Help(cmd) => cmd.execute(args),
      CommandsList::Set(cmd) => cmd.execute(args),
    }
  }
}

pub enum CommandType {
  Ping,
  Echo,
  Help,
  Set,
}

#[allow(dead_code)]
impl CommandType {
  pub fn from_command_type(cmd_type: &str) -> Option<CommandType> {
    match cmd_type.to_uppercase().as_str() {
      "PING" => Some(CommandType::Ping),
      "ECHO" => Some(CommandType::Echo),
      "HELP" => Some(CommandType::Help),
      "SET" => Some(CommandType::Set),
      _ => None,
    }
  }
}

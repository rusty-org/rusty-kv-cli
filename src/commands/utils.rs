use std::io;

use tokio::net::TcpStream;

use super::{echo::EchoCommand, ping::PingCommand, help::HelpCommand};

pub enum CommandsList {
  Ping(PingCommand),
  Echo(EchoCommand),
  Help(HelpCommand)
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
      },
      "HELP" => Some(CommandsList::Help(HelpCommand::new())),
      _ => None,
    }
  }
}

pub trait Command {
  async fn execute(&self, stream: &mut TcpStream) -> io::Result<()>;
  fn _matches(&self, _request: &str) -> bool {
    // Default implementation for matching
    false
  }
}

// Implement Command for the enum
impl Command for CommandsList {
  async fn execute(&self, stream: &mut TcpStream) -> io::Result<()> {
    match self {
      CommandsList::Ping(cmd) => cmd.execute(stream).await,
      CommandsList::Echo(cmd) => cmd.execute(stream).await,
      CommandsList::Help(cmd) => cmd.execute(stream).await,
    }
  }
}

pub mod echo;
pub mod ping;

use echo::EchoCommand;
use ping::PingCommand;
use std::io;
use tokio::net::TcpStream;

pub trait Command {
  async fn execute(&self, stream: &mut TcpStream) -> io::Result<()>;
  fn _matches(&self, _request: &str) -> bool {
    // @INFO Default implementation for matching
    false
  }
}

// Create an enum to hold all command types
pub enum CommandEnum {
  Ping(PingCommand),
  Echo(EchoCommand),
}

impl CommandEnum {}

// Implement Command for the enum
impl Command for CommandEnum {
  async fn execute(&self, stream: &mut TcpStream) -> io::Result<()> {
    match self {
      CommandEnum::Ping(cmd) => cmd.execute(stream).await,
      CommandEnum::Echo(cmd) => cmd.execute(stream).await,
    }
  }
}

// Update get_command to return CommandEnum instead of Box<dyn Command>
pub fn get_command(request: &str) -> Option<CommandEnum> {
  if request.starts_with("PING") {
    Some(CommandEnum::Ping(PingCommand::new()))
  } else if request.starts_with("ECHO") {
    let parts: Vec<&str> = request.splitn(2, ' ').collect();
    if parts.len() > 1 {
      Some(CommandEnum::Echo(EchoCommand::new(parts[1].to_string())))
    } else {
      None
    }
  } else {
    None
  }
}

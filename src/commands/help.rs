pub use crate::commands::lib::Command;
use std::io;
use tokio::io::AsyncWriteExt;
use tokio::net::TcpStream;

pub struct HelpCommand;

impl HelpCommand {
  pub fn new() -> Self {
    HelpCommand
  }
}

impl Command for HelpCommand {
  async fn execute(&self, socket: &mut TcpStream) -> io::Result<()> {
    let response = "+HELP: This is the help command.\r\n";
    socket.write_all(response.as_bytes()).await
  }

  fn _matches(&self, request: &str) -> bool {
    request.contains("HELP")
  }
}

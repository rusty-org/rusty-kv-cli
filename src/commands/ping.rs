use crate::commands::lib::Command;
use std::io;
use tokio::io::AsyncWriteExt;
use tokio::net::TcpStream;

pub struct PingCommand;

impl PingCommand {
  pub fn new() -> Self {
    PingCommand
  }
}

impl Command for PingCommand {
  async fn execute(&self, socket: &mut TcpStream) -> io::Result<()> {
    socket.write_all(b"+PONG\r\n").await
  }

  fn _matches(&self, request: &str) -> bool {
    request.contains("PING")
  }
}

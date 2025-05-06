use crate::commands::lib::Command;
use std::io;
use tokio::io::AsyncWriteExt;
use tokio::net::TcpStream;

pub struct EchoCommand {
  request: String,
}

impl EchoCommand {
  pub fn new(request: String) -> Self {
    EchoCommand { request }
  }
}

impl Command for EchoCommand {
  async fn execute(&self, socket: &mut TcpStream) -> io::Result<()> {
    let response = format!("{}\r\n", self.request.trim_start_matches("ECHO "));
    socket.write_all(response.as_bytes()).await
  }

  fn _matches(&self, request: &str) -> bool {
    request.contains("ECHO")
  }
}

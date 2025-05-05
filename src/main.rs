use tokio::net::{TcpListener, TcpStream};

use std::io;
use tokio::io::{AsyncReadExt, AsyncWriteExt};

mod commands;
use crate::commands::Command;

#[tokio::main]
async fn main() {
  let listener = TcpListener::bind("127.0.0.1:6379").await.unwrap();
  loop {
    let stream = listener.accept().await;
    match stream {
      Ok((stream, _)) => {
        tokio::spawn(async move {
          accept_connection(stream).await.unwrap();
        });
      }
      Err(e) => {
        println!("error: {}", e);
      }
    }
  }
}

async fn accept_connection(mut socket: TcpStream) -> io::Result<()> {
  loop {
    let mut buffer = [0; 1024];
    match socket.read(&mut buffer).await {
      Ok(0) => {
        println!("Connection closed");
        return Ok(());
      }
      Ok(n) => {
        println!("Accepted connection");
        let request = String::from_utf8_lossy(&buffer[..n]);

        if let Some(command) = commands::get_command(&request) {
          command.execute(&mut socket).await?;
        } else {
          println!("Received: {}", request);
          socket.write_all(b"-ERR unknown command\r\n").await?;
        }
      }
      Err(e) => {
        eprintln!("Error reading from socket: {}", e);
        return Err(e);
      }
    }
  }
}

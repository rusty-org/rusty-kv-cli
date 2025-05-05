use tokio::{
  net::{TcpListener, TcpStream},
  io::{AsyncReadExt, AsyncWriteExt},
};
use dotenv::dotenv;
use log::{info, error};
use simple_logger::SimpleLogger;

use std::io;

mod commands;
use commands::Command;

#[tokio::main]
async fn main() {
  SimpleLogger::new().init().unwrap();

  info!("Starting Redis clone server...");

  dotenv().unwrap_or_else(|_| {
    error!("Failed to load .env file, exiting...");
    std::process::exit(1);
  });

  let kv_host = std::env::var("KV_HOST").unwrap_or_else(|_| "127.0.0.1".to_string());
  let kv_port = std::env::var("KV_PORT").unwrap_or_else(|_| "6379".to_string());
  let listener = TcpListener::bind(format!("{}:{}", kv_host, kv_port)).await.unwrap();

  info!("Listening on {}:{}", kv_host, kv_port);

  loop {
    let stream = listener.accept().await;
    match stream {
      Ok((stream, _)) => {
        tokio::spawn(async move {
          if let Err(e) = accept_connection(stream).await {
            error!("Error handling connection: {}", e);
          }
        });
        info!("Accepted a new connection");
      }
      Err(e) => {
        error!("Error accepting connection: {}", e);
      }
    }
  }
}

async fn accept_connection(mut socket: TcpStream) -> io::Result<()> {
  loop {
    // @NOTE Max size of the buffer to read from the socket
    let mut buffer = [0; 1024]; // @TODO make this dynamic
    match socket.read(&mut buffer).await {
      Ok(0) => {
        info!("Connection closed");
        return Ok(());
      }
      Ok(n) => {
        info!("Accepted connection");
        let request = String::from_utf8_lossy(&buffer[..n]);

        if let Some(command) = commands::get_command(&request) {
          command.execute(&mut socket).await?;
        } else {
          info!("Received: {}", request);
          socket.write_all(b"-ERR unknown command\r\n").await?;
        }
      }
      Err(e) => {
        error!("Error reading from socket: {}", e);
        return Err(e);
      }
    }
  }
}

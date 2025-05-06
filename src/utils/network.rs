use std::io;

use log::{error, info, warn};
use tokio::{
  io::{AsyncReadExt, AsyncWriteExt},
  net::TcpStream,
};

use crate::commands::{self, lib::Command};

pub struct NetworkUtils;

impl NetworkUtils {
  pub async fn accept_connection(&self, mut socket: TcpStream) -> io::Result<()> {
    loop {
      // @NOTE Max size of the buffer to read from the socket
      let mut buffer = [0; 1024]; // @TODO make this dynamic
      match socket.read(&mut buffer).await {
        Ok(0) => {
          warn!("Connection closed");
          return Ok(());
        }
        Ok(n) => {
          let request = String::from_utf8_lossy(&buffer[..n]);

          info!("Request received: {}", request);

          if let Some(command) = commands::get_command(&request) {
            command.execute(&mut socket).await?;
          } else {
            info!("Received: {}", request);
            socket.write_all(b"-ERR unknown command\r\n").await?;

            error!("Unknown command received: {}", request);

            info!("Request details: {}", request);
          }
        }
        Err(e) => {
          error!("Error reading from socket: {}", e);
          return Err(e);
        }
      }
    }
  }
}

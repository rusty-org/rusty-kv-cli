use crate::resp::value::Value;
use anyhow::Result;
use bytes::{Buf, BytesMut};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::TcpStream;

use super::parser::RespParser;

pub struct RespHandler {
  stream: TcpStream,
  buffer: BytesMut,
}

impl RespHandler {
  pub fn new(stream: TcpStream) -> Self {
    Self {
      stream,
      buffer: BytesMut::with_capacity(1024),
    }
  }

  pub async fn read_value(&mut self) -> Result<Option<Value>> {
    loop {
      let bytes_read = self.stream.read_buf(&mut self.buffer).await?;
      if bytes_read == 0 {
        if self.buffer.is_empty() {
          return Ok(None);
        } else {
          return Err(anyhow::anyhow!("Connection closed unexpectedly"));
        }
      }

      match RespParser::parse_message(&mut self.buffer) {
        Ok(Some((val, consumed))) => {
          self.buffer.advance(consumed);
          return Ok(Some(val));
        }
        Ok(None) => continue,
        Err(e) => return Err(e),
      }
    }
  }

  pub async fn write_value(&mut self, value: Value) -> Result<()> {
    let data = value.serialize();
    self.stream.write_all(data.as_bytes()).await?;
    Ok(())
  }
}

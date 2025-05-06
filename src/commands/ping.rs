use crate::commands::lib::Command;
use crate::resp::value::Value;
use anyhow::Result;

pub struct PingCommand;

impl PingCommand {
  pub fn new() -> Self {
    Self
  }
}

impl Command for PingCommand {
  fn execute(&self, args: Vec<String>) -> Result<Value> {
    if args.is_empty() {
      Ok(Value::SimpleString("PONG".to_string()))
    } else {
      Ok(Value::BulkString(args[0].clone()))
    }
  }
}

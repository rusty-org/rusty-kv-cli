use crate::commands::lib::Command;
use crate::resp::value::Value;
use anyhow::{Result, anyhow};

pub struct SetCommand {
  key_value: String,
}

impl SetCommand {
  pub fn new(key_value: String) -> Self {
    Self { key_value }
  }
}

impl Command for SetCommand {
  fn execute(&self, args: Vec<String>) -> Result<Value> {
    if args.len() < 2 {
      return Err(anyhow!("SET requires a key and a value"));
    }

    Ok(Value::SimpleString("OK".to_string()))
  }
}

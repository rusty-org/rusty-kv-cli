use crate::resp::value::Value;
use anyhow::{Ok, Result, anyhow};
use std::collections::HashMap;
use std::sync::Arc;
use tokio::sync::Mutex;

pub struct CommandExecutor {
  db: Arc<Mutex<HashMap<String, Value>>>,
}

impl CommandExecutor {
  pub fn new() -> Self {
    Self {
      db: Arc::new(Mutex::new(HashMap::new())),
    }
  }

  pub async fn execute(&self, command: &str, args: Vec<String>) -> Result<Value> {
    match command {
      "PING" => self.ping(args).await,
      "HELP" => self.help(args).await,
      "ECHO" => self.echo(args).await,
      "GET" => self.get(args).await,
      "SET" => self.set(args).await,
      "DEL" => self.del(args).await,
      _ => Err(anyhow!("Unknown command: {}", command)),
    }
  }

  async fn ping(&self, args: Vec<String>) -> Result<Value> {
    if args.is_empty() {
      Ok(Value::SimpleString("PONG".to_string()))
    } else {
      Ok(Value::BulkString(args[0].clone()))
    }
  }

  async fn help(&self, _args: Vec<String>) -> Result<Value> {
    let help_text = "Available commands:\n\
                        PING - Test connection\n\
                        ECHO <message> - Echo back a message\n\
                        GET <key> - Get value for key\n\
                        SET <key> <value> - Set key to value\n\
                        DEL <key> [<key> ...] - Delete keys\n\
                        HELP - Show this help";

    Ok(Value::BulkString(help_text.to_string()))
  }

  async fn echo(&self, args: Vec<String>) -> Result<Value> {
    if args.is_empty() {
      return Err(anyhow!("ECHO requires at least one argument"));
    }
    Ok(Value::BulkString(args[0].clone()))
  }

  async fn get(&self, args: Vec<String>) -> Result<Value> {
    if args.is_empty() {
      return Err(anyhow!("GET requires a key"));
    }

    let db = self.db.lock().await;
    match db.get(&args[0]) {
      Some(value) => Ok(value.clone()),
      None => Ok(Value::Null),
    }
  }

  async fn set(&self, args: Vec<String>) -> Result<Value> {
    if args.len() < 2 {
      return Err(anyhow!("SET requires a key and a value"));
    }

    let key = args[0].clone();
    let value = Value::BulkString(args[1].clone());

    let mut db = self.db.lock().await;
    db.insert(key, value);

    Ok(Value::SimpleString("OK".to_string()))
  }

  async fn del(&self, args: Vec<String>) -> Result<Value> {
    if args.is_empty() {
      return Err(anyhow!("DEL requires at least one key"));
    }

    let mut count = 0;
    let mut db = self.db.lock().await;

    for key in args {
      if db.remove(&key).is_some() {
        count += 1;
      }
    }

    Ok(Value::Integer(count))
  }
}

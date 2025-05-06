#[derive(Clone, Debug)]
pub enum Value {
  Null,
  SimpleString(String),
  BulkString(String),
  Array(Vec<Value>),
  Error(String),
  Integer(i64),
  Boolean(bool),
}

impl Value {
  pub fn serialize(&self) -> String {
    match self {
      Value::Null => "$-1\r\n".to_string(),
      Value::SimpleString(s) => format!("+{}\r\n", s),
      Value::BulkString(s) => format!("${}\r\n{}\r\n", s.len(), s),
      Value::Integer(i) => format!(":{}\r\n", i),
      Value::Error(s) => format!("-{}\r\n", s),
      Value::Boolean(b) => format!("#{}\r\n", if *b { "t" } else { "f" }),
      Value::Array(arr) => {
        let mut s = format!("*{}\r\n", arr.len());
        for v in arr {
          s.push_str(&v.serialize());
        }
        s
      }
    }
  }

  pub fn to_command(&self) -> Option<(String, Vec<String>)> {
    if let Value::Array(elements) = self {
      if elements.is_empty() {
        return None;
      }
      let command = match &elements[0] {
        Value::BulkString(s) => s.clone(),
        Value::SimpleString(s) => s.clone(),
        _ => return None,
      };
      let args = elements[1..]
        .iter()
        .filter_map(|v| match v {
          Value::BulkString(s) | Value::SimpleString(s) => Some(s.clone()),
          _ => None,
        })
        .collect();
      Some((command.to_uppercase(), args))
    } else {
      None
    }
  }
}

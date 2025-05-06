use log::info;
use simple_logger::SimpleLogger;

pub struct Logger;

impl Logger {
  pub fn setup() {
    SimpleLogger::new()
      .with_colors(true)
      .with_level(log::LevelFilter::Trace)
      .with_timestamp_format(time::macros::format_description!(
        "[year]-[month]-[day] [hour]:[minute]:[second]"
      ))
      .init()
      .unwrap();
    info!("Setting up default logger !")
  }
}

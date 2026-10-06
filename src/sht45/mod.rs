use core::fmt;

use esp_hal::delay::Delay;

use crate::command_traits::{Command, ParseableResponse};

pub mod commands;

pub struct SHT45<I2C> {
    i2c: I2C,
    address: u8,
}

impl<I2C> SHT45<I2C>
where
    I2C: embedded_hal::i2c::I2c,
{
    pub fn new(i2c: I2C, address: u8) -> Self {
        Self { i2c, address }
    }

    pub fn read_data<C>(&mut self) -> Result<C::Output, I2C::Error>
    where
        C: Command + ParseableResponse,
        [(); C::RESP_LEN]:,
    {
        let mut buf = [0u8; C::RESP_LEN];

        self.i2c.write(self.address, &[C::BYTE])?;
        Delay::new().delay_millis(10); // Give the sensor time to read
        self.i2c.read(self.address, &mut buf)?;

        Ok(C::parse(&buf))
    }

    pub fn reset(&mut self) -> Result<(), I2C::Error> {
        self.i2c.write(self.address, &[0x00, commands::Reset::BYTE])
    }
}

#[derive(Debug, Clone, Copy)]
pub struct Measurement {
    pub temperature: f32,
    pub humidity: f32,
}

impl fmt::Display for Measurement {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}°F, {}% RH", self.temperature, self.humidity)
    }
}

impl Default for Measurement {
    fn default() -> Self {
        Self {
            temperature: 0.0,
            humidity: 0.0,
        }
    }
}

const fn raw_to_fahrenheit(raw: u16) -> f32 {
    -49.0 + 315.0 * (raw as f32 / u16::MAX as f32)
}

#[allow(dead_code)]
const fn raw_to_celsius(raw: u16) -> f32 {
    -45.0 + 175.0 * (raw as f32 / u16::MAX as f32)
}

const fn raw_to_humidity(raw: u16) -> f32 {
    -6.0 + 125.0 * (raw as f32 / u16::MAX as f32)
}

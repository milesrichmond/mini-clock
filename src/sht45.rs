use core::fmt;

use esp_hal::delay::Delay;

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
        self.i2c.write(self.address, &[0x00, Reset::BYTE])
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

pub trait Command {
    const RESP_LEN: usize;
    const BYTE: u8;
}

pub trait ParseableResponse {
    type Output;
    fn parse(buf: &[u8]) -> Self::Output;
}

pub struct TRHHigh;
impl Command for TRHHigh {
    const RESP_LEN: usize = 5;
    const BYTE: u8 = 0xFD;
}
impl ParseableResponse for TRHHigh {
    type Output = Measurement;
    fn parse(buf: &[u8]) -> Self::Output {
        let raw_temperature = u16::from_be_bytes([buf[0], buf[1]]);
        let raw_humidity = u16::from_be_bytes([buf[3], buf[4]]);

        Measurement {
            temperature: raw_to_fahrenheit(raw_temperature),
            humidity: raw_to_humidity(raw_humidity),
        }
    }
}

pub struct TRHMed;
impl Command for TRHMed {
    const RESP_LEN: usize = 5;
    const BYTE: u8 = 0xF6;
}
impl ParseableResponse for TRHMed {
    type Output = Measurement;
    fn parse(buf: &[u8]) -> Self::Output {
        let raw_temperature = u16::from_be_bytes([buf[0], buf[1]]);
        let raw_humidity = u16::from_be_bytes([buf[3], buf[4]]);

        Measurement {
            temperature: raw_to_fahrenheit(raw_temperature),
            humidity: raw_to_humidity(raw_humidity),
        }
    }
}

pub struct TRHLow;
impl Command for TRHLow {
    const RESP_LEN: usize = 5;
    const BYTE: u8 = 0xE0;
}
impl ParseableResponse for TRHLow {
    type Output = Measurement;
    fn parse(buf: &[u8]) -> Self::Output {
        let raw_temperature = u16::from_be_bytes([buf[0], buf[1]]);
        let raw_humidity = u16::from_be_bytes([buf[3], buf[4]]);

        Measurement {
            temperature: raw_to_fahrenheit(raw_temperature),
            humidity: raw_to_humidity(raw_humidity),
        }
    }
}

pub struct GetSerialNum;
impl Command for GetSerialNum {
    const RESP_LEN: usize = 2;
    const BYTE: u8 = 0x89;
}
impl ParseableResponse for GetSerialNum {
    type Output = u16;
    fn parse(_buf: &[u8]) -> Self::Output {
        todo!()
    }
}

pub struct Reset;
impl Command for Reset {
    const RESP_LEN: usize = 0;
    const BYTE: u8 = 0x94;
}

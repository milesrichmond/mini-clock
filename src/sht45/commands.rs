use crate::{
    command_traits::{Command, ParseableResponse},
    sht45::{Measurement, raw_to_fahrenheit, raw_to_humidity},
};

pub struct TRHHigh;
impl Command for TRHHigh {
    const BYTE: u8 = 0xFD;
}
impl ParseableResponse for TRHHigh {
    const RESP_LEN: usize = 5;
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
    const BYTE: u8 = 0xF6;
}
impl ParseableResponse for TRHMed {
    const RESP_LEN: usize = 5;
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
    const BYTE: u8 = 0xE0;
}
impl ParseableResponse for TRHLow {
    const RESP_LEN: usize = 5;
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
    const BYTE: u8 = 0x89;
}
impl ParseableResponse for GetSerialNum {
    const RESP_LEN: usize = 2;
    type Output = u16;
    fn parse(_buf: &[u8]) -> Self::Output {
        todo!()
    }
}

pub struct Reset;
impl Command for Reset {
    const BYTE: u8 = 0x94;
}

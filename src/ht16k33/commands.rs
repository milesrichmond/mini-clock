use super::BlinkConfig;
use crate::command_traits::Command;

pub struct CfgEnableOscillator;
impl Command for CfgEnableOscillator {
    const BYTE: u8 = 0x20;
}

impl CfgEnableOscillator {
    pub fn byte_with(enable: bool) -> u8 {
        Self::BYTE | (enable as u8)
    }
}

pub struct CfgBlink;
impl Command for CfgBlink {
    const BYTE: u8 = 0x80;
}

impl CfgBlink {
    pub fn byte_with(cfg: BlinkConfig, en_display: bool) -> u8 {
        use BlinkConfig as E;
        match cfg {
            E::Static => Self::BYTE | (en_display as u8),
            E::Blink2HZ => Self::BYTE | 0x1 << 1 | (en_display as u8),
            E::Blink1HZ => Self::BYTE | 0x2 << 1 | (en_display as u8),
            E::Blink05HZ => Self::BYTE | 0x3 << 1 | (en_display as u8),
        }
    }
}

pub struct CfgBrightness;
impl Command for CfgBrightness {
    const BYTE: u8 = 0xE0;
}

impl CfgBrightness {
    pub fn byte_with(brightness: u8) -> u8 {
        Self::BYTE | (brightness & 0x0F)
    }
}

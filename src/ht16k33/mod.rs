use log::info;

mod commands;
mod font;

pub struct HT16K33<I2C> {
    i2c: I2C,
    address: u8,

    pub config: Config,
}

impl<I2C> HT16K33<I2C>
where
    I2C: embedded_hal::i2c::I2c,
{
    pub fn new(i2c: I2C, address: u8) -> Self {
        Self {
            i2c,
            address,
            config: Config {
                en_display: true,
                en_oscillator: true,
                brightness: 0xF,
                display_blink: BlinkConfig::Blink1HZ,
                en_colon: false,
            },
        }
    }

    pub fn update_config(&mut self) -> Result<(), I2C::Error> {
        self.i2c.write(
            self.address,
            &[commands::CfgEnableOscillator::byte_with(
                self.config.en_oscillator,
            )],
        )?;

        self.i2c.write(
            self.address,
            &[commands::CfgBlink::byte_with(
                self.config.display_blink,
                self.config.en_display,
            )],
        )?;

        self.i2c.write(
            self.address,
            &[commands::CfgBrightness::byte_with(self.config.brightness)],
        )?;

        Ok(())
    }

    pub fn write_contents(&mut self, content: &str) -> Result<(), I2C::Error> {
        use crate::ht16k33::font::DisplayChar;
        let mut buffer = [0u8; 11];

        let mut char_idx = 0;
        for c in content.chars() {
            if c == '.' {
                if char_idx != 0 {
                    buffer[(char_idx - 1) * 2 + 1] |= DisplayChar::Decimal.to_byte();
                }
                continue;
            }

            if char_idx == 5 {
                break;
            }

            if char_idx == 2 {
                char_idx += 1;
            }

            buffer[char_idx * 2 + 1] |=
                DisplayChar::from(c).unwrap_or(DisplayChar::Space).to_byte();
            char_idx += 1;
        }

        self.i2c.write(self.address, &buffer)?;
        Ok(())
    }
}

#[derive(Debug, Clone, Copy)]
pub struct Config {
    pub en_display: bool,
    pub en_oscillator: bool,
    pub brightness: u8,
    pub display_blink: BlinkConfig,
    pub en_colon: bool,
}

#[derive(Debug, Clone, Copy)]
pub enum BlinkConfig {
    Static,
    Blink05HZ,
    Blink1HZ,
    Blink2HZ,
}

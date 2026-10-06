#![no_std]
#![no_main]
#![allow(incomplete_features)]
#![feature(generic_const_exprs)]

use core::cell::RefCell;

use embedded_hal_bus::i2c::RefCellDevice;
use esp_hal::{
    clock::CpuClock,
    main,
    peripherals::Peripherals,
    time::{Duration, Instant, Rate},
};
use fixedstr::{str_format, str8};
use log::{error, info};
use mini_clock::{
    ht16k33::{Config, HT16K33},
    sht45::{SHT45, commands::TRHHigh},
};

#[panic_handler]
fn panic(panic_info: &core::panic::PanicInfo) -> ! {
    error!("{}", panic_info);
    loop {}
}

esp_bootloader_esp_idf::esp_app_desc!();

fn esp_generate_config() -> Peripherals {
    // generator version: 1.4.0
    // generator parameters: -o esp32 -o esp32-pico-mini-01 -o log
    esp_println::logger::init_logger_from_env();
    let config = esp_hal::Config::default().with_cpu_clock(CpuClock::_80MHz);
    esp_hal::init(config)
}

#[main]
fn main() -> ! {
    let peripherals = esp_generate_config();

    // Bootstrap ports, don't use
    let _gpio6 = peripherals.GPIO6;
    let _gpio7 = peripherals.GPIO7;
    let _gpio8 = peripherals.GPIO8;
    let _gpio9 = peripherals.GPIO9;
    let _gpio10 = peripherals.GPIO10;
    let _gpio11 = peripherals.GPIO11;
    let _gpio16 = peripherals.GPIO16;
    let _gpio17 = peripherals.GPIO17;

    // I2C config
    let i2c_bus = esp_hal::i2c::master::I2c::new(
        peripherals.I2C0,
        esp_hal::i2c::master::Config::default().with_frequency(Rate::from_khz(100)),
    )
    .unwrap()
    .with_scl(peripherals.GPIO20)
    .with_sda(peripherals.GPIO22);
    let i2c_cell = RefCell::new(i2c_bus);

    // I2C devices
    let mut sht45 = SHT45::new(RefCellDevice::new(&i2c_cell), 0x44);

    let mut display_1 = HT16K33::new(RefCellDevice::new(&i2c_cell), 0x70);
    display_1.config = Config {
        en_display: true,
        en_oscillator: true,
        brightness: 0x8,
        display_blink: mini_clock::ht16k33::BlinkConfig::Static,
        en_colon: false,
    };
    if let Err(err) = display_1.update_config() {
        info!("{}", err);
    }

    // Application Loop
    loop {
        let data = sht45.read_data::<TRHHigh>();
        info!("data: {}", data.unwrap_or_default());
        let msg = str_format!(str8, "{:4.2}", data.unwrap().temperature);
        let _ = display_1.write_contents(&msg);

        let delay_start = Instant::now();
        while delay_start.elapsed() < Duration::from_millis(500) {}
    }
}

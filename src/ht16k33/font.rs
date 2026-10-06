#[allow(non_camel_case_types)]
#[derive(Debug)]
pub enum DisplayChar {
    Space,
    Decimal,
    Dash,
    Num_0,
    Num_1,
    Num_2,
    Num_3,
    Num_4,
    Num_5,
    Num_6,
    Num_7,
    Num_8,
    Num_9,
    Char_A,
    Char_C,
    Char_E,
    Char_F,
    Char_H,
    Char_b,
    Char_c,
    Char_d,
    Char_h,
    Char_n,
    Char_r,
    Char_o,
    Char_y,
}

impl DisplayChar {
    pub const fn from(c: char) -> Result<Self, ()> {
        match c {
            ' ' => Ok(Self::Space),
            '.' => Ok(Self::Decimal),
            '-' => Ok(Self::Dash),
            '0' => Ok(Self::Num_0),
            '1' => Ok(Self::Num_1),
            '2' => Ok(Self::Num_2),
            '3' => Ok(Self::Num_3),
            '4' => Ok(Self::Num_4),
            '5' => Ok(Self::Num_5),
            '6' => Ok(Self::Num_6),
            '7' => Ok(Self::Num_7),
            '8' => Ok(Self::Num_8),
            '9' => Ok(Self::Num_9),
            'A' => Ok(Self::Char_A),
            'C' => Ok(Self::Char_C),
            'E' => Ok(Self::Char_E),
            'F' => Ok(Self::Char_F),
            'H' => Ok(Self::Char_H),
            'b' => Ok(Self::Char_b),
            'c' => Ok(Self::Char_c),
            'd' => Ok(Self::Char_d),
            'h' => Ok(Self::Char_h),
            'n' => Ok(Self::Char_n),
            'r' => Ok(Self::Char_r),
            'o' => Ok(Self::Char_o),
            'y' => Ok(Self::Char_y),
            _ => Err(()),
        }
    }

    pub fn from_str<'a>(s: &str, buf: &'a mut [Self]) -> Result<&'a mut [Self], ()> {
        let mut i = 0;
        for c in s.chars() {
            if i >= buf.len() {
                return Err(());
            }
            buf[i] = Self::from(c)?;
            i += 1;
        }
        Ok(&mut buf[..i])
    }

    pub const fn to_byte(&self) -> u8 {
        use DisplayChar as E;
        match self {
            E::Space => 0x00,
            E::Decimal => 0x80,
            E::Dash => 0x40,
            E::Num_0 => 0x3F,
            E::Num_1 => 0x06,
            E::Num_2 => 0x5B,
            E::Num_3 => 0x4F,
            E::Num_4 => 0x66,
            E::Num_5 => 0x6D,
            E::Num_6 => 0x7D,
            E::Num_7 => 0x07,
            E::Num_8 => 0x7F,
            E::Num_9 => 0x6F,
            E::Char_A => 0x77,
            E::Char_C => 0x39,
            E::Char_E => 0x79,
            E::Char_F => 0x71,
            E::Char_H => 0x76,
            E::Char_b => 0x7C,
            E::Char_c => 0x58,
            E::Char_d => 0x5E,
            E::Char_h => 0x74,
            E::Char_n => 0x54,
            E::Char_r => 0x50,
            E::Char_o => 0x5C,
            E::Char_y => 0x49,
        }
    }

    pub fn to_byte_slice<'a>(arr: &[Self], buf: &'a mut [u8]) -> Result<&'a mut [u8], ()> {
        let mut i = 0;
        for c in arr.iter() {
            if i >= buf.len() {
                return Err(());
            }
            buf[i] = Self::to_byte(c);
            i += 1;
        }
        Ok(&mut buf[..i])
    }
}

pub trait Command {
    const BYTE: u8;
}

pub trait ParseableResponse {
    const RESP_LEN: usize;
    type Output;
    fn parse(buf: &[u8]) -> Self::Output;
}

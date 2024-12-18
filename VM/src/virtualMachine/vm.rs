//This will set everything up when the machine boots up
use super::{cpu::Registers, memory::Memory};
#[derive(Debug)]
pub struct VM {
    reg: Registers,
    mem: Memory,
}
impl VM {
    pub fn bootUp() -> Self {
        VM {
            reg: Registers::create(),
            mem: Memory::create(),
        }
    }
}

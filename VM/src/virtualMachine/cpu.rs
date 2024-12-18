use crate::virtualMachine::specs;
#[derive(Debug)]
pub struct Registers {
    registers: Box<[u64]>,
    stack_pointer: u16,
    program_counter: u16,
}
impl Registers {
    pub fn create() -> Self {
        Registers {
            registers: vec![0; specs::GENERAL_REGISTERS].into_boxed_slice(),
            stack_pointer: 0,
            program_counter: 0,
        }
    }
}

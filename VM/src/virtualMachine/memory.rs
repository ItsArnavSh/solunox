use crate::virtualMachine::specs;
use std::fmt::{self, Debug}; // Import Debug trait
pub struct Memory {
    memory: Box<[u8]>,
}
impl Debug for Memory {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "Memory:")?; // Write a header line
        for (index, &data) in self.memory.iter().enumerate() {
            if data != 0 {
                writeln!(f, "Index {}: {}", index, data)?; // Write non-zero data
            }
        }
        Ok(()) // Return Ok to satisfy fmt::Result
    }
}
impl Memory {
    pub fn create() -> Self {
        Memory {
            memory: vec![0; specs::STACK_SIZE].into_boxed_slice(),
        }
    }
}

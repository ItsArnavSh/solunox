use virtualMachine::vm::VM;

mod virtualMachine;
fn main() {
    let machine: VM = VM::bootUp();
    println!("The Details of the machine are: {:?}", machine);
}

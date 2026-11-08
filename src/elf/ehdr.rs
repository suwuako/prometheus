#![allow(non_camel_case_types)]
pub type Elf64_Byte = u8;
pub type Elf64_Half = u16;
pub type Elf64_Word = u32;
pub type Elf64_Addr = u64;
pub type Elf64_Offset = u64;

pub struct HeaderType(pub u16);
impl HeaderType {
}

#[derive(Clone, Copy, Debug)]
#[repr(C)]
/// Struct for elf headers
pub struct Elf64_Ehdr {
    pub magic: [Elf64_Byte; 4],
    pub class: Elf64_Byte,
    pub data: Elf64_Byte,
    pub version: Elf64_Byte,
    pub osabi: Elf64_Byte,
    pub abiversion: Elf64_Byte,
    pub padding: [Elf64_Byte; 7],
    pub e_type: Elf64_Half,
    pub e_machine: Elf64_Half,
    pub e_version: Elf64_Word,
    pub e_entry: Elf64_Addr,
    pub e_phoff: Elf64_Offset,
    pub e_shoff: Elf64_Offset,
    pub e_flags: Elf64_Word,
    pub e_ehsize: Elf64_Half,
    pub e_phentsize: Elf64_Half,
    pub e_phnum: Elf64_Half,
    pub e_shentsize: Elf64_Half,
    pub e_shnum: Elf64_Half,
    pub e_shstrndx: Elf64_Half,
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::mem;

    #[test]
    fn ehdr_is_64_bytes() {
        let size_by_type = mem::size_of::<Elf64_Ehdr>();
        assert!(size_by_type == 64);
    }
}

/*
struct Elf64_Ehdr {
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
  Elf64_Elf64_Half	e_type;			/* Object file type */
  Elf64_Elf64_Half	e_machine;		/* Architecture */
  Elf64_Elf64_Word	e_version;		/* Object file version */
  Elf64_Elf64_Addr	e_entry;		/* Entry point virtual Elf64_Address */
  Elf64_Off	e_phoff;		/* Program header table file offset */
  Elf64_Off	e_shoff;		/* Section header table file offset */
  Elf64_Elf64_Word	e_flags;		/* Processor-specific flags */
  Elf64_Elf64_Half	e_ehsize;		/* ELF header size in Elf64_Bytes */
  Elf64_Elf64_Half	e_phentsize;		/* Program header table entry size */
  Elf64_Elf64_Half	e_phnum;		/* Program header table entry count */
  Elf64_Elf64_Half	e_shentsize;		/* Section header table entry size */
  Elf64_Elf64_Half	e_shnum;		/* Section header table entry count */
  Elf64_Elf64_Half	e_shstrndx;		/* Section header string table index */
};
*/

# Floppy Writer

A 16-bit DOS program to write `.img` files to a floppy disk sector by sector. Designed for testing bootloaders or working with old hardware.

---

## Features

- Text-mode interface.  
- Prompt for `.img` file path.  
- Writes the file to drive A:.  
- Works on DOS or Windows XP/9x using NTVDM.
- Not tested but should run on Windows 3 and 3.1

**Warning:** Direct hardware access is used. Only run on old or disposable hardware.

---

## Usage

1. Compile with Borland C 5 (16-bit DOS).  
2. Run in DOS or inside NTVDM.  
3. Enter the path to the `.img` file when prompted.  
4. The program writes it to the floppy.


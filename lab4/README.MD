IRQ
Mouser controller raises IRQ line 12
Interrupt handler reads the bytes of the packet, one per interrupt 

Mouse Packets
PS/2 mouse uses 3-byte packets

The i8042
Commands are written in 0x64.

Writing commands to mouse

KBC-command 0xD4, allows the device driver to issue commands directly to the mouse in port 0x64
Send further mouse command to port 0x60
Receives an acknowledgment byte, which can be read from port 0x60 (Must be always read)
Executes the program and puts it's response (if any) in port 0x60, in case of 0xFE, signals an error and need to resent the entire command from the begginning


Synchronization Issues
Bit(3) of byte 1 must be always 1, if otherwise it means that the byte received is not byte 1 and we can perceive that the driver is not in sync with the mouse

Sending commands in stream mode
If has been enable with (0xF4), must be disabled (0xF5)

Interrupts subscription
Must be made to prevent the "stealing" of the interrupts using (IRQ_REENABLE | IRQ_EXCLUSE)

Waiting for the Mouse to Responde
Use tickdelay() just like in lab3

mouse_test_packet(uint32_t cnt)
1.Configure the mouse properly
2.Receive the number of mouse packets specified in its argument via interrupts
3.Parse the mouse packets
4.Print them in a friendly way
5.Reset the mouse to Minix's default configuration, before exiting
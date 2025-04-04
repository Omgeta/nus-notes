# sample2.asm
       .data 0x10000100
msg:   .asciiz "Hello"
       .text
main:  li $v0, 4
       la $a0, msg
       syscall
       li $v0, 10
       syscall

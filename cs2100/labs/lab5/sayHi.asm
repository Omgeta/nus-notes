# sayHi.asm
     .data 
str1: .asciiz "Before function\n"
str2: .asciiz "After function\n"
str3: .asciiz "Inside function: Say Hi!\n"

      .text
main: 
	li   $v0, 4    # system call code for print_string
	la   $a0, str1 # address of string to print
	syscall        # print the string

	jal sayHi      # Make a function call to sayHi()

	li   $v0, 4    # system call code for print_string
	la   $a0, str2 # address of string to print
	syscall        # print the string

	# End of main, make a syscall to "exit"
	li   $v0, 10   # system call code for exit
	syscall        # terminate program


# start of function sayHi()
sayHi:  
	li   $v0, 4    # system call code for print_string
	la   $a0, str3 # address of string to print
	syscall        # print the string
  jr	 $ra        # Use "jr" to go back to caller (main in this case)


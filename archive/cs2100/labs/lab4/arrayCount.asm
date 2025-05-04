# arrayCount.asm
  .data 
arrayA: .word 11, 0, 31, 22, 9, 17, 6, 9   # arrayA has 8 values
count:  .word 999             		   	   # dummy value

  .text
main:
    # code to setup the variable mappings
    la  $t0, arrayA
	la  $t8, count

    # code for reading in the user value X
	li  $v0, 5
	syscall
	add $t7, $zero, $v0

	# code for creating mask
	addi $t5, $t7, -1

    # code for counting multiples of X in arrayA
	li  $a0, 0
	
loop:
	beq $t0, $t8, exit
	lw  $t1, 0($t0)
	and $t1, $t1, $t5
	bne $t1, $zero, skip
	addi $a0, $a0, 1
skip:
	addi $t0, 4
	j loop

    # code for printing result
exit:
	li  $v0, 1
	syscall
	
    # code for terminating program
    li  $v0, 10
    syscall

# b92902106 ¸ê¤u¤G ²ø¨å¿Ä
.data
.globl main
word_array:
	.word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10

.text
main:
	la $t1 word_array	# load the address of word_array
	Loop:
		lw $t3 0($t1)	# load a word
		add $t0 $t0 $t3	# increase the sum by the word
		addi $t1 $t1 4	# increase the index by 4
		addi $t2 $t2 1	# increase the counter by 1
		bne $t2 10 Loop	# if counter is unequal to 10, back to Loop

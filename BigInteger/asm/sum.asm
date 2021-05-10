include bgnt.inc

.code

ASM_BigInteger_sum proc
	; Prototype void ASM_BigInteger_sum(BigInteger* a, BigInteger* b, BigInteger* c)

	push rsi
	push rdi
	push r9
	push r10
	push r11
	push r12
	push r13
	
	mov rsi, qword ptr[rcx+ BigInteger._value]
	mov rdi, qword ptr[rdx+ BigInteger._value]
	mov r9, qword ptr[rcx + BigInteger._size]
	mov r10, qword ptr[rdx + BigInteger._size]
	mov r13, qword ptr[r8 + BigInteger._value]

	cmp r9, r10
	
	cmovl r9, qword ptr[rdx + BigInteger._size]
	cmovl r10, qword ptr[rcx + BigInteger._size]
	cmovl rsi, qword ptr[rdx+ BigInteger._value]
	cmovl rdi, qword ptr[rcx+ BigInteger._value]
	
	xor rax, rax
	mov r11, r10

LB0:
	cmp r11, 0
	je LE0
	dec r11

	shr rax, 1
	mov r12, qword ptr[rsi]
	adc r12, qword ptr[rdi]
	rcl rax, 1

	mov qword ptr[r13], r12

	add rsi, 8
	add rdi, 8
	add r13, 8

	jmp LB0
LE0:

	mov r11, r9
	sub r11, r10

LB1:
	cmp r11, 0
	je LE1
	dec r11

	shr rax, 1
	mov r12, qword ptr[rsi]
	adc r12, 0
	rcl rax, 1

	mov qword ptr[r13], r12

	add rsi, 8
	add rdi, 8
	add r13, 8

	jmp LB1
LE1:

	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop rdi
	pop rsi

	ret
ASM_BigInteger_sum endp

end

include bgnt.inc

.code

ASM_BigInteger_bcd_to_bin proc
	; Prototype void ASM_BigInteger_bcd_to_bin(BigInteger* bg, __int64* value)

	push rsi
	push rdi
	push rcx
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r15

	mov r15, rcx

	mov rsi, qword ptr [r15 + BigInteger._value]
	mov rdi, rdx

	mov r8, qword ptr [r15 + BigInteger._size]
	shl r8, 6
	mov r10, 1

LB0:
	cmp r8, 0
	je LE0
	dec r8

	xor rax, rax
	mov r9, qword ptr [r15 + BigInteger._size]
	
LB1:
	cmp r9, 0
	je LE1
	dec r9

	mov r11, qword ptr [rsi + r9*8]
	shr rax, 1
	rcr r11, 1
	cmovc rax, r10

	mov cl, 64
LB3:
	cmp cl, 0
	je LE3
	sub cl, 4

	mov r12, 1111b
	mov r13, r12
	shl r12, cl
	shl r13, cl
	and r12, r11
	shr r12, cl
	cmp r12, 8
	jl CND0
	sub r12, 3
	shl r12, cl
	not r13
	and r11, r13
	or r11, r12
CND0:
	jmp LB3
LE3:

	mov qword ptr [rsi + r9*8], r11
	jmp LB1
LE1:
	mov r9, qword ptr [r15 + BigInteger._size]

LB2:
	cmp r9, 0
	je LE2
	dec r9

	mov r11, qword ptr [rdi + r9*8]
	shr rax, 1
	rcr r11, 1
	cmovc rax, r10

	mov qword ptr [rdi + r9*8], r11
	jmp LB2
LE2:
	
	jmp LB0
LE0:

	pop r15
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rcx
	pop rdi
	pop rsi

	ret
ASM_BigInteger_bcd_to_bin endp

ASM_BigInteger_bin_to_bcd proc
	; Prototype void ASM_BigInteger_bin_to_bcd(BigInteger* bg, __int64* value)

	push rsi
	push rdi
	push rcx
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r15

	mov r15, rcx

	mov rsi, qword ptr [r15 + BigInteger._value]
	mov rdi, rdx

	mov r8, qword ptr [r15 + BigInteger._size]
	shl r8, 6
	mov r10, 1

LB0:
	cmp r8, 0
	je LE0
	dec r8

	xor rax, rax
	mov r9, qword ptr [r15 + BigInteger._size]

	mov rsi, qword ptr [r15 + BigInteger._value]
	mov rdi, rdx
	
LB1:
	cmp r9, 0
	je LE1
	dec r9

	mov r11, qword ptr [rsi]
	shr rax, 1
	rcl r11, 1
	cmovc rax, r10

	mov qword ptr [rsi], r11
	add rsi, 8
	
	jmp LB1
LE1:
	mov r9, qword ptr [r15 + BigInteger._size]

LB2:
	cmp r9, 0
	je LE2
	dec r9

	mov r11, qword ptr [rdi]
	shr rax, 1
	rcl r11, 1
	cmovc rax, r10

	mov cl, 64
LB3:
	cmp cl, 0
	je LE3
	sub cl, 4

	cmp r8, 0
	je LE3
	mov r12, 1111b
	mov r13, r12
	shl r12, cl
	shl r13, cl
	and r12, r11
	shr r12, cl
	cmp r12, 5
	jl CND0
	add r12, 3
	shl r12, cl
	not r13
	and r11, r13
	or r11, r12
CND0:
	jmp LB3
LE3:
	mov qword ptr [rdi], r11
	add rdi, 8

	jmp LB2
LE2:
	
	jmp LB0
LE0:

	pop r15
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rcx
	pop rdi
	pop rsi

	ret
ASM_BigInteger_bin_to_bcd endp

end

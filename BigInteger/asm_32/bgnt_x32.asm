.model FLAT, C

include bgnt_x32.inc

.data

.code

asm_bgnt_set_size proc
	;Prototype: void asm_bgnt_set_size(BigInteger* bg, __int32 len)

	push ebp
	mov ebp, esp
	mov ecx, [ebp + 8]
	mov edx, [ebp + 12]
	
	xor eax, eax
	mov ebx, 08h

	test edx, 07h
	cmovnz eax, ebx
	
	and edx, 0fffffff8h
	add edx, eax
	shr edx, 1
	mov dword ptr[ecx + BigInteger._size], edx

	pop ebp
	ret
asm_bgnt_set_size endp

end
	.text

/* Switch from current_thread to next_thread. Make next_thread
 * the current_thread, and set next_thread to 0.
 * Use eax as a temporary register; it is caller saved.
 */
	.globl thread_switch
thread_switch:
	/* YOUR CODE HERE */
	// C语言函数调用会压入 ip
	pushal
	// eax 指向 sp
	movl current_thread, %eax
	// save sp
	movl %esp, (%eax)

	
	movl next_thread, %eax
	movl %eax, current_thread
	// restore sp
	movl (%eax), %esp
	
	popal

	movl $0x0, next_thread

	// popal 后esp指向return address
	ret				/* pop return address from stack */

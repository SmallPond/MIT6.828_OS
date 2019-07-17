#include <inc/mmu.h>
#include <inc/x86.h>
#include <inc/assert.h>

#include <kern/pmap.h>
#include <kern/trap.h>
#include <kern/console.h>
#include <kern/monitor.h>
#include <kern/env.h>
#include <kern/syscall.h>
#include <kern/sched.h>
#include <kern/kclock.h>
#include <kern/picirq.h>
#include <kern/cpu.h>
#include <kern/spinlock.h>
#include <kern/time.h>

static struct Taskstate ts;

/* For debugging, so print_trapframe can distinguish between printing
 * a saved trapframe and printing the current trapframe and print some
 * additional information in the latter case.
 */
static struct Trapframe *last_tf;

/* Interrupt descriptor table.  (Must be built at run time because
 * shifted function addresses can't be represented in relocation records.)
 */
struct Gatedesc idt[256] = { { 0 } };
struct Pseudodesc idt_pd = {
	sizeof(idt) - 1, (uint32_t) idt
};


static const char *trapname(int trapno)
{
	static const char * const excnames[] = {
		"Divide error",
		"Debug",
		"Non-Maskable Interrupt",
		"Breakpoint",
		"Overflow",
		"BOUND Range Exceeded",
		"Invalid Opcode",
		"Device Not Available",
		"Double Fault",
		"Coprocessor Segment Overrun",
		"Invalid TSS",
		"Segment Not Present",
		"Stack Fault",
		"General Protection",
		"Page Fault",
		"(unknown trap)",
		"x87 FPU Floating-Point Error",
		"Alignment Check",
		"Machine-Check",
		"SIMD Floating-Point Exception"
	};

	if (trapno < ARRAY_SIZE(excnames))
		return excnames[trapno];
	if (trapno == T_SYSCALL)
		return "System call";
	if (trapno >= IRQ_OFFSET && trapno < IRQ_OFFSET + 16)
		return "Hardware Interrupt";
	return "(unknown trap)";
}

// You will also need to modify trap_init() to initialize the idt to 
// point to each of these entry points defined in trapentry.S;
// the SETGATE macro will be helpful here
void
trap_init(void)
{

	extern struct Segdesc gdt[];
	void divide_handler();
	void debug_handler();
	void nmi_handler();
	void brkpt_handler();
	void oflow_handler();
	void bound_handler();
	void device_handler();
	void illop_handler();
	void tss_handler();
	void segnp_handler();
	void stack_handler();
	void gpflt_handler();
	void pgflt_handler();
	void fperr_handler();
	void align_handler();
	void mchk_handler();
	void simderr_handler();
	void syscall_handler();
	void dblflt_handler();
	void timer_handler();
	void kbd_handler();
	void serial_handler();
	void spurious_handler();
	void ide_handler();
	void error_handler();
	

	// LAB 3: Your code here.
	// GD_KT 全局描述符， kernel text
	SETGATE(idt[T_DIVIDE], 0, GD_KT, divide_handler, 0);
	SETGATE(idt[T_DEBUG],  0, GD_KT, debug_handler,  0);
	SETGATE(idt[T_NMI],    0, GD_KT, nmi_handler,    0);
	SETGATE(idt[T_BRKPT],  0, GD_KT, brkpt_handler,  3);
	SETGATE(idt[T_OFLOW],  0, GD_KT, oflow_handler,  0);
	SETGATE(idt[T_BOUND],  0, GD_KT, bound_handler,  0);
	SETGATE(idt[T_DEVICE], 0, GD_KT, device_handler, 0);
	SETGATE(idt[T_ILLOP],  0, GD_KT, illop_handler,  0);
	SETGATE(idt[T_DBLFLT], 0, GD_KT, dblflt_handler, 0);
	SETGATE(idt[T_TSS],    0, GD_KT, tss_handler,    0);
    SETGATE(idt[T_SEGNP],  0, GD_KT, segnp_handler,  0);
    SETGATE(idt[T_STACK],  0, GD_KT, stack_handler,  0);
    SETGATE(idt[T_GPFLT],  0, GD_KT, gpflt_handler,  0);
    SETGATE(idt[T_PGFLT],  0, GD_KT, pgflt_handler,  0);
    SETGATE(idt[T_FPERR],  0, GD_KT, fperr_handler,  0);
    SETGATE(idt[T_ALIGN],  0, GD_KT, align_handler,  0);
    SETGATE(idt[T_MCHK],   0, GD_KT, mchk_handler,   0);
    SETGATE(idt[T_SIMDERR], 0, GD_KT, simderr_handler, 0);
    SETGATE(idt[T_SYSCALL], 0, GD_KT, syscall_handler, 3);
	// IRQ
	SETGATE(idt[IRQ_OFFSET + IRQ_TIMER],    0, GD_KT, timer_handler, 0);
	SETGATE(idt[IRQ_OFFSET + IRQ_KBD],      0, GD_KT, kbd_handler,     0);
	SETGATE(idt[IRQ_OFFSET + IRQ_SERIAL],   0, GD_KT, serial_handler,  0);
	SETGATE(idt[IRQ_OFFSET + IRQ_SPURIOUS], 0, GD_KT, spurious_handler, 0);
	SETGATE(idt[IRQ_OFFSET + IRQ_IDE],      0, GD_KT, ide_handler,     0);
	SETGATE(idt[IRQ_OFFSET + IRQ_ERROR],    0, GD_KT, error_handler,   0);

	// Per-CPU setup 
	trap_init_percpu();
}

// Initialize and load the per-CPU TSS and IDT
void
trap_init_percpu(void)
{
	// The example code here sets up the Task State Segment (TSS) and
	// the TSS descriptor for CPU 0. But it is incorrect if we are
	// running on other CPUs because each CPU has its own kernel stack.
	// Fix the code so that it works for all CPUs.
	//
	// Hints:
	//   - The macro "thiscpu" always refers to the current CPU's
	//     struct CpuInfo;
	//   - The ID of the current CPU is given by cpunum() or
	//     thiscpu->cpu_id;
	//   - Use "thiscpu->cpu_ts" as the TSS for the current CPU,
	//     rather than the global "ts" variable;
	//   - Use gdt[(GD_TSS0 >> 3) + i] for CPU i's TSS descriptor;
	//   - You mapped the per-CPU kernel stacks in mem_init_mp()
	//   - Initialize cpu_ts.ts_iomb to prevent unauthorized environments
	//     from doing IO (0 is not the correct value!)
	//
	// ltr sets a 'busy' flag in the TSS selector, so if you
	// accidentally load the same TSS on more than one CPU, you'll
	// get a triple fault.  If you set up an individual CPU's TSS
	// wrong, you may not get a fault until you try to return from
	// user space on that CPU.
	//
	// LAB 4: Your code here:
	// 初始化 cpu 的栈段
	thiscpu->cpu_ts.ts_esp0 = KSTACKTOP - cpunum() * (KSTKGAP + KSTKSIZE);
    thiscpu->cpu_ts.ts_ss0 = GD_KD;
	thiscpu->cpu_ts.ts_iomb = sizeof(struct Taskstate);
    // Initialize the TSS slot of the gdt.
    // TSS Task State Segment
    gdt[(GD_TSS0 >> 3) + cpunum()] = SEG16(STS_T32A, (uint32_t) (&thiscpu->cpu_ts), sizeof(struct Taskstate) - 1, 0);
    gdt[(GD_TSS0 >> 3) + cpunum()].sd_s = 0;

    // Load the TSS selector (like other segment selectors, the
    // bottom three bits are special; we leave them 0)
    ltr(GD_TSS0 + sizeof(struct Segdesc) * cpunum());

    // Load the IDT
    lidt(&idt_pd);
}

void
print_trapframe(struct Trapframe *tf)
{
	cprintf("TRAP frame at %p from CPU %d\n", tf, cpunum());
	print_regs(&tf->tf_regs);
	cprintf("  es   0x----%04x\n", tf->tf_es);
	cprintf("  ds   0x----%04x\n", tf->tf_ds);
	cprintf("  trap 0x%08x %s\n", tf->tf_trapno, trapname(tf->tf_trapno));
	// If this trap was a page fault that just happened
	// (so %cr2 is meaningful), print the faulting linear address.
	if (tf == last_tf && tf->tf_trapno == T_PGFLT)
		cprintf("  cr2  0x%08x\n", rcr2());
	cprintf("  err  0x%08x", tf->tf_err);
	// For page faults, print decoded fault error code:
	// U/K=fault occurred in user/kernel mode
	// W/R=a write/read caused the fault
	// PR=a protection violation caused the fault (NP=page not present).
	if (tf->tf_trapno == T_PGFLT)
		cprintf(" [%s, %s, %s]\n",
			tf->tf_err & 4 ? "user" : "kernel",
			tf->tf_err & 2 ? "write" : "read",
			tf->tf_err & 1 ? "protection" : "not-present");
	else
		cprintf("\n");
	cprintf("  eip  0x%08x\n", tf->tf_eip);
	cprintf("  cs   0x----%04x\n", tf->tf_cs);
	cprintf("  flag 0x%08x\n", tf->tf_eflags);
	if ((tf->tf_cs & 3) != 0) {
		cprintf("  esp  0x%08x\n", tf->tf_esp);
		cprintf("  ss   0x----%04x\n", tf->tf_ss);
	}
}

void
print_regs(struct PushRegs *regs)
{
	cprintf("  edi  0x%08x\n", regs->reg_edi);
	cprintf("  esi  0x%08x\n", regs->reg_esi);
	cprintf("  ebp  0x%08x\n", regs->reg_ebp);
	cprintf("  oesp 0x%08x\n", regs->reg_oesp);
	cprintf("  ebx  0x%08x\n", regs->reg_ebx);
	cprintf("  edx  0x%08x\n", regs->reg_edx);
	cprintf("  ecx  0x%08x\n", regs->reg_ecx);
	cprintf("  eax  0x%08x\n", regs->reg_eax);
}

static void
trap_dispatch(struct Trapframe *tf)
{
	switch(tf->tf_trapno) {
		case T_PGFLT:
			page_fault_handler(tf);
			break;
		case T_BRKPT:
			monitor(tf);
			break;
		case T_SYSCALL:
			
			tf->tf_regs.reg_eax = syscall(tf->tf_regs.reg_eax, 
											tf->tf_regs.reg_edx, 
											tf->tf_regs.reg_ecx, 
											tf->tf_regs.reg_ebx, 
											tf->tf_regs.reg_edi, 
											tf->tf_regs.reg_esi);
			break;
		// Handle clock interrupts. Don't forget to acknowledge the
		// interrupt using lapic_eoi() before calling the scheduler!
		// LAB 4: Your code here.
		case (IRQ_OFFSET + IRQ_TIMER):
			// 回应8259A 接收中断。
			lapic_eoi();
			time_tick();
			sched_yield();
			break;
			
		case (IRQ_OFFSET + IRQ_KBD):
			//lapic_eoi();
			kbd_intr();
			break;
		case (IRQ_OFFSET + IRQ_SERIAL):
			// lapic_eoi();
			serial_intr();
			break;
			
		default: 
			// Unexpected trap: The user process or the kernel has a bug.
			print_trapframe(tf);
			if (tf->tf_cs == GD_KT)
				panic("unhandled trap in kernel");
			else {
				env_destroy(curenv);
				return;
			}
			break;
	}
	
}

void
trap(struct Trapframe *tf)
{
	// The environment may have set DF and some versions
	// of GCC rely on DF being clear
	asm volatile("cld" ::: "cc");

	// Halt the CPU if some other CPU has called panic()
	extern char *panicstr;
	if (panicstr)
		asm volatile("hlt");

	// Re-acqurie the big kernel lock if we were halted in
	// sched_yield()
	if (xchg(&thiscpu->cpu_status, CPU_STARTED) == CPU_HALTED)
		lock_kernel();
	// Check that interrupts are disabled.  If this assertion
	// fails, DO NOT be tempted to fix it by inserting a "cli" in
	// the interrupt path.
	assert(!(read_eflags() & FL_IF));

	if ((tf->tf_cs & 3) == 3) {
		// Trapped from user mode.
		// Acquire the big kernel lock before doing any
		// serious kernel work.
		// LAB 4: Your code here.
		lock_kernel();
		assert(curenv);
		
		// Garbage collect if current enviroment is a zombie
		if (curenv->env_status == ENV_DYING) {
			env_free(curenv);
			curenv = NULL;
			sched_yield();
		}

		// Copy trap frame (which is currently on the stack)
		// into 'curenv->env_tf', so that running the environment
		// will restart at the trap point.
		curenv->env_tf = *tf;
		// The trapframe on the stack should be ignored from here on.
		tf = &curenv->env_tf;
	}

	// Record that tf is the last real trapframe so
	// print_trapframe can print some additional information.
	last_tf = tf;

	// Dispatch based on what type of trap occurred
	trap_dispatch(tf);

	// If we made it to this point, then no other environment was
	// scheduled, so we should return to the current environment
	// if doing so makes sense.
	if (curenv && curenv->env_status == ENV_RUNNING)
		env_run(curenv);
	else
		sched_yield();
}


void
page_fault_handler(struct Trapframe *tf)
{
	uint32_t fault_va;

	// Read processor's CR2 register to find the faulting address
	fault_va = rcr2();

	// Handle kernel-mode page faults.

	// LAB 3: Your code here.

	// 怎么判断是内核模式， CPL位

	if(tf->tf_cs && 3 == 0) {
        panic("page_fault in kernel mode, fault address %d\n", fault_va);
    }
	
	// We've already handled kernel-mode exceptions, so if we get here,
	// the page fault happened in user mode.


	// Call the environment's page fault upcall, if one exists.  Set up a
	// page fault stack frame on the user exception stack (below
	// UXSTACKTOP), then branch to curenv->env_pgfault_upcall.
	//
	// The page fault upcall might cause another page fault, in which case
	// we branch to the page fault upcall recursively, pushing another
	// page fault stack frame on top of the user exception stack.
	//
	// It is convenient for our code which returns from a page fault
	// (lib/pfentry.S) to have one word of scratch space at the top of the
	// trap-time stack; it allows us to more easily restore the eip/esp. In
	// the non-recursive case, we don't have to worry about this because
	// the top of the regular user stack is free.  In the recursive case,
	// this means we have to leave an extra word between the current top of
	// the exception stack and the new stack frame because the exception
	// stack _is_ the trap-time stack.
	//
	// If there's no page fault upcall, the environment didn't allocate a
	// page for its exception stack or can't write to it, or the exception
	// stack overflows, then destroy the environment that caused the fault.
	// Note that the grade script assumes you will first check for the page
	// fault upcall and print the "user fault va" message below if there is
	// none.  The remaining three checks can be combined into a single test.
	//
	// Hints:
	//   user_mem_assert() and env_run() are useful here.
	//   To change what the user environment runs, modify 'curenv->env_tf'
	//   (the 'tf' variable points at 'curenv->env_tf').

	// LAB 4: Your code here.
	struct UTrapframe *utf;
	// cprintf("I'M in page_fault_handler [%08x] user fault va %08x \n",curenv->env_id, fault_va);
	if (curenv->env_pgfault_upcall) {
		
		if (tf->tf_esp >= UXSTACKTOP-PGSIZE && tf->tf_esp < UXSTACKTOP) {
			// 异常模式下陷入
			utf = (struct UTrapframe *)(tf->tf_esp - sizeof(struct UTrapframe) - 4);

		}
		else {
			// 非异常模式下陷入
			utf = (struct UTrapframe *)(UXSTACKTOP - sizeof(struct UTrapframe));	
		}
		// 检查异常栈是否溢出
		user_mem_assert(curenv, (const void *) utf, sizeof(struct UTrapframe), PTE_P|PTE_W);
			
		utf->utf_fault_va = fault_va;
		utf->utf_err      = tf->tf_trapno;
		utf->utf_regs     = tf->tf_regs;
		utf->utf_eflags   = tf->tf_eflags;
		// 保存陷入时现场，用于返回
		utf->utf_eip      = tf->tf_eip;
		utf->utf_esp      = tf->tf_esp;
		// 再次转向执行
		curenv->env_tf.tf_eip        = (uint32_t) curenv->env_pgfault_upcall;
		// 异常栈
		curenv->env_tf.tf_esp        = (uint32_t) utf;
		env_run(curenv);
	}
	else {
		// Destroy the environment that caused the fault.
		cprintf("[%08x] user fault va %08x ip %08x\n",
			curenv->env_id, fault_va, tf->tf_eip);
		print_trapframe(tf);
		env_destroy(curenv);
	}
}


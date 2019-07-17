#include <kern/e1000.h>
#include <kern/pmap.h>
#include <kern/pci.h>
#include <kern/pcireg.h>
#include <inc/string.h>


// LAB 6: Your driver code here
#define E1000_LOCATE(offset)  (offset >> 2)
volatile uint32_t *e1000;

/* 为描述符列表分配静态内存 */
struct E1000TxDesc tx_desc_list[TX_DESC_SIZE] __attribute__((aligned (PGSIZE))) ;
char pbuf[TX_DESC_SIZE][TX_PACKET_SIZE] __attribute__((aligned (PGSIZE))) ;

void
e1000_transmit_init() 
{
	size_t i;
	memset(tx_desc_list, 0 , sizeof(struct E1000TxDesc) * TX_DESC_SIZE);
	for (i = 0; i < TX_DESC_SIZE; i++) {
		tx_desc_list[i].buffer_addr = PADDR(pbuf[i]);
		tx_desc_list[i].status = E1000_TXD_STAT_DD;
		tx_desc_list[i].cmd    =  E1000_TXD_CMD_RS | E1000_TXD_CMD_EOP;
		
	}

	e1000[E1000_LOCATE(E1000_TDBAL)] = PADDR(tx_desc_list);
	e1000[E1000_LOCATE(E1000_TDBAH)] = 0;
	e1000[E1000_LOCATE(E1000_TDLEN)] = sizeof(struct E1000TxDesc) * TX_DESC_SIZE;
	// ensure that TDH and TDT are 0 index not offset
	e1000[E1000_LOCATE(E1000_TDH)] = 0;
	e1000[E1000_LOCATE(E1000_TDT)] = 0;

	// Initialize the Transmit Control Register (TCTL)
	e1000[E1000_LOCATE(E1000_TCTL)] = E1000_TCTL_EN | 
									  E1000_TCTL_PSP |
									  (E1000_TCTL_CT & (0x10 << 4)) |
									  (E1000_TCTL_COLD & (0x40 << 12));

	// 10 8 6 
	// 10 8 12
	e1000[E1000_LOCATE(E1000_TIPG)] = 10 | (8 << 10) | (12 << 20);
}

int 
e1000_transmit(void *addr, size_t len)
{
	size_t tdt = e1000[E1000_LOCATE(E1000_TDT)];
	struct E1000TxDesc *tail_desc = &tx_desc_list[tdt];

	
	if ( !(tail_desc->status & E1000_TXD_STAT_DD )) {
		// Status is not DD
		return -1;
	}
	memmove(pbuf[tdt], addr, len);
	
	tail_desc->length = (uint16_t )len;
	// clear DD 
	tail_desc->status &= (~E1000_TXD_STAT_DD);

	e1000[E1000_LOCATE(E1000_TDT)] = (tdt+1) % TX_DESC_SIZE;
	
	return 0;
	
}

int
pci_e1000_attach(struct pci_func * pcif) 
{
	int r;
	// 使能E1000，分配 MMIO等
	pci_func_enable(pcif);
	// 映射，并保存其虚拟地址，方便访问。
	e1000 = mmio_map_region(pcif->reg_base[0], pcif->reg_size[0]);
	e1000_transmit_init();
	
	cprintf("device status:[%08x]\n", e1000[E1000_LOCATE(E1000_DEVICE_STATUS)]);
	/*
	char mes[40] = "e1000 transmit :HELLO";
	for (size_t i = 0; i < 10 ; i++) {
		e1000_transmit(mes, strlen(mes));
	}
	*/
	return 0;
}


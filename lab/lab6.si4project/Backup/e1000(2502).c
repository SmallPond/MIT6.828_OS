#include <kern/e1000.h>
#include <kern/pmap.h>
#include <kern/pci.h>
#include <kern/pcireg.h>
#include <inc/string.h>


// LAB 6: Your driver code here
#define E1000_LOCATE(offset)  (offset >> 2)
volatile uint32_t *e1000;

/* 为描述符列表分配静态内存 */
struct E1000TxDesc tx_desc_list[TX_DESC_SIZE];
struct Packet pack_buf[TX_DESC_SIZE];


void
e1000_transmit_init() 
{
	size_t i;
	memset(tx_desc_list, 0 , sizeof(struct E1000TxDesc) * TX_DESC_SIZE);
	for (i = 0; i < TX_DESC_SIZE; i++) {
		tx_desc_list[i].buffer_addr = PADDR(&pack_buf[i]);
		tx_desc_list[i].status |= E1000_TXD_STAT_DD;
		tx_desc_list[i].cmd    |=  E1000_TXD_CMD_RS;
		
	}

	e1000[E1000_LOCATE(E1000_TDBAL)] = PADDR(tx_desc_list);
	e1000[E1000_LOCATE(E1000_TDBAH)] = 0;
	e1000[E1000_LOCATE(E1000_TDLEN)] = sizeof(struct E1000TxDesc) * TX_DESC_SIZE;
	// ensure that TDH and TDT are 0
	e1000[E1000_LOCATE(E1000_TDH)] = 0;
	e1000[E1000_LOCATE(E1000_TDT)] = 0;

	// Initialize the Transmit Control Register (TCTL)
	e1000[E1000_LOCATE(E1000_TCTL)] |= E1000_TCTL_EN;
	e1000[E1000_LOCATE(E1000_TCTL)] |= E1000_TCTL_PSP;
	e1000[E1000_LOCATE(E1000_TCTL)] |= (E1000_TCTL_CT & (0x10 << 4));
	e1000[E1000_LOCATE(E1000_TCTL)] |= (E1000_TCTL_CT & (0x40 << 12));

	e1000[E1000_LOCATE(E1000_TIPG)] = 10 | (8 << 10) | (6 << 20);
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
	
	return 0;
}


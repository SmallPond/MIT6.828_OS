#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#include <inc/types.h>

#define PCI_E1000_VENDER_ID 0x8086
#define PCI_E1000_DEVICE_ID 0x100E

/* 循环队列的长度*/
#define TX_DESC_SIZE     32
#define TX_PACKET_SIZE   2048

#define RX_DESC_SIZE     128
#define RX_PACKET_SIZE   2048

/* Register Set
 * 
 * RW - register is both readable and writable
 * 
 */

#define E1000_DEVICE_STATUS   0x00008  /* Device Status - RO */

#define E1000_ICS      0x000C8  /* Interrupt Cause Set - WO */
#define E1000_IMS      0x000D0  /* Interrupt Mask Set - RW */

#define E1000_RCTL     0x00100  /* RX Control - RW */
#define E1000_TCTL     0x00400  /* TX Control - RW */



#define E1000_RDBAL    0x02800  /* RX Descriptor Base Address Low - RW */
#define E1000_RDBAH    0x02804  /* RX Descriptor Base Address High - RW */
#define E1000_RDLEN    0x02808  /* RX Descriptor Length - RW */
#define E1000_RDH      0x02810  /* RX Descriptor Head - RW */
#define E1000_RDT      0x02818  /* RX Descriptor Tail - RW */
#define E1000_RA       0x05400  /* Receive Address - RW Array */



#define E1000_TDBAL    0x03800  /* TX Descriptor Base Address Low - RW */
#define E1000_TDBAH    0X03804  /* TX Descriptor Base Address High - RW */
#define E1000_TDLEN    0x03808  /* TX Descriptor Length - RW */

#define E1000_TDH      0x03810  /* TX Descriptor Head - RW */
#define E1000_TDT      0x03818  /* TX Descripotr Tail - RW */

#define E1000_TIPG     0x00410  /* TX Inter-packet gap -RW */

/* Transmit Control */
#define E1000_TCTL_RST    0x00000001    /* Reserved */
#define E1000_TCTL_EN     0x00000002    /* enable tx */
#define E1000_TCTL_BCE    0x00000004    /* Reserved */
#define E1000_TCTL_PSP    0x00000008    /* pad short packets */
#define E1000_TCTL_CT     0x00000ff0    /* collision threshold */
#define E1000_TCTL_COLD   0x003ff000    /* collision distance */
#define E1000_TCTL_SWXOFF 0x00400000    /* SW Xoff transmission */
#define E1000_TCTL_PBE    0x00800000    /* Reserved */
#define E1000_TCTL_RTLC   0x01000000    /* Re-transmit on late collision */
#define E1000_TCTL_NRTU   0x02000000    /* No Re-transmit on underrun */
#define E1000_TCTL_MULR   0x10000000    /* Reserved */


#define E1000_RCTL_EN     0x00000002    /* enable */
#define E1000_RCTL_BAM    0x00008000    /* broadcast enable */
#define E1000_RCTL_SECRC  0x04000000    /* Strip Ethernet CRC */



/* Transmit Descriptor */
struct E1000TxDesc {
    uint64_t buffer_addr;       /* Address of the descriptor's data buffer */

	uint16_t length;    /* Data buffer length */
    uint8_t cso;        /* Checksum offset */
    uint8_t cmd;        /* Descriptor control */

    uint8_t status;     /* Descriptor status */
    uint8_t css;        /* Checksum start */
    uint16_t special;

}__attribute__((packed));



/* Transmit Descriptor bit definitions */
#define E1000_TXD_DTYP_D     0x00100000 /* Data Descriptor */
#define E1000_TXD_DTYP_C     0x00000000 /* Context Descriptor */


#define E1000_TXD_CMD_EOP    0x01 /* End of Packet */
#define E1000_TXD_CMD_RS     0x08 /* Report Status */

#define E1000_TXD_STAT_DD    0x00000001 /* Descriptor Done */
#define E1000_TXD_STAT_EC    0x00000002 /* Excess Collisions */
#define E1000_TXD_STAT_LC    0x00000004 /* Late Collisions */
#define E1000_TXD_STAT_TU    0x00000008 /* Transmit underrun */
#define E1000_TXD_STAT_TC    0x00000004 /* Tx Underrun */


/* Receive Descriptor */
struct E1000RxDesc {
	uint64_t buffer_addr;
	uint16_t length;             /* Data buffer length */
	uint16_t chksum;             /* Check Sum */
	uint8_t  status;
	uint8_t  err;
	uint16_t special;
};

/* Transmit Descriptor bit definitions */


#define E1000_RAH_AV            0x80000000        /* Receive descriptor valid */
#define E1000_RXD_STAT_DD       0x01    /* Descriptor Done */
#define E1000_RXD_STAT_EOP      0x02    /* End of Packet */

/* these buffer sizes are valid if E1000_RCTL_BSEX is 0 */
#define E1000_RCTL_SZ_2048        0x00000000    /* rx buffer size 2048 */
#define E1000_RCTL_SZ_1024        0x00010000    /* rx buffer size 1024 */
#define E1000_RCTL_SZ_512         0x00020000    /* rx buffer size 512 */
#define E1000_RCTL_SZ_256         0x00030000    /* rx buffer size 256 */
/* these buffer sizes are valid if E1000_RCTL_BSEX is 1 */
#define E1000_RCTL_SZ_16384       0x00010000    /* rx buffer size 16384 */
#define E1000_RCTL_SZ_8192        0x00020000    /* rx buffer size 8192 */
#define E1000_RCTL_SZ_4096        0x00030000    /* rx buffer size 4096 */


int e1000_transmit(void *addr, size_t len);
int e1000_receive(void *buf, size_t *len);


#endif  // SOL >= 6



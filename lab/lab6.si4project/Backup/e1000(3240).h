#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#include <inc/types.h>

#define PCI_E1000_VENDOR_ID 0x8086
#define PCI_E1000_DEVICE_ID 0x100E

/* 循环队列的长度*/
#define TX_DESC_SIZE  32
/* Register Set
 * 
 * RW - register is both readable and writable
 * 
 */

#define E1000_DEVICE_STATUS   0x00008  /* Device Status - RO */

#define E1000_RCTL     0x00100  /* RX Control - RW */
#define E1000_TCTL     0x00400  /* TX Control - RW */
#define E1000_TIPG     0x00410  /* TX Inter-packet gap -RW */

#define E1000_TDBAL    0x03800  /* TX Descriptor Base Address Low - RW */
#define E1000_TDBAH    0X03804  /* TX Descriptor Base Address High - RW */
#define E1000_TDLEN    0x03808  /* TX Descriptor Length - RW */

#define E1000_TDH      0x03810  /* TX Descriptor Head - RW */
#define E1000_TDT      0x03818  /* TX Descripotr Tail - RW */


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
#define E1000_TXD_POPTS_IXSM 0x01       /* Insert IP checksum */
#define E1000_TXD_POPTS_TXSM 0x02       /* Insert TCP/UDP checksum */
#define E1000_TXD_CMD_EOP    0x01000000 /* End of Packet */
#define E1000_TXD_CMD_IFCS   0x02000000 /* Insert FCS (Ethernet CRC) */
#define E1000_TXD_CMD_IC     0x04000000 /* Insert Checksum */
#define E1000_TXD_CMD_RS     0x08000000 /* Report Status */
#define E1000_TXD_CMD_RPS    0x10000000 /* Report Packet Sent */
#define E1000_TXD_CMD_DEXT   0x20000000 /* Descriptor extension (0 = legacy) */
#define E1000_TXD_CMD_VLE    0x40000000 /* Add VLAN tag */
#define E1000_TXD_CMD_IDE    0x80000000 /* Enable Tidv register */
#define E1000_TXD_STAT_DD    0x00000001 /* Descriptor Done */
#define E1000_TXD_STAT_EC    0x00000002 /* Excess Collisions */
#define E1000_TXD_STAT_LC    0x00000004 /* Late Collisions */
#define E1000_TXD_STAT_TU    0x00000008 /* Transmit underrun */
#define E1000_TXD_CMD_TCP    0x01000000 /* TCP packet */
#define E1000_TXD_CMD_IP     0x02000000 /* IP packet */
#define E1000_TXD_CMD_TSE    0x04000000 /* TCP Seg enable */
#define E1000_TXD_STAT_TC    0x00000004 /* Tx Underrun */

/* packet buffer*/
struct Packet {
	char body[2048];
};



#endif  // SOL >= 6



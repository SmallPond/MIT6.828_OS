#include "ns.h"

extern union Nsipc nsipcbuf;

void
output(envid_t ns_envid)
{
	binaryname = "ns_output";

	// LAB 6: Your code here:
	// 	- read a packet from the network server
	//	- send the packet to the device driver
	uint32_t whom;
	int perm;
	int32_t req;

	while (1) {
		req = ipc_recv((envid_t *)&whom, &nsipcbuf, &perm);
		if (req != NSREQ_OUTPUT) {
			continue;
		}

		while (sys_pkt_try_send(nsipcbuf.pkt.jp_data, nsipcbuf.pkt.jp_len) < 0) {
			sys_yield();
		}
	}
}

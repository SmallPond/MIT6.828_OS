#include "ns.h"

extern union Nsipc nsipcbuf;

void
output(envid_t ns_envid)
{
	binaryname = "ns_output";
	envid_t envid;
	int perm;
	int32_t req;
	// LAB 6: Your code here:
	// 	- read a packet from the network server
	while(1) {
		req = ipc_recv(&envid, (void *) &nsipcbuf, &perm);
		if (req !=  NSREQ_OUTPUT)
			continue;
		
		struct jif_pkt *pkt = &(nsipcbuf.pkt);
		while (sys_pkt_try_send(pkt->jp_data, pkt->jp_len) < 0) {
			sys_yield();

		}
	}
	
	
}

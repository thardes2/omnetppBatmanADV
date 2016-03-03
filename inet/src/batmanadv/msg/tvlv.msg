cplusplus {{
#include "ManetAddress.h"
#include "omnetpp.h"
#include "specialFlags.h"
#include <vector>

/*#define BIT0 0000000000000000000000
#define BIT1 0000000000000000000001
#define BIT2 0000000000000000000010
#define BIT3 0000000000000000000011
#define BIT4 0000000000000000000100
#define BIT5 0000000000000000000101
#define BIT6 0000000000000000000110
#define BIT7 0000000000000000000111
#define BIT8 0000000000000000001000
#define BIT9 0000000000000000001001
#define BIT10 0000000000000000001010
#define BIT11 0000000000000000001011
#define BIT12 0000000000000000001100
#define BIT13 0000000000000000001101
#define BIT14 0000000000000000001110
#define BIT15 0000000000000000001111
*/
#define BIT0  0000000000000000000001
#define BIT1  0000000000000000000010
#define BIT2  0000000000000000000100
#define BIT3  0000000000000000001000
#define BIT4  0000000000000000010000
#define BIT5  0000000000000000100000
#define BIT6  0000000000000001000000
#define BIT7  0000000000000010000000
#define BIT8  0000000000000100000000
#define BIT9  0000000000001000000000
#define BIT10 0000000000010000000000
#define BIT11 0000000000100000000000
#define BIT12 0000000001000000000000
#define BIT13 0000000010000000000000
#define BIT14 0000000100000000000000
#define BIT15 0000001000000000000000
//typedef MACAddress *MACAddress;
//#define DIRECTLINK 0x40
//#define UNIDIRECTIONAL 0x80

//#ifdef __USE_MINHOP__
//#define BatADVPacketSize 17
//#else
//#define BatADVPacketSize 16
//#endif
class batadv_tvlv_tt_change;
typedef std::vector<batadv_tvlv_tt_change> tt_changeList;
}}

class noncobject MACAddress;
class noncobject ManetAddress;
class noncobject batadv_tt_client_flags;
class noncobject tt_changeList;

//  batadv_unicast_tvlv - generic unicast packet with tvlv payload
// @packet_type: batman-adv packet type, part of the general header
// @version: batman-adv protocol version, part of the genereal header
// @ttl: time to live for this packet, part of the genereal header
// @reserved: reserved field (for packet alignment)
// @src: address of the source
// @dst: address of the destination
// @tvlv_len: length of tvlv data following the unicast tvlv header
// @align: 2 bytes to align the header to a 4 byte boundry
packet batadv_unicast_tvlv_packet {
	int  packet_type;
	int  version;  //batman version field 
	int  ttl;
	int  reserved;
	MACAddress  dst;
	MACAddress  src;
	long tvlv_len;
	long align;
};



//  batadv_tvlv_gateway_data - gateway data propagated through gw tvlv
//  container
// @bandwidth_down: advertised uplink download bandwidth
// @bandwidth_up: advertised uplink upload bandwidth
//
packet batadv_tvlv_gateway_data {
	double bandwidth_down;
	double bandwidth_up;
};


//batadv_tvlv_tt_data - tt data propagated through the tt tvlv container
// @flags: translation table flags (see batadv_tt_data_flags)
// @ttvn: translation table version number
// @vlan_num: number of announced VLANs. In the TVLV this struct is followed by
//  one batadv_tvlv_tt_vlan_data object per announced vlan
 //
packet batadv_tvlv_tt_data {
	int flags;
	bool fullTable;
	int ttvn;
	long  num_vlan;
	int ttType;
	tt_changeList ttChanges;
};

packet batadv_tvlv_tt_change
{
        int flags;//123:  enum batadv_tt_client_flags - TT client specific flags
        //uint8_t reserved[3];
        //batadv_tt_client_flags tt_clientFlags;
        MACAddress addr;
        int vid;
};

packet batadv_tvlv_roam_adv {
	MACAddress client;
	long vid;
};

// struct batadv_tvlv_mcast_data - payload of a multicast tvlv
// @flags: multicast flags announced by the orig node
// @reserved: reserved field

packet batadv_tvlv_mcast_data {
	int	flags;
	//int reserved[3];
};
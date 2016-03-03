cplusplus {{
#include "ManetAddress.h"
#include "omnetpp.h"
#include "tvlv_m.h"
#include "tt_change.h"
#include <vector>

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

/*
#define BIT0 0000000000000000000000
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
#define BIT15 0000000000000000001111*/
//typedef MACAddress *MACAddress;
//#define DIRECTLINK 0x40
//#define UNIDIRECTIONAL 0x80

//#ifdef __USE_MINHOP__
//#define BatADVPacketSize 17
//#else
//#define BatADVPacketSize 16
//#endif
class batadv_ogm_packet;
typedef std::vector<cPacket *> ogmlist;
typedef std::vector<batadv_ogm_packetClass> ogmlistNoPointer;
}}

class noncobject MACAddress;
class noncobject Intlist;
class noncobject ManetAddress;
class noncobject batadv_tvlv_tt_data;
class noncobject batadv_tvlv_tt_change;
class noncobject batadv_tvlv_gateway_data;
class noncobject ogmlist;
class noncobject ogmlistNoPointer;
// struct batadv_ogm_packet - ogm (routing protocol) packet
// @packet_type: batman-adv packet type, part of the general header
// @version: batman-adv protocol version, part of the genereal header
// @ttl: time to live for this packet, part of the genereal header
// @flags: contains routing relevant flags - see enum batadv_iv_flags
// @tvlv_len: length of tvlv data following the ogm header
//
packet batadv_ogm_packet {
	int  packet_type;
	int  version;
	int  ttl;
	//Intlist bla;
	int  flags;
	long   seqno;
	ManetAddress  orig;
	ManetAddress  prev_sender;
	int  reserved;
	int  tq;
	long  tvlv_len;
	int meshID;
	batadv_tvlv_tt_data tt_data;
	batadv_tvlv_tt_change tt_change;
	batadv_tvlv_gateway_data gateway_data;
};
packet batadv_ogm_packet_raw {
	unsigned int  packet_type;
	unsigned int  version;
	unsigned int  ttl;
	unsigned int  flags;
	long   seqno;
	unsigned int  orig[6];
	unsigned int prev_sender[6];
	unsigned int  reserved;
	unsigned int  tq;
	long   tvlv_len;
};

//Packet to transmitt aggregates OGMS --> The real code doesn't need a thing
//like a "cPacket"
packet batadv_ogm_packet_agg {
	ogmlist ogms;
	ogmlistNoPointer ogmsNoPointer;
};
message vis_ADV_data
{
    int type;
    int data;
};

// enum batadv_iv_flags - flags used in B.A.T.M.A.N. IV OGM packets
// @BATADV_NOT_BEST_NEXT_HOP: flag is set when ogm packet is forwarded and was
//     previously received from someone else than the best neighbor.
// @BATADV_PRIMARIES_FIRST_HOP: flag is set when the primary interface address
//     is used, and the packet travels its first hop.
// @BATADV_DIRECTLINK: flag is for the first hop or if rebroadcasted from a
//     one hop neighbor on the interface where it was originally received.

enum batadv_iv_flags {
        BATADV_NOT_BEST_NEXT_HOP   = BIT0;
        BATADV_PRIMARIES_FIRST_HOP = BIT1;
        BATADV_DIRECTLINK          = BIT2;
};

// enum batadv_packettype - types for batman-adv encapsulated packets
// @BATADV_IV_OGM: originator messages for B.A.T.M.A.N. IV
// @BATADV_BCAST: broadcast packets carrying broadcast payload
// @BATADV_CODED: network coded packets
// @BATADV_UNICAST: unicast packets carrying unicast payload traffic
// @BATADV_UNICAST_FRAG: unicast packets carrying a fragment of the original
//     payload packet
// @BATADV_UNICAST_4ADDR: unicast packet including the originator address of
//     the sender
// @BATADV_ICMP: unicast packet like IP ICMP used for ping or traceroute
// @BATADV_UNICAST_TVLV: unicast packet carrying TVLV containers
enum batadv_packettype {
        // 0x00 - 0x3f: local packets or special rules for handling 
        BATADV_IV_OGM           = 0x00;
        BATADV_BCAST            = 0x01;
        BATADV_CODED            = 0x02;
        // 0x40 - 0x7f: unicast 
//#define BATADV_UNICAST_MIN     0x40
        BATADV_UNICAST          = 0x40;
        BATADV_UNICAST_FRAG     = 0x41;
        BATADV_UNICAST_4ADDR    = 0x42;
        BATADV_ICMP             = 0x43;
        BATADV_UNICAST_TVLV     = 0x44;
//#define BATADV_UNICAST_MAX     0x7f
        // 0x80 - 0xff: reserved 
};
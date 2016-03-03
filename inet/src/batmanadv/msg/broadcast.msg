//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
cplusplus {{
#include "ManetAddress.h"
#include "omnetpp.h"
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
}}

class noncobject MACAddress;
class noncobject ManetAddress;

// struct batadv_bcast_packet - broadcast packet for network payload
// @packet_type: batman-adv packet type, part of the general header
// @version: batman-adv protocol version, part of the genereal header
// @ttl: time to live for this packet, part of the genereal header
// @reserved: reserved byte for alignment
// @seqno: sequence identification
// @orig: originator of the broadcast packet
packet batadv_bcast_packet {
	int  packet_type;
	int  version;  // batman version field 
	int  ttl;
	int  reserved;
	long   seqno;
	MACAddress  orig;
	//"4 bytes boundary + 2 bytes" long to make the payload after the
	// following ethernet header again 4 bytes boundary aligned
	//
};
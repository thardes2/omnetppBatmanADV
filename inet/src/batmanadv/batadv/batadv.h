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
#include "ManetAddress.h"
#include "ogm_m.h"
#include "tvlv_m.h"
#include "EtherFrame.h"
#include "specialFlags.h"
#include "list.h"
//#include "tt_change.h"
#ifndef BATADV_H_
#define BATADV_H_
/*#define BIT0  0000000000000000000000
 #define BIT1  0000000000000000000001
 #define BIT2  0000000000000000000010
 #define BIT3  0000000000000000000011
 #define BIT4  0000000000000000000100
 #define BIT5  0000000000000000000101
 #define BIT6  0000000000000000000110
 #define BIT7  0000000000000000000111
 #define BIT8  0000000000000000001000
 #define BIT9  0000000000000000001001
 #define BIT10 0000000000000000001010
 #define BIT11 0000000000000000001011
 #define BIT12 0000000000000000001100
 #define BIT13 0000000000000000001101
 #define BIT14 0000000000000000001110
 #define BIT15 0000000000000000001111*/
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

/* purge originators after time in seconds if no valid packet comes in
 * -> TODO: check influence on BATADV_TQ_LOCAL_WINDOW_SIZE
 */
#define BATADV_PURGE_TIMEOUT            200 /* 200 seconds */

#define BATADV_TT_LOCAL_TIMEOUT         60000//10    /* 600000 in milliseconds */
#define BATADV_TT_CLIENT_ROAM_TIMEOUT   600 /* 600000in milliseconds */
#define BATADV_TT_CLIENT_TEMP_TIMEOUT   600 /* 600000 in milliseconds */
#define BATADV_TT_WORK_PERIOD           5 /* 5 seconds */
#define BATADV_ORIG_WORK_PERIOD         1 /* 1 second */
#define BATADV_DAT_ENTRY_TIMEOUT        (5*60000) /* 5 mins in milliseconds */
#define BATADV_TQ_LOCAL_WINDOW_SIZE     6000//64  //BIT!
#define BATADV_MAX_AGGREGATION_MS       100
/* should not be bigger than 512 bytes or change the size of
 * forw_packet->direct_link_flags
 */
#define BATADV_MAX_AGGREGATION_BYTES    512
#define BATADV_BCAST_QUEUE_LEN          256
#define BATADV_BATMAN_QUEUE_LEN         256

#define BATADV_OGM_HLEN 24//250
#define BATADV_TVLV_HLEN 4
#define BATADV_TVLV_HLEN 4
#define ETH_HLEN 16

/* milliseconds we have to keep pending tt_req */
#define BATADV_TT_REQUEST_TIMEOUT 3000

class batadv_hard_iface;

/* Time To Live of broadcast messages */
#define BATADV_TTL 50
/**
 * BATADV_TT_REMOTE_MASK - bitmask selecting the flags that are sent over the
 *  wire only
 */
#define BATADV_TT_REMOTE_MASK  255// 0x00FF

#define BATADV_GW_THRESHOLD 50
/**
 * enum batadv_dhcp_recipient - dhcp destination
 * @BATADV_DHCP_NO: packet is not a dhcp message
 * @BATADV_DHCP_TO_SERVER: dhcp message is directed to a server
 * @BATADV_DHCP_TO_CLIENT: dhcp message is directed to a client
 */
enum batadv_dhcp_recipient
{
    BATADV_DHCP_NO = 0, BATADV_DHCP_TO_SERVER, BATADV_DHCP_TO_CLIENT,
};
/**
 * enum batadv_tvlv_handler_flags - tvlv handler flags definitions
 * @BATADV_TVLV_HANDLER_OGM_CIFNOTFND: tvlv ogm processing function will call
 *  this handler even if its type was not found (with no data)
 * @BATADV_TVLV_HANDLER_OGM_CALLED: interval tvlv handling flag - the API marks
 *  a handler as being called, so it won't be called if the
 *  BATADV_TVLV_HANDLER_OGM_CIFNOTFND flag was set
 */
enum batadv_tvlv_handler_flags
{
    BATADV_TVLV_HANDLER_OGM_CIFNOTFND = BIT1, BATADV_TVLV_HANDLER_OGM_CALLED = BIT2,
};
/**
 * batadv_forw_mode - the way a packet should be forwarded as
 * @BATADV_FORW_ALL: forward the packet to all nodes (currently via classic
 *  flooding)
 * @BATADV_FORW_SINGLE: forward the packet to a single node (currently via the
 *  BATMAN unicast routing protocol)
 * @BATADV_FORW_NONE: don't forward, drop it
 */
enum batadv_forw_mode
{
    BATADV_FORW_ALL, BATADV_FORW_SINGLE, BATADV_FORW_NONE,
};
enum meshIDs
{
    noClusterMeshID = -1, buerenMeshID = 0, pbMeshID = 1, hoevelhofBaliSchlangenMeshID = 2, remainderMeshID = 3,
};

#define time_before(x, y) ((x) < (y))

/* tt data subtypes */
#define BATADV_TT_DATA_TYPE_MASK 0x0F

/* number of OGMs sent with the last tt diff */
#define BATADV_TT_OGM_APPEND_MAX 3

/* Time in which a client can roam at most ROAMING_MAX_COUNT times in
 * milliseconds
 */
#define BATADV_ROAMING_MAX_TIME 20000
#define BATADV_ROAMING_MAX_COUNT 5

#define MAC_MULTICAST "FF:FF:FF:FF:FF:FF"
#define TQ_GLOBAL_WINDOW_SIZE 5
#define TQ_LOCAL_WINDOW_SIZE 50000//64
#define TQ_LOCAL_BIDRECT_SEND_MIN  1
#define TQ_LOCAL_BIDRECT_RECV_MIN  1
#define TQ_TOTAL_BIDRECT_LIMIT  1
#define TQ_MAX 255
#define TQ_MIN 0
#define BATADV_JITTER 20
/* don't reset again within 30 seconds */
#define BATADV_RESET_PROTECTION_MS 30000
#define BATADV_EXPECTED_SEQNO_RANGE 65536
#define BATADV_COMPAT_VERSION 15
#define BATADV_NO_FLAGS 0

enum forwardReason
{
    FORWARD_OGM = 0, FORWARD_TT_REQUEST = 1, FORWARD_TT_RESPONSE = 2, FORWARD_MY_TT_REQUEST = 3, FORWARD_MY_TT_RESPONSE = 4, FORWARD_MY_OGM = 5, ROUTE_UNICAST = 6, GENERATED_TT_RESPONSE = 7, AGGREGATED = 7, FORWARD_UNICAST = 8
};

enum batadv_dup_status
{
    BATADV_NO_DUP = 0, BATADV_ORIG_DUP = 1, BATADV_NEIGH_DUP = 2, BATADV_PROTECTED = 3, DUP_ERROR = 4
};
enum batadv_window_status
{
    seqnoNew = 0, seqnoOld = 1
};
enum ReturnValuesOGMProcessing
{
    DONE = 0, SUCCESSFUL = 1, DROPPED = 2, CREATENEW = 3
};
enum RebroadcastedFlags
{
    NO = 0, YES = 1
};
/*enum UnicastPacketChecks
 {
 EBADR = 1, EREMOTE = 2, ENODATA = 3
 };*/
enum batadv_tt_data_flags
{
    BATADV_TT_OGM_DIFF = BIT0, BATADV_TT_REQUEST = BIT1, BATADV_TT_RESPONSE = BIT2, BATADV_TT_FULL_TABLE = BIT4
};
enum NET_XMIT
{
    NET_XMIT_DROP = 0, NET_XMIT_SUCCESS = 1, NET_XMIT_POLICED = 2, NET_RX_SUCCESS = 3, NET_RX_DROP = 4
};
/**
 * enum batadv_orig_capabilities - orig node capabilities
 * @BATADV_ORIG_CAPA_HAS_DAT: orig node has distributed arp table enabled
 * @BATADV_ORIG_CAPA_HAS_NC: orig node has network coding enabled
 * @BATADV_ORIG_CAPA_HAS_TT: orig node has tt capability
 * @BATADV_ORIG_CAPA_HAS_MCAST: orig node has some multicast capability
 *  (= orig node announces a tvlv of type BATADV_TVLV_MCAST)
 */
enum batadv_orig_capabilities
{
    BATADV_ORIG_CAPA_HAS_DAT = BIT0, BATADV_ORIG_CAPA_HAS_NC = BIT1, BATADV_ORIG_CAPA_HAS_TT = BIT2, BATADV_ORIG_CAPA_HAS_MCAST = BIT3
};
/**
 * enum batadv_tvlv_type - tvlv type definitions
 * @BATADV_TVLV_GW: gateway tvlv
 * @BATADV_TVLV_DAT: distributed arp table tvlv
 * @BATADV_TVLV_NC: network coding tvlv
 * @BATADV_TVLV_TT: translation table tvlv
 * @BATADV_TVLV_ROAM: roaming advertisement tvlv
 * @BATADV_TVLV_MCAST: multicast capability tvlv
 */
enum batadv_tvlv_type
{
    BATADV_TVLV_GW = 0x01, BATADV_TVLV_DAT = 0x02, BATADV_TVLV_NC = 0x03, BATADV_TVLV_TT = 0x04, BATADV_TVLV_ROAM = 0x05, BATADV_TVLV_MCAST = 0x06
};
enum GatewayModes
{
    defaultMode = 20, fastConnection = 1, stableConnection = 2, fastSwitch = 3
};
enum MeshState
{
    INACTIVE = 0, ACTIVE = 1, DEACTIVATED = 2
};
/*enum GwModes{
 CLIENT = 0, SERVER = 1
 };*/
/*enum GwSelectionClass{
 LATESWITCH=20, FASTCONNECTION = 1, STABLECONNECTION = 2, FASTSWITCHCONNECTION = 3
 };*/
enum batadv_gw_modes
{
    BATADV_GW_MODE_OFF, BATADV_GW_MODE_CLIENT, BATADV_GW_MODE_SERVER,
};
enum batadv_hard_if_state
{
    BATADV_IF_NOT_IN_USE, BATADV_IF_TO_BE_REMOVED, BATADV_IF_INACTIVE, BATADV_IF_ACTIVE, BATADV_IF_TO_BE_ACTIVATED, BATADV_IF_I_WANT_YOU,
};
/**
 * enum batadv_tt_client_flags - TT client specific flags
 * @BATADV_TT_CLIENT_DEL: the client has to be deleted from the table
 * @BATADV_TT_CLIENT_ROAM: the client roamed to/from another node and the new
 *  update telling its new real location has not been received/sent yet
 * @BATADV_TT_CLIENT_WIFI: this client is connected through a wifi interface.
 *  This information is used by the "AP Isolation" feature
 * @BATADV_TT_CLIENT_ISOLA: this client is considered "isolated". This
 *  information is used by the Extended Isolation feature
 * @BATADV_TT_CLIENT_NOPURGE: this client should never be removed from the table
 * @BATADV_TT_CLIENT_NEW: this client has been added to the local table but has
 *  not been announced yet
 * @BATADV_TT_CLIENT_PENDING: this client is marked for removal but it is kept
 *  in the table for one more originator interval for consistency purposes
 * @BATADV_TT_CLIENT_TEMP: this global client has been detected to be part of
 *  the network but no nnode has already announced it
 *
 * Bits from 0 to 7 are called _remote flags_ because they are sent on the wire.
 * Bits from 8 to 15 are called _local flags_ because they are used for local
 * computations only.
 *
 * Bits from 4 to 7 - a subset of remote flags - are ensured to be in sync with
 * the other nodes in the network. To achieve this goal these flags are included
 * in the TT CRC computation.
 */
//enum batadv_tt_client_flags
//{
//    BATADV_TT_CLIENT_DEL = BIT0, BATADV_TT_CLIENT_ROAM = BIT1, BATADV_TT_CLIENT_WIFI = BIT4, BATADV_TT_CLIENT_ISOLA = BIT5, BATADV_TT_CLIENT_NOPURGE = BIT8, BATADV_TT_CLIENT_NEW = BIT9, BATADV_TT_CLIENT_PENDING = BIT10, BATADV_TT_CLIENT_TEMP = BIT11,
//};
struct batadv_ogm_packetOrignal
{
        uint8_t packet_type;
        uint8_t version;
        uint8_t ttl;
        uint8_t flags;
        uint8_t seqno;
        uint8_t orig[6];
        uint8_t prev_sender[6];
        uint8_t reserved;
        uint8_t tq;
        uint8_t tvlv_len;
        /* __packed is not needed as the struct size is divisible by 4,
         * and the largest data type in this struct has a size of 4.
         */
};
struct batadv_unicast_packetOrignal
{
        uint8_t packet_type;
        uint8_t version;
        uint8_t ttl;
        uint8_t ttvn; /* destination translation table version number */
        uint8_t dest[6];
        /* "4 bytes boundary + 2 bytes" long to make the payload after the
         * following ethernet header again 4 bytes boundary aligned
         */
};
struct batadv_tvlv_tt_changeOriginal
{
        uint8_t flags;
        uint8_t reserved[3];
        uint8_t addr[6];
        uint8_t vid;
};

class slidingWindowEntry;
//Its a sequence of MACAddresses and a sequence number from this MAC
//typedef std::map<MACAddress, long> slidingWindowObject; //Map allows just unique keys - this doesn't work for a sliding window
typedef std::vector<slidingWindowEntry> slidingWindowObject;
class batadv_gw_node;
typedef std::vector<batadv_gw_node*> gwListObject;
class batadv_orig_node;
typedef std::map<ManetAddress, batadv_orig_node*> nodeList;

class batadv_tt_global_entry;
class batadv_tt_orig_list_entry;
class batadv_tt_local_entry;
typedef std::map<ManetAddress, batadv_tt_local_entry*> TranslationTableLocal;
typedef std::map<ManetAddress, batadv_tt_global_entry*> TranslationTableGlobal;

class slidingWindowEntry
{
    public:
        MACAddress addr;
        long seqno;
        int tq;
};
class batadv_neigh_node;
/**
 * struct batadv_hard_iface_bat_iv - per hard interface B.A.T.M.A.N. IV data
 * @ogm_buff: buffer holding the OGM packet
 * @ogm_buff_len: length of the OGM packet buffer
 * @ogm_seqno: OGM sequence number - used to identify each OGM
 */
class batadv_hard_iface_bat_iv
{
    public:
        std::vector<batadv_ogm_packet> ogm_buff;
        int ogm_buff_len;
        long ogm_seqno;
};

/**
 * struct batadv_hard_iface - network device known to batman-adv
 * @if_num: identificator of the interface
 * @if_status: status of the interface for batman-adv
 * @num_bcasts: number of payload re-broadcasts on this interface (ARQ)
 * @refcount: Gate, implemented within omnet++ ned files
 * @hardif_obj: kobject of the per interface sysfs "mesh" directory
 * @refcount: number of contexts the object is used
 * @soft_iface: the batman-adv interface which uses this network interface
 * @bat_iv: BATMAN IV specific per hard interface data
 */
class batadv_hard_iface
{
    public:
        int if_num;
        batadv_hard_if_state if_status;
        int num_bcasts;
        int refcount;
        int name;
        cGate *realGateIn;
        cGate *realGateOut;
        struct net_device *soft_iface;
        class batadv_hard_iface_bat_iv bat_iv;

};
class abcde
{
    public:
        std::vector<batadv_ogm_packet *> aggregated;
};

/**
 * struct batadv_tt_common_entry - tt local & tt global common data
 * @addr: mac address of non-mesh client
 * @vid: VLAN identifier
 * @hash_entry: hlist node for batadv_priv_tt::local_hash or for
 *  batadv_priv_tt::global_hash
 * @flags: various state handling flags (see batadv_tt_client_flags)
 * @added_at: timestamp used for purging stale tt common entries
 * @refcount: number of contexts the object is used
 * @rcu: struct used for freeing in an RCU-safe manner
 */
class batadv_tt_common_entry
{
    public:
        MACAddress addr;
        unsigned short vid;
        //struct hlist_node hash_entry;
        unsigned long flags;
        simtime_t added_at;
        /*atomic_t refcount;
         struct rcu_head rcu;*/
};

/**
 * struct batadv_tt_global_entry - translation table global entry data
 * @common: general translation table data
 * @orig_list: list of orig nodes announcing this non-mesh client
 * @orig_list_count: number of items in the orig_list
 * @list_lock: lock protecting orig_list
 * @roam_at: time at which TT_GLOBAL_ROAM was set
 */
class batadv_tt_global_entry
{
    public:
        batadv_tt_common_entry common;
        std::vector<batadv_tt_orig_list_entry> orig_list; //std::vector<batadv_orig_node*> orig_list;
        int orig_list_count;
        //spinlock_t list_lock;   /* protects orig_list */
        simtime_t roam_at;
};
/**
 * batadv_tt_orig_list_entry - orig node announcing a non-mesh client
 * @orig_node: pointer to orig node announcing this non-mesh client
 * @ttvn: translation table version number which added the non-mesh client
 */
class batadv_tt_orig_list_entry
{
    public:
        batadv_orig_node *orig_node;
        int ttvn;
        //batadv_tt_local_entry *tt_local_entry;
        //std::vector<batadv_tt_global_entry*> tt_global_entry_list;

};
/**
 * struct batadv_orig_ifinfo - originator info per outgoing interface
 * @if_outgoing: pointer to outgoing hard interface
 * @router: router that should be used to reach this originator
 * @last_real_seqno: last and best known sequence number
 * @last_ttl: ttl of last received packet
 * @batman_seqno_reset: time when the batman seqno window was reset
 * @refcount: number of contexts the object is used
 * @rcu: struct used for freeing in an RCU-safe manner
 */
class batadv_orig_ifinfo
{
    public:
        batadv_hard_iface *if_outgoing;
        int ifName;
        batadv_neigh_node *router;
        long last_real_seqno;
        long last_ttl;
        simtime_t batman_seqno_reset;
        int refcount;

};

class batadv
{
    public:
        batadv();
        virtual ~batadv();
};

class batadv_orig_node;
class batadv_neigh_ifinfo;
/**
 * struct batadv_neigh_node - structure for single hops neighbors
 * @list: list node for batadv_orig_node::neigh_list
 * @orig_node: pointer to corresponding orig_node
 * @addr: the MAC address of the neighboring interface
 * @ifinfo_list: list for routing metrics per outgoing interface
 * @if_incoming: pointer to incoming hard interface
 * @last_seen: when last packet via this neighbor was received
 * @last_ttl: last received ttl from this neigh node
 */
class batadv_neigh_node
{
    public:
        batadv_orig_node *orig_node;
        ManetAddress addr;
        int ifNameIncoming;
        std::vector<batadv_neigh_ifinfo *> ifinfo_list;
        batadv_hard_iface *if_incoming;
        simtime_t last_seen;
        int refcount;
};

/**
 * struct batadv_orig_bat_iv - B.A.T.M.A.N. IV private orig_node members
 * @bcast_own: bitfield containing the number of our OGMs this orig_node
 *  rebroadcasted "back" to us (relative to last_real_seqno)
 * @bcast_own_sum: counted result of bcast_own
 * @ogm_cnt_lock: lock protecting bcast_own, bcast_own_sum,
 *  neigh_node->bat_iv.real_bits & neigh_node->bat_iv.real_packet_count
 */
class batadv_orig_bat_iv
{
    public:
        unsigned long *bcast_own;
        long bcast_own_sum;
};

class batadv_orig_node : public cObject
{
    public:
        ManetAddress orig;
        std::vector<batadv_orig_ifinfo *> ifinfo_list;
        //list_t ifinfo_list;
        batadv_orig_ifinfo *ifinfo;
        //struct batadv_orig_ifinfo *last_bonding_candidate;
        /*#ifdef CONFIG_BATMAN_ADV_DAT
         batadv_dat_addr_t dat_addr;
         #endif*/
        simtime_t last_seen;
        simtime_t bcast_seqno_reset;
        /*#ifdef CONFIG_BATMAN_ADV_MCAST
         uint8_t mcast_flags;
         struct hlist_node mcast_want_all_unsnoopables_node;
         struct hlist_node mcast_want_all_ipv4_node;
         struct hlist_node mcast_want_all_ipv6_node;
         #endif*/
        int capabilities;
        long capa_initialized;
        long last_ttvn;
        std::vector<batadv_tvlv_tt_change> tt_buff_vector;
        long tt_buff_len;
        //spinlock_t tt_buff_lock; /* protects tt_buff & tt_buff_len */
        /* prevents from changing the table while reading it */
        //spinlock_t tt_lock;
        //DECLARE_BITMAP(bcast_bits, BATADV_TQ_LOCAL_WINDOW_SIZE);
        long last_bcast_seqno;
        std::vector<batadv_neigh_node *> neigh_list;
        /* neigh_list_lock protects: neigh_list and router */
        //spinlock_t neigh_list_lock;
        //struct hlist_node hash_entry;
        //batadv_priv *bat_priv;
        /* bcast_seqno_lock protects: bcast_bits & last_bcast_seqno */
        //spinlock_t bcast_seqno_lock;
        int refcount;
        // struct rcu_head rcu;
        //struct batadv_frag_table_entry fragments[BATADV_FRAG_BUFFER_COUNT];
        //cLinkedList vlan_list;
        //spinlock_t vlan_list_lock; /* protects vlan_list */
        batadv_orig_bat_iv bat_iv;
};

/**
 * struct batadv_forw_packet - structure for bcast packets to be sent/forwarded
 * @list: list node for batadv_socket_client::queue_list
 * @send_time: execution time for delayed_work (packet sending)
 * @own: bool for locally generated packets (local OGMs are re-scheduled after
 *  sending)
 * @skb: bcast packet's skb buffer
 * @packet_len: size of aggregated OGM packet inside the skb buffer
 * @direct_link_flags: direct link flags for aggregated OGM packets
 * @num_packets: counter for bcast packet retransmission
 * @delayed_work: work queue callback item for packet sending
 * @if_incoming: pointer to incoming hard-iface or primary iface if
 *  locally generated packet
 * @if_outgoing: packet where the packet should be sent to, or NULL if
 *  unspecified
 */
class batadv_forw_packet
{
    public:
        unsigned long send_time;
        double id;
        bool own;
        std::vector<batadv_ogm_packetClass> skb;
        unsigned long packet_len;
        unsigned long direct_link_flags;
        unsigned int num_packets;
        batadv_hard_iface if_incoming;
        batadv_hard_iface if_outgoing;
        int if_incomingName;
        int if_outgoingName;
        int name_if_incoming;
        int name_if_outgoing;
};



/**
 * struct batadv_neigh_ifinfo_bat_iv - neighbor information per outgoing
 *  interface for BATMAN IV
 * @tq_recv: ring buffer of received TQ values from this neigh node
 * @tq_index: ring buffer index
 * @tq_avg: averaged tq of all tq values in the ring buffer (tq_recv)
 * @real_bits: bitfield containing the number of OGMs received from this neigh
 *  node (relative to orig_node->last_real_seqno)
 * @real_packet_count: counted result of real_bits
 */
class batadv_neigh_ifinfo_bat_iv
{
    public:
        int tq_recv[TQ_GLOBAL_WINDOW_SIZE];
        //std::vector<int> tq_recv;
        int tq_index;
        int tq_avg;
        //std::vector<> real_bits;
        //DECLARE_BITMAP(real_bits, BATADV_TQ_LOCAL_WINDOW_SIZE);
        int real_packet_count;
};

/**
 * struct batadv_neigh_ifinfo - neighbor information per outgoing interface
 * @list: list node for batadv_neigh_node::ifinfo_list
 * @if_outgoing: pointer to outgoing hard interface
 * @bat_iv: B.A.T.M.A.N. IV private structure
 * @last_ttl: last received ttl from this neigh node
 * @refcount: number of contexts the object is used
 * @rcu: struct used for freeing in a RCU-safe manner
 */
class batadv_neigh_ifinfo
{
    public:
        batadv_hard_iface *if_outgoing;
        int ifName;
        batadv_neigh_ifinfo_bat_iv bat_iv;
        long last_ttl;
        int refcount;
};

/***************************************** START: SLIDING WINDOW *****************************************/
class batadv_sliding_window_entry
{
    public:
        MACAddress originator;
        long seqno;
};

/***************************************** END: SLIDING WINDOW *****************************************/

/**
 * struct batadv_tt_local_entry - translation table local entry data
 * @common: general translation table data
 * @last_seen: timestamp used for purging stale tt local entries
 */
class batadv_tt_local_entry
{
    public:
        batadv_tt_common_entry common;        // std::vector<batadv_tt_common_entry *> common;
        simtime_t last_seen;
};

/**
 * struct batadv_tvlv_tt_change - translation table diff data
 * @flags: status indicators concerning the non-mesh client (see
 *  batadv_tt_client_flags)
 * @reserved: reserved field - useful for alignment purposes only
 * @addr: mac address of non-mesh client that triggered this tt change
 * @vid: VLAN identifier
 */
/*class batadv_tvlv_tt_change
 {
 public:
 int flags;
 //uint8_t reserved[3];
 MACAddress addr;
 int vid;
 };*/

/**
 * struct batadv_tt_change_node - structure for tt changes occured
 * @list: list node for batadv_priv_tt::changes_list
 * @change: holds the actual translation table diff data
 */
class batadv_tt_change_node
{
    public:
        //struct list_head list;
        batadv_tvlv_tt_change change;
};

/**
 * struct batadv_tt_req_node - data to keep track of the tt requests in flight
 * @addr: mac address address of the originator this request was sent to
 * @issued_at: timestamp used for purging stale tt requests
 * @list: list node for batadv_priv_tt::req_list
 */
class batadv_tt_req_node
{
    public:
        MACAddress addr;
        simtime_t issued_at;
        //struct list_head list;
};

/**
 * struct batadv_priv_tvlv - per mesh interface tvlv data
 * @ogm_tt: tt data for the next ogm
 */
class batadv_priv_tvlv
{
    public:
        batadv_tvlv_tt_data *ogm_tt;
};
/**
 * struct batadv_priv_tt - per mesh interface translation table data
 * @vn: translation table version number
 * @ogm_append_cnt: counter of number of OGMs containing the local tt diff
 * @local_changes: changes registered in an originator interval
 * @changes_list: tracks tt local changes within an originator interval
 * @local_hash: local translation table hash table
 * @global_hash: global translation table hash table
 * @req_list: list of pending & unanswered tt_requests
 * @roam_list: list of the last roaming events of each client limiting the
 *  number of roaming events to avoid route flapping
 * @changes_list_lock: lock protecting changes_list
 * @req_list_lock: lock protecting req_list
 * @roam_list_lock: lock protecting roam_list
 * @last_changeset: last tt changeset this host has generated
 * @last_changeset_len: length of last tt changeset this host has generated
 * @last_changeset_lock: lock protecting last_changeset & last_changeset_len
 * @commit_lock: prevents from executing a local TT commit while reading the
 *  local table. The local TT commit is made up by two operations (data
 *  structure update and metdata -CRC/TTVN- recalculation) and they have to be
 *  executed atomically in order to avoid another thread to read the
 *  table/metadata between those.
 * @work: work queue callback item for translation table purging
 */
class batadv_priv_tt
{
    public:
        int vn;
        int ogm_append_cnt;
        int local_changes; // changes registered in an originator interval
        //struct list_head changes_list;
        std::vector<batadv_tt_change_node *> changes_list;        //TODO: Is this correct?

        /*struct batadv_hashtable *local_hash;
         struct batadv_hashtable *global_hash;*/

        //struct list_head c;
        std::vector<batadv_tt_req_node*> req_list;        //TODO: Is this correct?
        std::vector<batadv_tt_req_node*> roam_list;        //TODO: Is this correct?
        //struct list_head roam_list;
        //spinlock_t changes_list_lock; // protects changes
        //spinlock_t req_list_lock; // protects req_list
        //spinlock_t roam_list_lock; // protects roam_list
        std::vector<batadv_tvlv_tt_change> last_changeset;        //This should be a vector right?

        long last_changeset_len;
        /* protects last_changeset & last_changeset_len */
        //spinlock_t last_changeset_lock;
        /* prevents from executing a commit while reading the table */
        //spinlock_t commit_lock;
        //struct delayed_work work;
};
/**
 * struct batadv_tvlv_tt_vlan_data - vlan specific tt data propagated through
 *  the tt tvlv container
 * @crc: crc32 checksum of the entries belonging to this vlan
 * @vid: vlan identifier
 * @reserved: unused, useful for alignment purposes
 */
class batadv_tvlv_tt_vlan_data
{
    public:
        long crc;
        long vid;
        long reserved;
};
/***************************************** END: Translation table *****************************************/
/***************************************** START: Gateway *****************************************/

/**
 * struct batadv_gw_node - structure for orig nodes announcing gw capabilities
 * @list: list node for batadv_priv_gw::list
 * @orig_node: pointer to corresponding orig node
 * @bandwidth_down: advertised uplink download bandwidth
 * @bandwidth_up: advertised uplink upload bandwidth
 * @deleted: this object is scheduled for deletion
 */
class batadv_gw_node
{
    public:
        batadv_orig_node *orig_node;
        double bandwidth_down;
        double bandwidth_up;
        unsigned long deleted;
        bool reselect;
};

/**
 *  batadv_priv_gw - per mesh interface gateway data
 * @list: list of available gateway nodes
 * @list_lock: lock protecting gw_list & curr_gw
 * @curr_gw: pointer to currently selected gateway node
 * @bandwidth_down: advertised uplink download bandwidth (if gw_mode server)
 * @bandwidth_up: advertised uplink upload bandwidth (if gw_mode server)
 * @reselect: bool indicating a gateway re-selection is in progress
 */
class batadv_priv_gw
{
    public:
        batadv_gw_node *curr_gw;
        double bandwidth_down;
        double bandwidth_up;
        bool reselect;
};
/***************************************** END: Gateway*****************************************/
#endif /* BATADV_H_ */

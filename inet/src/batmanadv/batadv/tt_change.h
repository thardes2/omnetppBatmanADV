
#include "MACAddress.h"
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
        int flags;//123:  enum batadv_tt_client_flags - TT client specific flags
        //uint8_t reserved[3];
        MACAddress addr;
        int vid;
};*/

class batadv_ogm_packetClass
{
    public:
        int packet_type;
        int version;
        int ttl;
        int flags;
        long seqno;
        ManetAddress orig;
        ManetAddress prev_sender;
        int reserved;
        int tq;
        long tvlv_len;
        int meshID;
        batadv_tvlv_tt_data tt_data;
        batadv_tvlv_tt_change tt_change;
        batadv_tvlv_gateway_data gateway_data;
};


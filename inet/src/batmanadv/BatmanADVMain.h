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

#ifndef BATMANADVMAIN_H_
#define BATMANADVMAIN_H_
#include <Originator.h>
#include "batadv.h"

#include "slidingWindow.h"
#include <omnetpp.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "UDPPacket.h"
#include <stdio.h>
#include <string.h>
#include <cmath>
#include "Ieee80211MgmtBase.h"
#include "ILifecycle.h"
#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "IPv4ControlInfo.h"
#include "IPv4Datagram.h"
#include <stdlib.h>
#include "MACBase.h"
#include "StatisticsCollector.h"

#include "ogm_m.h"
#include "tvlv_m.h"
#include "unicast_m.h"
#include "broadcast_m.h"
#include "ogm_reminder_m.h"
#include "DHCPMessage_m.h"
#include "WirelessMacBase.h"
#include "AirFrame_m.h"
#include "Ieee80211Frame_m.h"
#include "Ieee802Ctrl_m.h"
#include "UDPPacket_m.h"

//#include "ManetRoutingBase.h"
#include <map>

class BatmanADVMain : public Ieee80211MgmtBase //cSimpleModule//: public WirelessMacBase//, public ManetRoutingBase
{

    public:
        //Statistics
        static int selectedGWNone;
        static int selectedGW1;
        static int selectedGW2;
        static int selectedGW3;
        static int selectedGW4;
        static int selectedGW5;
        static int selectedGW6;
        static int selectedGW7;
        static int selectedGW8;
    private:
        //Statistics
        cLongHistogram knownOrigsHisto;
        cLongHistogram hopCountStats;
        cOutVector vectorSelectedGWNone;
        cOutVector vectorSelectedGW1;
        cOutVector vectorSelectedGW2;
        cOutVector vectorSelectedGW3;
        cOutVector vectorSelectedGW4;
        cOutVector vectorSelectedGW5;
        cOutVector vectorSelectedGW6;
        cOutVector vectorSelectedGW7;
        cOutVector vectorSelectedGW8;

        cOutVector ogmAggregated;

        cOutVector numForwardOGMSent;
        cOutVector numOGMReceived;
        cOutVector numOGMReceivedWlan;
        cOutVector numOGMReceivedVpn;
        cOutVector numOwnOGMSent;
        cOutVector numOwnOGMSentWlan;
        cOutVector numOwnOGMSentVpn;
        cOutVector knownOrigs;
        cOutVector knownLocalClients;
        cOutVector knownGlobalClients;
        cOutVector purgedOrigs;
        long numForwardOGMSentCount;
        long numOGMReceivedCount;
        long numOGMReceivedCountWlan;
        long numOGMReceivedCountVpn;
        long numOwnOGMSentCount;
        long numOwnOGMSentCountWlan;
        long numOwnOGMSentCountVpn;
        long knownOrigsCount;
        long knownLocalClientsCount;
        long knownGlobalClientsCount;

        simtime_t warmupTime;
        int wlanName = 1;
        int meshName = 0;
    public:
        cOutVector connectedToGw1;
        cOutVector connectedToGw2;
        int connectedToGw1Count;
        int connectedToGw2Count;
    private:
        TranslationTableLocal tt_local;
        TranslationTableGlobal tt_global;
        /** START: Interfaces ***/
        //----> Mesh interfaces
        batadv_hard_iface* wlan0;
        batadv_hard_iface* meshvpn;
        batadv_hard_iface* primary_if;
        batadv_hard_iface* BATADV_IF_DEFAULT;
        batadv_hard_iface* secondary_if;
        batadv_hard_iface wlan0NP;
        batadv_hard_iface meshvpnNP;
        batadv_hard_iface primary_ifNP;
        batadv_hard_iface BATADV_IF_DEFAULTNP;
        batadv_hard_iface secondary_ifNP;

        //----> Non-Mesh interfaces
        batadv_hard_iface *wlan1;
        /** END: Interfaces ***/

    public:

        int aggSuccess;
        int mtusize = 1476;
        IInterfaceTable *ift;
        EtherFrame* getCurrentFrame();
        void deleteCurrentFrame();
        //int nodenr;
        std::string nodenr;

        bool cluster;
        bool gwTermination;
        int meshId;
        int sizeBueren;
        int sizePaderborn;
        int sizeBaliSchlangenHoevelhof;
        int sizeRemainder;

        int iNodenr;
        //int ttvn; //TranslationTableVersionNumber
        int vid = 0; //VLAN ID -> Not used in PB
bool wifiOnly;
        bool ogm_aggregation = true;
        int batman_queue_left;
        int bcast_queue_left;
        bool windowReset;
        batadv_priv_tt bat_priv_tt;
        batadv_priv_tvlv bat_priv_tvlv;
        batadv_priv_gw bat_priv_gw;
        std::vector<batadv_forw_packet> forw_bat_list;
        std::vector<batadv_forw_packet*> forw_bcast_list;


        batadv_ogm_packet* convertToOGM(batadv_ogm_packetClass object);
        batadv_ogm_packetClass convertFromOGM(batadv_ogm_packet* object);

        BatmanADVMain();
        virtual ~BatmanADVMain();
        StatisticsCollector* getGlobalStatisticsController();
        batadv_orig_node* batadv_iv_ogm_orig_get(batadv_ogm_packet *ogm);
        batadv_orig_node* batadv_iv_ogm_orig_get_by_mac(MACAddress mac);
        batadv_orig_node* checkOrigListForExistence(ManetAddress node);
        void ttLocalAdd(ManetAddress addr, batadv_tt_local_entry* tt_local_entry);

        //NET_XMIT batadv_send_packet_to_orig(EtherFrame* f, batadv_orig_node *orig_node, batadv_hard_iface *recv_if);
        void batadv_iv_ogm_schedule(batadv_hard_iface hard_iface);
        void batadv_tvlv_container_ogm_append(batadv_ogm_packet *ogm);
        batadv_hard_iface* getHardIfForCGate(cGate* gate);
        batadv_hard_iface getHardIfForCGateNP(cGate* gate);
        void gen_random(char *s, const int len);
        std::string random_string(int length);
        double fRand(double fMin, double fMax);
        long batadv_tt_local_remove(MACAddress *addr, int vid, char* message, bool roaming);
        unsigned long getBitForNumber(unsigned int nr);
        void handleOGM(batadv_ogm_packet* ogm, bool wifi);
        //Variables
    protected:

        int it;

        //statistics
        void initializeStatistics();
        void updateGwStats(batadv_gw_node *new_gw_node, batadv_gw_node *old);
        void batadv_collect_periodicstats();
        nodeList origMap;
        //START: OGM
        void batadv_iv_ogm_process(batadv_hard_iface if_incoming);
        void batadv_iv_ogm_process_per_outif(batadv_orig_node *orig_node, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing);
        batadv_neigh_node* batadv_orig_router_get(batadv_orig_node *orig_node,batadv_hard_iface *if_outgoing);
        batadv_orig_node* batadv_orig_node_new(MACAddress addr);
        batadv_neigh_node* batadv_iv_ogm_neigh_new(MACAddress neigh_addr, batadv_orig_node *orig_node, batadv_orig_node *orig_neigh, batadv_hard_iface *if_incoming);
        batadv_neigh_node* batadv_neigh_node_new(MACAddress neigh_addr, batadv_orig_node *orig_node, batadv_hard_iface *if_incoming);
        bool batadv_iv_ogm_calc_tq(batadv_orig_node *orig_node, batadv_orig_node *orig_neigh_node, batadv_ogm_packet *batadv_ogm_packet, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing);
        batadv_dup_status batadv_iv_ogm_update_seqnos(batadv_ogm_packet *ogm_packet, batadv_hard_iface if_incoming,batadv_hard_iface if_outgoing);
        batadv_neigh_ifinfo* batadv_neigh_ifinfo_get(batadv_neigh_node *neigh, batadv_hard_iface *if_outgoing);
        batadv_neigh_ifinfo* batadv_neigh_ifinfo_new(batadv_neigh_node *neigh, batadv_hard_iface *if_outgoing);
        batadv_neigh_node* batadv_neigh_node_get(batadv_orig_node *orig_node, MACAddress addr,batadv_hard_iface *hard_iface);
        void batadv_iv_ogm_orig_update(batadv_orig_node *orig_node, batadv_orig_ifinfo *orig_ifinfo, MACAddress ethhdr, batadv_ogm_packet *batadv_ogm_packet, batadv_hard_iface if_incoming,  batadv_hard_iface if_outgoing, batadv_dup_status dup_status);
        void batadv_iv_ogm_forward(batadv_orig_node *orig_node, MACAddress *ethhdr, batadv_ogm_packet *batadv_ogm_packet, bool is_single_hop_neigh, bool is_from_best_next_hop, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, forwardReason reason);
        int batadv_hop_penalty(int tq);
        void batadv_purge_orig();
        void _batadv_purge_orig();
        bool batadv_purge_orig_node(batadv_orig_node* orig_node);
        bool batadv_purge_orig_neighbors(batadv_orig_node* orig_node);
        bool batadv_purge_orig_ifinfo(batadv_orig_node *orig_node);
        batadv_neigh_node* batadv_find_best_neighbor(batadv_orig_node *orig_node, batadv_hard_iface *if_outgoing);
        void batadv_purge_neigh_ifinfo(/*struct batadv_priv *bat_priv,*/batadv_neigh_node *neigh);
        int batadv_iv_ogm_neigh_cmp(batadv_neigh_node *neigh1, batadv_hard_iface *if_outgoing1, batadv_neigh_node *neigh2, batadv_hard_iface *if_outgoing2);
        void batadv_iv_ogm_queue_add(batadv_ogm_packet *ogm_packet, long packet_len, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, bool own_packet, forwardReason reason, double send_time);
        ////Aggregation
        void batadv_iv_ogm_aggregate_new(batadv_ogm_packet *packet_buff, long packet_len, double send_time, bool direct_link, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, bool own_packet, forwardReason reason);
        void batadv_iv_ogm_aggregate(batadv_forw_packet& forw_packet_aggr, batadv_ogm_packet *ogm, long packet_len,bool direct_link);
        void batadv_iv_ogm_emit(batadv_forw_packet forw_packet, forwardReason reason);
        void batadv_iv_ogm_send_to_if(batadv_forw_packet forw_packet, batadv_hard_iface hard_iface, forwardReason reason);
        bool batadv_iv_ogm_can_aggregate(batadv_ogm_packet *new_bat_ogm_packet, int packet_len, double send_time, bool directlink, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, batadv_forw_packet forw_packet);
        bool batadv_iv_ogm_aggr_packet();
        double batadv_iv_ogm_emit_send_time();
        double batadv_iv_ogm_fwd_send_time();




        batadv_dup_status batadv_iv_ogm_update_seqnosIfChange(MACAddress* mac, batadv_ogm_packet * ogm_packet, batadv_hard_iface if_incoming);
        batadv_neigh_ifinfo* batadv_neigh_ifinfo_newNoOutgoing(batadv_neigh_node *neigh);
        batadv_orig_ifinfo* batadv_orig_ifinfo_newNoOutgoing(batadv_orig_node *orig_node, ManetAddress prevSender);
        batadv_neigh_node* batadv_orig_router_getNoOutgoing(batadv_orig_node *orig_node);
        batadv_neigh_ifinfo* batadv_neigh_ifinfo_getNoOutgoing(batadv_neigh_node *neigh);
        bool batadv_iv_ogm_calc_tqNoOutgoing(batadv_orig_node *orig_node, batadv_orig_node *orig_neigh_node, batadv_ogm_packet *ogm, batadv_hard_iface if_incoming);
        void batadv_iv_ogm_orig_updateNoOutgoing(batadv_orig_node *orig_node, batadv_orig_ifinfo *orig_ifinfo, MACAddress ethhdr,batadv_ogm_packet *ogm, batadv_hard_iface if_incoming, batadv_dup_status dup_status);
        void batadv_iv_ogm_forwardNoOutgoing(batadv_orig_node *orig_node, MACAddress *ethhdr, batadv_ogm_packet *batadv_ogm_packet, bool is_single_hop_neigh, bool is_from_best_next_hop, batadv_hard_iface if_incoming, forwardReason reason);
        void batadv_iv_ogm_queue_addNoOutgoing(batadv_ogm_packet *ogm_packet, long packet_len, batadv_hard_iface if_incoming, bool own_packet, forwardReason reason, double send_time);
        bool batadv_iv_ogm_can_aggregateNoOutgoing(batadv_ogm_packet *new_bat_ogm_packet, int packet_len, double send_time, bool directlink, batadv_hard_iface if_incoming, batadv_forw_packet forw_packet);
        void batadv_iv_ogm_aggregate_newNoOutgoing(batadv_ogm_packet *packet_buff, long packet_len, double send_time, bool direct_link, batadv_hard_iface if_incoming, bool own_packet, forwardReason reason);
        void batadv_update_routeNoOutgoing(batadv_orig_node *orig_node, batadv_neigh_node *neigh_node);
        void _batadv_update_routeNoOutgoing(batadv_orig_node *orig_node, batadv_neigh_node *neigh_node);
        void batadv_send_outstanding_bat_ogm_packetNoOutgoing(double id, forwardReason reason);
        void batadv_iv_ogm_send_to_ifNoOutgoing(batadv_forw_packet forw_packet,  forwardReason reason);
        void batadv_iv_ogm_emitNoOutgoing(batadv_forw_packet forw_packet, forwardReason reason);
        //END: OGM

        //START: IFINFO
        batadv_orig_ifinfo* batadv_orig_ifinfo_new(batadv_orig_node *orig_node, ManetAddress prevSender,       batadv_hard_iface * if_outgoing);
        batadv_orig_ifinfo* batadv_orig_ifinfo_get(batadv_orig_node *orig_node, batadv_hard_iface * if_outgoing);
        //END: IFINFO

        //START: ROUTING
        int batadv_window_protected(long seq_num_diff, simtime_t last_reset);
        bool batadv_has_timed_out(simtime_t timestamp, simtime_t timeout);
        void batadv_update_route(batadv_orig_node *orig_node, batadv_hard_iface recv_if, batadv_neigh_node *neigh_node);
        void _batadv_update_route(batadv_orig_node *orig_node, batadv_hard_iface recv_if, batadv_neigh_node *neigh_node);
        int batadv_check_unicast_packet(/*struct batadv_priv *bat_priv,batadv_unicast_4addr_packet* packet*/MACAddress dest, MACAddress src);
        batadv_neigh_node* batadv_find_router(/*struct batadv_priv *bat_priv,*/batadv_orig_node *orig_node, batadv_hard_iface *recv_if);
        batadv_neigh_node* batadv_find_routerNoOutgoing(batadv_orig_node *orig_node);
        int batadv_route_unicast_packet(cPacket *packet, batadv_hard_iface *recv_if);
        int batadv_recv_unicast_packet(batadv_hard_iface *recv_if, cMessage* unicast_packet);
        int batadv_check_unicast_ttvn(batadv_hard_iface *recv_if, batadv_unicast_packet* unicast_packet);
        bool batadv_reroute_unicast_packet(/*struct batadv_priv *bat_priv,*/batadv_unicast_packet *unicast_packet, MACAddress *dst_addr, unsigned short vid);
        int batadv_recv_bcast_packet(cMessage *msg, batadv_hard_iface *recv_if);
        int batadv_recv_unicast_tvlv(cMessage *msg, batadv_hard_iface *recv_if);
        //END: ROUTING

        //START: BITARRAY
        int batadv_test_bit(/*long seq_bits,*/long last_seqno, long curr_seqno);
        int batadv_bit_get_packet(unsigned long *seq_bits, long seq_num_diff, int set_mark);
        void batadv_set_bit(unsigned long *seq_bits, long n);
        //END: BITARRAY

        //START: SLIDING WINDOW
        slidingWindowObject ogmWindow;
        int batadv_iv_window_CheckSize();
        void batadv_iv_window_shift_left(long n);
        batadv_window_status batadv_iv_window_add_entry(MACAddress mac, long seqno, int tq);
        int batadv_sliding_window_avg();
        int batadv_iv_window_packetCount(MACAddress mac);
        void batadv_sliding_window_set_tq(MACAddress addr, int tq_value);
        void batadv_iv_window_create();
        void batadv_iv_window_iterate();
        bool batadv_sliding_window_is_dup(long diff);
        //END: SLIDING WINDOW

        //START: RINGBUFFER
        void batadv_ring_buffer_set(int lq_recv[], int *lq_index, int value);
        void batadv_ring_buffer_set_vector(std::vector<int> *buffer, int value);
        int batadv_ring_buffer_avg(int lq_recv[]);
        //END: RINGBUFFER

        //START: TranslationTable
        void batadv_tt_global_del_orig(batadv_orig_node *orig_node, long vid, char *message);
        batadv_tt_local_entry* checkLocalTranslationTableForExistence(ManetAddress node);
        batadv_tt_global_entry* checkGlobalTranslationTableForExistence(ManetAddress node);
        bool batadv_is_my_client(MACAddress addr);
        bool batadv_tt_local_add(MACAddress addr, batadv_hard_iface* device);
        void batadv_tt_local_event(batadv_tt_local_entry *tt_local_entry, int event_flags);
        void batadv_send_roam_adv(MACAddress client, unsigned short vid, batadv_tt_orig_list_entry *orig_node);
        void batadv_tt_global_free(batadv_tt_global_entry *tt_global, const char *message);
        void batadv_tt_local_purge_list(batadv_tt_local_entry *tt_local_local, int timeout);
        bool batadv_send_tt_request(batadv_orig_node *dst_orig_node, int ttvn, batadv_tvlv_tt_vlan_data *tt_vlan, long num_vlan, bool full_table);
        batadv_tt_req_node* batadv_new_tt_req_node(batadv_orig_node *orig_node);
        void batadv_tvlv_unicast_send(batadv_hard_iface *primary_if, ManetAddress dst, int version, batadv_tvlv_tt_data *tvlv_value, forwardReason reason);
        bool batadv_send_other_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress *req_src, MACAddress *req_dst);
        bool batadv_send_my_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress *req_src);
        bool batadv_send_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress *req_src, MACAddress *req_dst);
        long batadv_tt_prepare_tvlv_local_data(batadv_tvlv_tt_data **tt_data, batadv_tvlv_tt_change **tt_change, long *tt_len);
        void batadv_tt_global_del_orig_entry(batadv_tt_global_entry *tt_global_entry, batadv_tt_orig_list_entry *orig_entry);
        //void batadv_tt_update_orig(batadv_orig_node *orig_node, batadv_tvlv_tt_vlan_data *tt_buff, long tt_num_vlan, cArray paras, long tt_num_changes, int ttvn);
        void batadv_tt_update_orig(batadv_orig_node *orig_node, batadv_tvlv_tt_vlan_data *tt_buff, long tt_num_vlan, std::vector<batadv_tvlv_tt_change> tt_changeList, long tt_num_changes, int ttvn);
        void batadv_tt_update_changes(batadv_orig_node *orig_node, int ttvn, std::vector<batadv_tvlv_tt_change> tt_changeList);
        bool batadv_tt_add_temporary_global_entry(batadv_orig_node *orig_node, MACAddress *addr, unsigned int vid);
        bool batadv_tt_global_add(batadv_orig_node *orig_node, MACAddress *tt_addr, int vid, int flags, int ttvn);
        bool batadv_bla_is_backbone_gw_orig(ManetAddress orig, unsigned short vid);
        void batadv_tt_global_del_orig_list(batadv_tt_global_entry *tt_global_entry);
        void batadv_tt_global_orig_entry_add(batadv_tt_global_entry *tt_global, batadv_orig_node *orig_node, int ttvn);
        bool batadv_tt_global_to_purge(batadv_tt_global_entry *tt_global_entry, char **msg);
        bool batadv_tt_local_client_is_roaming(MACAddress *addr, unsigned short vid);
        void _batadv_tt_update_changes(batadv_orig_node *orig_node, std::vector<batadv_tvlv_tt_change> tt_changeList, int ttvn); //batadv_tvlv_tt_change *tt_change
        void batadv_tt_global_del(batadv_orig_node *orig_node, MACAddress *addr, unsigned short vid, const char *message, bool roaming);
        void batadv_tt_global_del_roaming(batadv_tt_global_entry *tt_global_entry, batadv_orig_node *orig_node, const char *message);
        void batadv_tt_global_del_orig_node(batadv_tt_global_entry *tt_global_entry, batadv_orig_node *orig_node, const char *message);
        void batadv_tt_global_purge();
        batadv_tt_orig_list_entry* batadv_tt_global_orig_entry_find(batadv_tt_global_entry *entry, batadv_orig_node *orig_node);
        void batadv_tt_purge();
        void batadv_tt_save_orig_buffer(batadv_orig_node *orig_node, std::vector<batadv_tvlv_tt_change> tt_changeList);
        void batadv_tt_local_commit_changes();
        void batadv_tt_local_commit_changes_nolock();
        void batadv_mcast_mla_update();
        void batadv_tt_tvlv_container_update();
        void batadv_tt_local_set_flags(long flags, bool enable, bool count);
        void batadv_tt_local_purge_pending_clients();
        void batadv_tt_local_set_pending(batadv_tt_local_entry *tt_local_entry, long flags, const char *message);
        void batadv_tt_local_purge(long timeout);
        //START: TranslationTable - Handlers
        int batadv_tt_tvlv_unicast_handler_v1(MACAddress *src, MACAddress *dst, batadv_tvlv_tt_data *tt_data/*,uint16_t tvlv_value_len*/);
        //END: TranslationTable - Handlers
        void batadv_handle_tt_response(batadv_tvlv_tt_data *tt_data, MACAddress *resp_src);
        void batadv_tt_fill_gtable(std::vector<batadv_tvlv_tt_change> tt_changeList, int ttvn, MACAddress *resp_src);
        void batadv_tt_tvlv_generate(TranslationTableLocal* tt_local, batadv_tvlv_tt_data* tt_data); //batadv_tvlv_tt_change *tt_change);
        void batadv_tt_tvlv_generate_global(TranslationTableGlobal global, batadv_tvlv_tt_data *tvlv_tt_data);
        long batadv_tt_prepare_tvlv_global_data(batadv_orig_node *orig_node, batadv_tvlv_tt_data **tt_data, batadv_tvlv_tt_change **tt_change, long *tt_len);
        bool batadv_del_tt_req_node(batadv_tt_req_node *tt_req_node);
        int batadv_tt_len(int changes_num);
        batadv_tt_orig_list_entry* batadv_transtable_best_orig(batadv_tt_global_entry *tt_global_entry);
        batadv_orig_node * batadv_transtable_search(MACAddress *src, MACAddress *addr, int vid);
        //END: TranslationTable

        //START: Hard interface
        bool batadv_is_wifi_netdev(batadv_hard_iface* device);
        void batadv_interface_rx(cMessage* msg, batadv_hard_iface *recv_if, batadv_orig_node *orig_node);
        //END: Hard interface

        //START: Gateway
        void batadv_gw_select(batadv_gw_node *new_gw_node);
        void batadv_gw_reselect();
        batadv_gw_node* batadv_gw_get_best_gw_node();
        void batadv_gw_selection();
        void batadv_gw_node_add(batadv_orig_node *orig_node, batadv_tvlv_gateway_data *gateway);
        void batadv_gw_node_update(batadv_orig_node *orig_node, batadv_tvlv_gateway_data *gateway);
        batadv_gw_node* batadv_gw_node_get(batadv_orig_node *orig_node);
        void batadv_gw_node_purge();
        void batadv_gw_node_delete(batadv_orig_node *orig_node);
        void batadv_gw_tvlv_ogm_handler_v1(batadv_orig_node *orig, batadv_tvlv_gateway_data *tvlv_value);
        void batadv_gw_check_selection(batadv_orig_node *orig_node);
        void batadv_gw_tvlv_ogm_handler_v1(batadv_orig_node *orig, int flags, batadv_tvlv_gateway_data *tvlv_value);
        void forwardDHCPRequest(DHCPMessage *dhcpMessage);
        batadv_dhcp_recipient batadv_gw_dhcp_recipient_get(cMessage *msg);
        bool batadv_gw_out_of_range(Ieee80211DataFrameWithSNAP* frame);
        batadv_orig_node* batadv_gw_get_selected_orig();
        batadv_gw_node *selectedGw;
        gwListObject gwList;
        GatewayModes gw_sel_class;
        batadv_gw_modes gw_mode;
        //END: Gateway

        //STARAT: Multicast
        batadv_forw_mode batadv_mcast_forw_mode(batadv_orig_node **orig);
        int batadv_mcast_forw_mode_check(bool *is_unsnoopable);
        //END: Multicast

        //START: send
        int batadv_send_skb_via_gw(Ieee80211DataFrameWithSNAP* frame);
        void batadv_schedule_bat_ogm(batadv_hard_iface hard_iface);
        //void batadv_send_outstanding_bat_ogm_packet(batadv_forw_packet *forw_packet_aggr, forwardReason reason);
        void batadv_send_outstanding_bat_ogm_packet(double id, forwardReason reason);
        int batadv_send_skb_packet(cPacket *skb, batadv_hard_iface hard_iface, MACAddress dst_addr, forwardReason reason);
        NET_XMIT batadv_send_skb_to_orig(cPacket *skb, batadv_orig_node *orig_node, batadv_hard_iface *recv_if, forwardReason reason);
        int batadv_send_skb_unicast(cPacket* skb, int packet_type, int packet_subtype, batadv_orig_node *orig_node, unsigned short vid);
        bool batadv_send_skb_prepare_unicast(cPacket *skb, batadv_orig_node *orig_node);
        bool batadv_send_skb_push_fill_unicast(cPacket *skb, int hdr_size, batadv_orig_node *orig_node);
        int batadv_send_skb_via_tt(Ieee80211DataFrameWithSNAP* frame, MACAddress dst_hint);
        int batadv_send_skb_via_tt_generic(cPacket *skb, int packet_type, int packet_subtype, MACAddress dst_hint);
        //END: send

        int batadv_is_my_mac(MACAddress *addr);
    private:
        cStdDev rnd;
        simtime_t originator_interval;
        int version;
        int ogm_ttl;
        double hopPenalty;
        double gwClass_upload_speed;
        double gwClass_download_speed;
        int numberClients;
        EtherFrame* frame; //Represents the frame that was received latest

        ManetAddress ownAddress;

    protected:
        int getRandomSeqno();
        unsigned long longRandom();
        void batadv_mesh_init();
        void batadv_non_mesh_init();
        int batadv_compare_eth(MACAddress mac1, MACAddress mac2);
        batadv_ogm_packet* createDefaultOGM(batadv_hard_iface hard_iface);

    protected:
        void sendOGM(batadv_ogm_packet *ogm, MACAddress dest, simtime_t delay);

        virtual void initialize(int stage);
        virtual int numInitStages() const
        {
            return 1;
        }
        virtual void finish();
        virtual void handleMessage(cMessage *msg);

        // MACBase functions
        virtual void handleLowerMsg(cPacket* msg);
        virtual void handleSelfMsg(cMessage* msg);
        /** Implements abstract to use ETX packets */
        virtual void handleEtxMessage(cPacket* m);

        /** Implements abstract to use routing protocols in the mac layer */
        virtual void handleRoutingMessage(cPacket* m);

        /** Implements abstract to use inter gateway communication */
        virtual void handleWateGayDataReceive(cPacket *m);

        /** Implements the redirection of a data packets from a gateway to other */
        virtual void handleReroutingGateway(Ieee80211DataFrame *m);

        /** Implements abstract Ieee80211MgmtBase method */
        virtual void handleTimer(cMessage *msg);

        /** Implements abstract Ieee80211MgmtBase method */
        void handleUpperMessage(cPacket *msg);

        /** Implements abstract Ieee80211MgmtBase method -- throws an error (no commands supported) */
        virtual void handleCommand(int msgkind, cObject *ctrl);

        /** Utility function for handleUpperMessage() */
        virtual Ieee80211DataFrame *encapsulate(cPacket *msg);

        /** Called by the NotificationBoard whenever a change occurs we're interested in */
        virtual void receiveChangeNotification(int category, const cPolymorphic *details);
        /** @name Processing of different frame types */
        //@{
        virtual void handleDataFrame(Ieee80211DataFrame *frame);
        virtual void handleAuthenticationFrame(Ieee80211AuthenticationFrame *frame);
        virtual void handleDeauthenticationFrame(Ieee80211DeauthenticationFrame *frame);
        virtual void handleAssociationRequestFrame(Ieee80211AssociationRequestFrame *frame);
        virtual void handleAssociationResponseFrame(Ieee80211AssociationResponseFrame *frame);
        virtual void handleReassociationRequestFrame(Ieee80211ReassociationRequestFrame *frame);
        virtual void handleReassociationResponseFrame(Ieee80211ReassociationResponseFrame *frame);
        virtual void handleDisassociationFrame(Ieee80211DisassociationFrame *frame);
        virtual void handleBeaconFrame(Ieee80211BeaconFrame *frame);
        virtual void handleProbeRequestFrame(Ieee80211ProbeRequestFrame *frame);
        virtual void handleProbeResponseFrame(Ieee80211ProbeResponseFrame *frame);

        /****
         * DEBUG
         */

        void debug_showOrigList();
        void debug_local_tt();
        void debug_global_tt();
        void batadv_iv_ogm_orig_print(batadv_hard_iface *if_outgoing);
        void batadv_iv_ogm_orig_print_neigh(batadv_orig_node *orig_node, batadv_hard_iface *if_outgoing, std::ofstream& out);
        void batadv_tt_local_print_text();
        void batadv_tt_global_print_entry(batadv_tt_global_entry *tt_global_entry, std::ofstream& out);
        void batadv_tt_global_print_text();
};

#endif /* BATMANADVMAIN_H_ */

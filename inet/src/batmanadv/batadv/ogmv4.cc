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

#include <ogmv4.h>

ogm_v4::ogm_v4()
{
}

ogm_v4::~ogm_v4()
{
}
unsigned long BatmanADVMain::longRandom()
{
    /*double beta = LONG_MAX;
     rnd.collect(beta);
     return (unsigned long) exponential(beta, 0);*/
    return (unsigned long) uniform(0, LONG_MAX);
}
double BatmanADVMain::batadv_iv_ogm_emit_send_time()
{
    unsigned int msecs = 0;
    msecs = (unsigned long) originator_interval.dbl() * 1000 - BATADV_JITTER; //BATADVJitter in msecs
    unsigned long rNumber = longRandom();
    unsigned long jitter = rNumber % (2 * BATADV_JITTER);
    msecs += jitter;
    msecs = simTime().dbl() * 1000 + msecs;
    return msecs;
}

double BatmanADVMain::batadv_iv_ogm_fwd_send_time()
{
    unsigned int msecs = 0;
    unsigned long rNumber = longRandom();
    msecs = rNumber % (BATADV_JITTER / 2);
    msecs = simTime().dbl() * 1000 + msecs;
    return msecs;
}
/**
 * batadv_iv_ogm_queue_add - queue up an OGM for transmission
 * @bat_priv: the bat priv with all the soft interface information
 * @packet_buff: pointer to the OGM
 * @packet_len: (total) length of the OGM
 * @if_incoming: interface where the packet was received
 * @if_outgoing: interface for which the retransmission should be considered
 * @own_packet: true if it is a self-generated ogm
 * @send_time: timestamp (jiffies) when the packet is to be sent
 */
void BatmanADVMain::batadv_iv_ogm_queue_add(batadv_ogm_packet *ogm_packet, long packet_len, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, bool own_packet, forwardReason reason, double send_time) //unsigned long send_time)
{
    batadv_forw_packet forw_packet_pos;
    bool direct_link = false;
    unsigned long max_aggregation_jiffies = 0;
    bool found = false;
    //printf("Node: %s ogm_packet->getFlags() %d-- BATADV_DIRECTLINK %d\n",nodenr.c_str(),ogm_packet->getFlags(),BATADV_DIRECTLINK);
    if ((ogm_packet->getFlags() & BATADV_DIRECTLINK) == BATADV_DIRECTLINK)
        direct_link = true;

    max_aggregation_jiffies = 100; // BATADV_MAX_AGGREGATION_MS;

    /* own packets are not to be aggregated */
    if (ogm_aggregation && !own_packet)
    {
        for (unsigned int i = 0; i < forw_bat_list.size(); i++)    //We need at least one package to aggregate it -> Otherwise the first one is created
        {
            forw_packet_pos = forw_bat_list.at(i);
            if (batadv_iv_ogm_can_aggregate(ogm_packet, packet_len, send_time, direct_link, if_incoming, if_outgoing, forw_packet_pos))    //There is already a package where we could put our stuff to
            {
                if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
                    printf("Can aggregate packet - node %s - time %f\n", nodenr.c_str(), simTime().dbl());
                EV << "Node: " << nodenr << ": Aggregation possible" << std::endl;
                //printf("Node: %s Aggregation possible\n", nodenr.c_str(), send_time, if_incoming.name, if_outgoing.name);
                batadv_iv_ogm_aggregate(forw_bat_list.at(i), ogm_packet, packet_len, direct_link);
                found = true;
                //delete ogm_packet;
                return;
            }
        }
    }
    /* nothing to aggregate with - either aggregation disabled or no
     * suitable aggregation packet found
     */
    if (!found)
    {
        /* if we could not aggregate this packet with one of the others
         * we hold it back for a while, so that it might be aggregated
         * later on
         */
        //printf("Node: %s Aggregation not possible\n", nodenr.c_str(), send_time, if_incoming.name, if_outgoing.name);
        if (!own_packet && ogm_aggregation)
            send_time += max_aggregation_jiffies;
        batadv_iv_ogm_aggregate_new(ogm_packet, packet_len, send_time, direct_link, if_incoming, if_outgoing, own_packet, reason);
        //delete ogm_packet;
        return;
    }
    throw new cException(this, "This is not possible");
}

void BatmanADVMain::batadv_iv_ogm_queue_addNoOutgoing(batadv_ogm_packet *ogm_packet, long packet_len, batadv_hard_iface if_incoming, bool own_packet, forwardReason reason, double send_time)
{
    batadv_forw_packet forw_packet_pos;
    bool direct_link = false;
    unsigned long max_aggregation_jiffies = 0;
    bool found = false;
    //printf("Node: %s ogm_packet->getFlags() %d-- BATADV_DIRECTLINK %d\n",nodenr.c_str(),ogm_packet->getFlags(),BATADV_DIRECTLINK);
    if ((ogm_packet->getFlags() & BATADV_DIRECTLINK) == BATADV_DIRECTLINK)
        direct_link = true;

    max_aggregation_jiffies = 100; // BATADV_MAX_AGGREGATION_MS;
    /* own packets are not to be aggregated */
    if (ogm_aggregation && !own_packet)
    {
        for (unsigned int i = 0; i < forw_bat_list.size(); i++)    //We need at least one package to aggregate it -> Otherwise the first one is created
        {
            forw_packet_pos = forw_bat_list.at(i);
            if (batadv_iv_ogm_can_aggregateNoOutgoing(ogm_packet, packet_len, send_time, direct_link, if_incoming, forw_packet_pos))    //There is already a package where we could put our stuff to
            {
                EV << "Node: " << nodenr << ": Aggregation possible" << std::endl;
                batadv_iv_ogm_aggregate(forw_bat_list.at(i), ogm_packet, packet_len, direct_link);
                found = true;
                //delete ogm_packet;
                return;
            }
        }
    }
    /* nothing to aggregate with - either aggregation disabled or no
     * suitable aggregation packet found
     */
    if (!found)
    {
        /* if we could not aggregate this packet with one of the others
         * we hold it back for a while, so that it might be aggregated
         * later on
         */
        //printf("Node: %s Aggregation not possible\n", nodenr.c_str(), send_time, if_incoming.name, if_outgoing.name);
        if (!own_packet && ogm_aggregation)
            send_time += max_aggregation_jiffies;
        batadv_iv_ogm_aggregate_newNoOutgoing(ogm_packet, packet_len, send_time, direct_link, if_incoming, own_packet, reason);
        //delete ogm_packet;
        return;
    }
    throw new cException(this, "This is not possible");
}
/**
 * batadv_iv_ogm_can_aggregate - find out if an OGM can be aggregated on an
 *  existing forward packet
 * @new_bat_ogm_packet: OGM packet to be aggregated
 * @bat_priv: the bat priv with all the soft interface information
 * @packet_len: (total) length of the OGM
 * @send_time: timestamp (jiffies) when the packet is to be sent
 * @directlink: true if this is a direct link packet
 * @if_incoming: interface where the packet was received
 * @if_outgoing: interface for which the retransmission should be considered
 * @forw_packet: the forwarded packet which should be checked
 *
 * Returns true if new_packet can be aggregated with forw_packet
 */
bool BatmanADVMain::batadv_iv_ogm_can_aggregate(batadv_ogm_packet *new_bat_ogm_packet, int packet_len, double send_time, bool directlink, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, batadv_forw_packet forw_packet)
{
    long aggregated_bytes = forw_packet.packet_len + packet_len;

    unsigned long aggregation_end_time = 0;
    aggregation_end_time = send_time;
    aggregation_end_time += BATADV_MAX_AGGREGATION_MS;
    //simtime_t s = (simtime_t) aggregation_end_time / 1000.0 + simTime();
    //aggregation_end_time = s.dbl()*1000;
    batadv_ogm_packetClass ogm_packet = forw_packet.skb[forw_packet.skb.size() - 1];
    //send_time = send_time/ 1000.0 + simTime().dbl();
    //printf("Node: %s - Aggregation - send time %f - ifacenr incoming %d - ifacenr outgoing %d \n", nodenr.c_str(), send_time, if_incoming.name, if_outgoing.name);
    /* we can aggregate the current packet to this aggregated packet
     * if:
     *
     * - the send time is within our MAX_AGGREGATION_MS time
     * - the resulting packet wont be bigger than
     *   MAX_AGGREGATION_BYTES
     */
    bool isTimeBefore = time_before(send_time, forw_packet.send_time);

    if (aggregated_bytes > BATADV_MAX_AGGREGATION_BYTES)
    {
        EV << "Node: " << nodenr << ": Cannot aggragate as the packet gets to big" << std::endl;
        return false;
    }

    /* packet is not leaving on the same interface. */
    if (forw_packet.name_if_outgoing != if_outgoing.name)
    {
        EV << "Node: " << nodenr << ": Cannot aggregate as packet is not leaving on the same interface" << std::endl;
        return false;
    }

    if (!isTimeBefore || aggregation_end_time < forw_packet.send_time)
    {
        EV << "Node: " << nodenr << ": Cannot aggregate as sent times of both packets differ too much" << std::endl;
        return false;
    }
    if (forw_packet.skb.size() == 19)
    {
        EV << "Node: " << nodenr << ": Cannot aggragate as the skb size is already 17" << std::endl;
        return false;
    }
    /* check aggregation compatibility
     * -> direct link packets are broadcasted on
     *    their interface only
     * -> aggregate packet if the current packet is
     *    a "global" packet as well as the base
     *    packet
     *  packets without direct link flag and high TTL
     * are flooded through the net
     */
    if (!directlink && !(ogm_packet.flags & BATADV_DIRECTLINK == BATADV_DIRECTLINK) && ogm_packet.ttl != 1
    /* own packets originating non-primary
     * interfaces leave only that interface
     */
    && (!forw_packet.own || (forw_packet.if_incoming.name == primary_ifNP.name)))
    {
        EV << "Node: " << nodenr << ": Can aggregate - !directLink, OGMDirectLink, ttl>1, !own, incoming=PrimaryIF " << std::endl;
        return true;
    }

    /* if the incoming packet is sent via this one
     * interface only - we still can aggregate
     */
    if ((directlink) && (new_bat_ogm_packet->getTtl() == 1) && (forw_packet.if_incoming.name == if_incoming.name) &&
    /* packets from direct neighbors or
     * own secondary interface packets
     * (= secondary interface packets in general)
     */
    (((ogm_packet.flags & BATADV_DIRECTLINK) == BATADV_DIRECTLINK) || (forw_packet.own && (forw_packet.if_incoming.name != primary_ifNP.name))))
    {
        EV << "Node: " << nodenr << ": Can aggregate - directLink,, ttl=1, own, incoming!=PrimaryIF " << std::endl;
        return true;
    }
    EV << "Node: " << nodenr << ": Cannot aggregate - directLink,, ttl=1, own, incoming!=PrimaryIF " << std::endl;
    return false;
}

bool BatmanADVMain::batadv_iv_ogm_can_aggregateNoOutgoing(batadv_ogm_packet *new_bat_ogm_packet, int packet_len, double send_time, bool directlink, batadv_hard_iface if_incoming, batadv_forw_packet forw_packet)
{
    long aggregated_bytes = forw_packet.packet_len + packet_len;

    unsigned long aggregation_end_time = 0;
    aggregation_end_time = send_time;
    aggregation_end_time += BATADV_MAX_AGGREGATION_MS;
    batadv_ogm_packetClass ogm_packet = forw_packet.skb[forw_packet.skb.size() - 1];
    /* we can aggregate the current packet to this aggregated packet
     * if:
     *
     * - the send time is within our MAX_AGGREGATION_MS time
     * - the resulting packet wont be bigger than
     *   MAX_AGGREGATION_BYTES
     */
    bool isTimeBefore = time_before(send_time, forw_packet.send_time);

    if (aggregated_bytes > BATADV_MAX_AGGREGATION_BYTES)
    {
        EV << "Node: " << nodenr << ": Cannot aggregate as the packet gets to big" << std::endl;
        return false;
    }

    if (!isTimeBefore || aggregation_end_time < forw_packet.send_time)
    {
        EV << "Node: " << nodenr << ": Cannot aggregate as sent times of both packets differ too much" << std::endl;
        return false;
    }
    if (forw_packet.skb.size() == 19)
    {
        EV << "Node: " << nodenr << ": Cannot aggregate as the skb size is already 17" << std::endl;
        return false;
    }
    /* check aggregation compatibility
     * -> direct link packets are broadcasted on
     *    their interface only
     * -> aggregate packet if the current packet is
     *    a "global" packet as well as the base
     *    packet
     *  packets without direct link flag and high TTL
     * are flooded through the net
     */
    if (!directlink && !((ogm_packet.flags & BATADV_DIRECTLINK) == BATADV_DIRECTLINK) && ogm_packet.ttl != 1
    /* own packets originating non-primary
     * interfaces leave only that interface
     */
    && (!forw_packet.own || (forw_packet.if_incoming.name == primary_ifNP.name)))
    {
        EV << "Node: " << nodenr << ": Can aggregate - !directLink, OGMDirectLink, ttl>1, !own, incoming=PrimaryIF " << std::endl;
        return true;
    }

    /* if the incoming packet is sent via this one
     * interface only - we still can aggregate
     */
    if ((directlink) && (new_bat_ogm_packet->getTtl() == 1) && (forw_packet.if_incoming.name == if_incoming.name) &&
    /* packets from direct neighbors or
     * own secondary interface packets
     * (= secondary interface packets in general)
     */
    (((ogm_packet.flags & BATADV_DIRECTLINK) == BATADV_DIRECTLINK) || (forw_packet.own && (forw_packet.if_incoming.name != primary_ifNP.name))))
    {
        EV << "Node: " << nodenr << ": Can aggregate - directLink,, ttl=1, own, incoming!=PrimaryIF " << std::endl;
        return true;
    }
    EV << "Node: " << nodenr << ": Cannot aggregate - directLink,, ttl=1, own, incoming!=PrimaryIF " << std::endl;
    return false;
}

void BatmanADVMain::batadv_iv_ogm_aggregate(batadv_forw_packet& forw_packet_aggr, batadv_ogm_packet *ogm, long packet_len, bool direct_link)
{
    EV << "Node: " << nodenr << ": batadv_iv_ogm_aggregate called" << std::endl;

    bool new_direct_link_flag;
    forw_packet_aggr.packet_len += packet_len;
    forw_packet_aggr.num_packets++;
    forw_packet_aggr.direct_link_flags = BATADV_NO_FLAGS;
    forw_packet_aggr.skb.push_back(convertFromOGM(ogm));
    EV << "Node: " << nodenr << ": batadv_iv_ogm_aggregate -- Created a new batadv_forw_packet entry and skb size is now " << forw_packet_aggr.skb.size() << std::endl;

    /* save packet direct link flag status */
    if (direct_link)
    {
        new_direct_link_flag = getBitForNumber(forw_packet_aggr.num_packets);
        forw_packet_aggr.direct_link_flags |= new_direct_link_flag;
    }

}

bool BatmanADVMain::batadv_iv_ogm_aggr_packet()
{

}
/**
 * batadv_iv_ogm_process - process an incoming batman iv OGM
 * @skb: the skb containing the OGM
 * @ogm_offset: offset to the OGM which should be processed (for aggregates)
 * @if_incoming: the interface where this packet was received
 */
void BatmanADVMain::batadv_iv_ogm_process(batadv_hard_iface if_incoming)
{
    batadv_orig_node *orig_neigh_node = NULL;
    //int if_incoming_seqno;
    bool has_directlink_flag = false;
    bool is_my_oldorig = false;
    bool is_my_addr = false;
    bool is_single_hop_neigh = false;
    bool is_my_orig = false;
    EtherFrame* frame = getCurrentFrame();
    cPacket* higherlayermsg = frame->decapsulate();
    batadv_ogm_packet* ogm_packet = check_and_cast<batadv_ogm_packet*>(higherlayermsg);

    batadv_hard_iface hard_iface;
    MACAddress frameSRC = frame->getSrc();
    enum batadv_dup_status dup_status;

    delete frame;
    hard_iface = if_incoming;

    if (ogm_packet->getVersion() != BATADV_COMPAT_VERSION)
    {
        delete ogm_packet;
        EV << "Node: " << nodenr << ": invalid version detected -> Dropped" << std::endl;
        return;
    }
    double si = simTime().dbl();
    if (iNodenr == -3 && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
    {
        printf("Node %s sees the new node %s at %f\n", nodenr.c_str(), ogm_packet->getOrig().getMAC().str().c_str(), si);
    }
//Read sequence number from arrived interface // --> Needed for that bitwindow thing in real code -> Here it is not needed
    //if_incoming_seqno = ogm_packet->getSeqno();

    if ((ogm_packet->getFlags() & BATADV_DIRECTLINK) == BATADV_DIRECTLINK)
        has_directlink_flag = true;
    else
        has_directlink_flag = false;

    if (batadv_compare_eth(frameSRC, ogm_packet->getOrig().getMAC()) == 0)
        is_single_hop_neigh = true;

    if (hard_iface.if_status != BATADV_IF_ACTIVE)
    {
        EV << "Node: " << nodenr << ": This interface isn't active" << std::endl;
        delete ogm_packet;
        return;
    }
    if (batadv_compare_eth(frameSRC, ownAddress.getMAC()) == 0)
    {
        is_my_addr = true;
        EV << "Node: " << nodenr << ": It's my OGM." << std::endl;
    }
    if (batadv_compare_eth(ogm_packet->getPrev_sender().getMAC(), ownAddress.getMAC()) == 0)
    {
        EV << "Node: " << nodenr << ": It's my old OGM" << std::endl;
        is_my_oldorig = true;
    }
    if (batadv_compare_eth(ogm_packet->getOrig().getMAC(), ownAddress.getMAC()) == 0)
    {
        EV << "Node: " << nodenr << ": I am the originator of this OGM." << std::endl;
        is_my_orig = true;
    }
    if (is_my_addr)
    {
        EV << "Node: " << nodenr << ": Drop packet: received my own broadcast" << std::endl;
        delete ogm_packet;
        return;
    }
    if (is_my_orig)    //Check for direct links
    {
        /*unsigned long *word;
         int offset;
         int32_t bit_pos;
         int16_t if_num;
         uint8_t *weight;*/
        //orig_neigh_node = batadv_iv_ogm_orig_get(ogm_packet);
        orig_neigh_node = batadv_iv_ogm_orig_get_by_mac(frameSRC);
        if (orig_neigh_node == NULL)
        {
            delete ogm_packet;
            return;
        }

        /* neighbor has to indicate direct link and it has to
         * come via the corresponding interface
         * save packet seqno for bidirectional check
         */
        if (has_directlink_flag && batadv_compare_eth(ownAddress.getMAC(), ogm_packet->getOrig().getMAC()) == 0)
        {
            EV << "Node: " << nodenr << ":  I have a direct link to " << ogm_packet->getOrig().getMAC() << "." << std::endl;
            orig_neigh_node->bat_iv.bcast_own_sum += 1;
            if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
                printf("Setting eq-count %d - node %s\n", orig_neigh_node->bat_iv.bcast_own_sum, nodenr.c_str());
        }
        EV << "Node: " << nodenr << ": Drop packet: originator packet from myself (via neighbor)" << std::endl;
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf(" Drop packet:  originator packet from myself - node %s \n", nodenr.c_str());

        delete ogm_packet;
        return;
    }

    if (is_my_oldorig)
    {
        EV << "Drop packet: ignoring all rebroadcast echos" << std::endl;
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf(" Drop packet: echo from %s - node %s\n", ogm_packet->getPrev_sender().getMAC().str().c_str(), nodenr.c_str());
        delete ogm_packet;
        return;
    }
    if ((ogm_packet->getFlags() & BATADV_NOT_BEST_NEXT_HOP) == BATADV_NOT_BEST_NEXT_HOP)
    {
        EV << "Node: " << nodenr << ": Drop packet: ignoring all packets not forwarded from the best next hop" << std::endl;
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf(" Drop packet: ignoring all packets not forwarded from the best next hop - node %s\n", nodenr.c_str());

        delete ogm_packet;
        return;
    }
    batadv_orig_node* orig_node = batadv_iv_ogm_orig_get(ogm_packet);
    if (!orig_node)
    {
        delete ogm_packet;
        return;
    }
    //////////////////////////////////////////////////////////
    batadv_neigh_node *router = NULL, *router_router = NULL, *orig_neigh_router = NULL;
    batadv_neigh_ifinfo *router_ifinfo = NULL;
    batadv_orig_ifinfo *orig_ifinfo = NULL;
    bool is_from_best_next_hop = false;
    bool is_bidirect = false;
    bool sameseq = false, similar_ttl = false;

    dup_status = batadv_iv_ogm_update_seqnosIfChange(&frameSRC, ogm_packet, if_incoming);
    if (dup_status == BATADV_PROTECTED)
    {
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf("Drop packet - protection time- node %s - time %f\n", nodenr.c_str(), simTime().dbl());

        EV << "Node: " << nodenr << ": Drop packet: packet within seqno protection time" << std::endl;
        delete ogm_packet;
        return;
    }
    if (ogm_packet->getTq() == 0)
    {

        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf("Drop packet - TQ==0 from %s - node %s - time %f\n", ogm_packet->getOrig().getMAC().str().c_str(), nodenr.c_str(), simTime().dbl());

        EV << "Node: " << nodenr << ": Drop packet: originator packet with tq equal 0" << std::endl;
        delete ogm_packet;
        return;
    }
    router = batadv_orig_router_getNoOutgoing(orig_node);
    if (router != NULL)
    {
        router_router = batadv_orig_router_getNoOutgoing(router->orig_node);
        EV << "Node: " << nodenr << ": router->orig_node: " << router->orig_node->orig.getMAC() << std::endl;

        EV << "Node: " << nodenr << ": router_router->addr: " << router_router->addr.getMAC() << std::endl;
        EV << "Node: " << nodenr << ": Trying to get ifinfo for router" << router->addr.getMAC() << std::endl;
        router_ifinfo = batadv_neigh_ifinfo_getNoOutgoing(router);
    }
    if (router == NULL)
        EV << "Node: " << nodenr << ": -------------router is NULL" << std::endl;
    if (!router_ifinfo)
        EV << "Node: " << nodenr << ": -------------router_ifinfo is NULL" << std::endl;
    if (!router_router)
        EV << "Node: " << nodenr << ": -------------router_router is NULL" << std::endl;
    if (router_ifinfo != NULL && router_ifinfo->bat_iv.tq_avg != 0)
    {
        ManetAddress routerManet = router->addr;
        MACAddress routerMac = routerManet.getMAC();
        if ((batadv_compare_eth(routerMac, frameSRC) == 0))        //router_ifinfo && router_ifinfo->bat_iv.tq_avg != 0 -> Line 1429 in bat_iv_ogm.c
        {
            EV << "Node: " << nodenr << ": Have a next best hop" << std::endl;
            is_from_best_next_hop = true;
        }
    }

    /* avoid temporary routing loops */
    if (router && router_router && (batadv_compare_eth(router->addr.getMAC(), ogm_packet->getPrev_sender().getMAC()) == 0) && !(batadv_compare_eth(ogm_packet->getOrig().getMAC(), ogm_packet->getPrev_sender().getMAC()) == 0) && (batadv_compare_eth(router->addr.getMAC(), router_router->addr.getMAC()) == 0))
    {
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf("Drop packet - loop - node %s - time %f\n", nodenr.c_str(), simTime().dbl());

        EV << "Node: " << nodenr << ": Drop packet: ignoring all rebroadcast packets that may make me loop" << std::endl;
        delete ogm_packet;
        return;
    }
    /* if sender is a direct neighbor the sender mac (frame) equals
     * originator mac
     */

    if (is_single_hop_neigh)
        orig_neigh_node = orig_node;
    else
        orig_neigh_node = batadv_iv_ogm_orig_get_by_mac(frameSRC);

    if (!orig_neigh_node)
    {
        delete ogm_packet;
        return;
    }
    orig_neigh_router = batadv_orig_router_getNoOutgoing(orig_neigh_node);
    ///////////////////////////////////TODO: Dangerous!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /*if (!is_single_hop_neigh && (!orig_neigh_router))
     {
     EV << "Node: " << nodenr << ": Drop packet: OGM via unknown neighbor! -> " << orig_neigh_node->orig.getMAC() << std::endl;
     delete ogm_packet;
     //printf("Node: %s -- Drop packet: OGM via unknown neighbor! %s \n", nodenr.c_str(), orig_neigh_node->orig.getMAC().str().c_str());
     return;
     }*/
    ///////////////////////////////////TODO: Dangerous!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    is_bidirect = batadv_iv_ogm_calc_tqNoOutgoing(orig_node, orig_neigh_node, ogm_packet, if_incoming);

    orig_ifinfo = batadv_orig_ifinfo_newNoOutgoing(orig_node, ogm_packet->getPrev_sender());
    if (!orig_ifinfo)
    {
        delete ogm_packet;
        return;
    }
    sameseq = orig_ifinfo->last_real_seqno == ogm_packet->getSeqno();
    similar_ttl = (orig_ifinfo->last_ttl - 3) <= ogm_packet->getTtl();
    if (is_bidirect && ((dup_status == BATADV_NO_DUP) || (sameseq && similar_ttl)))
    {
        EV << "Node: " << nodenr << ": Calling batadv_iv_ogm_orig_update from batadv_iv_ogm_process_per_outif" << std::endl;
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf("Update orig! - node %s - time %f\n", nodenr.c_str(), simTime().dbl());

        batadv_iv_ogm_orig_updateNoOutgoing(orig_node, orig_ifinfo, frameSRC, ogm_packet, if_incoming, dup_status);
    }
    else
    {
        EV << "Node: " << nodenr << ": No orig_update - sameseq: " << sameseq << ", similar_ttl: " << similar_ttl << ", dupstatus: " << dup_status << ::std::endl;
        EV << "Node: " << nodenr << ": orig_ifinfo->last_real_seqno: " << orig_ifinfo->last_real_seqno << ", orig_ifinfo->last_ttl - 3: " << orig_ifinfo->last_ttl - 3 << ", ogm_packet->getTtl(): " << ogm_packet->getTtl() << ::std::endl;
    }
    /* router = batadv_orig_router_getNoOutgoing(orig_node);
     if (router != NULL)
     {
     if ((batadv_compare_eth(router->addr.getMAC(), frameSRC) == 0))        //router_ifinfo && router_ifinfo->bat_iv.tq_avg != 0 -> Line 1429 in bat_iv_ogm.c
     {
     EV << "Node: " << nodenr << ": Have a next best hop" << std::endl;
     is_from_best_next_hop = true;
     }
     }*/

    /* is single hop (direct) neighbor */
    if (is_single_hop_neigh)
    {
        /* mark direct link on incoming interface */
        EV << "Node: " << nodenr << ": Forwarding packet: rebroadcast neighbor packet with direct link flag\n" << std::endl;
        batadv_iv_ogm_forwardNoOutgoing(orig_node, &frameSRC, ogm_packet, is_single_hop_neigh, is_from_best_next_hop, if_incoming, FORWARD_OGM);
        return;
    }
    if (!is_bidirect)
    {
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf("Drop packet - TQ==0 - no bidirectional link and not direct neighbors - node %s - time %f\n", nodenr.c_str(), simTime().dbl());

        EV << "Node: " << nodenr << ": Drop packet: not received via bidirectional link\n" << std::endl;
        delete ogm_packet;
        return;
    }

    if (dup_status == BATADV_NEIGH_DUP)
    {
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf("Drop packet - Duplicate - node %s - time %f\n", nodenr.c_str(), simTime().dbl());

        EV << "Node: " << nodenr << ": Drop packet: duplicate packet received\n" << std::endl;
        delete ogm_packet;
        return;
    }

    EV << "Node: " << nodenr << ": Forwarding packet: rebroadcast neighbor packet\n" << std::endl;
    batadv_iv_ogm_forwardNoOutgoing(orig_node, &frameSRC, ogm_packet, is_single_hop_neigh, is_from_best_next_hop, if_incoming, FORWARD_OGM);

    //batadv_iv_ogm_process_per_outif(orig_node, if_incoming, BATADV_IF_DEFAULTNP);

    /*if (gw_mode != BATADV_GW_MODE_SERVER)
     {
     EV << "Node: " << nodenr << ": ----------------- Processing OGM on interface wlan -----------------" << std::endl;
     batadv_iv_ogm_process_per_outif(orig_node, if_incoming, wlan0NP);
     }
     EV << "Node: " << nodenr << ": ----------------- Processing OGM on interface meshvpn -----------------" << std::endl;
     batadv_iv_ogm_process_per_outif(orig_node, if_incoming, meshvpnNP);*/

    /*if (BATADV_IF_DEFAULTNP.name == wlan0NP.name)
     {
     batadv_iv_ogm_process_per_outif(orig_node, if_incoming, wlan0NP);
     }
     else
     {
     batadv_iv_ogm_process_per_outif(orig_node, if_incoming, meshvpnNP);
     }*/
    return;

}

/**
 * batadv_iv_ogm_process_per_outif - process a batman iv OGM for an outgoing if
 * @ogm_offset: offset from skb->data to start of ogm header
 * @orig_node: the (cached) orig node for the originator of this OGM
 * @if_incoming: the interface where this packet was received
 * @if_outgoing: the interface for which the packet should be considered
 */
void BatmanADVMain::batadv_iv_ogm_process_per_outif(batadv_orig_node *orig_node, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing)
{
    batadv_orig_node *orig_neigh_node = NULL;
    EtherFrame* f = getCurrentFrame();
    cPacket* higherlayermsg = f->decapsulate();
    batadv_ogm_packet* ogm_packet = check_and_cast<batadv_ogm_packet*>(higherlayermsg);
    MACAddress frameMac = f->getSrc();
    batadv_neigh_node *router = NULL, *router_router = NULL, *orig_neigh_router = NULL;
    batadv_neigh_ifinfo *router_ifinfo = NULL;
    batadv_orig_ifinfo *orig_ifinfo = NULL;
    delete f;

    bool is_single_hop_neigh = false;
    bool is_from_best_next_hop = false;
    bool is_bidirect = false;
    bool sameseq = false, similar_ttl = false;
//ManetAddress prev_sender = ogm_packet->getPrev_sender();
    batadv_dup_status dup_status;
    dup_status = batadv_iv_ogm_update_seqnos(ogm_packet, if_incoming, if_outgoing);
    if (batadv_compare_eth(frameMac, ogm_packet->getOrig().getMAC()) == 0)    //The sender of this frame is the previous sender of the ogm
    {
        is_single_hop_neigh = true;
        // printf("Node: %s -- is_single_hop_neigh  %d -- %s\n", nodenr.c_str(), is_single_hop_neigh, ogm_packet->getOrig().getMAC().str().c_str());
    }
    else
    {
        //printf("Node: %s --NOT  is_single_hop_neigh  %d --  %s\n", nodenr.c_str(), is_single_hop_neigh, ogm_packet->getOrig().getMAC().str().c_str());

    }
    if (dup_status == BATADV_PROTECTED)
    {
        EV << "Node: " << nodenr << ": Drop packet: packet within seqno protection time" << std::endl;
        delete ogm_packet;
        return;
    }
    if (ogm_packet->getTq() == 0)
    {
        EV << "Node: " << nodenr << ": Drop packet: originator packet with tq equal 0" << std::endl;
        delete ogm_packet;
        return;
    }

    router = batadv_orig_router_get(orig_node, &if_outgoing);

    if (router != NULL)
    {
        router_router = batadv_orig_router_get(router->orig_node, &if_outgoing);
        EV << "Node: " << nodenr << ": router->orig_node: " << router->orig_node->orig.getMAC() << std::endl;

        EV << "Node: " << nodenr << ": router_router->addr: " << router_router->addr.getMAC() << std::endl;
        EV << "Node: " << nodenr << ": Trying to get ifinfo for router" << router->addr.getMAC() << std::endl;
        router_ifinfo = batadv_neigh_ifinfo_get(router, &if_outgoing);
    }
    if (router == NULL)
        EV << "Node: " << nodenr << ": -------------router is NULL" << std::endl;
    if (!router_ifinfo)
        EV << "Node: " << nodenr << ": -------------router_ifinfo is NULL" << std::endl;
    if (!router_router)
        EV << "Node: " << nodenr << ": -------------router_router is NULL" << std::endl;
    if (router_ifinfo != NULL && router_ifinfo->bat_iv.tq_avg != 0)
    {
        ManetAddress routerManet = router->addr;
        MACAddress routerMac = routerManet.getMAC();
        if ((batadv_compare_eth(routerMac, frameMac) == 0))        //router_ifinfo && router_ifinfo->bat_iv.tq_avg != 0 -> Line 1429 in bat_iv_ogm.c
        {
            EV << "Node: " << nodenr << ": Have a next best hop" << std::endl;
            is_from_best_next_hop = true;
        }
    }

    /* avoid temporary routing loops */
//TODO: This statement may avoid loops
    if (router && router_router && (batadv_compare_eth(router->addr.getMAC(), ogm_packet->getPrev_sender().getMAC()) == 0) && !(batadv_compare_eth(ogm_packet->getOrig().getMAC(), ogm_packet->getPrev_sender().getMAC()) == 0) && (batadv_compare_eth(router->addr.getMAC(), router_router->addr.getMAC()) == 0))
    {
        EV << "Node: " << nodenr << ": Drop packet: ignoring all rebroadcast packets that may make me loop" << std::endl;
        delete ogm_packet;
        return;
    }

//THIS IS DONE AT ANOTHER POINT IN THE CODE
    /* if (if_incoming == BATADV_IF_DEFAULT)
     batadv_tvlv_ogm_receive(ogm_packet, orig_node);
     */

    /* if sender is a direct neighbor the sender mac (frame) equals
     * originator mac
     */

    if (is_single_hop_neigh)
        orig_neigh_node = orig_node;
    else
        orig_neigh_node = batadv_iv_ogm_orig_get_by_mac(frameMac);

    if (!orig_neigh_node)
    {
        delete ogm_packet;
        return;
    }
    //Should be NULL at the first iteration?
    orig_neigh_router = batadv_orig_router_get(orig_neigh_node, &if_outgoing);
    /* drop packet if sender is not a direct neighbor and if we
     * don't have a route towards it
     */
    ///////////////////////////////////TODO: Dangerous!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /*if (!is_single_hop_neigh && (!orig_neigh_router))
     {
     EV << "Node: " << nodenr << ": Drop packet: OGM via unknown neighbor! -> " << orig_neigh_node->orig.getMAC() << std::endl;
     delete ogm_packet;
     //printf("Node: %s -- Drop packet: OGM via unknown neighbor! %s \n", nodenr.c_str(), orig_neigh_node->orig.getMAC().str().c_str());
     return;
     }*/
    ///////////////////////////////////TODO: Dangerous!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    is_bidirect = batadv_iv_ogm_calc_tq(orig_node, orig_neigh_node, ogm_packet, if_incoming, if_outgoing);

    /* update ranking if it is not a duplicate or has the same
     * seqno and similar ttl as the non-duplicate
     */
    orig_ifinfo = batadv_orig_ifinfo_new(orig_node, ogm_packet->getPrev_sender(), &if_outgoing);
    if (!orig_ifinfo)
    {
        delete ogm_packet;
        return;
    }
    sameseq = orig_ifinfo->last_real_seqno == ogm_packet->getSeqno();
    similar_ttl = (orig_ifinfo->last_ttl - 3) <= ogm_packet->getTtl();
    if (is_bidirect && ((dup_status == BATADV_NO_DUP) || (sameseq && similar_ttl)))
    {
        EV << "Node: " << nodenr << ": Calling batadv_iv_ogm_orig_update from batadv_iv_ogm_process_per_outif" << std::endl;
        batadv_iv_ogm_orig_update(orig_node, orig_ifinfo, frameMac, ogm_packet, if_incoming, if_outgoing, dup_status);
    }
    else
    {
        EV << "Node: " << nodenr << ": No orig_update - sameseq: " << sameseq << ", similar_ttl: " << similar_ttl << ", dupstatus: " << dup_status << ::std::endl;
        EV << "Node: " << nodenr << ": orig_ifinfo->last_real_seqno: " << orig_ifinfo->last_real_seqno << ", orig_ifinfo->last_ttl - 3: " << orig_ifinfo->last_ttl - 3 << ", ogm_packet->getTtl(): " << ogm_packet->getTtl() << ::std::endl;
    }
    /* only forward for specific interface, not for the default one. */
    /*if (if_outgoing == BATADV_IF_DEFAULT)//Test
     {
     EV << "Node: " << nodenr << ": if_outgoing != BATADV_IF_DEFAULT in batadv_iv_ogm_process_per_outif -- drop" << std::endl;
     delete ogm_packet;
     return;
     }*/

    /* is single hop (direct) neighbor */
    if (is_single_hop_neigh)
    {
        /* OGMs from secondary interfaces should only scheduled once
         * per interface where it has been received, not multiple times
         */
        if ((ogm_packet->getTtl() <= 2) && (if_incoming.name != if_outgoing.name))
        {
            EV << "Node: " << nodenr << ": Drop packet: OGM from secondary interface and wrong outgoing interface\n" << std::endl;
            //printf("Node: %s -- Drop packet: OGM from secondary interface and wrong outgoing interface - %s\n", nodenr.c_str(), ogm_packet->getOrig().getMAC().str().c_str());
            delete ogm_packet;
            return;
        }
        /* mark direct link on incoming interface */
        EV << "Node: " << nodenr << ": Forwarding packet: rebroadcast neighbor packet with direct link flag\n" << std::endl;
        batadv_iv_ogm_forward(orig_node, &frameMac, ogm_packet, is_single_hop_neigh, is_from_best_next_hop, if_incoming, if_outgoing, FORWARD_OGM);
        return;
    }

    /* multihop originator */
    if (!is_bidirect)
    {
        EV << "Node: " << nodenr << ": Drop packet: not received via bidirectional link\n" << std::endl;
        delete ogm_packet;
        return;
    }

    if (dup_status == BATADV_NEIGH_DUP)
    {
        EV << "Node: " << nodenr << ": Drop packet: duplicate packet received\n" << std::endl;
        delete ogm_packet;
        return;
    }

    EV << "Node: " << nodenr << ": Forwarding packet: rebroadcast neighbor packet\n" << std::endl;
    batadv_iv_ogm_forward(orig_node, &frameMac, ogm_packet, is_single_hop_neigh, is_from_best_next_hop, if_incoming, if_outgoing, FORWARD_OGM);
    return;
}

void BatmanADVMain::batadv_iv_ogm_forward(batadv_orig_node *orig_node, MACAddress *ethhdr, batadv_ogm_packet *batadv_ogm_packet, bool is_single_hop_neigh, bool is_from_best_next_hop, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, forwardReason reason)
{
    long tvlv_len;
    if (batadv_ogm_packet->getTtl() <= 1)
    {
        EV << "Node: " << nodenr << ": ttl exceeded" << std::endl;
        delete batadv_ogm_packet;
        return;
    }

    if (!is_from_best_next_hop)
    {
        EV << "Node: " << nodenr << ": OGM is not from next best hop - origMac: " << batadv_ogm_packet->getOrig().getMAC().str() << std::endl;
        /* Mark the forwarded packet when it is not coming from our
         * best next hop. We still need to forward the packet for our
         * neighbor link quality detection to work in case the packet
         * originated from a single hop neighbor. Otherwise we can
         * simply drop the ogm.
         */
        if (is_single_hop_neigh)
        {

            batadv_ogm_packet->setFlags(batadv_ogm_packet->getFlags() | BATADV_NOT_BEST_NEXT_HOP);
            EV << "Node: " << nodenr << ": OGM is a is_single_hop_neigh but not a best next hop: " << batadv_ogm_packet->getOrig().getMAC().str() << std::endl;
        }
        else
        {
            EV << "Node: " << nodenr << ": OGM is not a is_single_hop_neigh and not a best next hop: " << batadv_ogm_packet->getOrig().getMAC().str() << std::endl;

            if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(batadv_ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
                printf("Drop packet - OGM is not a is_single_hop_neigh and not a best next hop - node %s - time %f\n", nodenr.c_str(), simTime().dbl());

            delete batadv_ogm_packet;
            return;
        }
    }

    batadv_ogm_packet->setTtl(batadv_ogm_packet->getTtl() - 1);
    batadv_ogm_packet->setPrev_sender(ManetAddress(MACAddress(ethhdr->getInt())));
//batadv_ogm_packet->setPrev_sender(ownAddress);

    /* apply hop penalty */
    batadv_ogm_packet->setTq(batadv_hop_penalty(batadv_ogm_packet->getTq()));

    EV << "Node: " << nodenr << ": Forwarding packet: tq: " << batadv_ogm_packet->getTq() << ", ttl: " << batadv_ogm_packet->getTtl() << std::endl;
    /* switch of primaries first hop flag when forwarding */
    //batadv_ogm_packet->setFlags(batadv_ogm_packet->getFlags() & ~BATADV_PRIMARIES_FIRST_HOP);
    //printf("Node: %s -- directlink  %d && ogm_packet->getFlags() %d  \n",nodenr.c_str(),is_single_hop_neigh,batadv_ogm_packet->getFlags());
    if (is_single_hop_neigh)
    {
        batadv_ogm_packet->setFlags(batadv_ogm_packet->getFlags() | BATADV_DIRECTLINK);    //8 -> 72 | 0 -> 64
    }
    else
    {
        batadv_ogm_packet->setFlags(batadv_ogm_packet->getFlags() & ~BATADV_DIRECTLINK);    //8->8 | 64

    }
    //printf("---------- - Node: %s -- directlink  %d && ogm_packet->getFlags() %d  \n",nodenr.c_str(),is_single_hop_neigh,batadv_ogm_packet->getFlags());
//orig_node->bat_iv.bcast_own_sum += 1;
    tvlv_len = batadv_ogm_packet->getTvlv_len() + BATADV_OGM_HLEN;

    double forwardTime = batadv_iv_ogm_fwd_send_time();

    if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(batadv_ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
        printf("Forwarding packet (%f) from %s - node %s at %f\n", forwardTime, batadv_ogm_packet->getOrig().getMAC().str().c_str(), nodenr.c_str(), simTime().dbl());

    EV << "Node: " << nodenr << ": Adding package to queue with forwardTime " << forwardTime << std::endl;
    batadv_iv_ogm_queue_add(batadv_ogm_packet, tvlv_len, if_incoming, if_outgoing, false, reason, forwardTime);
    delete batadv_ogm_packet;
}

void BatmanADVMain::batadv_iv_ogm_forwardNoOutgoing(batadv_orig_node *orig_node, MACAddress *ethhdr, batadv_ogm_packet *batadv_ogm_packet, bool is_single_hop_neigh, bool is_from_best_next_hop, batadv_hard_iface if_incoming, forwardReason reason)
{
    long tvlv_len;
    if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(batadv_ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
        printf("Forward - node %s at %f\n", nodenr.c_str(), simTime().dbl());

    if (batadv_ogm_packet->getTtl() <= 1)
    {
        EV << "Node: " << nodenr << ": ttl exceeded" << std::endl;
        delete batadv_ogm_packet;
        return;
    }

    if (!is_from_best_next_hop)
    {
        EV << "Node: " << nodenr << ": OGM is not from next best hop - origMac: " << batadv_ogm_packet->getOrig().getMAC().str() << std::endl;
        /* Mark the forwarded packet when it is not coming from our
         * best next hop. We still need to forward the packet for our
         * neighbor link quality detection to work in case the packet
         * originated from a single hop neighbor. Otherwise we can
         * simply drop the ogm.
         */
        if (is_single_hop_neigh /*|| gw_mode==BATADV_GW_MODE_SERVER*/)
        {

            batadv_ogm_packet->setFlags(batadv_ogm_packet->getFlags() | BATADV_NOT_BEST_NEXT_HOP);
            EV << "Node: " << nodenr << ": OGM is a is_single_hop_neigh but not a best next hop: " << batadv_ogm_packet->getOrig().getMAC().str() << std::endl;
        }
        else
        {
            if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(batadv_ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
                printf("OGM is not a is_single_hop_neigh and not a best next hop - node %s at %f\n", nodenr.c_str(), simTime().dbl());
            EV << "Node: " << nodenr << ": OGM is not a is_single_hop_neigh and not a best next hop: " << batadv_ogm_packet->getOrig().getMAC().str() << std::endl;
            delete batadv_ogm_packet;
            return;
        }
    }

    batadv_ogm_packet->setTtl(batadv_ogm_packet->getTtl() - 1);
    batadv_ogm_packet->setPrev_sender(ManetAddress(MACAddress(ethhdr->getInt())));
//batadv_ogm_packet->setPrev_sender(ownAddress);

    /* apply hop penalty */
    batadv_ogm_packet->setTq(batadv_hop_penalty(batadv_ogm_packet->getTq()));

    EV << "Node: " << nodenr << ": Forwarding packet: tq: " << batadv_ogm_packet->getTq() << ", ttl: " << batadv_ogm_packet->getTtl() << std::endl;
    /* switch of primaries first hop flag when forwarding */
    //batadv_ogm_packet->setFlags(batadv_ogm_packet->getFlags() & ~BATADV_PRIMARIES_FIRST_HOP);
    //printf("Node: %s -- directlink  %d && ogm_packet->getFlags() %d  \n",nodenr.c_str(),is_single_hop_neigh,batadv_ogm_packet->getFlags());
    if (is_single_hop_neigh)
    {
        batadv_ogm_packet->setFlags(batadv_ogm_packet->getFlags() | BATADV_DIRECTLINK);    //8 -> 72 | 0 -> 64
    }
    else
    {
        batadv_ogm_packet->setFlags(batadv_ogm_packet->getFlags() & ~BATADV_DIRECTLINK);    //8->8 | 64

    }
    //printf("---------- - Node: %s -- directlink  %d && ogm_packet->getFlags() %d  \n",nodenr.c_str(),is_single_hop_neigh,batadv_ogm_packet->getFlags());
//orig_node->bat_iv.bcast_own_sum += 1;
    tvlv_len = batadv_ogm_packet->getTvlv_len() + BATADV_OGM_HLEN;

    double forwardTime = batadv_iv_ogm_fwd_send_time();
    EV << "Node: " << nodenr << ": Adding package to queue with forwardTime " << forwardTime << std::endl;
    batadv_iv_ogm_queue_addNoOutgoing(batadv_ogm_packet, tvlv_len, if_incoming, false, reason, forwardTime);
    delete batadv_ogm_packet;
}

/* apply hop penalty for a normal link */
int BatmanADVMain::batadv_hop_penalty(int tq)
{
    int new_tq;
    new_tq = tq * (TQ_MAX - hopPenalty);
    new_tq /= TQ_MAX;
    return new_tq;
}

/**
 * batadv_iv_ogm_orig_update - use OGM to update corresponding data in an
 *  originator
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the orig node who originally emitted the ogm packet
 * @orig_ifinfo: ifinfo for the outgoing interface of the orig_node
 * @ethhdr: Ethernet header of the OGM
 * @batadv_ogm_packet: the ogm packet
 * @if_incoming: interface where the packet was received
 * @if_outgoing: interface for which the retransmission should be considered
 * @dup_status: the duplicate status of this ogm packet.
 */
void BatmanADVMain::batadv_iv_ogm_orig_update(batadv_orig_node *orig_node, batadv_orig_ifinfo *orig_ifinfo, MACAddress ethhdr, batadv_ogm_packet *ogm, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, batadv_dup_status dup_status)
{

    //printf("Node: %s -- batadv_iv_ogm_orig_update with %s \n", nodenr.c_str(),orig_node->orig.getMAC().str().c_str());
    batadv_orig_node *orig_node_tmp = NULL;
    batadv_neigh_node *router = NULL;
    batadv_neigh_node *neigh_node = NULL, *tmp_neigh_node = NULL;
    ManetAddress neigh_addr;
    int sum_orig, sum_neigh;
    batadv_neigh_ifinfo *router_ifinfo = NULL;
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    int tq_avg;
    EV << "Node: " << nodenr << ": update_originator(): Searching and updating originator entry of received packet\n" << std::endl;
    for (unsigned int i = 0; i < orig_node->neigh_list.size(); i++)
    {
        tmp_neigh_node = orig_node->neigh_list[i];
        neigh_addr = tmp_neigh_node->addr;
        EV << "Node: " << nodenr << ": update_originator(): Temporary neigh - " << neigh_addr.getMAC().str() << std::endl;
        if (batadv_compare_eth(neigh_addr.getMAC(), ethhdr) == 0 && tmp_neigh_node->if_incoming->name == if_incoming.name)
        {
            /* if (WARN(neigh_node, "too many matching neigh_nodes"))
             batadv_neigh_node_free_ref(neigh_node);*/
            neigh_node = tmp_neigh_node;
            continue;
        }
        if (dup_status != BATADV_NO_DUP)
            continue;

        /* only update the entry for this outgoing interface */
        neigh_ifinfo = batadv_neigh_ifinfo_get(tmp_neigh_node, &if_outgoing);
        if (!neigh_ifinfo)
        {
            EV << "Node: " << nodenr << ": update_originator(): Temporary neigh - " << neigh_addr.getMAC().str() << "Couldn't find a neighIfInfo for " << tmp_neigh_node->addr.getMAC().str() << std::endl;
            continue;
        }
        //TODO: Test
        batadv_ring_buffer_set(neigh_ifinfo->bat_iv.tq_recv, &neigh_ifinfo->bat_iv.tq_index, 0);
        //batadv_ring_buffer_set_vector(neigh_ifinfo->bat_iv.tq_recv, 0);
        tq_avg = batadv_ring_buffer_avg(neigh_ifinfo->bat_iv.tq_recv);
        neigh_ifinfo->bat_iv.tq_avg = tq_avg;
        EV << "Node: " << nodenr << ": update_originator(): AVG TQ" << tq_avg << std::endl;
        neigh_ifinfo = NULL;
    }
    if (!neigh_node)
    {
        EV << "Node: " << nodenr << ": There is no neighnode in batadv_iv_ogm_orig_update() for " << std::endl;
        batadv_orig_node *orig_tmp;

        orig_tmp = batadv_iv_ogm_orig_get_by_mac(ethhdr);
        //orig_tmp = batadv_iv_ogm_orig_get(ogm);
        if (!orig_tmp)
        {
            //delete ogm;
            EV << "Node: " << nodenr << ": update_originator(): Couldn't find a orig entry for " << ogm->getOrig().getMAC().str() << std::endl;
            return;
        }
        neigh_node = batadv_iv_ogm_neigh_new(ethhdr, orig_node, orig_tmp, &if_incoming);
        if (!neigh_node)
        {
            return;
        }
    }
    else
        EV << "Node: " << nodenr << ": Updating existing last-hop neighbor of originator\n" << std::endl;
    neigh_ifinfo = batadv_neigh_ifinfo_new(neigh_node, &if_outgoing);
    if (!neigh_ifinfo)
    {        //delete ogm;
        EV << "Node: " << nodenr << ": Problem with neigh ifinfo" << std::endl;
        return;
    }

    neigh_node->last_seen = simTime();

    batadv_ring_buffer_set(neigh_ifinfo->bat_iv.tq_recv, &neigh_ifinfo->bat_iv.tq_index, ogm->getTq());
    tq_avg = batadv_ring_buffer_avg(neigh_ifinfo->bat_iv.tq_recv);
    EV << "Node: " << nodenr << ": update_originator(): AVG TQ" << tq_avg << std::endl;
    neigh_ifinfo->bat_iv.tq_avg = tq_avg;

    if (dup_status == BATADV_NO_DUP)
    {
        orig_ifinfo->last_ttl = ogm->getTtl();
        neigh_ifinfo->last_ttl = ogm->getTtl();
    }

    /* if this neighbor already is our next hop there is nothing
     * to change
     */
    router = batadv_orig_router_get(orig_node, &if_outgoing);
    if (router == neigh_node)
    {
        return;
    }
    if (router)
    {
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf("Update route - router found %s - node %s - time %f\n", ogm->getOrig().getMAC().str().c_str(), nodenr.c_str(), simTime().dbl());

        router_ifinfo = batadv_neigh_ifinfo_get(router, &if_outgoing);
        if (!router_ifinfo)
        {
            EV << "Node: " << nodenr << ": No router ifinfo in batadv_iv_ogm_orig_update" << std::endl;
            return;
        }

        /* if this neighbor does not offer a better TQ we won't
         * consider it
         */
        if (router_ifinfo->bat_iv.tq_avg > neigh_ifinfo->bat_iv.tq_avg)
        {
            //delete ogm;
            EV << "Node: " << nodenr << " router " << router->addr.getMAC().str() << " doesn't offer a better TQ value" << std::endl;
            return;
        }
    }
    /* if the TQ is the same and the link not more symmetric we
     * won't consider it either
     */
    if (router_ifinfo && (neigh_ifinfo->bat_iv.tq_avg == router_ifinfo->bat_iv.tq_avg))
    {
        orig_node_tmp = router->orig_node;

        sum_orig = orig_node_tmp->bat_iv.bcast_own_sum;

        orig_node_tmp = neigh_node->orig_node;
        sum_neigh = orig_node_tmp->bat_iv.bcast_own_sum;
        //spin_unlock_bh(&orig_node_tmp->bat_iv.ogm_cnt_lock);

        if (sum_orig >= sum_neigh)
        {        //delete ogm;
            return;
        }
    }
    batadv_update_route(orig_node, if_outgoing, neigh_node);

    return;
}

void BatmanADVMain::batadv_iv_ogm_orig_updateNoOutgoing(batadv_orig_node *orig_node, batadv_orig_ifinfo *orig_ifinfo, MACAddress ethhdr, batadv_ogm_packet *ogm, batadv_hard_iface if_incoming, batadv_dup_status dup_status)
{

    //printf("Node: %s -- batadv_iv_ogm_orig_update with %s \n", nodenr.c_str(),orig_node->orig.getMAC().str().c_str());
    batadv_orig_node *orig_node_tmp = NULL;
    batadv_neigh_node *router = NULL;
    batadv_neigh_node *neigh_node = NULL, *tmp_neigh_node = NULL;
    ManetAddress neigh_addr;
    int sum_orig, sum_neigh;
    batadv_neigh_ifinfo *router_ifinfo = NULL;
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    int tq_avg;
    EV << "Node: " << nodenr << ": update_originator(): Searching and updating originator entry of received packet\n" << std::endl;
    for (unsigned int i = 0; i < orig_node->neigh_list.size(); i++)
    {
        tmp_neigh_node = orig_node->neigh_list[i];
        neigh_addr = tmp_neigh_node->addr;
        EV << "Node: " << nodenr << ": update_originator(): Temporary neigh - " << neigh_addr.getMAC().str() << std::endl;
        if (batadv_compare_eth(neigh_addr.getMAC(), ethhdr) == 0 && tmp_neigh_node->if_incoming->name == if_incoming.name)
        {
            /* if (WARN(neigh_node, "too many matching neigh_nodes"))
             batadv_neigh_node_free_ref(neigh_node);*/
            neigh_node = tmp_neigh_node;
            continue;
        }
        if (dup_status != BATADV_NO_DUP)
            continue;

        /* only update the entry for this outgoing interface */
        neigh_ifinfo = batadv_neigh_ifinfo_getNoOutgoing(tmp_neigh_node);
        if (!neigh_ifinfo)
        {
            EV << "Node: " << nodenr << ": update_originator(): Temporary neigh - " << neigh_addr.getMAC().str() << "Couldn't find a neighIfInfo for " << tmp_neigh_node->addr.getMAC().str() << std::endl;
            continue;
        }
        //TODO: Test
        batadv_ring_buffer_set(neigh_ifinfo->bat_iv.tq_recv, &neigh_ifinfo->bat_iv.tq_index, 0);
        //batadv_ring_buffer_set_vector(neigh_ifinfo->bat_iv.tq_recv, 0);
        tq_avg = batadv_ring_buffer_avg(neigh_ifinfo->bat_iv.tq_recv);
        neigh_ifinfo->bat_iv.tq_avg = tq_avg;
        EV << "Node: " << nodenr << ": update_originator(): AVG TQ" << tq_avg << std::endl;
        neigh_ifinfo = NULL;
    }
    if (!neigh_node)
    {
        EV << "Node: " << nodenr << ": There is no neighnode in batadv_iv_ogm_orig_update() for " << std::endl;
        batadv_orig_node *orig_tmp;

        orig_tmp = batadv_iv_ogm_orig_get_by_mac(ethhdr);
        //orig_tmp = batadv_iv_ogm_orig_get(ogm);
        if (!orig_tmp)
        {
            //delete ogm;
            if ((iNodenr == -3 || iNodenr == -2) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            {
                printf("update_originator(): Couldn't find a orig entry for %s - node %s \n", ogm->getOrig().getMAC().str().c_str(), nodenr.c_str());
            }
            EV << "Node: " << nodenr << ": update_originator(): Couldn't find a orig entry for " << ogm->getOrig().getMAC().str() << std::endl;
            return;
        }
        neigh_node = batadv_iv_ogm_neigh_new(ethhdr, orig_node, orig_tmp, &if_incoming);
        if (!neigh_node)
        {
            return;
        }
    }
    else
        EV << "Node: " << nodenr << ": Updating existing last-hop neighbor of originator\n" << std::endl;
    neigh_ifinfo = batadv_neigh_ifinfo_newNoOutgoing(neigh_node);
    if (!neigh_ifinfo)
    {        //delete ogm;
        EV << "Node: " << nodenr << ": Problem with neigh ifinfo" << std::endl;
        return;
    }

    neigh_node->last_seen = simTime();

    batadv_ring_buffer_set(neigh_ifinfo->bat_iv.tq_recv, &neigh_ifinfo->bat_iv.tq_index, ogm->getTq());
    tq_avg = batadv_ring_buffer_avg(neigh_ifinfo->bat_iv.tq_recv);
    EV << "Node: " << nodenr << ": update_originator(): AVG TQ" << tq_avg << std::endl;
    neigh_ifinfo->bat_iv.tq_avg = tq_avg;

    if (dup_status == BATADV_NO_DUP)
    {
        orig_ifinfo->last_ttl = ogm->getTtl();
        neigh_ifinfo->last_ttl = ogm->getTtl();
    }

    /* if this neighbor already is our next hop there is nothing
     * to change
     */
    router = batadv_orig_router_getNoOutgoing(orig_node);
    if (router == neigh_node)
    {
        if ((iNodenr == -3 || iNodenr == -2) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
        {
            printf("update_originator(): Router %s is up to date - node %s \n", router->addr.getMAC().str().c_str(), nodenr.c_str());
        }
        return;
    }
    if (router)
    {
        router_ifinfo = batadv_neigh_ifinfo_getNoOutgoing(router);
        if (!router_ifinfo)
        {
            if ((iNodenr == -3 || iNodenr == -2) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            {
                printf("update_originator(): No router ifinfo in batadv_iv_ogm_orig_update for router %s- node %s \n", router->addr.getMAC().str().c_str(), nodenr.c_str());
            }
            EV << "Node: " << nodenr << ": No router ifinfo in batadv_iv_ogm_orig_update" << std::endl;
            return;
        }

        /* if this neighbor does not offer a better TQ we won't
         * consider it
         */
        if (router_ifinfo->bat_iv.tq_avg > neigh_ifinfo->bat_iv.tq_avg)
        {
            //delete ogm;
            EV << "Node: " << nodenr << " router " << router->addr.getMAC().str() << " doesn't offer a better TQ value" << std::endl;
            return;
        }
    }
    /* if the TQ is the same and the link not more symmetric we
     * won't consider it either
     */
    if (router_ifinfo && (neigh_ifinfo->bat_iv.tq_avg == router_ifinfo->bat_iv.tq_avg))
    {
        orig_node_tmp = router->orig_node;

        sum_orig = orig_node_tmp->bat_iv.bcast_own_sum;

        orig_node_tmp = neigh_node->orig_node;
        sum_neigh = orig_node_tmp->bat_iv.bcast_own_sum;
        //spin_unlock_bh(&orig_node_tmp->bat_iv.ogm_cnt_lock);

        if (sum_orig >= sum_neigh)
        {        //delete ogm;
            return;
        }
    }
    batadv_update_routeNoOutgoing(orig_node, neigh_node);

    return;
}

/**
 * batadv_iv_ogm_calc_tq - calculate tq for current received ogm packet
 * @orig_node: the orig node who originally emitted the ogm packet
 * @orig_neigh_node: the orig node struct of the neighbor who sent the packet
 * @batadv_ogm_packet: the ogm packet
 * @if_incoming: interface where the packet was received
 * @if_outgoing: interface for which the retransmission should be considered
 *
 * Returns 1 if the link can be considered bidirectional, 0 otherwise
 */
bool BatmanADVMain::batadv_iv_ogm_calc_tq(batadv_orig_node *orig_node, batadv_orig_node *orig_neigh_node, batadv_ogm_packet *ogm, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing)
{
    batadv_neigh_node *neigh_node = NULL;
    int orig_eq_count, neigh_rq_count, neigh_rq_inv, tq_own;
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    int total_count;
    unsigned int neigh_rq_inv_cube, neigh_rq_max_cube;
    int tq_asym_penalty, inv_asym_penalty;
    unsigned int combined_tq = 0;
    int tq_iface_penalty;
    bool ret = false;
    /* find corresponding one hop neighbor */
    for (unsigned int list_pos = 0; list_pos < orig_node->neigh_list.size(); list_pos++)
    {
        batadv_neigh_node* tmp_neigh_node = orig_neigh_node->neigh_list[list_pos];
        EV << "Node: " << nodenr << ": Checking neighbor entry " << tmp_neigh_node->addr.getMAC().str() << std::endl;
        if (tmp_neigh_node->ifNameIncoming != if_incoming.name)        //Test
            continue;
        int cResult = batadv_compare_eth(tmp_neigh_node->addr.getMAC(), orig_neigh_node->orig.getMAC());
        if (cResult != 0)
            continue;

        neigh_node = tmp_neigh_node;
        break;
    }

    /** Create a new one hop neighbor **/
    if (!neigh_node)        //263 - 1163
    {
        MACAddress m = orig_neigh_node->orig.getMAC();
        EV << "Node: " << nodenr << ": There is no neighbor in calTQ -> Creating new one with mac " << orig_neigh_node->orig.getMAC().str() << std::endl;
        neigh_node = batadv_iv_ogm_neigh_new(m, orig_neigh_node, orig_neigh_node, &if_incoming);

        // The the new node to the neighbor list of the originator
        //orig_node->neigh_list.push_back(neigh_node);//Remove?
        EV << "Node: " << nodenr << ": added a new neighbor: " << neigh_node->addr.getMAC().str() << std::endl;
    }
    if (!neigh_node)
        return false;
    EV << "Node: " << nodenr << ": The one hop neighbor is " << neigh_node->addr.getMAC().str() << std::endl;

    if (orig_node == orig_neigh_node)
        neigh_node->last_seen = simTime();
    orig_node->last_seen = simTime();

    /* find packet count of corresponding one hop neighbor */
    orig_eq_count = orig_neigh_node->bat_iv.bcast_own_sum;
    EV << "Node: " << nodenr << ": orig_eq_count (bcast_own_sum) value is " << orig_eq_count << std::endl;
    neigh_ifinfo = batadv_neigh_ifinfo_new(neigh_node, &if_outgoing); //search and possibly create
    /*if (neigh_ifinfo)
     {*/
    neigh_rq_count = neigh_ifinfo->bat_iv.real_packet_count;
    if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
        printf("eq %d - rq %d - node %s at %f\n", orig_eq_count, neigh_rq_count, nodenr.c_str(), simTime().dbl());

    EV << "Node: " << nodenr << ": neigh_rq_count (real_packet_count) value is " << neigh_rq_count << std::endl;
    /*}
     else
     {
     EV << "Node: " << nodenr << ": neigh_rq_count (real_packet_count) value is 0 because there is no ifinfo object for " << neigh_node->addr.getMAC().str() << std::endl;
     neigh_rq_count = 0;
     }*/

    /* pay attention to not get a value bigger than 100 % */
    if (orig_eq_count > neigh_rq_count)
        total_count = neigh_rq_count;
    else
        total_count = orig_eq_count;

    /* if we have too few packets (too less data) we set tq_own to zero
     * if we receive too few packets it is not considered bidirectional
     */
    if (total_count < TQ_LOCAL_BIDRECT_SEND_MIN || neigh_rq_count < TQ_LOCAL_BIDRECT_RECV_MIN)
        tq_own = 0;
    else
        /* neigh_node->real_packet_count is never zero as we
         * only purge old information when getting new
         * information
         */
        tq_own = (TQ_MAX * total_count) / neigh_rq_count;

    /* 1 - ((1-x) ** 3), normalized to TQ_MAX_VALUE this does
     * affect the nearly-symmetric links only a little, but
     * punishes asymmetric links more.  This will give a value
     * between 0 and TQ_MAX_VALUE
     */
    neigh_rq_inv = TQ_LOCAL_WINDOW_SIZE - neigh_rq_count;
    neigh_rq_inv_cube = neigh_rq_inv * neigh_rq_inv * neigh_rq_inv;
    neigh_rq_max_cube = TQ_LOCAL_WINDOW_SIZE *
    TQ_LOCAL_WINDOW_SIZE *
    TQ_LOCAL_WINDOW_SIZE;
    inv_asym_penalty = TQ_MAX * neigh_rq_inv_cube;
    inv_asym_penalty /= neigh_rq_max_cube;
    tq_asym_penalty = TQ_MAX - inv_asym_penalty;

    /* penalize if the OGM is forwarded on the same interface. WiFi
     * interfaces and other half duplex devices suffer from throughput
     * drops as they can't send and receive at the same time.
     */
    tq_iface_penalty = TQ_MAX;
    if ((if_incoming.name == if_outgoing.name) && batadv_is_wifi_netdev(&if_outgoing))
        tq_iface_penalty = batadv_hop_penalty(TQ_MAX);

    combined_tq = ogm->getTq() * tq_own * tq_asym_penalty * tq_iface_penalty;
    combined_tq /= TQ_MAX * TQ_MAX * TQ_MAX;
    ogm->setTq(combined_tq);
    EV << "Node: " << nodenr << ": new TQ is " << combined_tq << std::endl;
    /* if link has the minimum required transmission quality
     * consider it bidirectional
     */
    if (ogm->getTq() >= TQ_TOTAL_BIDRECT_LIMIT)
    {
        EV << "Node: " << nodenr << ": bidirectional!" << std::endl;
        ret = true;
    }
    return ret;        //Bidirectional
}

bool BatmanADVMain::batadv_iv_ogm_calc_tqNoOutgoing(batadv_orig_node *orig_node, batadv_orig_node *orig_neigh_node, batadv_ogm_packet *ogm, batadv_hard_iface if_incoming)
{
    batadv_neigh_node *neigh_node = NULL;
    int orig_eq_count, neigh_rq_count, neigh_rq_inv, tq_own;
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    int total_count;
    unsigned int neigh_rq_inv_cube, neigh_rq_max_cube;
    int tq_asym_penalty, inv_asym_penalty;
    unsigned int combined_tq = 0;
    int tq_iface_penalty;
    bool ret = false;
    /* find corresponding one hop neighbor */
    for (unsigned int list_pos = 0; list_pos < orig_neigh_node->neigh_list.size(); list_pos++)
    {
        batadv_neigh_node* tmp_neigh_node = orig_neigh_node->neigh_list[list_pos];
        EV << "Node: " << nodenr << ": Checking neighbor entry " << tmp_neigh_node->addr.getMAC().str() << std::endl;
        if (tmp_neigh_node->ifNameIncoming != if_incoming.name)
            continue;
        int cResult = batadv_compare_eth(tmp_neigh_node->addr.getMAC(), orig_neigh_node->orig.getMAC());
        if (cResult != 0)
            continue;

        neigh_node = tmp_neigh_node;
        break;
    }

    /** Create a new one hop neighbor **/
    if (!neigh_node)        //263 - 1163
    {
        MACAddress m = orig_neigh_node->orig.getMAC();
        EV << "Node: " << nodenr << ": There is no neighbor in calTQ -> Creating new one with mac " << orig_neigh_node->orig.getMAC().str() << std::endl;
        neigh_node = batadv_iv_ogm_neigh_new(m, orig_neigh_node, orig_neigh_node, &if_incoming);

        // The the new node to the neighbor list of the originator
        //orig_node->neigh_list.push_back(neigh_node);//Remove?
        EV << "Node: " << nodenr << ": added a new neighbor: " << neigh_node->addr.getMAC().str() << std::endl;
    }
    if (!neigh_node)
        return false;
    EV << "Node: " << nodenr << ": The one hop neighbor is " << neigh_node->addr.getMAC().str() << std::endl;

    if (orig_node == orig_neigh_node)
        neigh_node->last_seen = simTime();
    orig_node->last_seen = simTime();

    /* find packet count of corresponding one hop neighbor */
    orig_eq_count = orig_neigh_node->bat_iv.bcast_own_sum;
    EV << "Node: " << nodenr << ": orig_eq_count (bcast_own_sum) value is " << orig_eq_count << std::endl;
    neigh_ifinfo = batadv_neigh_ifinfo_newNoOutgoing(neigh_node); //search and possibly create
    /*if (neigh_ifinfo)
     {*/
    neigh_rq_count = neigh_ifinfo->bat_iv.real_packet_count;
    if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
        printf("eq %d - rq %d - node %s at %f\n", orig_eq_count, neigh_rq_count, nodenr.c_str(), simTime().dbl());

    EV << "Node: " << nodenr << ": neigh_rq_count (real_packet_count) value is " << neigh_rq_count << std::endl;
    /*}
     else
     {
     EV << "Node: " << nodenr << ": neigh_rq_count (real_packet_count) value is 0 because there is no ifinfo object for " << neigh_node->addr.getMAC().str() << std::endl;
     neigh_rq_count = 0;
     }*/

    /* pay attention to not get a value bigger than 100 % */
    if (orig_eq_count > neigh_rq_count)
        total_count = neigh_rq_count;
    else
        total_count = orig_eq_count;

    /* if we have too few packets (too less data) we set tq_own to zero
     * if we receive too few packets it is not considered bidirectional
     */
    if (total_count < TQ_LOCAL_BIDRECT_SEND_MIN || neigh_rq_count < TQ_LOCAL_BIDRECT_RECV_MIN)
        tq_own = 0;
    else
        /* neigh_node->real_packet_count is never zero as we
         * only purge old information when getting new
         * information
         */
        tq_own = (TQ_MAX * total_count) / neigh_rq_count;

    /* 1 - ((1-x) ** 3), normalized to TQ_MAX_VALUE this does
     * affect the nearly-symmetric links only a little, but
     * punishes asymmetric links more.  This will give a value
     * between 0 and TQ_MAX_VALUE
     */
    neigh_rq_inv = TQ_LOCAL_WINDOW_SIZE - neigh_rq_count;
    neigh_rq_inv_cube = neigh_rq_inv * neigh_rq_inv * neigh_rq_inv;
    neigh_rq_max_cube = TQ_LOCAL_WINDOW_SIZE *
    TQ_LOCAL_WINDOW_SIZE *
    TQ_LOCAL_WINDOW_SIZE;
    inv_asym_penalty = TQ_MAX * neigh_rq_inv_cube;
    inv_asym_penalty /= neigh_rq_max_cube;
    tq_asym_penalty = TQ_MAX - inv_asym_penalty;

    /* penalize if the OGM is forwarded on the same interface. WiFi
     * interfaces and other half duplex devices suffer from throughput
     * drops as they can't send and receive at the same time.
     */
    tq_iface_penalty = TQ_MAX;
    /*if ((if_incoming.name == if_outgoing.name) && batadv_is_wifi_netdev(&if_outgoing))
     tq_iface_penalty = batadv_hop_penalty(TQ_MAX);*/

    combined_tq = ogm->getTq() * tq_own * tq_asym_penalty * tq_iface_penalty;
    combined_tq /= TQ_MAX * TQ_MAX * TQ_MAX;
    ogm->setTq(combined_tq);
    if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
        printf("new TQ is %d - node %s at %f\n", combined_tq, nodenr.c_str(), simTime().dbl());

    EV << "Node: " << nodenr << ": new TQ is " << combined_tq << std::endl;
    /* if link has the minimum required transmission quality
     * consider it bidirectional
     */
    if (ogm->getTq() >= TQ_TOTAL_BIDRECT_LIMIT)
    {
        EV << "Node: " << nodenr << ": bidirectional!" << std::endl;
        ret = true;
    }
    return ret;        //Bidirectional
}

batadv_dup_status BatmanADVMain::batadv_iv_ogm_update_seqnosIfChange(MACAddress* mac, batadv_ogm_packet *ogm_packet, batadv_hard_iface if_incoming)
{
    batadv_orig_node *orig_node = NULL;
    batadv_orig_ifinfo *orig_ifinfo = NULL;
    long seq_diff = 0;
    batadv_dup_status ret = BATADV_NO_DUP;
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    batadv_neigh_node *neigh_node = NULL;
    //int set_mark;
    int is_dup = 0;
    int packet_count = 0;
    ManetAddress neigh_addr;

    long seqno = ogm_packet->getSeqno();

    orig_node = batadv_iv_ogm_orig_get(ogm_packet);
    if (!orig_node)
        return BATADV_NO_DUP;

    orig_ifinfo = batadv_orig_ifinfo_newNoOutgoing(orig_node, ogm_packet->getPrev_sender());
    //printf("Node: %s orig_node->ifinfo_list.size() %d\n", nodenr.c_str(), orig_node->ifinfo_list.size());
    seq_diff = seqno - orig_ifinfo->last_real_seqno;
    int window_protected = batadv_window_protected(seq_diff, orig_ifinfo->batman_seqno_reset);
    if (window_protected == 1 && orig_node->neigh_list.size() > 0)
        return BATADV_PROTECTED;        //Package has to be dropped afterwards

    for (unsigned int list_pos = 0; list_pos < orig_node->neigh_list.size(); list_pos++)
    {
        neigh_node = orig_node->neigh_list[list_pos];
        neigh_ifinfo = batadv_neigh_ifinfo_newNoOutgoing(neigh_node);
        neigh_addr = neigh_node->addr;
        is_dup = batadv_test_bit(orig_ifinfo->last_real_seqno, seqno);

        if (batadv_compare_eth(neigh_addr.getMAC(), frame->getSrc()) && neigh_node->if_incoming->name == if_incoming.name)
        {
            //set_mark = 1;
            if (is_dup == 1)
                ret = BATADV_NEIGH_DUP;
        }
        else
        {
            //set_mark = 0;
            if (is_dup == 1 && (ret != BATADV_NEIGH_DUP))
                ret = BATADV_ORIG_DUP;
        }
        //TODO: If a new package arrives, there is always a new entry?! Why the "need update" shit?

        EV << "Node: " << nodenr << ": Adding an entry to the sliding window - MAC: " << ogm_packet->getPrev_sender().getMAC() << std::endl;
        //EV << "Node: " << nodenr << ": Adding an entry to the sliding window - MAC: " << ogm_packet->getOrig().getMAC() << std::endl;

        batadv_iv_window_add_entry(ogm_packet->getOrig().getMAC(), ogm_packet->getSeqno(), ogm_packet->getTq());
        packet_count = batadv_iv_window_packetCount(ogm_packet->getOrig().getMAC());
        if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm_packet->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
            printf("Setting rq-count %d - node %s at %f\n", packet_count, nodenr.c_str(), simTime().dbl());

        neigh_ifinfo->bat_iv.real_packet_count = packet_count;
    }

    if (true)
    {
        EV << "Node: " << nodenr << ": updating last_seqno to " << seqno << std::endl;
        orig_ifinfo->last_real_seqno = seqno;
    }
    orig_node->ifinfo = orig_ifinfo;

    return ret;

}
/**
 * batadv_iv_ogm_update_seqnos -  process a batman packet for all interfaces,
 *  adjust the sequence number and find out whether it is a duplicate
 *
 * Returns duplicate status as enum batadv_dup_status
 */
batadv_dup_status BatmanADVMain::batadv_iv_ogm_update_seqnos(batadv_ogm_packet * ogm_packet, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing)
{
    batadv_orig_node *orig_node = NULL;
    batadv_orig_ifinfo *orig_ifinfo = NULL;
    long seq_diff;
    batadv_dup_status ret = BATADV_NO_DUP;
    batadv_neigh_ifinfo *neigh_ifinfo = NULL;
    batadv_neigh_node *neigh_node = NULL;
//int set_mark;
    int is_dup;
    int packet_count;
    ManetAddress neigh_addr;

    long seqno = ogm_packet->getSeqno();

    orig_node = batadv_iv_ogm_orig_get(ogm_packet);
    if (!orig_node)
        return BATADV_NO_DUP;

    orig_ifinfo = batadv_orig_ifinfo_new(orig_node, ogm_packet->getPrev_sender(), &if_outgoing);
    //printf("Node: %s orig_node->ifinfo_list.size() %d\n", nodenr.c_str(), orig_node->ifinfo_list.size());
    seq_diff = seqno - orig_ifinfo->last_real_seqno;
    int window_protected = batadv_window_protected(seq_diff, orig_ifinfo->batman_seqno_reset);
    if (window_protected == 1 && orig_node->neigh_list.size() > 0)
        return BATADV_PROTECTED;        //Package has to be dropped afterwards

    for (unsigned int list_pos = 0; list_pos < orig_node->neigh_list.size(); list_pos++)
    {
        neigh_node = orig_node->neigh_list[list_pos];
        neigh_ifinfo = batadv_neigh_ifinfo_new(neigh_node, &if_outgoing);
        if (!neigh_ifinfo)
            continue;

        neigh_addr = neigh_node->addr;
        is_dup = batadv_test_bit(orig_ifinfo->last_real_seqno, seqno);

        if (batadv_compare_eth(neigh_addr.getMAC(), frame->getSrc()) && neigh_node->if_incoming->name == if_incoming.name)
        {
            //set_mark = 1;
            if (is_dup == 1)
                ret = BATADV_NEIGH_DUP;
        }
        else
        {
            //set_mark = 0;
            if (is_dup == 1 && (ret != BATADV_NEIGH_DUP))
                ret = BATADV_ORIG_DUP;
        }
        //TODO: If a new package arrives, there is always a new entry?! Why the "need update" shit?

        EV << "Node: " << nodenr << ": Adding an entry to the sliding window - MAC: " << ogm_packet->getPrev_sender().getMAC() << std::endl;
        //EV << "Node: " << nodenr << ": Adding an entry to the sliding window - MAC: " << ogm_packet->getOrig().getMAC() << std::endl;

        batadv_iv_window_add_entry(ogm_packet->getOrig().getMAC(), ogm_packet->getSeqno(), ogm_packet->getTq());
        packet_count = batadv_iv_window_packetCount(ogm_packet->getOrig().getMAC());
        neigh_ifinfo->bat_iv.real_packet_count = packet_count;
    }

    if (true)
    {
        EV << "Node: " << nodenr << ": updating last_seqno to " << seqno << std::endl;
        orig_ifinfo->last_real_seqno = seqno;
    }
    orig_node->ifinfo = orig_ifinfo;

    return ret;
}

/**
 * batadv_iv_ogm_aggregate_new - create a new aggregated packet and add this
 *  packet to it.
 * @packet_buff: pointer to the OGM
 * @packet_len: (total) length of the OGM
 * @send_time: timestamp (jiffies) when the packet is to be sent
 * @direct_link: whether this OGM has direct link status
 * @if_incoming: interface where the packet was received
 * @if_outgoing: interface for which the retransmission should be considered
 * @own_packet: true if it is a self-generated ogm
 */
void BatmanADVMain::batadv_iv_ogm_aggregate_new(batadv_ogm_packet *packet_buff, long packet_len, double send_time, bool direct_link, batadv_hard_iface if_incoming, batadv_hard_iface if_outgoing, bool own_packet, forwardReason reason)
{
    EV << "Node: " << nodenr << ": batadv_iv_ogm_aggregate_new called" << std::endl;
    /* own packet should always be scheduled */
    if (!own_packet)
    {
        if (batman_queue_left == 1)
        {
            EV << "Node: " << nodenr << ": batman packet queue full" << std::endl;
            delete packet_buff;
            return;
        }
        batman_queue_left--;
    }
    batadv_forw_packet forw_packet_aggr = batadv_forw_packet();
    forw_packet_aggr.id = fRand(0, 9999999);
    forw_packet_aggr.own = own_packet;
    forw_packet_aggr.if_incoming = if_incoming;
    forw_packet_aggr.if_outgoing = if_outgoing;
    forw_packet_aggr.if_incomingName = if_incoming.name;
    forw_packet_aggr.if_outgoingName = if_outgoing.name;
    forw_packet_aggr.name_if_incoming = if_incoming.name;
    forw_packet_aggr.name_if_outgoing = if_outgoing.name;
    forw_packet_aggr.num_packets = 0;
    forw_packet_aggr.direct_link_flags = BATADV_NO_FLAGS;
    forw_packet_aggr.send_time = send_time;
    forw_packet_aggr.skb = std::vector<batadv_ogm_packetClass>();
    forw_packet_aggr.packet_len = packet_len + ETH_HLEN;
    EV << "Node: " << nodenr << " encapsulating package with ttvn " << packet_buff->getTt_data().getTtvn() << " from " << packet_buff->getOrig().getMAC().str() << " and seqno " << packet_buff->getSeqno() << " sendtime " << send_time << std::endl;
    forw_packet_aggr.skb.push_back(convertFromOGM(packet_buff));
    /* save packet direct link flag status */
    if (direct_link)
        forw_packet_aggr.direct_link_flags |= 1;

    forw_bat_list.push_back(forw_packet_aggr);

//Timer!
    delayedWork* d = new delayedWork();
    d->setReason(reason);
    d->setId(forw_packet_aggr.id);
//delete forw_packet_aggr;
    //simtime_t s = (simtime_t) send_time / 1000.0 + simTime();        //Convert to ms and add to current simulation time
    simtime_t s = (simtime_t) send_time / 1000.0;
    d->setName("delayedWork");
    EV << "Node: " << nodenr << ": aggregated packet scheduled for " << s.str() << " Name: " << d->getName() << std::endl;
    scheduleAt(s, d);
    if (!own_packet)
        batman_queue_left++;
    return;
}

void BatmanADVMain::batadv_iv_ogm_aggregate_newNoOutgoing(batadv_ogm_packet *packet_buff, long packet_len, double send_time, bool direct_link, batadv_hard_iface if_incoming, bool own_packet, forwardReason reason)
{
    EV << "Node: " << nodenr << ": batadv_iv_ogm_aggregate_new called" << std::endl;
    /* own packet should always be scheduled */
    if (!own_packet)
    {
        if (batman_queue_left == 1)
        {
            EV << "Node: " << nodenr << ": batman packet queue full" << std::endl;
            if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(packet_buff->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
                printf("node: %s batman packet queue full - at %f\n", nodenr.c_str(), simTime().dbl());

            delete packet_buff;
            return;
        }
        batman_queue_left--;
    }
    batadv_forw_packet forw_packet_aggr = batadv_forw_packet();
    forw_packet_aggr.id = fRand(0, 9999999);
    forw_packet_aggr.own = own_packet;
    forw_packet_aggr.if_incoming = if_incoming;
    forw_packet_aggr.if_incomingName = if_incoming.name;
    forw_packet_aggr.name_if_incoming = if_incoming.name;
    forw_packet_aggr.num_packets = 0;
    forw_packet_aggr.direct_link_flags = BATADV_NO_FLAGS;
    forw_packet_aggr.send_time = send_time;
    forw_packet_aggr.skb = std::vector<batadv_ogm_packetClass>();
    forw_packet_aggr.packet_len = packet_len + ETH_HLEN;
    EV << "Node: " << nodenr << " encapsulating package with ttvn " << packet_buff->getTt_data().getTtvn() << " from " << packet_buff->getOrig().getMAC().str() << " and seqno " << packet_buff->getSeqno() << " sendtime " << send_time << std::endl;
    forw_packet_aggr.skb.push_back(convertFromOGM(packet_buff));
    /* save packet direct link flag status */
    if (direct_link)
        forw_packet_aggr.direct_link_flags |= 1;

    forw_bat_list.push_back(forw_packet_aggr);

    //Timer!
    delayedWork* d = new delayedWork();
    d->setReason(reason);
    d->setId(forw_packet_aggr.id);
    //delete forw_packet_aggr;
    //simtime_t s = (simtime_t) send_time / 1000.0 + simTime();        //Convert to ms and add to current simulation time
    simtime_t s = (simtime_t) send_time / 1000.0;
    d->setName("delayedWork");
    EV << "Node: " << nodenr << ": aggregated packet scheduled for " << s.str() << " Name: " << d->getName() << std::endl;
    scheduleAt(s, d);
    if (!own_packet)
        batman_queue_left++;
    return;
}
/**
 * send scheduled OGM
 */

void BatmanADVMain::batadv_iv_ogm_emit(batadv_forw_packet forw_packet, forwardReason reason)
{
    /* EV << "Node: " << nodenr << ": batadv_iv_ogm_emit" << std::endl;
     if (!forw_packet->if_incoming)
     {
     EV << "Node: " << nodenr << ": Error - can't forward packet: incoming iface not specified" << std::endl;
     return;
     }
     if (forw_packet->if_incoming->if_status != BATADV_IF_ACTIVE)
     {
     EV << "Node: " << nodenr << ": Error - can't forward packet: incoming iface is not active" << std::endl;
     return;
     }*/
    EV << "Node: " << nodenr << ": Sending OGM to if" << std::endl;
    batadv_iv_ogm_send_to_if(forw_packet, forw_packet.if_outgoing, reason);
}

void BatmanADVMain::batadv_iv_ogm_emitNoOutgoing(batadv_forw_packet forw_packet, forwardReason reason)
{
    /* EV << "Node: " << nodenr << ": batadv_iv_ogm_emit" << std::endl;
     if (!forw_packet->if_incoming)
     {
     EV << "Node: " << nodenr << ": Error - can't forward packet: incoming iface not specified" << std::endl;
     return;
     }
     if (forw_packet->if_incoming->if_status != BATADV_IF_ACTIVE)
     {
     EV << "Node: " << nodenr << ": Error - can't forward packet: incoming iface is not active" << std::endl;
     return;
     }*/
    EV << "Node: " << nodenr << ": Sending OGM to if" << std::endl;
    batadv_iv_ogm_send_to_ifNoOutgoing(forw_packet, reason);
}
/* send a batman ogm to a given interface */
void BatmanADVMain::batadv_iv_ogm_send_to_ifNoOutgoing(batadv_forw_packet forw_packet, forwardReason reason)
{
    batadv_ogm_packet *ogm_packet = NULL;
    /* adjust all flags and log packets */

    if (simTime() >= warmupTime)
    {
        StatisticsCollector* stats = getGlobalStatisticsController();
        stats->recordSendByteLength(forw_packet.packet_len, meshId);
    }
    if (forw_packet.skb.size() == 1)
    {
        EV << "Node: " << nodenr << ": Size of buffer is 1 - sending normal ogm" << std::endl;
        ogm_packet = convertToOGM(forw_packet.skb[0]);
        forw_packet.skb.pop_back();
        if (ogm_packet == NULL)
        {
            //delete forw_packet;
            return;
        }
        EV << "Node: " << nodenr << "decapsulate ogm with ttvn " << ogm_packet->getTt_data().getTtvn() << " and seqnr " << ogm_packet->getSeqno() << std::endl;
        if (forw_packet.packet_len < 1500)
        {
            ogm_packet->setByteLength(forw_packet.packet_len);
        }
        else
        {
            printf("Here is some stupid shit with a packet, which is too large");
        }

        //non-primary OGMs are only broadcasted on their interface
        if ((ogm_packet->getFlags() & BATADV_DIRECTLINK == BATADV_DIRECTLINK))
            ogm_packet->setFlags(ogm_packet->getFlags() & BATADV_DIRECTLINK);
        else
            ogm_packet->setFlags(ogm_packet->getFlags() & BATADV_DIRECTLINK);
        if ((forw_packet.if_incomingName != primary_ifNP.name && forw_packet.own) || (ogm_packet->getFlags() & BATADV_DIRECTLINK == BATADV_DIRECTLINK))
        {
            if ((iNodenr == -1))
                printf("\nNEWNODE SENDING OWN OGM -- SINGLE -> TIME: %f\n", simTime().dbl());
            batadv_send_skb_packet(ogm_packet, forw_packet.if_incoming, MACAddress::BROADCAST_ADDRESS, reason);
        }
        else
        {
            if (forw_packet.own)
            {
                if ((iNodenr == -1))
                    printf("\nNEWNODE SENDING OWN OGM -- SINGLE -> TIME: %f\n", simTime().dbl());
            }
            if (gw_mode != BATADV_GW_MODE_SERVER)
                batadv_send_skb_packet(ogm_packet->dup(), wlan0NP, MACAddress::BROADCAST_ADDRESS, reason);
            batadv_send_skb_packet(ogm_packet, meshvpnNP, MACAddress::BROADCAST_ADDRESS, reason);
        }
        //delete forw_packet;
    }
    else
    {
        std::vector<batadv_ogm_packetClass> ogmlistNoPointer = std::vector<batadv_ogm_packetClass>();
        batadv_ogm_packet_agg* skbBuff = new batadv_ogm_packet_agg();
        for (unsigned int i = 0; i < forw_packet.skb.size(); i++)
        {
            ogmlistNoPointer.push_back(forw_packet.skb[i]);
        }
        if (forw_packet.packet_len < 1500)
        {
            skbBuff->setByteLength(forw_packet.packet_len);
        }
        else
        {
            printf("Here is some stupid shit with a packet, which is too large");
        }

        skbBuff->setOgmsNoPointer(ogmlistNoPointer);
        EV << "Node: " << nodenr << ": Size of buffer is > 1 " << std::endl;
        //skbBuff->setOgms(forw_packet.skb);
        //batadv_send_skb_packet(skbBuff, hard_iface, MACAddress::BROADCAST_ADDRESS, reason);

        //non-primary OGMs are only broadcasted on their interface

        if ((forw_packet.if_incomingName != primary_ifNP.name && forw_packet.own) || ((forw_packet.skb[0].flags & BATADV_DIRECTLINK) == BATADV_DIRECTLINK))
        {
            if ((iNodenr == -1))
                printf("\n NEWNODE SENDING OWN OGM -- AGGREGATION -> TIME: %f\n", simTime().dbl());
            batadv_send_skb_packet(skbBuff, forw_packet.if_incoming, MACAddress::BROADCAST_ADDRESS, reason);
        }
        /* broadcast on every interface */
        else
        {
            if (forw_packet.own)
            {
                if ((iNodenr == -1))
                    printf("\nNEWNODE SENDING OWN OGM -- SINGLE -> TIME: %f\n", simTime().dbl());
            }
            if (gw_mode != BATADV_GW_MODE_SERVER)
                batadv_send_skb_packet(skbBuff->dup(), wlan0NP, MACAddress::BROADCAST_ADDRESS, reason);
            batadv_send_skb_packet(skbBuff, meshvpnNP, MACAddress::BROADCAST_ADDRESS, reason);
        }
    }

    forw_packet.skb.clear();
}

/* send a batman ogm to a given interface */
void BatmanADVMain::batadv_iv_ogm_send_to_if(batadv_forw_packet forw_packet, batadv_hard_iface hard_iface, forwardReason reason)
{
    batadv_ogm_packet *ogm_packet = NULL;
    /* adjust all flags and log packets */
//while (canAggregate)
    if (forw_packet.skb.size() == 1)
    {
        EV << "Node: " << nodenr << ": Size of buffer is 1 - sending normal ogm" << std::endl;
        ogm_packet = convertToOGM(forw_packet.skb[0]);
        forw_packet.skb.pop_back();
        if (ogm_packet == NULL)
        {
            //delete forw_packet;
            return;
        }
        EV << "Node: " << nodenr << "decapsulate ogm with ttvn " << ogm_packet->getTt_data().getTtvn() << " and seqnr " << ogm_packet->getSeqno() << std::endl;
        ogm_packet->setByteLength(forw_packet.packet_len);
        batadv_send_skb_packet(ogm_packet, hard_iface, MACAddress::BROADCAST_ADDRESS, reason);
        //delete forw_packet;
    }
    else
    {
        std::vector<batadv_ogm_packetClass> ogmlistNoPointer = std::vector<batadv_ogm_packetClass>();
        batadv_ogm_packet_agg* skbBuff = new batadv_ogm_packet_agg();
        for (unsigned int i = 0; i < forw_packet.skb.size(); i++)
        {
            if (forw_packet.direct_link_flags)
                forw_packet.skb[i].flags |= BATADV_DIRECTLINK;

            ogmlistNoPointer.push_back(forw_packet.skb[i]);
        }
        skbBuff->setByteLength(forw_packet.packet_len);
        skbBuff->setOgmsNoPointer(ogmlistNoPointer);
        EV << "Node: " << nodenr << ": Size of buffer is > 1 " << std::endl;
        //skbBuff->setOgms(forw_packet.skb);
        batadv_send_skb_packet(skbBuff, hard_iface, MACAddress::BROADCAST_ADDRESS, reason);
    }

    forw_packet.skb.clear();
}

batadv_ogm_packet* BatmanADVMain::convertToOGM(batadv_ogm_packetClass object)
{
    batadv_ogm_packet* packet = new batadv_ogm_packet();
    packet->setPacket_type(object.packet_type);
    packet->setVersion(object.version);
    packet->setTtl(object.ttl);
    packet->setFlags(object.flags);
    packet->setSeqno(object.seqno);
    packet->setOrig(object.orig);
    packet->setPrev_sender(object.prev_sender);
    packet->setReserved(object.reserved);
    packet->setTq(object.tq);
    packet->setTvlv_len(object.tvlv_len);
    packet->setMeshID(object.meshID);
    packet->setTt_data(object.tt_data);
    packet->setTt_change(object.tt_change);
    packet->setGateway_data(object.gateway_data);
    return packet;
}

batadv_ogm_packetClass BatmanADVMain::convertFromOGM(batadv_ogm_packet* object)
{
    batadv_ogm_packetClass packet = batadv_ogm_packetClass();
    packet.flags = object->getFlags();
    packet.gateway_data = object->getGateway_data();
    packet.meshID = object->getMeshID();
    packet.orig = object->getOrig();
    packet.packet_type = object->getPacket_type();
    packet.prev_sender = object->getPrev_sender();
    packet.reserved = object->getReserved();
    packet.seqno = object->getSeqno();
    packet.tq = object->getTq();
    packet.tt_change = object->getTt_change();
    packet.tt_data = object->getTt_data();
    packet.ttl = object->getTtl();
    packet.tvlv_len = object->getTvlv_len();
    packet.version = object->getVersion();
    return packet;
}


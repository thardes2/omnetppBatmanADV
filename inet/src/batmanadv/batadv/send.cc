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

#include <send.h>

send::send()
{
}

send::~send()
{
}

void BatmanADVMain::batadv_schedule_bat_ogm(batadv_hard_iface hard_iface)
{
    int name = hard_iface.name;
    /*
     if ((hard_iface->if_status == BATADV_IF_NOT_IN_USE) || (hard_iface->if_status == BATADV_IF_TO_BE_REMOVED))
     return;*/
    /* the interface gets activated here to avoid race conditions between
     * the moment of activating the interface in
     * hardif_activate_interface() where the originator mac is set and
     * outdated packets (especially uninitialized mac addresses) in the
     * packet queue
     */

    /*  if (hard_iface->if_status == BATADV_IF_TO_BE_ACTIVATED)
     hard_iface->if_status = BATADV_IF_ACTIVE;*/
    batadv_iv_ogm_schedule(hard_iface);

}
/**
 * batadv_send_skb_to_orig - Lookup next-hop and transmit skb.
 * @skb: Packet to be transmitted.
 * @orig_node: Final destination of the packet.
 * @recv_if: Interface used when receiving the packet (can be NULL).
 *
 * Looks up the best next-hop towards the passed originator and passes the
 * skb on for preparation of MAC header. If the packet originated from this
 * host, NULL can be passed as recv_if and no interface alternating is
 * attempted.
 *
 * Returns NET_XMIT_SUCCESS on success, NET_XMIT_DROP on failure, or
 * NET_XMIT_POLICED if the skb is buffered for later transmit.
 */
NET_XMIT BatmanADVMain::batadv_send_skb_to_orig(cPacket *skb, batadv_orig_node *orig_node, batadv_hard_iface *recv_if, forwardReason reason)
{
    batadv_neigh_node *neigh_node;
    NET_XMIT ret = NET_XMIT_DROP;

    neigh_node = batadv_find_routerNoOutgoing(orig_node);
    if (!neigh_node)
    {
        EV << "Node: " << nodenr << ": There is no router for the orig_node " << orig_node->orig.getMAC().str() << std::endl;
        delete skb;
        return ret;
    }
    /* Check if the skb is too large to send in one piece and fragment
     * it if needed.
     * --->fragmentation missing
     */
    //batadv_send_skb_packet(skb, neigh_node->if_incoming, neigh_node->addr.getMAC(), reason);
    ret = NET_XMIT_SUCCESS;
    return ret;
}

/* send out an already prepared packet to the given address via the
 * specified batman interface
 */
int BatmanADVMain::batadv_send_skb_packet(cPacket *skb, batadv_hard_iface hard_iface, MACAddress dst_addr, forwardReason reason)
{
    if (dynamic_cast<batadv_ogm_packet *>(skb) != NULL)
    {
        if (dynamic_cast<batadv_ogm_packet *>(skb)->getGateway_data().getBandwidth_down() != 0)
            int x = 0;
    }
    std::string fReason = "";
    std::string fName = "";
    std::stringstream ss;
    EV << "Node: " << nodenr << ": Sending SKB packet" << std::endl;
    if (reason == FORWARD_MY_OGM)
        fReason = fName = " my own ogm";

    if (reason == FORWARD_OGM)
    {
        ss << " ogm of another node - "; //<< ((batadv_ogm_packet*) skb)->getOrig().getMAC().str();
        fReason = fName = ss.str();
    }
    else if (reason == FORWARD_MY_TT_REQUEST)
        fReason = fName = " my TT request";
    else if (reason == FORWARD_MY_TT_RESPONSE)
        fReason = fName = " my TT response";
    else if (reason == FORWARD_TT_REQUEST)
    {
        ss << " TT request of another node - " << ((batadv_unicast_tvlv_packet*) skb)->getSrc().str();
        fReason = fName = ss.str();
    }
    else if (reason == FORWARD_TT_RESPONSE)
    {
        ss << " TT response of another node" << ((batadv_unicast_tvlv_packet*) skb)->getSrc().str();
        fReason = fName = ss.str();
    }
    else if (reason == ROUTE_UNICAST)
    {
        fReason = fName = " Reroute unicast";
    }
    else if (reason == GENERATED_TT_RESPONSE)
    {
        ss << " Answer TT response for node " << ((batadv_unicast_tvlv_packet*) skb)->getDst().str();
        fReason = fName = ss.str();
    }

    if (hard_iface.name == wlan0NP.name)
    {
        Ieee802Ctrl *ctrl = new Ieee802Ctrl();
        ctrl->setDest(dst_addr);
        ctrl->setSrc(ownAddress.getMAC());
        ctrl->setInterfaceId(0);
        ctrl->setEtherType(ETHERTYPE_L2_ISIS);
        EtherFrame* frame = new EtherFrame();
        frame->setDest(dst_addr);
        frame->setSrc(ownAddress.getMAC());
        AirFrame* af = new AirFrame();
        af->setControlInfo(ctrl);
        frame->encapsulate(skb);
        af->encapsulate(frame);
        af->setName(fName.c_str());

        if (dynamic_cast<batadv_ogm_packet *>(skb) != NULL || dynamic_cast<batadv_ogm_packet_agg *>(skb) != NULL)
        {
            if (simTime() >= warmupTime)
            {
                numOwnOGMSentCountWlan++;
                numOwnOGMSentCount++;
            }
        }
        else if (dynamic_cast<batadv_unicast_tvlv_packet *>(skb) != NULL)
        {
            EV << "Node: " << nodenr << ": Sending tvlv_packet unicast on interface wlan0" << std::endl;
        }
        else
            EV << "Node: " << nodenr << ": Transmitting package on interface wlan0" << std::endl;
        send(af, "wlan0$o");
        return NET_XMIT_SUCCESS;
    }
    if (!gwTermination || gw_mode == BATADV_GW_MODE_CLIENT)
    {
        if (hard_iface.name == meshvpnNP.name)
        {
            for (int i = 0; i < gateSize("meshVpno"); i++)
            {
                cGate *meshVPNGate = gate("meshVpno", i);    //Gate, index
                EtherFrame* frame2 = new EtherFrame(fName.c_str());
                frame2->setDest(dst_addr);
                frame2->setSrc(ownAddress.getMAC());
                frame2->encapsulate(skb->dup());
                Ieee802Ctrl *ctrl = new Ieee802Ctrl();
                ctrl->setDest(dst_addr);
                ctrl->setSrc(ownAddress.getMAC());
                ctrl->setInterfaceId(0);
                ctrl->setEtherType(ETHERTYPE_L2_ISIS);
                frame2->setControlInfo(ctrl);
                unsigned int msecs = 0;
                msecs = (unsigned long) originator_interval.dbl() * 1000 - BATADV_JITTER; //BATADVJitter in msecs
                unsigned long rNumber = longRandom();
                unsigned long jitter = rNumber % (2 * BATADV_JITTER);
                msecs += jitter;
                msecs = simTime().dbl() * 1000 + msecs;
                sendDelayed(frame2, (i + uniform(0, 10)) / 1000.0, meshVPNGate);
                if (dynamic_cast<batadv_unicast_tvlv_packet *>(skb) != NULL)
                {
                    EV << "Node: " << nodenr << ": Sending tvlv_packet unicast on interface meshVpno-" << i << std::endl;
                }
                else
                    EV << "Node: " << nodenr << ": Transmitting package on interface meshVpno-" << i << std::endl;
            }
            if (dynamic_cast<batadv_ogm_packet *>(skb) != NULL || dynamic_cast<batadv_ogm_packet_agg *>(skb) != NULL)
            {
                if (simTime() >= warmupTime)
                {
                    numOwnOGMSentCount++;
                    numOwnOGMSentCountVpn++;
                }
            }
            delete skb;
            return NET_XMIT_SUCCESS;
        }
    }
    if (gwTermination)
    {
        if (hard_iface.name == meshvpnNP.name)
        {
            /*
             * Check the (aggregated) packet and remove everything that doesn't contain gw tvlv data
             */
            batadv_ogm_packet_agg *aggNodes = new batadv_ogm_packet_agg();
            if (dynamic_cast<batadv_ogm_packet_agg *>(skb) != NULL)
            {
                batadv_ogm_packet_agg *agg = dynamic_cast<batadv_ogm_packet_agg *>(skb);
                std::vector<batadv_ogm_packetClass> ogmlistNoPointer = std::vector<batadv_ogm_packetClass>();
                batadv_ogm_packetClass tmp_packet;
                for (int k = 0; k < agg->getOgmsNoPointer().size(); k++)
                {
                    tmp_packet = agg->getOgmsNoPointer().at(k);
                    if (tmp_packet.gateway_data.getBandwidth_down() != 0 && tmp_packet.gateway_data.getBandwidth_up() != 0)
                        ogmlistNoPointer.push_back(tmp_packet);
                }
                if (ogmlistNoPointer.size() != 0)
                {
                    aggNodes->setOgmsNoPointer(ogmlistNoPointer);
                    skb = aggNodes;
                }
            }/*
             else
             delete aggNodes;*/
            for (int i = 0; i < gateSize("gwgo"); i++)
            {
                cGate *meshVPNGate = gate("gwgo", i);    //Gate, index
                EtherFrame* frame2 = new EtherFrame(fName.c_str());
                frame2->setDest(dst_addr);
                frame2->setSrc(ownAddress.getMAC());
                frame2->encapsulate(skb->dup());
                Ieee802Ctrl *ctrl = new Ieee802Ctrl();
                ctrl->setDest(dst_addr);
                ctrl->setSrc(ownAddress.getMAC());
                ctrl->setInterfaceId(0);
                ctrl->setEtherType(ETHERTYPE_L2_ISIS);
                frame2->setControlInfo(ctrl);
                unsigned int msecs = 0;
                msecs = (unsigned long) originator_interval.dbl() * 1000 - BATADV_JITTER; //BATADVJitter in msecs
                unsigned long rNumber = longRandom();
                unsigned long jitter = rNumber % (2 * BATADV_JITTER);
                msecs += jitter;
                msecs = simTime().dbl() * 1000 + msecs;
                sendDelayed(frame2, (i + uniform(0, 10)) / 1000.0, meshVPNGate);
                if (dynamic_cast<batadv_unicast_tvlv_packet *>(skb) != NULL)
                {
                    EV << "Node: " << nodenr << ": Sending tvlv_packet unicast on interface meshVpno-" << i << std::endl;
                }
                else
                    EV << "Node: " << nodenr << ": Transmitting package on interface meshVpno-" << i << std::endl;
            }
            if (dynamic_cast<batadv_ogm_packet *>(skb) != NULL)
            {
                batadv_ogm_packet *ogm = dynamic_cast<batadv_ogm_packet *>(skb);
                if (ogm->getGateway_data().getBandwidth_down() == 0 || ogm->getGateway_data().getBandwidth_up() == 0)
                {
                    delete ogm;
                    return NET_XMIT_SUCCESS;
                }
            }
            if (dynamic_cast<batadv_ogm_packet_agg *>(skb) != NULL && aggNodes->getOgmsNoPointer().size() == 0)
            {
                delete aggNodes;
                return NET_XMIT_SUCCESS;
            }
            for (int i = 0; i < gateSize("meshVpno"); i++)
            {
                cGate *meshVPNGate = gate("meshVpno", i);    //Gate, index
                EtherFrame* frame2 = new EtherFrame(fName.c_str());
                frame2->setDest(dst_addr);
                frame2->setSrc(ownAddress.getMAC());
                //frame2->setByteLength(0);
                if (dynamic_cast<batadv_ogm_packet *>(skb) != NULL)
                    frame2->encapsulate(skb->dup());
                if (dynamic_cast<batadv_ogm_packet_agg *>(skb) != NULL)
                    frame2->encapsulate(aggNodes->dup());
                Ieee802Ctrl *ctrl = new Ieee802Ctrl();
                ctrl->setDest(dst_addr);
                ctrl->setSrc(ownAddress.getMAC());
                ctrl->setInterfaceId(0);
                ctrl->setEtherType(ETHERTYPE_L2_ISIS);
                frame2->setControlInfo(ctrl);
                unsigned int msecs = 0;
                msecs = (unsigned long) originator_interval.dbl() * 1000 - BATADV_JITTER; //BATADVJitter in msecs
                unsigned long rNumber = longRandom();
                unsigned long jitter = rNumber % (2 * BATADV_JITTER);
                msecs += jitter;
                msecs = simTime().dbl() * 1000 + msecs;
                sendDelayed(frame2, (i + uniform(0, 10)) / 1000.0, meshVPNGate);
            }
            if (dynamic_cast<batadv_ogm_packet *>(skb) != NULL || dynamic_cast<batadv_ogm_packet_agg *>(skb) != NULL)
            {
                if (simTime() >= warmupTime)
                {
                    numOwnOGMSentCount++;
                    numOwnOGMSentCountVpn++;
                }
            }
            delete skb;
            return NET_XMIT_SUCCESS;

        }
    }
    return NET_XMIT_DROP;
}

/***
 *
 */
void BatmanADVMain::batadv_send_outstanding_bat_ogm_packet(double id, forwardReason reason)
{
//Delete from list of aggregated OGMs that will be forwarded
    batadv_forw_packet tmp_packet;
    batadv_forw_packet forw_packet;
    bool cprResult;
    EV << "Node: " << nodenr << ": Try to delete forw_bat_list " << std::endl;
    for (std::vector<batadv_forw_packet>::iterator it = forw_bat_list.begin(); it != forw_bat_list.end(); it++)
    {
        tmp_packet = *it;
        cprResult = tmp_packet.id == id;
        if (cprResult)
        {
            forw_packet = tmp_packet;
            forw_bat_list.erase(it);
            EV << "Node: " << nodenr << ": Delete and break -> new size is " << forw_bat_list.size() << std::endl;
            break;
        }
    }
    batadv_iv_ogm_emit(forw_packet, reason);
    if (forw_packet.own /*&& batman_queue_left > 0*/)
    {
        EV << "Node: " << nodenr << ": Creating new ogm_reminder " << std::endl;
        ogm_reminder *ogm_remin = new ogm_reminder();
        if (forw_packet.if_incomingName == meshvpnNP.name)
            ogm_remin->setHard_iface(meshvpnNP);
        else
            ogm_remin->setHard_iface(wlan0NP);
        simtime_t nextOGM = simTime() + originator_interval + uniform(0, 1);
        EV << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& Node: " << nodenr << ": Scheduling a new OGM for time " << nextOGM.str() << std::endl;
        scheduleAt(nextOGM, ogm_remin); //Reminder to send the OGM after "orignator_interval"
    }
    if (!forw_packet.own)
    {
        batman_queue_left++;
    }
}

void BatmanADVMain::batadv_send_outstanding_bat_ogm_packetNoOutgoing(double id, forwardReason reason)
{
//Delete from list of aggregated OGMs that will be forwarded
    batadv_forw_packet tmp_packet;
    batadv_forw_packet forw_packet;
    bool cprResult;
    EV << "Node: " << nodenr << ": Try to delete forw_bat_list " << std::endl;
    for (std::vector<batadv_forw_packet>::iterator it = forw_bat_list.begin(); it != forw_bat_list.end(); it++)
    {
        tmp_packet = *it;
        cprResult = tmp_packet.id == id;
        if (cprResult)
        {
            forw_packet = tmp_packet;
            forw_bat_list.erase(it);
            EV << "Node: " << nodenr << ": Delete and break -> new size is " << forw_bat_list.size() << std::endl;
            break;
        }
    }
    batadv_iv_ogm_emitNoOutgoing(forw_packet, reason);
    if (forw_packet.own /*&& forw_packet.if_incomingName == forw_packet.if_outgoingName /*&& batman_queue_left > 0*/)
    {
        EV << "Node: " << nodenr << ": Creating new ogm_reminder " << std::endl;
        ogm_reminder *ogm_remin = new ogm_reminder();
        if (forw_packet.if_incomingName == meshvpnNP.name)
            ogm_remin->setHard_iface(meshvpnNP);
        else
            ogm_remin->setHard_iface(wlan0NP);
        simtime_t nextOGM = simTime() + originator_interval + uniform(0, 1);

        if (iNodenr == -1)
            printf("\nNEXT OGM SCHEDULED FOR -- %f\n", nextOGM.dbl());

        EV << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& Node: " << nodenr << ": Scheduling a new OGM for time " << nextOGM.str() << std::endl;
        scheduleAt(nextOGM, ogm_remin); //Reminder to send the OGM after "orignator_interval"
    }
    if (!forw_packet.own)
    {
        batman_queue_left++;
    }
}

int BatmanADVMain::batadv_send_skb_via_gw(Ieee80211DataFrameWithSNAP* frame)
{
    batadv_orig_node *orig_node;
    orig_node = batadv_gw_get_selected_orig();
    return batadv_send_skb_unicast(frame, BATADV_UNICAST, 0, orig_node, 0);
}
int BatmanADVMain::batadv_send_skb_unicast(cPacket* skb, int packet_type, int packet_subtype, batadv_orig_node *orig_node, unsigned short vid)
{
    int ret = NET_XMIT_DROP;
    /***/
//START: This is not the correct way...
    batadv_unicast_packet* unicast = new batadv_unicast_packet();
    unicast->encapsulate(skb);
//END: This is not the correct way...
    if (!orig_node)
    {
        delete skb;
        return ret;
    }
    switch (packet_type)
    {
        case BATADV_UNICAST:
            if (!batadv_send_skb_prepare_unicast(unicast, orig_node)) //should be skb not unicast //TODO: Check if the skb's content has been changed afterwards
                return ret;
            break;
        case BATADV_UNICAST_4ADDR:
            break;
        default:
            /* this function supports UNICAST and UNICAST_4ADDR only. It
             * should never be invoked with any other packet type
             */
            break;
    }

//TODO: Roaming missing!
//batadv_unicast_packet *unicast_packet;
//unicast_packet = (batadv_unicast_packet *) skb;/
    if (batadv_send_skb_to_orig(unicast, orig_node, NULL, FORWARD_UNICAST) != NET_XMIT_DROP)
        ret = NET_XMIT_SUCCESS;
    return ret;
}
/**
 * batadv_send_skb_prepare_unicast - encapsulate an skb with a unicast header
 * @skb: the skb containing the payload to encapsulate
 * @orig_node: the destination node
 *
 * Returns false if the payload could not be encapsulated or true otherwise.
 */
bool BatmanADVMain::batadv_send_skb_prepare_unicast(cPacket *skb, batadv_orig_node *orig_node)
{
    long uni_size = sizeof(batadv_unicast_packet);    //TODO: Sizeissue -->

    return batadv_send_skb_push_fill_unicast(skb, uni_size, orig_node);
}
/**
 * batadv_send_skb_push_fill_unicast - extend the buffer and initialize the
 *  common fields for unicast packets
 * @skb: the skb carrying the unicast header to initialize
 * @hdr_size: amount of bytes to push at the beginning of the skb
 * @orig_node: the destination node
 *
 * Returns false if the buffer extension was not possible or true otherwise.
 */
bool BatmanADVMain::batadv_send_skb_push_fill_unicast(cPacket *skb, int hdr_size, batadv_orig_node *orig_node)
{
    batadv_unicast_packet *unicast_packet;
    unsigned int ttvn = orig_node->last_ttvn;

    /*if (batadv_skb_head_push(skb, hdr_size) < 0)
     return false;*/

    unicast_packet = (batadv_unicast_packet *) skb;
    unicast_packet->setVersion(BATADV_COMPAT_VERSION);
    /* batman packet type: unicast */
    unicast_packet->setPacket_type(BATADV_UNICAST);
    /* set unicast ttl */
    unicast_packet->setTtl(BATADV_TTL);
    /* copy the destination for faster routing */
    unicast_packet->setDest(orig_node->orig.getMAC());
    /* set the destination tt version number */
    unicast_packet->setTtvn(ttvn);

    return true;
}
/**
 * batadv_send_skb_via_tt - send an skb via TT lookup
 * @skb: the payload to send
 *
 * Look up the recipient node for the destination address in the ethernet
 * header via the translation table. Wrap the given skb into a batman-adv
 * unicast header. Then send this frame to the according destination node.
 *
 * Returns NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int BatmanADVMain::batadv_send_skb_via_tt(Ieee80211DataFrameWithSNAP* frame, MACAddress dst_hint)
{
    batadv_send_skb_via_tt_generic(frame, BATADV_UNICAST, 0, dst_hint);
}
/**
 * batadv_send_skb_via_tt_generic - send an skb via TT lookup
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @packet_type: the batman unicast packet type to use
 * @packet_subtype: the unicast 4addr packet subtype (only relevant for unicast
 *  4addr packets)
 * @dst_hint: can be used to override the destination contained in the skb
 * @vid: the vid to be used to search the translation table
 *
 * Look up the recipient node for the destination address in the ethernet
 * header via the translation table. Wrap the given skb into a batman-adv
 * unicast or unicast-4addr header depending on whether BATADV_UNICAST or
 * BATADV_UNICAST_4ADDR was supplied as packet_type. Then send this frame
 * to the according destination node.
 *
 * Returns NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int BatmanADVMain::batadv_send_skb_via_tt_generic(cPacket *skb, int packet_type, int packet_subtype, MACAddress dst_hint)
{
    Ieee80211DataFrameWithSNAP* frame = (Ieee80211DataFrameWithSNAP*) skb;
    batadv_orig_node *orig_node;
    MACAddress src = frame->getTransmitterAddress();
    MACAddress dst = frame->getReceiverAddress();
    orig_node = batadv_transtable_search(&src, &dst, 0);

    batadv_send_skb_unicast(frame, packet_type, packet_subtype, orig_node, 0);

}

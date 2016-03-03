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

#include <hardInterface.h>
#include <BatmanADVMain.h>

hardInterface::hardInterface()
{
}

hardInterface::~hardInterface()
{
}

/**
 * batadv_is_wifi_netdev - check if the given net_device struct is a wifi
 *  interface
 * @net_device: the device to check
 *
 * Returns true if the net device is a 802.11 wireless device, false otherwise.
 */
bool BatmanADVMain::batadv_is_wifi_netdev(batadv_hard_iface* device)
{
    if(device->name==wlanName)
        return true;
    else
        return false;/*
    if(!device || device == meshvpn)
        return false;
    return true;*/
}

void BatmanADVMain::batadv_interface_rx(cMessage* msg, batadv_hard_iface *recv_if, batadv_orig_node *orig_node)
{
    frame = (EtherFrame*) msg;
    EtherFrame* f = getCurrentFrame();
    batadv_unicast_packet* innerPacketUnicast = (batadv_unicast_packet*) f->decapsulate();
    Ieee80211DataFrameWithSNAP* orig_frame = dynamic_cast<Ieee80211DataFrameWithSNAP *>(innerPacketUnicast->decapsulate());
    Ieee80211DataFrameWithSNAP* frame = orig_frame->dup();
    if (frame == NULL)
    {
        delete frame;
        return;
    }
    /* check for IP header  */
    IPv4Datagram* v4D = dynamic_cast<IPv4Datagram *>(frame->decapsulate());
    if (v4D == NULL)
    {
        delete frame;
        return;
    }

    /* check for udp header  */
    UDPPacket* udp = dynamic_cast<UDPPacket *>(v4D->decapsulate());
    if (udp == NULL)
    {
        delete frame;
        return;
    }
    cPacket* dhcpThing = udp->decapsulate();
    if (dynamic_cast<DHCPMessage*>(dhcpThing) != NULL)
    {
        send(dhcpThing, "upperLayerOut");
    }
}

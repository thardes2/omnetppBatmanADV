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

package inet.batmanadv.device;
import inet.batmanadv.BatmanADVMain;
import inet.linklayer.IMACAddressTable;
import inet.linklayer.IMACRelayUnit;
import inet.nodes.inet.WirelessHost;
import inet.networklayer.IManetRouting;
import inet.nodes.inet.AdhocHost;
import inet.base.NotificationBoard;
import inet.linklayer.ieee80211.Ieee80211Nic;
import inet.nodes.wireless.AccessPoint;
import inet.mobility.contract.IMobility;
import inet.networklayer.autorouting.ipv4.HostAutoConfigurator;
import inet.linklayer.IWirelessNic;
import inet.networklayer.common.InterfaceTable;
import inet.linklayer.IWiredNic;
import inet.status.NodeStatus;

module FreifunkRouter
{
    parameters:
        //routingProtocol = "BatmanADVMain";
        //IPForward = true;    
        @node;
        @labels(node,ethernet-node,wireless-node);
        @display("i=device/accesspoint;");
        int numRadios = default(2);
        string mobilityType = default("StationaryMobility");
        string routingProtocol @enum("AODVUU","DYMOUM","DYMO","DSRUU","OLSR","OLSR_ETX","DSDV_2","Batman") = default("");  // used mobile routing protocol. see: inet.networklayer.manetrouting
        string relayUnitType = default("MACRelayUnit");
        string ralayMacTableType = default("MACAddressTable"); // type of the ~IMACAddressTable
        bool hasStatus = default(false);
        bool wifiOnly;
        int meshId;
        bool cluster;
        bool gwTermination;

        //int nodenr;
        string nodenr;
        int ogm_ttl = default(3);
        int numClients = default(0);
        string mac = default("co:ff:ee:ba:be:01");
        int GWClass_download_speed @unit(kbps) = default(0);    // internet access download speed on preferedGateway, 0 means: this node doesn't have direct internet access
        int GWClass_upload_speed @unit(kbps) = default(0);      // internet access upload speed on preferedGateway, 0 means: this node doesn't have direct internet access
        int nodeMaxStarttime = default(3);

        networkLayer.configurator.networkConfiguratorModule = default("");
        wlan[0].mgmtType = dfault("Ieee80211MgmtAdhoc");
        wlan[1].mgmtType = "Ieee80211MgmtAP";
    gates:
        input radioIn[2] @directIn;
        inout ethg[] @labels(EtherFrame-conn);
    submodules:

        // linklayer
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=53,386");
        }
        macTable: <"MACAddressTable"> like IMACAddressTable {
            @display("p=73,19;is=s");
        }
        status: NodeStatus if hasStatus {
            @display("p=50,50");
        }
        //relayUnit: <"BatmanADVMain"> like IMACRelayUnit {
        //    parameters:
               // nodenr = nodenr;
                //ogm_ttl = ogm_ttl;
                //numClients = numClients;
                //mac = mac;
                //GWClass_download_speed = GWClass_download_speed;
                //GWClass_upload_speed = GWClass_upload_speed;
                //@display("p=421,152");

        //}
        batmanADV: BatmanADVMain {
            parameters:
                @display("i=batmanSmall;p=286,205");
                nodenr = nodenr;
                ogm_ttl = ogm_ttl;
                numClients = numClients;
                mac = mac;
                GWClass_download_speed = GWClass_download_speed;
                GWClass_upload_speed = GWClass_upload_speed;
                nodeMaxStarttime = nodeMaxStarttime;
                meshId = meshId;
                gwTermination = gwTermination;
                cluster = cluster;
                wifiOnly = wifiOnly;
        }

        notificationBoard: NotificationBoard {
            parameters:
                @display("p=53,194");
        }
         // optional mobility module. Required only if wireless cards are present
        mobility: <mobilityType> like IMobility if numRadios>0 {
            parameters:
                @display("p=53,121");
        }
          // network layer
        eth[sizeof(ethg)]: <default("EthernetInterface")> like IWiredNic {//-1
            parameters:
                @display("p=415,331,row,90;q=txQueue");
        }
        wlan0: <default("Ieee80211Nic")> like IWirelessNic {
            parameters:
                @display("p=286,314;q=queue");
                mgmt.encapDecap = "false";
        }
        wlan1: <default("Ieee80211Nic")> like IWirelessNic {
            parameters:
                mgmt.forceRelayUnit = true;
                mgmt.encapDecap = "false";
                @display("p=189,331;q=queue");
        }
    connections allowunconnected:

        radioIn[0] --> wlan0.radioIn;
        wlan0.upperLayerOut --> batmanADV.wlan0$i;
        wlan0.upperLayerIn <-- batmanADV.wlan0$o;
        radioIn[1] --> wlan1.radioIn;
        wlan1.upperLayerOut --> batmanADV.wlan1$i;
        wlan1.upperLayerIn <-- batmanADV.wlan1$o;
        for i=0..sizeof(ethg)-1 {
            eth[i].upperLayerIn <-- batmanADV.meshVpno++;
            eth[i].upperLayerOut --> batmanADV.meshVpni++;
            eth[i].phys <--> ethg[i];
        }
}

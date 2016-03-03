
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
import inet.nodes.inet.StandardHost;
import inet.linklayer.IMACRelayUnit;
import inet.networklayer.IManetRouting;
import inet.linklayer.IMACAddressTable;
import inet.base.NotificationBoard;
import inet.mobility.contract.IMobility;
import inet.networklayer.autorouting.ipv4.HostAutoConfigurator;
import inet.linklayer.IWiredNic;
import inet.linklayer.loopback.LoopbackInterface;
import inet.networklayer.common.InterfaceTable;


import inet.applications.IPingApp;
import inet.applications.ISCTPApp;
import inet.applications.ITCPApp;
import inet.applications.IUDPApp;
import inet.transport.ISCTP;
import inet.transport.ITCP;
import inet.transport.IUDP;

import inet.nodes.inet.NetworkLayer;
import inet.networklayer.ipv4.RoutingTable;

module FreifunkGateway
{
    parameters:
        @node;
        @labels(node,ethernet-node,wireless-node);
        @display("i=device/server");
        int numRadios = default(2);
        string mobilityType = default("StationaryMobility");
		bool cluster;
		bool gwTermination;
        //int nodenr;
        string nodenr;
        int ogm_ttl = default(3);
        int numClients = default(0);
        string mac = default("");
        int GWClass_download_speed @unit(kbps) = default(0);    // internet access download speed on preferedGateway, 0 means: this node doesn't have direct internet access
        int GWClass_upload_speed @unit(kbps) = default(0);      // internet access upload speed on preferedGateway, 0 means: this node doesn't have direct internet accessnodeMaxStarttime
        int nodeMaxStarttime = default(3);
        int meshId;
        
        bool wifiOnly;
        
		int numTcpApps = default(0);  // no of TCP apps. Specify the app types in INI file with tcpApp[0..1].typename="TCPEchoApp" syntax
        int numUdpApps = default(0);  // no of UDP apps. Specify the app types in INI file with udpApp[0..1].typename="UDPVideoStreamCli" syntax
        int numSctpApps = default(0); // no of SCTP apps. Specify the app types in INI file with sctpApp[0..1].typename="SCTPServer" syntax
        int numPingApps = default(0);  // no of PING apps. Specify the app types in INI file with pingApp[0..1].typename="PingApp" syntax
        bool hasTcp = default(numTcpApps>0);
        bool hasUdp = default(numUdpApps>0);
        bool hasSctp = default(numSctpApps>0);
         string tcpType = default(firstAvailable("TCP", "TCP_lwIP", "TCP_NSC", "TCP_None"));  // tcp implementation (e.g. ~TCP, ~TCP_lwIP, ~TCP_NSC) or ~TCPSpoof
        string udpType = default(firstAvailable("UDP","UDP_None"));
        string sctpType = default(firstAvailable("SCTP","SCTP_None"));
      
        networkLayer.proxyARP = default(false);
          string routingFile = default("");
        bool IPForward = default(true);
        bool forwardMulticast = default(false);
    gates:
        inout ethg[] @labels(EtherFrame-conn);
        inout gwg[] @labels(EtherFrame-conn);
    submodules:
        // linklayer
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=53,386");
        }
        macTable: <"MACAddressTable"> like IMACAddressTable {
            @display("p=73,19;is=s");
        }
           tcpApp[numTcpApps]: <> like ITCPApp {
            parameters:
                @display("p=147,54,row,60");
        }
        tcp: <tcpType> like ITCP if hasTcp {
            parameters:
                @display("p=147,141");
        }
        udpApp[numUdpApps]: <> like IUDPApp {
            parameters:
                @display("p=329,54,row,60");
        }
        udp: <udpType> like IUDP if hasUdp {
            parameters:
                @display("p=329,141");
        }
          // network layer
        networkLayer: NetworkLayer {
            parameters:
                @display("p=329,287;q=queue");
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
        relayUnit: BatmanADVMain {
            parameters:
                nodenr = nodenr;
                ogm_ttl = ogm_ttl;
                numClients = numClients;
                mac = mac;
                GWClass_download_speed = GWClass_download_speed;
                GWClass_upload_speed = GWClass_upload_speed;
                nodeMaxStarttime = nodeMaxStarttime;
                meshId = meshId;
                cluster = cluster;
                gwTermination = gwTermination;
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
           // network layer
        gw[sizeof(gwg)]: <default("EthernetInterface")> like IWiredNic {//-1
            parameters:
                @display("p=415,331,row,90;q=txQueue");
        }
        routingTable: RoutingTable {
            parameters:
                @display("p=53,225;is=s");
                IPForward = IPForward;
                forwardMulticast = forwardMulticast;
                routingFile = routingFile;
        }
    connections allowunconnected:
        for i=0..sizeof(ethg)-1 {
            eth[i].upperLayerIn <-- relayUnit.meshVpno++;
            eth[i].upperLayerOut --> relayUnit.meshVpni++;
            eth[i].phys <--> ethg[i];
        } for i=0..sizeof(gwg)-1 {
            gw[i].upperLayerIn <-- relayUnit.gwgo++;
            gw[i].upperLayerOut --> relayUnit.gwgi++;
            gw[i].phys <--> gwg[i];
        }
        for i=0..numTcpApps-1 {
            tcpApp[i].tcpOut --> tcp.appIn++;
            tcpApp[i].tcpIn <-- tcp.appOut++;
        }

        tcp.ipOut --> networkLayer.transportIn++ if hasTcp;
        tcp.ipIn <-- networkLayer.transportOut++ if hasTcp;
          for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
        }
//        udp.ipOut --> networkLayer.transportIn++ if hasUdp;
//        udp.ipIn <-- networkLayer.transportOut++ if hasUdp;

        udp.ipOut --> relayUnit.upperLayerIn if hasUdp;
        udp.ipIn <-- relayUnit.upperLayerOut if hasUdp;
}

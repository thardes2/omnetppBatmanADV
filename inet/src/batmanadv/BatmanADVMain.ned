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

package inet.batmanadv;
import inet.networklayer.manetrouting.base.BaseRouting;
import inet.networklayer.IManetRouting;
import inet.nodes.inet.AdhocHost;
import inet.nodes.inet.WirelessHost;
import inet.linklayer.IMACRelayUnit;
import inet.base.NotificationBoard;

import inet.linklayer.ieee80211.mgmt.IIeee80211Mgmt;

simple BatmanADVMain //like IMACRelayUnit
{
    parameters:
        @display("i=block/routing"); // add a default icon
        @class(BatmanADVMain);
        string ssid = default("SSID");
        int nodeMaxStarttime=default(3);
        //int nodenr;
        string nodenr;
        int debugLevel = default(4);
        //wlan[*].mgmtType = default("Ieee80211MgmtAdhoc");  // use adhoc management
        double purgeTimeout @unit(s) = default(200s);
        double originatorInterval @unit(s) = default(5s);   // originator message interval
        string preferedGateWay = default("");
        string mac = default("co:ff:ee:ba:be:01");
        int ogm_ttl = default(3);
        int numClients = default(0);
        int hopPenalty = default(30);//cat /sys/class/net/bat0/mesh/hop_penalty -->30
        bool wifiOnly;
		bool gwTermination;
        int sizeBueren;
        int sizePaderborn;
        int sizeBaliSchlangenHoevelhof;
        int sizeRemainder;
		int meshId;
		bool cluster;
        int gatewayMode = default(20);
        // this parameter is used to set the routing behaviour
        // *  0: set no default route
        // *  1: use fast internet connection (gw_flags * TQ)
        // *  2: use stable internet connection (TQ)
        // *  3: use fast-switch internet connection (TQ but switch as soon as a better gateway appears)
        int routingClass = default(0);
        bool aggregationEnable = default(true);     // enable/disable multiple batman packet sending/receiving in one UDP packet
        int GWClass_download_speed @unit(kbps) = default(0);    // internet access download speed on preferedGateway, 0 means: this node doesn't have direct internet access
        int GWClass_upload_speed @unit(kbps) = default(0);      // internet access upload speed on preferedGateway, 0 means: this node doesn't have direct internet access
        volatile double jitter @unit(s) = default(uniform (0s,0.1s));       // 0..JITTER
        volatile double jitter2 @unit(s) = default(uniform (-0.1s,0.1s));   // -JITTER..+JITTER
        double MAX_AGGREGATION_MS @unit(s) = default(0.1s);                 // MAX_AGGREGATION_MS >= JITTER
        int MAX_AGGREGATION_BYTES @unit(B) = default(512B);
        double desynchronized @unit(s) = default(0s);
        volatile double broadcastDelay @unit("s") = default(uniform(0s,0.005s));  // the delay added to broadcast operations if EqualDelay is set (used to model processing time)
        bool PublicRoutingTables = default(false);
        string announcedNetworks = default("");


        int frameCapacity = default(100);
        bool useHwmp = default(false); // If active automatically deactivate useProactive and useReactive and activate ETXEstimate
        bool useProactive = default(false);
        bool useReactive = default(true);

        bool useGreenie = default(false);
        bool greenieCompleteNode = default(true);

        bool FixNode = default(false); // used by routing protocol to indicate no mobility node
        bool UseLwMpls = default(false);
        double maxDelay = default(0.1);
        int maxTTL = default(32); // the same that IP
        bool ETXEstimate = default(false);
        bool IsGateWay = default(false);
        double GateWayAnnounceInterval @unit("s") = default(100s);
        volatile double MacBroadcastDelay @unit("s") = default(uniform(0s,0.01s));
        bool forceRelayUnit = default(false);
        bool floodingConfirmation = default(false);
        // pending of remove variables
        //         bool ProactiveFeedback = default(true);
        //        int maxHopProactiveFeedback = default(-1); // Maximun number of hops for to use the proactive feedback, not used yet, TO-DO
        //        int maxHopProactive = default(-1); // Maximun number of hops in the fix part of the network with the proactive feedback
        //        int maxHopReactive = default(-1); // Maximun number of hops by the reactive part for to use the proactive feedback, not used yet, TO-DO
        bool locatorActive = default(false);
        double coverageArea @unit(m) = default(-1m);
        bool inteligentForward = default(false);
        double lifeTimeForInterface @unit(s) = default(5s);
        string selectionCriteria @enum("Etx","MinQueue","LastUsed","MinQueueLastUsed","LastUsedMinQueue") = default("MinQueue");// if ETX active the ETX proc
        bool inteligentBroadcastRouting = default(false);
        @signal[enqueuePk](type=cMessage);
        @signal[dequeuePk](type=cMessage);
        @signal[dropPkByQueue](type=cMessage);
        @signal[queueingTime](type=simtime_t; unit=s);
        @signal[dataQueueLen](type=long);  // number of stored packets
        @signal[numHopsSignal](type=long);
        @signal[numFixHopsSignal](type=long);
        @statistic[numHops](title="numHops"; source=numHopsSignal; record=histogram ; interpolationmode=none);
        @statistic[numFixHops](title="numHops fix"; source=numFixHopsSignal; record=histogram ; interpolationmode=none);
        @statistic[dataQueueLen](title="data queue length"; record=max,timeavg,vector; interpolationmode=sample-hold);
        @statistic[dropPkByQueue](title="packets dropped by queue"; source=dropPkByQueue; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);




    gates:
        input from_ip @loose;//upperLayerIn
        output to_ip @loose; //upperLayerOut
        inout bat0 @labels(bat0) @loose;
        inout batAdv @labels(batAdv) @loose;
        inout meshVpn[] @labels(meshVpn) @loose;
        input meshVpni[] @labels(meshVpn) @loose;
        output meshVpno[] @labels(meshVpn) @loose;
        input gwgi[] @labels(meshVpn) @loose;
        output gwgo[] @labels(meshVpn) @loose;
        inout brWAN @labels(brWAN) @loose;
        inout brClient @labels(brClient) @loose;
        inout eth01[4] @labels(eth01) @loose;
        inout eth02 @labels(eth01) @loose;
        inout wifi @labels(wifi) @loose;
        inout wlan0 @labels(wifi) @loose;
        inout wlan1 @labels(wifi) @loose;
        input ifIn[] @labels(EtherFrame);
        output ifOut[] @labels(EtherFrame);
        input stpIn @loose;
        output stpOut @loose;
        input upperLayerIn @loose;
        output upperLayerOut @loose;
       // input macIn @labels(Ieee80211Frame) @loose;
       // output macOut @labels(Ieee80211Frame) @loose;
       // input agentIn @loose;   // optional gates used for the agent in stations
       // output agentOut @loose; // optional gates used for the agent in stations
}

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

import ned.DatarateChannel;
import inet.batmanadv.nonMesh.NonMeshClient;
import inet.batmanadv.nonMesh.NonMeshHost;
import inet.batmanadv.device.FreifunkRouter;
import inet.batmanadv.device.FreifunkGateway;
import inet.world.radio.ChannelControl;
import inet.nodes.wireless.AccessPoint;
import inet.nodes.inet.WirelessHost;
import inet.nodes.inet.AdhocHost;
import inet.base.LifecycleController;
import inet.world.scenario.ScenarioManager;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.world.radio.IdealChannelModel;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.batmanadv.nonMesh.NonMeshClient;
import inet.batmanadv.statistics.StatisticsCollector;
network ffpb_network
{
parameters:
        int clientnr;
    types:
        channel meshVPNChannel extends DatarateChannel
        {
            delay = 50ns;
            datarate = 10Mbps;
        }
        channel wifiMeshChannel extends DatarateChannel
        {
            delay = 50ns;
            datarate = 300Mbps;
        }
    submodules:
		lifecycleController: LifecycleController {
            @display("p=73,185");
        }
statisticsCollector: StatisticsCollector;
scenarioManager: ScenarioManager {
            script = xmldoc("scenario.xml");
            @display("p=100,100");
        }

channelControl: ChannelControl {
                    parameters:
                    @display("p=50,50");
                }

gw0:  FreifunkGateway{
        @display("i=device/server");
        };
gw1:  FreifunkGateway{
        @display("i=device/server");
        };
gw2:  FreifunkGateway{
        @display("i=device/server");
        };
gw3:  FreifunkGateway{
        @display("i=device/server");
        };
gw4:  FreifunkGateway{
        @display("i=device/server");
        };
gw5:  FreifunkGateway{
        @display("i=device/server");
        };
gw6:  FreifunkGateway{
        @display("i=device/server");
        };
gw7:  FreifunkGateway{
        @display("i=device/server");
        };
ap0: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap0_0: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap1: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap1_1: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap2: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap3: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap4: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap5: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap5_0: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap5_1: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap6: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap6_0: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap7: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap8: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap9: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap9_0: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap9_3: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
ap10: FreifunkRouter {
            parameters:
                @display("i=device/accesspoint");
        }
connections allowunconnected:

///////////////////////////////////// Nodes: Bueren /////////////////////////////////////
gw0.ethg++<--> meshVPNChannel <-->ap3.ethg++;//Bueren
gw0.ethg++<--> meshVPNChannel <-->ap5.ethg++;//Bueren
gw0.ethg++<--> meshVPNChannel <-->ap5_1.ethg++;//Bueren
gw0.ethg++<--> meshVPNChannel <-->ap9.ethg++;//Bueren
gw0.ethg++<--> meshVPNChannel <-->ap9_0.ethg++;//Bueren

///////////////////////////////////// Nodes: Paderborn /////////////////////////////////////
gw1.ethg++<--> meshVPNChannel <-->ap0.ethg++;//Paderborn
gw1.ethg++<--> meshVPNChannel <-->ap0_0.ethg++;//Paderborn
gw1.ethg++<--> meshVPNChannel <-->ap6.ethg++;//Paderborn
gw1.ethg++<--> meshVPNChannel <-->ap6_0.ethg++;//Paderborn
gw1.ethg++<--> meshVPNChannel <-->ap8.ethg++;//Paderborn


///////////////////////////////////// Nodes: BaliSchlangenHoevelhof /////////////////////////////////////
gw2.ethg++<--> meshVPNChannel <-->ap2.ethg++;//BaliSchlangenHoevelhof


///////////////////////////////////// Nodes: Remainder /////////////////////////////////////
gw3.ethg++<--> meshVPNChannel <-->ap1.ethg++;//Remainder
gw3.ethg++<--> meshVPNChannel <-->ap4.ethg++;//Remainder
gw3.ethg++<--> meshVPNChannel <-->ap7.ethg++;//Remainder

///////////////////////////////////// Nodes: Gateways /////////////////////////////////////
gw0.gwg++<--> meshVPNChannel <-->gw1.gwg++;
gw0.gwg++<--> meshVPNChannel <-->gw2.gwg++;
gw0.gwg++<--> meshVPNChannel <-->gw3.gwg++;
gw0.gwg++<--> meshVPNChannel <-->gw4.gwg++;
gw0.gwg++<--> meshVPNChannel <-->gw5.gwg++;
gw0.gwg++<--> meshVPNChannel <-->gw6.gwg++;
gw0.gwg++<--> meshVPNChannel <-->gw7.gwg++;
gw1.gwg++<--> meshVPNChannel <-->gw2.gwg++;
gw1.gwg++<--> meshVPNChannel <-->gw3.gwg++;
gw1.gwg++<--> meshVPNChannel <-->gw4.gwg++;
gw1.gwg++<--> meshVPNChannel <-->gw5.gwg++;
gw1.gwg++<--> meshVPNChannel <-->gw6.gwg++;
gw1.gwg++<--> meshVPNChannel <-->gw7.gwg++;
gw2.gwg++<--> meshVPNChannel <-->gw3.gwg++;
gw2.gwg++<--> meshVPNChannel <-->gw4.gwg++;
gw2.gwg++<--> meshVPNChannel <-->gw5.gwg++;
gw2.gwg++<--> meshVPNChannel <-->gw6.gwg++;
gw2.gwg++<--> meshVPNChannel <-->gw7.gwg++;
gw3.gwg++<--> meshVPNChannel <-->gw4.gwg++;
gw3.gwg++<--> meshVPNChannel <-->gw5.gwg++;
gw3.gwg++<--> meshVPNChannel <-->gw6.gwg++;
gw3.gwg++<--> meshVPNChannel <-->gw7.gwg++;
gw4.gwg++<--> meshVPNChannel <-->gw5.gwg++;
gw4.gwg++<--> meshVPNChannel <-->gw6.gwg++;
gw4.gwg++<--> meshVPNChannel <-->gw7.gwg++;
gw5.gwg++<--> meshVPNChannel <-->gw6.gwg++;
gw5.gwg++<--> meshVPNChannel <-->gw7.gwg++;
gw6.gwg++<--> meshVPNChannel <-->gw7.gwg++;
}

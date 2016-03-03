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

package inet.examples.wireless.dynamicNetwork;

import inet.nodes.inet.WirelessHost;
import inet.world.scenario.ScenarioManager;
import inet.world.radio.ChannelControl;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;

network DynamicRadioNetwork
{
    
    submodules:
        channelControl: ChannelControl {
            parameters:
                @display("p=50,50");
        }
        destinationNode: WirelessHost {
            parameters:
                //@display("p=250,50");
        }
        sourceNode[0]: WirelessHost {
            parameters:
                @dynamic(true);
                //@display("p=250,150");
        }
        configurator: IPv4NetworkConfigurator {
            parameters:
                @display("p=100,150");
        }
        scenarioManager: ScenarioManager {
            parameters:
                @display("p=100,250");
        }
}


package inet.tictoc;

import inet.world.scenario.ScenarioManager;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;

import inet.world.radio.ChannelControl;

simple Txc1
{
    gates:
        input in;
        output out;
}

network Tictoc1
{
    submodules:
        tic: Txc1 {
            @display("p=19,19");
            @dynamic(true);
        }
        toc: Txc1 {
            @display("p=10,10");
            @dynamic(true);
        }
        scenarioManager: ScenarioManager {
            script = xmldoc("scenario.xml");
            @display("p=100,100");
        }
channelControl: ChannelControl {
            parameters:
                @display("p=50,50");
        }
        configurator: IPv4NetworkConfigurator {
            parameters:
                @display("p=100,150");
        }
    connections:
        //tic.out --> {  delay = 100ms; } --> toc.in;
        //tic.in <-- {  delay = 100ms; } <-- toc.out;
}


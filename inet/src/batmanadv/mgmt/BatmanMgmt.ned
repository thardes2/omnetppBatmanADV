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

package inet.batmanadv.mgmt;
import inet.linklayer.ieee80211.mgmt.IIeee80211Mgmt;

module BatmanMgmt like IIeee80211Mgmt
{
    @macRouting;  // Mac layer with routing protocol working in the level 2
    gates:
        input macIn @labels(Ieee80211Frame);
        output macOut @labels(Ieee80211Frame);
        input upperLayerIn;
        output upperLayerOut;
        input agentIn;   // optional gates used for the agent in stations
        output agentOut; // optional gates used for the agent in stations
}

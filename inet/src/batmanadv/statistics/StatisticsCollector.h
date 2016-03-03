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

#ifndef __INET_STATISTICSCOLLECTOR_H_
#define __INET_STATISTICSCOLLECTOR_H_

#include <omnetpp.h>
#include "MACAddress.h"

class StatisticsCollector : public cSimpleModule
{
    private:
        int localClientCountAdd;
        int globalClientCountAdd;
        int localClientCountDel;
        int globalClientCountDel;
        int overallNodeCount;
        int tt_reqsend;
        int tt_reqrcv;
        int tt_ogmReceived;
        int tt_reqAnswered;

        cOutVector ogmReceivedBueren;
        cOutVector ogmReceivedPaderborn;
        cOutVector ogmReceivedBaliSchlangenHoevelhof;
        cOutVector ogmReceivedRemainder;

        cOutVector ogmSentBueren;
        cOutVector ogmSentPaderborn;
        cOutVector ogmSentBaliSchlangenHoevelhof;
        cOutVector ogmSentRemainder;

        cOutVector ogmAggregatedBueren;
        cOutVector ogmAggregatedPaderborn;
        cOutVector ogmAggregatedBaliSchlangenHoevelhof;
        cOutVector ogmAggregatedRemainder;
        cOutVector ogmAggregatedBuerenAvg;
        cOutVector ogmAggregatedPaderbornAvg;
        cOutVector ogmAggregatedBaliSchlangenHoevelhofAvg;
        cOutVector ogmAggregatedRemainderAvg;

        cOutVector ogmByteSizeBuerenAvg;
        cOutVector ogmByteSizePaderbornAvg;
        cOutVector ogmByteSizeBaliSchlangenHoevelhofAvg;
        cOutVector ogmByteSizeRemainderAvg;

        cOutVector ogmByteSizeBuerenSendAvg;
        cOutVector ogmByteSizePaderbornSendAvg;
        cOutVector ogmByteSizeBaliSchlangenHoevelhofSendAvg;
        cOutVector ogmByteSizeRemainderSendAvg;

        cOutVector ogmAggregatedBuerenWifi;
        cOutVector ogmAggregatedPaderbornWifi;
        cOutVector ogmAggregatedBaliSchlangenHoevelhofWifi;
        cOutVector ogmAggregatedRemainderWifi;

        int sizeBueren;
        int sizePaderborn;
        int sizeBaliSchlangenHoevelhof;
        int sizeRemainder;

        cOutVector localNodeClientPropagation;
        simtime_t warmupTime;
    public:
        void add_tt_reqSend();
        int get_tt_reqSend();
        void add_tt_reqrcv();
        int get_tt_reqrcv();
        void add_tt_ogmReceived();
        int get_tt_ogmReceived();

        void add_sizeBueren();
        void add_sizePaderborn();
        void add_sizeBaliSchlangenHoevelhof();
        void add_sizeRemainder();
        int get_sizeBueren();
        int get_sizePaderborn();
        int get_sizeBaliSchlangenHoevelhof();
        int get_sizeRemainder();

        void add_tt_reqAnswered();
        int get_tt_reqAnswered();

        void addOverallNodeCount();
        int getOverallNodeCount();

        void addLocalClientCountAdd();
        int getLocalClientCountAdd();
        void addGlobalClientCountAdd(int meshID);
        int getGlobalClientCountAdd();

        void addLocalClientCountDel();
        int getLocalClientCountDel();
        void addGlobalClientCountDel();
        int getGlobalClientCountDel();

        void checkClientAllNodesReached();
        void checkClientAllNodesReachedLocal();
        void storeTimeForLocalClient();
        void storeTimeForGlobalClient();
        void checkAllNodesKnown(int currentCount, int meshID);
        void checkAllNodesKnownMac(MACAddress mac, int meshID, std::string nodenr);
        int batadv_compare_eth(MACAddress mac1, MACAddress mac2);
        void recordOgmReceivedVPN(int ogmReceived, int meshID);
        void recordOgmSentVPN(int ogmReceived, int meshID);
        void recordOgmAggregation(int ogmAggregated, int meshID);
        void recordOgmAggregationAvg(int ogmAggregated, int meshID);
        void recordwifiNode(int wifi, int meshID);
        void recordByteSize(int ogmByteSize, int meshID);
        void recordSendByteLength(int recordSendByteLength, int meshID);
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif

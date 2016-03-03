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

#include "StatisticsCollector.h"
#include "batadv.h"

Define_Module(StatisticsCollector);

void StatisticsCollector::initialize()
{
    tt_ogmReceived = 0;
    tt_reqrcv = 0;
    tt_reqsend = 0;
    localClientCountAdd = 0;
    localClientCountDel = 0;
    globalClientCountAdd = 0;
    globalClientCountDel = 0;
    overallNodeCount = 0;
    localNodeClientPropagation.setName("localNodeClientPropagation");
    ogmReceivedBueren.setName("ogmReceivedBueren");
    ogmReceivedPaderborn.setName("ogmReceivedPaderborn");
    ogmReceivedBaliSchlangenHoevelhof.setName("ogmReceivedBaliSchlangenHoevelhof");
    ogmReceivedRemainder.setName("ogmReceivedRemainder");
    ogmSentBueren.setName("ogmSentBueren");
    ogmSentPaderborn.setName("ogmSentPaderborn");
    ogmSentBaliSchlangenHoevelhof.setName("ogmSentBaliSchlangenHoevelhof");
    ogmSentRemainder.setName("ogmSentRemainder");

    ogmAggregatedBueren.setName("ogmAggregatedBueren");
    ogmAggregatedPaderborn.setName("ogmAggregatedPaderborn");
    ogmAggregatedBaliSchlangenHoevelhof.setName("ogmAggregatedBaliSchlangenHoevelhof");
    ogmAggregatedRemainder.setName("ogmAggregatedRemainder");

    ogmAggregatedBuerenAvg.setName("ogmAggregatedBuerenAvg");
    ogmAggregatedPaderbornAvg.setName("ogmAggregatedPaderbornAvg");
    ogmAggregatedBaliSchlangenHoevelhofAvg.setName("ogmAggregatedBaliSchlangenHoevelhofAvg");
    ogmAggregatedRemainderAvg.setName("ogmAggregatedRemainderAvg");

    ogmAggregatedBuerenWifi.setName("ogmAggregatedBuerenWifi");
    ogmAggregatedPaderbornWifi.setName("ogmAggregatedPaderbornWifi");
    ogmAggregatedBaliSchlangenHoevelhofWifi.setName("ogmAggregatedBaliSchlangenHoevelhofWifi");
    ogmAggregatedRemainderWifi.setName("ogmAggregatedRemainderWifi");

    ogmByteSizeBuerenAvg.setName("ogmByteSizeBuerenAvg");
    ogmByteSizePaderbornAvg.setName("ogmByteSizePaderbornAvg");
    ogmByteSizeBaliSchlangenHoevelhofAvg.setName("ogmByteSizeBaliSchlangenHoevelhofAvg");
    ogmByteSizeRemainderAvg.setName("ogmByteSizeRemainderAvg");

    ogmByteSizeBuerenSendAvg.setName("ogmByteSizeBuerenSendAvg");
    ogmByteSizePaderbornSendAvg.setName("ogmByteSizePaderbornSendAvg");
    ogmByteSizeBaliSchlangenHoevelhofSendAvg.setName("ogmByteSizeBaliSchlangenHoevelhofSendAvg");
    ogmByteSizeRemainderSendAvg.setName("ogmByteSizeRemainderSendAvg");

    sizeBueren = 0;
    sizePaderborn = 0;
    sizeBaliSchlangenHoevelhof = 0;
    sizeRemainder = 0;
    warmupTime = 50 * 1000;
}

void StatisticsCollector::handleMessage(cMessage *msg)
{
}

void StatisticsCollector::addOverallNodeCount()
{

    overallNodeCount += 1;

}
int StatisticsCollector::getOverallNodeCount()
{
    return overallNodeCount;
}

void StatisticsCollector::addLocalClientCountAdd()
{
    localClientCountAdd += 1;
    storeTimeForLocalClient();
    checkClientAllNodesReachedLocal();
}

int StatisticsCollector::getLocalClientCountAdd()
{
    return localClientCountAdd;
}

void StatisticsCollector::addGlobalClientCountAdd(int meshID)
{
    globalClientCountAdd += 1;

    if (meshID == buerenMeshID)
    {
        recordScalar("#clientCompleteReachedAllNodesBueren", simTime());
    }
    else if (meshID == pbMeshID)
    {
        recordScalar("#clientCompleteReachedAllNodesPb", simTime());
    }
    else if (meshID == hoevelhofBaliSchlangenMeshID)
    {
        recordScalar("#clientCompleteReachedAllNodesBali", simTime());
    }
    else if (meshID == remainderMeshID)
    {
        recordScalar("#clientCompleteReachedAllNodesRemainder", simTime());
    }
    /* storeTimeForGlobalClient();
     checkClientAllNodesReached();*/
}

int StatisticsCollector::getGlobalClientCountAdd()
{
    return globalClientCountAdd;
}

void StatisticsCollector::addLocalClientCountDel()
{
    localClientCountDel += 1;
}

int StatisticsCollector::getLocalClientCountDel()
{
    return localClientCountDel;
}

void StatisticsCollector::addGlobalClientCountDel()
{
    globalClientCountDel += 1;
}

int StatisticsCollector::getGlobalClientCountDel()
{
    return globalClientCountDel;
}

void StatisticsCollector::add_tt_reqSend()
{
    tt_reqsend += 1;
}
int StatisticsCollector::get_tt_reqSend()
{
    return tt_reqsend;
}
void StatisticsCollector::add_tt_reqrcv()
{
    tt_reqrcv += 1;
}
int StatisticsCollector::get_tt_reqrcv()
{
    return tt_reqrcv;
}
void StatisticsCollector::add_tt_ogmReceived()
{
    tt_ogmReceived += 1;
}
int StatisticsCollector::get_tt_ogmReceived()
{
    return tt_ogmReceived;
}
void StatisticsCollector::checkClientAllNodesReached()
{
    localNodeClientPropagation.record(globalClientCountAdd);

    recordScalar("#clientCompleteReachedAllNodes", simTime());
    EV << "Recorded sim time - all nodes reached " << simTime() << std::endl;
}
void StatisticsCollector::checkClientAllNodesReachedLocal()
{
    localNodeClientPropagation.record(localClientCountAdd);
    recordScalar("#clientCompleteReachedAllNodes", simTime());
    EV << "Recorded sim time - all nodes reached " << simTime() << std::endl;
}
void StatisticsCollector::storeTimeForLocalClient()
{
    recordScalar("#clientAdded", simTime());
}
void StatisticsCollector::storeTimeForGlobalClient()
{
    recordScalar("#clientAdded", simTime());
}

int StatisticsCollector::batadv_compare_eth(MACAddress mac1, MACAddress mac2)
{
    int compareResult = mac1.compareTo(mac2);
    return compareResult;
}
void StatisticsCollector::checkAllNodesKnownMac(MACAddress mac, int meshID, std::string nodenr)
{
    double simt = simTime().dbl();
    if(meshID==noClusterMeshID&&simt>75)
         int x = 10;
    if(meshID==hoevelhofBaliSchlangenMeshID&&simt>60)
         int x = 10;
    if (batadv_compare_eth(mac, MACAddress("12:34:56:78:9A:BA")) == 0)
    {
        if (simt > 75.0)
            int x = 5;
        if (nodenr.compare("1_1"))
            int x = 10;

      //  printf("Node %s at time %f\n",nodenr.c_str(),simt);
        recordScalar("#allNodesKnown", simTime());
    }
    if (meshID == buerenMeshID)
    {
        if (batadv_compare_eth(mac, MACAddress("12:34:56:78:9A:BD")) == 0)
            recordScalar("#allNodesKnownBueren", simTime());
    }
    else if (meshID == pbMeshID)
    {
        if (batadv_compare_eth(mac, MACAddress("12:34:56:78:9A:BE")) == 0)
            recordScalar("#allNodesKnownPb", simTime());
    }
    else if (meshID == hoevelhofBaliSchlangenMeshID)
    {
        if (batadv_compare_eth(mac, MACAddress("12:34:56:78:9A:BF")) == 0)
            recordScalar("#allNodesKnownHoevelhofBaliSchlangenMeshID", simTime());
    }
    else if (meshID == remainderMeshID)
    {
        if (batadv_compare_eth(mac, MACAddress("12:34:56:78:9A:BC")) == 0)
            recordScalar("#allNodesKnownRemainder", simTime());
    }
}
void StatisticsCollector::checkAllNodesKnown(int currentCount, int meshID)
{
    if (overallNodeCount == currentCount)
        recordScalar("#allNodesKnown", simTime());

    if (meshID == buerenMeshID)
    {
        if (sizeBueren == currentCount)
            recordScalar("#allNodesKnownBueren", simTime());
    }
    else if (meshID == pbMeshID)
    {
        if (sizePaderborn == currentCount)
            recordScalar("#allNodesKnownPb", simTime());
    }
    else if (meshID == hoevelhofBaliSchlangenMeshID)
    {
        if (sizeBaliSchlangenHoevelhof == currentCount)
            recordScalar("#allNodesKnownHoevelhofBaliSchlangenMeshID", simTime());
    }
    else if (meshID == remainderMeshID)
    {
        if (sizeRemainder == currentCount)
            recordScalar("#allNodesKnownRemainder", simTime());
    }
}

void StatisticsCollector::add_sizeBueren()
{
    sizeBueren++;
}
void StatisticsCollector::add_sizePaderborn()
{
    sizePaderborn++;
}
void StatisticsCollector::add_sizeBaliSchlangenHoevelhof()
{
    sizeBaliSchlangenHoevelhof++;
}
void StatisticsCollector::add_sizeRemainder()
{
    sizeRemainder++;
}
int StatisticsCollector::get_sizeBueren()
{
    return sizeBueren;
}
int StatisticsCollector::get_sizePaderborn()
{
    return sizePaderborn;
}
int StatisticsCollector::get_sizeBaliSchlangenHoevelhof()
{
    return sizeBaliSchlangenHoevelhof;
}
int StatisticsCollector::get_sizeRemainder()
{
    return sizeRemainder;
}

void StatisticsCollector::recordOgmReceivedVPN(int ogmReceived, int meshID)
{
    if (meshID == buerenMeshID)
        ogmReceivedBueren.record(ogmReceived);
    else if (meshID == pbMeshID)
        ogmReceivedPaderborn.record(ogmReceived);
    else if (meshID == hoevelhofBaliSchlangenMeshID)
        ogmReceivedBaliSchlangenHoevelhof.record(ogmReceived);
    else
        /*if (meshID == remainderMeshID)*/
        ogmReceivedRemainder.record(ogmReceived);

}
void StatisticsCollector::recordOgmSentVPN(int ogmReceived, int meshID)
{
    if (meshID == buerenMeshID)
        ogmSentBueren.record(ogmReceived);
    else if (meshID == pbMeshID)
        ogmSentPaderborn.record(ogmReceived);
    else if (meshID == hoevelhofBaliSchlangenMeshID)
        ogmSentBaliSchlangenHoevelhof.record(ogmReceived);
    else
        /*if (meshID == remainderMeshID)*/
        ogmSentRemainder.record(ogmReceived);
}

void StatisticsCollector::recordOgmAggregation(int ogmAggregated, int meshID)
{
    if (meshID == buerenMeshID)
        ogmAggregatedBueren.record(ogmAggregated);
    else if (meshID == pbMeshID)
        ogmAggregatedPaderborn.record(ogmAggregated);
    else if (meshID == hoevelhofBaliSchlangenMeshID)
        ogmAggregatedBaliSchlangenHoevelhof.record(ogmAggregated);
    else
        /*if (meshID == remainderMeshID)*/
        ogmAggregatedRemainder.record(ogmAggregated);
}
void StatisticsCollector::recordOgmAggregationAvg(int ogmAggregated, int meshID)
{
    if (meshID == buerenMeshID)
        ogmAggregatedBuerenAvg.record(ogmAggregated);
    else if (meshID == pbMeshID)
        ogmAggregatedPaderbornAvg.record(ogmAggregated);
    else if (meshID == hoevelhofBaliSchlangenMeshID)
        ogmAggregatedBaliSchlangenHoevelhofAvg.record(ogmAggregated);
    else
        ogmAggregatedRemainderAvg.record(ogmAggregated); //Also used if there is no clustering
}
void StatisticsCollector::recordByteSize(int ogmByteSize, int meshID)
{
    if (meshID == buerenMeshID)
        ogmByteSizeBuerenAvg.record(ogmByteSize);
    else if (meshID == pbMeshID)
        ogmByteSizePaderbornAvg.record(ogmByteSize);
    else if (meshID == hoevelhofBaliSchlangenMeshID)
        ogmByteSizeBaliSchlangenHoevelhofAvg.record(ogmByteSize);
    else
        /*if (meshID == remainderMeshID)*/
        ogmByteSizeRemainderAvg.record(ogmByteSize); //Also used if there is no clustering
}
void StatisticsCollector::recordSendByteLength(int ogmByteSize, int meshID)
{
    if (meshID == buerenMeshID)
        ogmByteSizeBuerenSendAvg.record(ogmByteSize);
    else if (meshID == pbMeshID)
        ogmByteSizePaderbornSendAvg.record(ogmByteSize);
    else if (meshID == hoevelhofBaliSchlangenMeshID)
        ogmByteSizeBaliSchlangenHoevelhofSendAvg.record(ogmByteSize);
    else
        ogmByteSizeRemainderSendAvg.record(ogmByteSize); //Also used if there is no clustering
}

void StatisticsCollector::recordwifiNode(int wifi, int meshID)
{
    if (meshID == buerenMeshID)
        ogmAggregatedBuerenWifi.record(wifi);
    else if (meshID == pbMeshID)
        ogmAggregatedPaderbornWifi.record(wifi);
    else if (meshID == hoevelhofBaliSchlangenMeshID)
        ogmAggregatedBaliSchlangenHoevelhofWifi.record(wifi);
    else
        /*if (meshID == remainderMeshID)*/
        ogmAggregatedRemainderWifi.record(wifi); //Also used if there is no clustering
}

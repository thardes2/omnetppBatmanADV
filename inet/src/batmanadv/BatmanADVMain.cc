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

#include "BatmanADVMain.h"

Define_Module(BatmanADVMain);

int BatmanADVMain::selectedGWNone;
int BatmanADVMain::selectedGW1;
int BatmanADVMain::selectedGW2;
int BatmanADVMain::selectedGW3;
int BatmanADVMain::selectedGW4;
int BatmanADVMain::selectedGW5;
int BatmanADVMain::selectedGW6;
int BatmanADVMain::selectedGW7;
int BatmanADVMain::selectedGW8;
BatmanADVMain::BatmanADVMain()
{

}

BatmanADVMain::~BatmanADVMain()
{
}
void BatmanADVMain::finish()
{
    batadv_iv_ogm_orig_print(meshvpn);
    EV << "This is node " << nodenr << std::endl;

    StatisticsCollector* stats = getGlobalStatisticsController();
    int localClientCountAdd = stats->getLocalClientCountAdd();
    int localClientCountDel = stats->getLocalClientCountDel();
    int globalClientCountAdd = stats->getGlobalClientCountAdd();
    int globalClientCountDel = stats->getGlobalClientCountDel();
    int overallNodeCount = stats->getOverallNodeCount();
    EV << "localClientCountAdd: " << localClientCountAdd << " - " << "localClientCountDel: " << localClientCountDel << " - " << "globalClientCountAdd: " << globalClientCountAdd << " - " << "globalClientCountDel: " << globalClientCountDel << std::endl;
    EV << "overallNodeCount: " << overallNodeCount << std::endl;
    EV << "sizeBueren: " << stats->get_sizeBueren() << std::endl;
    EV << "sizePaderborn: " << stats->get_sizePaderborn() << std::endl;
    EV << "sizeBaliSchlangenHoevelhof: " << stats->get_sizeBaliSchlangenHoevelhof() << std::endl;
    EV << "sizeRemainder: " << stats->get_sizeRemainder() << std::endl;

    int tt_ogmReceived = stats->get_tt_ogmReceived();
    int tt_reqSend = stats->get_tt_reqSend();
    int tt_reqrcv = stats->get_tt_reqrcv();
    EV << "GLOBAL:" << std::endl;
    EV << "tt_ogmReceived : " << tt_ogmReceived << " - " << "tt_reqSend: " << tt_reqSend << " - " << "tt_reqrcv: " << tt_reqrcv << std::endl;

    recordScalar("#numOwnOGMSentCount", numOwnOGMSentCount);
    recordScalar("#numOGMReceivedCount", numOGMReceivedCount);
    recordScalar("#numOwnOGMSentCountVpn", numOwnOGMSentCountVpn);
    recordScalar("#numOwnOGMSentCountWlan", numOwnOGMSentCountWlan);

    recordScalar("#knownOrigs", origMap.size());
    recordScalar("#sizeLocalTT", tt_local.size());
    recordScalar("#sizeGlobalTT", tt_global.size());

    recordScalar("#numOGMReceivedCountVpn", numOGMReceivedCountVpn);
    recordScalar("#numOGMReceivedCountWlan", numOGMReceivedCountWlan);

    hopCountStats.recordAs("numOwnOGMSentCountVpn");
    EV << "LOCAL:" << std::endl;
    EV << "numOwnOGMSentCount: " << numOwnOGMSentCount << " - numOGMReceivedCount: " << numOGMReceivedCount << std::endl;
    EV << "numOwnOGMSentCountVpn: " << numOwnOGMSentCountVpn << " - numOwnOGMSentCountWlan: " << numOwnOGMSentCountWlan << std::endl;
    EV << "numOGMReceivedCountVpn: " << numOGMReceivedCountVpn << " -  numOGMReceivedCountWlan: " << numOGMReceivedCountWlan << std::endl;
    EV << "sizeLocalTT: " << tt_local.size() << " - sizeGlobalTT: " << tt_global.size() << std::endl;
    EV << "knownOrigs: " << origMap.size() << std::endl;
    EV << "aggSuccess: " << aggSuccess << std::endl;
    EV << "MeshID: " << meshId << std::endl;
    EV << "Known gateways: " << gwList.size() << std::endl;
    EV << "" << std::endl;
    EV << "" << std::endl;

    stats->recordOgmReceivedVPN(numOGMReceivedCountVpn, meshId);
    stats->recordOgmSentVPN(numOwnOGMSentCountVpn, meshId);

    if (gateSize("meshVpno") == 0)
        stats->recordwifiNode(1, meshId);
    else
        stats->recordwifiNode(0, meshId);
    stats->recordOgmAggregation(aggSuccess, meshId);
    debug_showOrigList();
}

void BatmanADVMain::initializeStatistics()
{
    selectedGW1 = 0;
    selectedGW2 = 0;
    selectedGW3 = 0;
    selectedGW4 = 0;
    selectedGW5 = 0;
    selectedGW6 = 0;
    selectedGW7 = 0;
    selectedGW8 = 0;
    selectedGWNone = 0;
    vectorSelectedGW1.setName("Select_gw1");
    vectorSelectedGW2.setName("Select_gw2");
    vectorSelectedGW3.setName("Select_gw3");
    vectorSelectedGW4.setName("Select_gw4");
    vectorSelectedGW5.setName("Select_gw5");
    vectorSelectedGW6.setName("Select_gw6");
    vectorSelectedGW7.setName("Select_gw7");
    vectorSelectedGW8.setName("Select_gw8");

    ogmAggregated.setName("ogmAggregated");

    numForwardOGMSent.setName("forward_ogm_send");
    numOGMReceived.setName("received_ogm");
    numOGMReceivedVpn.setName("received_ogm_vpn");
    numOGMReceivedWlan.setName("received_ogm_wlan");

    hopCountStats.setName("hopCountStats");

    numOwnOGMSent.setName("own_ogm_send");
    numOwnOGMSentVpn.setName("own_ogm_send_vpn");
    numOwnOGMSentWlan.setName("own_ogm_send_wlan");
    knownOrigs.setName("known_originator");
    knownGlobalClients.setName("known_global_clients");
    knownLocalClients.setName("known_local_clients");
    purgedOrigs.setName("purged_originator");

    knownOrigsHisto.setName("knownOrigsHisto");

    connectedToGw1.setName("connectedToGw1");
    connectedToGw2.setName("connectedToGw2");
    numForwardOGMSentCount = 0;
    numOGMReceivedCount = 0;
    numOGMReceivedCountWlan = 0;
    numOGMReceivedCountVpn = 0;
    numOwnOGMSentCount = 0;
    numOwnOGMSentCountWlan = 0;
    numOwnOGMSentCountVpn = 0;
    knownOrigsCount = 0;
    knownLocalClientsCount = 0;
    knownGlobalClientsCount = 0;
}

void BatmanADVMain::initialize(int stage)
{
    StatisticsCollector* stats = getGlobalStatisticsController();
    stats->addOverallNodeCount();
    initializeStatistics();
    //MACBase::initialize(stage);
    aggSuccess = 0;
    nodenr = "";
    bat_priv_tt = batadv_priv_tt();
    bat_priv_tt.local_changes = 0;
    bat_priv_tt.vn = 0;
    bat_priv_tt.last_changeset_len = 0;
    bat_priv_tt.ogm_append_cnt = 0;
    bat_priv_tt.local_changes = 0;
    windowReset = false;
    forw_bat_list = std::vector<batadv_forw_packet>();
    forw_bcast_list = std::vector<batadv_forw_packet*>();
    batman_queue_left = BATADV_BATMAN_QUEUE_LEN;
    bcast_queue_left = BATADV_BCAST_QUEUE_LEN;
    batadv_iv_window_create();
    std::string ssid = par("ssid").stringValue();
    isOperational = true;
    //ttvn = 0;
    meshId = (int) par("meshId");

    sizeBueren = (int) par("sizeBueren");
    sizePaderborn = (int) par("sizePaderborn");
    sizeBaliSchlangenHoevelhof = (int) par("sizeBaliSchlangenHoevelhof");
    sizeRemainder = (int) par("sizeRemainder");

    cluster = (bool) par("cluster");
    gwTermination = (bool) par("gwTermination");

    switch (meshId)
    {
        case buerenMeshID:
            stats->add_sizeBueren();
            break;
        case pbMeshID:
            stats->add_sizePaderborn();
            break;
        case hoevelhofBaliSchlangenMeshID:
            stats->add_sizeBaliSchlangenHoevelhof();
            break;
        case remainderMeshID:
            stats->add_sizeRemainder();
            break;
    }

    selectedGw = NULL;
    originator_interval = par("originatorInterval");
    ogm_ttl = par("ogm_ttl");
    hopPenalty = par("hopPenalty");
    if (hopPenalty < 0 || hopPenalty >= 255)
        throw new cException(this, "hopPenalty must be a value between 0 and 99");
    //hopPenalty /= 100;
    gwClass_upload_speed = par("GWClass_upload_speed");
    gwClass_download_speed = par("GWClass_download_speed");
    if (gwClass_upload_speed != 0 && gwClass_download_speed != 0)
    {
        gw_mode = BATADV_GW_MODE_SERVER;
        EV << "Node: " << nodenr << ": Switched to gateway mode!" << std::endl;
    }
    else
        gw_mode = BATADV_GW_MODE_CLIENT; //BATADV_GW_MODE_CLIENT;

    if (originator_interval < 0.001)
        throw cRuntimeError("Invalid 'originatorInterval' parameter");
    cPar myMac = par("mac");
    numberClients = par("numClients");
    cPar gwClass = par("gatewayMode");
    switch ((int) gwClass)
    {
        case 1:
            gw_sel_class = fastSwitch;
            break;
        case 2:
            gw_sel_class = stableConnection;
            break;
        case 3:
            gw_sel_class = fastSwitch;
        case 20:
            gw_sel_class = defaultMode;
            break;
        default:
            gw_sel_class = defaultMode;
            break;
    }
    it = 0;
    nodenr = par("nodenr").str();
    iNodenr = atoi(nodenr.substr(1, 2).c_str());
    /*    if (iNodnr == 50)
     {
     printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!nobody wants me :-(\n");
     return;
     }*/
    MACAddress mac = MACAddress();
    mac.setAddress(myMac);
    ManetAddress addr = ManetAddress(mac);
    ownAddress = addr;
    batadv_mesh_init();
    batadv_non_mesh_init();

    EV << "Node: " << nodenr << ": Node " + mac.str() << "successfully initialized" << std::endl;
    ogm_reminder *c = new ogm_reminder();
    ogm_reminder *cMesh = new ogm_reminder();
    cMesh->setHard_iface(meshvpnNP);
    c->setHard_iface(wlan0NP);

    simtime_t nextOGM = simTime() + uniform(0, 10);
    EV << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& Node: " << nodenr << ": Scheduling a new OGM for time " << nextOGM.str() << std::endl;

    cConfiguration *config = ev.getConfig();
    const char* warmuptimeConfig = config->getConfigValue("warmup-period");
    std::stringstream strValue;
    strValue << warmuptimeConfig;

    unsigned int intValue;
    strValue >> intValue;
    warmupTime = intValue;
    EV << "Simtime: " << simTime() << " - Warmup: " << warmupTime << std::endl;
    bat_priv_gw.bandwidth_down = gwClass_download_speed;
    bat_priv_gw.bandwidth_up = gwClass_upload_speed;
    bat_priv_gw.reselect = false;
    bat_priv_gw.curr_gw = NULL;

    //delete c;

    if (iNodenr == -1)
    {
        primary_ifNP = wlan0NP;
        BATADV_IF_DEFAULTNP = wlan0NP;
        scheduleAt(warmupTime, cMesh); //Reminder to send the OGM after "orignator_interval"
        scheduleAt(warmupTime, c); //Reminder to send the OGM after "orignator_interval"
        //scheduleAt(warmupTime, new tt_work_period());
        return;
    }
    scheduleAt(nextOGM, cMesh); //Reminder to send the OGM after "orignator_interval" for interface meshvpn
    if (gw_mode == BATADV_GW_MODE_SERVER)
    {
        delete c;
    }
    else
    {
        scheduleAt(nextOGM, c); //Reminder to send the OGM after "orignator_interval" for interface wlan0
    }
    scheduleAt(BATADV_TT_WORK_PERIOD, new tt_work_period());

}
void BatmanADVMain::gen_random(char *s, const int len)
{
    static const char alphanum[] = "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i)
    {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

double BatmanADVMain::fRand(double fMin, double fMax)
{
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

unsigned long BatmanADVMain::getBitForNumber(unsigned int nr)
{
    if (nr == 0)
        return 1;
    if (nr == 1)
        return 10;
    if (nr == 2)
        return 100;
    if (nr == 3)
        return 1000;
    if (nr == 4)
        return 10000;
    if (nr == 5)
        return 100000;
    if (nr == 6)
        return 1000000;
    if (nr == 7)
        return 10000000;
    if (nr == 8)
        return 100000000;
    if (nr == 9)
        return 1000000000;
    if (nr == 10)
        return 10000000000;
    if (nr == 11)
        return 100000000000;
}

void BatmanADVMain::batadv_non_mesh_init()
{
    wlan1 = new batadv_hard_iface();
    wlan1->if_num = 2;
    wlan1->if_status = BATADV_IF_ACTIVE;
    wlan1->name = wlanName;
    wlan1->num_bcasts = 0;
    wlan1->refcount = 0;
}

void BatmanADVMain::batadv_mesh_init()
{
    meshvpnNP = batadv_hard_iface();
    wlan0NP = batadv_hard_iface();
    meshvpnNP.if_num = 0;
    wlan0NP.if_num = 1;
    meshvpnNP.if_status = BATADV_IF_ACTIVE;
    wlan0NP.if_status = BATADV_IF_ACTIVE;
    meshvpnNP.num_bcasts = 0;
    wlan0NP.num_bcasts = 0;
    meshvpnNP.refcount = 0;
    wlan0NP.refcount = 0;

    meshvpnNP.bat_iv.ogm_seqno = getRandomSeqno();
    wlan0NP.bat_iv.ogm_seqno = getRandomSeqno();

    meshvpnNP.name = meshName; //0
    wlan0NP.name = wlanName; //1
    BATADV_IF_DEFAULTNP = meshvpnNP;
    secondary_ifNP = wlan0NP;
    meshvpnNP.bat_iv.ogm_buff_len = BATADV_OGM_HLEN;
    wlan0NP.bat_iv.ogm_buff_len = BATADV_OGM_HLEN;

    meshvpn = new batadv_hard_iface();
    wlan0 = new batadv_hard_iface();
    meshvpn->if_num = 0;
    wlan0->if_num = 1;
    meshvpn->if_status = BATADV_IF_ACTIVE;
    wlan0->if_status = BATADV_IF_ACTIVE;
    meshvpn->num_bcasts = 0;
    wlan0->num_bcasts = 0;
    meshvpn->refcount = 0;
    wlan0->refcount = 0;

    meshvpn->bat_iv.ogm_seqno = getRandomSeqno();
    wlan0->bat_iv.ogm_seqno = getRandomSeqno();

    meshvpn->name = meshName; //0
    wlan0->name = wlanName; //1
    BATADV_IF_DEFAULT = meshvpn;
    secondary_if = meshvpn;
    /* if (gwClass_download_speed != 0 && gwClass_upload_speed != 0)
     primary_if = wlan0;
     else
     primary_if = meshvpn;*/
    bat_priv_tvlv.ogm_tt = NULL;

    if (gw_mode == BATADV_GW_MODE_SERVER)
    {
        primary_ifNP = meshvpnNP;
        BATADV_IF_DEFAULTNP = meshvpnNP;
    }
    else
    {
        int a = rand() % 2;
        if (a != 1)
        {
            primary_ifNP = meshvpnNP;
            BATADV_IF_DEFAULTNP = meshvpnNP;
        }
        else
        {
            primary_ifNP = wlan0NP;
            BATADV_IF_DEFAULTNP = wlan0NP;
        }
    }

    meshvpn->bat_iv.ogm_buff_len = BATADV_OGM_HLEN;
    wlan0->bat_iv.ogm_buff_len = BATADV_OGM_HLEN;
}

/**
 * prepare a new outgoing OGM for the send queue
 */
void BatmanADVMain::batadv_iv_ogm_schedule(batadv_hard_iface hard_iface)
{
    batadv_ogm_packet *batadv_ogm_packet = createDefaultOGM(hard_iface);
    int ogm_buff_len = hard_iface.bat_iv.ogm_buff_len;
    double send_time = 0;

    if (hard_iface.name == primary_ifNP.name)
    {
        batadv_tt_local_commit_changes();
        if (bat_priv_tvlv.ogm_tt != NULL)
        {
            ogm_buff_len += bat_priv_tvlv.ogm_tt->getTtChanges().size() * 15;
        }
        batadv_tvlv_container_ogm_append(batadv_ogm_packet);
    }
    send_time = batadv_iv_ogm_emit_send_time();
    batadv_iv_ogm_queue_addNoOutgoing(batadv_ogm_packet, ogm_buff_len, hard_iface, true, FORWARD_MY_OGM, send_time);
    return;
}

void BatmanADVMain::batadv_tvlv_container_ogm_append(batadv_ogm_packet *ogm)
{
    batadv_tvlv_tt_data tt_data = batadv_tvlv_tt_data();
    batadv_tvlv_gateway_data gatewayData = batadv_tvlv_gateway_data();

//batadv_tvlv_gateway_data
    gatewayData.setBandwidth_down(gwClass_download_speed);
    gatewayData.setBandwidth_up(gwClass_upload_speed);

//batadv_tvlv_tt_data tt_data;
    if (bat_priv_tvlv.ogm_tt != NULL)
    {
        tt_data = *bat_priv_tvlv.ogm_tt;
    }
    else
        tt_data.setTtChanges(std::vector<batadv_tvlv_tt_change>());
    tt_data.setTtvn(bat_priv_tt.vn);
    tt_data.setNum_vlan(0);    //We don't use vlan stuff in our network
    tt_data.setFlags(BATADV_TT_OGM_DIFF);
    tt_data.setFullTable(false);
    tt_data.setTtType(0);
    bat_priv_tvlv.ogm_tt = NULL;    //Reset the old tt_data that should be appended to the ogm -> It is not longer needed

    ogm->setTt_data(tt_data);
    ogm->setGateway_data(gatewayData);
    //ogm->setTvlv_len(ogm->getTvlv_len()+BATADV_TVLV_HLEN);//Experimental
}

void BatmanADVMain::sendOGM(batadv_ogm_packet *ogm, MACAddress dest, simtime_t delay = 0)
{

}

void BatmanADVMain::handleMessage(cMessage *msg)
{
    if(gwClass_download_speed==0)
            int x = 0;
    EV << "Node " << nodenr << " with MAC " << ownAddress.getMAC().str() << std::endl;
    EV << "Node " << nodenr << " got message " << msg->getName() << std::endl;
    StatisticsCollector* stats = getGlobalStatisticsController();
    if (iNodenr == -1 && simTime() < warmupTime)
    {
        delete msg;
        return;
    }
    if ((simTime() >= warmupTime) && !windowReset)
    {
        ogmWindow.clear();
        windowReset = true;
    }
    if (dynamic_cast<ogm_reminder *>(msg) != NULL)
    {

        ogm_reminder* c = dynamic_cast<ogm_reminder*>(msg);
        if (c->getHard_iface().name == meshvpnNP.name)
            EV << "Node: " << nodenr << ": Send my own scheduled OGM - " << ownAddress.getMAC().str() << " - Interface: MeshVPN" << std::endl;
        else
            EV << "Node: " << nodenr << ": Send my own scheduled OGM - " << ownAddress.getMAC().str() << " - Interface: WLAN0" << std::endl;
        //printf("Node: %s - Send my own scheduled OGM on interface %d\n", nodenr.c_str(), c->getHard_iface().name);
        batadv_schedule_bat_ogm(c->getHard_iface());
        return;
    }
    else if (dynamic_cast<delayedWork*>(msg) != NULL)
    {
        EV << "Node: " << nodenr << ": Starting delayed work to send outstanding OGMs" << std::endl;
        delayedWork* c = dynamic_cast<delayedWork*>(msg);

        batadv_send_outstanding_bat_ogm_packetNoOutgoing(c->getId(), c->getReason());
        delete (msg);
        return;
    }
    else if (dynamic_cast<tt_work_period *>(msg) != NULL)
    {
        EV << "Node: " << nodenr << ": Starting delayed work to purge TT entries. " << std::endl;
        batadv_tt_purge();
        batadv_collect_periodicstats();
        scheduleAt(simTime() + BATADV_TT_WORK_PERIOD, msg);
        return;
    }
    if (dynamic_cast<AirFrame *>(msg) != NULL || dynamic_cast<EtherFrame *>(msg) != NULL)
    {
        cGate *arrivalGate = msg->getArrivalGate();
        bool wifi = false;
        if (dynamic_cast<AirFrame *>(msg) != NULL)
        {
            AirFrame* aframe = (AirFrame*) msg;
            msg = (EtherFrame*) aframe->decapsulate();
            delete aframe;
            wifi = true;
            EV << "Node: " << nodenr << ": Received airframe on wlan0" << std::endl;
        }
        else
            EV << "Node: " << nodenr << ": Received etherframe on meshvpn" << std::endl;
        /*else if (dynamic_cast<EtherFrame *>(msg) != NULL)
         {*/
        frame = (EtherFrame*) (msg);
        //}

        EtherFrame* frame_local = getCurrentFrame();
        cPacket* higherlayermsg = frame_local->decapsulate();
        if (dynamic_cast<batadv_ogm_packet_agg *>(higherlayermsg) != NULL)
        {
            batadv_ogm_packet_agg* ogm_aggr = check_and_cast<batadv_ogm_packet_agg*>(higherlayermsg);
            std::vector<cPacket *> ogms = ogm_aggr->getOgms();
            std::vector<batadv_ogm_packetClass> ogmsNoPointer = ogm_aggr->getOgmsNoPointer();
            if (simTime() >= warmupTime)
            {
                if (wifi)
                {
                    numOGMReceivedCountWlan = numOGMReceivedCountWlan + 1;
                }
                else if (dynamic_cast<EtherFrame *>(msg) != NULL)
                {
                    numOGMReceivedCountVpn = numOGMReceivedCountVpn + 1;
                }
            }
            batadv_ogm_packet *ogm = NULL;
            batadv_ogm_packet *ogm2 = NULL;
            EtherFrame *tmpEthFrame = NULL;
            bool statsRecorded = false;

            for (unsigned int packet_num = 0; packet_num < ogmsNoPointer.size(); packet_num++)
            {
                ogm = convertToOGM(ogmsNoPointer[packet_num]);

                if(gwClass_download_speed!=0 || gwClass_upload_speed!=0)
                    int x = 0;


                if ((ogm->getMeshID() != meshId && meshId != -1) || gwTermination)
                {
                    if (simTime() >= warmupTime)
                    {
                        if (wifi)
                            numOGMReceivedCountWlan--;
                        else
                            numOGMReceivedCountVpn--;
                        break;
                    }
                    break;
                }
                else
                {  //This OGM is for our subnet - record the size of the buffer once!
                    if (!statsRecorded)
                    {
                        if (!wifi)
                        {
                            //Just once
                            if (cluster)
                            {
                                stats->recordOgmAggregationAvg(ogmsNoPointer.size(), meshId);
                            }
                            else
                            {
                                ogmAggregated.record(ogmsNoPointer.size());
                            }
                            if (simTime() >= warmupTime)
                                stats->recordByteSize(ogm_aggr->getByteLength(), meshId);

                            statsRecorded = true;
                        }
                    }
                }
                ogm2 = ogm->dup();
                tmpEthFrame = new EtherFrame();
                tmpEthFrame->setSrc(frame_local->getSrc());
                tmpEthFrame->setDest(frame_local->getDest());
                tmpEthFrame->encapsulate(ogm2);
                this->frame = tmpEthFrame;

                if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
                    printf("Aggregated packet - node %s at %f\n", nodenr.c_str(), simTime().dbl());
                handleOGM(ogm, wifi);
                if (this->frame != NULL)
                {
                    if (!this->frame->isScheduled())
                    {
                        if (this->frame->hasEncapsulatedPacket())
                            delete this->frame->decapsulate();
                        delete this->frame;

                        this->frame = NULL;
                    }
                }

            }
            if (simTime() >= warmupTime)
            {
                aggSuccess++;
            }
            if (this->frame != NULL)
            {
                if (this->frame->hasEncapsulatedPacket())
                    delete this->frame->decapsulate();
                if (!this->frame->isScheduled())
                    delete this->frame;
                this->frame = NULL;
            }

            if (frame_local->hasEncapsulatedPacket())
                delete frame_local->decapsulate();
            if (msg != NULL && meshId == -1)
            {
               // delete msg;
            }

            delete frame_local;
            delete ogm_aggr;

            return;
        }
        else if (dynamic_cast<batadv_forw_packet *>(higherlayermsg) != NULL)
        {
            delete higherlayermsg;
            delete frame_local;
            delete msg;
            return;
        }
        else if (dynamic_cast<batadv_ogm_packet *>(higherlayermsg) != NULL)
        {
            if(gwClass_download_speed==0)
                    int x = 0;
            batadv_ogm_packet* ogm = check_and_cast<batadv_ogm_packet*>(higherlayermsg);
            if (ogm)
            {
                if ((iNodenr == -2 || iNodenr == -3) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
                    printf("Single OGM packet - node %s at %f\n", nodenr.c_str(), simTime().dbl());
                if (ogm->getMeshID() != meshId && !gwTermination)    //This is not for us :-( and we don't use GW termination
                {
                    delete ogm;
                    ogm = NULL;
                    delete frame_local;
                    if (this->frame->hasEncapsulatedPacket())
                        delete this->frame->decapsulate();
                    if (!(this->frame->isScheduled()))
                        delete this->frame;
                    return;
                }
                if (simTime() >= warmupTime)
                {
                    if(gwClass_download_speed==0)
                            int x = 0;
                    ///if (dynamic_cast<AirFrame *>(msg) != NULL)
                    if (wifi)
                    {
                        numOGMReceivedCountWlan = numOGMReceivedCountWlan + 1;
                    }
                    else if (dynamic_cast<EtherFrame *>(msg) != NULL)
                    {
                        numOGMReceivedCountVpn = numOGMReceivedCountVpn + 1;
                        if (cluster)
                            stats->recordOgmAggregationAvg(1, meshId);
                        else
                            ogmAggregated.record(1);
                        stats->recordByteSize(ogm->getByteLength(), meshId);
                    }

                }

                handleOGM(ogm, wifi);
            }
            delete frame_local;
            if (this->frame->hasEncapsulatedPacket())
                delete this->frame->decapsulate();
            if (!(this->frame->isScheduled()))
                delete this->frame;
            return;
        }
        else if (dynamic_cast<batadv_unicast_4addr_packet *>(higherlayermsg) != NULL)
        {
            batadv_unicast_4addr_packet* unicast_packet = check_and_cast<batadv_unicast_4addr_packet*>(msg);
            if (unicast_packet)
                batadv_recv_unicast_packet(meshvpn, msg);
            else
            {
                delete msg;
            }
            delete frame_local;
            return;
        }
        else if (dynamic_cast<batadv_unicast_packet *>(higherlayermsg) != NULL)
        {
            batadv_recv_unicast_packet(getHardIfForCGate(msg->getArrivalGate()), msg);
            delete frame_local;
            return;
        }
        else if (dynamic_cast<batadv_unicast_tvlv_packet *>(higherlayermsg) != NULL)
        {
            batadv_recv_unicast_tvlv(getCurrentFrame(), getHardIfForCGate(msg->getArrivalGate()));
        }
        delete frame_local;
        delete higherlayermsg;
        delete this->frame->decapsulate();
        if (!(this->frame->isScheduled()))
            delete this->frame;
    }
    else if (dynamic_cast<Ieee80211DataFrameWithSNAP *>(msg) != NULL) //Client payload data
    {
        bool do_bcast = false, client_added;
        bool ret = false;
        batadv_dhcp_recipient dhcp_rcp;
        /* Register the client MAC in the transtable */
        Ieee80211DataFrameWithSNAP* frame = dynamic_cast<Ieee80211DataFrameWithSNAP *>(msg);
        MACAddress clientMac = frame->getTransmitterAddress();
        batadv_hard_iface* device;

        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %s\n", nodenr.c_str());
        return;
        device = wlan1;
        /* Register the client MAC in the transtable */
        if (!clientMac.isMulticast())
        {
            client_added = batadv_tt_local_add(clientMac, device);
            double a = simTime().dbl();
            dhcp_rcp = batadv_gw_dhcp_recipient_get(msg);
            if (!client_added)
                return;
        }
        if (clientMac.isMulticast())
        {
            dhcp_rcp = batadv_gw_dhcp_recipient_get(msg);

            if (gw_mode == BATADV_GW_MODE_OFF)
            {
                do_bcast = true;
            }
            if ((gw_mode == BATADV_GW_MODE_SERVER) && (dhcp_rcp == BATADV_DHCP_TO_SERVER))
            {
                /* gateways should not forward any DHCP message if
                 * directed to a DHCP server
                 */
                delete frame;
                return;
            }
            /* else if (gw_mode == BATADV_GW_MODE_OFF) //Ups -> Forwards this to our selected GW
             {
             //forwardDHCPRequest(dhcpMessage);
             }
             if (do_bcast && clientMac.isBroadcast())
             {

             }*/
        }

        /* ethernet packet should be broadcasted */
        if (do_bcast)
        {

        }
        else //Unicast
        {

            /* DHCP packets going to a server will use the GW feature */

            //TMP
            /*if (dhcp_rcp == BATADV_DHCP_TO_SERVER)
             {
             ret = batadv_gw_out_of_range(frame);
             if (ret)
             {
             delete frame;
             return;
             }
             ret = batadv_send_skb_via_gw(frame);
             }
             //Here some strange statement in the code as the condition here never becomes true -> Skipped this
             else {
             batadv_send_skb_via_tt(frame,clientMac);
             }*/
        }
        delete msg;
        return;
    }
}

void BatmanADVMain::handleOGM(batadv_ogm_packet* ogm, bool wifi)
{
    EV << "Node: " << nodenr << ": Received ogm from " << ogm->getPrev_sender().str() << " and originator " << ogm->getOrig().str() << " ttl: " << ogm->getTtl() << " tq: " << ogm->getTq() << std::endl;
    if ((iNodenr == -3 || iNodenr == -2) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
    {
        printf("\n");
        printf("\n");
    }
    ManetAddress ogmOrig = ogm->getOrig();
    if ((iNodenr == -3 || iNodenr == -2) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
        printf("Node %d got a message from %s at %f via %s\n", iNodenr, ogm->getOrig().getMAC().str().c_str(), simTime().dbl(), frame->getSrc().str().c_str());
    if (wifi)
        batadv_iv_ogm_process(wlan0NP);
    else
        batadv_iv_ogm_process(meshvpnNP);
    if ((iNodenr == -3 || iNodenr == -2) && batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
    {
        printf("\n");
        printf("\n");
    }
    batadv_orig_node *orig_node = batadv_iv_ogm_orig_get(ogm);
    if (ogm != NULL && ogm->getOrig() != ownAddress && orig_node != NULL)
    {

        batadv_tvlv_tt_data tt_data = ogm->getTt_data();
        batadv_tvlv_gateway_data gatewayData = ogm->getGateway_data();

        EV << "Node: " << nodenr << ": OGM processing done - Going to tvlv data" << std::endl;

        if ((orig_node->last_ttvn != tt_data.getTtvn()) && tt_data.getTtvn() != -1)
        {
            double a = simTime().dbl();
            StatisticsCollector* stats = getGlobalStatisticsController();
            stats->add_tt_ogmReceived();
            EV << "Node: " << nodenr << ": TT data is attached!" << std::endl;
            batadv_tt_update_orig(orig_node, NULL, 0, tt_data.getTtChanges(), tt_data.getTtChanges().size(), tt_data.getTtvn());
        }
        else
            EV << "Node: " << nodenr << ": No new TT data is attached!" << std::endl;
        if (gatewayData.getBandwidth_down() != 0 && gatewayData.getBandwidth_up() != 0)
        {
            EV << "Node: " << nodenr << ": Gateway data is attached!" << std::endl;
            batadv_gw_tvlv_ogm_handler_v1(orig_node, BATADV_TVLV_HANDLER_OGM_CIFNOTFND, &gatewayData);
        }
        else
            EV << "Node: " << nodenr << ": No gateway data is attached!" << std::endl;
    }
    delete ogm;
    batadv_purge_orig();

}

batadv_hard_iface* BatmanADVMain::getHardIfForCGate(cGate* gate)
{
    std::string str(gate->getFullName());
    std::string str2("wlan");
    std::size_t found = str.find(str2);
    if (found != std::string::npos)
        return wlan0;
    else
        return meshvpn;
}

batadv_hard_iface BatmanADVMain::getHardIfForCGateNP(cGate* gate)
{
    std::string str(gate->getFullName());
    std::string str2("wlan");
    std::size_t found = str.find(str2);
    if (found != std::string::npos)
        return wlan0NP;
    else
        return meshvpnNP;
}

void BatmanADVMain::forwardDHCPRequest(DHCPMessage *dhcpMessage)
{

    EtherFrame* forwardFrame = new EtherFrame();
    forwardFrame->setSrc(ownAddress.getMAC());
    if (selectedGw == NULL)
    {
        delete dhcpMessage;
        return;
    }
    forwardFrame->setDest(selectedGw->orig_node->orig.getMAC());
    forwardFrame->encapsulate(dhcpMessage);
    for (int i = 0; i < numberClients; i++)
    {
        cGate *meshVPNGates = gate("meshVpn$o", i);    //Gate, index
        send(frame->dup(), meshVPNGates);
    }
}

EtherFrame* BatmanADVMain::getCurrentFrame()
{
    return frame->dup();
}

void BatmanADVMain::deleteCurrentFrame()
{
    delete frame;
}
int BatmanADVMain::getRandomSeqno()
{
    /*double beta = 10000;
     rnd.collect(beta);
     return (int) exponential(beta, 0);*/
    return (int) uniform(0, BATADV_EXPECTED_SEQNO_RANGE);
}
batadv_ogm_packet* BatmanADVMain::createDefaultOGM(batadv_hard_iface hard_iface)
{
    batadv_ogm_packet *packet = new batadv_ogm_packet("batadv_ogm_packet");

    packet->setPacket_type(BATADV_IV_OGM);
    packet->setVersion(BATADV_COMPAT_VERSION);
    packet->setTtl(BATADV_TTL);
    packet->setFlags(BATADV_NO_FLAGS);
    if (hard_iface.name == primary_ifNP.name)
        packet->setFlags(BATADV_PRIMARIES_FIRST_HOP);

    packet->setSeqno(getRandomSeqno());
    packet->setOrig(ownAddress);
    packet->setPrev_sender(ownAddress);
    packet->setReserved(0);
    packet->setTq(TQ_MAX);
    packet->setTvlv_len(0);
    packet->setMeshID(meshId);
    EV << "Node: " << nodenr << ": Created OGM with seqno " << packet->getSeqno() << std::endl;
    return packet;
}

/**
 * batadv_iv_ogm_orig_get - retrieve or create (if does not exist) an originator
 * @bat_priv: the bat priv with all the soft interface information
 * @ogm: OGM package
 *
 * Returns the originator object corresponding to the passed mac address or NULL
 * on failure.
 * If the object does not exists it is created an initialized.
 */
batadv_orig_node* BatmanADVMain::batadv_iv_ogm_orig_get(batadv_ogm_packet *ogm)
{
    batadv_orig_node *orig_node = NULL;
    if (origMap.count(ogm->getOrig()) > 0)
        return origMap.at(ogm->getOrig());

    //orig_node = checkOrigListForExistence(ogm->getOrig());
    EV << "Node: " << nodenr << ": Size of originator map is " << origMap.size() << std::endl;

    orig_node = batadv_orig_node_new(ogm->getOrig().getMAC());

    //Create it, if it is not there
    orig_node->bat_iv.bcast_own = 0;
    orig_node->bat_iv.bcast_own_sum = 0;
    orig_node->bcast_seqno_reset = simTime();
    orig_node->capa_initialized = 0;
    orig_node->ifinfo = NULL;
    orig_node->last_bcast_seqno = 0;
    orig_node->last_seen = simTime();
    orig_node->last_ttvn = 0;
    orig_node->refcount = 0;
    orig_node->tt_buff_len = 0;
    orig_node->ifinfo_list = std::vector<batadv_orig_ifinfo *>();
    orig_node->neigh_list = std::vector<batadv_neigh_node *>();
    EV << "Node: " << nodenr << ": Originator with mac " << ogm->getOrig().getMAC().str() << " is unknown - I am creating a new one." << std::endl;
    origMap.insert(std::pair<ManetAddress, batadv_orig_node*>(ogm->getOrig(), orig_node)); //Add the node to the list
    if (simTime() > warmupTime)
    {
        StatisticsCollector* stats = getGlobalStatisticsController();
        //stats->checkAllNodesKnown(origMap.size(), meshId);
        /*if (batadv_compare_eth(ogm->getOrig().getMAC(), MACAddress("12:34:56:78:9A:BA")) == 0)
         printf("Node %s - Created the new orig entry at %f - from %s \n", nodenr.c_str(), simTime().dbl(), ogm->getPrev_sender().getMAC().str().c_str());*/
        stats->checkAllNodesKnownMac(ogm->getOrig().getMAC(), meshId, nodenr);
    }
    return orig_node;
}

/**
 * batadv_iv_ogm_orig_get - retrieve or create (if does not exist) an originator
 * @bat_priv: the bat priv with all the soft interface information
 * @ogm: OGM package
 *
 * Returns the originator object corresponding to the passed mac address or NULL
 * on failure.
 * If the object does not exists it is created an initialized.
 */
batadv_orig_node* BatmanADVMain::batadv_iv_ogm_orig_get_by_mac(MACAddress mac)
{
    batadv_orig_node* orig_node = NULL;

    if (origMap.count(ManetAddress(mac)) > 0)
        return origMap.at(ManetAddress(mac));
    orig_node = batadv_orig_node_new(mac);

    //Create it, if it is not there
    orig_node->bat_iv.bcast_own = 0;
    orig_node->bat_iv.bcast_own_sum = 0;
    orig_node->bcast_seqno_reset = simTime();
    orig_node->capa_initialized = 0;
    orig_node->ifinfo = NULL;
    orig_node->last_bcast_seqno = 0;
    orig_node->last_seen = simTime();
    orig_node->last_ttvn = 0;
    orig_node->refcount = 0;
    orig_node->tt_buff_len = 0;
    orig_node->ifinfo_list = std::vector<batadv_orig_ifinfo *>();
    orig_node->neigh_list = std::vector<batadv_neigh_node *>();
    EV << "Node: " << nodenr << ": Originator with mac " << mac.str() << " is unknown - I am creating a new one." << std::endl;
    origMap.insert(std::pair<ManetAddress, batadv_orig_node*>(ManetAddress(mac), orig_node)); //Add the node to the list

    if (simTime() > warmupTime)
    {
        StatisticsCollector* stats = getGlobalStatisticsController();
        //stats->checkAllNodesKnown(origMap.size(), meshId);
        /*if (batadv_compare_eth(mac, MACAddress("12:34:56:78:9A:BA")) == 0)
         printf("Node %s - Created the new orig entry at %f \n", nodenr.c_str(), simTime().dbl());*/
        stats->checkAllNodesKnownMac(mac, meshId, nodenr);
    }
    return orig_node;
}

/**
 * Checks whether a ManetAdress matches an entry in the orignatorlist
 */
batadv_orig_node* BatmanADVMain::checkOrigListForExistence(ManetAddress node)
{
    if (origMap.count(node) > 0)
        return origMap.at(node);
    return NULL;
}

void BatmanADVMain::ttLocalAdd(ManetAddress addr, batadv_tt_local_entry *tt_local_entry)
{
    tt_local.insert(std::pair<ManetAddress, batadv_tt_local_entry*>(addr, tt_local_entry)); //Add the entry to the list
    /**** Collect statistics ****/
    StatisticsCollector* stats = getGlobalStatisticsController();
    stats->addLocalClientCountAdd();
    /**** Collect statistics ****/
}

/**
 * Checks whether a ManetAdress matches an entry in an translation table object
 */
batadv_tt_local_entry* BatmanADVMain::checkLocalTranslationTableForExistence(ManetAddress node)
{
    TranslationTableLocal::iterator it = tt_local.find(node); //Check whether this node is already part of the known hosts
    batadv_tt_local_entry* tt_entry = NULL;
    if (it != tt_local.end())
    {
        tt_entry = it->second;
    }
    return tt_entry;
}
batadv_tt_global_entry* BatmanADVMain::checkGlobalTranslationTableForExistence(ManetAddress node)
{
    TranslationTableGlobal::iterator it = tt_global.find(node); //Check whether this node is already part of the known hosts
    batadv_tt_global_entry *tt_entry = NULL;
    if (it != tt_global.end())
    {
        tt_entry = it->second;
    }
    return tt_entry;
}

/**
 * returns 0 if both addresses are the same
 */
int BatmanADVMain::batadv_compare_eth(MACAddress mac1, MACAddress mac2)
{
    int compareResult = mac1.compareTo(mac2);
    return compareResult;
}

void BatmanADVMain::batadv_tvlv_unicast_send(batadv_hard_iface *primary_if, ManetAddress dst, int version, batadv_tvlv_tt_data *tvlv_value, forwardReason reason)
{ //---> batadv_send_skb_unicast

    batadv_unicast_tvlv_packet *unicast_tvlv_packet = new batadv_unicast_tvlv_packet();
    unicast_tvlv_packet->setPacket_type(BATADV_UNICAST_TVLV);
    unicast_tvlv_packet->setVersion(BATADV_COMPAT_VERSION);
    unicast_tvlv_packet->setTtl(BATADV_TTL);
    unicast_tvlv_packet->setReserved(0);
    unicast_tvlv_packet->setTvlv_len(0);
    unicast_tvlv_packet->setAlign(0);
    unicast_tvlv_packet->setDst(dst.getMAC());
    unicast_tvlv_packet->setSrc(ownAddress.getMAC());
    unicast_tvlv_packet->setAlign(0);
    unicast_tvlv_packet->encapsulate(tvlv_value);

    batadv_orig_node *orig_node = checkOrigListForExistence(dst);
    if (!orig_node)
        return;

    batadv_send_skb_to_orig(unicast_tvlv_packet, orig_node, NULL, reason);

}

StatisticsCollector* BatmanADVMain::getGlobalStatisticsController()
{
    cModule *modp = getModuleByPath("statisticsCollector");
    StatisticsCollector *stats = check_and_cast<StatisticsCollector *>(modp);
    return stats;
}

/**
 * batadv_is_my_mac - check if the given mac address belongs to any of the real
 * interfaces in the current mesh
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the address to check
 */
void BatmanADVMain::handleSelfMsg(cMessage* msg)
{
    msg->getArrivalGate();
}
/**
 * returns 0 iff addr is the same like the own address
 */
int BatmanADVMain::batadv_is_my_mac(/*struct batadv_priv *bat_priv, */MACAddress *addr)
{
    return batadv_compare_eth(MACAddress(addr->getInt()), ownAddress.getMAC());
}

void BatmanADVMain::handleLowerMsg(cPacket* msg)
{

    Ieee80211DataFrame *frame = new Ieee80211DataFrame();
    frame->setReceiverAddress(MACAddress("11:22:33:44:55:66"));
    sendOrEnqueue(frame);
}
/** Implements abstract to use ETX packets */
void BatmanADVMain::handleEtxMessage(cPacket* m)
{
    Ieee80211DataFrame *frame = new Ieee80211DataFrame();
    frame->setReceiverAddress(MACAddress("11:22:33:44:55:66"));
    sendOrEnqueue(frame);
}

/** Implements abstract to use routing protocols in the mac layer */
void BatmanADVMain::handleRoutingMessage(cPacket* m)
{
    Ieee80211DataFrame *frame = new Ieee80211DataFrame();
    frame->setReceiverAddress(MACAddress("11:22:33:44:55:66"));
    sendOrEnqueue(frame);
}

/** Implements abstract to use inter gateway communication */
void BatmanADVMain::handleWateGayDataReceive(cPacket *m)
{
    EV << "BATMAN!" << std::endl;
}

/** Implements the redirection of a data packets from a gateway to other */
void BatmanADVMain::handleReroutingGateway(Ieee80211DataFrame *)
{
    EV << "BATMAN!" << std::endl;
    Ieee80211DataFrame *frame = new Ieee80211DataFrame();
    frame->setReceiverAddress(MACAddress("11:22:33:44:55:66"));

    sendOrEnqueue(frame);
}

/** Implements abstract Ieee80211MgmtBase method */
void BatmanADVMain::handleTimer(cMessage *msg)
{

}

/** Implements abstract Ieee80211MgmtBase method */
void BatmanADVMain::handleUpperMessage(cPacket *msg)
{
    EV << "BATMAN-handleUpperMessage" << std::endl;
    Ieee80211DataFrame *frame = new Ieee80211DataFrame();
    frame->setReceiverAddress(MACAddress("11:22:33:44:55:66"));

    sendOrEnqueue(frame);
}

/** Implements abstract Ieee80211MgmtBase method -- throws an error (no commands supported) */
void BatmanADVMain::handleCommand(int msgkind, cObject *ctrl)
{

}

/** Utility function for handleUpperMessage() */
Ieee80211DataFrame* BatmanADVMain::encapsulate(cPacket *msg)
{
    return NULL;
}

/** Called by the NotificationBoard whenever a change occurs we're interested in */
void BatmanADVMain::receiveChangeNotification(int category, const cPolymorphic *details)
{
    EV << "BATMAN! receiveChangeNotification" << std::endl;
}
void BatmanADVMain::handleDataFrame(Ieee80211DataFrame *frame)
{
    EV << "BATMAN! handleDataFrame" << std::endl;
}
void BatmanADVMain::handleAuthenticationFrame(Ieee80211AuthenticationFrame *frame)
{
    EV << "BATMAN! handleAuthenticationFrame" << std::endl;
}
void BatmanADVMain::handleDeauthenticationFrame(Ieee80211DeauthenticationFrame *frame)
{
    EV << "BATMAN! handleDeauthenticationFrame" << std::endl;
}
void BatmanADVMain::handleAssociationRequestFrame(Ieee80211AssociationRequestFrame *frame)
{
    EV << "BATMAN! handleAssociationRequestFrame" << std::endl;
}
void BatmanADVMain::handleAssociationResponseFrame(Ieee80211AssociationResponseFrame *frame)
{
    EV << " BATMAN! handleAssociationResponseFrame" << std::endl;
}
void BatmanADVMain::handleReassociationRequestFrame(Ieee80211ReassociationRequestFrame *frame)
{
    EV << "BATMAN! handleReassociationRequestFrame" << std::endl;
}
void BatmanADVMain::handleReassociationResponseFrame(Ieee80211ReassociationResponseFrame *frame)
{
    EV << "BATMAN! handleReassociationResponseFrame" << std::endl;
}
void BatmanADVMain::handleDisassociationFrame(Ieee80211DisassociationFrame *frame)
{
    EV << "BATMAN! handleDisassociationFrame" << std::endl;
}
void BatmanADVMain::handleBeaconFrame(Ieee80211BeaconFrame *frame)
{
    EV << "handleBeaconFrame" << std::endl;
}
void BatmanADVMain::handleProbeRequestFrame(Ieee80211ProbeRequestFrame *frame)
{
    EV << "BATMAN! handleProbeRequestFrame" << std::endl;
}
void BatmanADVMain::handleProbeResponseFrame(Ieee80211ProbeResponseFrame *frame)
{
    EV << "BATMAN! handleProbeResponseFrame" << std::endl;
}


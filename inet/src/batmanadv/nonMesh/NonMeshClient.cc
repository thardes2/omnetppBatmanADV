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

#include <nonMesh/NonMeshClient.h>

Define_Module(NonMeshClient);

NonMeshClient::NonMeshClient()
{

}

NonMeshClient::~NonMeshClient()
{

}

#define MSGKIND_CONNECT  1
#define MSGKIND_SEND     2
#define MSGKIND_CLOSE    3
void NonMeshClient::initialize(int stage)
{
    //TCPAppBase::initialize(stage);
    if (stage == 0)
    {
        bytesSent = bytesRcvd = 0;
        activeOpen = par("active");
        tOpen = par("tOpen");
        tSend = par("tSend");
        tClose = par("tClose");
        sendBytes = par("sendBytes");
        commandIndex = 0;

        //socket.setOutputGate(gate("tcpOut"));
        socket.readDataTransferModePar(*this);
        nodeStatus = dynamic_cast<NodeStatus *>(findContainingNode(this)->getSubmodule("status"));
        ConnectionID = 0;

        cPar macParam = par("mac");
        cPar wakeupTime = par("wakeupTime");
        myMac.setAddress(macParam);
        nodenr = par("nodenr");
        EV << "Client-Node: " << nodenr << ": Collected all parameters and sending a DHCP Discovery to my node" << std::endl;
        /*
         wakeUpMessage = new cMessage("Wakeup");
         std::vector<const char*> x = getGateNames();
         for (unsigned int i = 0; i < x.size(); i++)
         {
         EV << "Client-Node: " << nodenr << ": Gate " << x[i] << std::endl;
         }
         scheduleAt(simTime() + uniform(1, 2), wakeUpMessage);*/
    }
    else if (stage == 3)
    {
        timeoutMsg = new cMessage("timer");
        nodeStatus = dynamic_cast<NodeStatus *>(findContainingNode(this)->getSubmodule("status"));

        if (isNodeUp())
        {
            timeoutMsg->setKind(MSGKIND_CONNECT);
            scheduleAt(tOpen, timeoutMsg);
        }
    }
}

bool NonMeshClient::isNodeUp()
{
    return !nodeStatus || nodeStatus->getState() == NodeStatus::UP;
}
/*
bool NonMeshClient::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
{
    EV << "Client-Node: " << nodenr << ": handleOperationStage" << std::endl;
    Enter_Method_Silent
    ();
    if (dynamic_cast<NodeStartOperation *>(operation))
    {
        if (stage == NodeStartOperation::STAGE_APPLICATION_LAYER)
            startListening();
    }
    else if (dynamic_cast<NodeShutdownOperation *>(operation))
    {
        if (stage == NodeShutdownOperation::STAGE_APPLICATION_LAYER)
            // TODO: wait until socket is closed
            stopListening();
    }
    else if (dynamic_cast<NodeCrashOperation *>(operation))
        ;
    else
        throw cRuntimeError("Unsupported lifecycle operation '%s'", operation->getClassName());
    return true;
}

void NonMeshClient::handleTimer(cMessage *msg)
{
    switch (msg->getKind())
    {
        case MSGKIND_CONNECT:
            EV << "opening connection\n";
            if (activeOpen)
                connect(); // sending will be scheduled from socketEstablished()
            else
                ; //TODO
            break;

        case MSGKIND_SEND:
            sendData();
            break;

        case MSGKIND_CLOSE:
            close();
            break;

        default:
            throw cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void NonMeshClient::sendData()
{
    sendPacket(createDHCPDiscovery());
}*/

void NonMeshClient::startClient()
{
    sendDHCPDiscovery();
}
void NonMeshClient::handleMessage(cMessage *msg)
{
    //if (msg == timeoutMsg)
        //sendData();
    //else
    //{

        EtherFrame* frame = check_and_cast<EtherFrame*>(msg);

        cPacket* higherlayermsg = frame->decapsulate();
        //delete (frame);
        DHCPMessage* dhcp = dynamic_cast<DHCPMessage*>(higherlayermsg);
        if (dhcp)
        {
            EV << "Client-Node: " << nodenr << ": Received DHCP message" << std::endl;

            IPv4Address ip = dhcp->getYiaddr();
            //Byte serverIdB = dhcpOffer->getOptions().get(SERVER_ID);
            IPv4Address serverId = dhcp->getOptions().getServerIdentifier();

            // minimal information to configure the interface
            // create the lease to request

            //lease = new DHCPLease();
            //lease->ip = ip;
             ///lease->mac = myMac;
             //lease->serverId = serverId;
        }
    //}
}

void NonMeshClient::sendDHCPDiscovery()
{
    DHCPMessage* dhcp = createDHCPDiscovery();
    /*TCPCommand* tcpCommand = new TCPCommand();
     tcpCommand->setUserId(nodenr);
     tcpCommand->setConnId(ConnectionID);*/
    TCPSendCommand* tcpCommand = new TCPSendCommand();
    tcpCommand->setConnId(ConnectionID);
    //ConnectionID++;

    /*frame->setControlInfo(tcpCommand);
     frame->setKind(TCP_C_SEND);*/
    dhcp->setKind(TCP_C_SEND);

    dhcp->setControlInfo(tcpCommand);
//    send(frame, "tcpOut");
    //sendDown(frame);
    //sendDown(dhcp);
}

DHCPMessage* NonMeshClient::createDHCPDiscovery()
{
    DHCPMessage* d = new DHCPMessage();
    d->setChaddr(myMac);
    d->setXid(getRandomNr());
    return d;
}

unsigned int NonMeshClient::getRandomNr()
{
    double beta = 10000;
    rnd.collect(beta);
    return (int) exponential(beta, 0);
}
/*
void NonMeshClient::socketEstablished(int connId, void *ptr)
{
    TCPAppBase::socketEstablished(connId, ptr);

    ASSERT(commandIndex==0);
    timeoutMsg->setKind(MSGKIND_SEND);
    scheduleAt(simTime(), timeoutMsg);
}

void NonMeshClient::socketDataArrived(int connId, void *ptr, cPacket *msg, bool urgent)
{
    TCPAppBase::socketDataArrived(connId, ptr, msg, urgent);
}

void NonMeshClient::socketClosed(int connId, void *ptr)
{
    TCPAppBase::socketClosed(connId, ptr);
    //cancelEvent(timeoutMsg);
}

void NonMeshClient::socketFailure(int connId, void *ptr, int code)
{
    TCPAppBase::socketFailure(connId, ptr, code);
    //cancelEvent(timeoutMsg);
}
void NonMeshClient::finish()
{

}

void NonMeshClient::parseScript(const char *script){
}*/


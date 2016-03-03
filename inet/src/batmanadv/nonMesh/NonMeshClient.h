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

#ifndef NONMESHCLIENT_H_
#define NONMESHCLIENT_H_



#include <stdio.h>
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <omnetpp.h>
#include "DHCPMessage_m.h"
#include "TCPAppBase.h"
#include "EtherFrame.h"
#include "TCPSocket.h"
#include "TCPCommand_m.h"
#include "INETDefs.h"
#include "NodeStatus.h"
#include "NodeOperations.h"


#include "ByteArrayMessage.h"
#include "IPvXAddressResolver.h"
#include "ModuleAccess.h"
//#include "TCPSocket.h"
//#include "TCPSocketMap.h"
//#include "IPvXAddressResolver.h"
class NonMeshClient:  public cSimpleModule//public TCPAppBase//public cSimpleModule,public ILifecycle// public INotifiable,
{
    protected:
        TCPSocket socket;
        static simsignal_t rcvdPkSignal;
        static simsignal_t sentPkSignal;
        NodeStatus *nodeStatus;
        long bytesRcvd;
        long bytesSent;
        int commandIndex;
        cMessage *timeoutMsg;


        bool activeOpen;
        simtime_t tOpen;
        simtime_t tSend;
        simtime_t tClose;
        int sendBytes;

    public:

        int ConnectionID;
        int nodenr;
        MACAddress myMac;
        cMessage *wakeUpMessage;
        IPv4Address ipv4;
        //DHCPLease *lease; // leased IP information
        NonMeshClient();
        virtual ~NonMeshClient();
    private:
        cStdDev rnd;
        void sendDHCPDiscovery();
        DHCPMessage* createDHCPDiscovery();
        void startClient();
        unsigned int getRandomNr();

        void stopListening();
        void startListening();
        bool isNodeUp();
    protected:
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);
        virtual int numInitStages() const { return 4; }
        /*virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback);

        virtual void socketEstablished(int connId, void *yourPtr);
        virtual void socketDataArrived(int connId, void *yourPtr, cPacket *msg, bool urgent);
        virtual void socketClosed(int connId, void *yourPtr);
        virtual void socketFailure(int connId, void *yourPtr, int code);


        virtual void parseScript(const char *script);
        virtual void sendData();
        virtual void handleTimer(cMessage *msg);

        virtual void finish();*/
};

#endif /* NONMESHCLIENT_H_ */

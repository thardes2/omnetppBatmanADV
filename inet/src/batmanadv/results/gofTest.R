gof <- function()
{
  path <-  "/home/tobias/Documents/ccs/results/networkSize/sameChannel/noCluster/General-0.sca"
  numOGMReceivedCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOwnOGMSentCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  
  numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<10000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]
  
  overallVPN <- c(numOGMReceivedCountVpnVectorSubset, numOwnOGMSentCountVpnVectorSubset) 
  
  ############################## START: Kolmogorov-Smirnov ##############################
  
  ks.test(numOGMReceivedCountVpnVectorSubset,measuredVPNRealReceived)
  ks.test(measuredVPNRealSend,numOwnOGMSentCountVpnVectorSubset)
  
  ############################## END: Kolmogorov-Smirnov ##############################  
  ############################## START: Kruskal–Wallis ##############################
  receivedTable <- data.table(numOGMReceivedCountVpnVectorSubset)
  receivedTable <- data.table(receivedTable,measuredVPNRealReceived)
  
  sendTable <-  data.table(numOwnOGMSentCountVpnVectorSubset)
  sendTable <-  data.table(sendTable,measuredVPNRealSend)
  
  kruskal.test(numOGMReceivedCountVpnVectorSubset ~ measuredVPNRealReceived, data = receivedTable) 
  kruskal.test(numOwnOGMSentCountVpnVectorSubset ~ measuredVPNRealSend, data = sendTable) 
  ############################## END: Kruskal–Wallis ##############################
}
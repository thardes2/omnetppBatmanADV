analyzeOGMcountGWTermination<- function(path)
{
  path <-  "/home/tobias/Documents/ccs/results/gwTermination/tmp/General-0.sca"
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/gwTermination/tmp/General-0.sca"
  
  
  pathModel <-  "/home/tobias/Documents/ccs/results/singleFastD/noCluster/General-0.sca"
  
  numOwnOGMSentCountVpn <-loadDataset(pathModel, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOwnOGMSentCountVpnVectorSubsetModel <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubsetModel <- numOwnOGMSentCountVpnVectorSubsetModel[numOwnOGMSentCountVpnVectorSubsetModel>15]
  
  numOGMReceivedCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOwnOGMSentCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  
  numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<3000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<100]
  
  overallVPN <- c(numOGMReceivedCountVpnVectorSubset, numOwnOGMSentCountVpnVectorSubset) 
  
  dfReceived <-  data.frame(numOGMReceivedCountVpnVectorSubset)
  dfSent <- data.frame(numOwnOGMSentCountVpnVectorSubset)
  myPlot <- ggplot(dfReceived, aes(numOGMReceivedCountVpnVectorSubset)) + geom_histogram() +ylab("eCDF")+ xlab("Number of received Packets")+
    theme_my(base_family = "Palatino")+expand_limits(x=0) 
  print(myPlot)
  ggsave(filename=paste(mainPathImprovement2,"receivedEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
  myPlot<- ggplot(dfSent, aes(numOwnOGMSentCountVpnVectorSubset)) + geom_histogram() +ylab("Number of nodes")+ xlab("Number of sent Packets")+
    theme_my(base_family = "Palatino")
  print(myPlot)
  
  dfSent <-  rbind(data.frame(value = numOwnOGMSentCountVpnVectorSubset, Legend = 'GW Termination (779)'),
               data.frame(value = numOwnOGMSentCountVpnVectorSubsetModel, Legend = 'No improvement (779)'))
  
  myPlot<- ggplot(dfSent, aes(value, colour = factor(Legend,labels=c("GW Termination (779 Nodes)","No improvement (779 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf()+
    scale_size_manual(breaks=c("GW Termination (779 Nodes)","No improvement (779 Nodes)"), values=c(1,1))+
    scale_color_manual(breaks=c("GW Termination (779 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#9B9B9B"))+
    scale_linetype_manual(breaks=c("GW Termination (779 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(7,1))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
    xlab("Number of packets") + ylab("eCDF")
  
  myPlot<-makePlot(myPlot)
  myPlot <- myPlot +theme(legend.position = c(0.4, 0.12))
  print(myPlot)
  
  ggsave(filename=paste(mainPathImprovement2,"sendEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}

analyzeOGMcountGWTerminationMultiRun<- function(path)
{
  path <- "/home/tobias/Documents/ccs/results/multiRun/gwTermination/packetCount/General-"
  
  
  numOGMReceivedCountVpn <-loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOwnOGMSentCountVpn <-loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]
  
  
  aSent<- data.table(numOwnOGMSentCountVpnVectorSubset[1:532])
  aReceived<- data.table(numOGMReceivedCountVpnVectorSubset[5:677])
  
  x <- c(3,4,6,7,8,9)
  for(i in x)
  {
    numOGMReceivedCountVpn <-loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
    numOwnOGMSentCountVpn <-loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
    numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
    numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
    numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
    numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
    numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
    numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
    numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<1000]
    numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
    numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]
    aSent<- data.table(aSent,numOwnOGMSentCountVpnVectorSubset[1:532])
    aReceived<- data.table(aReceived,numOGMReceivedCountVpnVectorSubset[5:677])
  }
  
  mSent <- rowMeans(aSent,na.rm = TRUE)
  mReceived <- rowMeans(aReceived,na.rm = TRUE)
  sdmReceived <- apply(aReceived,1,sd)
  sdmSent<- apply(aSent,1,sd)
  
  pff <- rowSums(aSent)/ncol(aSent)
  pffReceived <- rowSums(aReceived)/ncol(aReceived)
  error <- qnorm(0.975)*sdmSent/sqrt(length(sdmSent))
  errorReceived <- qnorm(0.975)*sdmReceived/sqrt(length(sdmReceived))
  #######################################################################################################################
  
  errorLower <- pff - error
  errorUpper <- pff + error
  
  limits <- aes(ymax = errorUpper, ymin=errorLower)
  dfSent <- data.frame(pff) 
  
  myPlot<- ggplot(dfSent, aes(pff)) + geom_bar() +ylab("Number of nodes")+ xlab("Number of received Packets")+
    theme_my(base_family = "Palatino") 
  #+geom_errorbar(limits,position=dodge, width=0.25)
  print(myPlot)
  
  dfSent <- data.frame(numOwnOGMSentCountVpnVectorSubset) 
  
  myPlot<- ggplot(dfSent, aes(numOwnOGMSentCountVpnVectorSubset)) + geom_bar() +ylab("Number of nodes")+ xlab("Number of received Packets")+
    theme_my(base_family = "Palatino")# +geom_errorbar(limits,position=dodge, width=0.25)
  print(myPlot)
  
  
  
  errorLower <- pff - errorReceived
  errorUpper <- pff + errorReceived
  limits <- aes(ymax = errorUpper, ymin=errorLower)
  dfReceived <- data.frame(pffReceived) 
  myPlot<- ggplot(dfReceived, aes(pffReceived)) + geom_histogram() +ylab("Number of nodes")+ xlab("Number of received Packets")+
    theme_my(base_family = "Palatino")+geom_errorbar(limits, position=dodge, width=0.25)
  print(myPlot)
  
  
}
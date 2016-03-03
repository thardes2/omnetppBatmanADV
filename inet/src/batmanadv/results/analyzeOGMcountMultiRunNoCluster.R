
analyzeOGMcountNoClusterMultiRun <- function(path)
{
  path <- "/home/tobias/Documents/ccs/results/networkSize/equalDistribution/noCluster/v2/General-"
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/packetCount/General-"
  
  
  numOGMReceivedCountVpn <-loadDataset(paste(paste(path,1, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOwnOGMSentCountVpn <-loadDataset(paste(paste(path,1, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<10000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]


  aSent<- data.table(numOwnOGMSentCountVpnVectorSubset[1:670])
  aReceived<- data.table(numOGMReceivedCountVpnVectorSubset[1:670])
  x <- c(12,16,1,22,34,38,42,45)
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
    numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<10000]
    numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
    numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]
    aSent<- data.table(aSent,numOwnOGMSentCountVpnVectorSubset[1:670])
    aReceived<- data.table(aReceived,numOGMReceivedCountVpnVectorSubset[1:670])
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
   
  errorLower <- numOwnOGMSentCountVpnVectorSubset - error
  errorUpper <- numOwnOGMSentCountVpnVectorSubset + error
  errorLower <- pff - sdmSent
  errorUpper <- pff + sdmSent
  
  
  dSent <- rbind(data.frame(value=pff, Legend = 'Model - mean (779 Nodes)'),
                 data.frame(value=errorLower, Legend = 'Variance'),
                 data.frame(value=errorUpper, Legend = 'Variance2'),
                 data.frame(value=measuredVPNRealSend, Legend = 'Real (779 Nodes)'))  

  #myPlot<- ggplot(dSent, aes(value, colour = factor(Legend,labels=c("Model (779 Nodes)","Variance","","Real (779 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf() + 
  #  scale_size_manual(breaks=c("Model (779 Nodes)","Variance","Variance","Real (779 Nodes)"),name="Legend", values=c(2,1,1,2))+
  #  scale_linetype_manual(breaks=c("Model (779 Nodes)","Variance","Variance","Real (779 Nodes)"), name="Legend",values=c(7,5,5,1))+
  #  scale_color_manual(breaks=c("Model (779 Nodes)","Variance","Variance","Real (779 Nodes)"), name="Legend",values=c("#377EB8","#999999","#999999","#FF7F00"))+
  #  theme_my(base_family = "Palatino") +expand_limits(x=0) + xlab("Number of packets") + ylab("eCDF")+    labs(color = "Legend")
  
  myPlot<- ggplot(dSent, aes(value, colour = factor(Legend,labels=c("Model - mean (779 Nodes)","Variance","","Real (779 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf() + 
    scale_size_manual(breaks=c("Model - mean (779 Nodes)","Variance","Variance","Real (779 Nodes)"),name="Legend", values=c(1,1,1,1))+
    scale_linetype_manual(breaks=c("Model - mean (779 Nodes)","Variance","Variance","Real (779 Nodes)"), name="Legend",values=c(1,5,5,1))+
    scale_color_manual(breaks=c("Model - mean (779 Nodes)","Variance","Variance","Real (779 Nodes)"), name="Legend",values=c("#373737","#CDCDCD","#CDCDCD","#9B9B9B"))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + xlab("Number of packets") + ylab("eCDF")+    labs(color = "Legend")
  
  
  
  
  myPlot<-makePlot(myPlot)
  myPlot<- myPlot+ theme(legend.position = c(0.2, 0.22))
  print(myPlot)
  
  ggsave(filename=paste(mainPath,"sendEcdfMultiRun.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
  
  length(errorUpper)
  length(errorReceived)
  length(numOGMReceivedCountVpnVectorSubset)
  errorLower <- numOGMReceivedCountVpnVectorSubset - errorReceived[1:670]
  errorUpper <- numOGMReceivedCountVpnVectorSubset + errorReceived[1:670]
  errorLower <- pffReceived- sdmReceived
  errorUpper <- pffReceived + sdmReceived
  
  dReceived <- rbind(data.frame(value=pffReceived[1:670], Legend = 'Model - mean (779 Nodes)'),
                 data.frame(value=errorLower, Legend = 'Variance'),
                 data.frame(value=errorUpper, Legend = 'Variance2'),
                 data.frame(value=measuredVPNRealReceived, Legend = 'Real (779 Nodes)'))  
# myPlot<- ggplot(dReceived, aes(x=value, color=factor(source,labels=c("Model (779 Nodes)","95% confidence Lower","95% confidence Upper","Real (779)")),linetype=source))+stat_ecdf()+
#   xlab("Number of packets") + ylab("eCDF") + 
#  scale_color_manual(breaks=c("Model (779 Nodes)","95% confidence Lower","95% confidence Upper","Real (779)"),name="Legend", values=c("#377EB8","#999999","#999999","#FF7F00"))+
#  scale_linetype_manual(breaks=c("Model (779 Nodes)","95% confidence Lower","95% confidence Upper","Real (779)"), name="Legend",values=c(7,5,5,1))+
#    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")

myPlot<- ggplot(dReceived, aes(value, colour = factor(Legend,labels=c("Model - mean (779 Nodes)","Variance","","Real (779 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf()+
  scale_size_manual(breaks=c("Model - mean (779 Nodes)","Variance","Variance","Real (779 Nodes)"), values=c(1,1,1,1))+
  scale_color_manual(breaks=c("Model - mean (779 Nodes)","Variance","Variance","Real (779 Nodes)"),name="Legend", values=c("#373737","#CDCDCD","#CDCDCD","#9B9B9B"))+
  scale_linetype_manual(breaks=c("Model - mean (779 Nodes)","Variance","Variance","Real (779 Nodes)"), name="Legend",values=c(1,5,5,1))+
  theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
  xlab("Number of packets") + ylab("eCDF") 
myPlot<-makePlot(myPlot)

myPlot<- myPlot+ theme(legend.position = c(0.2, 0.22))
print(myPlot)
  ggsave(filename=paste(mainPath,"receivedEcdfMultiRun.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)



#################################################EQUAL DISTRIBUTION######################################################################
if(FALSE)
{
dReceived <- rbind(data.frame(value=numOGMReceivedCountVpnVectorSubset, Legend = 'Model (779 Nodes)'),
                   data.frame(value=measuredVPNRealReceived, Legend = 'Real (779 Nodes)')) 

myPlot<- ggplot(dReceived, aes(value, colour = factor(Legend,labels=c("Model (779 Nodes)","Real (779 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf()+
  scale_size_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"), values=c(1,1))+
  scale_color_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"),name="Legend", values=c("#373737","#9B9B9B"))+
  scale_linetype_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"), name="Legend",values=c(1,1))+
  theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
  xlab("Number of packets") + ylab("eCDF") 
myPlot<-makePlot(myPlot)
print(myPlot)
ggsave(filename=paste(mainPathImprovement,"receivedEcdfEqualDistribution.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}
}
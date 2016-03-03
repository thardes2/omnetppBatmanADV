
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
  
  
  dSent <- rbind(data.frame(value=pff, Legend = 'Model (779 Nodes)'),
                 data.frame(value=errorLower, Legend = 'Variance'),
                 data.frame(value=errorUpper, Legend = 'Variance2'),
                 data.frame(value=measuredVPNRealSend, Legend = 'Real (779 Nodes)'))  
  
  meanSentModel <- mean(pff)
  meanSentVLModel <- mean(errorLower)
  meanSentVUModel <- mean(errorUpper)
  meanSentReal <- mean(measuredVPNRealSend)
  limitsSent <- aes(ymax = meanSentVLModel, ymin=meanSentVUModel)
  
  errorLower <- numOGMReceivedCountVpnVectorSubset - errorReceived
  errorUpper <- numOGMReceivedCountVpnVectorSubset + errorReceived
  errorLower <- pffReceived - sdmReceived
  errorUpper <- pffReceived + sdmReceived
  meanReceivedModel <- mean(pffReceived)
  meanReceivedVLModel <- mean(errorLower)
  meanReceivedVUModel <- mean(errorUpper)
  meanReceivedReal <- mean(measuredVPNRealReceived)
  

  
  x<-data.frame(value = meanSentModel,def="OGM")
  y<-data.frame(value = meanSentReal,def="OGM improved")
  dat <-  rbind(x,y)
  
  
  x<-data.frame(value = c(meanSentModel,meanSentReal))
  y<-data.frame(def= =c("OGMs","OGM improved")
  dat <-  rbind(x,y)
  
  
###############################SENT###############################
mat <- matrix(c(meanSentModel,meanSentReal),nrow=1,ncol=2) 
colnames(mat) <- c("Simulation","Real world")
tmp <- melt(mat)
names(tmp) <- c("Year","Type", "Rate")
dodge <- position_dodge(width=0.9)



df <- data.frame(values=c(meanSentModel,mean(measuredVPNRealSend)),
                 min=c(max(pff),max(measuredVPNRealSend)),
                 max=c(min(pff),min(measuredVPNRealSend)))
limitsSent <- aes(ymax = min, ymin=max)

myPlot <- ggplot(df, aes(x=c("Simulation","Real world"), y=values, fill=factor(values))) +
  geom_bar(stat="identity", position="dodge") +
  scale_fill_manual(values=c("#9B9B9B","#CDCDCD"))+xlab("")+ylab("Number of packets sent")+ theme_my(base_family = "Palatino")+ 
  scale_colour_continuous(guide = FALSE)+
  guides(fill=FALSE)+theme(legend.position = "right")+ geom_errorbar(limitsSent, position=dodge, width=0.25)+
  theme(axis.title.y=element_text(vjust=1.2))
print(myPlot)
ggsave(filename=paste(defensePath,"numOGMSentBarPlot.pdf", sep = ""), width = 8, height = 6, dpi = 120,plot=myPlot)
#############################################################################################


###############################RECEIVED###############################
mat <- matrix(c(meanReceivedModel,meanReceivedReal),nrow=1,ncol=2) 
colnames(mat) <- c("Simulation","Real world")
tmp <- melt(mat)
names(tmp) <- c("Year","Type", "Rate")

df <- data.frame(values=c(meanReceivedModel,meanReceivedReal),
                 min=c(max(pffReceived),max(measuredVPNRealReceived)),
                 max=c(min(pffReceived),min(measuredVPNRealReceived)))
limitsReceived <- aes(ymax = min, ymin=max)
#limitsReceived <- aes(ymax = meanReceivedVLModel, ymin=meanReceivedVUModel)


myPlot <- ggplot(df, aes(x=c("Simulation","Real world"), y=values, fill=factor(values))) +
  geom_bar(stat="identity", position="dodge") +
  scale_fill_manual(values=c("#9B9B9B","#CDCDCD"))+xlab("")+ylab("Number of packets received")+ theme_my(base_family = "Palatino")+ 
  scale_colour_continuous(guide = FALSE)+guides(fill=FALSE)+theme(legend.position = "right")+ geom_errorbar(limitsReceived, position=dodge, width=0.25)+
  theme(axis.title.y=element_text(vjust=1.2))
print(myPlot)
ggsave(filename=paste(defensePath,"numOGMReceivedBarPlot.pdf", sep = ""), width = 8, height = 6, dpi = 120,plot=myPlot)
}



analyzeOGMcountOnefastd <- function(path)
{
  path <- "/home/tobias/Documents/ccs/results/singleFastD/noCluster/General-0.sca"
  path <- "/tmp/nedFile/General-0.sca"
  
  pathOldModel <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/packetCount/General-1.sca"
  
  ###################################################### Old ###################################################### 
  numOGMReceivedCountVpn <-loadDataset(pathOldModel, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOwnOGMSentCountVpn <-loadDataset(pathOldModel, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  
  numOGMReceivedCountVpnVectorSubsetOld <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<9000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubsetOld <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]
  
  
  ###################################################### New ###################################################### 
  numOGMReceivedCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOwnOGMSentCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  
  numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<9000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]
  
  overallVPN <- c(numOGMReceivedCountVpnVectorSubset, numOwnOGMSentCountVpnVectorSubset) 
  
  ############################## START: RECEIVED ##############################
  
  df <-  rbind(data.frame(value = numOGMReceivedCountVpnVectorSubset, Legend = 'Model(820)'),
               data.frame(value = numOGMReceivedCountVpnVectorSubsetOld, Legend = 'Model(779)'),
               data.frame(value = measuredVPNRealReceivedOpti, Legend = 'Real(820)'))
  # pdf(file= paste(mainPath,'receivedEcdf.pdf',sep = ""))
  # myPlot<- ggplot(df, aes(x=value, color=factor(Legend,labels=c("Model (779 Nodes)","Real (779 Nodes)")),linetype=Legend))+ stat_ecdf()+  xlab("Number of packets") + ylab("eCDF") +#
  
  #   scale_color_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"),name="Legend", values=c("#377EB8","#FF7F00"))+
  #   scale_linetype_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"), name="Legend",values=c(5,7))+
  #  theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")
  
  # print(myPlot)
  
  meanNumOGMReceivedCountVpnVectorSubset <- mean(numOGMReceivedCountVpnVectorSubset)
  meanNumOGMReceivedCountVpnVectorSubsetOld <- mean(numOGMReceivedCountVpnVectorSubsetOld)
  meanMeasuredVPNRealReceivedOpti <- mean(measuredVPNRealReceivedOpti)
  
  df <- data.frame(values=c(meanNumOGMReceivedCountVpnVectorSubset,meanNumOGMReceivedCountVpnVectorSubsetOld,meanMeasuredVPNRealReceivedOpti),
                   min=c(max(numOGMReceivedCountVpnVectorSubset),max(numOGMReceivedCountVpnVectorSubsetOld),max(measuredVPNRealReceivedOpti)),
                   max=c(min(numOGMReceivedCountVpnVectorSubset),min(numOGMReceivedCountVpnVectorSubsetOld),min(measuredVPNRealReceivedOpti)))
  limitsReceived <- aes(ymax = min, ymin=max)
  #limitsReceived <- aes(ymax = meanReceivedVLModel, ymin=meanReceivedVUModel)
  
  
  
  myPlot <- ggplot(df, aes(x=c("One VPN - Simulation (820 nodes)","No improvement - Simulation (779 nodes)","One VPN - Real (820 nodes)"), y=values, fill=factor(values))) +
    geom_bar(stat="identity", position="dodge") +
    scale_fill_manual(values=c("#6E6E6E","#CDCDCD","#9B9B9B"))+xlab("")+ylab("Number of packets received")+ theme_my(base_family = "Palatino")+ 
    scale_colour_continuous(guide = FALSE)+guides(fill=FALSE)+theme(legend.position = "right")+ geom_errorbar(limitsReceived, position=dodge, width=0.25)+
    theme(axis.title.y=element_text(vjust=1.2))+theme(axis.text.x = element_text(angle = 54, hjust = 1))
  print(myPlot)
  ggsave(filename=paste(defensePath,"numOGMReceivedOneFastDBarPlot.pdf", sep = ""), width = 8, height = 6, dpi = 120,plot=myPlot)
  
  
  
  numOwnOGMSentCountVpnVectorSubset
  
  meanNumOGMSentCountVpnVectorSubset <- mean(numOwnOGMSentCountVpnVectorSubset)
  meanNumOGMSentCountVpnVectorSubsetOld <- mean(numOwnOGMSentCountVpnVectorSubsetOld)
  meanMeasuredVPNRealSentOpti <- mean(measuredVPNRealSendOpti)
  
  df <- data.frame(values=c(meanNumOGMSentCountVpnVectorSubset,meanNumOGMSentCountVpnVectorSubsetOld,meanMeasuredVPNRealSentOpti),
                   min=c(max(numOwnOGMSentCountVpnVectorSubset),max(numOwnOGMSentCountVpnVectorSubsetOld),max(measuredVPNRealSendOpti)),
                   max=c(min(numOwnOGMSentCountVpnVectorSubset),min(numOwnOGMSentCountVpnVectorSubsetOld),min(measuredVPNRealSendOpti)))
  limitsReceived <- aes(ymax = min, ymin=max)
  #limitsReceived <- aes(ymax = meanReceivedVLModel, ymin=meanReceivedVUModel)
  
  
  
  myPlot <- ggplot(df, aes(x=c("One VPN - Simulation (820 nodes)","No improvement - Simulation (779 nodes)","One VPN - Real (820 nodes)"), y=values, fill=factor(values))) +
    geom_bar(stat="identity", position="dodge") +
    scale_fill_manual(values=c("#6E6E6E","#CDCDCD","#9B9B9B"))+xlab("")+ylab("Number of packets sent")+ theme_my(base_family = "Palatino")+ 
    scale_colour_continuous(guide = FALSE)+guides(fill=FALSE)+theme(legend.position = "right")+ geom_errorbar(limitsReceived, position=dodge, width=0.25)+
    theme(axis.title.y=element_text(vjust=1.2))+theme(axis.text.x = element_text(angle =54, hjust = 1))
  print(myPlot)
  
  ggsave(filename=paste(defensePath,"numOGMSentOneFastDBarPlot.pdf", sep = ""), width = 8, height = 6, dpi = 120,plot=myPlot)
  
}
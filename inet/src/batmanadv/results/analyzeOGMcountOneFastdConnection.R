analyzeOGMcountNoCluster <- function(path)
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
  
  myPlot<- ggplot(df, aes(value, colour = factor(Legend,labels=c("Model - mean (820 Nodes)","Old implementation (779 Nodes)","Real - mean (820 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf()+
    scale_size_manual(breaks=c("Model - mean (820 Nodes)","Old implementation (779 Nodes)","Real - mean (820 Nodes)"), values=c(1,1,1))+
    scale_color_manual(breaks=c("Model - mean (820 Nodes)","Old implementation (779 Nodes)","Real - mean (820 Nodes)"),name="Legend", values=c("#373737","#696969","#CDCDCD"))+
    scale_linetype_manual(breaks=c("Model - mean (820 Nodes)","Old implementation (779 Nodes)","Real - mean (820 Nodes)"), name="Legend",values=c(1,1,1))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
    xlab("Number of packets") + ylab("eCDF")
  myPlot<-myPlot + theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+expand_limits(x=0)
 myPlot <- myPlot+  theme(legend.position = c(0.25, 0.8))+expand_limits(x=0)
  print(myPlot)
  ggsave(filename=paste(mainPathImprovement3,"receivedEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
  
  
  
  ############################## START: SENT ##############################
dSent <- rbind(data.frame(value=numOwnOGMSentCountVpnVectorSubset, Legend = 'Model(820)'),
               data.frame(value=numOwnOGMSentCountVpnVectorSubsetOld, Legend = 'Model(779)'),
               data.frame(value=measuredVPNRealSendOpti, Legend = 'Real(820)'))  
myPlot<- ggplot(dSent, aes(value, colour = factor(Legend,labels=c("Model - mean (820 Nodes)","Old implementation (779 Nodes)","Real - mean (820 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf() + 
  scale_size_manual(breaks=c("Model - mean (820 Nodes)","Old implementation (779 Nodes)","Real - mean (820 Nodes)"), values=c(1,1,1))+
  scale_color_manual(breaks=c("Model - mean (820 Nodes)","Old implementation (779 Nodes)","Real - mean (820 Nodes)"),name="Legend", values=c("#373737","#9B9B9B","#CDCDCD"))+
  scale_linetype_manual(breaks=c("Model - mean (820 Nodes)","Old implementation (779 Nodes)","Real - mean (820 Nodes)"), name="Legend",values=c(1,1,1))+
  theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
  xlab("Number of packets") + ylab("eCDF")

myPlot<-makePlot(myPlot)
myPlot<-myPlot+ theme(legend.position = c(0.35, 0.22))
print(myPlot)
ggsave(filename=paste(mainPathImprovement3,"sendEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)


############################## START: SENT-Multipage ##############################
pushViewport(viewport(layout = grid.layout(2, 1)))

dSent <- rbind(data.frame(value=numOwnOGMSentCountVpnVectorSubsetOld, source = 'Model(779)'))
myPlotOld<- ggplot(dSent, aes(value, colour = factor(source,labels=c("Model - Old implementation (779 Nodes)")),linetype=source,size=source)) + stat_ecdf() + 
  scale_size_manual(breaks=c("Model - Old implementation (779 Nodes)"), values=c(1))+
  scale_color_manual(breaks=c("Model - Old implementation (779 Nodes)"),name="Legend", values=c("#373737"))+
  scale_linetype_manual(breaks=c("Model - Old implementation (779 Nodes)"), name="Legend",values=c(7))+
  theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
  xlab("Number of packets") + ylab("eCDF")

myPlotOld<-makePlot(myPlotOld)
vplayout <- function(x, y) 
  viewport(layout.pos.row = x, layout.pos.col = y)
print(myPlot, vp = vplayout(1, 1))
print(myPlotOld, vp = vplayout(2, 1))
}
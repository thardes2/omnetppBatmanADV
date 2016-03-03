
analyzeOGMcountNodeDistribution <- function(path)
{
  patheven <- "/home/tobias/Documents/ccs/results/networkSize/equalDistribution/noCluster/General-0.sca"
  path <-  "/home/tobias/Documents/ccs/results/networkSize/sameChannel/noCluster/General-0.sca"
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
  
  
  numOGMReceivedCountVpnEven <-loadDataset(patheven, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOwnOGMSentCountVpnEven <-loadDataset(patheven, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  
  numOGMReceivedCountVpnVectorEven <-numOGMReceivedCountVpnEven$scalars$value
  numOGMReceivedCountVpnVectorEven <- numOGMReceivedCountVpnVectorEven[numOGMReceivedCountVpnVectorEven !=max(numOGMReceivedCountVpnVectorEven)]
  numOwnOGMSentCountVpnVectorEven <-numOwnOGMSentCountVpnEven$scalars$value
  numOwnOGMSentCountVpnVectorEven <- numOwnOGMSentCountVpnVectorEven[numOwnOGMSentCountVpnVectorEven !=max(numOwnOGMSentCountVpnVectorEven)]
  numOwnOGMSentCountVpnVectorEven <- numOwnOGMSentCountVpnVectorEven[numOwnOGMSentCountVpnVectorEven != "0"]
  numOGMReceivedCountVpnVectorEven <- numOGMReceivedCountVpnVectorEven[numOGMReceivedCountVpnVectorEven != "0"]
  
  numOGMReceivedCountVpnVectorSubsetEven <- numOGMReceivedCountVpnVectorEven[numOGMReceivedCountVpnVectorEven<9000]
  numOwnOGMSentCountVpnVectorSubsetEven<- numOwnOGMSentCountVpnVectorEven[numOwnOGMSentCountVpnVectorEven<1000]
  numOwnOGMSentCountVpnVectorSubsetEven <- numOwnOGMSentCountVpnVectorSubsetEven[numOwnOGMSentCountVpnVectorSubsetEven>15]
  
  
  ############################## START: RECEIVED ##############################
  
  df <-  rbind(data.frame(value = measuredVPNRealReceived, Legend = 'Real(779)',improvement='Real world'),
               data.frame(value = numOGMReceivedCountVpnVectorSubset, Legend = 'Model(779)',improvement='Old'),
               data.frame(value = numOGMReceivedCountVpnVectorSubsetEven, Legend = 'Model(779)',improvement='Even distribution'))
  myPlot<- ggplot(df, aes(value, colour = factor(Legend,labels=c("Model (779 Nodes)","Real (779 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf()+
    scale_size_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"), values=c(1,1))+
    scale_color_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"),name="Legend", values=c("#373737","#9B9B9B"))+
    scale_linetype_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"), name="Legend",values=c(7,1))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
    xlab("Number of packets") + ylab("eCDF")+facet_grid(improvement~.)
  

  myPlot<-makePlot(myPlot)
  myPlot <- myPlot +theme(legend.position = "none")
  print(myPlot)
  ggsave(filename=paste(mainPathImprovement,"receivedEcdfEqualDistributionCompare.pdf", sep = ""), width = 8, height = 6, dpi = 120, plot=myPlot)
}

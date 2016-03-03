ogmAggregationGwTermination <- function(path)
{
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.vec"
  numAggOGMModel <- loadVectorsShaped(path, add(type="vector",select="name(ogmAggregated)"))
  dAggregated <- rbind(data.frame(value=numAggOGMModel$ogmAggregated))
  myPlot <- ggplot(dAggregated, aes(x=value)) + stat_ecdf()
  myPlot<-makePlot(myPlot)
  myPlot <- myPlot+ theme(legend.position = c(0.8, 0.22))+expand_limits(x=0)+  theme_my(base_family = "Palatino")
  print(myPlot)
  
  
}



ogmAggregation <- function(path)
{
  path <- "/home/tobias/Documents/ccs/results/networkSize/sameChannel/noCluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/clientAnnouncement/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/aggregation/General-0.vec"
  numAggOGMModel <- loadVectorsShaped(path, add(type="vector",select="name(ogmAggregated)"))
 
  x <- read.table("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/wireshark/csv/aggregationcount_batman_dev02.csv",sep = ",", header=FALSE)
 
  dAggregated <- rbind(data.frame(value=numAggOGMModel$ogmAggregated, Legend="Model(779)"),
                      data.frame(value=x$V1, Legend="Real(779)"))
 
  myPlot <- ggplot(dAggregated, aes(x=value, color=factor(Legend,labels=c("Model (779 Nodes)","Real (779 Nodes)")),linetype = Legend,size=Legend)) + stat_ecdf()+
    scale_linetype_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"), name="Legend",values=c(7,1))+
  xlab("Number of aggregated OGMs per packet")+ ylab("eCDF")+  theme_my(base_family = "Palatino") +
  scale_size_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"), values=c(1,1))+
  scale_color_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"),name="Legend", values=c("#373737","#CDCDCD"))+
  theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")
  myPlot<-makePlot(myPlot)
  myPlot <- myPlot+ theme(legend.position = c(0.8, 0.22))+expand_limits(x=0)
  print(myPlot)

  ks.test(numAggOGMModel$ogmAggregated,x$V1)
  ks.test (numAggOGMModel$ogmAggregated,x$V1, alternative="g",NULL)$p.value
  
  ggsave(filename=paste(mainPath,"aggregationEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
  
}

ogmAggregationCluster <- function(path,pathNoCluster)
{
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/packetCount/General-0.vec"
  numAggOGMModel <- loadVectorsShaped(pathNoCluster, add(type="vector",select="name(ogmAggregated)"))
  
  numAggOGMBueren <- loadDataset(path, add(type="vector",select="name(ogmAggregatedBuerenAvg)"))
  numAggOGMPb <- loadDataset(path, add(type="vector",select="name(ogmAggregatedPaderbornAvg)"))
  numAggOGMBali <- loadDataset(path, add(type="vector",select="name(ogmAggregatedBaliSchlangenHoevelhofAvg)"))
  numAggOGMRemainder <- loadDataset(path, add(type="vector",select="name(ogmAggregatedRemainderAvg)"))
  
  numAggOGMBuerenVectors <- loadVectors(numAggOGMBueren,NULL)
  numAggOGMPbVectors <- loadVectors(numAggOGMPb,NULL)
  numAggOGMBaliVectors <- loadVectors(numAggOGMBali,NULL)
  numAggOGMRemainderVectors <- loadVectors(numAggOGMRemainder,NULL)
  dAggregated <- rbind(data.frame(value=numAggOGMBuerenVectors$vectordata$y, Legend="Bueren"),
                       data.frame(value=numAggOGMPbVectors$vectordata$y, Legend="Paderborn"),
                       data.frame(value=numAggOGMBaliVectors$vectordata$y, Legend="Lippspringe"), 
                       data.frame(value=numAggOGMRemainderVectors$vectordata$y, Legend="Remainder"),
                       data.frame(value=numAggOGMModel$ogmAggregated, Legend="Model")#,data.frame(value=x$V1, source="Real")
  )
  pdf(file= paste(mainPath,'improvement/aggregationEcdf.pdf',sep = ""))
  myPlot <- ggplot(dAggregated, aes(x=value, color=Legend,linetype = Legend)) + stat_ecdf()+ ggtitle("Aggregated packets (Mesh-vpn)")+
    xlab("# packets")+ ylab("Cumulative of aggregated packets")+  theme_my(base_family = "Palatino") 
    scale_color_manual(breaks=c("Model(779)","Real(779)"),name="Legend", values=c("#377EB8","#FF7F00"))+
    scale_linetype_manual(breaks=c("Model(779)","Real(779)"), name="Legend",values=c(5,7))+
    theme_my(base_family = "Palatino") 
  
  myPlot<-makePlot(myPlot)
  myPlot <- myPlot+ theme(legend.position = c(0.8, 0.22))+expand_limits(x=0)
  ggsave(filename=paste(mainPathImprovement,"aggregationEcdf.pdf", sep = ""), plot=myPlot)
}
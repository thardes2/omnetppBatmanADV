

propagationModels <- function()
{
  pathFreeSpaceModel <- "/home/tobias/Documents/ccs/results/networkSize/propagationModels/FreeSpaceModel/General-0.sca"
  pathLogNormalShadowingModel <- "/home/tobias/Documents/ccs/results/networkSize/propagationModels/LogNormalShadowingModel/General-0.sca"
  pathNakagamiModel <- "/home/tobias/Documents/ccs/results/networkSize/propagationModels/NakagamiModel/General-0.sca"
  pathRayleighModel <- "/home/tobias/Documents/ccs/results/networkSize/propagationModels/RayleighModel/General-0.sca"
  pathRiceModel <- "/home/tobias/Documents/ccs/results/networkSize/propagationModels/RiceModel/General-0.sca"
  pathTwoRayGroundModel <- "/home/tobias/Documents/ccs/results/networkSize/propagationModels/TwoRayGroundModel/General-0.sca"
  
  
  FreeSpaceReceived <-loadDataset(pathFreeSpaceModel, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  LogNormalReceived <-loadDataset(pathLogNormalShadowingModel, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  NakagamiReceived <-loadDataset(pathNakagamiModel, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  RayleighReceived <-loadDataset(pathRayleighModel, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  RicModelReceived <-loadDataset(pathRiceModel, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  TwoRayGroundReceived <-loadDataset(pathTwoRayGroundModel, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  
  FreeSpaceSent <-loadDataset(pathFreeSpaceModel, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  LogNormalSent <-loadDataset(pathLogNormalShadowingModel, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  NakagamiSent <-loadDataset(pathNakagamiModel, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  RayleighSent <-loadDataset(pathRayleighModel, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  RicModelSent <-loadDataset(pathRiceModel, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  TwoRayGroundSent <-loadDataset(pathTwoRayGroundModel, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  
  FreeSpaceReceivedVector <- FreeSpaceReceived$scalars$value
  LogNormalReceivedVector <- LogNormalReceived$scalars$value
  NakagamiReceivedVector <- NakagamiReceived$scalars$value
  RayleighReceivedVector <- RayleighReceived$scalars$value
  RicModelReceivedVector <- RicModelReceived$scalars$value
  TwoRayGroundReceivedVector <- TwoRayGroundReceived$scalars$value
  
  FreeSpaceSentVector <- FreeSpaceSent$scalars$value
  LogNormalSentVector <- LogNormalSent$scalars$value
  NakagamiSentVector <- NakagamiSent$scalars$value
  RayleighSentVector <- RayleighSent$scalars$value
  RicModelSentVector <- RicModelSent$scalars$value
  TwoRayGroundSentVector <- TwoRayGroundSent$scalars$value
  
  FreeSpaceReceivedVector <- FreeSpaceReceivedVector[FreeSpaceReceivedVector != "0"]
  LogNormalReceivedVector <- LogNormalReceivedVector[LogNormalReceivedVector != "0"]
  NakagamiReceivedVector <- NakagamiReceivedVector[NakagamiReceivedVector != "0"]
  RayleighReceivedVector <- RayleighReceivedVector[RayleighReceivedVector != "0"]
  RicModelReceivedVector <- RicModelReceivedVector[RicModelReceivedVector != "0"]
  TwoRayGroundReceivedVector <- TwoRayGroundReceivedVector[TwoRayGroundReceivedVector != "0"]
  
  FreeSpaceReceivedSubset <- FreeSpaceReceivedVector[FreeSpaceReceivedVector<10000]
  LogNormalReceivedSubset <- LogNormalReceivedVector[LogNormalReceivedVector<10000]
  NakagamiReceivedSubset <- NakagamiReceivedVector[NakagamiReceivedVector<10000]
  RayleighReceivedSubset <- RayleighReceivedVector[RayleighReceivedVector<10000]
  RicModelReceivedSubset <- RicModelReceivedVector[RicModelReceivedVector<10000]
  TwoRayGroundReceivedSubset <- TwoRayGroundReceivedVector[TwoRayGroundReceivedVector<10000]
  
  FreeSpaceSentSubset <- FreeSpaceSentVector[FreeSpaceSentVector<1000]
  FreeSpaceSentSubset <- FreeSpaceSentSubset[FreeSpaceSentSubset>15]  
  LogNormalSentSubset <- LogNormalSentVector[LogNormalSentVector<1000]
  LogNormalSentSubset <- LogNormalSentSubset[LogNormalSentSubset>15]    
  NakagamiSentSubset <- NakagamiSentVector[NakagamiSentVector<1000]
  NakagamiSentSubset <- NakagamiSentSubset[NakagamiSentSubset>15]    
  RayleighSentSubset <- RayleighSentVector[RayleighSentVector<1000]
  RayleighSentSubset <- RayleighSentSubset[RayleighSentSubset>15]   
  RicModelSentSubset <- RicModelSentVector[RicModelSentVector<1000]
  RicModelSentSubset <- RicModelSentSubset[RicModelSentSubset>15]    
  TwoRayGroundSentSubset <- TwoRayGroundSentVector[TwoRayGroundSentVector<1000]
  TwoRayGroundSentSubset <- TwoRayGroundSentSubset[TwoRayGroundSentSubset>15]
  
  dSent <- rbind(data.frame(value=FreeSpaceReceivedSubset, source="FreeSpace"), 
                 data.frame(value=LogNormalReceivedSubset, source="LogNormal"),
                 data.frame(value=NakagamiReceivedSubset, source="Nakagami"), 
                 data.frame(value=RayleighReceivedSubset, source="Rayleigh"),
                 data.frame(value=RicModelReceivedSubset, source="RiceModel"),
                 data.frame(value=TwoRayGroundReceivedSubset, source = 'TwoRayGround'))
  #,data.frame(value=measuredVPNRealSend, source = 'Real(779)'))
  
  
  
  pdf(file= paste(mainPath,'/propagationModel.pdf',sep = ""))
  ggplot(dSent, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Received packets (Mesh-vpn)")+ xlab("#packets") + ylab("Cumulative of nodes") +
    scale_color_manual(breaks=c("FreeSpace","LogNormal","Nakagami","Rayleigh","RiceModel","TwoRayGround"),name="Legend", values=c("#999999","#FF7F00","#A65628","#377EB8","#F781BF","#984EA3"))+
    scale_linetype_manual(breaks=c("FreeSpace","LogNormal","Nakagami","Rayleigh","RiceModel","TwoRayGround"), name="Legend",values=c(5,1,7,4,5,9))+
    theme_my(base_family = "Palatino")#+ coord_cartesian(xlim=c(2000,5000)) #+ theme(legend.position = c(0.8, 0.2)) +  theme(legend.background = element_rect(colour = 'black', size = 1, linetype='dashed'))
  dev.off()
  
  
}










analyzeNewNodeClusterPropagation <- function(){
  nodeReachedPB <- loadDataset("General-0.sca", add(type="scalar",select="name(#allNodesKnownPb)"))
  nodeReachedHoevelhofBaliSchlangen <- loadDataset("General-0.sca", add(type="scalar",select="name(#allNodesKnownHoevelhofBaliSchlangenMeshID)"))
  nodeReachedRemainder <- loadDataset("General-0.sca", add(type="scalar",select="name(#allNodesKnownRemainder)"))
  nodeReachedBueren <- loadDataset("General-0.sca", add(type="scalar",select="name(#allNodesKnownBueren)"))
  #nodeReached <- loadDataset("/home/tobias/Documents/ccs/results/General-0.sca", add(type="scalar",select="name(#allNodesKnown)"))
  #nodeReachedVector <-nodeReached$scalars$value
  
  nodeReachedPBVector <- nodeReachedPB$scalars$value
  nodeReachedHoevelhofBaliSchlangenVector <- nodeReachedHoevelhofBaliSchlangen$scalars$value
  nodeReachedRemainderVector <-nodeReachedRemainder$scalars$value
  nodeReachedBuerenVector <- nodeReachedBueren$scalars$value
  
  
  
  #PB
  df <- data.frame(x = nodeReachedPBVector)
  #pdf(file= paste(mainPath,'delayGlobalNodeAnnouncement.pdf',sep = ""))
  ggplot(df, aes(x, colour = 'Simulation')) + stat_ecdf()+ ggtitle("Node propagation delay") + xlab("Time (seconds)") + ylab("Nodes reached (%)") 
  
  #Buren
  df <- data.frame(x = nodeReachedBuerenVector)
  #pdf(file= paste(mainPath,'delayGlobalNodeAnnouncement.pdf',sep = ""))
  ggplot(df, aes(x, colour = 'Simulation')) + stat_ecdf()+ ggtitle("Node propagation delay") + xlab("Time (seconds)") + ylab("Nodes reached (%)") 
  
  #Remainder
  df <- data.frame(x = nodeReachedRemainderVector)
  #pdf(file= paste(mainPath,'delayGlobalNodeAnnouncement.pdf',sep = ""))
  ggplot(df, aes(x, colour = 'Simulation')) + stat_ecdf()+ ggtitle("Node propagation delay") + xlab("Time (seconds)") + ylab("Nodes reached (%)") 
  
  
  #Schlangen
  df <- data.frame(x = nodeReachedHoevelhofBaliSchlangenVector)
  #pdf(file= paste(mainPath,'delayGlobalNodeAnnouncement.pdf',sep = ""))
  ggplot(df, aes(x, colour = 'Simulation')) + stat_ecdf()+ ggtitle("Node propagation delay") + xlab("Time (seconds)") + ylab("Nodes reached (%)") 
  
  
  
  #Density plot
    #d <- rbind(data.frame(count=nodeReachedVector, source="sim"))
    #pSent <- ggplot(d) + geom_density(aes(colour=source,x=count)) + expand_limits(x=0)+ ggtitle("Node propagation delay")+ annotate("text", x = 50, y = -0.01, label = "New node joins")
    #+ annotate("text", x = 60, y = 0.005, label = "Gateway reached")+ annotate("text", x = 81.34, y = 0.028, label = "Next hop after gateway and all remaining\n gateways reached")+ annotate("text", x = 100.20, y = 0.07, label = "All direct neighbors of gateways reached")+ annotate("text", x = 102.49, y = 0.042, label = "Next mesh\nnodes reached")
    #print(pSent)
  
  #ECDF Plot
    #df <- data.frame(x = nodeReachedVector)
  #pdf(file= paste(mainPath,'delayGlobalNodeAnnouncement.pdf',sep = ""))
   #ggplot(df, aes(x, colour = 'Simulation')) + stat_ecdf()+ ggtitle("Node propagation delay") + xlab("Time (seconds)") + ylab("Nodes reached (%)") 
  #dev.off()
}

plotRealClientData <- function()
{
  clientData = read.csv("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/networkData/overallClients.csv",sep = ",",header=TRUE)  # read csv file 
  ts.dat <- read.zoo("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/networkData/overallClients.csv",sep = ",",header=FALSE
                     ,format = '%Y-%M-%d %H:%M:%S', FUN=as.POSIXct)
  #dat <- data.frame(x= clientData$X2015.06.30.16.12.00,y= clientData$X1648.0)
  #dat.xts <- xts(clientData$X1648.0,clientData$X2015.06.30.16.12.00)
  ep <- endpoints(ts.dat,'minutes',k=60)
  period.apply(x=ts.dat,ep,FUN=mean )
  #pdf(file= paste(mainPath,'clientsMonth.pdf',sep = ""))
    plot(ts.dat, xlab = "Time",ylab="Number of non-mesh clients",main="Number of non-mesh clients in July 2015")
  ts.dat
  ts.dat$
  ggplot()+geom_histogram(data=clientData,aes(X2015.06.30.16.12.00))
  
  clientData$
  ggplot(clientData) + geom_density(aes(x=X1648.0))

 # dev.off()
}
##!!!!
analyzeAggregationCountAVG <- function(cluster)
{
  path <- "/home/tobias/Documents/ccs/results/optiNetSplitting/base/v10/cluster/General-0.vec"
  
  meanVectorBueren<- getAvgOfVectorData(path,"name(ogmAggregatedBuerenAvg)")#6.728037
  meanVectorPaderborn<-getAvgOfVectorData(path,"name(ogmAggregatedPaderbornAvg)")#6.677061
  meanVectorBaliSchlangenHoevelhof <-getAvgOfVectorData(path,"name(ogmAggregatedBaliSchlangenHoevelhofAvg)")   #6.297392
  meanVectorRemainder <-getAvgOfVectorData(path,"name(ogmAggregatedRemainderAvg)")#5.543824
  #boxplot(meanVectorBueren)
  #boxplot(meanVectorPaderborn)
  #boxplot(meanVectorBaliSchlangenHoevelhof)
  #boxplot(meanVectorRemainder)
}

analyzeOGMByteLengthSend <- function(path, vectorname )
{  
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v7/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/nodeAnnouncement/tmp/General-0.vec"
  byteLengthsOGMBueren <- loadDataset(path, add(type="vector",select="name(ogmByteSizeBuerenSendAvg)"))
  byteLengthsOGMPb <- loadDataset(path, add(type="vector",select="name(ogmByteSizePaderbornSendAvg)"))
  byteLengthsOGMBali <- loadDataset(path, add(type="vector",select="name(ogmByteSizeBaliSchlangenHoevelhofSendAvg)"))
  byteLengthsOGMRemainder <- loadDataset(path, add(type="vector",select="name(ogmByteSizeRemainderSendAvg)"))
  
  byteLengthsOGMBuerenVectors <- loadVectors(byteLengthsOGMBueren,NULL)
  byteLengthsOGMPbVectors <- loadVectors(byteLengthsOGMPb,NULL)
  byteLengthsOGMBaliVectors <- loadVectors(byteLengthsOGMBali,NULL)
  byteLengthsOGMRemainderVectors <- loadVectors(byteLengthsOGMRemainder,NULL)
  
  x <- read.table("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/wireshark/csv/length_batman_dev02.csv",sep = ",", header=FALSE)
  
  path <- "/home/tobias/Documents/ccs/results/newMemory/v7/noCluster/General-0.vec"
  byteLengthsOGMRemainder <- loadDataset(path, add(type="vector",select="name(ogmByteSizeRemainderSendAvg)"))
  byteLengthsOGMModelVectors <- loadVectors(byteLengthsOGMRemainder,NULL)
  
  mean(byteLengthsOGMModelVectors$vectordata$y)
  dLength <- rbind(data.frame(value=byteLengthsOGMModelVectors$vectordata$y, source="Model"),
                   data.frame(value=x$V1, source="Real"))
  
  dLength <- rbind(data.frame(value=byteLengthsOGMBuerenVectors$vectordata$y, source="Bueren (139)"),
                   data.frame(value=byteLengthsOGMBaliVectors$vectordata$y, source="BaliSchlangenHoevelhof (58)"),
                   data.frame(value=byteLengthsOGMPbVectors$vectordata$y, source="Paderborn (271)"),
                   data.frame(value=byteLengthsOGMRemainderVectors$vectordata$y, source="Remainder (318)"),
                   data.frame(value=byteLengthsOGMModelVectors$vectordata$y, source="Model"),
                   data.frame(value=x$V1, source="Real"))
  ggplot(dLength, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Length of sent packets (Mesh-vpn)")+xlab("# packets")+ylab("")+  theme_my(base_family = "Palatino") 
}


analyzeOGMByteLengthReceived <- function(path, vectorname )
{
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.vec"
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v6/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/nodeAnnouncement/tmp/General-0.vec"
  
  byteLengthsOGMBueren <- loadDataset(path, add(type="vector",select="name(ogmByteSizeBuerenAvg)"))
  byteLengthsOGMPb <- loadDataset(path, add(type="vector",select="name(ogmByteSizePaderbornAvg)"))
  byteLengthsOGMBali <- loadDataset(path, add(type="vector",select="name(ogmByteSizeBaliSchlangenHoevelhofAvg)"))
  byteLengthsOGMRemainder <- loadDataset(path, add(type="vector",select="name(ogmByteSizeRemainderAvg)"))
  
  byteLengthsOGMBuerenVectors <- loadVectors(byteLengthsOGMBueren,NULL)
  byteLengthsOGMPbVectors <- loadVectors(byteLengthsOGMPb,NULL)
  byteLengthsOGMBaliVectors <- loadVectors(byteLengthsOGMBali,NULL)
  byteLengthsOGMRemainderVectors <- loadVectors(byteLengthsOGMRemainder,NULL)
  
  
  
  path <- "/home/tobias/Documents/ccs/results/newMemory/v6/noCluster/General-0.vec"
  byteLengthsOGMModel <- loadDataset(path, add(type="vector",select="name(ogmByteSizeRemainderAvg)"))
  byteLengthsOGMModelVectors <- loadVectors(byteLengthsOGMRemainder,NULL)
  
  
  x <- read.table("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/wireshark/csv/length_batman_dev02.csv",sep = ",", header=FALSE)
  dLength <- rbind(data.frame(value=byteLengthsOGMModelVectors$vectordata$y, source="Model"),
                   data.frame(value=x$V1, source="Real"))
    
  dLength <- rbind(data.frame(value=byteLengthsOGMBuerenVectors$vectordata$y, source="Bueren (139)"),
                   data.frame(value=byteLengthsOGMBaliVectors$vectordata$y, source="BaliSchlangenHoevelhof (58)"),
                   data.frame(value=byteLengthsOGMPbVectors$vectordata$y, source="Paderborn (271)"),
                   data.frame(value=byteLengthsOGMRemainderVectors$vectordata$y, source="Remainder (318)"),
                   data.frame(value=byteLengthsOGMModelVectors$vectordata$y, source="Model"),
                   data.frame(value=x$V1, source="Real"))
  ggplot(dLength, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Length of packets (Mesh-vpn)")+xlab("# packets")+ylab("")+  theme_my(base_family = "Palatino") 
  mean(x$V1)
  mean(byteLengthsOGMRemainderVectors$vectordata$y)
}

getAvgOfVectorData <- function(path, vectorname )
{
  #path <- "/home/tobias/Documents/ccs/results/optiNetSplitting/base/v10/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/optiNetSplitting/base/v11/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v2/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v9/cluster/General-0.vec"
 
  numAggOGMBueren <- loadDataset(path, add(type="vector",select="name(ogmAggregatedBuerenAvg)"))
  numAggOGMPb <- loadDataset(path, add(type="vector",select="name(ogmAggregatedPaderbornAvg)"))
  numAggOGMBali <- loadDataset(path, add(type="vector",select="name(ogmAggregatedBaliSchlangenHoevelhofAvg)"))
  numAggOGMRemainder <- loadDataset(path, add(type="vector",select="name(ogmAggregatedRemainderAvg)"))
  
  numAggOGMBuerenVectors <- loadVectors(numAggOGMBueren,NULL)
  numAggOGMPbVectors <- loadVectors(numAggOGMPb,NULL)
  numAggOGMBaliVectors <- loadVectors(numAggOGMBali,NULL)
  numAggOGMRemainderVectors <- loadVectors(numAggOGMRemainder,NULL)
  
  summary(numAggOGMBuerenVectors$vectordata$y)
  sd(numAggOGMBuerenVectors$vectordata$y)
  
  summary(numAggOGMPbVectors$vectordata$y)
  sd(numAggOGMPbVectors$vectordata$y)
  summary(numAggOGMBaliVectors$vectordata$y)
  sd(numAggOGMBaliVectors$vectordata$y)
  summary(numAggOGMRemainderVectors$vectordata$y)
  sd(numAggOGMRemainderVectors$vectordata$y)
  
  
    path <- "/home/tobias/Documents/ccs/results/newMemory/v1/noCluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v4/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v9/noCluster/General-0.vec"
           
  
  numAggOGMModel <- loadVectorsShaped(path, add(type="vector",select="name(ogmAggregated)"))
  #numAggOGMModelVectors <- loadVectors(numAggOGMModel,NULL)
  #summary(numAggOGMModelVectors$vectordata$y)
  
  #RealWorld
  #clientData = read.csv("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/wireshark/csv/thardes2_batman_dev02",sep = ",", header=TRUE)  # read csv file 
  #summary(clientData$Length)
  x <- read.table("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/wireshark/csv/aggregationcount_batman_dev02.csv",sep = ",", header=FALSE)
  
  dAggregated <- rbind(data.frame(value=numAggOGMModel$ogmAggregated, Legend="Model(779)"),
                       data.frame(value=x$V1, Legend="Real(779)"))
  

  ggplot(dAggregated, aes(x=value, color=Legend,linetype = Legend)) + geom_histogram(aes(y=..count../sum(..count..)))+ ggtitle("Aggregated packets (Mesh-vpn)")+
    xlab("# packets")+ ylab("Cumulative of aggregated packets")+  theme_my(base_family = "Palatino") 
  
  
  
  scale_color_manual(breaks=c("Model(779)","Real(779)"),name="Legend", values=c("#377EB8","#FF7F00"))+
    scale_linetype_manual(breaks=c("Model(779)","Real(779)"), name="Legend",values=c(5,7))+
    theme_my(base_family = "Palatino") 
  
  
  
  pdf(file= paste(mainPath,'aggregationEcdf.pdf',sep = ""))
  ggplot(dAggregated, aes(x=value, color=Legend,linetype = Legend)) + stat_ecdf()+ ggtitle("Aggregated packets (Mesh-vpn)")+
    xlab("# packets")+ ylab("Cumulative of aggregated packets")+  theme_my(base_family = "Palatino") 
  scale_color_manual(breaks=c("Model(779)","Real(779)"),name="Legend", values=c("#377EB8","#FF7F00"))+
    scale_linetype_manual(breaks=c("Model(779)","Real(779)"), name="Legend",values=c(5,7))+
    theme_my(base_family = "Palatino") 
  dev.off()
  
  
  
  dAggregated <- rbind(data.frame(value=numAggOGMBuerenVectors$vectordata$y, Legend="Bueren"),
                       data.frame(value=numAggOGMPbVectors$vectordata$y, Legend="Paderborn"),
                       data.frame(value=numAggOGMBaliVectors$vectordata$y, Legend="Lippspringe"), 
                       data.frame(value=numAggOGMRemainderVectors$vectordata$y, Legend="Remainder"),
                       data.frame(value=numAggOGMModel$ogmAggregated, Legend="Model")#,data.frame(value=x$V1, source="Real")
                       )
  pdf(file= paste(mainPath,'improvement/aggregationEcdf.pdf',sep = ""))
  ggplot(dAggregated, aes(x=value, color=Legend,linetype = Legend)) + stat_ecdf()+ ggtitle("Aggregated packets (Mesh-vpn)")+
    xlab("# packets")+ ylab("Cumulative of aggregated packets")+  theme_my(base_family = "Palatino") 
  scale_color_manual(breaks=c("Model(779)","Real(779)"),name="Legend", values=c("#377EB8","#FF7F00"))+
    scale_linetype_manual(breaks=c("Model(779)","Real(779)"), name="Legend",values=c(5,7))+
    theme_my(base_family = "Palatino") 
  dev.off()
  
 table(x$Length)
#   1 = 3893
#   2 = 192
#   3 = 57
#   4 = 43
#   5 = 63
#   6 = 83
#   7 = 104
#   8 = 95
#   9 = 128
#   10 = 120
#   11 = 120
#   12 = 141
#   13 = 247
#   14 = 94 
#   15 = 81
#   16 = 50
#   17 = 49
#   18 = 42
#   19 = 64


  
  #numAggOGMVectorsSplit <- split(numAggOGMVectors$vectordata, numAggOGMVectors$vectordata$resultkey)

  #meanVector <- c()
  #numIterations <-max(numAggOGMVectors$vectordata$resultkey)
  #numIterations <- numIterations+1
  #x <- 1:numIterations
  #for(i in x){
  #  meanVector[i] <-mean(numAggOGMVectorsSplit[[i]]$y)
  #}  
  
  
}

analyzeWiresharkCsv <- function()
{
  
  clientData = read.csv("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/wireshark/csv/thardes2_batman_dev02",sep = ",")  # read csv file 
  #ts.dat <- read.zoo("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/wireshark/csv/thardes2_batman_dev02",sep = ",",header=TRUE)
  summary(clientData$Length)
  allCount <- count(clientData, "Length")
  summary(allCount)
  sum(allCount$freq)
}


analyzeAggregationCount <- function(cluster)
{
  path <- "/home/tobias/Documents/ccs/results/optiNetSplitting/base/v9/cluster/General-0.vec"
 # path <- "General-0.vec"
  cluster <- 1
  if(cluster==1)
  {
    ogmAggregatedBueren<-loadVectorsShaped(path, add(type="vector",select="name(ogmAggregatedBueren)"))#vectors$ogmReceivedBueren
    ogmAggregatedPaderborn<-loadVectorsShaped(path, add(type="vector",select="name(ogmAggregatedPaderborn)"))#vectors$ogmReceivedBueren
    ogmAggregatedBaliSchlangenHoevelhof <-loadVectorsShaped(path, add(type="vector",select="name(ogmAggregatedBaliSchlangenHoevelhof)"))#vectors$ogmReceivedBueren
    ogmAggregatedBueren<-ogmAggregatedBueren[!(ogmAggregatedBueren$ogmAggregatedBueren==0|ogmAggregatedBueren$ogmAggregatedBueren>4000),]
    ogmAggregatedPaderborn<-ogmAggregatedPaderborn[!(ogmAggregatedPaderborn$ogmAggregatedPaderborn==0|ogmAggregatedPaderborn$ogmAggregatedPaderborn>4000),]
    ogmAggregatedBaliSchlangenHoevelhof<-ogmAggregatedBaliSchlangenHoevelhof[!(ogmAggregatedBaliSchlangenHoevelhof$ogmAggregatedBaliSchlangenHoevelhof==0|ogmAggregatedBaliSchlangenHoevelhof$ogmAggregatedBaliSchlangenHoevelhof>4000),]
  }
  ogmAggregatedRemainder <-loadVectorsShaped(path, add(type="vector",select="name(ogmAggregatedRemainder)"))#vectors$ogmReceivedBueren
  ogmAggregatedRemainder<-ogmAggregatedRemainder[!(ogmAggregatedRemainder$ogmAggregatedRemainder==0|ogmAggregatedRemainder$ogmAggregatedRemainder>4000),]
 
  if(cluster==1)
  {

    dAggregated <- rbind(data.frame(value=ogmAggregatedBueren$ogmAggregatedBueren, source="Bueren"),
                         data.frame(value=ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof, source="BaliSchlangenHoevelhof"),
                         data.frame(value=ogmAggregatedPaderborn$ogmAggregatedPaderborn, source="Paderborn"), 
                         data.frame(value=ogmAggregatedRemainder$ogmAggregatedRemainder, source="Remainder"))
    ggplot(dAggregated, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Aggregated packets (Mesh-vpn)") + coord_cartesian(xlim=c(0,2000))
    pAggregated <- ggplot(dAggregated) + geom_boxplot(aes(x=source,y=value)) + expand_limits(y=0)+ ggtitle("Aggregated packets") + xlab("Subnets")+ ylab("Packet count")
    #pdf(file= paste(mainPath,'/improvement/numOGMAggregatedCluster.pdf',sep = ""))
      print(pAggregated)
   # dev.off()
  }
  else
  {
    dAggregated <- rbind(data.frame(count=ogmAggregatedRemainder$ogmAggregatedRemainder, source="Remainder"))
    pAggregated <- ggplot(dAggregated) + geom_boxplot(aes(x=source,y=count)) + expand_limits(y=0)+ ggtitle("Aggregated packets") + xlab("Subnets")+ ylab("Packet count")
    
    #pdf(file= paste(mainPath,'/improvement/numOGMAggregatedNonCluster.pdf',sep = ""))
      print(pAggregated)
    #dev.off()
  }
}


analyzeOGMTEMP <- function()
{
  
  path <- "/home/tobias/Documents/ccs/results/newMemory/v7/cluster/General-0.sca"
  
  
  OwnOGMSentCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  #numOGMReceivedCountWLAN <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOGMReceivedCountWLAN)"))
  #numOwnOGMSentCountWlan <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOwnOGMSentCountWlan)"))
  numOwnOGMSentCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  
}


analyzeOGMcountNetworkSplitting <- function()
{
  #path <- "/home/tobias/Documents/ccs/results/optiNetSplitting/base/v10/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v1/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v7/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v9/cluster/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/networkSize/sameChannel/cluster/General-0.vec"
  
  #path <- "General-0.vec"
  #path <- "/home/tobias/Documents/ccs/results/General-0.vec"
  
  ogmReceivedBueren <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedBueren)"))
  
  
  ogmReceivedBueren <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedBueren)"))#vectors$ogmReceivedBueren
  ogmReceivedPaderborn <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedPaderborn)"))#vectors$ogmReceivedPaderborn
  ogmReceivedBaliSchlangenHoevelhof <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedBaliSchlangenHoevelhof)"))#vectors$ogmReceivedPaderborn
  ogmReceivedRemainder <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedRemainder)"))#vectors$ogmReceivedRemainder
  ogmSentBueren <- loadVectorsShaped(path, add(type="vector",select="name(ogmSentBueren)"))#vectors$ogmSentBueren
  ogmSentPaderborn<- loadVectorsShaped(path, add(type="vector",select="name(ogmSentPaderborn)"))#vectors$ogmSentPaderborn
  ogmSentBaliSchlangenHoevelhof <- loadVectorsShaped(path, add(type="vector",select="name(ogmSentBaliSchlangenHoevelhof)"))#vectors$ogmSentBaliSchlangenHoevelhof
  ogmSentRemainder <- loadVectorsShaped(path, add(type="vector",select="name(ogmSentRemainder)"))#vectors$ogmSentRemainder
  
  #Remove data from "Mesh-only" nodes and gateways
  ogmReceivedBueren<-ogmReceivedBueren[!(ogmReceivedBueren$ogmReceivedBueren==0|ogmReceivedBueren$ogmReceivedBueren>10000),]
  ogmReceivedPaderborn<-ogmReceivedPaderborn[!(ogmReceivedPaderborn$ogmReceivedPaderborn<10|ogmReceivedPaderborn$ogmReceivedPaderborn>10000),]
  ogmReceivedBaliSchlangenHoevelhof<-ogmReceivedBaliSchlangenHoevelhof[!(ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof==0|ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof>4000),]
  ogmReceivedRemainder<-ogmReceivedRemainder[!(ogmReceivedRemainder$ogmReceivedRemainder==0|ogmReceivedRemainder$ogmReceivedRemainder>10000),]
  
  ogmSentBueren<-ogmSentBueren[!(ogmSentBueren$ogmSentBueren<50|ogmSentBueren$ogmSentBueren>400),]
  ogmSentPaderborn<-ogmSentPaderborn[!(ogmSentPaderborn$ogmSentPaderborn<50|ogmSentPaderborn$ogmSentPaderborn>1000),]
  ogmSentBaliSchlangenHoevelhof<-ogmSentBaliSchlangenHoevelhof[!(ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof<400|ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof>600),]
  ogmSentRemainder<-ogmSentRemainder[!(ogmSentRemainder$ogmSentRemainder<50|ogmSentRemainder$ogmSentRemainder>1000),]
  #ogmSentBueren <- ogmSentBueren[ogmSentBueren > 100]
  #ogmSentPaderborn <- ogmSentPaderborn[ogmSentPaderborn > 100]
  #ogmSentBaliSchlangenHoevelhof <- ogmSentBaliSchlangenHoevelhof[ogmSentBaliSchlangenHoevelhof > 100]
  #ogmSentRemainder <- ogmSentRemainder[ogmSentRemainder > 100]
  
  #ogmReceivedBueren <- ogmReceivedBueren[ogmReceivedBueren > 0]
  #ogmSentBueren <- ogmSentBueren[ogmSentBueren > 100]
  #ogmSentPaderborn <- ogmSentPaderborn[ogmSentPaderborn > 100]
  #ogmSentBaliSchlangenHoevelhof <- ogmSentBaliSchlangenHoevelhof[ogmSentBaliSchlangenHoevelhof > 100]
  
  
  dReceivedBueren <- data.frame(count=ogmReceivedBueren)
  dRealdReceivedPB<- data.frame(count=ogmReceivedPaderborn)
  dRealdReceivedBaliSchlangenHoevelhof<- data.frame(count=ogmReceivedBaliSchlangenHoevelhof)
  dRealdReceivedRemainder<- data.frame(count=ogmReceivedRemainder)
  ############################## START: RECEIVED ##############################
  #Received ---- Boxplot
  #dReceived <- rbind(data.frame(count=ogmReceivedBueren$ogmReceivedBueren, source="Bueren"),data.frame(count=ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof, source="BaliSchlangenHoevelhof"),data.frame(count=ogmReceivedPaderborn$ogmReceivedPaderborn, source="Paderborn"), data.frame(count=ogmReceivedRemainder$ogmReceivedRemainder, source="Remainder"))
  #pReceived <- ggplot(dReceived) + geom_boxplot(aes(x=source,y=count)) + expand_limits(y=0)+ ggtitle("Received packets MESH-VPN") + xlab("Subnets")+ ylab("Packet count")
  #pdf(file= paste(mainPath,'/improvement/numOGMReceivedBoxPlotCluster.pdf',sep = ""))
    #print(pReceived)
  #dev.off()
  #Received ---- Density
  dReal <- data.frame(count=measuredVPNRealReceived)
  dReceivedBueren <-  rbind(data.frame(count=ogmReceivedBueren$ogmReceivedBueren, source="Bueren"))
  dReceivedPB     <-  rbind(data.frame(count=ogmReceivedPaderborn$ogmReceivedPaderborn, source="Pb"))
  dReceivedBaliSchlangenHoevelhof <-  rbind(data.frame(count=ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof, source="Bali"))
  dReceivedRemainder <-  rbind(data.frame(count=ogmReceivedRemainder$ogmReceivedRemainder, source="Remainder"))
  #p <- ggplot()+geom_density(data=dReceivedBueren,aes(x=count,color="Bueren"))+geom_density(data=dReceivedBaliSchlangenHoevelhof,aes(x=count,color="Bali"))+geom_density(data=dReceivedRemainder,aes(x=count,color="Remainder"))+geom_density(data=dReal,aes(x=count,color="real"))+geom_density(data=dRealdReceivedPB,aes(x=count,color="PB"))+ggtitle("Received OGMs")
  #ggplot(p, aes(x, colour = 'Simulation')) + stat_ecdf()+ ggtitle("Node propagation delay") + xlab("Time (seconds)") + ylab("Nodes reached (%)") 
  
  #old
  #path <- "/home/tobias/Documents/ccs/results/optiNetSplitting/base/v10/noCluster/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v1/noCluster/General-0.sca"
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/newMemory/v5/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/networkSize/sameChannel/noCluster/General-0.sca"
           
  #path <- "/home/tobias/Documents/ccs/results/networkSize/equalDistribution/noCluster/General-0.sca"
  
  
  numOGMReceivedCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  #numOGMReceivedCountWLAN <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOGMReceivedCountWLAN)"))
  #numOwnOGMSentCountWlan <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOwnOGMSentCountWlan)"))
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
  
  df <-  rbind(data.frame(value = ogmReceivedBueren$ogmReceivedBueren, source = 'Bueren (139)'),
               data.frame(value = ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof, source = 'BaliSchlangenHoevelhof (58)'),
               data.frame(value = ogmReceivedPaderborn$ogmReceivedPaderborn, source = 'Paderborn (271)'),
               data.frame(value = ogmReceivedRemainder$ogmReceivedRemainder, source = 'Remainder (318)'),
               data.frame(value = numOGMReceivedCountVpnVectorSubset, source = 'Model(779)'))
              # data.frame(value = measuredVPNRealReceived, source = 'Real(779)'))
  
  pdf(file= paste(mainPath,'/improvement/receivedEcdf.pdf',sep = ""))
  ggplot(df, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Received packets (Mesh-vpn)")+ xlab("#packets") + ylab("Cumulative of nodes") +
    scale_color_manual(breaks=c("Bueren (139)","BaliSchlangenHoevelhof (58)","Paderborn (271)","Remainder (318)","Model(779)"),name="Legend", values=c("#999999","#FF7F00","#A65628","#377EB8","#F781BF"))+
    scale_linetype_manual(breaks=c("Bueren (139)","BaliSchlangenHoevelhof (58)","Paderborn (271)","Remainder (318)","Model(779)"), name="Legend",values=c(5,1,7,4,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) 
  dev.off()
  
  
  df <-  rbind(data.frame(value = numOGMReceivedCountVpnVectorSubset, source = 'Model(779)'),
               data.frame(value = measuredVPNRealReceived, source = 'Real(779)'))
 pdf(file= paste(mainPath,'receivedEcdf.pdf',sep = ""))
  ggplot(df, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Received packets (Mesh-vpn)")+ xlab("#packets") + ylab("Cumulative of nodes") +
    scale_color_manual(breaks=c("Model(779)","Real(779)"),name="Legend", values=c("#377EB8","#FF7F00"))+
    scale_linetype_manual(breaks=c("Model(779)","Real(779)"), name="Legend",values=c(5,7))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) 
 dev.off()
  #pdf(file= paste(mainPath,'/improvement/numOGMReceivedDensity.pdf',sep = ""))
    #print(p)
  #dev.off()
  ############################## END: RECEIVED ##############################
  ############################## START: WIFI / VPN #######################################
  #   wifiVPNRelation <- rbind(data.frame(value = 58, source = 'wifi',value2 = 58),
  #                            data.frame(value = 79, source = 'VPN',value2 = 58)),
  #   data.frame(value = 141, source = '#Bueren'),
  #   data.frame(value = 57, source = 'wifi Paderborn'),
  #   data.frame(value = 213, source = 'VPN Paderborn'),
  #   data.frame(value = 271, source = '#Paderborn'),
  #   data.frame(value = 15, source = 'wifi Lippspringe'),
  #   data.frame(value = 41, source = 'VPN Lippspringe'),
  #   data.frame(value = 56, source = '#Lippspringe'),
  #   data.frame(value = 74, source = 'wifi Remainder'),
  #   data.frame(value = 242, source = 'VPN Remainder'),
  #   data.frame(value = 316, source = '#Remainder'))
  
  wifiVPNRelation <- rbind(data.frame(wifi = 33,vpn = 106, source = 'Bueren',sum=139),
                           data.frame(vpn = 230, source = 'Paderborn',wifi = 36,sum=271),
                           data.frame(vpn = 49, source = 'Lippspringe',wifi = 9,sum=58),
                           data.frame(vpn = 270, source = 'Remainder',wifi = 46,sum=318),
                           data.frame(vpn = 655, source = 'Model',wifi =124 ,sum=779))
  data.m <- melt(wifiVPNRelation, id.vars='source')

pdf(file= paste(mainPath,'improvement/nodeTypes.pdf',sep = ""))
ggplot(data.m, aes(source, value)) + geom_bar(aes(fill = variable),  stat="identity",position = position_dodge())+ xlab("Subnetworks") + ylab("Number of nodes")+
  ggtitle("VPN and 'wifi only' nodes per subnet") +  theme_my(base_family = "Palatino") + scale_fill_manual(values = c("#377EB8","#FF7F00","#A65628"),labels=c("Wifi only","VPN","Sum"))+
  guides(fill=guide_legend(title="Nodetype"))+theme(axis.text.x = element_text(angle = 90, hjust = 1))
dev.off()
  
  
  
  
  
  
  
  ggplot(data.m, aes(x = factor(value), y = value, fill=factor(source),color=factor(source))) + 
    geom_bar(stat = "identity", position=position_dodge()) +
    geom_text(aes(y=value, ymax=value, label=source),position= position_dodge(width=0.9), vjust=-.5)
  
  #+ theme(axis.text.x = element_text(angle = 90, hjust = 1))

  ############################## END: WIFI / VPN #######################################
  
  ############################## START: SENT ##############################
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset > 100]
  dSent <- rbind(data.frame(value=ogmSentBueren$ogmSentBueren, source="Bueren (139)"), 
                 data.frame(value=ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof, source="BaliSchlangenHoevelhof (58)"),
                 data.frame(value=ogmSentPaderborn$ogmSentPaderborn, source="Paderborn (271)"), 
                 data.frame(value=ogmSentRemainder$ogmSentRemainder, source="Remainder (318)"),
                 data.frame(value=numOwnOGMSentCountVpnVectorSubset, source = 'Model(779)'))
                  #,data.frame(value=measuredVPNRealSend, source = 'Real(779)'))
  
  
  
  #pdf(file= paste(mainPath,'/improvement/sendEcdf.pdf',sep = ""))
  ggplot(dSent, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Sent packets (Mesh-vpn)")+ xlab("#packets") + ylab("Cumulative of nodes") +
    scale_color_manual(breaks=c("Bueren (139)","BaliSchlangenHoevelhof (58)","Paderborn (271)","Remainder (318)","Model(779)"),name="Legend", values=c("#999999","#FF7F00","#A65628","#377EB8","#F781BF"))+
    scale_linetype_manual(breaks=c("Bueren (139)","BaliSchlangenHoevelhof (58)","Paderborn (271)","Remainder (318)","Model(779)"), name="Legend",values=c(5,1,7,4,5))+
    theme_my(base_family = "Palatino") + coord_cartesian(xlim=c(0,1200))#+ theme(legend.position = c(0.8, 0.2)) +  theme(legend.background = element_rect(colour = 'black', size = 1, linetype='dashed'))
  #dev.off()
  
  

  dSent <- rbind(data.frame(value=numOwnOGMSentCountVpnVectorSubset, source = 'Model(779)'),
                 data.frame(value=measuredVPNRealSend, source = 'Real(779)'))
  
  
 pdf(file= paste(mainPath,'sendEcdfEqualDistribution.pdf',sep = ""))
  ggplot(dSent, aes(x=value, color=source,linetype=source))  +stat_ecdf()+ ggtitle("Sent packets (Mesh-vpn)")+ xlab("#packets") + ylab("Cumulative of nodes") +
    scale_color_manual(breaks=c("Model(779)","Real(779)"),name="Legend", values=c("#377EB8","#FF7F00"))+
    scale_linetype_manual(breaks=c("Model(779)","Real(779)"), name="Legend",values=c(5,7))+
    theme_my(base_family = "Palatino")+expand_limits(x=0) 
  dev.off()
  
###################################################################################################################
  dReal <- data.frame(count=measuredVPNRealSend)
  dSentBueren <-  rbind(data.frame(count=ogmSentBueren$ogmSentBueren, source="Bueren (139)"))
  dSentPB     <-  rbind(data.frame(count=ogmSentPaderborn$ogmSentPaderborn, source="Paderborn (271)"))
  dSentBaliSchlangenHoevelhof <-  rbind(data.frame(count=ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof, source="BaliSchlangenHoevelhof (58)"))
  dSentRemainder <-  rbind(data.frame(count=ogmSentRemainder$ogmSentRemainder, source="Remainder (318)"))
  p <- ggplot()+geom_density(data=dReal,aes(x=count,color="real")) +geom_density(data=dSentRemainder,aes(x=count,color="Remainder"))+geom_density(data=dSentPB,aes(x=count,color="PB"))+geom_density(data=dSentBueren,aes(x=count,color="Bueren"))+geom_density(data=dSentBaliSchlangenHoevelhof,aes(x=count,color="Bali"))
  #pdf(file= paste(mainPath,'/improvement/numOGMSentDensity.pdf',sep = ""))
    print(p)
  #dev.off()
  ############################## SENT ##############################

}

analyzeOGMcount <- function()
{
  #/home/tobias/Documents/ccs/results/run1/results/
  path <- "/home/tobias/Documents/ccs/results/optiNetSplitting/base/v10/noCluster/General-0.sca"
 
  numOGMReceivedCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  #numOGMReceivedCountWLAN <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOGMReceivedCountWLAN)"))
  #numOwnOGMSentCountWlan <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOwnOGMSentCountWlan)"))
  numOwnOGMSentCountVpn <-loadDataset(path, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  
  numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[8:582]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[8:582]
  overallVPN <- c(numOGMReceivedCountVpnVectorSubset, numOwnOGMSentCountVpnVectorSubset) 
  d <- data.frame(count=numOGMReceivedCountVpnVectorSubset)
  measuredVPNRealAll <- c(4860,4323,6431)
  measuredVPNRealSend <- c(684,842,756)
  measuredVPNRealReceived <- c(4176,3481,5675)
  dReal <- data.frame(count=measuredVPNRealReceived) 
  
  #numOwnOGMReceivedCountVpn
  p <- ggplot()+geom_density(data=d,aes(x=count,color="Sim"))+geom_density(data=dReal,aes(x=count,color="real"))+  ggtitle("Received packets MESH-VPN")
  #pdf(file= paste(mainPath,'numOGMReceivedDensity.pdf',sep = ""))
    print(p)
  #dev.off()
  dSent <- rbind(data.frame(value=measuredVPNRealSend, source="Real (779)"),data.frame(value=numOwnOGMSentCountVpnVectorSubset, source="Sim (779)"))
  ggplot(dSent, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Sent packets (Mesh-vpn)") + coord_cartesian(xlim=c(0,2000))
  
  
  d <- rbind(data.frame(count=numOGMReceivedCountVpnVectorSubset, source="sim"), 
             data.frame(count=measuredVPNRealReceive, source="real"))
  
  dReceived <- rbind(data.frame(value=measuredVPNRealReceived, source="Real (779)"),data.frame(value=numOGMReceivedCountVpnVectorSubset, source="Sim (779)"))
  ggplot(dReceived, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Received packets (Mesh-vpn)") #+ coord_cartesian(xlim=c(0,2000))
  
  pReceived <-  ggplot(d)+geom_boxplot(aes(x=source,y=count))+  expand_limits(y=0)+ ggtitle("Received packets MESH-VPN")
  #pdf(file= paste(mainPath,'numOGMReceivedBoxPlot.pdf',sep = ""))
    print(pReceived)
  #dev.off()
  
  #numOwnOGMSentCountVpn
  d <- rbind(data.frame(count=numOwnOGMSentCountVpnVectorSubset, source="sim"), data.frame(count=measuredVPNRealSend, source="real"))
  
  pSent <- ggplot(d) + geom_density(aes(colour=source,x=count)) + expand_limits(x=0)+ ggtitle("Sent packets MESH-VPN")+xlab("Number of packets") + scale_shape_discrete(name  ="Payer", breaks=c("Female", "Male"),labels=c("Woman", "Man"))
  
  #pdf(file= paste(mainPath,'numOGMSentBoxPlot.pdf',sep = ""))
  print(pSent)
  #dev.off()
  
  
  dReceived <- rbind(data.frame(value=measuredVPNRealSend, source="Real (779)"),data.frame(value=numOGMReceivedCountVpnVectorSubset, source="Sim (779)"))
  ggplot(dReceived, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ ggtitle("Received packets (Mesh-vpn)") #+ coord_cartesian(xlim=c(0,2000))
  
  
  #pSent <- ggplot(d) + geom_boxplot(aes(x=source,y=count)) + expand_limits(y=0)+ ggtitle("Sent packets MESH-VPN")
  #pdf(file= paste(mainPath,'numOGMSentDensity.pdf',sep = ""))
  #  print(pSent)
  #dev.off()

}
#+ xlab("Time (seconds)") + ylab("My y label") 

analyseContentAggOGM <- function()
{
  numAggOGM <- loadDataset("/home/tobias/Documents/ccs/results/optiNetSplitting/base/v10/cluster/General-0.vec", add(type="vector",select="name(ogmAggregated)"))
  
  numAggOGMVectors <- loadVectors(numAggOGM,NULL)
  numAggOGMVectorsSplit <- split(numAggOGMVectors$vectordata, numAggOGMVectors$vectordata$resultkey) 
  meanVector <- getAvgOfVectorData(numAggOGMVectorsSplit)
 
  numIterations <-max(numAggOGMVectors$vectordata$resultkey)
  numIterations <- numIterations+1
  x <- 1:numIterations
  for(i in x){
    meanVector[i] <-numAggOGMVectorsSplit[[i]]$y
  }  
  boxplot(meanVector)
}


delayLocalClientPropagation <- function(){
  clientCompleteReachedAllNodes <- loadDataset("General-0.sca", add(type="scalar",select="name(#clientCompleteReachedAllNodes)"))
  clientCompleteReachedAllNodesVector <-clientCompleteReachedAllNodes$scalars$value
 # boxplot(clientCompleteReachedAllNodesVector, main="Delay local client announcement (800 nodes)",  ylab="Time (seconds)")
  
  d <- rbind(data.frame(count=clientCompleteReachedAllNodesVector, source="sim"))
  pSent <- ggplot(d) + geom_density(aes(colour=source,x=count)) + expand_limits(x=0)+ ggtitle("Non-mesh client propagation delay")
  
  print(pSent)
  plot(ecdf(clientCompleteReachedAllNodesVector), verticals=TRUE, do.points=FALSE, main=NA)
  
  df <- data.frame(x = clientCompleteReachedAllNodesVector)
 #pdf(file= paste(mainPath,'delayGlobalClientAnnouncement.pdf',sep = ""))
  ggplot(df, aes(x, colour = 'Simulation')) + stat_ecdf()+ ggtitle("Client propagation delay") + xlab("Time (seconds)") + ylab("My y label") 
 #dev.off()
}
analyzeNewNodePropagation <- function(){
  
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.sca"
  nodeReached <- loadDataset(path, add(type="scalar",select="name(#allNodesKnown)"))
  #nodeReached <- loadDataset("/home/tobias/Documents/ccs/results/General-0.sca", add(type="scalar",select="name(#allNodesKnown)"))
  nodeReachedVector <-nodeReached$scalars$value
  print(nodeReachedVector)
  a <- table(nodeReachedVector)

  #ECDF Plot
  df <- data.frame(x = nodeReachedVector)
  #pdf(file= paste(mainPath,'delayGlobalNodeAnnouncement.pdf',sep = ""))
  ggplot(df, aes(x, colour = 'Simulation')) + stat_ecdf()+ ggtitle("Node propagation delay") + xlab("Time (seconds)") + ylab("Nodes reached (%)") 
  #dev.off()
}



analyseNodeCount <- function(){
  known_originator <-loadDataset("General-0.vec", add("vector",select="name(known_originator)"))
    
  origVectors <- loadVectors(known_originator,NULL)
  origVectorSplit <- split(origVectors$vectordata, origVectors$vectordata$resultkey) 
  vectorNodeCount <- c() #A vector which hold all values for the nodecount of all nodes
  vectorNodeCountTen <- c()
  avgVector <- c()
  numIterations <-max(origVectors$vectordata$resultkey)
  numIterations <- numIterations+1
  x <- 1:numIterations
  for(i in x){
    origVectorx <- origVectorSplit[[i]]$x
    origVectory <- origVectorSplit[[i]]$y
    if(length(avgVector)==0)
      avgVector <- origVectory
    #print(avgVector[i])
    #print(origVectory[i])
    avgVector <- avgVector + origVectory
    #print(length(origVectorSplit[[i]]$y))
    maxNode<- max(origVectory)
    append(vectorNodeCount,maxNode)
    vectorNodeCount <- c(vectorNodeCount,maxNode) 
    if(i < 10)
    {
      plot(origVectorx,origVectory,xlab = "Time (seconds)",ylab="# Nodes",type="l",main=paste("Nodecount for node #",origVectors$vectors$module[[i]][1]))
      text(paste("Maximum = ",maxNode),adj=c(1,0),col="red", simTime, y=max(origVectory)+0.05)
    }
  }
  boxplot(vectorNodeCount)
  
}
analyzeScalars <- function(){
  numOGMReceivedCountVpn <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOGMReceivedCountWLAN <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOGMReceivedCountWLAN)"))
  numOwnOGMSentCountWlan <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOwnOGMSentCountWlan)"))
  numOwnOGMSentCountVpn <-loadDataset("General-0.sca", add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  
  #numOGMReceivedCountWLANVector <-numOGMReceivedCountWLAN$scalars$value
  #numOGMReceivedCountWLANVector <- numOGMReceivedCountWLANVector[numOGMReceivedCountWLANVector !=max(numOGMReceivedCountWLANVector)]
  
  numOwnOGMSentCountWlanVector <-numOwnOGMSentCountWlan$scalars$value
  numOwnOGMSentCountWlanVector <- numOwnOGMSentCountWlanVector[numOwnOGMSentCountWlanVector !=min(numOwnOGMSentCountWlanVector)]
  
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  
  
  #numOGMReceivedCountWLAN <- numOGMReceivedCountWLAN[numOGMReceivedCountWLAN != "0"]
  #numOwnOGMSentCountWlan <- numOwnOGMSentCountWlan[numOwnOGMSentCountWlan != "0"]
  #numOwnOGMSentCountVpn <- numOwnOGMSentCountVpn[numOwnOGMSentCountVpn != "0"]
  
  boxplot(numOGMReceivedCountVpnVector,main="numOGMReceivedCountVpn")
  # boxplot(numOGMReceivedCountWLAN$scalars$value,main="numOGMReceivedCountWLAN")
  boxplot(numOwnOGMSentCountWlanVector,main="numOwnOGMSentCountWlan")
  boxplot(numOwnOGMSentCountVpnVector,main="numOwnOGMSentCountVpn")
}


analyseOwnOgmSend <- function(){
  own_ogm_send <-loadDataset("General-0.vec", add("vector",select="name(own_ogm_send)"))
  own_ogm_sendVPN <-loadDataset("General-0.vec", add("vector",select="name(own_ogm_send_vpn)"))
  own_ogm_sendWLAN <-loadDataset("General-0.vec", add("vector",select="name(own_ogm_send_wlan)"))
  
  received_ogm <-loadDataset("General-0.vec", add("vector",select="name(received_ogm)"))
  received_ogm_vpn <-loadDataset("General-0.vec", add("vector",select="name(received_ogm_vpn)"))
  received_ogm_wlan <-loadDataset("General-0.vec", add("vector",select="name(received_ogm_wlan)"))
  
  own_ogm_sendVectorsSplit <- splitvector(own_ogm_send)
  received_ogm_VectorsSplit <- splitvector(received_ogm)
  received_ogm_vpnSplit <- splitvector(received_ogm_vpn)
  received_ogm_wlanSplit <- splitvector(received_ogm_wlan)
  own_ogm_sendVPNSplit <- splitvector(own_ogm_sendVPN)
  own_ogm_sendWLANSplit <- splitvector(own_ogm_sendWLAN)
  numIterations <- max(own_ogm_send$vectors$resultkey) #max(own_ogm_sendVectors$vectordata$resultkey)
  
  numIterations <- numIterations+1
  x <- 1:numIterations#numIterations
  for(i in x){
    own_ogm_sendVectorsx <- own_ogm_sendVectorsSplit[[i]]$x
    own_ogm_sendVectorsy <- own_ogm_sendVectorsSplit[[i]]$y
    received_ogmVectorsx <-received_ogm_VectorsSplit[[i]]$x
    received_ogm_Vectorsy <- received_ogm_VectorsSplit[[i]]$y
    
    received_ogmVectorsVPNx <-received_ogm_vpnSplit[[i]]$x
    received_ogm_VectorsVPNy <- received_ogm_vpnSplit[[i]]$y
    received_ogmVectorsWLANx <-received_ogm_wlanSplit[[i]]$x
    received_ogm_VectorsWLANy <- received_ogm_wlanSplit[[i]]$y
    
    #maxOGM<- max(own_ogm_sendVectorsy)
    plot(own_ogm_sendVectorsx,own_ogm_sendVectorsy,,xlab = "Time (seconds)",ylab="# OGMs",type="l",
         main=paste("OGM count for node #",own_ogm_send$vectors$module[[i]][1]))
    #text(paste("Maximum = ",maxOGM),adj=c(1,0),col="red", simTime, y=max(own_ogm_sendVectorsy)+0.05)
    
    lines(own_ogm_sendVectorsx,own_ogm_sendVectorsy,col="green")
    lines(received_ogmVectorsx,received_ogm_Vectorsy,col="red")
    
    
    lines(received_ogmVectorsVPNx,received_ogm_VectorsVPNy,col="black")
    lines(received_ogmVectorsWLANx,received_ogm_VectorsWLANy,col="blue")
    legend('bottomright', c("Sent OGMs","Received OGMs","Received OGMs VPN","Received OGMs WLAN"),col=c("green","red","black","blue"),pch=c("-","-","-","-"))
  }
}
splitvector <- function(recVector){
  loadedVector <- loadVectors(recVector,NULL)
  loadedVectorSplit <-  split(loadedVector$vectordata, loadedVector$vectordata$resultkey) 
}

createRealGwStats <- function(){
  gwStats<-c(263,201,143,130,19,8,5,3)
  
  gwNames <- c("Gateway 1","Gateway 2","Gateway 3","Gateway 4","Gateway 5","Gateway 6","Gateway 7","Gateway 8")
  gwNames <- c("GW 1","GW 2","GW 3","GW 4","GW 5","GW 6","GW 7","GW 8")
  data <- factor(gwStats,gwNames)
  
 # pdf(file= paste(mainPath,'realGatewayDistribution.pdf',sep = ""))
  ggplot(data.frame(data),aes(x=factor(gwNames),y=gwStats/sum(gwStats)))+geom_bar(stat="identity") + ylab("Percentage of connected nodes") +xlab("")+
  scale_y_continuous(labels = percent_format())+
    geom_text(aes(label = paste(round(gwStats/sum(gwStats)*100),"%"), y = gwStats/sum(gwStats)+0.015, size = 3))+
    theme_my(base_family = "Palatino")  + theme(legend.position="none")
  #dev.off()
 
  
  gwStats<-c(245,124,181,140)
  gwNames <- c("Gateway 1","Gateway 2","Gateway 3","Gateway 4")
  data <- factor(gwStats,gwNames)
  pdf(file= paste(mainPath,'realGatewayDistributionSimulation.pdf',sep = ""))
  ggplot(data.frame(data),aes(x=factor(gwNames),y=gwStats/sum(gwStats)))+geom_bar(stat="identity") + ylab("Percentage of connected nodes") +xlab("")+
    scale_y_continuous(labels = percent_format())+
    geom_text(aes(label = paste(round(gwStats/sum(gwStats)*100),"%"), y = gwStats/sum(gwStats)+0.015, size = 3))+
    theme_my(base_family = "Palatino")  + theme(legend.position="none")
  dev.off()
  
}
createGwStats <- function(){
  gwStats = loadDataset("General-0.sca", add('scalar',select='module(ffpb_network.gw0.relayUnit)'))
   gwNames <- c("Gateway 1","Gateway 2","Gateway 3","Gateway 4","Gateway 5","Gateway 6","Gateway 7","Gateway 8")
   scalarValues <- gwStats$scalars$value
   gwValues <-  scalarValues[1:8]
   
   #pdf(file= paste(mainPath,'gatewayDistribution.pdf',sep = ""))
   par(mar=c(5,10,4,4)+.1)
   par(las=2)
  c <- 
   text(x= gwStats$scalars$value+3, y= barplot(gwValues, main="Gateway distribution",horiz=TRUE, xlab="Number of nodes",
                                                  names.arg= gwNames, cex.names=0.8, beside=TRUE,legend = rownames(gwValues)),
        labels=as.character(gwStats$scalars$value), xpd=TRUE)
  
  
   #dev.off()
   
   
   gwOrigHist  = loadVectorsShaped("General-0.vec", add('vector',select='module(ffpb_network.gw0.relayUnit)'))
  
}
deviceDistribution <- function()
{
  deviceStats = c(145,110,14,12,4,586,3,59)
  deviceStatsNames = c("TL-WR841N/ND v8","TL-WR1043N/ND v1","TL-WR1043N/ND v2","TL-WDR4300 v1","Nanostation M","TL-WR841N/ND v9","UniFiAP Outdoor","other")
  
 
  par(mar=c(5,10,4,4)+.1)
  par(las=2)

  barplot(deviceStats)
  theTable <- within(data,deviceStats <- factor(deviceStats, levels=names(sort(table(deviceStats), decreasing=TRUE))))
  
  
  d <- data.frame(deviceStats=c(586,145,110,14,12,4,3,59), names=c("TL-WR841N/ND v9","TL-WR841N/ND v8","TL-WR1043N/ND v1","TL-WR1043N/ND v2","TL-WDR4300 v1","Nanostation M","UniFiAP Outdoor","Other"))
  d$devicestats2 <- factor(d$names,as.character(d$names))
  p <- ggplot(d, aes(y=deviceStats))  
  pdf(file= paste(mainPath,'deviceDistribution.pdf',sep = ""))
  p + geom_bar(aes(x=devicestats2), data=d, stat="identity")+ ylab("# Nodes") +xlab("")+ ggtitle("Device distribution")+
    theme_my(base_family = "Palatino")  + theme(legend.position="none")+theme(axis.text.x = element_text(angle = 90, hjust = 1))+coord_flip() +
    geom_text(aes(label = deviceStats, y = deviceStats+30,x=names))
    scale_y_continuous(labels =deviceStats) 
  dev.off()
}

analyseLocalClientCount <- function()
{
  clientCount <- 0
  known_local_clients <-loadDataset("General-0.vec", add("vector",select="name(known_local_clients)"))
  known_local_clients_Vectors <- loadVectors(known_local_clients,NULL)
  known_local_clients_VectorsSplit <- split(known_local_clients_Vectors$vectordata, known_local_clients_Vectors$vectordata$resultkey) 
  numIterations <- max(known_local_clients_Vectors$vectordata$resultkey)
  numIterations <- numIterations+1
  increased <- 0
  maxNode <- 0
  x <- 1:numIterations
  for(i in x){
    known_local_clientsx <- known_local_clients_VectorsSplit[[i]]$x
    known_local_clientsy <- known_local_clients_VectorsSplit[[i]]$y
    
    #maxNode <- if(known_local_clientsy==1) known_local_clientsy else 0.0
    nrItems <- 1:length(known_local_clientsy)
    for(k in nrItems)
    {
      if(known_local_clientsy[[k]]==1) {
        if(increased == 0)
        {
          maxNode <- known_local_clientsx[[k]]
          break;
        }
      }
    }  
    plot(known_local_clientsx,known_local_clientsy,xlab = "Time (seconds)",ylab="# Clients",type="l",main=paste("Local client count for node #",known_local_clients_Vectors$vectors$module[[i]][1]))#,known_local_clients_Vectors$vectors$module[[i]][1])
    abline(v=maxNode,col="red")
    text(maxNode,adj=c(0,0),col="red", x =maxNode, y=0)
    maxNode <- 0
  }
}

analyseGlobalClientCount <- function()
{
  known_global_clients <-loadDataset("General-0.vec", add("vector",select="name(known_global_clients)"))
  known_global_clients_Vectors <- loadVectors(known_global_clients,NULL)
  known_global_clients_VectorsSplit <- split(known_global_clients_Vectors$vectordata, known_global_clients_Vectors$vectordata$resultkey) 
  numIterations <-max(known_global_clients_Vectors$vectordata$resultkey)
  numIterations <- numIterations+1
  increased <- 0
  maxNode <- 0
  x <- 1:numIterations
  for(i in x){
    known_global_clientsx <- known_global_clients_VectorsSplit[[i]]$x
    known_global_clientsy <- known_global_clients_VectorsSplit[[i]]$y
    nrItems <- 1:length(known_global_clientsy)
    for(k in nrItems)
    {
      if(known_global_clientsy[[k]]==1) {
        if(increased == 0)
        {
          maxNode <- known_global_clientsx[[k]]
          break;
        }
      }
    }  
    plot(known_global_clientsx,known_global_clientsy,xlab = "Time (seconds)",ylab="# Nodes",type="l",main=paste("Global client count for node #",known_global_clients_Vectors$vectors$module[[i]][1]))#,known_global_clients_Vectors$vectors$module[[i]][1])
    abline(v=maxNode,col="red")
    text(maxNode,adj=c(0,0),col="red", x =maxNode, y=0)
  }
}


makePlot <- function(ggplot)
{
  ggplot<-ggplot+theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    theme(legend.position = c(0.15, 0.22))+expand_limits(x=0)
  return (ggplot)
}

saveplot <- function(filename, width, height,dpi,myPlot){  
  ggsave(filename=paste(mainPath,filename, sep = ""), width = width, height = height, dpi = dpi, plot=myPlot)
}

saveplotImprovement <- function(filename, width, height,dpi,myPlot){  
  ggsave(filename=paste(mainPathImprovement,filename, sep = ""), width = width, height = height, dpi = dpi, plot=myPlot)
}


theme_my <- function(base_size = 13, base_family = "Palatino")
{
  txt <- element_text(size = 13, colour = "black", face = "plain")
  bold_txt <- txt
  bold_txt[2] <- "bold"
  
  theme_bw(base_size = base_size, base_family = base_family) +
    theme(
      legend.key = element_blank(),
      strip.background = element_blank(),
      
      text = txt,
      plot.title = txt,
      
      axis.title = txt,
      axis.text = txt,
      
      legend.title = bold_txt,
      legend.text = txt )
}

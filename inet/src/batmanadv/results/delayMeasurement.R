######THIS!
analyzeNewNodePropagation <- function(path){
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/nodeAnnouncement/General-0.sca"
    path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/nodeAnnouncement/tmp/General-0.sca"
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/nodeAnnouncement/rlab3Test/General-0.sca"
  nodeReached <- loadDataset(path, add(type="scalar",select="name(#allNodesKnown)"))
  #nodeReached <- loadDataset("/home/tobias/Documents/ccs/results/General-0.sca", add(type="scalar",select="name(#allNodesKnown)"))
  nodeReachedVector <-nodeReached$scalars$value
  print(nodeReachedVector)
  
  #ECDF Plot
  df <- data.frame(x = nodeReachedVector)
  myPlot<-ggplot(df, aes(x, colour = 'Simulation (779 Nodes)')) + stat_ecdf()+  xlab("Time (seconds)") + ylab("eCDF") +
    scale_color_manual(breaks=c("Simulation (779 Nodes)"), values=c("#373737"))+#coord_cartesian(xlim=c(50,59))+
    theme_my(base_family = "Palatino") + labs(color = "Legend")
  myPlot<-makePlot(myPlot)
  myPlot <- myPlot+ theme(legend.position = c(0.2, 0.22))
  print(myPlot)
  #ggsave(filename=paste(mainPath,"delayGlobalNodeAnnouncement.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)

}
######THIS!
analyzeNewClientPropagation <- function(path){
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/localClientDelay/validationNew/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/clientAnnouncement/General-0.sca"
  nodeReached <- loadDataset(path, add(type="scalar",select="name(#clientCompleteReachedAllNodes)"))
  #nodeReached <- loadDataset("/home/tobias/Documents/ccs/results/General-0.sca", add(type="scalar",select="name(#allNodesKnown)"))
  nodeReachedVector <-nodeReached$scalars$value
  df <- data.frame(x = nodeReachedVector)
  #pdf(file= paste(mainPath,'delayGlobalNodeAnnouncement.pdf',sep = ""))
  myPlot<-ggplot(df, aes(x, colour = 'Simulation')) + stat_ecdf()+ xlab("Time (seconds)") + ylab("eCDF") +
    scale_color_manual(breaks=c("Simulation"), values=c("#373737"))+
    theme_my(base_family = "Palatino") + labs(color = "Legend")+expand_limits(x=0) 
  myPlot<-makePlot(myPlot)
  print(myPlot)
  ggsave(filename=paste(mainPath,"delayGlobalClientAnnouncement.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}
  

######THIS!
analyzeNewNodePropagationCluster <- function(path){
  path <- "/home/tobias/Documents/ccs/results/localClientDelay/validationNew/cluster/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/nodeAnnouncement/General-2.sca"
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/nodeAnnouncement/tmp/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/nodeAnnouncement/tmp2/General-0.sca"
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/nodeAnnouncement/rlab3Test/General-0.sca"
  pathModel <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/nodeAnnouncement/rlab3Test/General-0.sca"
  nodeReached <- loadDataset(pathModel, add(type="scalar",select="name(#allNodesKnown)"))
  #nodeReached <- loadDataset("/home/tobias/Documents/ccs/results/General-0.sca", add(type="scalar",select="name(#allNodesKnown)"))
  nodeReachedVector <-nodeReached$scalars$value
  
  nodeReachedBueren <- loadDataset(path, add(type="scalar",select="name(#allNodesKnownBueren)"))
  nodeReachedBuerenVector <-nodeReachedBueren$scalars$value
  print(nodeReachedBuerenVector)
  
  nodeReachedPb <- loadDataset(path, add(type="scalar",select="name(#allNodesKnownPb)"))
  nodeReachedPbVector <-nodeReachedPb$scalars$value
  nodeReachedBali <- loadDataset(path, add(type="scalar",select="name(#allNodesKnownHoevelhofBaliSchlangenMeshID)"))
  nodeReachedBaliVector <-nodeReachedBali$scalars$value
  nodeReachedRemainder <- loadDataset(path, add(type="scalar",select="name(#allNodesKnownRemainder)"))
  nodeReachedRemainderVector <-nodeReachedRemainder$scalars$value
  print(nodeReachedRemainderVector)
  print(nodeReachedBaliVector)
  print(nodeReachedPbVector)
  print(nodeReachedBuerenVector)
  #path <- "/home/tobias/Documents/ccs/results/localClientDelay/validationNew/General-0.sca"
  #nodeReached <- loadDataset(path, add(type="scalar",select="name(#allNodesKnown)"))
  ##nodeReached <- loadDataset("/home/tobias/Documents/ccs/results/General-0.sca", add(type="scalar",select="name(#allNodesKnown)"))
  #nodeReachedVector <-nodeReached$scalars$value
  
  
  df <- rbind(data.frame(value=nodeReachedBuerenVector, source="Büren (139 Nodes)"), 
                 data.frame(value=nodeReachedBaliVector, source="Schlangen (58 Nodes)"),
                 data.frame(value=nodeReachedPbVector, source="Paderborn (271 Nodes)"),
                 data.frame(value=nodeReachedRemainderVector, source="Remainder (318 Nodes)"),
                 data.frame(value=nodeReachedVector, source="No improvement (779 Nodes)"))
  
  myPlot<-ggplot(df, aes(value, colour = source,linetype = source,size=source)) + stat_ecdf()+  xlab("Time (seconds)") + ylab("eCDF") +
    theme_my(base_family = "Palatino") + labs(color = "Legend")+
    scale_size_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c(1,1,1,1,2))+
    #scale_color_manual(breaks=c("Bueren (139)","Schlangen (58)","Paderborn (271)","Remainder (318)"),name="Legend", values=c("#F781BF","#FF7F00","#A65628","#377EB8"))+
    scale_color_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E"))+
    scale_linetype_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(1,1,1,1,1))+
    theme_my(base_family = "Palatino") +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    expand_limits(x=0)#+coord_cartesian(xlim=c(0,80)) #
  myPlot<-makePlot(myPlot)
  myPlot <- myPlot+theme(legend.position = c(0.2, 0.2))
  print(myPlot)
  
  ggsave(filename=paste(mainPathImprovement,"delayGlobalNodeAnnouncement.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)

}
######THIS!
analyzeNewClientPropagationCluster <- function(path){
  path <- "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/localClientDelay/validationNew/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/clientAnnouncement/General-0.sca"
  
  pathModel <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/clientAnnouncement/General-0.sca"
  nodeReached <- loadDataset(path, add(type="scalar",select="name(#clientCompleteReachedAllNodes)"))
  #nodeReached <- loadDataset("/home/tobias/Documents/ccs/results/General-0.sca", add(type="scalar",select="name(#allNodesKnown)"))
  nodeReachedVector <-nodeReached$scalars$value  
  
  
  nodeReachedBueren <- loadDataset(path, add(type="scalar",select="name(#clientCompleteReachedAllNodesBueren)"))
  nodeReachedBuerenVector <-nodeReachedBueren$scalars$value
  
  nodeReachedPb <- loadDataset(path, add(type="scalar",select="name(#clientCompleteReachedAllNodesPb)"))
  nodeReachedPbVector <-nodeReachedPb$scalars$value
  
  nodeReachedBali <- loadDataset(path, add(type="scalar",select="name(#clientCompleteReachedAllNodesBali)"))
  nodeReachedBaliVector <-nodeReachedBali$scalars$value
  
  nodeReachedRemainder <- loadDataset(path, add(type="scalar",select="name(#clientCompleteReachedAllNodesRemainder)"))
  nodeReachedRemainderVector <-nodeReachedRemainder$scalars$value
  
  
  df <- rbind(data.frame(value=nodeReachedBuerenVector, source="Büren (139 Nodes)"), 
    data.frame(value=nodeReachedBaliVector, source="Schlangen (58 Nodes)"),
    data.frame(value=nodeReachedPbVector, source="Paderborn (271 Nodes)"), 
    data.frame(value=nodeReachedRemainderVector, source="Remainder (318 Nodes)"),
    data.frame(value=nodeReachedVector, source="No improvement (779 Nodes)"))
  
  myPlot<-ggplot(df, aes(value, colour = source,linetype = source,size=source)) + stat_ecdf()+  xlab("Time (seconds)") + ylab("eCDF") +
    theme_my(base_family = "Palatino") + labs(color = "Legend")+
      scale_size_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c(1,1,1,1,2))+
    scale_color_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E"))+
    #scale_color_manual(breaks=c("Bueren (139)","Schlangen (58)","Paderborn (271)","Remainder (318)"),name="Legend", values=c("#E41A1C","#4DAF4A","#A65628","#377EB8"))+
    scale_linetype_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(1,1,1,1,1))+
    theme_my(base_family = "Palatino") +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+expand_limits(x=0)
   
  myPlot<-makePlot(myPlot)
  myPlot <- myPlot +theme(legend.position = c(0.2, 0.2))
  print(myPlot)
  ggsave(filename=paste(mainPathImprovement,"delayGlobalClientAnnouncement.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}

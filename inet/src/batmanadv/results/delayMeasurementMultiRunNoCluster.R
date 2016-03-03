
###############THIS###############
analyzeNewClientPropagationMultiRun <- function(path){
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/clientAnnouncement/General-"
  
  nodeReached <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodes)"))
  nodeReachedVectorOld <-nodeReached$scalars$value
  
  nodeReached <- loadDataset(paste(paste(path,1, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodes)"))
  nodeReachedVector <-nodeReached$scalars$value
  
  c <- colMeans(rbind(nodeReachedVectorOld))
  a<- data.table(nodeReachedVectorOld[1:776], nodeReachedVector[1:776])
  a<- data.table(a, nodeReachedVector[1:776])
  
  x <- c(2,3,4,5,6,7,8,9)
  for(i in x)
  {
    nodeReached <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodes)"))
    nodeReachedVector <-nodeReached$scalars$value  
    a<- data.table(a, nodeReachedVector[1:776])  
  }
  mean <- rowMeans(a,na.rm = TRUE)
  sdm <- apply(a,1,sd)
  pff <- rowSums(a)/ncol(a)
  error <- qnorm(0.975)*sdm/sqrt(length(sdm))
  errorLower <- pff - sdm
  errorUpper <- pff + sdm
  df <- rbind(data.frame(value=pff, Legend = 'Simulation - mean (779 Nodes)'),
                 data.frame(value=errorLower, Legend = 'Variance'),
                 data.frame(value=errorUpper, Legend = 'Variance2'))
  
 myPlot<- ggplot(df, aes(x=value, color=factor(Legend,labels=c("Simulation - mean (779 Nodes)","Variance","")),linetype=Legend,size=Legend))+stat_ecdf()+
    xlab("Time (seconds)") + ylab("eCDF") + 
    scale_size_manual(breaks=c("Simulation - mean (779 Nodes)","Variance","Variance"), values=c(1,1,1))+
    scale_color_manual(breaks=c("Simulation - mean (779 Nodes)","Variance","Variance"), values=c("#373737","#CDCDCD","#CDCDCD"))+
    scale_linetype_manual(breaks=c("Simulation - mean (779 Nodes)","Variance","Variance"),values=c(7,5,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")
    myPlot<-makePlot(myPlot)
 myPlot <- myPlot+    theme(legend.position = c(0.22, 0.22))
 print(myPlot)
 ggsave(filename=paste(mainPath,"delayGlobalClientAnnouncementMultiRun.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}
###############THIS###############
analyzeNewNodePropagationMultiRun <- function(path){
  path <- "/home/tobias/Documents/ccs/results/multiRun/noCluster/nodeAnnouncement/General-"
  
  nodeReached <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnown)"))
  #nodeReached <- loadDataset("/home/tobias/Documents/ccs/results/General-0.sca", add(type="scalar",select="name(#allNodesKnown)"))
  nodeReachedVector <-nodeReached$scalars$value
  print(nodeReachedVector)
  
  a<- data.table(nodeReachedVector[1:776])
  
  x <- c(2,3,4,5,6,7,8,9)
  for(i in x)
  {
    nodeReached <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnown)"))
    nodeReachedVector <-nodeReached$scalars$value  
    a<- data.table(a, nodeReachedVector[1:776])  
  }
  
  m <- rowMeans(a,na.rm = TRUE)
  sdm <- apply(a,1,sd,na.rm=TRUE)
  pff <- rowSums(a)/ncol(a)
  error <- qnorm(0.975)*sdm/sqrt(length(sdm))
  errorLower <- m - sdm # maybe pff?
  errorUpper <- m + sdm # maybe pff?
  
  df <- rbind(#data.frame(value=m, Legend = 'Simulation'),
              data.frame(value=pff, Legend = 'Simulation - mean (779 Nodes)'), 
              data.frame(value=errorLower, Legend = 'Variance'),
              data.frame(value=errorUpper, Legend = 'Variance2'))
  
  
  myPlot<- ggplot(df, aes(x=value, color=factor(Legend,labels=c("Simulation - mean (779 Nodes)","Variance","")),linetype=Legend,size=Legend))+stat_ecdf()+
    xlab("Time (seconds)") + ylab("eCDF") + 
    scale_size_manual(breaks=c("Simulation - mean (779 Nodes)","Variance","Variance"), values=c(1,1,1))+
   scale_color_manual(breaks=c("Simulation - mean (779 Nodes)","Variance","Variance"), values=c("#373737","#CDCDCD","#CDCDCD"))+
    scale_linetype_manual(breaks=c("Simulation - mean (779 Nodes)","Variance","Variance"),values=c(7,5,5))+
    theme_my(base_family = "Palatino")  + labs(color = "Legend")+expand_limits(x=0)#+coord_cartesian(xlim=c(53,59)) #
  
  
  myPlot<-makePlot(myPlot)
  
  myPlot<-myPlot+ 
    theme(legend.position = c(0.25, 0.22))
  
  print(myPlot) 
  
  ggsave(filename=paste(mainPath,"delayGlobalNodeAnnouncementMultiRun.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
  
}
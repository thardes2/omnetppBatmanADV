analyzeNewClientPropagationMultiRunCluster <- function(path){
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/clientAnnouncement/General-"
  
  
  nodeReachedBueren <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodesBueren)"))
  nodeReachedBuerenVector <-nodeReachedBueren$scalars$value
  nodeReachedPb <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodesPb)"))
  nodeReachedPbVector <-nodeReachedPb$scalars$value
  nodeReachedBali <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodesBali)"))
  nodeReachedBaliVector <-nodeReachedBali$scalars$value
  nodeReachedRemainder <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodesRemainder)"))
  nodeReachedRemainderVector <-nodeReachedRemainder$scalars$value

  
  
  aBueren<- data.table(nodeReachedBuerenVector)
  aPb<- data.table(nodeReachedPbVector)
  aSchlangen<- data.table(nodeReachedBaliVector)
  aRemainder<- data.table(nodeReachedRemainderVector)
  
  x <- c(2,3,4,5,6,7,8,9)
  for(i in x)
  {
    
    nodeReachedBueren <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodesBueren)"))
    nodeReachedBuerenVector <-nodeReachedBueren$scalars$value
    nodeReachedPb <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodesPb)"))
    nodeReachedPbVector <-nodeReachedPb$scalars$value
    nodeReachedBali <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodesBali)"))
    nodeReachedBaliVector <-nodeReachedBali$scalars$value
    nodeReachedRemainder <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#clientCompleteReachedAllNodesRemainder)"))
    nodeReachedRemainderVector <-nodeReachedRemainder$scalars$value
    aBueren<- data.table(aBueren, nodeReachedBuerenVector)  
    aPb<- data.table(aPb, nodeReachedPbVector)  
    aSchlangen<- data.table(aSchlangen, nodeReachedBaliVector)  
    aRemainder<- data.table(aRemainder, nodeReachedRemainderVector)  
    
  }
  sdmBuren <- apply(aBueren,1,sd)
  pffBuren <- rowSums(aBueren)/ncol(aBueren)
  errorBuren <- qnorm(0.975)*sdmBueren/sqrt(length(sdmBueren))
  errorLowerBuren <- pffBuren - sdmBuren
  errorUpperBuren <- pffBuren + sdmBuren
  
  sdmPb <- apply(aPb,1,sd)
  pffPb <- rowSums(aPb)/ncol(aPb)
  errorPb <- qnorm(0.975)*sdm/sqrt(length(sdmPb))
  errorLowerPb <- pffPb - sdmPb
  errorUpperPb <- pffPb + sdmPb
  
  sdmBali <- apply(aSchlangen,1,sd)
  pffBali <- rowSums(aSchlangen)/ncol(aSchlangen)
  errorBali <- qnorm(0.975)*sdmBali/sqrt(length(sdmBali))
  errorLowerBali <- pffBali - sdmBali
  errorUpperBali <- pffBali + sdmBali
  
  sdmRemainder <- apply(aRemainder,1,sd)
  pffRemainder <- rowSums(aRemainder)/ncol(aRemainder)
  errorRemainder <- qnorm(0.975)*sdmRemainder/sqrt(length(sdmRemainder))
  errorLowerRemainder <- pffRemainder - sdmRemainder
  errorUpperRemainder <- pffRemainder + sdmRemainder
  
  df <- rbind(data.frame(value=pffBueren, Legend = 'Bueren'),
              data.frame(value=errorLowerBueren, Legend = '95confidenceBueren'),
              data.frame(value=errorUpperBueren, Legend = '95confidenceBueren2'),
              data.frame(value=pffPb, Legend = 'Pb'),
              data.frame(value=errorLowerPb, Legend = '95confidencePb'),
              data.frame(value=errorUpperPb, Legend = '95confidencePb2'),
              data.frame(value=pffSchlangen, Legend = 'Schlangen'),
              data.frame(value=errorLowerSchlangen, Legend = '95confidenceSchlangen'),
              data.frame(value=errorUpperSchlangen, Legend = '95confidenceSchlangen2'),
              data.frame(value=pffRemainder, Legend = 'Remainder'),
              data.frame(value=errorLowerRemainder, Legend = '95confidenceRemainder'),
              data.frame(value=errorUpperRemainder, Legend = '95confidenceRemainder2'))
  
  
  
  
  labeling <- c("Bueren","95% confidence Lower Bueren","95% confidence Upper Bueren",
                "Paderborn","95% confidence Lower Paderborn","95% confidence Upper Paderborn",
                "Schlangen","95% confidence Lower Schlangen","95% confidence Upper Schlangen",
                "Remainder","95% confidence Lower Remainder","95% confidence Upper Remainder")
  
  myPlot<-   ggplot(df, aes(x=value, color=factor(Legend)))+stat_ecdf()+
    xlab("Number of packets") + ylab("eCDF") +    
    scale_linetype_manual(breaks=labeling,values=c(7,5,5,7,5,5,7,5,5,7,5))+
    scale_color_manual(breaks=labeling, values=c("#377EB8","#999999","#999999","#377EB8","#999999","#999999","#377EB8","#999999","#999999","#377EB8","#999999","#999999"))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")  
  
  myPlot <- myPlot +theme(legend.position = c(0.3, 0.2))
  print(myPlot)
  
  
 ggsave(filename=paste(mainPath,"delayGlobalClientAnnouncementMultiRun.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}

analyzeNewNodePropagationMultiRunCluster <- function(path){
   path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/nodeAnnouncement/General-"
  
   nodeReachedBueren <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnownBueren)"))
   nodeReachedBueren<-nodeReachedBueren$scalars$value
   length(nodeReachedBueren)
   nodeReachedPb <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnownPb)"))
   nodeReachedPb <-nodeReachedPb$scalars$value
   nodeReachedSchlangen <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnownHoevelhofBaliSchlangenMeshID)"))
   nodeReachedSchlangen<-nodeReachedSchlangen$scalars$value
   nodeReachedRemainder <- loadDataset(paste(paste(path,0, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnownRemainder)"))
   nodeReachedRemainder<-nodeReachedRemainder$scalars$value
   
   aBueren <- data.table(nodeReachedBueren)
   aPb <- data.table(nodeReachedPb)
   aSchlangen <- data.table(nodeReachedSchlangen)
   aRemainder <- data.table(nodeReachedRemainder)
   
  
  
  x <- c(1,2,3,4,5,6,7,8,9)
  for(i in x)
  {
    i <- 6
    nodeReachedBueren <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnownBueren)"))
    nodeReachedBueren<-nodeReachedBueren$scalars$value
    nodeReachedPb <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnownPb)"))
    nodeReachedPb <-nodeReachedPb$scalars$value
    nodeReachedSchlangen <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnownHoevelhofBaliSchlangenMeshID)"))
    nodeReachedSchlangen<-nodeReachedSchlangen$scalars$value
    nodeReachedRemainder <- loadDataset(paste(paste(path,i, sep = ""),".sca", sep = ""), add(type="scalar",select="name(#allNodesKnownRemainder)"))
    nodeReachedRemainder<-nodeReachedRemainder$scalars$value
    aBueren<- data.table(aBueren, nodeReachedBueren)  
    aPb <- data.table(aPb, nodeReachedPb)  
    aSchlangen <- data.table(aSchlangen, nodeReachedSchlangen)  
    aRemainder <- data.table(aRemainder,nodeReachedRemainder)  
  }
  
  mBueren <- rowMeans(aBueren,na.rm = TRUE)
  sdmBueren <- apply(aBueren,1,sd)
  pffBueren <- rowSums(aBueren)/ncol(aBueren)
  errorBueren <- qnorm(0.975)*sdmBueren/sqrt(length(sdmBueren))
  errorLowerBueren <- pffBueren - sdmBueren
  errorUpperBueren <- pffBueren + sdmBueren
  
  mPb <- rowMeans(aPb,na.rm = TRUE)
  sdmPb <- apply(aPb,1,sd)
  pffPb <- rowSums(aPb)/ncol(aPb)
  errorPb <- qnorm(0.975)*sdmPb/sqrt(length(sdmPb))
  errorLowerPb <- pffPb - sdmPb
  errorUpperPb <- pffPb + sdmPb
  
  mSchlangen <- rowMeans(aSchlangen,na.rm = TRUE)
  sdmSchlangen <- apply(aSchlangen,1,sd)
  pffSchlangen <- rowSums(aSchlangen)/ncol(aSchlangen)
  errorSchlangen <- qnorm(0.975)*sdmSchlangen/sqrt(length(sdmSchlangen))
  errorLowerSchlangen <- pffSchlangen - sdmSchlangen
  errorUpperSchlangen <- pffSchlangen + sdmSchlangen
  
  mRemainder <- rowMeans(aRemainder,na.rm = TRUE)
  sdmRemainder <- apply(aRemainder,1,sd)
  pffRemainder <- rowSums(aRemainder)/ncol(aRemainder)
  errorRemainder <- qnorm(0.975)*sdmRemainder/sqrt(length(sdmRemainder))
  errorLowerRemainder <- pffRemainder - sdmRemainder
  errorUpperRemainder <- pffRemainder + sdmRemainder
  
  df <- rbind(data.frame(value=pffBueren, Legend = 'Bueren'),
              data.frame(value=errorLowerBueren, Legend = '95confidenceBueren'),
              data.frame(value=errorUpperBueren, Legend = '95confidenceBueren2'),
              data.frame(value=pffPb, Legend = 'Pb'),
              data.frame(value=errorLowerPb, Legend = '95confidencePb'),
              data.frame(value=errorUpperPb, Legend = '95confidencePb2'),
              data.frame(value=pffSchlangen, Legend = 'Schlangen'),
              data.frame(value=errorLowerSchlangen, Legend = '95confidenceSchlangen'),
              data.frame(value=errorUpperSchlangen, Legend = '95confidenceSchlangen2'),
              data.frame(value=pffRemainder, Legend = 'Remainder'),
              data.frame(value=errorLowerRemainder, Legend = '95confidenceRemainder'),
              data.frame(value=errorUpperRemainder, Legend = '95confidenceRemainder2'))
  
 
  
  
  labeling <- c("Bueren","95% confidence Lower Bueren","95% confidence Upper Bueren",
                "Paderborn","95% confidence Lower Paderborn","95% confidence Upper Paderborn",
                "Schlangen","95% confidence Lower Schlangen","95% confidence Upper Schlangen",
                "Remainder","95% confidence Lower Remainder","95% confidence Upper Remainder")
  myPlot<-    ggplot(df, aes(x=value, color=factor(Legend,labels=labeling)))+stat_ecdf()+
    xlab("Number of packets") + ylab("eCDF") +     scale_linetype_manual(breaks=labeling,values=c(7,5,5,7,5,5,7,5,5,7,5))+
    scale_color_manual(breaks=labeling, values=c("#377EB8","#999999","#999999","#377EB8","#999999","#999999","#377EB8","#999999","#999999","#377EB8","#999999","#999999"))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")
  
  myPlot<-makePlot(myPlot)
  print(myPlot)
  ggsave(filename=paste(mainPathImprovement,"delayGlobalNodeAnnouncementMultiRun.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}
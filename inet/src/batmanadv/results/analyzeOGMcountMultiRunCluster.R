
analyzeOGMcountNoClusterMultiRun <- function(path)
{
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/packetCount/General-"
  
  ogmReceivedBueren <- loadVectorsShaped(paste(paste(path,4, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedBueren)"))
  ogmReceivedPaderborn <- loadVectorsShaped(paste(paste(path,4, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedPaderborn)"))
  ogmReceivedBaliSchlangenHoevelhof <- loadVectorsShaped(paste(paste(path,4, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedBaliSchlangenHoevelhof)"))
  ogmReceivedRemainder <- loadVectorsShaped(paste(paste(path,4, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedRemainder)"))
  ogmSentBueren <- loadVectorsShaped(paste(paste(path,4, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentBueren)"))
  ogmSentPaderborn<- loadVectorsShaped(paste(paste(path,4, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentPaderborn)"))
  ogmSentBaliSchlangenHoevelhof <- loadVectorsShaped(paste(paste(path,4, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentBaliSchlangenHoevelhof)"))
  ogmSentRemainder <- loadVectorsShaped(paste(paste(path,4, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentRemainder)"))
  
  #Remove data from "Mesh-only" nodes and gateways
  ogmReceivedBueren<-ogmReceivedBueren[!(ogmReceivedBueren$ogmReceivedBueren==0|ogmReceivedBueren$ogmReceivedBueren>10000),]
  ogmReceivedPaderborn<-ogmReceivedPaderborn[!(ogmReceivedPaderborn$ogmReceivedPaderborn<10|ogmReceivedPaderborn$ogmReceivedPaderborn>10000),]
  ogmReceivedBaliSchlangenHoevelhof<-ogmReceivedBaliSchlangenHoevelhof[!(ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof==0|ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof>4000),]
  ogmReceivedRemainder<-ogmReceivedRemainder[!(ogmReceivedRemainder$ogmReceivedRemainder==0|ogmReceivedRemainder$ogmReceivedRemainder>10000),]
  
  ogmSentBueren<-ogmSentBueren[!(ogmSentBueren$ogmSentBueren<50|ogmSentBueren$ogmSentBueren>400),]
  ogmSentPaderborn<-ogmSentPaderborn[!(ogmSentPaderborn$ogmSentPaderborn<50|ogmSentPaderborn$ogmSentPaderborn>1000),]
  ogmSentBaliSchlangenHoevelhof<-ogmSentBaliSchlangenHoevelhof[!(ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof<300|ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof>600),]
  ogmSentRemainder<-ogmSentRemainder[!(ogmSentRemainder$ogmSentRemainder<50|ogmSentRemainder$ogmSentRemainder>1000),]
  

  aSentBueren <- data.table(ogmSentBueren$ogmSentBueren)
  aReceivedBueren <- data.table(ogmReceivedBueren$ogmReceivedBueren)
  
  aSentPb <- data.table(ogmSentPaderborn$ogmSentPaderborn)
  aReceivedPb <- data.table(ogmReceivedPaderborn$ogmReceivedPaderborn)
  
  aSentSchlangen <- data.table(ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof)
  aReceivedSchlangen <- data.table(ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof)
  
  aSentRemainder <- data.table(ogmSentRemainder$ogmSentRemainder)
  aReceivedRemainder <- data.table(ogmReceivedRemainder$ogmReceivedRemainder)
  
  
  #x <- c(1,4,6,8,9)
  x <- c(9)
  for(i in x)
  {
    ogmReceivedBueren <- loadVectorsShaped(paste(paste(path,i, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedBueren)"))
    ogmReceivedPaderborn <- loadVectorsShaped(paste(paste(path,i, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedPaderborn)"))
    ogmReceivedBaliSchlangenHoevelhof <- loadVectorsShaped(paste(paste(path,i, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedBaliSchlangenHoevelhof)"))
    ogmReceivedRemainder <- loadVectorsShaped(paste(paste(path,i, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedRemainder)"))
    ogmSentBueren <- loadVectorsShaped(paste(paste(path,i, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentBueren)"))
    ogmSentPaderborn<- loadVectorsShaped(paste(paste(path,i, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentPaderborn)"))
    ogmSentBaliSchlangenHoevelhof <- loadVectorsShaped(paste(paste(path,i, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentBaliSchlangenHoevelhof)"))
    ogmSentRemainder <- loadVectorsShaped(paste(paste(path,i, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentRemainder)"))
    #Remove data from "Mesh-only" nodes and gateways
    ogmReceivedBueren<-ogmReceivedBueren[!(ogmReceivedBueren$ogmReceivedBueren==0|ogmReceivedBueren$ogmReceivedBueren>10000),]
    ogmReceivedPaderborn<-ogmReceivedPaderborn[!(ogmReceivedPaderborn$ogmReceivedPaderborn<10|ogmReceivedPaderborn$ogmReceivedPaderborn>10000),]
    ogmReceivedBaliSchlangenHoevelhof<-ogmReceivedBaliSchlangenHoevelhof[!(ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof==0|ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof>4000),]
    ogmReceivedRemainder<-ogmReceivedRemainder[!(ogmReceivedRemainder$ogmReceivedRemainder==0|ogmReceivedRemainder$ogmReceivedRemainder>10000),]
    ogmSentBueren<-ogmSentBueren[!(ogmSentBueren$ogmSentBueren<50|ogmSentBueren$ogmSentBueren>400),]
    ogmSentPaderborn<-ogmSentPaderborn[!(ogmSentPaderborn$ogmSentPaderborn<50|ogmSentPaderborn$ogmSentPaderborn>1000),]
    ogmSentBaliSchlangenHoevelhof<-ogmSentBaliSchlangenHoevelhof[!(ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof<300|ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof>600),]
    ogmSentRemainder<-ogmSentRemainder[!(ogmSentRemainder$ogmSentRemainder<50|ogmSentRemainder$ogmSentRemainder>1000),]
    print(i)
    aSentBueren <- data.table(aSentBueren,ogmSentBueren$ogmSentBueren)
    aReceivedBueren <- data.table(aReceivedBueren,ogmReceivedBueren$ogmReceivedBueren)
    
    aSentPb <- data.table(aSentPb,ogmSentPaderborn$ogmSentPaderborn)
    aReceivedPb <- data.table(aReceivedPb,ogmReceivedPaderborn$ogmReceivedPaderborn)
    
    aSentSchlangen <- data.table(aSentSchlangen,ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof)
    aReceivedSchlangen <- data.table(aReceivedSchlangen,ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof)
    
    aSentRemainder <- data.table(aSentRemainder,ogmSentRemainder$ogmSentRemainder)
    aReceivedRemainder <- data.table(aReceivedRemainder,ogmReceivedRemainder$ogmReceivedRemainder)
    
  }
  mSentBueren <- rowMeans(aSentBueren,na.rm = TRUE)
  mReceivedBueren <- rowMeans(aReceivedBueren,na.rm = TRUE)
  sdmReceivedBueren <- apply(aReceivedBueren,1,sd)
  sdmSentBueren<- apply(aSentBueren,1,sd)
  pffBueren <- rowSums(aSentBueren)/ncol(aSentBueren)
  pffReceivedBueren <- rowSums(aReceived)/ncol(aReceived)
  error <- qnorm(0.975)*sdmSentBueren/sqrt(length(sdmSentBueren))
  errorReceivedBueren <- qnorm(0.975)*sdmReceivedBueren/sqrt(length(sdmReceivedBueren))
  errorLowerBueren <- pffBueren - sdmSentBueren
  errorUpperBueren <- pffBueren + sdmSentBueren
  errorLowerBuerenReceived <- pffReceivedBueren - sdmReceivedBueren
  errorUpperBuerenReceived <- pffReceivedBueren + sdmReceivedBueren
  ############################################################################################
  mSentPb <- rowMeans(aSentPb,na.rm = TRUE)
  mReceivedPb <- rowMeans(aReceivedPb,na.rm = TRUE)
  sdmReceivedPb <- apply(aReceivedPb,1,sd)
  sdmSentPb<- apply(aSentPb,1,sd)
  pffPb <- rowSums(aSentPb)/ncol(aSentPb)
  pffReceivedPb <- rowSums(aReceivedPb)/ncol(aReceivedPb)
  errorPb <- qnorm(0.975)*sdmSentPb/sqrt(length(sdmSentPb))
  errorReceivedPb <- qnorm(0.975)*sdmReceivedPb/sqrt(length(sdmReceivedPb))
  errorLowerPb <- pffPb - sdmSentPb
  errorUpperPb <- pffPb + sdmSentPb
  errorLowerPbReceived <- pffReceivedPb - sdmReceivedPb
  errorUpperPbReceived <- pffReceivedPb + sdmReceivedPb
  ############################################################################################
  mSentSchlangen <- rowMeans(aSentSchlangen,na.rm = TRUE)
  mReceivedSchlangen <- rowMeans(aReceivedSchlangen,na.rm = TRUE)
  sdmReceivedSchlangen <- apply(aReceivedSchlangen,1,sd)
  sdmSentSchlangen<- apply(aSentSchlangen,1,sd)
  pffSchlangen <- rowSums(aSentSchlangen)/ncol(aSentSchlangen)
  pffReceivedSchlangen <- rowSums(aReceivedSchlangen)/ncol(aReceivedSchlangen)
  errorSchlangen <- qnorm(0.975)*sdmSentSchlangen/sqrt(length(sdmSentSchlangen))
  errorReceivedSchlangen <- qnorm(0.975)*sdmReceivedSchlangen/sqrt(length(sdmReceivedSchlangen))
  errorLowerSchlangen <- pffSchlangen - sdmSentSchlangen
  errorUpperSchlangen <- pffSchlangen + sdmSentSchlangen
  errorLowerSchlangenReceived <- pffReceivedSchlangen- sdmReceivedSchlangen
  errorUpperSchlangenReceived <- pffReceivedSchlangen + sdmReceivedSchlangen
  ############################################################################################
  mReceivedRemainder <- rowMeans(aReceivedRemainder,na.rm = TRUE)
  sdmReceivedRemainder <- apply(aReceivedRemainder,1,sd)
  sdmSentRemainder<- apply(aSentRemainder,1,sd)
  pffRemainder <- rowSums(aSentRemainder)/ncol(aSentRemainder)
  pffReceivedRemainder <- rowSums(aReceivedRemainder)/ncol(aReceivedRemainder)
  errorRemainder <- qnorm(0.975)*sdmSentRemainder/sqrt(length(sdmSentRemainder))
  errorReceivedRemainder <- qnorm(0.975)*sdmReceivedRemainder/sqrt(length(sdmReceivedRemainder))
  errorLowerRemainder <- pffRemainder - sdmSentRemainder
  errorUpperRemainder <- pffRemainder + sdmSentRemainder
  errorLowerRemainderReceived <- pffReceivedRemainder - sdmReceivedRemainder
  errorUpperRemainderReceived <- pffReceivedRemainder + sdmReceivedRemainder
  
  dSent <- rbind(data.frame(value=pffBueren, source = 'Bueren'),
                 data.frame(value=errorLowerBueren, source = '95confidenceBueren'),
                 data.frame(value=errorUpperBueren, source = '95confidence2Bueren'),
                 data.frame(value=pffPb, source = 'Paderborn'),
                 data.frame(value=errorLowerPb, source = '95confidencePaderborn'),
                 data.frame(value=errorUpperPb, source = '95confidence2Paderborn'),
                 data.frame(value=pffSchlangen, source = 'Schlangen'),
                 data.frame(value=errorLowerSchlangen, source = '95confidenceSchlangen'),
                 data.frame(value=errorUpperSchlangen, source = '95confidence2Schlangen'),
                 data.frame(value=pffRemainder, source = 'Remainder'),
                 data.frame(value=errorLowerRemainder, source = '95confidenceRemainder'),
                 data.frame(value=errorUpperRemainder, source = '95confidence2Remainder'))
  
  labeling <- c("Bueren","Variance","Variance",
                "Paderborn","Variance","Variance",
                "Schlangen","Variance","Variance",
                "Remainder","Variance","Variance")
  labeling2 <- c("Bueren","Variance","",
                "Paderborn","Variance Paderborn","",
                "Schlangen","Variance Schlangen","",
                "Remainder","Variance Remainder","")
  
  myPlot<- ggplot(dSent, aes(x=value, color=factor(source,labels=labeling2)))+stat_ecdf()+
    xlab("Number of packets") + ylab("eCDF") + 
    scale_color_manual(breaks=labeling,name="Legend", values=c("#377EB8","#999999","#999999","#FF7F00","#999999","#999999","#A65628","#999999","#999999","#984EA3","#999999","#999999"))+
    scale_linetype_manual(breaks=labeling, name="Legend",values=c(7,5,5,7,5,5,7,5,5,7,5,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")
  myPlot<-makePlot(myPlot)
  print(myPlot)
  #ggsave(filename=paste(mainPathImprovement,"sendEcdfMultiRun.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
  
  
  
  
  dReceived <- rbind(data.frame(value=pffReceivedBueren, source = 'Bueren'),
                 data.frame(value=errorLowerBuerenReceived, source = '95confidenceBueren'),
                 data.frame(value=errorUpperBuerenReceived, source = '95confidence2Bueren'),
                 data.frame(value=pffReceivedPb, source = 'Paderborn'),
                 data.frame(value=errorLowerPbReceived, source = '95confidencePaderborn'),
                 data.frame(value=errorUpperPbReceived, source = '95confidence2Paderborn'),
                 data.frame(value=pffReceivedSchlangen, source = 'Schlangen'),
                 data.frame(value=errorLowerSchlangenReceived, source = '95confidenceSchlangen'),
                 data.frame(value=errorUpperSchlangenReceived, source = '95confidence2Schlangen'),
                 data.frame(value=pffReceivedRemainder, source = 'Remainder'),
                 data.frame(value=errorLowerRemainderReceived, source = '95confidenceRemainder'),
                 data.frame(value=errorUpperRemainderReceived, source = '95confidence2Remainder'))
  
  myPlot<- ggplot(dReceived, aes(x=value, color=factor(source,labels=labeling),linetype=source))+stat_ecdf()+
    xlab("Number of packets") + ylab("eCDF") + 
    scale_color_manual(breaks=labeling,name="Legend", values=c("#377EB8","#999999","#999999","#FF7F00","#999999","#999999","#A65628","#999999","#999999","#984EA3","#999999","#999999"))+
    scale_linetype_manual(breaks=labeling, name="Legend",values=c(7,5,5,7,5,5,7,5,5,7,5,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")
  myPlot<-makePlot(myPlot)
  print(myPlot)
  ggsave(filename=paste(mainPathImprovement,"receivedEcdfMultiRun.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}
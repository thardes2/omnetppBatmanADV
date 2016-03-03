
analyzeOGMcountNoClusterMultiRun <- function(path)
{
  path <- "/home/tobias/Documents/ccs/results/singleFastDCluster/General-"
  path <- "/tmp/tmpClusterResults/new/General-"
  path <- "/tmp/tmpClusterResults/General-"
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/packetCount/General-"
  
  pathNoCluster <- "/home/tobias/Documents/ccs/results/networkSize/sameChannel/noCluster/General-0.sca"
  
  numOGMReceivedCountVpn <-loadDataset(pathNoCluster, add(type="scalar",select="name(#numOGMReceivedCountVpn)"))
  numOwnOGMSentCountVpn <-loadDataset(pathNoCluster, add(type="scalar",select="name(#numOwnOGMSentCountVpn)"))
  
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  
  numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<10000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]
  
  numOGMReceivedCountVpnVector <-numOGMReceivedCountVpn$scalars$value
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector !=max(numOGMReceivedCountVpnVector)]
  numOwnOGMSentCountVpnVector <-numOwnOGMSentCountVpn$scalars$value
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector !=max(numOwnOGMSentCountVpnVector)]
  numOwnOGMSentCountVpnVector <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector != "0"]
  numOGMReceivedCountVpnVector <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector != "0"]
  
  numOGMReceivedCountVpnVectorSubset <- numOGMReceivedCountVpnVector[numOGMReceivedCountVpnVector<10000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVector[numOwnOGMSentCountVpnVector<1000]
  numOwnOGMSentCountVpnVectorSubset <- numOwnOGMSentCountVpnVectorSubset[numOwnOGMSentCountVpnVectorSubset>15]
  
  
  
  
  path <- "/home/tobias/Documents/ccs/results/singleFastDCluster/new/General-"
  ogmReceivedBueren <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedBueren)"))
  ogmReceivedPaderborn <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedPaderborn)"))
  ogmReceivedBaliSchlangenHoevelhof <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedBaliSchlangenHoevelhof)"))
  ogmReceivedRemainder <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedRemainder)"))
  ogmSentBueren <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentBueren)"))
  ogmSentPaderborn<- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentPaderborn)"))
  ogmSentBaliSchlangenHoevelhof <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentBaliSchlangenHoevelhof)"))
  ogmSentRemainder <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentRemainder)"))
  
  #Remove data from "Mesh-only" nodes and gateways
  ogmReceivedBueren<-ogmReceivedBueren[!(ogmReceivedBueren$ogmReceivedBueren==0|ogmReceivedBueren$ogmReceivedBueren>10000|ogmReceivedBueren$ogmReceivedBueren<285),]
  ogmReceivedPaderborn<-ogmReceivedPaderborn[!(ogmReceivedPaderborn$ogmReceivedPaderborn<10|ogmReceivedPaderborn$ogmReceivedPaderborn>10000|ogmReceivedPaderborn$ogmReceivedPaderborn<310),]
  ogmReceivedBaliSchlangenHoevelhof<-ogmReceivedBaliSchlangenHoevelhof[!(ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof==0|ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof>4000|ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof<200),]
  ogmReceivedRemainder<-ogmReceivedRemainder[!(ogmReceivedRemainder$ogmReceivedRemainder==0|ogmReceivedRemainder$ogmReceivedRemainder>10000|ogmReceivedRemainder$ogmReceivedRemainder<400),]
  
  ogmSentBueren<-ogmSentBueren[!(ogmSentBueren$ogmSentBueren<50|ogmSentBueren$ogmSentBueren>400),]
  ogmSentPaderborn<-ogmSentPaderborn[!(ogmSentPaderborn$ogmSentPaderborn<50|ogmSentPaderborn$ogmSentPaderborn>900),]
  #ogmSentBaliSchlangenHoevelhof<-ogmSentBaliSchlangenHoevelhof[!(ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof<300|ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof>600),]
  ogmSentRemainder<-ogmSentRemainder[!(ogmSentRemainder$ogmSentRemainder<50|ogmSentRemainder$ogmSentRemainder>1000),]
  df <-  rbind(data.frame(value = ogmReceivedBueren$ogmReceivedBueren, source = 'Büren (139 Nodes)'),
               data.frame(value = ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof, source = 'Schlangen (58 Nodes)'),
               data.frame(value = ogmReceivedPaderborn$ogmReceivedPaderborn, source = 'Paderborn (271 Nodes)'),
               data.frame(value = ogmReceivedRemainder$ogmReceivedRemainder, source = 'Remainder (318 Nodes)'),
               data.frame(value = numOGMReceivedCountVpnVectorSubset, source = 'No improvement (779 Nodes)'))
  #ReceivedOneFastD
  myPlotReceivedOneFastD <- ggplot(df, aes(x=value, color=source,linetype = source, size=source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
    scale_size_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c(1,1,1,1,2))+
    scale_color_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E"))+
    scale_linetype_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(8,1,7,4,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    theme(legend.position = c(0.6, 0.8))+expand_limits(x=0)
  print(myPlotReceivedOneFastD)  
  
  path <- "/home/tobias/Documents/ccs/results/singleFastDCluster/General-"
  ogmReceivedBueren <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedBueren)"))
  ogmReceivedPaderborn <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedPaderborn)"))
  ogmReceivedBaliSchlangenHoevelhof <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedBaliSchlangenHoevelhof)"))
  ogmReceivedRemainder <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmReceivedRemainder)"))
  ogmSentBueren <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentBueren)"))
  ogmSentPaderborn<- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentPaderborn)"))
  ogmSentBaliSchlangenHoevelhof <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentBaliSchlangenHoevelhof)"))
  ogmSentRemainder <- loadVectorsShaped(paste(paste(path,3, sep = ""),".vec", sep = ""), add(type="vector",select="name(ogmSentRemainder)"))
  
  #Remove data from "Mesh-only" nodes and gateways
  ogmReceivedBueren<-ogmReceivedBueren[!(ogmReceivedBueren$ogmReceivedBueren==0|ogmReceivedBueren$ogmReceivedBueren>10000),]
  ogmReceivedPaderborn<-ogmReceivedPaderborn[!(ogmReceivedPaderborn$ogmReceivedPaderborn<10|ogmReceivedPaderborn$ogmReceivedPaderborn>10000),]
  ogmReceivedBaliSchlangenHoevelhof<-ogmReceivedBaliSchlangenHoevelhof[!(ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof==0|ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof>4000),]
  ogmReceivedRemainder<-ogmReceivedRemainder[!(ogmReceivedRemainder$ogmReceivedRemainder==0|ogmReceivedRemainder$ogmReceivedRemainder>10000),]
  
  ogmSentBueren<-ogmSentBueren[!(ogmSentBueren$ogmSentBueren<50|ogmSentBueren$ogmSentBueren>400),]
  ogmSentPaderborn<-ogmSentPaderborn[!(ogmSentPaderborn$ogmSentPaderborn<50|ogmSentPaderborn$ogmSentPaderborn>900),]
  #ogmSentBaliSchlangenHoevelhof<-ogmSentBaliSchlangenHoevelhof[!(ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof<300|ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof>600),]
  ogmSentRemainder<-ogmSentRemainder[!(ogmSentRemainder$ogmSentRemainder<50|ogmSentRemainder$ogmSentRemainder>1000),]
  dfImprovement <- df
  df <-  rbind(data.frame(value = ogmReceivedBueren$ogmReceivedBueren, source = 'Büren (139 Nodes)'),
               data.frame(value = ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof, source = 'Schlangen (58 Nodes)'),
               data.frame(value = ogmReceivedPaderborn$ogmReceivedPaderborn, source = 'Paderborn (271 Nodes)'),
               data.frame(value = ogmReceivedRemainder$ogmReceivedRemainder, source = 'Remainder (318 Nodes)'),
               data.frame(value = numOGMReceivedCountVpnVectorSubset, source = 'No improvement (779 Nodes)'))
  myPlot <- ggplot(df, aes(x=value, color=source,linetype = source, size=source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
    scale_size_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c(1,1,1,1,2))+
    scale_color_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E"))+
    scale_linetype_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(8,1,7,4,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    theme(legend.position = c(0.6, 0.8))+expand_limits(x=0)
  print(myPlot)  

  
  dfImprovement$improvement <- "Cluster"
  df$improvement  <- "Cluster + one fastd" 
  dfnew <- rbind(df,dfImprovement)
  dfnew$improvement[dfnew$source == "No improvement (779 Nodes)"] = "Old"
  
  f <- factor(c(), levels=c("both", "none", "old"))
  f <- factor(c(), levels=c("both", "old", "none")); f <- levels(f)
    dfnew$improvement <- factor( c(), levels( dfnew$improvement)[c("both", "old", "none")])
  head(dfnew$improvement)
  myPlotImprovement <- ggplot(dfnew, aes(x=value, color=source,linetype = source, size=source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
    scale_size_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c(1,1,1,1,2))+
    scale_color_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E"))+
    scale_linetype_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(8,1,7,4,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    theme(legend.position = c(0.0, 0.0),legend.justification=c(0.0, 0.0))+expand_limits(x=0)+facet_grid(improvement~.)
  
  print(myPlotImprovement)
    
  ggsave(filename=paste(mainPathImprovement4,"receivedEcdfCompare.pdf", sep = ""), width = 8, height = 6, dpi = 120,plot=myPlotImprovement)
  
  
  
  
  
  
  
  dSent <- rbind(data.frame(value=ogmSentBueren$ogmSentBueren, source="Büren (139 Nodes)"), 
                 data.frame(value=ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof, source="Schlangen (58 Nodes)"),
                 data.frame(value=ogmSentPaderborn$ogmSentPaderborn, source="Paderborn (271 Nodes)"), 
                 data.frame(value=ogmSentRemainder$ogmSentRemainder, source="Remainder (318 Nodes)"),
                 data.frame(value=numOwnOGMSentCountVpnVectorSubset, source = 'No improvement (779 Nodes)'))
  
  #myPlot <- ggplot(dSent, aes(x=value, color=source,linetype = source, size=source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
  #  scale_size_manual(breaks=c("Simulation","Variance","Variance"), values=c(2,2,2,2,2))+
  ##  scale_color_manual(breaks=c("Bueren (139)","Schlangen (58)","Paderborn (271)","Remainder (318)","Model(779)"),name="Legend", values=c("#999999","#FF7F00","#A65628","#377EB8","#F781BF"))+
  #  scale_linetype_manual(breaks=c("Bueren (139)","Schlangen (58)","Paderborn (271)","Remainder (318)","Model(779)"), name="Legend",values=c(8,1,7,4,5))+
  #  theme_my(base_family = "Palatino") +expand_limits(x=0) +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
  #  theme(legend.position = c(0.15, 0.4))+expand_limits(x=0)
  
  myPlotSendOneFastD <- ggplot(dSent, aes(x=value, color=source,linetype = source, size=source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
    scale_size_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c(1,1,1,1,1))+
    scale_color_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E"))+
    scale_linetype_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(8,1,7,4,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    theme(legend.position = c(0.154, 0.8))+expand_limits(x=0)
  print(myPlotSendOneFastD)
  #ggsave(filename=paste(mainPathImprovement4,"sendEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlotSendOneFastD)
  
 
  
}
analyzeOGMcountNoCluster <- function(path)
{
  #
  path <- "/home/tobias/Documents/ccs/results/networkSize/equalDistribution/noCluster/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/tmp/15/General-0.sca"
  path <-  "/home/tobias/Documents/ccs/results/networkSize/sameChannel/noCluster/General-0.sca"
  path <-  "/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/General-0.sca"
  
  path <- "/home/tobias/Documents/ccs/results/singleFastD/noCluster/General-0.sca"
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
  
  df <-  rbind(data.frame(value = numOGMReceivedCountVpnVectorSubset, Legend = 'Model(779)'),
               data.frame(value = measuredVPNRealReceived, Legend = 'Real(779)'))
  df <-  rbind(data.frame(value = numOGMReceivedCountVpnVectorSubset, Legend = 'Model(779)'),
               data.frame(value = measuredVPNRealReceivedOpti, Legend = 'Real(820)'))
 # pdf(file= paste(mainPath,'receivedEcdf.pdf',sep = ""))
# myPlot<- ggplot(df, aes(x=value, color=factor(Legend,labels=c("Model (779 Nodes)","Real (779 Nodes)")),linetype=Legend))+ stat_ecdf()+  xlab("Number of packets") + ylab("eCDF") +#
   
#   scale_color_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"),name="Legend", values=c("#377EB8","#FF7F00"))+
#   scale_linetype_manual(breaks=c("Model (779 Nodes)","Real (779 Nodes)"), name="Legend",values=c(5,7))+
#  theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")
 
# print(myPlot)

 myPlot<- ggplot(df, aes(value, colour = factor(Legend,labels=c("Model (779 Nodes)","Real (820 Nodes)")),linetype=Legend,size=Legend)) + stat_ecdf()+
   scale_size_manual(breaks=c("Model (779 Nodes)","Real (820 Nodes)"), values=c(1,1))+
   scale_color_manual(breaks=c("Model (779 Nodes)","Real (820 Nodes)"),name="Legend", values=c("#373737","#9B9B9B"))+
   scale_linetype_manual(breaks=c("Model (779 Nodes)","Real (820 Nodes)"), name="Legend",values=c(7,1))+
   theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
   xlab("Number of packets") + ylab("eCDF")
 
 myPlot<-makePlot(myPlot)
myPlot <- myPlot +theme(legend.position = c(0.15, 0.52))
 print(myPlot)

ggsave(filename=paste(mainPathImprovement3,"receivedEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
if(FALSE)
{
  ggsave(filename=paste(mainPathImprovement,"receivedEcdfEqualDistribution.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}
########################################################## START: GOF ##########################################################
ks.test(numOGMReceivedCountVpnVectorSubset,measuredVPNRealReceived)
ks.test(measuredVPNRealSend,numOwnOGMSentCountVpnVectorSubset)

, alternative="gr")

?chisq.test(tbl)
wilcox.test(mpg ~ am, data=mtcars) 
########################################################## END: GOF ##########################################################
 # dev.off()
  ############################## START: SENT ##############################
dSent <- rbind(data.frame(value=numOwnOGMSentCountVpnVectorSubset, source = 'Model(779)'),
               data.frame(value=measuredVPNRealSend, source = 'Real(779)'))  
dSent <- rbind(data.frame(value=numOwnOGMSentCountVpnVectorSubset, source = 'Model(779)'),
               data.frame(value=measuredVPNRealSendOpti, source = 'Real(820)'))  
 

myPlot<- ggplot(dSent, aes(value, colour = factor(source,labels=c("Model (779 Nodes)","Real (820 Nodes)")),linetype=source,size=source)) + stat_ecdf() + 
  scale_size_manual(breaks=c("Model (779 Nodes)","Real (820 Nodes)"), values=c(1,1))+
  scale_color_manual(breaks=c("Model (779 Nodes)","Real (820 Nodes)"),name="Legend", values=c("#373737","#9B9B9B"))+
  scale_linetype_manual(breaks=c("Model (779 Nodes)","Real (820 Nodes)"), name="Legend",values=c(7,1))+
  theme_my(base_family = "Palatino") +expand_limits(x=0) + labs(color = "Legend")+
  xlab("Number of packets") + ylab("eCDF")

 myPlot<-makePlot(myPlot)
print(myPlot)

  ggsave(filename=paste(mainPathImprovement3,"sendEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)

}

analyzeOGMcountCluster <- function(path,pathNoCluster)
{
  pathCluster <- "/home/tobias/Documents/ccs/results/networkSize/sameChannel/cluster/General-3.vec"
  path <- pathCluster
  pathNoCluster <- "/home/tobias/Documents/ccs/results/networkSize/sameChannel/noCluster/General-0.sca"
  path <- "/home/tobias/Documents/ccs/results/tmp/14/General-0.vec"
  path <- "/home/tobias/Documents/ccs/results/multiRun/cluster/packetCount/General-4.vec"
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
  
  ogmReceivedBueren <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedBueren)"))
  ogmReceivedPaderborn <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedPaderborn)"))
  ogmReceivedBaliSchlangenHoevelhof <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedBaliSchlangenHoevelhof)"))
  ogmReceivedRemainder <- loadVectorsShaped(path, add(type="vector",select="name(ogmReceivedRemainder)"))
  ogmSentBueren <- loadVectorsShaped(path, add(type="vector",select="name(ogmSentBueren)"))
  ogmSentPaderborn<- loadVectorsShaped(path, add(type="vector",select="name(ogmSentPaderborn)"))
  ogmSentBaliSchlangenHoevelhof <- loadVectorsShaped(path, add(type="vector",select="name(ogmSentBaliSchlangenHoevelhof)"))
  ogmSentRemainder <- loadVectorsShaped(path, add(type="vector",select="name(ogmSentRemainder)"))
  
  #Remove data from "Mesh-only" nodes and gateways
  ogmReceivedBueren<-ogmReceivedBueren[!(ogmReceivedBueren$ogmReceivedBueren==0|ogmReceivedBueren$ogmReceivedBueren>10000),]
  ogmReceivedPaderborn<-ogmReceivedPaderborn[!(ogmReceivedPaderborn$ogmReceivedPaderborn<10|ogmReceivedPaderborn$ogmReceivedPaderborn>10000),]
  ogmReceivedBaliSchlangenHoevelhof<-ogmReceivedBaliSchlangenHoevelhof[!(ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof==0|ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof>4000),]
  ogmReceivedRemainder<-ogmReceivedRemainder[!(ogmReceivedRemainder$ogmReceivedRemainder==0|ogmReceivedRemainder$ogmReceivedRemainder>10000),]
  
  ogmSentBueren<-ogmSentBueren[!(ogmSentBueren$ogmSentBueren<50|ogmSentBueren$ogmSentBueren>400),]
  ogmSentPaderborn<-ogmSentPaderborn[!(ogmSentPaderborn$ogmSentPaderborn<50|ogmSentPaderborn$ogmSentPaderborn>900),]
  ogmSentBaliSchlangenHoevelhof<-ogmSentBaliSchlangenHoevelhof[!(ogmSentBaliSchlangenHoevelhof$ogmSentBaliSchlangenHoevelhof>400),]
  ogmSentRemainder<-ogmSentRemainder[!(ogmSentRemainder$ogmSentRemainder<50|ogmSentRemainder$ogmSentRemainder>1000),]
  
  
  df <-  rbind(data.frame(value = ogmReceivedBueren$ogmReceivedBueren, source = 'Büren (139 Nodes)'),
               data.frame(value = ogmReceivedBaliSchlangenHoevelhof$ogmReceivedBaliSchlangenHoevelhof, source = 'Schlangen (58 Nodes)'),
               data.frame(value = ogmReceivedPaderborn$ogmReceivedPaderborn, source = 'Paderborn (271 Nodes)'),
               data.frame(value = ogmReceivedRemainder$ogmReceivedRemainder, source = 'Remainder (318 Nodes)'),
               data.frame(value = numOGMReceivedCountVpnVectorSubset, source = 'No improvement (779 Nodes)'))
  
  #myPlot <- ggplot(df, aes(x=value, color=source,linetype = source, size=source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
    #scale_size_manual(breaks=c("Simulation","Variance","Variance"), values=c(1,1,1,1,1))+
    #scale_color_manual(breaks=c("Bueren (139)","Schlangen (58)","Paderborn (271)","Remainder (318)","Model(779)"),name="Legend", values=c("#999999","#FF7F00","#A65628","#377EB8","#F781BF"))+
    #scale_linetype_manual(breaks=c("Bueren (139)","Schlangen (58)","Paderborn (271)","Remainder (318)","Model(779)"), name="Legend",values=c(8,1,7,4,5))+
    #theme_my(base_family = "Palatino") +expand_limits(x=0) +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    #theme(legend.position = c(0.6, 0.8))+expand_limits(x=0)
  myPlot <- ggplot(df, aes(x=value, color=source,linetype = source, size=source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
    scale_size_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c(1,1,1,1,2))+
    scale_color_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E"))+
    scale_linetype_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(8,1,7,4,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    theme(legend.position = c(0.6, 0.8))+expand_limits(x=0)
  print(myPlot)
  
    ggsave(filename=paste(mainPathImprovement,"receivedEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
  
  
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
  myPlot <- ggplot(dSent, aes(x=value, color=source,linetype = source, size=source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
    scale_size_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c(1,1,1,1,1))+
    scale_color_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E"))+
    scale_linetype_manual(breaks=c("Büren (139 Nodes)","Schlangen (58 Nodes)","Paderborn (271 Nodes)","Remainder (318 Nodes)","No improvement (779 Nodes)"), name="Legend",values=c(8,1,7,4,5))+
    theme_my(base_family = "Palatino") +expand_limits(x=0) +theme(axis.title.y=element_text(vjust=1.5))+  theme(axis.title.x=element_text(vjust=-0.5))+ 
    theme(legend.position = c(0.7, 0.4))+expand_limits(x=0)
  print(myPlot)
  ggsave(filename=paste(mainPathImprovement,"sendEcdf.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
}




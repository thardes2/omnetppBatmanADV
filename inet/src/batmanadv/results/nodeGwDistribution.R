analyzeWifiVpnDistribution <- function(path)
{
  
wifiVPNRelation <- rbind(data.frame(wifi = 33,vpn = 106, source = 'Büren',sum=139),
                         data.frame(vpn = 230, source = 'Paderborn',wifi = 36,sum=271),
                         data.frame(vpn = 49, source = 'Schlangen',wifi = 9,sum=58),
                         data.frame(vpn = 270, source = 'Remainder',wifi = 46,sum=318),
                         data.frame(vpn = 655, source = 'Model',wifi =124 ,sum=779))
data.m <- melt(wifiVPNRelation, id.vars='source')

myPlot<-ggplot(data.m, aes(source, value)) + geom_bar(aes(fill = variable),  stat="identity",position = position_dodge())+ xlab("Subnetworks") + ylab("Number of nodes")+
  #theme_my(base_family = "Palatino") + scale_fill_manual(values = c("#377EB8","#FF7F00","#A65628"),labels=c("Wifi only","VPN","Sum"))+
  theme_my(base_family = "Palatino") + scale_fill_manual(values = c("#373737","#696969","#9B9B9B"),labels=c("Wifi only","VPN","Sum"))+
  guides(fill=guide_legend(title="Nodetype"))+theme(axis.text.x = element_text(angle = 45, hjust = 1))

myPlot <- makePlot(myPlot)
myPlot <- myPlot+   theme(legend.position = c(0.1, 0.8))
print(myPlot)
ggsave(filename=paste(mainPathImprovement,"nodeTypes.pdf", sep = ""),  width = 8, height = 5, dpi = 120, plot=myPlot)
}

deviceDistribution <- function()
{
  deviceStats = c(145,110,14,12,4,586,3,59)
  deviceStatsNames = c("TL-WR841N/ND v8","TL-WR1043N/ND v1","TL-WR1043N/ND v2","TL-WDR4300 v1","Nanostation M","TL-WR841N/ND v9","UniFiAP Outdoor","other")
  
  
  par(mar=c(5,10,4,4)+.1)
  par(las=2)
  
  theTable <- within(data,deviceStats <- factor(deviceStats, levels=names(sort(table(deviceStats), decreasing=TRUE))))
  
  
  d <- data.frame(deviceStats=c(586,145,110,14,12,4,3,59), names=c("TL-WR841N/ND v9","TL-WR841N/ND v8","TL-WR1043N/ND v1","TL-WR1043N/ND v2","TL-WDR4300 v1","Nanostation M","UniFiAP Outdoor","Other"))
  d$devicestats2 <- factor(d$names,as.character(d$names))
  p <- ggplot(d, aes(y=deviceStats))  
  #pdf(file= paste(mainPath,'deviceDistribution.pdf',sep = ""))
  myPlot <- p + geom_bar(aes(x=devicestats2), data=d, stat="identity")+ ylab("# Nodes") +xlab("")+ ggtitle("Device distribution")+
    theme_my(base_family = "Palatino")  + theme(legend.position="none")+coord_flip() +
    geom_text(aes(label = deviceStats, y = deviceStats+30,x=names))#+theme(axis.text.x = element_text(angle = 90, hjust = 1))
  scale_y_continuous(labels =deviceStats) 
  print(myPlot)
  ggsave(filename=paste(mainPath,"deviceDistribution.pdf", sep = ""),  width = 8, height = 5, dpi = 120, plot=myPlot)
 # dev.off()
}


plotRealClientData <- function()
{
  clientData = read.csv("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/networkData/overallClients.csv",sep = ",",header=TRUE)  # read csv file 
  ts.dat <- read.zoo("/home/tobias/Documents/ccs/batman-hardes/ffpb_data/networkData/overallClients.csv",sep = ",",header=FALSE
                     ,format = '%Y-%M-%d %H:%M:%S', FUN=as.POSIXct)
  #dat <- data.frame(x= clientData$X2015.06.30.16.12.00,y= clientData$X1648.0)
  #dat.xts <- xts(clientData$X1648.0,clientData$X2015.06.30.16.12.00)
  ep <- endpoints(ts.dat,'hours',k=5)
abc <-  period.apply(x=ts.dat,ep,FUN=mean )
#  myPlot <- plot(abc, xlab = "Time",ylab="Number of non-mesh clients",width=50,height=50)
 # print(myPlot)
  
#  pdf(file= paste(mainPath,'clientsMonth.pdf',sep = ""))
# myPlot <- plot(ts.dat, xlab = "Time",ylab="Number of non-mesh clients",width=50,height=50)
 # print(myPlot)
#  dev.off()
 


myPlot <- autoplot(abc, facet = NULL)+ylab("Number of non-mesh clients")+ xlab("Time")+expand_limits(y=0) +
  theme_my(base_family = "Palatino")
print(myPlot)
  
  ggsave(filename=paste(mainPath,"clientsMonth.pdf", sep = ""),  width = 6, height = 3, dpi = 120, plot=myPlot)
    
  
}



#Deprecated!!
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


createRealGwStats <- function(){
  gwStats<-c(263,201,143,130,19,8,5,3)
  
  gwNames <- c("Gateway 1","Gateway 2","Gateway 3","Gateway 4","Gateway 5","Gateway 6","Gateway 7","Gateway 8")
  gwNames <- c("GW 1","GW 2","GW 3","GW 4","GW 5","GW 6","GW 7","GW 8")
  data <- factor(gwStats,gwNames)
  
  # pdf(file= paste(mainPath,'realGatewayDistribution.pdf',sep = ""))
  myPlot<-ggplot(data.frame(data),aes(x=factor(gwNames),y=gwStats/sum(gwStats)))+geom_bar(stat="identity") + ylab("Percentage of connected nodes") +xlab("")+
    scale_y_continuous(labels = percent_format())+
    geom_text(aes(label = paste(round(gwStats/sum(gwStats)*100),"%"), y = gwStats/sum(gwStats)+0.015, size = 3))+
    theme_my(base_family = "Palatino")  + theme(legend.position="none")
  print(myPlot
  ggsave(filename=paste(mainPath,"realGatewayDistribution.pdf", sep = ""),  width = 8, height = 5, dpi = 120, plot=myPlot)
  #dev.off()  
  
  gwStats<-c(245,124,181,140)
  gwNames <- c("Gateway 1","Gateway 2","Gateway 3","Gateway 4")
  data <- factor(gwStats,gwNames)
 # pdf(file= paste(mainPath,'realGatewayDistributionSimulation.pdf',sep = ""))
 myPlot <-ggplot(data.frame(data),aes(x=factor(gwNames),y=gwStats/sum(gwStats)))+geom_bar(stat="identity") + ylab("Percentage of connected nodes") +xlab("")+
    scale_y_continuous(labels = percent_format())+
    geom_text(aes(label = paste(round(gwStats/sum(gwStats)*100),"%"), y = gwStats/sum(gwStats)+0.015))+
    theme_my(base_family = "Palatino")  + theme(legend.position="none")
 print(myPlot)
 myPlot<-makePlot(myPlot)
 print(myPlot)
  ggsave(filename=paste(mainPath,"realGatewayDistributionSimulation.pdf", sep = ""),  width = 8, height = 5, dpi = 120, plot=myPlot)
  #dev.off()  
}
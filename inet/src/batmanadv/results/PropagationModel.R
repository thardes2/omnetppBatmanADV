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
  
  
  
  #pdf(file= paste(mainPath,'/propagationModel.pdf',sep = ""))
  myPlot<-ggplot(dSent, aes(x=value, color=source,linetype = source)) + stat_ecdf()+ xlab("Number of packets") + ylab("eCDF") +
   # scale_color_manual(breaks=c("FreeSpace","LogNormal","Nakagami","Rayleigh","RiceModel","TwoRayGround"),name="Legend", values=c("#999999","#FF7F00","#A65628","#377EB8","#F781BF","#984EA3"))+
    scale_color_manual(breaks=c("FreeSpace","LogNormal","Nakagami","Rayleigh","RiceModel","TwoRayGround"),name="Legend", values=c("#373737","#696969","#9B9B9B","#CDCDCD","#6E6E6E","#0A0A0A"))+
    scale_linetype_manual(breaks=c("FreeSpace","LogNormal","Nakagami","Rayleigh","RiceModel","TwoRayGround"), name="Legend",values=c(5,1,7,4,5,9))+
    theme_my(base_family = "Palatino")
  print(myPlot)
  myPlot<-makePlot(myPlot)
  
  
  #savePlot("propagationModel.pdf",8,5,120,myPlot)
  ggsave(filename=paste(mainPath,"propagationModel.pdf", sep = ""), width = 8, height = 5, dpi = 120, plot=myPlot)
  #dev.off()
  
  
}

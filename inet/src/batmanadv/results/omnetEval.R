# require(ggplot2)
mainPath <- "/home/tobias/Documents/ccs/svn/tobias.hardes/thesis/figures/plots/"
mainPathImprovement <- paste(mainPath,"improvement/",sep = "")
mainPathImprovement2 <- paste(mainPath,"improvement2/",sep = "")
mainPathImprovement3 <- paste(mainPath,"improvement3/",sep = "")
mainPathImprovement4 <- paste(mainPath,"improvement4/",sep = "")
defensePath <-"/home/tobias/Documents/ccs/svn/tobias.hardes/defence_thesis/plots/"
setwd('/home/tobias/Documents/ccs/batman-hardes/simulation_omnet/inet/src/batmanadv/results/')
#setwd('/home/tobias/Documents/ccs/sims/sim1/inet/src/batmanadv/results/')     #sim1

measuredVPNRealAll <- c(4860,4323,6231)
measuredVPNRealSend <- c(684,756,792)
measuredVPNRealReceived <- c(4176,3567,5439)

measuredVPNRealSendOpti <- c(701,670,741)
measuredVPNRealReceivedOpti <- c(1858,1752,2379)
nodesBueren <- 139
nodesBali <- 58
nodesPB <- 271 
nodesRemainder <- 318
nodeCountvector <- c(nodesBueren,nodesBali,nodesPB,nodesRemainder)
source("omnet_helpers.R")
source("functions.R")
source("analyzeOGMcount.R")
source("nodeGwDistribution.R")
source("PropagationModel.R")
source("delayMeasurement.R")
require("plyr")
require("xts")
require("zoo")
require("scales")
require("chron")
require("reshape2")
require("MASS")
require("agricolae")
require("data.table")
require("grid")
require("gridExtra")
#require("devtools")
labelsModel <- c("Model (779 Nodes)","Variance","","Real (779 Nodes)")
pathNoCluster <- "/home/tobias/Documents/ccs/results/networkSize/sameChannel/noCluster/General-0.sca"
pathCluster <- "/home/tobias/Documents/ccs/results/networkSize/sameChannel/cluster/General-0.vec"


df <- data.frame(
  trt = factor(c(1, 1, 2, 2)),
  resp = c(1, 5, 3, 4),
  group = factor(c(1, 2, 1, 2)),
  se = c(0.1, 0.3, 0.3, 0.2)
)
df2 <- df[c(1,3),]

# Define the top and bottom of the errorbars
limits <- aes(ymax = resp + se, ymin=resp - se)

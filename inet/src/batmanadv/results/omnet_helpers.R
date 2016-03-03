require(omnetpp)
require(ggplot2)
loadVectorsShaped <- function(vecFiles, ..., with.module.names=F){
	d <- loadDataset(vecFiles, ...)

	vectornames <- levels(d$vectors$name)
	if(length(vectornames) == 0) {
		warning("no vectors loaded!")
		return
	}
	if(length(d$scalars$name)){
		warning("scalars loaded, but will not be considered in this function")
	}

	#preprocess first vector
	reskeys <- subset(d$vectors$resultkey, d$vectors$name == vectornames[1])
	v <- loadVectors(d,reskeys)
	result <- v$vectordata

	#process remaining vectors
	if(length(vectornames) > 1) {
		for(name in vectornames[2:length(vectornames)]) {
			reskeys <- subset(d$vectors$resultkey, d$vectors$name == name)
			v <- loadVectors(d,reskeys)
			result <- merge(result, v$vectordata, by=c("eventno", "resultkey", "x"), all.x = TRUE, all.y = TRUE, suffixes = c("",name))
		}
	}
	names(result) <- c(names(result)[1:2], "time", vectornames)

	if (with.module.names) {
		result <- merge(result, d$vectors, by="resultkey", sort=F)
	}

	return (result)
}

listVectors <- function(vecFiles, ...){
	d <- loadDataset(vecFiles, ...)
	return (levels(d$vectors$name))
}

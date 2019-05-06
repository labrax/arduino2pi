library(readr)
library(ggplot2)

read_v2 <- function() {
  cols <- c("year", "month", "day", "hour", "minute", "second", "amount_samples", "pir_min", "pir_max", "pir_sum", "mv_min", "mv_max", "mv_sum", "phr_min", "phr_max", "bme_temp_avg", "bme_pressure_avg")
  df <- NULL
  
  for(file in c("20190107_v2.csv", "20190108_v2.csv")) {
    cdf <- read_csv(paste0("data/", file), col_names = cols)
    if(is.null(df)) {
      df <- cdf
    } else {
      df <- rbind(df, cdf)
    }
  }
  timestr <- paste(paste(df$year, df$month, df$day, sep = '-'), paste(df$hour, df$minute, df$second, sep = ':'))
  times <- as.POSIXct(timestr, tz="Europe/Berlin") # it is recorded in german time
  attributes(times)$tzone <- "Europe/London" #we need it in uk time
  
  df$year <- NULL
  df$month <- NULL
  df$day <- NULL
  df$hour <- NULL
  df$minute <- NULL
  df$second <- NULL
  
  df$time <- times
  return(df);
}


df <- read_v2();

# df <- df[df$time > as.POSIXct("2019/01/08 08:10:00", tz="Europe/London"),]
# df <- df[df$time < as.POSIXct("2019/01/08 08:30:00", tz="Europe/London"),]

# ggplot(df, aes(x=time, y=bme_temp_avg, col=4)) + geom_line() + theme(legend.position="none")
# ggplot(df, aes(x=time, y=bme_pressure_avg, col=5)) + geom_line() + theme(legend.position="none")

head(df)

df$phr_max <- df$phr_max/max(df$phr_max)
df$mv_max <- df$mv_max/max(df$mv_max)
df$bme_temp_avg <- df$bme_temp_avg/max(df$bme_temp_avg)
df$bme_pressure_avg <- df$bme_pressure_avg/max(df$bme_pressure_avg)

ggplot(df, aes(x=time)) +
  geom_point(aes(y=pir_max, color=1)) +
  geom_line(aes(y=mv_max), color=2) +
  geom_line(aes(y=phr_max), color=3) +
  geom_line(aes(y=bme_temp_avg), color=4) +
  geom_line(aes(y=bme_pressure_avg), color=5) +
  ylab('') +
  theme(legend.position="none")

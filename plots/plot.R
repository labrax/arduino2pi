
library(readr)
library(ggplot2)
cols <- c("year", "month", "day", "hour", "minute", "second", "amount_samples", "pir_min", "pir_max", "pir_sum", "mv_min", "mv_max", "mv_sum", "phr_min", "phr_max")
df <- read_csv("data/20190106.csv", col_names = cols)

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

df <- df[df$time > as.POSIXct("2019/01/06 04:35:00", tz="Europe/London"),]

df <- df[df$time < as.POSIXct("2019/01/06 04:45:00", tz="Europe/London"),]

head(df)

df$phr_max <- df$phr_max/max(df$phr_max)
df$mv_max <- df$mv_max/max(df$mv_max)

ggplot(df, aes(x=time)) + 
  geom_point(aes(y=pir_max, color=1)) + 
  geom_line(aes(y=mv_max), color=2) + 
  geom_line(aes(y=phr_max), color=3) + 
  ylab('') +
  theme(legend.position="none")


  scale_colour_manual(name = 'Sensores', 
                      guide = 'legend',
                      labels = c('PIR - movimento', 'Som (máximo medido em V)', 'Luminosidade (normalizado)'))

# arduino2pi

## Install

### Software

```bash
sudo apt-get install python3-pip
pip3 install pyserial requests
```

### InfluxDB setting

```bash
$ influxdb
> CREATE DATABASE roomsensors
```

## Set to run on boot

$ crontab -e

@reboot python $HOME/arduino2pi/realtime_process.py

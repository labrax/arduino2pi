

import serial
import time
import requests
from util import InfluxHandler

def run():
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    a = InfluxHandler('localhost:8086', 'roomsensors', 'nearbed', 'vvv-nano-bed')
    while True:
        read_serial = ser.readline()
        s = ser.readline().decode().strip()
        E = s.split(',')
        if len(E) < 5:
            continue
        PIR, MIC_V, PHR_S, BME_T, BME_P, BME_H = E
        a.sendInflux({'passiveinfrared': PIR, 'mic': MIC_V, 'photoresistor': PHR_S, 'temperature': BME_T, 'pressure': BME_P, 'humidity': BME_H})


if __name__ == '__main__':
    while True:
        try:
            run()
        except Exception as e:
            print(e)
        time.sleep(5)



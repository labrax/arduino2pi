

import serial
import time
import requests

url_string = 'http://localhost:8086/write?db=roomsensors_window'


def sendInflux(name, units, value, host="vvv-nano-2"):
    data_string = '{name},host={host},units={units} value={value}'.format(name=name, host=host, units=units, value=value)
    r = requests.post(url_string, data=data_string)


class MinMaxSum:
    def __init__(self):
        self.reset()
    def reset(self):
        self.min = 999999999
        self.max = -999999999
        self.sum = 0
        self.len = 0
    def add(self, n):
        if n < self.min:
            self.min = n
        if n > self.max:
            self.max = n
        self.sum += n
        self.len += 1

def run():
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    while True:
        read_serial = ser.readline()
        s = ser.readline().decode().strip()
        #print(s)
        E = s.split(',')
        if len(E) < 4:
            continue
        _, PHR, MIC_V, TEMP = E
        sendInflux("mic", "volts", MIC_V)
        sendInflux("photoresistor", "volts", PHR)
        sendInflux("temperature", "celsius", TEMP)

if __name__ == '__main__':
    while True:
        try:
            run()
        except Exception as e:
            print(e)
        time.sleep(5)



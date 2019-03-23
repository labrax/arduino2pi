

import serial
import time

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

        

#startup
filename = '/home/pi/arduino2pi/data/' + time.strftime("%Y%m%d") + '_v2.csv'
open_file = open(filename, "a")
print(open_file)

def output(s):
    global open_file
    global filename
    T = time.strftime("%Y,%m,%d,%H,%M,%S,")
    #print(T + s)
    open_file.write(T + s + '\n')
    nfile = '/home/pi/arduino2pi/data/' + time.strftime("%Y%m%d") + '_v2.csv'
    if nfile != filename:
        open_file.close()
        open_file = open(nfile, "a")
        filename = nfile

def run():
    ser = serial.Serial('/dev/ttyUSB0',9600)
    PIR_LIST, MIC_V_LIST, MIC_C_LIST, PHR_S_LIST, BMP_T_LIST, BMP_P_LIST = MinMaxSum(), MinMaxSum(), MinMaxSum(), MinMaxSum(), MinMaxSum(), MinMaxSum()
    workingwith = time.strftime("%M")
    while True:
        read_serial = ser.readline()
        s = ser.readline().decode().strip()
        #print(s)
        E = s.split(',')
        if len(E) < 6:
            continue
        _, PIR, MIC_V, MIC_C, PHR_S, BMP_T, BMP_P = E
        PIR_LIST.add(int(PIR))
        MIC_V_LIST.add(float(MIC_V))
        MIC_C_LIST.add(int(MIC_C))
        PHR_S_LIST.add(int(PHR_S))
        BMP_T_LIST.add(float(BMP_T))
        BMP_P_LIST.add(float(BMP_P))
        cur = time.strftime("%M")
        if cur != workingwith:
            Plen = PIR_LIST.len
            if Plen > 0:
                Pmin = PIR_LIST.min
                Pmax = PIR_LIST.max
                Psum = PIR_LIST.sum
                MVmin = MIC_V_LIST.min
                MVmax = MIC_V_LIST.max
                MVsum = MIC_V_LIST.sum
                #MCmin = min(MIC_C_LIST)
                #MCmax = max(MIC_C_LIST)
                #MCsum = sum(MIC_C_LIST)
                PHRmin = PHR_S_LIST.min
                PHRmax = PHR_S_LIST.max
                BTavg = BMP_T_LIST.sum/Plen
                BPavg = BMP_P_LIST.sum/Plen
                #PHRsum = sum(PHR_S_LIST)
                acc = ','.join([str(round(i, 2)) for i in [Plen, Pmin, Pmax, Psum, MVmin, MVmax, MVsum, PHRmin, PHRmax, BTavg, BPavg]])
                output(acc)
            PIR_LIST.reset()
            MIC_V_LIST.reset()
            MIC_C_LIST.reset()
            PHR_S_LIST.reset()
            BMP_T_LIST.reset()
            BMP_P_LIST.reset()
            workingwith = cur


if __name__ == '__main__':
    while True:
        try:
            run()
        except Exception as e:
            print(e)
        time.sleep(5)



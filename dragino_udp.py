#!/usr/bin/python3

import subprocess
import socket
import threading


def inputWorker(inpstream):
    print("Waiting for input")
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(("127.0.0.1", 30002))
    while True:
        data, _addr = s.recvfrom(4096)
        #print("received message:", data)
        inpstream.write(data.decode("utf-8"))


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP

proc = subprocess.Popen(
    ["stdbuf", "-oL", 'rf95modem'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True, shell=False, bufsize=1)

t = threading.Thread(target=inputWorker, args=(proc.stdin,))
t.start()

while True:
    line = proc.stdout.readline()
    if not line:
        break
    #print("READ:", line.rstrip())
    sock.sendto(bytes(line, "utf-8"), ("127.0.0.1", 30001))

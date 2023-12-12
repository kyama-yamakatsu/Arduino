#!/usr/bin/python
# -*- coding: utf-8 -*-
#
import socket

# コマンド設定
#command = 'acPower'
#command = 'spSwap'
#command = 'sp2nd'
#command = 'volume'
command = 'lightA'

param = 'start'

acPower = '1'
spSwap = '1'
sp2nd = '0'
volume = '50'
light = '8'


print(acPower)
print(spSwap)
print(sp2nd)
print(volume)
print(light)

# サーバーに依頼する
#host = '127.0.0.1'
host = '192.168.1.6'

port = 2000
server = (host, port)
socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

socket.connect(server)
socket.send((command+'\n'+param+'\n'+acPower+'\n'+spSwap+'\n'+sp2nd).encode())
socket.close()

exit(0)

#!/usr/bin/env python3

import argparse, serial, datetime, os

# system variables
base_dir = '~/zcfs_logs/'

# pkt variables
header_size = 9
address_buf = 4
eol_pkt = b'_end'
eol_size = len(eol_pkt)

# disk variables
disk_size = 1024  # 5 MB
#disk_size = 1024 * 1024 * 5 # 5 MB
disk = [b'\x00'] * disk_size

def write_to_disk(address, data):
    for (pos,d) in enumerate(data):
        disk[address + pos] = bytes([d])

# TODO
def print_disk(p_disk_size=4):
    for address in range(0, disk_size, p_disk_size):
        #print(('{} ' + ' {}'*4).format(hex(x), disk[x], disk[x+1], disk[x+2], disk[x+3]))
        #print(('0x{:03x}: ' + ' {:02x}'*p_disk_size).format(x, ord(disk[x]), ord(disk[x+1]), ord(disk[x+2]), ord(disk[x+3])))
        print(('0x{:03x}: ' + ' {:02x}'*p_disk_size).format(address, *[ord(disk[idx]) for idx in range(address, address+p_disk_size)]))

parser = argparse.ArgumentParser()
parser.add_argument('tty_port', help='tty of the connected STM32 board')
parser.add_argument('tty_speed', type=int, help='baudrate tty of the connected STM32 board')
parser.add_argument('-v', '--verbose', action='store_true', help='increase output verbosity')
args = parser.parse_args()

if args.verbose:
    print('port set to:{}{}\nbaudrate set to: {}'.format(' '*5, args.tty_port, args.tty_speed))

ser = serial.Serial(args.tty_port, args.tty_speed, timeout=None, xonxoff=False, rtscts=False, dsrdtr=False)


try:
    while True:
        pkt = ser.read_until(eol_pkt)
        print(pkt)
        print('header: ', pkt[:header_size])
        #### '0x' +''.join([hex(x)[2:] for x in a.encode()][::-1])
        addr = pkt[header_size : header_size+address_buf]
        addr_p = '0x' +''.join(['0' + hex(x)[2:] for x in addr][::-1]) # TODO
        print('address: ', addr_p)
        data_pkt = pkt[header_size+address_buf : - eol_size]
        print('body: ', data_pkt)


        # TODO
        write_to_disk(int(addr_p, 16), data_pkt)
        print_disk()
        
except Exception as e:
    e.printStackTrace()
    print('Exiting...')


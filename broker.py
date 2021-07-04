#!/usr/bin/env python3

import sys, os
import argparse, datetime, serial, signal

# system variables
base_dir = '~/zcfs_logs/'

# pkt variables
header_size = 9
address_buf = 4
eol_pkt = b'_end'
eol_size = len(eol_pkt)

# disk variables
#disk_size = 1024 * 10  # 5 MB
disk_size = 1024 * 1024 * 5 # 5 MB
disk = [b'\x00'] * disk_size
superblock_address_5MB = 0x4f7bf4


#
# Signal handler
#
def signal_handler(sig, frame):
    print('You pressed Ctrl+C!')
    clean_shutdown()
    sys.exit(0)


#
# Print Information
#
def print_sys_configuration():
    conf_title = ['# system variables', '# disk variables']
    conf_info = {
                 0:[( 'base dir', base_dir)],
                 1:[('disk size', disk_size)]
                }

    conf_max_len = [len(max([s for (s,d) in conf_info[els]], key=len)) for els in range(len(conf_title))]

    for idx in range(len(conf_title)):
        print('{}'.format(conf_title[idx]))
        for el in conf_info[idx]:
            print(('{}'.format(el[0])).rjust(conf_max_len[idx]) + ': {}'.format(el[1]))
    print()


def print_tty_configuration(args):
    print('port set to:{}{}\nbaudrate set to: {}'.format(' '*5, args_in.tty_port, args_in.tty_speed))

def print_pkt_metadata(pkt):
    # pkt header
    header = pkt[:header_size]
    # disk address
    raw_addr = pkt[header_size : header_size+address_buf]
    disk_addr = '0x' +''.join(['{:02x}'.format(x) for x in raw_addr[::-1]])
    # pkt data
    pkt_data = pkt[header_size+address_buf : - eol_size]

    infos = [
             (  'raw packet',       pkt),
             (  'pkt header',    header),
             ('disk address', disk_addr),
             (    'pkt body',  pkt_data)
            ]
    inf_max_len = len(max([s for (s,d) in infos], key=len))

    for s,d in infos:
        print(('{}'.format(s)).rjust(inf_max_len) + ': {}'.format(d))
    print()


#
# Pkt utilities
#
def get_addr_pkt_wrt(pkt):
    raw_addr = pkt[header_size : header_size+address_buf]
    return '0x' +''.join(['{:02x}'.format(x) for x in raw_addr[::-1]])

def get_data_pkt(pkt):
     return pkt[header_size+address_buf : - eol_size]


#
# Disk utilities
#
def get_last_data_address():
    last_data_addr = disk[superblock_address_5MB:superblock_address_5MB+4]
    data_addr = int('0x' +''.join(['{:02x}'.format(ord(x)) for x in last_inode_addr[::-1]]), 16)
    return data_addr

def get_last_dinode_address():
    last_dinode_addr = disk[superblock_address_5MB+4:superblock_address_5MB+8]
    dinode_addr = int('0x' +''.join(['{:02x}'.format(ord(x)) for x in last_dinode_addr[::-1]]), 16)
    return dinode_addr

def write_to_disk(address, data):
    print('address: ',address)
    for (pos,d) in enumerate(data):
        disk[address + pos] = bytes([d])

def print_disk(starts=0, ends=disk_size, p_disk_size=4):
    for address in range(starts, ends, p_disk_size):
        print(('0x{:03x}: ' + ' {:02x}'*p_disk_size).format(address, *[ord(disk[idx]) for idx in range(address, address+p_disk_size)]))

def partial_print(p_disk_size=8):
    print('*** Disk ***')
    print_disk(0, 64, p_disk_size)
    print('.....')
    print('*** Superblock ***')
    print_disk(superblock_address_5MB, superblock_address_5MB + 64, p_disk_size)

def print_superblock(stop=64, p_disk_size=8):
    print('Superblock:')
    print_disk(superblock_address_5MB, superblock_address_5MB + stop, p_disk_size)

def print_dinode(p_disk_size=8):
    dinode_addr = get_last_dinode_address()
    print('last data inode_address:', hex(dinode_addr))
    print_disk(dinode_addr, superblock_address_5MB, p_disk_size)

def print_data(p_disk_size=8):
    data_addr = get_last_data_address()
    print('last data_address:', hex(data_addr))
    print_disk(0, data_addr, p_disk_size)


#
# General utilities
#
def clean_shutdown():
    print('Start performing clean shutdown...')
    # TODO
    print('End...')

def dump_disk():
    # Save disk in the host
    pass


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('tty_port', help='tty of the connected STM32 board')
    parser.add_argument('tty_speed', type=int, help='baudrate tty of the connected STM32 board')
    parser.add_argument('-v', '--verbose', action='store_true', help='increase output verbosity')
    args = parser.parse_args()

    signal.signal(signal.SIGINT, signal_handler)
    
    if args.verbose:
        print_tty_configuration(args)
    
    try:
        ser = serial.Serial(args.tty_port, args.tty_speed, timeout=None, xonxoff=False, rtscts=False, dsrdtr=False)
    except serial.serialutil.SerialException as se:
        print('{} tty not found'.format(args.tty_port))
        sys.exit(1)
    
    try:
        print_sys_configuration()
        while True:
            pkt = ser.read_until(eol_pkt)

            print("==== pkt read arrived")
            if(len(pkt) < 50):
                print_pkt_metadata(pkt)

            pkt_data = get_data_pkt(pkt)
            disk_addr = get_addr_pkt_wrt(pkt)
            #write_to_disk(int(disk_addr, 16), pkt_data)
            #print_disk(16)
            #partial_print()
            #print(pkt[:128])
            # print_dinode()
            # print_superblock(64)
            print_pkt_metadata(pkt)
            
    except Exception as e:
        e.printStackTrace()
        print('Exiting...')
    

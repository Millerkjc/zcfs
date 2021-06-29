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


def write_to_disk(address, data):
    for (pos,d) in enumerate(data):
        disk[address + pos] = bytes([d])

def print_disk(p_disk_size=4):
    for address in range(0, disk_size, p_disk_size):
        print(('0x{:03x}: ' + ' {:02x}'*p_disk_size).format(address, *[ord(disk[idx]) for idx in range(address, address+p_disk_size)]))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('tty_port', help='tty of the connected STM32 board')
    parser.add_argument('tty_speed', type=int, help='baudrate tty of the connected STM32 board')
    parser.add_argument('-v', '--verbose', action='store_true', help='increase output verbosity')
    args = parser.parse_args()
    
    if args.verbose:
        print_tty_configuration(args)
    
    ser = serial.Serial(args.tty_port, args.tty_speed, timeout=None, xonxoff=False, rtscts=False, dsrdtr=False)
    
    
    try:
        print_sys_configuration()
        while True:
            pkt = ser.read_until(eol_pkt)
            print_pkt_metadata(pkt)

            #write_to_disk(int(addr_p, 16), data_pkt)
            #print_disk()
            
    except Exception as e:
        e.printStackTrace()
        print('Exiting...')
    

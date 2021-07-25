#!/usr/bin/env python3

import sys, os
import argparse, datetime, serial, signal

# system variables
base_dir = './zcfs_logs/'

# pkt variables
header_size = 9
address_buf = 4
eol_pkt = b'_end'
eol_size = len(eol_pkt)

# disk variables
disk_size = 1024 * 1024 * 5 # 5 MB
disk = [b'\x00'] * disk_size
superblock_address_5MB = 0x4f6bf4

# user gui utilities
pkts_buffer = ([], 5)
console_buffer = ([], 5)

#
# Signal handler
#
def signal_handler(sig, frame):
    # print('DATA')
    # print_data()
    # print('DINODE')
    # print_dinode()
    # print_superblock(128)
    #partial_print()
    #print('DINODE')
    #print_disk(superblock_address_5MB-128, superblock_address_5MB)
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

def print_pkt_console(pkt):
    # pkt header
    header = pkt[:header_size]
    pkt_data = pkt[header_size : - eol_size]
    infos = [
             (  'raw packet',       pkt),
             (  'pkt header',    header),
             (    'pkt body',  pkt_data)
            ]
    inf_max_len = len(max([s for (s,d) in infos], key=len))

    for s,d in infos:
        print(('{}'.format(s)).rjust(inf_max_len) + ': {}'.format(d))
    print()

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
    data_addr = int('0x' +''.join(['{:02x}'.format(ord(x)) for x in last_data_addr[::-1]]), 16)
    return data_addr

def get_last_dinode_address():
    last_dinode_addr = disk[superblock_address_5MB+4:superblock_address_5MB+8]
    dinode_addr = int('0x' +''.join(['{:02x}'.format(ord(x)) for x in last_dinode_addr[::-1]]), 16)
    return dinode_addr

def write_to_disk(address, data):
    print('address: ',address)
    for (pos,d) in enumerate(data):
        disk[address + pos] = bytes([d])

def print_disk(starts=0, ends=disk_size, p_disk_size=4, hx=0):
    for address in range(starts, ends, p_disk_size):
        print(('0x{:08x}: ' + ' {:02x}'*p_disk_size + (' | ' + '{:c}'*p_disk_size)*hx).format(address, *[ord(disk[idx]) for idx in range(address, address+p_disk_size)], *[ord(disk[idx]) if chr(ord(disk[idx])).isprintable() else 46 for idx in range(address, address+p_disk_size)]))
        ## 46  == '.'
        

def partial_print(p_disk_size=8):
    print('*** Disk ***')
    Bprint_disk(0, 64, p_disk_size)
    print('.....')
    print('*** Superblock ***')
    print_disk(superblock_address_5MB, superblock_address_5MB + 128, p_disk_size)

def print_superblock(stop=64, p_disk_size=8):
    print('Superblock:')
    print_disk(superblock_address_5MB, superblock_address_5MB + stop, p_disk_size, 1)

def print_dinode(p_disk_size=8):
    dinode_addr = get_last_dinode_address()
    print('last data inode_address:', hex(dinode_addr))
    print_disk(dinode_addr, superblock_address_5MB, p_disk_size)

def print_data(p_disk_size=8):
    data_addr = get_last_data_address()
    print('last data_address:', hex(data_addr))
    print_disk(0, data_addr, p_disk_size, 1)


#
# General utilities
#
def clean_shutdown():
    print('Start performing clean shutdown...')
    # TODO
    # dump all fs on a file
    # dump_disk()
    print('End...')

def dump_disk():
    # Save disk in the host
    f = open(base_dir + datetime.datetime.now().strftime("%Y%m%d_%H%M%S") + '.zcfs', 'wb')
    f.write(b''.join(disk))
    f.close()









#
# User
#

def make_currying(f, a):
    def fc(*args):
        return f(a, *args)
    return fc

broker_utilities = {
                    0:('print last x pkt (MAX 5)', None),
                    1:('print superblock', print_superblock),
                    2:('print data', print_data),
                    3:('print last data address', get_last_data_address),
                    4:('print dinode', print_dinode),
                    5:('print last dinode address', get_last_dinode_address),
                    6:('print all file name and id', None),
                    7:('get file x (use the id)', None)
                   }

def menu_gui():
    cr_utilities = lambda u,x : ('{}) ' + u).format(x)

    #utilities = (
    #             'print last x pkt (MAX 5)',
    #             'print superblock',
    #             'print data',
    #             'print last data address',
    #             'print dinode',
    #             'print last dinode address',
    #             'print all file name and id',
    #             'get file x (use the id)'
    #            )

    print('=== Broker functionalities ===')
    for i in range(len(broker_utilities)):
        print(make_currying(cr_utilities, broker_utilities[i][0])(i+1))
    print()


def write_to_file(f, addr, data):
    f.seek(addr, 0)
    f.write(data)


def dump_all():
    pass











# def argparse_conf(parser):
#     direct_args = [
#                    #(       name, type,                                        help)
#                     ( 'tty_port',  str,          'tty of the connected STM32 board'),
#                     ('tty_speed',  int, 'baudrate tty of the connected STM32 board')
#                   ]
# 
#     option_str_arg = [
#                      #(   option_strings,       action,                        help)
#                       ('-v', '--verbose', 'store_true', 'increase output verbosity')
#                      ]
# 
#     for el in direct_args:
#         parser.add_argument(el[0], type=el[1], help=el[1])
# 
#     for el in option_str_arg:
#         parser.add_argument(el[0], el[1], action=el[2], help=el[3])



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    #argparse_conf(parser)
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
        #menu_gui()
        print('{} tty not found'.format(args.tty_port))
        sys.exit(1)
    
    print_sys_configuration()

    try:
        while True:
            pkt = ser.read_until(eol_pkt)

            # if(len(pkt) < 50):
            #     print_pkt_metadata(pkt)

            pkt_data = get_data_pkt(pkt)
            disk_addr = get_addr_pkt_wrt(pkt)
            #print_disk(16)
            #partial_print()
            #print(pkt[:128])
            # print_dinode()
            # print_superblock(64)
            #print_superblock(128)


            if pkt[:header_size] == b'zcfs_wrt_':
                write_to_disk(int(disk_addr, 16), pkt_data)
                # print_pkt_metadata(pkt)
            elif pkt[:header_size] == b'zcfs_cns_':
                print('#'*20);
                print('### CONSOLE PACKEEEET ###');
                print('#'*20);
                print_pkt_console(pkt)
            else:
                print("==== pkt read arrived")
                print("START WRITING")
                print_pkt_metadata(pkt)
                start = get_addr_pkt_wrt(pkt)
                end = get_data_pkt(pkt)
                end_2 = '0x' +''.join(['{:02x}'.format(x) for x in end[::-1]])

                #print("s - e:", start, ", ", end_2)
                #print("d: ", b''.join(disk[int(start,16):int(start,16) + int(end_2,16)]))

                #print('start: ', start, ", t:", type(start), ", int: ", int(start,16))
                #print('end_2: ', end_2, ", t:", type(end_2), ", int: ", int(end_2,16))

                ser.write(b''.join(disk[int(start,16):int(start,16) + int(end_2,16)]))
                print("STOP WRITING")

            # print('DATA')
            # print_data()
            # print('DINODE')
            # print_dinode(12)
            # print_superblock(256)
            
    except Exception as e:
        e.printStackTrace()
        print('Exiting...')
    

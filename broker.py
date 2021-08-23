#!/usr/bin/env python3

import sys, os
import argparse, datetime, serial, signal


#################
### VARIABLES ###
#################

#####
### system variables
#####
base_dir = './zcfs_logs/'

#####
### pkt variables
#####
header_size = 9
address_buf = 4
eol_pkt = b'_end'
eol_size = len(eol_pkt)

#####
### disk variables
#####
disk_size = 1024 * 1024 * 5 # 5 MB
disk = [b'\x00'] * disk_size
superblock_address_5MB = 0x4f6bf4


# user gui utilities
pkts_buffer = ([], 5)
console_buffer = ([], 5)


##########################
### SYSTEM ENVIRONMENT ###
##########################

def setup_env_store_disk():
    if not os.path.exists(base_dir):
        os.mkdir(base_dir)
        print('\ncreated ' + base_dir + ' under the following path: ' + os.getcwd())
        print('- file will be saved under ' + os.getcwd() + base_dir[1:] + '\n')


#################################
### SIGNAL HANDLER MANAGEMENT ###
#################################

#####
### Signal handler
#####
def signal_handler(sig, frame):
    print('You pressed Ctrl+C!')
    clean_shutdown()
    sys.exit(0)


##########################
### SHUTDOWN UTILITIES ###
##########################

#####
### Clean shutdown upon error
#####
def error_clean_shutdown():
    print('Start performing error clean shutdown...')
    # dump all fs on a file
    error_dump_disk()
    print('End...')

#####
### Clean shutdown
#####
def clean_shutdown():
    print('Start performing clean shutdown...')
    # dump all fs on a file
    dump_disk()
    print('End...')

#####
### dump disk
#####
def error_dump_disk():
    '''
        Save disk in the host
    '''
    f = open(base_dir + datetime.datetime.now().strftime("%Y%m%d_%H%M%S") + 'ERROR' + '.zcfs', 'wb')
    f.write(b''.join(disk))
    f.close()
    
def dump_disk():
    '''
        Save disk in the host
    '''
    f = open(base_dir + datetime.datetime.now().strftime("%Y%m%d_%H%M%S") + '.zcfs', 'wb')
    f.write(b''.join(disk))
    f.close()


#########################
### GENERAL UTILITIES ###
#########################

#####
### Print Information
#####
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


#####
### Print the tty configuration (port and baudrate)
#####
def print_tty_configuration(args_in):
    print('port set to:{}{}\nbaudrate set to: {}'.format(' '*5, args_in.tty_port, args_in.tty_speed))

#####
### Print the information console packets
#####
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

#####
### Print all pkt metadata information
#####
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


#####################
### PKT UTILITIES ###
#####################

#####
### Get address to write data from pkt
#####
def get_addr_pkt_wrt(pkt):
    raw_addr = pkt[header_size : header_size+address_buf]
    return '0x' +''.join(['{:02x}'.format(x) for x in raw_addr[::-1]])

#####
### Get data from pkt
#####
def get_data_pkt(pkt):
     return pkt[header_size+address_buf : - eol_size]


######################
### DISK UTILITIES ###
######################

#####
### Get the address of the next position after the last data write on disk.
#####
def get_last_data_address():
    last_data_addr = disk[superblock_address_5MB:superblock_address_5MB+4]
    data_addr = int('0x' +''.join(['{:02x}'.format(ord(x)) for x in last_data_addr[::-1]]), 16)
    return data_addr

#####
### Get the address of the last dinode written on disk
#####
def get_last_dinode_address():
    last_dinode_addr = disk[superblock_address_5MB+4:superblock_address_5MB+8]
    dinode_addr = int('0x' +''.join(['{:02x}'.format(ord(x)) for x in last_dinode_addr[::-1]]), 16)
    return dinode_addr

#####
### Write data to disk
#####
def write_to_disk(address, data):
    # print('address: ',address)
    for (pos,d) in enumerate(data):
        disk[address + pos] = bytes([d])

#####
### Print disk
#### default: entire disk with 4 byte per row
#####
def print_disk(starts=0, ends=disk_size, p_disk_size=4, hx=0):
    for address in range(starts, ends, p_disk_size):
        print(('0x{:08x}: ' + ' {:02x}'*p_disk_size + (' | ' + '{:c}'*p_disk_size)*hx).format(address, *[ord(disk[idx]) for idx in range(address, address+p_disk_size)], *[ord(disk[idx]) if chr(ord(disk[idx])).isprintable() else 46 for idx in range(address, address+p_disk_size)]))
        ## 46  == '.'
        

#####
### Print only partial disk
#### default: 8 byte per row
####          64 bytes of the disk's head
####          128 bytes from the superblock address
#####
def partial_print(p_disk_size=8):
    print('*** Disk ***')
    Bprint_disk(0, 64, p_disk_size)
    print('.....')
    print('*** Superblock ***')
    print_disk(superblock_address_5MB, superblock_address_5MB + 128, p_disk_size)

#####
### Print superblock
#####
def print_superblock(stop=64, p_disk_size=8):
    print('\n=== SUPERBLOCK')
    print('- Start')
    print_disk(superblock_address_5MB, superblock_address_5MB + stop, p_disk_size, 1)
    print('- End')

#####
### Print all dinodes
#####
def print_dinode(p_disk_size=8):
    print('\n=== Print all dinodes')
    print('- Start')
    dinode_addr = get_last_dinode_address()
    # print('last data inode_address:', hex(dinode_addr))
    print_disk(dinode_addr, superblock_address_5MB, p_disk_size)
    print('- End')

#####
### Print all data
#####
def print_data(p_disk_size=8):
    data_addr = get_last_data_address()
    print('\n=== Last data address')
    print(hex(data_addr))
    print_disk(0, data_addr, p_disk_size, 1)










def argparse_conf(parser):
    direct_args = [
                   #(       name, type,                                        help)
                    ( 'tty_port',  str,          'tty of the connected STM32 board'),
                    ('tty_speed',  int, 'baudrate tty of the connected STM32 board')
                  ]


    for el in direct_args:
        parser.add_argument(el[0], type=el[1], help=el[1])

    '''
    option_str_arg = [
                     #(   option_strings,       action,                        help)
                      ('-v', '--verbose', 'store_true', 'increase output verbosity')
                     ]

    for el in option_str_arg:
        parser.add_argument(el[0], el[1], action=el[2], help=el[3])
    '''


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    argparse_conf(parser)
    # parser.add_argument('tty_port', help='tty of the connected STM32 board')
    # parser.add_argument('tty_speed', type=int, help='baudrate tty of the connected STM32 board')
    # parser.add_argument('-v', '--verbose', action='store_true', help='increase output verbosity')
    args = parser.parse_args()

    signal.signal(signal.SIGINT, signal_handler)
    
    #if args.verbose:
    print_tty_configuration(args)
    setup_env_store_disk()


    '''
        Usage example:
            python3 broker.py /dev/ttyUSB0 38400
    '''

    try:
        ser = serial.Serial(args.tty_port, args.tty_speed, timeout=None, xonxoff=False, rtscts=False, dsrdtr=False)
    except serial.serialutil.SerialException as se:
        print('{} tty not found'.format(args.tty_port))
        sys.exit(1)
    
    print_sys_configuration()

    while True:
        try:
            # Set the serial packet EOL
            pkt = ser.read_until(eol_pkt)


            '''
                === PKTs MANAGEMENT
            '''

            if pkt[:header_size] == b'zcfs_wrt_':
                '''
                    Write pkt
                    It writes data directly on disk
                '''
                print("==== write pkt arrived")
                print('- Start')
                
                disk_addr = get_addr_pkt_wrt(pkt)
                pkt_data = get_data_pkt(pkt)
                write_to_disk(int(disk_addr, 16), pkt_data)
                print_pkt_metadata(pkt)
                
                print('- End')

            elif pkt[:header_size] == b'zcfs_cns_':
                '''
                    Console pkt (for user msg)
                    It prints msg to user - for debugging purpose or
                                            for gui purpose (future work)

                '''
                print("==== console pkt arrived")
                print('- Start')
                
                print_pkt_console(pkt)

                print('- End')

            else:
                '''
                    Read pkt
                    It reads bytes from the disk and it sends them to the board.
                '''
                print("==== read pkt arrived")
                print('- Start')

                print_pkt_metadata(pkt)
                start = get_addr_pkt_wrt(pkt)
                end = get_data_pkt(pkt)
                end_2 = '0x' +''.join(['{:02x}'.format(x) for x in end[::-1]])

                ser.write(b''.join(disk[int(start,16):int(start,16) + int(end_2,16)]))

                print('- End')

            '''
                - print all data
                - print all dinodes
                - print 32*8 bytes of superblock
            '''
            print_data()
            print_dinode(12)
            print_superblock(256)
            
        except Exception as e:
            e.printStackTrace()
            print('Exiting...')
            error_clean_shutdown()
            exit(1)
    

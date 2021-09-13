#!/usr/bin/env python3

import struct


def introspect(self):
    res = ''
    for k,v in self.__dict__.items():
        res += '{0}: {1}\n'.format(k, v)
    return res


class pure_data:
    def __init__(self, raw_data):
        self.raw_data = raw_data

class dinode:
    def __init__(self, raw_dinode):
        self.data_ptr = raw_dinode[:4]
        self.data_len = raw_dinode[4:8]
        self.next_dinode = raw_dinode[8:]

    def get_data_ptr_pretty_str(self):
        return '0x{:08x}'.format(struct.unpack('<i', self.data_ptr)[0])

    def get_next_dinode_pretty_str(self):
        return '0x{:08x}'.format(struct.unpack('<i', self.next_dinode)[0])

    def __str__(self):
        return introspect(self)


class inode:
    def __init__(self, raw_inode):
        self.fd = raw_inode[:4]
        self.name = raw_inode[4:20]
        self.time = raw_inode[20:24]
        self.size = raw_inode[24:28]
        self.is_open = raw_inode[28:29]
        self.last_dinode = raw_inode[29:33]
        self.next_dinode = raw_inode[33:]

    def __str__(self):
        return introspect(self)

class superblock:
    def __init__(self, address):
        self.address = address

a = b''.join(list(map(lambda x: bytes([int(x,16)]), '22 00 00 00 12 00 00 00 ac 6b 4f 00'.split(' '))))
print(a)

d = dinode(a)
print(d)
print(d.get_data_ptr_pretty_str())
print(d.get_next_dinode_pretty_str())

ina = b'\x03\x00\x00\x00test0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00*\x00\x00\x00)\x00\x00\x00\x01\xdckO\x00\xe4kO\x00'
inode_t = inode(ina)
print(inode_t)

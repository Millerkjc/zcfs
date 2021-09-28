#!/usr/bin/env python3

import struct, sys
import os.path
import graphviz

#############
### Vars ####
#############

superblock_address_5MB = 0x4f6bf4
dinode_len = 12
inode_len = 37

###############
### Classes ###
###############

def introspect(self):
    res = ''
    for k,v in self.__dict__.items():
        if k in ['ptr_data_address', 'ptr_dinode_address', 'dinode_start', 'dinode_end', 'last_dinode', 'next_dinode']:
            res += '{0}: {1}\n'.format(k, hex(v))
        else:
            if type(v) is dict:
                for kd, vd in v.items():
                    res += str(hex(kd)) + ': \n'
                    res += vd.__str__()
                    res += '\n'
            else:
                res += '{0}: {1}\n'.format(k, v)
    return res


class DataStack:
    def __init__(self, raw_disk, data_end):
        self.data_end = data_end
        self.data_stack = raw_disk[: self.data_end]

    def get_data(self, start, end):
        return self.data_stack[start: start+end]


class Dinode:
    def __init__(self, raw_dinode, datas):
        '''
               data_ptr : uint32_t
               data_len : uint32_t
            next_dinode : uint32_t
        '''
        def ret_correct_didata(raw_di, data_len, base, unpack=False, format_unpack="<i"):
            sliced_di = raw_di[base: base+data_len]
            base += len(sliced_di)
            sliced_di = (not unpack and sliced_di or struct.unpack(format_unpack, sliced_di)[0])
            return sliced_di, base

        '''
        self.data_ptr = raw_dinode[:4]
        self.data_len = raw_dinode[4:8]
        self.next_dinode = raw_dinode[8:]
        '''

        base = 0
        self.data_ptr,base = ret_correct_didata(raw_dinode, 4, base, True)
        self.data_len,base = ret_correct_didata(raw_dinode, 4, base, True)
        self.next_dinode,_ = ret_correct_didata(raw_dinode, 4, base, True)
        self.data = datas.get_data(self.data_ptr, self.data_len)

    def get_raw_data(self):
        return self.data

    def get_data_len(self):
        return self.data_len

    def get_data_address_len(self):
        return (self.data_ptr, self.data_len)

    def get_next_dinode_address(self):
        return self.next_dinode

    def get_data_ptr_pretty_str(self):
        return '0x{:08x}'.format(struct.unpack('<i', self.data_ptr)[0])

    def get_next_dinode_pretty_str(self):
        return '0x{:08x}'.format(struct.unpack('<i', self.next_dinode)[0])

    def __str__(self):
        return introspect(self)


class Inode:
    def __init__(self, raw_inode):
        '''
                     fd : uint32_t
                   name : char[16]
                   time : uint32_t
                   size : uint32_t
                is_open : uint8_t
            last_dinode : idfile_t*
            next_dinode : idfile_t*

            tot_len = 4 + 16 + 4 + 4 + 1 + 4 + 4 = 37
        '''
        def ret_correct_idata(raw_i, data_len, base, unpack=False, format_unpack="<i"):
            sliced_i = raw_i[base: base+data_len]
            base += len(sliced_i)
            sliced_i = (not unpack and sliced_i or struct.unpack(format_unpack, sliced_i)[0])
            return sliced_i, base

        '''
        base = 0
        self.fd = raw_inode[:4]
        base += len(self.fd)
        self.name = raw_inode[base:base+16]
        base += len(self.name)
        self.time = raw_inode[base:base+4]
        base += len(self.time)
        self.size = raw_inode[base:base+4]
        base += len(self.size)
        self.is_open = raw_inode[base:base+1]
        base += len(self.is_open)
        self.last_dinode = raw_inode[base:base+4]
        base += len(self.last_dinode)
        self.next_dinode = raw_inode[base:]
        '''

        base = 0
        self.fd,base = ret_correct_idata(raw_inode, 4, base, True)
        self.name,base = ret_correct_idata(raw_inode, 16, base, True, "<16s")
        self.time,base = ret_correct_idata(raw_inode, 4, base, True)
        self.size,base = ret_correct_idata(raw_inode, 4, base, True)
        self.is_open,base = ret_correct_idata(raw_inode, 1, base, True, "<B")
        self.last_dinode,base = ret_correct_idata(raw_inode, 4, base, True)
        self.next_dinode,_ = ret_correct_idata(raw_inode, 4, base, True)

        self.name = (self.name).decode().rstrip('\x00')

    def get_fd(self):
        return self.fd

    def get_filename(self):
        return self.name

    def get_first_dinode(self):
        return self.next_dinode

    def __str__(self):
        return introspect(self)

class Superblock:
    def __init__(self, raw_disk, sb_address):
        '''
		  ptr_data_address : uint32_t
		ptr_dinode_address : uint32_t
			   next_fd : uint32_t
			inode_list : ifile_t[_INODE_LIST_LIMIT] # _INODE_LIST_LIMIT == 1024
        '''
        def ret_correct_sbdata(raw_sb, data_len, base, unpack=False, format_unpack="<i"):
            sliced_sb = raw_sb[base: base+data_len]
            base += len(sliced_sb)
            sliced_sb = (not unpack and sliced_sb or struct.unpack(format_unpack, sliced_sb)[0])
            return sliced_sb, base


        '''
        base = 0
        self.ptr_data_address = raw_superblock[:4]
        base += len(self.ptr_data_address)
        self.ptr_dinode_address = raw_superblock[base:base+4]
        base += len(self.ptr_dinode_address)
        self.next_fd = raw_superblock[base:base+4]
        base += len(self.next_fd)
        self.inode_list = raw_superblock[base:]
        '''

        base = 0

        self.superblock_address = sb_address
        raw_superblock = raw_disk[sb_address:]

        self.ptr_data_address,base = ret_correct_sbdata(raw_superblock, 4, base, True)
        self.ptr_dinode_address,base = ret_correct_sbdata(raw_superblock, 4, base, True)
        self.next_fd,base = ret_correct_sbdata(raw_superblock, 4, base, True)
        self.inode_list,_ = ret_correct_sbdata(raw_superblock, inode_len * self.next_fd, base)

        
    def get_inode(self, num):
        base = num*inode_len
        return Inode(self.inode_list[base: base+inode_len])

    def get_all_inodes(self):
        inodes = []
        #for fd in range(2, self.next_fd):
        for fd in range(0, self.next_fd):
            inodes.append(self.get_inode(fd))
        return inodes

    def __str__(self):
        return introspect(self)


class DinodeStack:
    def __init__(self, raw_disk, dinode_start, dinode_end, datas):
        self.dinode_start = dinode_start
        self.dinode_end = dinode_end
        
        self.raw_di_stack = raw_disk[self.dinode_start: self.dinode_end]
        self.dict_di_stack = {}
        for di in range(0, len(self.raw_di_stack), dinode_len):
            self.dict_di_stack[self.dinode_start + di] = Dinode(self.raw_di_stack[di : di+dinode_len], datas)

    def get_next_dinode(self, di_address):
        return self.dict_di_stack[di_address]

    def get_all_file_dinode(self, first_dinode_address):
        file_dinodes = []
        
        if first_dinode_address not in self.dict_di_stack.keys():
            return []

        current_dinode = self.get_next_dinode(first_dinode_address)
        file_dinodes.append((first_dinode_address, current_dinode),)

        while current_dinode.get_next_dinode_address() != 0x0:
            next_address = current_dinode.get_next_dinode_address()
            current_dinode = self.get_next_dinode(current_dinode.get_next_dinode_address())
            file_dinodes.append((next_address, current_dinode),)

        return file_dinodes

    def __str__(self):
        return introspect(self)
        


class Memory:
    def __init__(self, raw_disk):
        self.sb = Superblock(raw_disk, superblock_address_5MB)
        self.datas = DataStack(raw_disk, self.sb.ptr_data_address)
        self.ds = DinodeStack(raw_disk, self.sb.ptr_dinode_address, self.sb.superblock_address, self.datas)

    def create_dot_graph(self, name):
        files = self.sb.get_all_inodes()
        name = name.split('.')[0]
        dot = graphviz.Digraph(comment=name+'_dot_graph')

        for f_in in files:
            #str_edge = str(f.get_fd())
            str_edge = '{0}\nfd -> {1}'.format(f_in.get_filename(), str(f_in.get_fd()))
            dot.edge('Superblock', str_edge)
            
            current_root = str_edge
            for dinode_f in self.ds.get_all_file_dinode(f_in.get_first_dinode()):
                dot.edge_attr.update(arrowhead='none')
                node = str(hex(dinode_f[0])) + ' - ' + str(dinode_f[1].get_data_len()) + 'b'

                dot.edge(current_root, node)
                current_root = node

        dot.render(name+'.gv', view=True)  


############
### Main ###
############

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print('usage: ' + sys.argv[0] + ' <file_path>.zcfs')
        sys.exit(1)

    file_zcfs = sys.argv[1]

    if os.path.exists(file_zcfs) and (file_zcfs).endswith('zcfs'):
        f = open(file_zcfs, 'rb').read()

        #sb = Superblock(f, superblock_address_5MB)
        #datas = DataStack(f, sb.ptr_data_address)
        #ds = DinodeStack(f, sb.ptr_dinode_address, sb.superblock_address, datas)
        #print(ds)
        #for file_i in range(2, sb.next_fd):
        #    print(Inode(sb.get_inode(file_i)))
        #print(ds.get_all_file_dinode(0x4f6be8))
        mem = Memory(f)

        mem.create_dot_graph(os.path.basename(file_zcfs))
    else:
        print('file not exists or incorrect format..')



'''
a = b''.join(list(map(lambda x: bytes([int(x,16)]), '22 00 00 00 12 00 00 00 ac 6b 4f 00'.split(' '))))
print(a)

d = dinode(a)
print(d)
print(d.get_data_ptr_pretty_str())
print(d.get_next_dinode_pretty_str())

ina = b'\x03\x00\x00\x00test0\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00*\x00\x00\x00)\x00\x00\x00\x01\xdckO\x00\xe4kO\x00'
inode_t = inode(ina)
print(inode_t)
'''

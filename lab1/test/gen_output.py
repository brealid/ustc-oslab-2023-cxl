data = '<oslab> helloworld_PB21000033_zhaoyi'
assert len(data) % 2 == 0

color = 0x09
col_num = color * 0x01000100

for i in range(0, len(data), 2):
    num = col_num + (ord(data[i + 1]) << 16 | ord(data[i]))
    pos = 0xB8000 + i * 2
    print(f'movl $0x{hex(num)[2:].zfill(8)}, {hex(pos)}')


'''
movl $0x01420150, 0xb8000
movl $0x01310132, 0xb8004
movl $0x01300130, 0xb8008
movl $0x01300130, 0xb800c
movl $0x01330133, 0xb8010
movl $0x017a015f, 0xb8014
movl $0x01610168, 0xb8018
movl $0x0179016f, 0xb801c
movl $0x010a0169, 0xb8020
'''
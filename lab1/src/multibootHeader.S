.globl start

MAGIC = 0x1BADB002              # 必须为 0x1BADB002
FLAGS = 0                       # 无其他功能需要, flags 为 0
CHECKSUM = -(MAGIC + FLAGS)     # magic + checksum + flags 的结果必须是 32 位无符号整数的 0

.section ".multiboot_header_brealid"
    .align 4                    # 按每 4 个字节对齐
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.text
.code32
start:
    # VGA 输出
    movl $0x096f093c, 0xb8000
    movl $0x096c0973, 0xb8004
    movl $0x09620961, 0xb8008
    movl $0x0920093e, 0xb800c
    movl $0x09650968, 0xb8010
    movl $0x096c096c, 0xb8014
    movl $0x0977096f, 0xb8018
    movl $0x0972096f, 0xb801c
    movl $0x0964096c, 0xb8020
    movl $0x0950095f, 0xb8024
    movl $0x09320942, 0xb8028
    movl $0x09300931, 0xb802c
    movl $0x09300930, 0xb8030
    movl $0x09330930, 0xb8034
    movl $0x095f0933, 0xb8038
    movl $0x0968097a, 0xb803c
    movl $0x096f0961, 0xb8040
    movl $0x09690979, 0xb8044

    # 串口输出
    movw $0x3F8, %dx    # set up output port

    movb $0x7a, %al     # char:z
    outb %al, %dx
    movb $0x79, %al     # char:y
    outb %al, %dx
    movb $0x0a, %al     # char:\n
    outb %al, %dx
    hlt                 # 结束
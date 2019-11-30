target remote 10.10.114.70:2331
set mem inaccessible-by-default off
monitor speed auto
monitor endian little
monitor reset
monitor flash device = S32K144
monitor flash breakpoints = 1
monitor flash download = 1
load
monitor reg sp = (0x00000000)
monitor reg pc = (0x00000004)
#break ResetHandler
break main
continue
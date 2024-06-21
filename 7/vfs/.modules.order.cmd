cmd_/home/hieubich/6-SEM-OS/vfs/modules.order := {   echo /home/hieubich/6-SEM-OS/vfs/main.ko; :; } | awk '!x[$$0]++' - > /home/hieubich/6-SEM-OS/vfs/modules.order

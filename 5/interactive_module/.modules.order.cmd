cmd_/home/hieubich/6-SEM-OS/load-module/p2/modules.order := {   echo /home/hieubich/6-SEM-OS/load-module/p2/md1.ko;   echo /home/hieubich/6-SEM-OS/load-module/p2/md2.ko;   echo /home/hieubich/6-SEM-OS/load-module/p2/md3.ko; :; } | awk '!x[$$0]++' - > /home/hieubich/6-SEM-OS/load-module/p2/modules.order

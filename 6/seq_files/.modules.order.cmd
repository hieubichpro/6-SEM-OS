cmd_/home/hieubich/6-SEM-OS/seq_files/modules.order := {   echo /home/hieubich/6-SEM-OS/seq_files/single_file.ko;   echo /home/hieubich/6-SEM-OS/seq_files/seq_file.ko; :; } | awk '!x[$$0]++' - > /home/hieubich/6-SEM-OS/seq_files/modules.order
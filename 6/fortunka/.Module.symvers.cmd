cmd_/home/hieubich/6-SEM-OS/Fortunka/Module.symvers := sed 's/\.ko$$/\.o/' /home/hieubich/6-SEM-OS/Fortunka/modules.order | scripts/mod/modpost -m -a  -o /home/hieubich/6-SEM-OS/Fortunka/Module.symvers -e -i Module.symvers   -T -
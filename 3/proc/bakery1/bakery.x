struct BAKERY
{
    int num;
    int pid;
    char res;
};

program BAKERY_PROG
{
    version BAKERY_VER
    {
        struct BAKERY GET_NUMBER(struct BAKERY) = 1; 
        struct BAKERY BAKERY_WAIT(struct BAKERY) = 2;
        struct BAKERY BAKERY_SERVICE(struct BAKERY) = 3;
    } = 1;
} = 0x20000001;


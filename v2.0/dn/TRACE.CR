-----   Samedi   12 Octobre   2019  19:57:03
-----   Fichier var : rpn.var
-----   Fichier mis : rpn.mis
-----   Fichier v   : rpn.v
-----   Reference   : /home/machine/src/SCCS/MB

-----   make -f dn_makefile -i VFILE=rpn.v LIB=lib
        MIS=/home/machine/src/SCCS/MB MAKE=/usr/bin/make AR=/usr/bin/ar
        LINT=/bin/lint LIB=lib RANLIB=/usr/bin/ranlib TEE=/usr/bin/tee CC=gcc
        CFLAGS=-Wall -Werror -Wuninitialized BE_C=-Wall -Werror -Wuninitialized
SERIAL = 142
gcc -Wall -Werror -Wuninitialized -DSERIAL=`cat serial` -shared -fPIC -o libstats.so dn_mod_stats.c
cp libstats.so ../rpn_modules
-----   Creation d'objets terminee.
-----   Samedi   12 Octobre   2019  19:57:03

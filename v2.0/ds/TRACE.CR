-----   Dimanche 13 Octobre   2019  16:24:58
-----   Fichier var : rpn.var
-----   Fichier mis : rpn.mis
-----   Fichier v   : rpn.v
-----   Reference   : /home/machine/src/SCCS/MB

-----   make -f ds_makefile -i VFILE=rpn.v LIB=lib
        MIS=/home/machine/src/SCCS/MB MAKE=/usr/bin/make AR=/usr/bin/ar
        LINT=/bin/lint LIB=lib RANLIB=/usr/bin/ranlib TEE=/usr/bin/tee CC=gcc
        CFLAGS=-Wall -Werror -Wuninitialized BE_C=-Wall -Werror -Wuninitialized
SERIAL = 203
gcc -Wall -Werror -Wuninitialized -lm -DSERIAL=`cat serial` -shared -fPIC -o libcore.so ds_mod_core.c
cp libcore.so ../rpn_modules
-----   Creation d'objets terminee.
-----   Dimanche 13 Octobre   2019  16:24:58

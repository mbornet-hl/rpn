-----   Jeudi    17 Octobre   2019  17:26:19
-----   Fichier var : rpn.var
-----   Fichier mis : rpn.mis
-----   Fichier v   : rpn.v
-----   Reference   : /home/machine/src/SCCS/MB

-----   make -f dt_makefile -i VFILE=rpn.v LIB=lib
        MIS=/home/machine/src/SCCS/MB MAKE=/usr/bin/make AR=/usr/bin/ar
        LINT=/bin/lint LIB=lib RANLIB=/usr/bin/ranlib TEE=/usr/bin/tee CC=gcc
        CFLAGS=-Wall -Werror -Wuninitialized BE_C=-Wall -Werror -Wuninitialized
SERIAL = 426
gcc -Wall -Werror -Wuninitialized -lm -DSERIAL=`cat serial` -shared -fPIC -o libmath.so dt_mod_math.c
cp libmath.so ../rpn_modules
-----   Creation d'objets terminee.
-----   Jeudi    17 Octobre   2019  17:26:20

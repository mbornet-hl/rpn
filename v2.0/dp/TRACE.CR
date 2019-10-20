-----   Jeudi    17 Octobre   2019  22:10:47
-----   Fichier var : rpn.var
-----   Fichier mis : rpn.mis
-----   Fichier v   : rpn.v
-----   Reference   : /home/machine/src/SCCS/MB

-----   make -f dp_makefile -i VFILE=rpn.v LIB=lib
        MIS=/home/machine/src/SCCS/MB MAKE=/usr/bin/make AR=/usr/bin/ar
        LINT=/bin/lint LIB=lib RANLIB=/usr/bin/ranlib TEE=/usr/bin/tee CC=gcc
        CFLAGS=-Wall -Werror -Wuninitialized BE_C=-Wall -Werror -Wuninitialized
SERIAL = 157
gcc -Wall -Werror -Wuninitialized -DSERIAL=`cat serial` -shared -fPIC -o libai.so dp_mod_ai.c ../df/lib
cp libai.so ../rpn_modules
-----   Creation d'objets terminee.
-----   Jeudi    17 Octobre   2019  22:10:47

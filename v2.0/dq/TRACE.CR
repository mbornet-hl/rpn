-----   Mercredi  9 Octobre   2019  16:43:45
-----   Fichier var : rpn.var
-----   Fichier mis : rpn.mis
-----   Fichier v   : rpn.v
-----   Reference   : /home/machine/src/SCCS/MB

-----   make -f dq_makefile -i VFILE=rpn.v LIB=lib
        MIS=/home/machine/src/SCCS/MB MAKE=/usr/bin/make AR=/usr/bin/ar
        LINT=/bin/lint LIB=lib RANLIB=/usr/bin/ranlib TEE=/usr/bin/tee CC=gcc
        CFLAGS=-Wall -Werror -Wuninitialized BE_C=-Wall -Werror -Wuninitialized
SERIAL = 135
gcc  -DSERIAL=`cat serial` -shared -fPIC -o libjson.so dq_mod_json.c
cp libjson.so ../rpn_modules
-----   Creation d'objets terminee.
-----   Mercredi  9 Octobre   2019  16:43:45

-include ../../fuentes.mk

CC := gcc

ARCHIVOS :=  socket.o Nodo.o atenderJob.o atenderNodoYFS.o funcionesDepuracion.o funcionesParaConfiguracionYDemas.o funcionesParaElCombiner.o funcionesParaEnviar.o funcionesParaEscribir.o manejoDeMemoria.o rutinasMapperYReducer.o
NOMBREFINAL := kernel
LIBRERIAS := -lcommons -lpthread

instalar: $(ARCHIVOS)
	$(CC) -o $(NOMBREFINAL) $(ARCHIVOS) -I$(LIBH4S) -I$(LIBCOMM) -I$(LIBAS) -L$(LIBH4SEXEC) -L$(LIBCOMMEXEC) -L$(LIBASEXEC) $(LIBRERIAS)

%.o:%.c
	$(CC) -c "$<" -I$(LIBH4S) -I$(LIBCOMM) -o "$@" $(LIBRERIAS)

desinstalar:
	rm -rf $(NOMBREFINAL) $(ARCHIVOS) 

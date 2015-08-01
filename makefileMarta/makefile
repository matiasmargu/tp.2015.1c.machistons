-include ../../fuentes.mk

CC := gcc

ARCHIVOS :=  socket.o Marta.o pedidos_fs.o pedidos_job.o planificacion_map.o planificar_reduce.o
NOMBREFINAL := marta
LIBRERIAS := -lpthread $(pwd)funciones/Debug/libcommons.so

instalar: $(ARCHIVOS)
	$(CC) -o $(NOMBREFINAL) $(ARCHIVOS) -I$(LIBH4S) -I$(LIBCOMM) -I$(LIBAS) -L$(LIBH4SEXEC) -L$(LIBCOMMEXEC) -L$(LIBASEXEC) $(LIBRERIAS)

%.o:%.c
	$(CC) -c "$<" -I$(LIBH4S) -I$(LIBCOMM) -o "$@" $(LIBRERIAS)

desinstalar:
	rm -rf $(NOMBREFINAL) $(ARCHIVOS) 

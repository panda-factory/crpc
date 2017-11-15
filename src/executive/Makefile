SOURCES_FILE = \
			   /home/crpc/src/utils/buffer.c \
			   /home/crpc/src/utils/list.c \
			   /home/crpc/src/utils/log.c \
			   /home/crpc/src/utils/hash.c \
			   /home/crpc/src/utils/tlv.c \
			   /home/crpc/src/crpc/crpc_server.c 


INCLUDE_DIR = \
			  -I/home/crpc/src/includes

exe:
	gcc -g -o $@ crpcd.c $(SOURCES_FILE) $(INCLUDE_DIR)

clean:
	-rm exe 

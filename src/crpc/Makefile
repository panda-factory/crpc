SOURCES_FILE = \
			   /home/panda/src/utils/panda_buffer.c \
			   /home/panda/src/utils/panda_list.c \
			   /home/panda/src/utils/panda_log.c \
			   /home/panda/src/utils/panda_hash.c \
			   /home/panda/src/utils/panda_tlv.c 


INCLUDE_DIR = \
			  -I/home/panda/src/includes

srv:
	gcc -g -o $@ crpc_server.c $(SOURCES_FILE) $(INCLUDE_DIR)

cli:
	gcc -g -o $@ panda_client.c $(SOURCES_FILE) $(INCLUDE_DIR)

clean:
	-rm srv cli 

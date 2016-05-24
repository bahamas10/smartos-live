NICTAG_PATH = ../common/nictag

CFLAGS = -Wall
LIBS = -lnvpair -lnictag -ldladm

nictagadm: nictagadm.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

nictagadm-local: nictagadm.c
	(cd $(NICTAG_PATH) && make)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) \
		-L$(NICTAG_PATH) \
		-I$(NICTAG_PATH) \
		-Wl,-rpath=$(NICTAG_PATH) \

.PHONY: local
local: nictagadm-local

.PHONY: clean
clean:
	rm -f nictagadm

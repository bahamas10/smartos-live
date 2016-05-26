NICTAG_PATH = ../common/nictag

LIBS = -lnvpair \
       -ldevinfo \
       -lsmbios \
       -lzfs \
       -lkstat \
       -ldladm \
       -lsocket \
       -lnsl \
       -lcmdutils \
       -lscf \
       -lnictag

CFLAGS = -Wall

DEPS_OBJ = sysinfo_bootparams.o \
	   sysinfo_uname.o \
	   sysinfo_smartdc.o \
	   sysinfo_smbios.o \
	   sysinfo_uptime.o \
	   sysinfo_sysconf.o \
	   sysinfo_zfs.o \
	   sysinfo_disks.o \
	   sysinfo_network.o \
	   sysinfo_kstat.o \
	   utils.o

sysinfo: sysinfo.c $(DEPS_OBJ) ../dockerinit/json-nvlist/json-nvlist.c
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

.PHONY: local
local: CFLAGS += -L$(NICTAG_PATH)
local: CFLAGS += -I$(NICTAG_PATH)
local: CFLAGS += -Wl,-rpath=$(NICTAG_PATH)
local: sysinfo-local
sysinfo-local: sysinfo.c $(DEPS_OBJ) ../dockerinit/json-nvlist/json-nvlist.c
	(cd $(NICTAG_PATH) && make)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)


.PHONY: clean
clean:
	rm -f sysinfo sysinfo-local *.o

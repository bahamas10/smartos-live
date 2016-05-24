#include <err.h>
#include <sys/utsname.h>
#include <string.h>
#include <unistd.h>

#include <libnvpair.h>

void sysinfo_uname(nvlist_t *root_nvl) {
	struct utsname buf;
	char *image;

	if (uname(&buf) == -1) {
		warn("uname(2)");
		return;
	}

	nvlist_add_string(root_nvl, "System Type", buf.sysname);
	nvlist_add_string(root_nvl, "Hostname", buf.nodename);

	(void) strtok(buf.version, "_");
	image = strtok(NULL, "_");
	nvlist_add_string(root_nvl, "Live Image", image);
}

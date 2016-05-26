#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <libnvpair.h>
#include <zone.h>

#include "utils.h"

void sysinfo_smartdc(nvlist_t *root_nvl) {
	FILE *f;
	char version[256];
	boolean_t smartdc_version_found = B_FALSE;
        static const char *agents_dir = "/opt/smartdc/agents/lib/node_modules";
	nvlist_t *setup_nvl;
	boolean_t smartdc_setup = B_FALSE;

	/* sdc setup completion status */
	setup_nvl = read_json_file("/var/lib/setup.json");
	if (setup_nvl != NULL) {
		if (nvlist_lookup_boolean_value(setup_nvl, "setup", &smartdc_setup) != 0)
			warnx("can't find .setup in /var/lib/setup.json");
		nvlist_free(setup_nvl);
	}

	nvlist_add_boolean_value(root_nvl, "Setup", smartdc_setup);
	/* smartdc version */
	f = fopen("/.smartdc_version", "r");
	if (f != NULL) {
		smartdc_version_found = B_TRUE;
		if (fscanf(f, "%s", version) == 1)
			nvlist_add_string(root_nvl, "SDC Version", version);
		fclose(f);
	}

	/* SDC agents */
        if (getzoneid() == GLOBAL_ZONEID &&
		smartdc_version_found) {
                DIR *d;
                struct dirent *dp;
		char package_file[MAXPATHLEN + 1];
		nvlist_t *versions[1024];
		int i, j;

                d = opendir(agents_dir);
                if (d == NULL) {
			if (errno == ENOENT) {
				// ignore this
				return;
			}
                        warn("failed to open %s", agents_dir);
                        return;
                }

		i = 0;
                while ((dp = readdir(d)) != NULL) {
			char *name, *version;
			nvlist_t *nvl = NULL;
			nvlist_t *new_nvl = NULL;

                        if (dp->d_name[0] == '.')
				goto next;

			snprintf(package_file, MAXPATHLEN, "%s/%s/package.json",
			    agents_dir, dp->d_name);
			package_file[MAXPATHLEN] = '\0';

			nvl = read_json_file(package_file);
			if (nvl == NULL) {
				warn("read_json_file failure: %s", package_file);
				goto next;
			}

			if (nvlist_lookup_string(nvl, "name", &name) != 0 ||
			    nvlist_lookup_string(nvl, "version", &version) != 0) {
				warnx("nvlist_lookup_string failure: %s", package_file);
				goto next;
			}

			if (nvlist_alloc(&new_nvl, NV_UNIQUE_NAME, 0) != 0) {
				warn("nvlist_alloc");
				goto next;
			}
			if (nvlist_add_string(new_nvl, "name", name) != 0 ||
			    nvlist_add_string(new_nvl, "version", version)) {
				warn("nvlist_add_string");
				goto next;
			}

			versions[i++] = new_nvl;

next:
			nvlist_free(nvl);
                }
                closedir(d);

		nvlist_add_nvlist_array(root_nvl, "SDC Agents", versions, i);

		for (j = 0; j < i; j++)
			nvlist_free(versions[j]);
        }
}

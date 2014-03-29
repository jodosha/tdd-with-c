#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

#include "test.h"

int main(int argc, char **argv) {
  DIR *d;
  struct dirent *dir;
  d = opendir(argv[1]);

  if (d) {
    /* look for all the files in the given directory */
    while((dir = readdir(d)) != NULL) {
      char *dylib = strstr(dir->d_name, ".so");

      /* only consider dynamic libraries */
      if (dylib) {
        char path[256] = "";
        char nm[512]   = "nm -Uj ";

        strcat(path, argv[1]);
        strcat(path, "/");
        strcat(path, dir->d_name);

        strcat(nm, path);

        /* read the symbols with `nm` */
        FILE *tests = popen(nm, "r");

        if (!tests) {
          printf("Error while reading functions from: %s", path);
        }

        char buffer[512];
        char *line;

        void *handle;
        char *error;
        int (*function)();

        /* open the dynamic library */
        handle = dlopen(path, RTLD_LAZY);

        if (!handle) {
          fprintf(stderr, "%s\n", dlerror());
          exit(EXIT_FAILURE);
        }

        dlerror(); /* cleanup dl errors */

        /* for each test candidate */
        while((line = fgets(buffer, sizeof(buffer), tests)) != NULL) {
          char *test = strstr(line, "_test_");

          /* if the function name starts with `test_` */
          if (test) {
            /* remove the prefix underscore from `nm` output and the trailing
             * carriage return */
            char *test_name[512];
            memcpy(test_name, &line[1], strlen(line));
            char *ptr = strchr((char*)test_name, '\n');
            if(ptr) *ptr = '\0';

            /* get the reference from the dynamic lib into `function` */
            *(void **) (&function) = dlsym(handle, (char*)test_name);

            if ((error = dlerror()) != NULL)  {
              fprintf(stderr, "%s\n", error);
              exit(EXIT_FAILURE);
            }

            /* run and verify the test */
            _verify(function);
          }
        }

        dlclose(handle);
        pclose(tests);
      }
    }

    closedir(d);
  }

  _summary();
  return total_failures != 0;
}

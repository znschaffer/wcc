#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <zip.h>

int main(int argc, char *argv[]) {
  int bytes = 0;
  int words = 0;
  int lines = 0;
  char buffer[1];
  enum states { WHITESPACE, WORD };
  int state;

  zip_t *zt = zip_open("./test.zip", ZIP_CREATE, NULL);
  zip_file_add(zt, "myfile.txt",
               zip_source_buffer(zt, "test text in here",
                                 sizeof("test text in here"), 0),
               ZIP_FL_ENC_UTF_8);
  zip_close(zt);

  if (argc < 2) {
    printf("usage: wcc file.txt\n");
    return 1;
  }

  int file = open(argv[1], O_RDONLY);
  if (file == -1) {
    printf("can not find '%s'\n", argv[1]);
    return 1;
  } else {
    while (read(file, buffer, 1) == 1) {
      bytes++;
      if (buffer[0] == ' ' || buffer[0] == '\t') {
        state = WHITESPACE;
      } else if (buffer[0] == '\n') {
        lines++;
        state = WHITESPACE;
      } else {
        if (state == WHITESPACE) {
          words++;
        }
        state = WORD;
      }
    }

    if (argc == 3 && argv[2][0] == '-') {
      switch (argv[2][1]) {
        case 'l':
          printf("%8d %s\n", lines, argv[1]);
          break;
        case 'w':
          printf("%8d %s\n", words, argv[1]);
          break;
        case 'c':
          printf("%8d %s\n", bytes, argv[1]);
          break;
        default:
          printf("unknown flag '%s'\n", argv[2]);
          break;
      }
    } else {
      printf("%8d%8d%8d %s\n", lines, words, bytes, argv[1]);
    }
  }
}

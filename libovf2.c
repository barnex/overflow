#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "libovf2.h"


#define LINEBUF 2047 // maximum header line length

void panic(const char *msg) {
    fprintf(stderr, "panic: %s\n", msg);
    abort();
}

bool streq(const char *a, const char *b) {
    if(a==NULL || b==NULL) {
        panic("streq: NULL input");
    }
    return (strcmp(a, b) == 0);
}

bool hasprefix(const char *s, const char *prefix) {
    if(s==NULL || prefix==NULL) {
        panic("hasprefix: NULL input");
    }
    return (strstr(s, prefix) == s);
}

void strtolower(char *s) {
    for(int i=0; s[i] != 0; i++) {
        s[i] = tolower(s[i]);
    }
}

void efputs(const char *str, FILE *stream) {
    int n = fputs(str, stream);
    if (n < 0) {
        fprintf(stderr, "fputs(\"%s, %p\"): error %d\n", str, stream, n);
        abort();
    }
}

void efread(void* ptr, size_t size, size_t count, FILE *stream) {
    int ret = fread(ptr, size, count, stream);
    if (ret < count) {
        panic("fread");
    }
}


void ovf2_gets(char *line, FILE* in) {
    fgets(line, LINEBUF, in);
    strtolower(line);
}

int ovf2_datalen(ovf2_data data) {
    return data.valuedim * data.xnodes * data.ynodes * data.znodes;
}

ovf2_data ovf2_read(FILE* in) {
ovf2_data data = {err:NULL};
    char line[LINEBUF+1] = {}; // init to 0s

    // read header
    for (ovf2_gets(line, in); !hasprefix(line, "# begin: data"); ovf2_gets(line, in)) {
        // TODO: strip extra ## comments

        if(hasprefix(line, "# valuedim:")) {
            data.valuedim = atoi(&line[11]);
            continue;
        }
        if(hasprefix(line, "# xnodes:")) {
            data.xnodes = atoi(&line[9]);
            continue;
        }
        if(hasprefix(line, "# ynodes:")) {
            data.ynodes = atoi(&line[9]);
            continue;
        }
        if(hasprefix(line, "# znodes:")) {
            data.znodes = atoi(&line[9]);
            continue;
        }
    }

    //if (!streq(line, "# begin: data binary 4")){
    //	panic(line);
    //}

	// control number
	float control = 0;
    efread(&control, sizeof(float), 1, in);
	if (control != OVF2_CONTROL_NUMBER){
		panic("invalid ovf control number");
	}

    size_t nfloat = ovf2_datalen(data);
    data.data = (float*)malloc(nfloat * sizeof(float));
    efread(data.data, sizeof(float), nfloat, in);

    ovf2_gets(line, in);
    //if (!streq(line, "# end: data")) {
    //    panic(line);
    //}

    return data;
}


ovf2_data ovf2_readfile(const char *filename) {
    FILE *in = fopen(filename, "r");
    if(in == NULL) {
        fprintf(stderr, "ovf2_readfile: failed to open %s: errno %d\n", filename, errno);
        abort();
    }

    ovf2_data data = ovf2_read(in);
    fclose(in);
    return data;
}


float ovf2_get(ovf2_data *data, int c, int x, int y, int z) {
    int Nx = data->xnodes;
    int Ny = data->ynodes;
    int Nz = data->znodes;
    int Nc = data->valuedim;

    assert(x >= 0 && x < Nx &&
           y >= 0 && y < Ny &&
           z >= 0 && z < Nz &&
           c >= 0 && c < Nc);

    return data->data[((c*Nz+z)*Ny + y)*Nx + x];
}


void ovf2_write(FILE* out, ovf2_data data) {
    if(data.err != NULL) {
        efputs(data.err, out);
        return;
    }

    efputs("# OOMMF OVF 2.0\n", out);
    efputs("# Segment count: 1\n", out);
    efputs("# Begin: Segment\n", out);
    efputs("# Begin: Header\n", out);
    fprintf(out, "# valuedim: %d\n", data.valuedim); // TODO: e
    fprintf(out, "# xnodes: %d\n", data.xnodes); // TODO: e
    fprintf(out, "# ynodes: %d\n", data.ynodes); // TODO: e
    fprintf(out, "# znodes: %d\n", data.znodes); // TODO: e

    for(int z=0; z<data.znodes; z++) {
    	for(int y=0; y<data.ynodes; y++) {
    		for(int x=0; x<data.xnodes; x++) {
    			for(int c=0; c<data.valuedim; c++) {
                    float v = ovf2_get(&data, c, x, y, z);
                    fprintf(out, "%f ", v);
                }
            }
            efputs("\n", out);
        }
        efputs("\n", out);
    }
}

void ovf2_writeffile(const char *filename, ovf2_data data) {
    FILE *out = fopen(filename, "w+");

    if(out == NULL) {
        fprintf(stderr, "ovf2_writefile: failed to open %s: errno %d\n", filename, errno);
        abort();
    }

    fclose(out);
}



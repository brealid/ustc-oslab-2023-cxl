int strLength(char *str){
    int n=0;
    while(*str++) n++;
    return n;
}

int strcpy(char *src, char *dst){
    int n=0;
    while(*src) {
        *dst++ = *src++;
        n++;
    }
    return n;
}

int strncpy(const char *src, char *dst, unsigned int n){
    int nn=0;
    while(*src) {
        *dst++ = *src++;
        nn++;
	    if (nn >= n) break;
    }
    return nn;
}

int strcmp(const char *str1, const char *str2){
    char c1, c2;
    do {
        c1 = *str1++;
        c2 = *str2++;
        if (c1 != c2) return (c1 > c2)?1:-1;
        if (c1 == '\0') break;
    } while(1);
    return 0;  //equal    
}

/*
int strncmp(char *str1, char *str2, unsigned int n){
    char c1, c2;    

    while(n){
        c1 = *str1++;
        c2 = *str2++;        
        if (c1 != c2) return (c1 > c2)?1:-1;
        if (c1 == '\0') break;
    };
    return 0;
}*/

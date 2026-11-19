#include<iostream>
namespace FastIO {
    constexpr int LEN = 1 << 20;
    inline char getchar() {
        static char buf[LEN], *l = buf, *r = buf;
        if (l == r) r = (l = buf) + fread(buf, 1, LEN, stdin);
        return l == r ? EOF : *l++;
    }
    char obuf[LEN], *p = obuf;
    inline void putchar(const char &c) {
        if (p == obuf + LEN) fwrite(p = obuf, 1, LEN, stdout);
        *p++ = c;
    }
    inline void flush() {
        fwrite(obuf, 1, p - obuf, stdout),p=obuf;
    }
    struct Reader {
        template<typename T> inline Reader &operator >> (T &x) {
            x = 0;
            short f = 1;
            char c = getchar();
            while (!isdigit(c)) {
                if (c == '-') f = -1;
                c = getchar();
            }
            while (isdigit(c)) x = 10 * x + (c - '0'), c = getchar();
            x *= f;
            return *this;
        }
        inline Reader &operator >> (std::string &s) {
            s.clear();
            char c = getchar();
            while (c != EOF && c <= 32) c = getchar();
            while (c > 32) {
                s += c;
                c = getchar();
            }
            return *this;
        }
    } cin;
    struct Keep{int len;};
    Keep keep(int l){return {l};}
    struct Writer {
        int keeplen=6;
        template<typename T> inline Writer &operator << (T x) {
            if (x < 0) putchar('-'), x = -x;
            static unsigned char tp = 0, s[40];
            do s[++tp] = x % 10 + '0', x /= 10; while (x);
            while (tp) putchar(s[tp--]);
            return *this;
        }
        inline Writer &operator << (const char *s) {
            int i = 0;
            while (s[i]) putchar(s[i++]);
            return *this;
        }
        inline Writer &operator << (char *s) {
            int i = 0;
            while (s[i]) putchar(s[i++]);
            return *this;
        }
        inline Writer &operator << (const char &c) {
            putchar(c);
            return *this;
        }
        inline Writer &operator << (const std::string &s) {
            return (*this) << s.c_str();
        }
        inline Writer &operator << (const Keep &k) {
            keeplen=k.len;
            return *this;
        }
        inline Writer &operator << (double d) {
            static char s[0x180];
            static short len;
            len=snprintf(s, sizeof(s), "%.*f", keeplen, d);
            if(len>0&&len<0x180)
            {
                if(p-obuf+len>=LEN)flush();
                memcpy(p,s,len),p+=len;
            }
            return *this;
        }

        ~Writer() {
            flush();
        }
    }cout;
}
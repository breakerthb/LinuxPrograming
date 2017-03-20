# 1. 口令文件

    /etc/passwd

    root:x:0:0:root:/root:/bin/bash
    squid:x:23:23::/var/spool/squid:/dev/null
    nobody:x:65534:65534:Nobody:/home:/bin/sh
    sar:x:205:105:Stephen Rago:/home/sar:/bin/bash

通过命令查看用户信息：

    $ finger -p sar
    Login: sar Name: Steve Rago
    Directory: /home/sar Shell: /bin/sh
    Office: SF 5-121, 555-1111 Home Phone: 555-2222
    On since Mon Jan 19 03:57 (EST) on ttyv0 (messages off)
    No Mail.

通过函数查看用户信息：

    #include <pwd.h>
    struct passwd *getpwuid(uid_t uid);
    struct passwd *getpwnam(const char *name);
    
如果要查看整个文件的记录项：

    #include <pwd.h>
    struct passwd *getpwent(void);
    //Returns: pointer if OK, NULL on error or end of file

    void setpwent(void);
    void endpwent(void);

# 2. 阴影口令

    #include <shadow.h>
    struct spwd *getspnam(const char *name);
    struct spwd *getspent(void);
    // Both return: pointer if OK, NULL on error
    void setspent(void);
    void endspent(void);

# 3. 组文件

    #include <grp.h>
    struct group *getgrgid(gid_t gid);
    struct group *getgrnam(const char *name);

    #include <grp.h>
    struct group *getgrent(void);
    // Returns: pointer if OK, NULL on error or end of file
    void setgrent(void);
    void endgrent(void);

# 4. 附属组ID

    #include <unistd.h>
    int getgroups(int gidsetsize, gid_t grouplist[]);
    // Returns: number of supplementary group IDs if OK, −1 on error
    #include <grp.h> /* on Linux */
    #include <unistd.h> /* on FreeBSD, Mac OS X, and Solaris */
    int setgroups(int ngroups, const gid_t grouplist[]);
    #include <grp.h> /* on Linux and Solaris */
    #include <unistd.h> /* on FreeBSD and Mac OS X */
    int initgroups(const char *username, gid_t basegid);
    // Both return: 0 if OK, −1 on error

# 5. 实现区别

### Ref : 148

# 6. 其他数据文件

一般情况，对每个数据文件至少有3个函数：

- get()
- set()
- end()

![6-6](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/6-6.png)

# 7. 登录账户记录

    struct utmp {
        char ut_line[8]; /* tty line: "ttyh0", "ttyd0", "ttyp0", ... */
        char ut_name[8]; /* login name */
        long ut_time; /* seconds since Epoch */
    };

# 8. 系统标识

    #include <sys/utsname.h>
    int uname(struct utsname *name);
    // Returns: non-negative value if OK, −1 on error

    struct utsname {
        char sysname[]; /* name of the operating system */
        char nodename[]; /* name of this node */
        char release[]; /* current release of operating system */
        char version[]; /* current version of this release */
        char machine[]; /* name of hardware type */
    };

历史上，有只返回主机名的接口：

    #include <unistd.h>
    int gethostname(char *name, int namelen);
    // Returns: 0 if OK, −1 on error

# 9. 时间和日期

ref : <https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/Time.md>








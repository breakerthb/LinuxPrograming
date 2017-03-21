# strtok()|strtok_s()

strtok 和 strtok_s 都是用于 char 拆分字符串的. 

    char_to_cstring(char *str); 函数进行转换.

# strtok()

    char *strtok(char *strToken, const char *strDelimit);

## 参数

- strToken

需要分割的字符串

- strDelimit

分隔符集合

## 返回值

得到的第一个子串。

    char t[] = "a,b,c,de";
    const char *split = ",";
    char *p = strtok(t,split);
    CString R = L"";
    while(p != NULL){
        R += char_to_cstring(p);
        R += L"\r\n";
        R += L"-----------------";
        R += L"\r\n";
        p = strtok(NULL, split);
    }
    this->MessageBox(R);

## strtok_s()

    char *strtok_s(char *strToken, const char *strDelimit, char **buf);

仅在windows下VS中使用。

## 参数

- strToken

需要分割的字符串

- strDelimit

分割符集合

- buf

返回分割出的第一个子串。由于空间在函数内申请，因此传入指针的 地址。

## 返回值

第一个子串

注意：第二次调用时strToken传入NULL，分割上一次分割的剩余字串

    char t[] = "a,b,c,de";
    const char *split = ",";
    char *p = NULL, *pNext = NULL;
    p = strtok_s(t,split,&pNext);
    CString R = L"";
    while(p!=NULL){
        R += char_to_cstring(p);
        R += L"\r\n";
        R += L"-----------------";
        R += L"\r\n";
        p = strtok_s(NULL,split,&pNext);
    }
    this->MessageBox(R);
    
## Demo

通过strtok_s()统计输入参数中每个单词出现的次数。

ref : <https://github.com/breakerthb/LinuxPrograming/blob/master/CStandard/Demo/strtok_s.c>
//client

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
char *name;
void display_file()
{
    int file = open(name,O_RDONLY);
    char c[100];
    printf("Displaying the file contents\n");
    while(1)
    {
        
        int t = read(file,c,100);
        printf("%s",c);
        if(t<100)
            break;
    }
    printf("\nExiting...\n");
    exit(0);
}
void auth_fail()
{
    printf("Authentication failed!\nExiting...\n");
    exit(0);
}
int main(int argc,char* argv[])
{
    signal(SIGUSR1,display_file);
    signal(SIGUSR2,auth_fail);
    int wrt = open("fifo",O_WRONLY);
    printf("Enter the file path you want to access\n");
    char path[20];
    scanf("%s",path);
    name=path;
    char *pwd = strdup(getpass("Password: ")); 
    char str[40];
    int pid = getpid(),temp=pid,n=1;
    while(temp>1)
    {
        temp/=10;
        if(temp)
        n*=10;
    }
    
    int i=0;
    for(;n>0;i++)
    {
        str[i]= '0' + pid/n;
        pid%=n;
        n/=10;
    }
    str[i++]=' ';
    for(int j=0;j<strlen(path);j++)
    {
        str[i+j]=path[j];
    }
    i+=strlen(path);
    str[i++]=' ';
    char *a = getlogin();
    for(int j=0;j<strlen(a);j++)
    {
        str[i+j]=a[j];
    }
    i+=strlen(a);
    str[i++]=' ';
    
    for(int j=0;j<strlen(pwd);j++)
    {
        str[i+j]=pwd[j];
    }
    // printf("%s\n",str);
    write(wrt,str,strlen(str));
    while(1)
    {

    }
}

//login

#include<stdio.h>
#include<shadow.h>
#include<crypt.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/stat.h>

char* get_salt(char* shadow_entry)
{
    // Extracting salt from the shadow entry
    int count=0,j=0;
    char* salt = (char*)malloc(sizeof(char)*50);
    while(j<strlen(shadow_entry))
    {
        if(shadow_entry[j]=='$')
        {
            count++;
        }
        salt[j]=shadow_entry[j];
        j++;
        if(count==4)
        break;
    }
    return salt;
}
int authn(char *usr,char* pwd)
{
    char s[100];
    struct spwd *a=NULL;
    a = getspnam(usr);
    if(a!=NULL)
    {
        char *salt = get_salt(a->sp_pwdp);
        return strcmp(a->sp_pwdp,crypt(pwd,salt));
    }
    else
    {
        printf("Shadow entry not found!\n");
    }
    return -1;
}
int rdr;
char buff[50];
void sig_hand()
{
    int pid=0;
    int j=0;
    while(buff[j]!=' ')
    {
        pid+=buff[j]-'0';
        pid*=10;
        j++;
    }
    pid/=10;
    j++;
    //printf("Entry recieved: %s\n",buff);
    char *p_ = strtok(buff," ");
    char *file = strtok(NULL," ");
    char *usr = strtok(NULL," ");
    char *pwd = strtok(NULL,"\n");
    //printf("user:%s\n",usr);
    //printf("pwd: %s\n",pwd);
   // printf("Pid of the process: %d\n",pid);
    //printf("Authenticating..\n");
    if(!authn(usr,pwd))
    {
        printf("Authentication sucessfull!\n");
        kill(pid,SIGUSR1);
    }
    else
    {
        printf("Authentication failed!\n");
        kill(pid,SIGUSR2);
    }
}
int main()
{
    int ret = fork();
    if(ret==0)
    {
        if( setsid()<0 ) { //failed to become session leader
        fprintf(stderr,"error: failed setsid\n");
        exit(EXIT_FAILURE);
        }

        signal(SIGCHLD,SIG_IGN);
        signal(SIGHUP,SIG_IGN);

        int child;
        if ( (child=fork())<0) { //failed fork
            fprintf(stderr,"error: failed fork\n");
            exit(EXIT_FAILURE);
        }
        if( child>0 ) { //parent
            exit(EXIT_SUCCESS);
        }
    
        mkfifo("fifo",0666);
        // printf("%d\n",getpid());
        rdr = open("fifo",O_RDONLY);
        printf("Reading log entries...\n");
        while(1)
        {
            read(rdr,buff,30);
            if(strlen(buff))
            {
                //printf("Buffer: %s\n",buff);
                sig_hand();
                for(int i=0;i<50;i++) //clean buffer
                    buff[i]='\0';
            }
        }
    }
    return 0;
}

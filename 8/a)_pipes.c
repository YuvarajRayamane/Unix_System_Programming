#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
int fd[2];
pipe(fd);
int cid = fork();
if(cid==0)
{
char content[100];
int res = close(fd[0]);
printf("I am a child and write the content which you want to enter into the file\n ");
scanf("%[^\n]%*c",content);
write(fd[1],content,sizeof(content)+1);
}
else{
char content[100];
int res = close(fd[1]);
printf("I am a parent and waiting for the content which you entered into the file\n ");
read(fd[0],content,sizeof(content));
printf("%s",content);
}
}

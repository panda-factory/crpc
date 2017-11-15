#include <sys/un.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

#include "buffer.h"

#define UNIX_DOMAIN ("/usr/local/etc/crpc/UNIX.domain")
#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    int ret, i;
    int pid;
    int conn_fd;
    int epoll_fd;
    int events_num;
    struct sockaddr_un connect_addr;
    struct epoll_event event_registe;
    struct epoll_event event_queue[20];
    buffer_t *send_buf;
    buffer_t *recv_buf;

    epoll_fd = epoll_create(256);

    conn_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (conn_fd < 0) {
        perror("cannot create client socket.");
        return 1;
    }

    connect_addr.sun_family = AF_UNIX;
    strcpy(connect_addr.sun_path, UNIX_DOMAIN);

    ret = connect(conn_fd, (struct sockaddr *)&connect_addr, sizeof(connect_addr));
    if (-1 == ret) {
        perror("cannot connect to the server");
        return 1;
    }

    event_registe.events = EPOLLOUT | EPOLLIN | EPOLLET;
    event_registe.data.fd = conn_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &event_registe);

    send_buf = buffer_new(BUFFER_SIZE);
    recv_buf = buffer_new(BUFFER_SIZE);
    
    if ((pid = fork()) < 0) {
        perror("fork error.");
        return 1;
    } else if (pid > 0) {
        while (1) {
            fgets(buffer_data(send_buf), buffer_total(send_buf), stdin);
            write(conn_fd, buffer_data(send_buf), buffer_total(send_buf));
        }
    } else {
        for (;;) {
            events_num = epoll_wait(epoll_fd, event_queue, 20, 500);
            for (i = 0; i<events_num; i++) {
                if ((conn_fd = event_queue[i].data.fd) > 0) {
                    if (event_queue[i].events & EPOLLIN) {
                        /* 接收事件*/
                        read(conn_fd, buffer_data(recv_buf), buffer_total(recv_buf));
                        printf("recv data from server: %s\n", buffer_data(recv_buf));
                    } else if (event_queue[i].events & EPOLLOUT) {
                        /* 发送事件*/
                        printf("send data to server: %s\n", buffer_data(send_buf));
                    } else {
                        /* 其他事件*/
                    }
                }
            }
        }
    }

    close(conn_fd);
    close(epoll_fd);

    return 0;
}

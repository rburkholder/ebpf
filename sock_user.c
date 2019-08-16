#include <stdio.h>
#include <assert.h>
#include <linux/bpf.h>
#include <bpf/bpf.h>
#include <bpf/bpf_load.h>
#include <bpf/sock_example.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
  if (load_bpf_file("sock_kern.o") != 0) {
    printf("%s", bpf_log_buf);
    return 1;
  }

  int sock = open_raw_sock("lo");

  assert( 0 == setsockopt( sock, SOL_SOCKET, SO_ATTACH_BPF, prog_fd, sizeof( prog_fd[0] ) ) );

  for ( int i = 0; i < 10; i++ ) {
    long long tcp_cnt, udp_cnt, icmp_cnt;
    int key;

    key = IPPROTO_TCP;
    assert( 0 == bpf_map_lookup_elem( map_fd[0], &key, &tcp_cnt) );

    key = IPPROTO_UDP;
    assert( 0 == bpf_map_lookup_elem( map_fd[0], &key, &udp_cnt ) );

    key = IPPROTO_ICMP;
    assert( 0 == bpf_map_lookup_elem( map_fd[0], &key, &icmp_cnt ) );

    printf( "TCP %lld UDP %lld ICMP %lld packets\n", tcp_cnt, udp_cnt, icmp_cnt );
    sleep( 1 );
  }

 return 0;
}


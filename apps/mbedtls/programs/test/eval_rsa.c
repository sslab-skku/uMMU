#include <cmi_app_common.h>
#include "eval.h"
//#include "S_bignum.h"

int main(int argc, char *argv[]) {
  int i = 0;
  char title[TITLE_LEN];
  unsigned char *tmp = malloc(TMP_BUFSIZE); // anno_secret_malloc(TMP_BUFSIZE);
  todo_list todo;
  unsigned char *buf = malloc(BUFSIZE);
  char *op;
  int keysize;
  op = (argc > 1) ? argv[1] : "none";
  keysize = (argc > 2) ? atoi(argv[2]) : 2048; // 2048 or 4096
  // unsigned char* buf = anno_secret_malloc(BUFSIZE);

  memset(buf, 0xAA, BUFSIZE);
  memset(tmp, 0xBB, TMP_BUFSIZE);

  mbedtls_rsa_context rsa;

  mbedtls_snprintf(title, sizeof(title), "RSA-%d", keysize);

  mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V15, 0);
  /* CMI_INIT_SECURE_MPI(rsa.D); */
  /* CMI_INIT_SECURE_MPI(rsa.P); */
  /* CMI_INIT_SECURE_MPI(rsa.Q); */

  mbedtls_rsa_gen_key(&rsa, myrand, NULL, keysize, 65537);
  /* mbedtls_printf("0x%llx\n",rsa.D.p); */

  MAKE_SENSITIVE(rsa.D.p, rsa.D.n * 8);
  MAKE_SENSITIVE(rsa.P.p, rsa.P.n * 8);
  MAKE_SENSITIVE(rsa.Q.p, rsa.Q.n * 8);
  MAKE_SENSITIVE(rsa.DP.p, rsa.DP.n * 8);
  MAKE_SENSITIVE(rsa.DQ.p, rsa.DQ.n * 8);

#if defined(MBEDTLS_RSA_NO_CRT)
  mbedtls_printf("MBEDTLS_RSA_NO_CRT is defined\n");
#endif

  /* TIME_PUBLIC_DURING(0.5, title, "public", */
  /*              buf[0] = 0; */
  /*              ret = mbedtls_rsa_public( &rsa, buf, buf );); */

  uint64_t prev_clock, rep = 5;
#if 0
  get_and_reset_timer();
  prev_clock=clock();
  for(int i=0;i<rep;i++){
      buf[0] = 0;
      mbedtls_rsa_public( &rsa, buf, buf );
  }
  mbedtls_printf("%9lu cycles, %f pub/s \n",
                 get_and_reset_timer()/rep,
                 (double)rep*CLOCKS_PER_SEC/(clock()-prev_clock));
#endif

#if 1
  // private
  get_and_reset_timer();
  prev_clock = clock();
  for (int i = 0; i < rep; i++) {
    buf[0] = 0;
    mbedtls_rsa_private(&rsa, myrand, NULL, buf, buf);
  }
  mbedtls_printf("%9lu cycles, %f priv/s \n", get_and_reset_timer() / rep,
                 (double)rep * CLOCKS_PER_SEC / (clock() - prev_clock));
#endif

  mbedtls_rsa_free(&rsa);
  return 0;
}

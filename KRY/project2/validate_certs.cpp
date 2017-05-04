/*
 * @file validate_certs.c
 * @author Tomas Coufal <xcoufa09>
 * @brief Connect to server pool using SSL and validate their certs
 */

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
// SSL Connection
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BASE_HOST ".minotaur.fi.muni.cz"
#define PORT "443"
#define SERVER_COUNT 100
#define LOCAL_CERT "crocs-ca.pem"

#define eprintf_(host, msg, retval) {\
  fprintf(stderr, "\x1B[31mERROR [%s] \033[0m%s\n", host.c_str(), msg);\
  return retval;\
}
#define eprintf(host, msg) eprintf_(host, msg, EXIT_FAILURE)
#define success_printf(host, msg) \
  fprintf(stderr, "\x1B[32mOK    [%s] \033[0m%s\n", host.c_str(), msg);

#define oprintf(host,cat, msg) printf("%s,%c,%s\n", host.c_str(), cat, msg)

#define in_category(arr, x) any_of(begin(arr), end(arr), [&](long i) { return i == x; })

typedef struct {
  X509 * cert;
  int errno;
} SSL_STATUS;

using namespace std;

vector<string> get_hostnames() {
  vector<string> hostnames;
  ostringstream host;

  for (size_t i = 0; i < SERVER_COUNT; i++) {
    host << setw(2) << setfill('0') << i << BASE_HOST;
    hostnames.push_back(host.str());
    host.str(std::string());
  }

  return hostnames;
}
//
// void verify_cert(const string host, X509 * cert) {
//   // cout << host <<endl;
//
//   BIO *bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);
//
//   //Subject
//   BIO_printf(bio_out,"Subject: ");
//   X509_NAME_print(bio_out,X509_get_subject_name(cert),0);
//   BIO_printf(bio_out,"\n");
//
//   //Issuer
//   BIO_printf(bio_out,"Issuer: ");
//   X509_NAME_print(bio_out,X509_get_issuer_name(cert),0);
//   BIO_printf(bio_out,"\n");
//
//   //Public Key
//   EVP_PKEY *pkey=X509_get_pubkey(cert);
//   EVP_PKEY_print_public(bio_out, pkey, 0, NULL);
//   EVP_PKEY_free(pkey);
//   //
//   // //Signature
//   X509_signature_print(bio_out, cert->sig_alg, cert->signature);
//   BIO_printf(bio_out,"\n");
//
//   cout << endl;
//   BIO_free(bio_out);
// }

int setup_ssl(SSL_CTX ** ctx) {
  const SSL_METHOD* meth = SSLv23_client_method();
  OpenSSL_add_ssl_algorithms();

	*ctx = SSL_CTX_new(meth);
  if (!*ctx) eprintf( string("GLOBAL"), "Unable to setup SSL_CTX");
	SSL_CTX_set_verify_depth(*ctx, 4);

	SSL_CTX_set_options(*ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION);
  const char *dir = getenv(X509_get_default_cert_dir_env());
  if (!dir) dir = X509_get_default_cert_dir();
	SSL_CTX_load_verify_locations(*ctx, NULL, dir);
  SSL_CTX_load_verify_locations(*ctx, LOCAL_CERT, NULL);
  return EXIT_SUCCESS;
}


int connect_to_host(const string host, SSL_CTX * ctx, SSL ** ssl) {
	BIO * bio = BIO_new_ssl_connect(ctx);
  if (!bio) eprintf(host, "Unable to setup BIO");

	BIO_set_conn_hostname(bio, host.c_str());
  BIO_set_conn_port(bio, PORT);

	BIO_get_ssl(bio, ssl);
  if (!ssl) eprintf(host, "Unable to setup SSL");

	SSL_set_mode(*ssl, SSL_MODE_AUTO_RETRY);
	SSL_set_tlsext_host_name(*ssl, host.c_str());

	BIO_new_fp(stdout, BIO_NOCLOSE);
	long res = BIO_do_connect(bio);
  if (res <= 0) {ERR_print_errors_fp(stderr); eprintf(host, "Unable to connect");}
	res = BIO_do_handshake(bio);
  if (res <= 0) {ERR_print_errors_fp(stderr); eprintf(host, "Unable to handshake");}
  return EXIT_SUCCESS;
}

int verify_certificate(const string host, SSL * ssl) {
  long cat_1[] = {X509_V_OK};
  long cat_2[] = {X509_V_ERR_CERT_HAS_EXPIRED};
  // long cat_3[] = {};

  // X509 *cert = SSL_get_peer_certificate(ssl);
  // STACK_OF(X509) *sk = SSL_get_peer_cert_chain(ssl);

  long res = SSL_get_verify_result(ssl);

  char cat = '0';
	if (in_category(cat_1, res)) cat = '1';
  else if (in_category(cat_2, res)) cat = '2';
  // else if (in_category(cat_3, res)) cat = '3';
  else cat = '4';
  oprintf(host, cat, X509_verify_cert_error_string(res));

  return EXIT_SUCCESS;
}

int main() {
  vector<string> hostnames = get_hostnames();
  SSL_load_error_strings();
  SSL_library_init();

  SSL_CTX * ctx;
  SSL * ssl;
  setup_ssl(&ctx);
  for (vector<string>::iterator it = hostnames.begin(); it < hostnames.end(); it++) {
    connect_to_host(*it, ctx, &ssl);
    verify_certificate(*it, ssl);
  }
  return EXIT_SUCCESS;
}

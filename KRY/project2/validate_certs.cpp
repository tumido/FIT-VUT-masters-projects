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
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
// SSL Connection
#include <openssl/ssl.h>

#define BASE_HOST ".minotaur.fi.muni.cz"
#define PORT "443"
#define SERVER_COUNT 100

#define eprintf_(host, msg, retval) {\
  fprintf(stderr, "\x1B[31m[%s] %s\033[0m\n", host.c_str(), msg);\
  return retval;\
}
#define eprintf(host, msg) eprintf_(host, msg, EXIT_FAILURE)

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

void verify_cert(const string host, X509 * cert) {
  // cout << host <<endl;

  BIO *bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);

  //Subject
  BIO_printf(bio_out,"Subject: ");
  X509_NAME_print(bio_out,X509_get_subject_name(cert),0);
  BIO_printf(bio_out,"\n");

  //Issuer
  // BIO_printf(bio_out,"Issuer: ");
  X509_NAME_print(bio_out,X509_get_issuer_name(cert),0);
  // BIO_printf(bio_out,"\n");

  //Public Key
  EVP_PKEY *pkey=X509_get_pubkey(cert);
  EVP_PKEY_print_public(bio_out, pkey, 0, NULL);
  EVP_PKEY_free(pkey);
  //
  // //Signature
  X509_signature_print(bio_out, cert->sig_alg, cert->signature);
  // BIO_printf(bio_out,"\n");

  cout << endl;
  BIO_free(bio_out);
}

int connect_to_host(const string host) {
  struct addrinfo hints, * res;
	int status, sock;

	//clear hints
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(host.c_str(), PORT, &hints, &res);
	if(status != 0) eprintf(host, "Unable to get ADDR info");

	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sock < 0) eprintf(host, "Error when creating socket");

	status = connect(sock, res->ai_addr, res->ai_addrlen);
	if(status < 0) eprintf(host, "Unable to open TCP connection");

	freeaddrinfo(res);
  return sock;
}

X509 * fetch_certificate(const string host, int sock) {
  const SSL_METHOD *meth = SSLv23_client_method();
  OpenSSL_add_ssl_algorithms();

  SSL_CTX *ctx = SSL_CTX_new(meth);
  SSL *ssl = SSL_new(ctx);
  BIO *sbio = BIO_new_socket(sock,BIO_NOCLOSE);

  SSL_set_tlsext_host_name(ssl, host.c_str());

  SSL_set_bio(ssl,sbio,sbio);
  if (SSL_connect(ssl) <= 0)
    eprintf_(host, "Unable to establish SSL", NULL);

  X509 *peer = SSL_get_peer_certificate(ssl);

  SSL_CTX_free(ctx);
  return peer;
}

int main() {
  vector<string> hostnames = get_hostnames();
  SSL_load_error_strings();
  SSL_library_init();

  int sock;
  X509 * cert;
  for (vector<string>::iterator it = hostnames.begin(); it < hostnames.end(); it++) {
    sock = connect_to_host(*it);
    cert = fetch_certificate(*it, sock);
    verify_cert(*it, cert);
    X509_free(cert);
    close(sock);
  }
  return EXIT_SUCCESS;
}

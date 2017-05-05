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

#define eprintf(host, msg) fprintf(stderr, "\x1B[31mERROR [%s] \033[0m%s\n", host.c_str(), msg);
#define oprintf(host,cat, msg) printf("%s,%c,%s\n", host.c_str(), cat, msg)

using namespace std;

/**
 * Create a vector of HOSTNAMEs in sequence 00..SERVER_COUNT
 */
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

/**
 * Setup SSL connection
 * @param  ctx Store SSL_CTX when configured and ready to open connection
 */
void setup_ssl(SSL_CTX ** ctx) {
  // Initialize SSL_METHOD used
  const SSL_METHOD* meth = SSLv23_client_method();
  OpenSSL_add_ssl_algorithms();
  // Prepare SSL_CTX object
	*ctx = SSL_CTX_new(meth);
  if (!*ctx) eprintf(string("GLOBAL"), "Unable to setup SSL_CTX");
	SSL_CTX_set_verify_depth(*ctx, 4);
  // Additional options
	SSL_CTX_set_options(*ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION);
  // Load certificates
  const char *dir = getenv(X509_get_default_cert_dir_env());
  if (!dir) dir = X509_get_default_cert_dir();
	SSL_CTX_load_verify_locations(*ctx, NULL, dir);
  SSL_CTX_load_verify_locations(*ctx, LOCAL_CERT, NULL);
}

/**
 * Establish a SSL conection to host
 * @param host server hostname
 * @param ctx  SSL_CTX connection configuration
 * @param ssl  SSL object used to connect with, data stored here
 */
void connect_to_host(const string host, SSL_CTX * ctx, SSL ** ssl) {
  // Create IO stream connection
	BIO * bio = BIO_new_ssl_connect(ctx);
  if (!bio) eprintf(host, "Unable to setup BIO");
  // Provide host and PORT setup
	BIO_set_conn_hostname(bio, host.c_str());
  BIO_set_conn_port(bio, PORT);
  // Load up the SSL
	BIO_get_ssl(bio, ssl);
  if (!ssl) eprintf(host, "Unable to setup SSL");
  // Enable retry and SNI
	SSL_set_mode(*ssl, SSL_MODE_AUTO_RETRY);
	SSL_set_tlsext_host_name(*ssl, host.c_str());
  // Connect
	BIO_new_fp(stdout, BIO_NOCLOSE);
	long res = BIO_do_connect(bio);
  if (res <= 0) {ERR_print_errors_fp(stderr); eprintf(host, "Unable to connect");}
	res = BIO_do_handshake(bio);
  if (res <= 0) {ERR_print_errors_fp(stderr); eprintf(host, "Unable to handshake");}
  // Close connection
  BIO_ssl_shutdown(bio);
}

/**
 * Verify used Public key encryption method
 * @param  key The key to verify
 * @param  msg Error string message
 * @return     true if the used cipher is weak
 */
bool is_weak(EVP_PKEY * key, string * msg) {
  // If RSA is used but, the key is weak
  if (key->type == EVP_PKEY_RSA && EVP_PKEY_bits(key) < 2048)
    {msg->assign("weak RSA public key"); return true;}
  // If Elliptical curves are used, but the key weak
  if (key->type == EVP_PKEY_EC && EVP_PKEY_bits(key) < 256)
    {msg->assign("weak EC public key"); return true;}
  // If any other method is used, it's unsupported
  if (((EVP_PKEY_EC | EVP_PKEY_RSA) & key->type) == 0)
    {msg->append(", unsupported public key type"); return true;}
  return false;
}

/**
 * Verify the secrecy and print trust level
 * @param host Hostname string
 * @param ssl  SSL object previously used in connection with this host
 */
void verify_certificate(const string host, SSL * ssl) {
  // Get certificate and public key
  X509 *cert = SSL_get_peer_certificate(ssl);
  EVP_PKEY* public_key = X509_get_pubkey(cert);
  // Verify the SSL
  long res = SSL_get_verify_result(ssl);

  // Categorize
  char cat = '1';
  string msg = X509_verify_cert_error_string(res);
  // CATEGORY 1 = Everything is OK
	if (res == X509_V_OK) cat = '1';
  // CATEGORY 2 = Certificate is expired
  else if (res == X509_V_ERR_CERT_HAS_EXPIRED) cat = '2';
  // CATEGORY 3 = used weak encryption
  if (is_weak(public_key, &msg)) cat = '3';
  // CATEGORY 4 = Any other error
  else if (res != X509_V_OK && res != X509_V_ERR_CERT_HAS_EXPIRED) cat = '4';

  // Print out the findings
  oprintf(host, cat, msg.c_str());

  EVP_PKEY_free(public_key);
  X509_free(cert);
}

/**
 * Analyze SSL connection
 * @return EXIT_SUCCESS
 */
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
    SSL_free(ssl);
  }
  SSL_CTX_free(ctx);
  return EXIT_SUCCESS;
}

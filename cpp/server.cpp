#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>

void handleClient(SSL* ssl) {
    char buffer[1024] = {0};
    while (true) {
        int bytes = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        std::cout << "Received: " << buffer << std::endl;
        SSL_write(ssl, buffer, strlen(buffer)); // Echo the message back
    }
    SSL_free(ssl); // Free SSL structure
}

int main() {
    // Initialize SSL library
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    const SSL_METHOD* method = SSLv23_server_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    
    if (ctx == nullptr) {
        ERR_print_errors_fp(stderr);
        return EXIT_FAILURE;
    }

    // Load your SSL certificates
    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return EXIT_FAILURE;
    }

    // Create server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        SSL_CTX_free(ctx);
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4433); // Port number
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        SSL_CTX_free(ctx);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        SSL_CTX_free(ctx);
        return EXIT_FAILURE;
    }

    std::cout << "Server is listening on port 4433..." << std::endl;

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            perror("Accept failed");
            continue; // Continue accepting new connections
        }

        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_fd);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            std::thread clientThread(handleClient, ssl); // Pass SSL pointer to the thread
            clientThread.detach(); // Detach the thread to run independently
        }

        close(client_fd); // Close the client socket after creating the thread
    }

    close(server_fd);
    SSL_CTX_free(ctx);
    return 0;
}

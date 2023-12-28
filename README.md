# C++ Webserver

# Motivation:
This project was a start to solidy my understanding within multithreading, c++ and also networking. As I created a custom website before, I decided that it would be a great opportunity to understand how these static pages are hosted on the web. This project was very interesting to find new ways to improve performance of a webserver including problems that can occur within I/O and compute bottlenecks. I would like to come back to this project to further understand server side rendering, security, further I/O improvements and potentially the utilization of CDNs.

# Summary:
I've utilized different network communication techniques, multithreading, and several optimization strategies to produce efficient webserver to serve static content. At the core of the server's network functionality are sockets, which are crucial for setting up the TCP/IP connections needed for client-server communication. I've specifically chosen TCP for its reliability in data transmission.

To handle concurrency, I've integrated a ThreadPool class. This is essential for managing multiple simultaneous connections efficiently. By enqueuing incoming client requests as tasks in this thread pool, the server can handle multiple requests in parallel improving throughput and responsiveness. The server's architecture allows each client connection to be handled in a separate thread without the overhead of thread creation/deletion. This approach not only improves scalability but also ensures the main thread is always available to accept new connections, maintaining a non-blocking behavior essential for high throughput.

Optimization is key in any server application, and I've focused on a few critical areas. Firstly, I pre-load the content of "index.html" into memory reducing expensive I/O operations. I also utilize string streams for efficient file reading and a fixed-size buffer for network data, which is a standard practice in socket programming for effective memory management.

I've also implemented basic HTTP protocol handling, parsing HTTP requests, and constructing appropriate responses. This includes setting standard HTTP headers and managing different types of HTTP requests and responses. While the server is currently basic and tailored to serve static content, it lays a strong foundation for more advanced features like SSL/TLS implementation, advanced error handling, and dynamic content support.

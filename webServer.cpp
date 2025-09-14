// **************************************************************************************
// * webServer (webServer.cpp)
// * - Implements a very limited subset of HTTP/1.0, use -v to enable verbose debugging output.
// * - Port number 1701 is the default, if in use random number is selected.
// *
// * - GET requests are processed, all other metods result in 400.
// *     All header gracefully ignored
// *     Files will only be served from cwd and must have format file\d.html or image\d.jpg
// *
// * - Response to a valid get for a legal filename
// *     status line (i.e., response method)
// *     Cotent-Length:
// *     Content-Type:
// *     \r\n
// *     requested file.
// *
// * - Response to a GET that contains a filename that does not exist or is not allowed
// *     statu line w/code 404 (not found)
// *
// * - CSCI 471 - All other requests return 400
// * - CSCI 598 - HEAD and POST must also be processed.
// *
// * - Program is terminated with SIGINT (ctrl-C)
// **************************************************************************************
#include "webServer.h"


// **************************************************************************************
// * Signal Handler.
// * - Display the signal and exit (returning 0 to OS indicating normal shutdown)
// * - Optional for 471, required for 598
// **************************************************************************************
// void sig_handler(int signo) {}


// **************************************************************************************
// * processRequest,
//   - Return HTTP code to be sent back
//   - Set filename if appropriate. Filename syntax is valided but existance is not verified.
// **************************************************************************************
int readRequest(int sockFd,std::string &filename) {
  int returnCode = 400; // default return code

  std::string container; // container to hold line
  int bytesRead; // keep track of number of bytes read
  char buffer[BUFFER_SIZE]; // buffer to store bytes read
  container = ""; // intialize container to empty string

  while (true) { // loop until entire line is read
    bzero(buffer,BUFFER_SIZE); // initialize blank buffer

    /* Read data sent from client */
    if ((bytesRead = read(sockFd, buffer, BUFFER_SIZE)) < 1) {
      if (bytesRead < 0) { // if bytesRead < 0, errno will have more information about error
        std::cout << "read() failed: " << strerror(errno) << std::endl;
        exit(-1);
      }
      // if bytesRead = 0, then connection closed.
      std::cout << "connection closed unexpectedly" << std::endl;
      break;
    }
    std::cout << "We read " << bytesRead << "bytes" << std::endl;
    container.append(buffer, bytesRead); // append to container
    if (container.find("\r\n") != std::string::npos) { // check for line terminator
      break;
    }
  }

  // check if container holds a GET request
  std::smatch get_matches;
  if (std::regex_search(container, get_matches, GET_REGEX)) {
    // print useful information
    if (get_matches.size() == 4) { // 0-th element is the full match, then 4 capture groups
      std::cout << "Full Match: " << get_matches[0].str() << std::endl;
      std::cout << "Method: " << get_matches[1].str() << std::endl;
      std::cout << "Path: " << get_matches[2].str() << std::endl;
      std::cout << "HTTP Version: " << get_matches[3].str() << std::endl;
    }
    filename = get_matches[2].str();

    // check if file format is valid
    std::smatch file_matches;
    if (std::regex_search(filename, file_matches, FILE_REGEX)) { // if valid file format
      std::cout << "200: HTTP Request OK." << std::endl;
      returnCode = 200;
    } else { // if invalid file format
      std::cout << "404: File Not Found." << std::endl;
      returnCode = 404;
    }
  } else { // if format didn't match GET request
    std::cout << "400: Bad Request." << std::endl;
    returnCode = 400;
  }
  
  return returnCode;
}


// **************************************************************************
// * Send one line (including the line terminator <LF><CR>)
// * - Assumes the terminator is not included, so it is appended.
// **************************************************************************
void sendLine(int socketFd, const std::string &stringToSend) {
  /* send container back to client */
  std::string buffer = stringToSend + "\r\n"; // make new string with crlf
  int bytesWritten;
  if ((bytesWritten = write(socketFd, buffer.c_str(), buffer.length())) < 0) {
    std::cout << "write() failed: " << strerror(errno) << std::endl;
    exit(-1);
  }
  return;
}

// **************************************************************************
// * Send the entire 404 response, header and body.
// **************************************************************************
void send404(int sockFd) {
  sendLine(sockFd, "HTTP/1.1 404 Not Found");
  sendLine(sockFd, "content-type: text/html");
  sendLine(sockFd, "content-length: 118");
  sendLine(sockFd, ""); // end message header
  sendLine(sockFd, "File not found. Please request a file of the following format: fileX.html or imageX.jpg where X is a single digit 0-9."); // friendly message
  sendLine(sockFd, ""); // end message body
  return;
}

// **************************************************************************
// * Send the entire 400 response, header and body.
// **************************************************************************
void send400(int sockFd) {
  sendLine(sockFd, "HTTP/1.1 400 Bad Request");
  sendLine(sockFd, "content-type: text/html");
  sendLine(sockFd, "content-length: 67");
  sendLine(sockFd, ""); // end message header
  sendLine(sockFd, "Bad request. Please send a GET request using the standard HTTP/1.1."); // friendly message
  sendLine(sockFd, ""); // end message body
  return;
}


// **************************************************************************************
// * sendFile
// * -- Send a file back to the browser.
// **************************************************************************************
void sendFile(int sockFd,std::string filename) {
  return;
}


// **************************************************************************************
// * processConnection
// * -- process one connection/request.
// **************************************************************************************
int processConnection(int sockFd) {
 
  // Call readHeader()

  // If read header returned 400, send 400

  // If read header returned 404, call send404

  // 471: If read header returned 200, call sendFile
  
  // 598 students
  // - If the header was valid and the method was GET, call sendFile()
  // - If the header was valid and the method was HEAD, call a function to send back the header.
  // - If the header was valid and the method was POST, call a function to save the file to dis.

  // std::string container;
  // int bytesRead;
  // int bytesWritten;
  // char buffer[BUFFER_SIZE];

  // while (strcmp(container.c_str(), TERM_STRING) != 0) {
  //   container = "";

  //   while (true) { // loop until entire message is read
  //     bzero(buffer,BUFFER_SIZE); // initialize blank buffer

  //     /* Read data sent from client */
  //     if ((bytesRead = read(sockFd, buffer, BUFFER_SIZE)) < 1) {
  //       if (bytesRead < 0) {
  //         std::cout << "read() failed: " << strerror(errno) << std::endl;
  //         exit(-1);
  //       }
  //       std::cout << "connection closed unexpectedly" << std::endl;
  //       break;
  //     }
  //     std::cout << "We read " << bytesRead << "bytes" << std::endl;
  //     container.append(buffer, bytesRead); // append to container
  //     std::cout << container << std::endl; // DEBUGGING LINE
  //     std::cout << buffer << std::endl; //DEBUGGING
  //     if (container.find("\r\n") != std::string::npos) { // check for line terminator
  //       break;
  //     }
  //   }

  //   /* send container back to client */
  //   if ((bytesWritten = write(sockFd, container.c_str(), container.length())) < 0) {
  //     std::cout << "write() failed: " << strerror(errno) << std::endl;
  //     exit(-1);
  //   }
  // }

  std::string filename;

  int returnCode = readRequest(sockFd, filename); // read request
  std::cout << "Return Code: " << returnCode << " Filename: " << filename << std::endl;

  if (returnCode == 400) {
    send400(sockFd);
  } else if (returnCode == 404) {
    send404(sockFd);
  } else if (returnCode == 200) {
    sendFile(sockFd, filename);
  } else {
    std::cout << "Unexpected return code: " << returnCode << std::endl;
    exit(-1);
  }

  return 0;
}
    

int main (int argc, char *argv[]) {


  // ********************************************************************
  // * Process the command line arguments
  // ********************************************************************
  int opt = 0;
  while ((opt = getopt(argc,argv,"d:")) != -1) {
    
    switch (opt) {
    case 'd':
      LOG_LEVEL = std::stoi(optarg);
      break;
    case ':':
    case '?':
    default:
      std::cout << "useage: " << argv[0] << " -d LOG_LEVEL" << std::endl;
      exit(-1);
    }
  }


  // *******************************************************************
  // * Catch all possible signals
  // ********************************************************************
  DEBUG << "Setting up signal handlers" << ENDL;
  

  
  // *******************************************************************
  // * Creating the inital socket using the socket() call.
  // ********************************************************************
  int listenFd = -1;
  if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "Failed to create listening socket " << strerror(errno) << std::endl;
    exit(-1);
  }
  DEBUG << "Calling Socket() assigned file descriptor " << listenFd << ENDL;

  
  // ********************************************************************
  // * The bind() call takes a structure used to spefiy the details of the connection. 
  // *
  // * struct sockaddr_in servaddr;
  // *
  // On a cient it contains the address of the server to connect to. 
  // On the server it specifies which IP address and port to lisen for connections.
  // If you want to listen for connections on any IP address you use the
  // address INADDR_ANY
  // ********************************************************************
  struct sockaddr_in {
    sa_family_t sin_family; /* Address family */
    in_port_t sin_port; /* Port number. */
    struct in_addr sin_addr; /* Internet address. */

    /* Pad to size of `struct sockaddr'. */
    unsigned char sin_zero[8];
  };

  // Define the structure
  struct sockaddr_in servaddr;

  // Zero the whole thing.
  bzero(&servaddr, sizeof(servaddr));

  // IPv4 Protocol Family
  servaddr.sin_family = AF_INET;

  // Let the system pick the IP address.
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  // You pick a random high-numbered port
  servaddr.sin_port = htons(PORT);




  // ********************************************************************
  // * Binding configures the socket with the parameters we have
  // * specified in the servaddr structure.  This step is implicit in
  // * the connect() call, but must be explicitly listed for servers.
  // *
  // * Don't forget to check to see if bind() fails because the port
  // * you picked is in use, and if the port is in use, pick a different one.
  // ********************************************************************
  uint16_t port; // set port to default value
  DEBUG << "Calling bind()" << ENDL;

  bool bound = false; // boolean is false until bind() call is successful
  while (!bound) {
      if (bind(listenFd, (sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        if (errno == EADDRINUSE) { // error due to busy port number
          servaddr.sin_port = htons(ntohs(servaddr.sin_port) + 1); // try next port number
        } else { // any other error
        std::cout << "bind() failed: " << strerror(errno) << std::endl;
        exit(-1);
        }
    } else {
      bound = true; // bind() call was successful
    }
  }

  port = ntohs(servaddr.sin_port);
  std::cout << "Using port: " << port << std::endl;


  // ********************************************************************
  // * Setting the socket to the listening state is the second step
  // * needed to being accepting connections.  This creates a que for
  // * connections and starts the kernel listening for connections.
  // ********************************************************************
  DEBUG << "Calling listen()" << ENDL;

  int listenq = 1;
  if (listen(listenFd, listenq) < 0) {
    std::cout << "listen() failed: " << strerror(errno) << std::endl;
    exit(-1);
  }


  // ********************************************************************
  // * The accept call will sleep, waiting for a connection.  When 
  // * a connection request comes in the accept() call creates a NEW
  // * socket with a new fd that will be used for the communication.
  // ********************************************************************
  int quitProgram = 0;
  while (!quitProgram) {
    int connFd = -1;
    DEBUG << "Calling connFd = accept(fd,NULL,NULL)." << ENDL;

    if ((connFd = accept(listenFd, (sockaddr *) NULL, NULL)) < 0) {
      std::cout << "accept() failed: " << strerror(errno) << std::endl;
      exit(-1);
    }

    DEBUG << "We have recieved a connection on " << connFd << ". Calling processConnection(" << connFd << ")" << ENDL;
    quitProgram = processConnection(connFd);
    DEBUG << "processConnection returned " << quitProgram << " (should always be 0)" << ENDL;
    DEBUG << "Closing file descriptor " << connFd << ENDL;
    close(connFd);
  }
  

  ERROR << "Program fell through to the end of main. A listening socket may have closed unexpectadly." << ENDL;
  closefrom(3);

}

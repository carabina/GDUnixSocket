//
//  GDUnixSocketServer.h
//  GDUnixSocketExample
//
//  Created by Alex G on 10.02.16.
//  Copyright © 2016 Alexey Gordiyenko. All rights reserved.
//

#import "GDUnixSocket.h"

/** Default number of simultaneous connections (5). */
extern const int kGDUnixSocketServerMaxConnectionsDefault;

@protocol GDUnixSocketServerDelegate;

/**
 Class which represents server-side Unix domain socket connection.
 */
@interface GDUnixSocketServer : GDUnixSocket

/** Delegate object that receives messages from `GDUnixSocketServer` object. */
@property (nonatomic, readwrite, weak) id<GDUnixSocketServerDelegate> delegate;

/**
 Listens for incoming connections on the socket.
 @discussion First the address is retrieved from socket path (previously passed as an argument to designated initializer). Then it binds the socket: assigns that address. Finally it starts listening on the socket, that is, marks it as a socket that will be used to accept incoming connection requests. The socket is closed if any error occurs.
 @param maxConnections The maximum simultaneous connections count. If 0 is passed `kGDUnixSocketServerMaxConnectionsDefault` value is used (which is 5). If a connection request arrives when the queue is full, the client may receive an error with an indication of ECONNREFUSED or, if the underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds.
 @return An error object if any error occured or `nil` otherwise.
 */
- (NSError *)listenWithMaxConnections:(int)maxConnections;

/**
 Sends `listenWithMaxConnections:` message with 0 as an argument.
 @see - (NSError *)listenWithMaxConnections:(NSInteger)maxConnections;
 */
- (NSError *)listen;

/**
 Writes data to socket associated with client synchronously.
 @param data Data to be written. If you pass `nil` or empty data, this method does nothing.
 @param clientID A client connection unique identifier.
 @param error If an error occurs, upon return contains an NSError object that describes the problem. Can be `nil`.
 @return The number of bytes written. If an error occurs, -1 is returned and an error object pointed by `error` parameter is set.
 */
- (ssize_t)sendData:(NSData *)data toClientWithID:(NSString *)clientID error:(NSError **)error;

/**
 Writes data to socket associated with client asynchronously.
 @param data Data to be written. If you pass `nil` or empty data, this method does nothing.
 @param clientID A client connection unique identifier.
 @param completion Block to be called upon completion of writing operation. This block has no return value and receives two parameters: `error` if any error occurs and `size` which represents the number of bytes written.
 */
- (void)sendData:(NSData *)data toClientWithID:(NSString *)clientID completion:(void(^)(NSError *error, ssize_t size))completion;

@end

/**
 Protocol of `GDUnixSocketServer`'s delegate.
 */
@protocol GDUnixSocketServerDelegate <NSObject>
@optional
/**
 Called when delegate's owner closes its socket.
 @param unixSocketServer Delegate's owner, a server listening on incoming connections.
 @param error Error object describing the problem or `nil` if closed successfully.
 */
- (void)unixSocketServerDidClose:(GDUnixSocketServer *)unixSocketServer error:(NSError *)error;

/**
 Called when delegate's owner accepts a new incoming connection.
 @param unixSocketServer Delegate's owner, a server listening on incoming connections.
 @param newClientID A new client connection unique identifier.
 */
- (void)unixSocketServer:(GDUnixSocketServer *)unixSocketServer didAcceptClientWithID:(NSString *)newClientID;

/**
 Called when delegate's owner receives data from a particular client.
 @param unixSocketServer Delegate's owner, a server listening on incoming connections.
 @param data Data object received from client.
 @param clientID A client connection unique identifier.
 */
- (void)unixSocketServer:(GDUnixSocketServer *)unixSocketServer didReceiveData:(NSData *)data fromClientWithID:(NSString *)clientID;

/**
 Called when delegate's owner failed to data from socket associated with particular client.
 @param unixSocketServer Delegate's owner, a server listening on incoming connections.
 @param clientID A client connection unique identifier.
 @param error Error object describing the problem.
 */
- (void)unixSocketServer:(GDUnixSocketServer *)unixSocketServer didFailToReadForClientID:(NSString *)clientID error:(NSError *)error;

/**
 Called when delegate's owner failed to accept connection.
 @discussion Delegate's owner first calls this method, then closes its socket.
 @param unixSocketServer Delegate's owner, a server listening on incoming connections.
 @param error Error object describing the problem.
 */
- (void)unixSocketServerDidFailToAcceptConnection:(GDUnixSocketServer *)unixSocketServer error:(NSError *)error;

@end
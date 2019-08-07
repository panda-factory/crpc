/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: crpc_protobuf.proto */

#ifndef PROTOBUF_C_crpc_5fprotobuf_2eproto__INCLUDED
#define PROTOBUF_C_crpc_5fprotobuf_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003002 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _CrpcIdentityRequest CrpcIdentityRequest;
typedef struct _CrpcIdentityAck CrpcIdentityAck;
typedef struct _CrpcCallbackRequest CrpcCallbackRequest;
typedef struct _CrpcCallbackAck CrpcCallbackAck;


/* --- enums --- */


/* --- messages --- */

struct  _CrpcIdentityRequest
{
  ProtobufCMessage base;
  uint32_t magic;
  char *name;
  uint32_t operate;
};
#define CRPC_IDENTITY_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&crpc_identity_request__descriptor) \
    , 0, NULL, 0 }


struct  _CrpcIdentityAck
{
  ProtobufCMessage base;
  uint32_t magic;
  char *name;
  uint32_t result;
};
#define CRPC_IDENTITY_ACK__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&crpc_identity_ack__descriptor) \
    , 0, NULL, 0 }


struct  _CrpcCallbackRequest
{
  ProtobufCMessage base;
  uint32_t magic;
  char *name;
  uint32_t callback_id;
  protobuf_c_boolean has_paramete;
  ProtobufCBinaryData paramete;
};
#define CRPC_CALLBACK_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&crpc_callback_request__descriptor) \
    , 0, NULL, 0, 0, {0,NULL} }


struct  _CrpcCallbackAck
{
  ProtobufCMessage base;
  uint32_t magic;
  char *name;
  uint32_t callback_id;
  protobuf_c_boolean has_return_;
  ProtobufCBinaryData return_;
};
#define CRPC_CALLBACK_ACK__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&crpc_callback_ack__descriptor) \
    , 0, NULL, 0, 0, {0,NULL} }


/* CrpcIdentityRequest methods */
void   crpc_identity_request__init
                     (CrpcIdentityRequest         *message);
size_t crpc_identity_request__get_packed_size
                     (const CrpcIdentityRequest   *message);
size_t crpc_identity_request__pack
                     (const CrpcIdentityRequest   *message,
                      uint8_t             *out);
size_t crpc_identity_request__pack_to_buffer
                     (const CrpcIdentityRequest   *message,
                      ProtobufCBuffer     *buffer);
CrpcIdentityRequest *
       crpc_identity_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   crpc_identity_request__free_unpacked
                     (CrpcIdentityRequest *message,
                      ProtobufCAllocator *allocator);
/* CrpcIdentityAck methods */
void   crpc_identity_ack__init
                     (CrpcIdentityAck         *message);
size_t crpc_identity_ack__get_packed_size
                     (const CrpcIdentityAck   *message);
size_t crpc_identity_ack__pack
                     (const CrpcIdentityAck   *message,
                      uint8_t             *out);
size_t crpc_identity_ack__pack_to_buffer
                     (const CrpcIdentityAck   *message,
                      ProtobufCBuffer     *buffer);
CrpcIdentityAck *
       crpc_identity_ack__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   crpc_identity_ack__free_unpacked
                     (CrpcIdentityAck *message,
                      ProtobufCAllocator *allocator);
/* CrpcCallbackRequest methods */
void   crpc_callback_request__init
                     (CrpcCallbackRequest         *message);
size_t crpc_callback_request__get_packed_size
                     (const CrpcCallbackRequest   *message);
size_t crpc_callback_request__pack
                     (const CrpcCallbackRequest   *message,
                      uint8_t             *out);
size_t crpc_callback_request__pack_to_buffer
                     (const CrpcCallbackRequest   *message,
                      ProtobufCBuffer     *buffer);
CrpcCallbackRequest *
       crpc_callback_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   crpc_callback_request__free_unpacked
                     (CrpcCallbackRequest *message,
                      ProtobufCAllocator *allocator);
/* CrpcCallbackAck methods */
void   crpc_callback_ack__init
                     (CrpcCallbackAck         *message);
size_t crpc_callback_ack__get_packed_size
                     (const CrpcCallbackAck   *message);
size_t crpc_callback_ack__pack
                     (const CrpcCallbackAck   *message,
                      uint8_t             *out);
size_t crpc_callback_ack__pack_to_buffer
                     (const CrpcCallbackAck   *message,
                      ProtobufCBuffer     *buffer);
CrpcCallbackAck *
       crpc_callback_ack__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   crpc_callback_ack__free_unpacked
                     (CrpcCallbackAck *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*CrpcIdentityRequest_Closure)
                 (const CrpcIdentityRequest *message,
                  void *closure_data);
typedef void (*CrpcIdentityAck_Closure)
                 (const CrpcIdentityAck *message,
                  void *closure_data);
typedef void (*CrpcCallbackRequest_Closure)
                 (const CrpcCallbackRequest *message,
                  void *closure_data);
typedef void (*CrpcCallbackAck_Closure)
                 (const CrpcCallbackAck *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor crpc_identity_request__descriptor;
extern const ProtobufCMessageDescriptor crpc_identity_ack__descriptor;
extern const ProtobufCMessageDescriptor crpc_callback_request__descriptor;
extern const ProtobufCMessageDescriptor crpc_callback_ack__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_crpc_5fprotobuf_2eproto__INCLUDED */

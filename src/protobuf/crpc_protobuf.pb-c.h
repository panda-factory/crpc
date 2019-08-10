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


typedef struct _CrpcMessageHead CrpcMessageHead;
typedef struct _CrpcMessageAck CrpcMessageAck;
typedef struct _CrpcCbReqRegister CrpcCbReqRegister;
typedef struct _CrpcCbRespRegister CrpcCbRespRegister;


/* --- enums --- */


/* --- messages --- */

struct  _CrpcMessageHead
{
  ProtobufCMessage base;
  uint32_t magic;
  uint32_t type;
  uint32_t msg_id;
  char *name;
};
#define CRPC_MESSAGE_HEAD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&crpc_message_head__descriptor) \
    , 0, 0, 0, NULL }


struct  _CrpcMessageAck
{
  ProtobufCMessage base;
  CrpcMessageHead *head;
  uint32_t result;
  char *description;
};
#define CRPC_MESSAGE_ACK__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&crpc_message_ack__descriptor) \
    , NULL, 0, NULL }


struct  _CrpcCbReqRegister
{
  ProtobufCMessage base;
  CrpcMessageHead *head;
  uint32_t callback_id;
  uint32_t register_id;
};
#define CRPC_CB_REQ_REGISTER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&crpc_cb_req_register__descriptor) \
    , NULL, 0, 0 }


struct  _CrpcCbRespRegister
{
  ProtobufCMessage base;
  CrpcMessageHead *head;
  uint32_t callback_id;
  uint32_t return_;
};
#define CRPC_CB_RESP_REGISTER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&crpc_cb_resp_register__descriptor) \
    , NULL, 0, 0 }


/* CrpcMessageHead methods */
void   crpc_message_head__init
                     (CrpcMessageHead         *message);
size_t crpc_message_head__get_packed_size
                     (const CrpcMessageHead   *message);
size_t crpc_message_head__pack
                     (const CrpcMessageHead   *message,
                      uint8_t             *out);
size_t crpc_message_head__pack_to_buffer
                     (const CrpcMessageHead   *message,
                      ProtobufCBuffer     *buffer);
CrpcMessageHead *
       crpc_message_head__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   crpc_message_head__free_unpacked
                     (CrpcMessageHead *message,
                      ProtobufCAllocator *allocator);
/* CrpcMessageAck methods */
void   crpc_message_ack__init
                     (CrpcMessageAck         *message);
size_t crpc_message_ack__get_packed_size
                     (const CrpcMessageAck   *message);
size_t crpc_message_ack__pack
                     (const CrpcMessageAck   *message,
                      uint8_t             *out);
size_t crpc_message_ack__pack_to_buffer
                     (const CrpcMessageAck   *message,
                      ProtobufCBuffer     *buffer);
CrpcMessageAck *
       crpc_message_ack__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   crpc_message_ack__free_unpacked
                     (CrpcMessageAck *message,
                      ProtobufCAllocator *allocator);
/* CrpcCbReqRegister methods */
void   crpc_cb_req_register__init
                     (CrpcCbReqRegister         *message);
size_t crpc_cb_req_register__get_packed_size
                     (const CrpcCbReqRegister   *message);
size_t crpc_cb_req_register__pack
                     (const CrpcCbReqRegister   *message,
                      uint8_t             *out);
size_t crpc_cb_req_register__pack_to_buffer
                     (const CrpcCbReqRegister   *message,
                      ProtobufCBuffer     *buffer);
CrpcCbReqRegister *
       crpc_cb_req_register__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   crpc_cb_req_register__free_unpacked
                     (CrpcCbReqRegister *message,
                      ProtobufCAllocator *allocator);
/* CrpcCbRespRegister methods */
void   crpc_cb_resp_register__init
                     (CrpcCbRespRegister         *message);
size_t crpc_cb_resp_register__get_packed_size
                     (const CrpcCbRespRegister   *message);
size_t crpc_cb_resp_register__pack
                     (const CrpcCbRespRegister   *message,
                      uint8_t             *out);
size_t crpc_cb_resp_register__pack_to_buffer
                     (const CrpcCbRespRegister   *message,
                      ProtobufCBuffer     *buffer);
CrpcCbRespRegister *
       crpc_cb_resp_register__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   crpc_cb_resp_register__free_unpacked
                     (CrpcCbRespRegister *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*CrpcMessageHead_Closure)
                 (const CrpcMessageHead *message,
                  void *closure_data);
typedef void (*CrpcMessageAck_Closure)
                 (const CrpcMessageAck *message,
                  void *closure_data);
typedef void (*CrpcCbReqRegister_Closure)
                 (const CrpcCbReqRegister *message,
                  void *closure_data);
typedef void (*CrpcCbRespRegister_Closure)
                 (const CrpcCbRespRegister *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor crpc_message_head__descriptor;
extern const ProtobufCMessageDescriptor crpc_message_ack__descriptor;
extern const ProtobufCMessageDescriptor crpc_cb_req_register__descriptor;
extern const ProtobufCMessageDescriptor crpc_cb_resp_register__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_crpc_5fprotobuf_2eproto__INCLUDED */

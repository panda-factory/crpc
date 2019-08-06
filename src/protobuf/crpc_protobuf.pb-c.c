/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: crpc_protobuf.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "crpc_protobuf.pb-c.h"
void   crpc_identity_request__init
                     (CrpcIdentityRequest         *message)
{
  static const CrpcIdentityRequest init_value = CRPC_IDENTITY_REQUEST__INIT;
  *message = init_value;
}
size_t crpc_identity_request__get_packed_size
                     (const CrpcIdentityRequest *message)
{
  assert(message->base.descriptor == &crpc_identity_request__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t crpc_identity_request__pack
                     (const CrpcIdentityRequest *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &crpc_identity_request__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t crpc_identity_request__pack_to_buffer
                     (const CrpcIdentityRequest *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &crpc_identity_request__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
CrpcIdentityRequest *
       crpc_identity_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (CrpcIdentityRequest *)
     protobuf_c_message_unpack (&crpc_identity_request__descriptor,
                                allocator, len, data);
}
void   crpc_identity_request__free_unpacked
                     (CrpcIdentityRequest *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &crpc_identity_request__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   crpc_identity_ack__init
                     (CrpcIdentityAck         *message)
{
  static const CrpcIdentityAck init_value = CRPC_IDENTITY_ACK__INIT;
  *message = init_value;
}
size_t crpc_identity_ack__get_packed_size
                     (const CrpcIdentityAck *message)
{
  assert(message->base.descriptor == &crpc_identity_ack__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t crpc_identity_ack__pack
                     (const CrpcIdentityAck *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &crpc_identity_ack__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t crpc_identity_ack__pack_to_buffer
                     (const CrpcIdentityAck *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &crpc_identity_ack__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
CrpcIdentityAck *
       crpc_identity_ack__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (CrpcIdentityAck *)
     protobuf_c_message_unpack (&crpc_identity_ack__descriptor,
                                allocator, len, data);
}
void   crpc_identity_ack__free_unpacked
                     (CrpcIdentityAck *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &crpc_identity_ack__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor crpc_identity_request__field_descriptors[3] =
{
  {
    "magic",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(CrpcIdentityRequest, magic),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "name",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(CrpcIdentityRequest, name),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "operate",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(CrpcIdentityRequest, operate),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned crpc_identity_request__field_indices_by_name[] = {
  0,   /* field[0] = magic */
  1,   /* field[1] = name */
  2,   /* field[2] = operate */
};
static const ProtobufCIntRange crpc_identity_request__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor crpc_identity_request__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "crpc_identity_request",
  "CrpcIdentityRequest",
  "CrpcIdentityRequest",
  "",
  sizeof(CrpcIdentityRequest),
  3,
  crpc_identity_request__field_descriptors,
  crpc_identity_request__field_indices_by_name,
  1,  crpc_identity_request__number_ranges,
  (ProtobufCMessageInit) crpc_identity_request__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor crpc_identity_ack__field_descriptors[3] =
{
  {
    "magic",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(CrpcIdentityAck, magic),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "name",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(CrpcIdentityAck, name),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "result",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(CrpcIdentityAck, result),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned crpc_identity_ack__field_indices_by_name[] = {
  0,   /* field[0] = magic */
  1,   /* field[1] = name */
  2,   /* field[2] = result */
};
static const ProtobufCIntRange crpc_identity_ack__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor crpc_identity_ack__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "crpc_identity_ack",
  "CrpcIdentityAck",
  "CrpcIdentityAck",
  "",
  sizeof(CrpcIdentityAck),
  3,
  crpc_identity_ack__field_descriptors,
  crpc_identity_ack__field_indices_by_name,
  1,  crpc_identity_ack__number_ranges,
  (ProtobufCMessageInit) crpc_identity_ack__init,
  NULL,NULL,NULL    /* reserved[123] */
};

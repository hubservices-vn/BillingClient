// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CLHandshakePack.proto

#ifndef PROTOBUF_CLHandshakePack_2eproto__INCLUDED
#define PROTOBUF_CLHandshakePack_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace Models {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_CLHandshakePack_2eproto();
void protobuf_AssignDesc_CLHandshakePack_2eproto();
void protobuf_ShutdownFile_CLHandshakePack_2eproto();

class CLHandshakePack;
class CLHandshakeAckPack;

// ===================================================================

class CLHandshakePack : public ::google::protobuf::MessageLite {
 public:
  CLHandshakePack();
  virtual ~CLHandshakePack();
  
  CLHandshakePack(const CLHandshakePack& from);
  
  inline CLHandshakePack& operator=(const CLHandshakePack& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const CLHandshakePack& default_instance();
  
  void Swap(CLHandshakePack* other);
  
  // implements Message ----------------------------------------------
  
  CLHandshakePack* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const CLHandshakePack& from);
  void MergeFrom(const CLHandshakePack& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional string ver = 1;
  inline bool has_ver() const;
  inline void clear_ver();
  static const int kVerFieldNumber = 1;
  inline const ::std::string& ver() const;
  inline void set_ver(const ::std::string& value);
  inline void set_ver(const char* value);
  inline void set_ver(const char* value, size_t size);
  inline ::std::string* mutable_ver();
  inline ::std::string* release_ver();
  
  // optional string timestamp = 2;
  inline bool has_timestamp() const;
  inline void clear_timestamp();
  static const int kTimestampFieldNumber = 2;
  inline const ::std::string& timestamp() const;
  inline void set_timestamp(const ::std::string& value);
  inline void set_timestamp(const char* value);
  inline void set_timestamp(const char* value, size_t size);
  inline ::std::string* mutable_timestamp();
  inline ::std::string* release_timestamp();
  
  // @@protoc_insertion_point(class_scope:Models.CLHandshakePack)
 private:
  inline void set_has_ver();
  inline void clear_has_ver();
  inline void set_has_timestamp();
  inline void clear_has_timestamp();
  
  ::std::string* ver_;
  ::std::string* timestamp_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_CLHandshakePack_2eproto();
  friend void protobuf_AssignDesc_CLHandshakePack_2eproto();
  friend void protobuf_ShutdownFile_CLHandshakePack_2eproto();
  
  void InitAsDefaultInstance();
  static CLHandshakePack* default_instance_;
};
// -------------------------------------------------------------------

class CLHandshakeAckPack : public ::google::protobuf::MessageLite {
 public:
  CLHandshakeAckPack();
  virtual ~CLHandshakeAckPack();
  
  CLHandshakeAckPack(const CLHandshakeAckPack& from);
  
  inline CLHandshakeAckPack& operator=(const CLHandshakeAckPack& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const CLHandshakeAckPack& default_instance();
  
  void Swap(CLHandshakeAckPack* other);
  
  // implements Message ----------------------------------------------
  
  CLHandshakeAckPack* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const CLHandshakeAckPack& from);
  void MergeFrom(const CLHandshakeAckPack& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional int32 client_id = 1;
  inline bool has_client_id() const;
  inline void clear_client_id();
  static const int kClientIdFieldNumber = 1;
  inline ::google::protobuf::int32 client_id() const;
  inline void set_client_id(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:Models.CLHandshakeAckPack)
 private:
  inline void set_has_client_id();
  inline void clear_has_client_id();
  
  ::google::protobuf::int32 client_id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_CLHandshakePack_2eproto();
  friend void protobuf_AssignDesc_CLHandshakePack_2eproto();
  friend void protobuf_ShutdownFile_CLHandshakePack_2eproto();
  
  void InitAsDefaultInstance();
  static CLHandshakeAckPack* default_instance_;
};
// ===================================================================


// ===================================================================

// CLHandshakePack

// optional string ver = 1;
inline bool CLHandshakePack::has_ver() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CLHandshakePack::set_has_ver() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CLHandshakePack::clear_has_ver() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CLHandshakePack::clear_ver() {
  if (ver_ != &::google::protobuf::internal::kEmptyString) {
    ver_->clear();
  }
  clear_has_ver();
}
inline const ::std::string& CLHandshakePack::ver() const {
  return *ver_;
}
inline void CLHandshakePack::set_ver(const ::std::string& value) {
  set_has_ver();
  if (ver_ == &::google::protobuf::internal::kEmptyString) {
    ver_ = new ::std::string;
  }
  ver_->assign(value);
}
inline void CLHandshakePack::set_ver(const char* value) {
  set_has_ver();
  if (ver_ == &::google::protobuf::internal::kEmptyString) {
    ver_ = new ::std::string;
  }
  ver_->assign(value);
}
inline void CLHandshakePack::set_ver(const char* value, size_t size) {
  set_has_ver();
  if (ver_ == &::google::protobuf::internal::kEmptyString) {
    ver_ = new ::std::string;
  }
  ver_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* CLHandshakePack::mutable_ver() {
  set_has_ver();
  if (ver_ == &::google::protobuf::internal::kEmptyString) {
    ver_ = new ::std::string;
  }
  return ver_;
}
inline ::std::string* CLHandshakePack::release_ver() {
  clear_has_ver();
  if (ver_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ver_;
    ver_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string timestamp = 2;
inline bool CLHandshakePack::has_timestamp() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CLHandshakePack::set_has_timestamp() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CLHandshakePack::clear_has_timestamp() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CLHandshakePack::clear_timestamp() {
  if (timestamp_ != &::google::protobuf::internal::kEmptyString) {
    timestamp_->clear();
  }
  clear_has_timestamp();
}
inline const ::std::string& CLHandshakePack::timestamp() const {
  return *timestamp_;
}
inline void CLHandshakePack::set_timestamp(const ::std::string& value) {
  set_has_timestamp();
  if (timestamp_ == &::google::protobuf::internal::kEmptyString) {
    timestamp_ = new ::std::string;
  }
  timestamp_->assign(value);
}
inline void CLHandshakePack::set_timestamp(const char* value) {
  set_has_timestamp();
  if (timestamp_ == &::google::protobuf::internal::kEmptyString) {
    timestamp_ = new ::std::string;
  }
  timestamp_->assign(value);
}
inline void CLHandshakePack::set_timestamp(const char* value, size_t size) {
  set_has_timestamp();
  if (timestamp_ == &::google::protobuf::internal::kEmptyString) {
    timestamp_ = new ::std::string;
  }
  timestamp_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* CLHandshakePack::mutable_timestamp() {
  set_has_timestamp();
  if (timestamp_ == &::google::protobuf::internal::kEmptyString) {
    timestamp_ = new ::std::string;
  }
  return timestamp_;
}
inline ::std::string* CLHandshakePack::release_timestamp() {
  clear_has_timestamp();
  if (timestamp_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = timestamp_;
    timestamp_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// CLHandshakeAckPack

// optional int32 client_id = 1;
inline bool CLHandshakeAckPack::has_client_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CLHandshakeAckPack::set_has_client_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CLHandshakeAckPack::clear_has_client_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CLHandshakeAckPack::clear_client_id() {
  client_id_ = 0;
  clear_has_client_id();
}
inline ::google::protobuf::int32 CLHandshakeAckPack::client_id() const {
  return client_id_;
}
inline void CLHandshakeAckPack::set_client_id(::google::protobuf::int32 value) {
  set_has_client_id();
  client_id_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Models

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_CLHandshakePack_2eproto__INCLUDED

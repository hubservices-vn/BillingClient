// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ScreenLockPack.proto

#ifndef PROTOBUF_ScreenLockPack_2eproto__INCLUDED
#define PROTOBUF_ScreenLockPack_2eproto__INCLUDED

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
void  protobuf_AddDesc_ScreenLockPack_2eproto();
void protobuf_AssignDesc_ScreenLockPack_2eproto();
void protobuf_ShutdownFile_ScreenLockPack_2eproto();

class ScreenLockRequestPack;
class ScreenLockReplyPack;
class ScreenUnlockRequestPack;
class ScreenUnlockReplyPack;

// ===================================================================

class ScreenLockRequestPack : public ::google::protobuf::MessageLite {
 public:
  ScreenLockRequestPack();
  virtual ~ScreenLockRequestPack();
  
  ScreenLockRequestPack(const ScreenLockRequestPack& from);
  
  inline ScreenLockRequestPack& operator=(const ScreenLockRequestPack& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const ScreenLockRequestPack& default_instance();
  
  void Swap(ScreenLockRequestPack* other);
  
  // implements Message ----------------------------------------------
  
  ScreenLockRequestPack* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const ScreenLockRequestPack& from);
  void MergeFrom(const ScreenLockRequestPack& from);
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
  
  // optional int32 mode = 1;
  inline bool has_mode() const;
  inline void clear_mode();
  static const int kModeFieldNumber = 1;
  inline ::google::protobuf::int32 mode() const;
  inline void set_mode(::google::protobuf::int32 value);
  
  // optional string pwd = 2;
  inline bool has_pwd() const;
  inline void clear_pwd();
  static const int kPwdFieldNumber = 2;
  inline const ::std::string& pwd() const;
  inline void set_pwd(const ::std::string& value);
  inline void set_pwd(const char* value);
  inline void set_pwd(const char* value, size_t size);
  inline ::std::string* mutable_pwd();
  inline ::std::string* release_pwd();
  
  // optional string acc_name = 3;
  inline bool has_acc_name() const;
  inline void clear_acc_name();
  static const int kAccNameFieldNumber = 3;
  inline const ::std::string& acc_name() const;
  inline void set_acc_name(const ::std::string& value);
  inline void set_acc_name(const char* value);
  inline void set_acc_name(const char* value, size_t size);
  inline ::std::string* mutable_acc_name();
  inline ::std::string* release_acc_name();
  
  // optional string acc_pwd_raw = 4;
  inline bool has_acc_pwd_raw() const;
  inline void clear_acc_pwd_raw();
  static const int kAccPwdRawFieldNumber = 4;
  inline const ::std::string& acc_pwd_raw() const;
  inline void set_acc_pwd_raw(const ::std::string& value);
  inline void set_acc_pwd_raw(const char* value);
  inline void set_acc_pwd_raw(const char* value, size_t size);
  inline ::std::string* mutable_acc_pwd_raw();
  inline ::std::string* release_acc_pwd_raw();
  
  // @@protoc_insertion_point(class_scope:Models.ScreenLockRequestPack)
 private:
  inline void set_has_mode();
  inline void clear_has_mode();
  inline void set_has_pwd();
  inline void clear_has_pwd();
  inline void set_has_acc_name();
  inline void clear_has_acc_name();
  inline void set_has_acc_pwd_raw();
  inline void clear_has_acc_pwd_raw();
  
  ::std::string* pwd_;
  ::std::string* acc_name_;
  ::std::string* acc_pwd_raw_;
  ::google::protobuf::int32 mode_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_ScreenLockPack_2eproto();
  friend void protobuf_AssignDesc_ScreenLockPack_2eproto();
  friend void protobuf_ShutdownFile_ScreenLockPack_2eproto();
  
  void InitAsDefaultInstance();
  static ScreenLockRequestPack* default_instance_;
};
// -------------------------------------------------------------------

class ScreenLockReplyPack : public ::google::protobuf::MessageLite {
 public:
  ScreenLockReplyPack();
  virtual ~ScreenLockReplyPack();
  
  ScreenLockReplyPack(const ScreenLockReplyPack& from);
  
  inline ScreenLockReplyPack& operator=(const ScreenLockReplyPack& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const ScreenLockReplyPack& default_instance();
  
  void Swap(ScreenLockReplyPack* other);
  
  // implements Message ----------------------------------------------
  
  ScreenLockReplyPack* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const ScreenLockReplyPack& from);
  void MergeFrom(const ScreenLockReplyPack& from);
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
  
  // optional int32 mode = 1;
  inline bool has_mode() const;
  inline void clear_mode();
  static const int kModeFieldNumber = 1;
  inline ::google::protobuf::int32 mode() const;
  inline void set_mode(::google::protobuf::int32 value);
  
  // optional bool result = 2;
  inline bool has_result() const;
  inline void clear_result();
  static const int kResultFieldNumber = 2;
  inline bool result() const;
  inline void set_result(bool value);
  
  // @@protoc_insertion_point(class_scope:Models.ScreenLockReplyPack)
 private:
  inline void set_has_mode();
  inline void clear_has_mode();
  inline void set_has_result();
  inline void clear_has_result();
  
  ::google::protobuf::int32 mode_;
  bool result_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_ScreenLockPack_2eproto();
  friend void protobuf_AssignDesc_ScreenLockPack_2eproto();
  friend void protobuf_ShutdownFile_ScreenLockPack_2eproto();
  
  void InitAsDefaultInstance();
  static ScreenLockReplyPack* default_instance_;
};
// -------------------------------------------------------------------

class ScreenUnlockRequestPack : public ::google::protobuf::MessageLite {
 public:
  ScreenUnlockRequestPack();
  virtual ~ScreenUnlockRequestPack();
  
  ScreenUnlockRequestPack(const ScreenUnlockRequestPack& from);
  
  inline ScreenUnlockRequestPack& operator=(const ScreenUnlockRequestPack& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const ScreenUnlockRequestPack& default_instance();
  
  void Swap(ScreenUnlockRequestPack* other);
  
  // implements Message ----------------------------------------------
  
  ScreenUnlockRequestPack* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const ScreenUnlockRequestPack& from);
  void MergeFrom(const ScreenUnlockRequestPack& from);
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
  
  // optional int32 mode = 1;
  inline bool has_mode() const;
  inline void clear_mode();
  static const int kModeFieldNumber = 1;
  inline ::google::protobuf::int32 mode() const;
  inline void set_mode(::google::protobuf::int32 value);
  
  // optional string pwd = 2;
  inline bool has_pwd() const;
  inline void clear_pwd();
  static const int kPwdFieldNumber = 2;
  inline const ::std::string& pwd() const;
  inline void set_pwd(const ::std::string& value);
  inline void set_pwd(const char* value);
  inline void set_pwd(const char* value, size_t size);
  inline ::std::string* mutable_pwd();
  inline ::std::string* release_pwd();
  
  // optional string acc_name = 3;
  inline bool has_acc_name() const;
  inline void clear_acc_name();
  static const int kAccNameFieldNumber = 3;
  inline const ::std::string& acc_name() const;
  inline void set_acc_name(const ::std::string& value);
  inline void set_acc_name(const char* value);
  inline void set_acc_name(const char* value, size_t size);
  inline ::std::string* mutable_acc_name();
  inline ::std::string* release_acc_name();
  
  // optional string acc_pwd_raw = 4;
  inline bool has_acc_pwd_raw() const;
  inline void clear_acc_pwd_raw();
  static const int kAccPwdRawFieldNumber = 4;
  inline const ::std::string& acc_pwd_raw() const;
  inline void set_acc_pwd_raw(const ::std::string& value);
  inline void set_acc_pwd_raw(const char* value);
  inline void set_acc_pwd_raw(const char* value, size_t size);
  inline ::std::string* mutable_acc_pwd_raw();
  inline ::std::string* release_acc_pwd_raw();
  
  // @@protoc_insertion_point(class_scope:Models.ScreenUnlockRequestPack)
 private:
  inline void set_has_mode();
  inline void clear_has_mode();
  inline void set_has_pwd();
  inline void clear_has_pwd();
  inline void set_has_acc_name();
  inline void clear_has_acc_name();
  inline void set_has_acc_pwd_raw();
  inline void clear_has_acc_pwd_raw();
  
  ::std::string* pwd_;
  ::std::string* acc_name_;
  ::std::string* acc_pwd_raw_;
  ::google::protobuf::int32 mode_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_ScreenLockPack_2eproto();
  friend void protobuf_AssignDesc_ScreenLockPack_2eproto();
  friend void protobuf_ShutdownFile_ScreenLockPack_2eproto();
  
  void InitAsDefaultInstance();
  static ScreenUnlockRequestPack* default_instance_;
};
// -------------------------------------------------------------------

class ScreenUnlockReplyPack : public ::google::protobuf::MessageLite {
 public:
  ScreenUnlockReplyPack();
  virtual ~ScreenUnlockReplyPack();
  
  ScreenUnlockReplyPack(const ScreenUnlockReplyPack& from);
  
  inline ScreenUnlockReplyPack& operator=(const ScreenUnlockReplyPack& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const ScreenUnlockReplyPack& default_instance();
  
  void Swap(ScreenUnlockReplyPack* other);
  
  // implements Message ----------------------------------------------
  
  ScreenUnlockReplyPack* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const ScreenUnlockReplyPack& from);
  void MergeFrom(const ScreenUnlockReplyPack& from);
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
  
  // optional int32 mode = 1;
  inline bool has_mode() const;
  inline void clear_mode();
  static const int kModeFieldNumber = 1;
  inline ::google::protobuf::int32 mode() const;
  inline void set_mode(::google::protobuf::int32 value);
  
  // optional bool result = 2;
  inline bool has_result() const;
  inline void clear_result();
  static const int kResultFieldNumber = 2;
  inline bool result() const;
  inline void set_result(bool value);
  
  // @@protoc_insertion_point(class_scope:Models.ScreenUnlockReplyPack)
 private:
  inline void set_has_mode();
  inline void clear_has_mode();
  inline void set_has_result();
  inline void clear_has_result();
  
  ::google::protobuf::int32 mode_;
  bool result_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_ScreenLockPack_2eproto();
  friend void protobuf_AssignDesc_ScreenLockPack_2eproto();
  friend void protobuf_ShutdownFile_ScreenLockPack_2eproto();
  
  void InitAsDefaultInstance();
  static ScreenUnlockReplyPack* default_instance_;
};
// ===================================================================


// ===================================================================

// ScreenLockRequestPack

// optional int32 mode = 1;
inline bool ScreenLockRequestPack::has_mode() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ScreenLockRequestPack::set_has_mode() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ScreenLockRequestPack::clear_has_mode() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ScreenLockRequestPack::clear_mode() {
  mode_ = 0;
  clear_has_mode();
}
inline ::google::protobuf::int32 ScreenLockRequestPack::mode() const {
  return mode_;
}
inline void ScreenLockRequestPack::set_mode(::google::protobuf::int32 value) {
  set_has_mode();
  mode_ = value;
}

// optional string pwd = 2;
inline bool ScreenLockRequestPack::has_pwd() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ScreenLockRequestPack::set_has_pwd() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ScreenLockRequestPack::clear_has_pwd() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ScreenLockRequestPack::clear_pwd() {
  if (pwd_ != &::google::protobuf::internal::kEmptyString) {
    pwd_->clear();
  }
  clear_has_pwd();
}
inline const ::std::string& ScreenLockRequestPack::pwd() const {
  return *pwd_;
}
inline void ScreenLockRequestPack::set_pwd(const ::std::string& value) {
  set_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    pwd_ = new ::std::string;
  }
  pwd_->assign(value);
}
inline void ScreenLockRequestPack::set_pwd(const char* value) {
  set_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    pwd_ = new ::std::string;
  }
  pwd_->assign(value);
}
inline void ScreenLockRequestPack::set_pwd(const char* value, size_t size) {
  set_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    pwd_ = new ::std::string;
  }
  pwd_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScreenLockRequestPack::mutable_pwd() {
  set_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    pwd_ = new ::std::string;
  }
  return pwd_;
}
inline ::std::string* ScreenLockRequestPack::release_pwd() {
  clear_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = pwd_;
    pwd_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string acc_name = 3;
inline bool ScreenLockRequestPack::has_acc_name() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ScreenLockRequestPack::set_has_acc_name() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ScreenLockRequestPack::clear_has_acc_name() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ScreenLockRequestPack::clear_acc_name() {
  if (acc_name_ != &::google::protobuf::internal::kEmptyString) {
    acc_name_->clear();
  }
  clear_has_acc_name();
}
inline const ::std::string& ScreenLockRequestPack::acc_name() const {
  return *acc_name_;
}
inline void ScreenLockRequestPack::set_acc_name(const ::std::string& value) {
  set_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    acc_name_ = new ::std::string;
  }
  acc_name_->assign(value);
}
inline void ScreenLockRequestPack::set_acc_name(const char* value) {
  set_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    acc_name_ = new ::std::string;
  }
  acc_name_->assign(value);
}
inline void ScreenLockRequestPack::set_acc_name(const char* value, size_t size) {
  set_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    acc_name_ = new ::std::string;
  }
  acc_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScreenLockRequestPack::mutable_acc_name() {
  set_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    acc_name_ = new ::std::string;
  }
  return acc_name_;
}
inline ::std::string* ScreenLockRequestPack::release_acc_name() {
  clear_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = acc_name_;
    acc_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string acc_pwd_raw = 4;
inline bool ScreenLockRequestPack::has_acc_pwd_raw() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ScreenLockRequestPack::set_has_acc_pwd_raw() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ScreenLockRequestPack::clear_has_acc_pwd_raw() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ScreenLockRequestPack::clear_acc_pwd_raw() {
  if (acc_pwd_raw_ != &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_->clear();
  }
  clear_has_acc_pwd_raw();
}
inline const ::std::string& ScreenLockRequestPack::acc_pwd_raw() const {
  return *acc_pwd_raw_;
}
inline void ScreenLockRequestPack::set_acc_pwd_raw(const ::std::string& value) {
  set_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_ = new ::std::string;
  }
  acc_pwd_raw_->assign(value);
}
inline void ScreenLockRequestPack::set_acc_pwd_raw(const char* value) {
  set_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_ = new ::std::string;
  }
  acc_pwd_raw_->assign(value);
}
inline void ScreenLockRequestPack::set_acc_pwd_raw(const char* value, size_t size) {
  set_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_ = new ::std::string;
  }
  acc_pwd_raw_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScreenLockRequestPack::mutable_acc_pwd_raw() {
  set_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_ = new ::std::string;
  }
  return acc_pwd_raw_;
}
inline ::std::string* ScreenLockRequestPack::release_acc_pwd_raw() {
  clear_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = acc_pwd_raw_;
    acc_pwd_raw_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// ScreenLockReplyPack

// optional int32 mode = 1;
inline bool ScreenLockReplyPack::has_mode() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ScreenLockReplyPack::set_has_mode() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ScreenLockReplyPack::clear_has_mode() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ScreenLockReplyPack::clear_mode() {
  mode_ = 0;
  clear_has_mode();
}
inline ::google::protobuf::int32 ScreenLockReplyPack::mode() const {
  return mode_;
}
inline void ScreenLockReplyPack::set_mode(::google::protobuf::int32 value) {
  set_has_mode();
  mode_ = value;
}

// optional bool result = 2;
inline bool ScreenLockReplyPack::has_result() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ScreenLockReplyPack::set_has_result() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ScreenLockReplyPack::clear_has_result() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ScreenLockReplyPack::clear_result() {
  result_ = false;
  clear_has_result();
}
inline bool ScreenLockReplyPack::result() const {
  return result_;
}
inline void ScreenLockReplyPack::set_result(bool value) {
  set_has_result();
  result_ = value;
}

// -------------------------------------------------------------------

// ScreenUnlockRequestPack

// optional int32 mode = 1;
inline bool ScreenUnlockRequestPack::has_mode() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ScreenUnlockRequestPack::set_has_mode() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ScreenUnlockRequestPack::clear_has_mode() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ScreenUnlockRequestPack::clear_mode() {
  mode_ = 0;
  clear_has_mode();
}
inline ::google::protobuf::int32 ScreenUnlockRequestPack::mode() const {
  return mode_;
}
inline void ScreenUnlockRequestPack::set_mode(::google::protobuf::int32 value) {
  set_has_mode();
  mode_ = value;
}

// optional string pwd = 2;
inline bool ScreenUnlockRequestPack::has_pwd() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ScreenUnlockRequestPack::set_has_pwd() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ScreenUnlockRequestPack::clear_has_pwd() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ScreenUnlockRequestPack::clear_pwd() {
  if (pwd_ != &::google::protobuf::internal::kEmptyString) {
    pwd_->clear();
  }
  clear_has_pwd();
}
inline const ::std::string& ScreenUnlockRequestPack::pwd() const {
  return *pwd_;
}
inline void ScreenUnlockRequestPack::set_pwd(const ::std::string& value) {
  set_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    pwd_ = new ::std::string;
  }
  pwd_->assign(value);
}
inline void ScreenUnlockRequestPack::set_pwd(const char* value) {
  set_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    pwd_ = new ::std::string;
  }
  pwd_->assign(value);
}
inline void ScreenUnlockRequestPack::set_pwd(const char* value, size_t size) {
  set_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    pwd_ = new ::std::string;
  }
  pwd_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScreenUnlockRequestPack::mutable_pwd() {
  set_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    pwd_ = new ::std::string;
  }
  return pwd_;
}
inline ::std::string* ScreenUnlockRequestPack::release_pwd() {
  clear_has_pwd();
  if (pwd_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = pwd_;
    pwd_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string acc_name = 3;
inline bool ScreenUnlockRequestPack::has_acc_name() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ScreenUnlockRequestPack::set_has_acc_name() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ScreenUnlockRequestPack::clear_has_acc_name() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ScreenUnlockRequestPack::clear_acc_name() {
  if (acc_name_ != &::google::protobuf::internal::kEmptyString) {
    acc_name_->clear();
  }
  clear_has_acc_name();
}
inline const ::std::string& ScreenUnlockRequestPack::acc_name() const {
  return *acc_name_;
}
inline void ScreenUnlockRequestPack::set_acc_name(const ::std::string& value) {
  set_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    acc_name_ = new ::std::string;
  }
  acc_name_->assign(value);
}
inline void ScreenUnlockRequestPack::set_acc_name(const char* value) {
  set_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    acc_name_ = new ::std::string;
  }
  acc_name_->assign(value);
}
inline void ScreenUnlockRequestPack::set_acc_name(const char* value, size_t size) {
  set_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    acc_name_ = new ::std::string;
  }
  acc_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScreenUnlockRequestPack::mutable_acc_name() {
  set_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    acc_name_ = new ::std::string;
  }
  return acc_name_;
}
inline ::std::string* ScreenUnlockRequestPack::release_acc_name() {
  clear_has_acc_name();
  if (acc_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = acc_name_;
    acc_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string acc_pwd_raw = 4;
inline bool ScreenUnlockRequestPack::has_acc_pwd_raw() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ScreenUnlockRequestPack::set_has_acc_pwd_raw() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ScreenUnlockRequestPack::clear_has_acc_pwd_raw() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ScreenUnlockRequestPack::clear_acc_pwd_raw() {
  if (acc_pwd_raw_ != &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_->clear();
  }
  clear_has_acc_pwd_raw();
}
inline const ::std::string& ScreenUnlockRequestPack::acc_pwd_raw() const {
  return *acc_pwd_raw_;
}
inline void ScreenUnlockRequestPack::set_acc_pwd_raw(const ::std::string& value) {
  set_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_ = new ::std::string;
  }
  acc_pwd_raw_->assign(value);
}
inline void ScreenUnlockRequestPack::set_acc_pwd_raw(const char* value) {
  set_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_ = new ::std::string;
  }
  acc_pwd_raw_->assign(value);
}
inline void ScreenUnlockRequestPack::set_acc_pwd_raw(const char* value, size_t size) {
  set_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_ = new ::std::string;
  }
  acc_pwd_raw_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ScreenUnlockRequestPack::mutable_acc_pwd_raw() {
  set_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    acc_pwd_raw_ = new ::std::string;
  }
  return acc_pwd_raw_;
}
inline ::std::string* ScreenUnlockRequestPack::release_acc_pwd_raw() {
  clear_has_acc_pwd_raw();
  if (acc_pwd_raw_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = acc_pwd_raw_;
    acc_pwd_raw_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// ScreenUnlockReplyPack

// optional int32 mode = 1;
inline bool ScreenUnlockReplyPack::has_mode() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ScreenUnlockReplyPack::set_has_mode() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ScreenUnlockReplyPack::clear_has_mode() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ScreenUnlockReplyPack::clear_mode() {
  mode_ = 0;
  clear_has_mode();
}
inline ::google::protobuf::int32 ScreenUnlockReplyPack::mode() const {
  return mode_;
}
inline void ScreenUnlockReplyPack::set_mode(::google::protobuf::int32 value) {
  set_has_mode();
  mode_ = value;
}

// optional bool result = 2;
inline bool ScreenUnlockReplyPack::has_result() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ScreenUnlockReplyPack::set_has_result() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ScreenUnlockReplyPack::clear_has_result() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ScreenUnlockReplyPack::clear_result() {
  result_ = false;
  clear_has_result();
}
inline bool ScreenUnlockReplyPack::result() const {
  return result_;
}
inline void ScreenUnlockReplyPack::set_result(bool value) {
  set_has_result();
  result_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Models

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ScreenLockPack_2eproto__INCLUDED

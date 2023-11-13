// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.26.0
// 	protoc        v3.19.0
// source: cloud/filestore/public/api/protos/locks.proto

package protos

import (
	protos "github.com/ydb-platform/nbs/cloud/storage/core/protos"
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type ELockType int32

const (
	ELockType_E_SHARED    ELockType = 0
	ELockType_E_EXCLUSIVE ELockType = 1
	ELockType_E_UNLOCK    ELockType = 2
)

// Enum value maps for ELockType.
var (
	ELockType_name = map[int32]string{
		0: "E_SHARED",
		1: "E_EXCLUSIVE",
		2: "E_UNLOCK",
	}
	ELockType_value = map[string]int32{
		"E_SHARED":    0,
		"E_EXCLUSIVE": 1,
		"E_UNLOCK":    2,
	}
)

func (x ELockType) Enum() *ELockType {
	p := new(ELockType)
	*p = x
	return p
}

func (x ELockType) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (ELockType) Descriptor() protoreflect.EnumDescriptor {
	return file_cloud_filestore_public_api_protos_locks_proto_enumTypes[0].Descriptor()
}

func (ELockType) Type() protoreflect.EnumType {
	return &file_cloud_filestore_public_api_protos_locks_proto_enumTypes[0]
}

func (x ELockType) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use ELockType.Descriptor instead.
func (ELockType) EnumDescriptor() ([]byte, []int) {
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP(), []int{0}
}

// Lock origins
type ELockOrigin int32

const (
	ELockOrigin_E_FCNTL ELockOrigin = 0
	ELockOrigin_E_FLOCK ELockOrigin = 1
)

// Enum value maps for ELockOrigin.
var (
	ELockOrigin_name = map[int32]string{
		0: "E_FCNTL",
		1: "E_FLOCK",
	}
	ELockOrigin_value = map[string]int32{
		"E_FCNTL": 0,
		"E_FLOCK": 1,
	}
)

func (x ELockOrigin) Enum() *ELockOrigin {
	p := new(ELockOrigin)
	*p = x
	return p
}

func (x ELockOrigin) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (ELockOrigin) Descriptor() protoreflect.EnumDescriptor {
	return file_cloud_filestore_public_api_protos_locks_proto_enumTypes[1].Descriptor()
}

func (ELockOrigin) Type() protoreflect.EnumType {
	return &file_cloud_filestore_public_api_protos_locks_proto_enumTypes[1]
}

func (x ELockOrigin) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use ELockOrigin.Descriptor instead.
func (ELockOrigin) EnumDescriptor() ([]byte, []int) {
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP(), []int{1}
}

type TAcquireLockRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// Optional request headers.
	Headers *THeaders `protobuf:"bytes,1,opt,name=Headers,proto3" json:"Headers,omitempty"`
	// FileSystem identifier.
	FileSystemId string `protobuf:"bytes,2,opt,name=FileSystemId,proto3" json:"FileSystemId,omitempty"`
	// Node.
	NodeId uint64 `protobuf:"varint,3,opt,name=NodeId,proto3" json:"NodeId,omitempty"`
	// IO handle.
	Handle uint64 `protobuf:"varint,4,opt,name=Handle,proto3" json:"Handle,omitempty"`
	// Lock owner.
	Owner uint64 `protobuf:"varint,5,opt,name=Owner,proto3" json:"Owner,omitempty"`
	// Starting offset for lock.
	Offset uint64 `protobuf:"varint,6,opt,name=Offset,proto3" json:"Offset,omitempty"`
	// Number of bytes to lock.
	Length uint64 `protobuf:"varint,7,opt,name=Length,proto3" json:"Length,omitempty"`
	// Lock type.
	LockType ELockType `protobuf:"varint,8,opt,name=LockType,proto3,enum=NCloud.NFileStore.NProto.ELockType" json:"LockType,omitempty"`
	// Process holding lock.
	Pid int32 `protobuf:"varint,9,opt,name=Pid,proto3" json:"Pid,omitempty"`
	// Request origin.
	LockOrigin ELockOrigin `protobuf:"varint,10,opt,name=LockOrigin,proto3,enum=NCloud.NFileStore.NProto.ELockOrigin" json:"LockOrigin,omitempty"`
}

func (x *TAcquireLockRequest) Reset() {
	*x = TAcquireLockRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[0]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *TAcquireLockRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TAcquireLockRequest) ProtoMessage() {}

func (x *TAcquireLockRequest) ProtoReflect() protoreflect.Message {
	mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[0]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TAcquireLockRequest.ProtoReflect.Descriptor instead.
func (*TAcquireLockRequest) Descriptor() ([]byte, []int) {
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP(), []int{0}
}

func (x *TAcquireLockRequest) GetHeaders() *THeaders {
	if x != nil {
		return x.Headers
	}
	return nil
}

func (x *TAcquireLockRequest) GetFileSystemId() string {
	if x != nil {
		return x.FileSystemId
	}
	return ""
}

func (x *TAcquireLockRequest) GetNodeId() uint64 {
	if x != nil {
		return x.NodeId
	}
	return 0
}

func (x *TAcquireLockRequest) GetHandle() uint64 {
	if x != nil {
		return x.Handle
	}
	return 0
}

func (x *TAcquireLockRequest) GetOwner() uint64 {
	if x != nil {
		return x.Owner
	}
	return 0
}

func (x *TAcquireLockRequest) GetOffset() uint64 {
	if x != nil {
		return x.Offset
	}
	return 0
}

func (x *TAcquireLockRequest) GetLength() uint64 {
	if x != nil {
		return x.Length
	}
	return 0
}

func (x *TAcquireLockRequest) GetLockType() ELockType {
	if x != nil {
		return x.LockType
	}
	return ELockType_E_SHARED
}

func (x *TAcquireLockRequest) GetPid() int32 {
	if x != nil {
		return x.Pid
	}
	return 0
}

func (x *TAcquireLockRequest) GetLockOrigin() ELockOrigin {
	if x != nil {
		return x.LockOrigin
	}
	return ELockOrigin_E_FCNTL
}

type TAcquireLockResponse struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// Optional error, set only if error happened.
	Error *protos.TError `protobuf:"bytes,1,opt,name=Error,proto3" json:"Error,omitempty"`
	// Optional response headers.
	Headers *TResponseHeaders `protobuf:"bytes,1000,opt,name=Headers,proto3" json:"Headers,omitempty"`
}

func (x *TAcquireLockResponse) Reset() {
	*x = TAcquireLockResponse{}
	if protoimpl.UnsafeEnabled {
		mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[1]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *TAcquireLockResponse) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TAcquireLockResponse) ProtoMessage() {}

func (x *TAcquireLockResponse) ProtoReflect() protoreflect.Message {
	mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[1]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TAcquireLockResponse.ProtoReflect.Descriptor instead.
func (*TAcquireLockResponse) Descriptor() ([]byte, []int) {
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP(), []int{1}
}

func (x *TAcquireLockResponse) GetError() *protos.TError {
	if x != nil {
		return x.Error
	}
	return nil
}

func (x *TAcquireLockResponse) GetHeaders() *TResponseHeaders {
	if x != nil {
		return x.Headers
	}
	return nil
}

type TReleaseLockRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// Optional request headers.
	Headers *THeaders `protobuf:"bytes,1,opt,name=Headers,proto3" json:"Headers,omitempty"`
	// FileSystem identifier.
	FileSystemId string `protobuf:"bytes,2,opt,name=FileSystemId,proto3" json:"FileSystemId,omitempty"`
	// Node.
	NodeId uint64 `protobuf:"varint,3,opt,name=NodeId,proto3" json:"NodeId,omitempty"`
	// IO handle.
	Handle uint64 `protobuf:"varint,4,opt,name=Handle,proto3" json:"Handle,omitempty"`
	// Lock owner.
	Owner uint64 `protobuf:"varint,5,opt,name=Owner,proto3" json:"Owner,omitempty"`
	// Starting offset for lock.
	Offset uint64 `protobuf:"varint,6,opt,name=Offset,proto3" json:"Offset,omitempty"`
	// Number of bytes to lock.
	Length uint64 `protobuf:"varint,7,opt,name=Length,proto3" json:"Length,omitempty"`
	// Process holding lock.
	Pid int32 `protobuf:"varint,8,opt,name=Pid,proto3" json:"Pid,omitempty"`
	// Request origin.
	LockOrigin ELockOrigin `protobuf:"varint,9,opt,name=LockOrigin,proto3,enum=NCloud.NFileStore.NProto.ELockOrigin" json:"LockOrigin,omitempty"`
}

func (x *TReleaseLockRequest) Reset() {
	*x = TReleaseLockRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[2]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *TReleaseLockRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TReleaseLockRequest) ProtoMessage() {}

func (x *TReleaseLockRequest) ProtoReflect() protoreflect.Message {
	mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[2]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TReleaseLockRequest.ProtoReflect.Descriptor instead.
func (*TReleaseLockRequest) Descriptor() ([]byte, []int) {
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP(), []int{2}
}

func (x *TReleaseLockRequest) GetHeaders() *THeaders {
	if x != nil {
		return x.Headers
	}
	return nil
}

func (x *TReleaseLockRequest) GetFileSystemId() string {
	if x != nil {
		return x.FileSystemId
	}
	return ""
}

func (x *TReleaseLockRequest) GetNodeId() uint64 {
	if x != nil {
		return x.NodeId
	}
	return 0
}

func (x *TReleaseLockRequest) GetHandle() uint64 {
	if x != nil {
		return x.Handle
	}
	return 0
}

func (x *TReleaseLockRequest) GetOwner() uint64 {
	if x != nil {
		return x.Owner
	}
	return 0
}

func (x *TReleaseLockRequest) GetOffset() uint64 {
	if x != nil {
		return x.Offset
	}
	return 0
}

func (x *TReleaseLockRequest) GetLength() uint64 {
	if x != nil {
		return x.Length
	}
	return 0
}

func (x *TReleaseLockRequest) GetPid() int32 {
	if x != nil {
		return x.Pid
	}
	return 0
}

func (x *TReleaseLockRequest) GetLockOrigin() ELockOrigin {
	if x != nil {
		return x.LockOrigin
	}
	return ELockOrigin_E_FCNTL
}

type TReleaseLockResponse struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// Optional error, set only if error happened.
	Error *protos.TError `protobuf:"bytes,1,opt,name=Error,proto3" json:"Error,omitempty"`
	// Optional response headers.
	Headers *TResponseHeaders `protobuf:"bytes,1000,opt,name=Headers,proto3" json:"Headers,omitempty"`
}

func (x *TReleaseLockResponse) Reset() {
	*x = TReleaseLockResponse{}
	if protoimpl.UnsafeEnabled {
		mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[3]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *TReleaseLockResponse) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TReleaseLockResponse) ProtoMessage() {}

func (x *TReleaseLockResponse) ProtoReflect() protoreflect.Message {
	mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[3]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TReleaseLockResponse.ProtoReflect.Descriptor instead.
func (*TReleaseLockResponse) Descriptor() ([]byte, []int) {
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP(), []int{3}
}

func (x *TReleaseLockResponse) GetError() *protos.TError {
	if x != nil {
		return x.Error
	}
	return nil
}

func (x *TReleaseLockResponse) GetHeaders() *TResponseHeaders {
	if x != nil {
		return x.Headers
	}
	return nil
}

type TTestLockRequest struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// Optional request headers.
	Headers *THeaders `protobuf:"bytes,1,opt,name=Headers,proto3" json:"Headers,omitempty"`
	// FileSystem identifier.
	FileSystemId string `protobuf:"bytes,2,opt,name=FileSystemId,proto3" json:"FileSystemId,omitempty"`
	// Node.
	NodeId uint64 `protobuf:"varint,3,opt,name=NodeId,proto3" json:"NodeId,omitempty"`
	// IO handle.
	Handle uint64 `protobuf:"varint,4,opt,name=Handle,proto3" json:"Handle,omitempty"`
	// Lock owner.
	Owner uint64 `protobuf:"varint,5,opt,name=Owner,proto3" json:"Owner,omitempty"`
	// Starting offset for lock.
	Offset uint64 `protobuf:"varint,6,opt,name=Offset,proto3" json:"Offset,omitempty"`
	// Number of bytes to lock.
	Length uint64 `protobuf:"varint,7,opt,name=Length,proto3" json:"Length,omitempty"`
	// Lock type.
	LockType ELockType `protobuf:"varint,8,opt,name=LockType,proto3,enum=NCloud.NFileStore.NProto.ELockType" json:"LockType,omitempty"`
	// Process holding lock.
	Pid int32 `protobuf:"varint,9,opt,name=Pid,proto3" json:"Pid,omitempty"`
	// Request origin.
	LockOrigin ELockOrigin `protobuf:"varint,10,opt,name=LockOrigin,proto3,enum=NCloud.NFileStore.NProto.ELockOrigin" json:"LockOrigin,omitempty"`
}

func (x *TTestLockRequest) Reset() {
	*x = TTestLockRequest{}
	if protoimpl.UnsafeEnabled {
		mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[4]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *TTestLockRequest) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TTestLockRequest) ProtoMessage() {}

func (x *TTestLockRequest) ProtoReflect() protoreflect.Message {
	mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[4]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TTestLockRequest.ProtoReflect.Descriptor instead.
func (*TTestLockRequest) Descriptor() ([]byte, []int) {
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP(), []int{4}
}

func (x *TTestLockRequest) GetHeaders() *THeaders {
	if x != nil {
		return x.Headers
	}
	return nil
}

func (x *TTestLockRequest) GetFileSystemId() string {
	if x != nil {
		return x.FileSystemId
	}
	return ""
}

func (x *TTestLockRequest) GetNodeId() uint64 {
	if x != nil {
		return x.NodeId
	}
	return 0
}

func (x *TTestLockRequest) GetHandle() uint64 {
	if x != nil {
		return x.Handle
	}
	return 0
}

func (x *TTestLockRequest) GetOwner() uint64 {
	if x != nil {
		return x.Owner
	}
	return 0
}

func (x *TTestLockRequest) GetOffset() uint64 {
	if x != nil {
		return x.Offset
	}
	return 0
}

func (x *TTestLockRequest) GetLength() uint64 {
	if x != nil {
		return x.Length
	}
	return 0
}

func (x *TTestLockRequest) GetLockType() ELockType {
	if x != nil {
		return x.LockType
	}
	return ELockType_E_SHARED
}

func (x *TTestLockRequest) GetPid() int32 {
	if x != nil {
		return x.Pid
	}
	return 0
}

func (x *TTestLockRequest) GetLockOrigin() ELockOrigin {
	if x != nil {
		return x.LockOrigin
	}
	return ELockOrigin_E_FCNTL
}

type TTestLockResponse struct {
	state         protoimpl.MessageState
	sizeCache     protoimpl.SizeCache
	unknownFields protoimpl.UnknownFields

	// Optional error, set only if error happened.
	Error *protos.TError `protobuf:"bytes,1,opt,name=Error,proto3" json:"Error,omitempty"`
	// Lock owner of the conflicting lock if any.
	Owner uint64 `protobuf:"varint,2,opt,name=Owner,proto3" json:"Owner,omitempty"`
	// Starting offset of the conflicting lock if any.
	Offset uint64 `protobuf:"varint,3,opt,name=Offset,proto3" json:"Offset,omitempty"`
	// Number of bytes of the conflicting lock if any.
	Length uint64 `protobuf:"varint,4,opt,name=Length,proto3" json:"Length,omitempty"`
	// Lock type of the conflicting lock if any.
	LockType *ELockType `protobuf:"varint,5,opt,name=LockType,proto3,enum=NCloud.NFileStore.NProto.ELockType,oneof" json:"LockType,omitempty"`
	// Process of the conflicting lock if any.
	Pid *int32 `protobuf:"varint,6,opt,name=Pid,proto3,oneof" json:"Pid,omitempty"`
	// Incompatible lock origin that is already been held.
	IncompatibleLockOrigin *ELockOrigin `protobuf:"varint,7,opt,name=IncompatibleLockOrigin,proto3,enum=NCloud.NFileStore.NProto.ELockOrigin,oneof" json:"IncompatibleLockOrigin,omitempty"`
	// Optional response headers.
	Headers *TResponseHeaders `protobuf:"bytes,1000,opt,name=Headers,proto3" json:"Headers,omitempty"`
}

func (x *TTestLockResponse) Reset() {
	*x = TTestLockResponse{}
	if protoimpl.UnsafeEnabled {
		mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[5]
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		ms.StoreMessageInfo(mi)
	}
}

func (x *TTestLockResponse) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*TTestLockResponse) ProtoMessage() {}

func (x *TTestLockResponse) ProtoReflect() protoreflect.Message {
	mi := &file_cloud_filestore_public_api_protos_locks_proto_msgTypes[5]
	if protoimpl.UnsafeEnabled && x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use TTestLockResponse.ProtoReflect.Descriptor instead.
func (*TTestLockResponse) Descriptor() ([]byte, []int) {
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP(), []int{5}
}

func (x *TTestLockResponse) GetError() *protos.TError {
	if x != nil {
		return x.Error
	}
	return nil
}

func (x *TTestLockResponse) GetOwner() uint64 {
	if x != nil {
		return x.Owner
	}
	return 0
}

func (x *TTestLockResponse) GetOffset() uint64 {
	if x != nil {
		return x.Offset
	}
	return 0
}

func (x *TTestLockResponse) GetLength() uint64 {
	if x != nil {
		return x.Length
	}
	return 0
}

func (x *TTestLockResponse) GetLockType() ELockType {
	if x != nil && x.LockType != nil {
		return *x.LockType
	}
	return ELockType_E_SHARED
}

func (x *TTestLockResponse) GetPid() int32 {
	if x != nil && x.Pid != nil {
		return *x.Pid
	}
	return 0
}

func (x *TTestLockResponse) GetIncompatibleLockOrigin() ELockOrigin {
	if x != nil && x.IncompatibleLockOrigin != nil {
		return *x.IncompatibleLockOrigin
	}
	return ELockOrigin_E_FCNTL
}

func (x *TTestLockResponse) GetHeaders() *TResponseHeaders {
	if x != nil {
		return x.Headers
	}
	return nil
}

var File_cloud_filestore_public_api_protos_locks_proto protoreflect.FileDescriptor

var file_cloud_filestore_public_api_protos_locks_proto_rawDesc = []byte{
	0x0a, 0x2d, 0x63, 0x6c, 0x6f, 0x75, 0x64, 0x2f, 0x66, 0x69, 0x6c, 0x65, 0x73, 0x74, 0x6f, 0x72,
	0x65, 0x2f, 0x70, 0x75, 0x62, 0x6c, 0x69, 0x63, 0x2f, 0x61, 0x70, 0x69, 0x2f, 0x70, 0x72, 0x6f,
	0x74, 0x6f, 0x73, 0x2f, 0x6c, 0x6f, 0x63, 0x6b, 0x73, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12,
	0x18, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x74, 0x6f,
	0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x1a, 0x2f, 0x63, 0x6c, 0x6f, 0x75, 0x64,
	0x2f, 0x66, 0x69, 0x6c, 0x65, 0x73, 0x74, 0x6f, 0x72, 0x65, 0x2f, 0x70, 0x75, 0x62, 0x6c, 0x69,
	0x63, 0x2f, 0x61, 0x70, 0x69, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2f, 0x68, 0x65, 0x61,
	0x64, 0x65, 0x72, 0x73, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x1a, 0x25, 0x63, 0x6c, 0x6f, 0x75,
	0x64, 0x2f, 0x73, 0x74, 0x6f, 0x72, 0x61, 0x67, 0x65, 0x2f, 0x63, 0x6f, 0x72, 0x65, 0x2f, 0x70,
	0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2f, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x2e, 0x70, 0x72, 0x6f, 0x74,
	0x6f, 0x22, 0x87, 0x03, 0x0a, 0x13, 0x54, 0x41, 0x63, 0x71, 0x75, 0x69, 0x72, 0x65, 0x4c, 0x6f,
	0x63, 0x6b, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x12, 0x3c, 0x0a, 0x07, 0x48, 0x65, 0x61,
	0x64, 0x65, 0x72, 0x73, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x22, 0x2e, 0x4e, 0x43, 0x6c,
	0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e,
	0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x54, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x52, 0x07,
	0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x12, 0x22, 0x0a, 0x0c, 0x46, 0x69, 0x6c, 0x65, 0x53,
	0x79, 0x73, 0x74, 0x65, 0x6d, 0x49, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0c, 0x46,
	0x69, 0x6c, 0x65, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x49, 0x64, 0x12, 0x16, 0x0a, 0x06, 0x4e,
	0x6f, 0x64, 0x65, 0x49, 0x64, 0x18, 0x03, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06, 0x4e, 0x6f, 0x64,
	0x65, 0x49, 0x64, 0x12, 0x16, 0x0a, 0x06, 0x48, 0x61, 0x6e, 0x64, 0x6c, 0x65, 0x18, 0x04, 0x20,
	0x01, 0x28, 0x04, 0x52, 0x06, 0x48, 0x61, 0x6e, 0x64, 0x6c, 0x65, 0x12, 0x14, 0x0a, 0x05, 0x4f,
	0x77, 0x6e, 0x65, 0x72, 0x18, 0x05, 0x20, 0x01, 0x28, 0x04, 0x52, 0x05, 0x4f, 0x77, 0x6e, 0x65,
	0x72, 0x12, 0x16, 0x0a, 0x06, 0x4f, 0x66, 0x66, 0x73, 0x65, 0x74, 0x18, 0x06, 0x20, 0x01, 0x28,
	0x04, 0x52, 0x06, 0x4f, 0x66, 0x66, 0x73, 0x65, 0x74, 0x12, 0x16, 0x0a, 0x06, 0x4c, 0x65, 0x6e,
	0x67, 0x74, 0x68, 0x18, 0x07, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06, 0x4c, 0x65, 0x6e, 0x67, 0x74,
	0x68, 0x12, 0x3f, 0x0a, 0x08, 0x4c, 0x6f, 0x63, 0x6b, 0x54, 0x79, 0x70, 0x65, 0x18, 0x08, 0x20,
	0x01, 0x28, 0x0e, 0x32, 0x23, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69,
	0x6c, 0x65, 0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x45,
	0x4c, 0x6f, 0x63, 0x6b, 0x54, 0x79, 0x70, 0x65, 0x52, 0x08, 0x4c, 0x6f, 0x63, 0x6b, 0x54, 0x79,
	0x70, 0x65, 0x12, 0x10, 0x0a, 0x03, 0x50, 0x69, 0x64, 0x18, 0x09, 0x20, 0x01, 0x28, 0x05, 0x52,
	0x03, 0x50, 0x69, 0x64, 0x12, 0x45, 0x0a, 0x0a, 0x4c, 0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67,
	0x69, 0x6e, 0x18, 0x0a, 0x20, 0x01, 0x28, 0x0e, 0x32, 0x25, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75,
	0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72,
	0x6f, 0x74, 0x6f, 0x2e, 0x45, 0x4c, 0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x52,
	0x0a, 0x4c, 0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x22, 0x8a, 0x01, 0x0a, 0x14,
	0x54, 0x41, 0x63, 0x71, 0x75, 0x69, 0x72, 0x65, 0x4c, 0x6f, 0x63, 0x6b, 0x52, 0x65, 0x73, 0x70,
	0x6f, 0x6e, 0x73, 0x65, 0x12, 0x2b, 0x0a, 0x05, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x18, 0x01, 0x20,
	0x01, 0x28, 0x0b, 0x32, 0x15, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x50, 0x72,
	0x6f, 0x74, 0x6f, 0x2e, 0x54, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x52, 0x05, 0x45, 0x72, 0x72, 0x6f,
	0x72, 0x12, 0x45, 0x0a, 0x07, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x18, 0xe8, 0x07, 0x20,
	0x01, 0x28, 0x0b, 0x32, 0x2a, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69,
	0x6c, 0x65, 0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x54,
	0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x52,
	0x07, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x22, 0xc6, 0x02, 0x0a, 0x13, 0x54, 0x52, 0x65,
	0x6c, 0x65, 0x61, 0x73, 0x65, 0x4c, 0x6f, 0x63, 0x6b, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74,
	0x12, 0x3c, 0x0a, 0x07, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x18, 0x01, 0x20, 0x01, 0x28,
	0x0b, 0x32, 0x22, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65,
	0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x54, 0x48, 0x65,
	0x61, 0x64, 0x65, 0x72, 0x73, 0x52, 0x07, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x12, 0x22,
	0x0a, 0x0c, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x49, 0x64, 0x18, 0x02,
	0x20, 0x01, 0x28, 0x09, 0x52, 0x0c, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d,
	0x49, 0x64, 0x12, 0x16, 0x0a, 0x06, 0x4e, 0x6f, 0x64, 0x65, 0x49, 0x64, 0x18, 0x03, 0x20, 0x01,
	0x28, 0x04, 0x52, 0x06, 0x4e, 0x6f, 0x64, 0x65, 0x49, 0x64, 0x12, 0x16, 0x0a, 0x06, 0x48, 0x61,
	0x6e, 0x64, 0x6c, 0x65, 0x18, 0x04, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06, 0x48, 0x61, 0x6e, 0x64,
	0x6c, 0x65, 0x12, 0x14, 0x0a, 0x05, 0x4f, 0x77, 0x6e, 0x65, 0x72, 0x18, 0x05, 0x20, 0x01, 0x28,
	0x04, 0x52, 0x05, 0x4f, 0x77, 0x6e, 0x65, 0x72, 0x12, 0x16, 0x0a, 0x06, 0x4f, 0x66, 0x66, 0x73,
	0x65, 0x74, 0x18, 0x06, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06, 0x4f, 0x66, 0x66, 0x73, 0x65, 0x74,
	0x12, 0x16, 0x0a, 0x06, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x18, 0x07, 0x20, 0x01, 0x28, 0x04,
	0x52, 0x06, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x12, 0x10, 0x0a, 0x03, 0x50, 0x69, 0x64, 0x18,
	0x08, 0x20, 0x01, 0x28, 0x05, 0x52, 0x03, 0x50, 0x69, 0x64, 0x12, 0x45, 0x0a, 0x0a, 0x4c, 0x6f,
	0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x18, 0x09, 0x20, 0x01, 0x28, 0x0e, 0x32, 0x25,
	0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x74, 0x6f,
	0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x45, 0x4c, 0x6f, 0x63, 0x6b, 0x4f,
	0x72, 0x69, 0x67, 0x69, 0x6e, 0x52, 0x0a, 0x4c, 0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67, 0x69,
	0x6e, 0x22, 0x8a, 0x01, 0x0a, 0x14, 0x54, 0x52, 0x65, 0x6c, 0x65, 0x61, 0x73, 0x65, 0x4c, 0x6f,
	0x63, 0x6b, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x12, 0x2b, 0x0a, 0x05, 0x45, 0x72,
	0x72, 0x6f, 0x72, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x15, 0x2e, 0x4e, 0x43, 0x6c, 0x6f,
	0x75, 0x64, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x54, 0x45, 0x72, 0x72, 0x6f, 0x72,
	0x52, 0x05, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x12, 0x45, 0x0a, 0x07, 0x48, 0x65, 0x61, 0x64, 0x65,
	0x72, 0x73, 0x18, 0xe8, 0x07, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x2a, 0x2e, 0x4e, 0x43, 0x6c, 0x6f,
	0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50,
	0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x54, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x48, 0x65,
	0x61, 0x64, 0x65, 0x72, 0x73, 0x52, 0x07, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x22, 0x84,
	0x03, 0x0a, 0x10, 0x54, 0x54, 0x65, 0x73, 0x74, 0x4c, 0x6f, 0x63, 0x6b, 0x52, 0x65, 0x71, 0x75,
	0x65, 0x73, 0x74, 0x12, 0x3c, 0x0a, 0x07, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x18, 0x01,
	0x20, 0x01, 0x28, 0x0b, 0x32, 0x22, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46,
	0x69, 0x6c, 0x65, 0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e,
	0x54, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x52, 0x07, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72,
	0x73, 0x12, 0x22, 0x0a, 0x0c, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x49,
	0x64, 0x18, 0x02, 0x20, 0x01, 0x28, 0x09, 0x52, 0x0c, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x79, 0x73,
	0x74, 0x65, 0x6d, 0x49, 0x64, 0x12, 0x16, 0x0a, 0x06, 0x4e, 0x6f, 0x64, 0x65, 0x49, 0x64, 0x18,
	0x03, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06, 0x4e, 0x6f, 0x64, 0x65, 0x49, 0x64, 0x12, 0x16, 0x0a,
	0x06, 0x48, 0x61, 0x6e, 0x64, 0x6c, 0x65, 0x18, 0x04, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06, 0x48,
	0x61, 0x6e, 0x64, 0x6c, 0x65, 0x12, 0x14, 0x0a, 0x05, 0x4f, 0x77, 0x6e, 0x65, 0x72, 0x18, 0x05,
	0x20, 0x01, 0x28, 0x04, 0x52, 0x05, 0x4f, 0x77, 0x6e, 0x65, 0x72, 0x12, 0x16, 0x0a, 0x06, 0x4f,
	0x66, 0x66, 0x73, 0x65, 0x74, 0x18, 0x06, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06, 0x4f, 0x66, 0x66,
	0x73, 0x65, 0x74, 0x12, 0x16, 0x0a, 0x06, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x18, 0x07, 0x20,
	0x01, 0x28, 0x04, 0x52, 0x06, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x12, 0x3f, 0x0a, 0x08, 0x4c,
	0x6f, 0x63, 0x6b, 0x54, 0x79, 0x70, 0x65, 0x18, 0x08, 0x20, 0x01, 0x28, 0x0e, 0x32, 0x23, 0x2e,
	0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x74, 0x6f, 0x72,
	0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x45, 0x4c, 0x6f, 0x63, 0x6b, 0x54, 0x79,
	0x70, 0x65, 0x52, 0x08, 0x4c, 0x6f, 0x63, 0x6b, 0x54, 0x79, 0x70, 0x65, 0x12, 0x10, 0x0a, 0x03,
	0x50, 0x69, 0x64, 0x18, 0x09, 0x20, 0x01, 0x28, 0x05, 0x52, 0x03, 0x50, 0x69, 0x64, 0x12, 0x45,
	0x0a, 0x0a, 0x4c, 0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x18, 0x0a, 0x20, 0x01,
	0x28, 0x0e, 0x32, 0x25, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c,
	0x65, 0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x45, 0x4c,
	0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x52, 0x0a, 0x4c, 0x6f, 0x63, 0x6b, 0x4f,
	0x72, 0x69, 0x67, 0x69, 0x6e, 0x22, 0xbe, 0x03, 0x0a, 0x11, 0x54, 0x54, 0x65, 0x73, 0x74, 0x4c,
	0x6f, 0x63, 0x6b, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x12, 0x2b, 0x0a, 0x05, 0x45,
	0x72, 0x72, 0x6f, 0x72, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x15, 0x2e, 0x4e, 0x43, 0x6c,
	0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x54, 0x45, 0x72, 0x72, 0x6f,
	0x72, 0x52, 0x05, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x12, 0x14, 0x0a, 0x05, 0x4f, 0x77, 0x6e, 0x65,
	0x72, 0x18, 0x02, 0x20, 0x01, 0x28, 0x04, 0x52, 0x05, 0x4f, 0x77, 0x6e, 0x65, 0x72, 0x12, 0x16,
	0x0a, 0x06, 0x4f, 0x66, 0x66, 0x73, 0x65, 0x74, 0x18, 0x03, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06,
	0x4f, 0x66, 0x66, 0x73, 0x65, 0x74, 0x12, 0x16, 0x0a, 0x06, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68,
	0x18, 0x04, 0x20, 0x01, 0x28, 0x04, 0x52, 0x06, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x12, 0x44,
	0x0a, 0x08, 0x4c, 0x6f, 0x63, 0x6b, 0x54, 0x79, 0x70, 0x65, 0x18, 0x05, 0x20, 0x01, 0x28, 0x0e,
	0x32, 0x23, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65, 0x53,
	0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x45, 0x4c, 0x6f, 0x63,
	0x6b, 0x54, 0x79, 0x70, 0x65, 0x48, 0x00, 0x52, 0x08, 0x4c, 0x6f, 0x63, 0x6b, 0x54, 0x79, 0x70,
	0x65, 0x88, 0x01, 0x01, 0x12, 0x15, 0x0a, 0x03, 0x50, 0x69, 0x64, 0x18, 0x06, 0x20, 0x01, 0x28,
	0x05, 0x48, 0x01, 0x52, 0x03, 0x50, 0x69, 0x64, 0x88, 0x01, 0x01, 0x12, 0x62, 0x0a, 0x16, 0x49,
	0x6e, 0x63, 0x6f, 0x6d, 0x70, 0x61, 0x74, 0x69, 0x62, 0x6c, 0x65, 0x4c, 0x6f, 0x63, 0x6b, 0x4f,
	0x72, 0x69, 0x67, 0x69, 0x6e, 0x18, 0x07, 0x20, 0x01, 0x28, 0x0e, 0x32, 0x25, 0x2e, 0x4e, 0x43,
	0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e,
	0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x45, 0x4c, 0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67,
	0x69, 0x6e, 0x48, 0x02, 0x52, 0x16, 0x49, 0x6e, 0x63, 0x6f, 0x6d, 0x70, 0x61, 0x74, 0x69, 0x62,
	0x6c, 0x65, 0x4c, 0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x88, 0x01, 0x01, 0x12,
	0x45, 0x0a, 0x07, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x18, 0xe8, 0x07, 0x20, 0x01, 0x28,
	0x0b, 0x32, 0x2a, 0x2e, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x46, 0x69, 0x6c, 0x65,
	0x53, 0x74, 0x6f, 0x72, 0x65, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2e, 0x54, 0x52, 0x65,
	0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x52, 0x07, 0x48,
	0x65, 0x61, 0x64, 0x65, 0x72, 0x73, 0x42, 0x0b, 0x0a, 0x09, 0x5f, 0x4c, 0x6f, 0x63, 0x6b, 0x54,
	0x79, 0x70, 0x65, 0x42, 0x06, 0x0a, 0x04, 0x5f, 0x50, 0x69, 0x64, 0x42, 0x19, 0x0a, 0x17, 0x5f,
	0x49, 0x6e, 0x63, 0x6f, 0x6d, 0x70, 0x61, 0x74, 0x69, 0x62, 0x6c, 0x65, 0x4c, 0x6f, 0x63, 0x6b,
	0x4f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x2a, 0x38, 0x0a, 0x09, 0x45, 0x4c, 0x6f, 0x63, 0x6b, 0x54,
	0x79, 0x70, 0x65, 0x12, 0x0c, 0x0a, 0x08, 0x45, 0x5f, 0x53, 0x48, 0x41, 0x52, 0x45, 0x44, 0x10,
	0x00, 0x12, 0x0f, 0x0a, 0x0b, 0x45, 0x5f, 0x45, 0x58, 0x43, 0x4c, 0x55, 0x53, 0x49, 0x56, 0x45,
	0x10, 0x01, 0x12, 0x0c, 0x0a, 0x08, 0x45, 0x5f, 0x55, 0x4e, 0x4c, 0x4f, 0x43, 0x4b, 0x10, 0x02,
	0x2a, 0x27, 0x0a, 0x0b, 0x45, 0x4c, 0x6f, 0x63, 0x6b, 0x4f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x12,
	0x0b, 0x0a, 0x07, 0x45, 0x5f, 0x46, 0x43, 0x4e, 0x54, 0x4c, 0x10, 0x00, 0x12, 0x0b, 0x0a, 0x07,
	0x45, 0x5f, 0x46, 0x4c, 0x4f, 0x43, 0x4b, 0x10, 0x01, 0x42, 0x34, 0x5a, 0x32, 0x61, 0x2e, 0x79,
	0x61, 0x6e, 0x64, 0x65, 0x78, 0x2d, 0x74, 0x65, 0x61, 0x6d, 0x2e, 0x72, 0x75, 0x2f, 0x63, 0x6c,
	0x6f, 0x75, 0x64, 0x2f, 0x66, 0x69, 0x6c, 0x65, 0x73, 0x74, 0x6f, 0x72, 0x65, 0x2f, 0x70, 0x75,
	0x62, 0x6c, 0x69, 0x63, 0x2f, 0x61, 0x70, 0x69, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x62,
	0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_cloud_filestore_public_api_protos_locks_proto_rawDescOnce sync.Once
	file_cloud_filestore_public_api_protos_locks_proto_rawDescData = file_cloud_filestore_public_api_protos_locks_proto_rawDesc
)

func file_cloud_filestore_public_api_protos_locks_proto_rawDescGZIP() []byte {
	file_cloud_filestore_public_api_protos_locks_proto_rawDescOnce.Do(func() {
		file_cloud_filestore_public_api_protos_locks_proto_rawDescData = protoimpl.X.CompressGZIP(file_cloud_filestore_public_api_protos_locks_proto_rawDescData)
	})
	return file_cloud_filestore_public_api_protos_locks_proto_rawDescData
}

var file_cloud_filestore_public_api_protos_locks_proto_enumTypes = make([]protoimpl.EnumInfo, 2)
var file_cloud_filestore_public_api_protos_locks_proto_msgTypes = make([]protoimpl.MessageInfo, 6)
var file_cloud_filestore_public_api_protos_locks_proto_goTypes = []interface{}{
	(ELockType)(0),               // 0: NCloud.NFileStore.NProto.ELockType
	(ELockOrigin)(0),             // 1: NCloud.NFileStore.NProto.ELockOrigin
	(*TAcquireLockRequest)(nil),  // 2: NCloud.NFileStore.NProto.TAcquireLockRequest
	(*TAcquireLockResponse)(nil), // 3: NCloud.NFileStore.NProto.TAcquireLockResponse
	(*TReleaseLockRequest)(nil),  // 4: NCloud.NFileStore.NProto.TReleaseLockRequest
	(*TReleaseLockResponse)(nil), // 5: NCloud.NFileStore.NProto.TReleaseLockResponse
	(*TTestLockRequest)(nil),     // 6: NCloud.NFileStore.NProto.TTestLockRequest
	(*TTestLockResponse)(nil),    // 7: NCloud.NFileStore.NProto.TTestLockResponse
	(*THeaders)(nil),             // 8: NCloud.NFileStore.NProto.THeaders
	(*protos.TError)(nil),        // 9: NCloud.NProto.TError
	(*TResponseHeaders)(nil),     // 10: NCloud.NFileStore.NProto.TResponseHeaders
}
var file_cloud_filestore_public_api_protos_locks_proto_depIdxs = []int32{
	8,  // 0: NCloud.NFileStore.NProto.TAcquireLockRequest.Headers:type_name -> NCloud.NFileStore.NProto.THeaders
	0,  // 1: NCloud.NFileStore.NProto.TAcquireLockRequest.LockType:type_name -> NCloud.NFileStore.NProto.ELockType
	1,  // 2: NCloud.NFileStore.NProto.TAcquireLockRequest.LockOrigin:type_name -> NCloud.NFileStore.NProto.ELockOrigin
	9,  // 3: NCloud.NFileStore.NProto.TAcquireLockResponse.Error:type_name -> NCloud.NProto.TError
	10, // 4: NCloud.NFileStore.NProto.TAcquireLockResponse.Headers:type_name -> NCloud.NFileStore.NProto.TResponseHeaders
	8,  // 5: NCloud.NFileStore.NProto.TReleaseLockRequest.Headers:type_name -> NCloud.NFileStore.NProto.THeaders
	1,  // 6: NCloud.NFileStore.NProto.TReleaseLockRequest.LockOrigin:type_name -> NCloud.NFileStore.NProto.ELockOrigin
	9,  // 7: NCloud.NFileStore.NProto.TReleaseLockResponse.Error:type_name -> NCloud.NProto.TError
	10, // 8: NCloud.NFileStore.NProto.TReleaseLockResponse.Headers:type_name -> NCloud.NFileStore.NProto.TResponseHeaders
	8,  // 9: NCloud.NFileStore.NProto.TTestLockRequest.Headers:type_name -> NCloud.NFileStore.NProto.THeaders
	0,  // 10: NCloud.NFileStore.NProto.TTestLockRequest.LockType:type_name -> NCloud.NFileStore.NProto.ELockType
	1,  // 11: NCloud.NFileStore.NProto.TTestLockRequest.LockOrigin:type_name -> NCloud.NFileStore.NProto.ELockOrigin
	9,  // 12: NCloud.NFileStore.NProto.TTestLockResponse.Error:type_name -> NCloud.NProto.TError
	0,  // 13: NCloud.NFileStore.NProto.TTestLockResponse.LockType:type_name -> NCloud.NFileStore.NProto.ELockType
	1,  // 14: NCloud.NFileStore.NProto.TTestLockResponse.IncompatibleLockOrigin:type_name -> NCloud.NFileStore.NProto.ELockOrigin
	10, // 15: NCloud.NFileStore.NProto.TTestLockResponse.Headers:type_name -> NCloud.NFileStore.NProto.TResponseHeaders
	16, // [16:16] is the sub-list for method output_type
	16, // [16:16] is the sub-list for method input_type
	16, // [16:16] is the sub-list for extension type_name
	16, // [16:16] is the sub-list for extension extendee
	0,  // [0:16] is the sub-list for field type_name
}

func init() { file_cloud_filestore_public_api_protos_locks_proto_init() }
func file_cloud_filestore_public_api_protos_locks_proto_init() {
	if File_cloud_filestore_public_api_protos_locks_proto != nil {
		return
	}
	file_cloud_filestore_public_api_protos_headers_proto_init()
	if !protoimpl.UnsafeEnabled {
		file_cloud_filestore_public_api_protos_locks_proto_msgTypes[0].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*TAcquireLockRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_cloud_filestore_public_api_protos_locks_proto_msgTypes[1].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*TAcquireLockResponse); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_cloud_filestore_public_api_protos_locks_proto_msgTypes[2].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*TReleaseLockRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_cloud_filestore_public_api_protos_locks_proto_msgTypes[3].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*TReleaseLockResponse); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_cloud_filestore_public_api_protos_locks_proto_msgTypes[4].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*TTestLockRequest); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
		file_cloud_filestore_public_api_protos_locks_proto_msgTypes[5].Exporter = func(v interface{}, i int) interface{} {
			switch v := v.(*TTestLockResponse); i {
			case 0:
				return &v.state
			case 1:
				return &v.sizeCache
			case 2:
				return &v.unknownFields
			default:
				return nil
			}
		}
	}
	file_cloud_filestore_public_api_protos_locks_proto_msgTypes[5].OneofWrappers = []interface{}{}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_cloud_filestore_public_api_protos_locks_proto_rawDesc,
			NumEnums:      2,
			NumMessages:   6,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_cloud_filestore_public_api_protos_locks_proto_goTypes,
		DependencyIndexes: file_cloud_filestore_public_api_protos_locks_proto_depIdxs,
		EnumInfos:         file_cloud_filestore_public_api_protos_locks_proto_enumTypes,
		MessageInfos:      file_cloud_filestore_public_api_protos_locks_proto_msgTypes,
	}.Build()
	File_cloud_filestore_public_api_protos_locks_proto = out.File
	file_cloud_filestore_public_api_protos_locks_proto_rawDesc = nil
	file_cloud_filestore_public_api_protos_locks_proto_goTypes = nil
	file_cloud_filestore_public_api_protos_locks_proto_depIdxs = nil
}

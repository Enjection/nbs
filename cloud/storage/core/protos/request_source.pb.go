// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.26.0
// 	protoc        v3.19.0
// source: cloud/storage/core/protos/request_source.proto

package protos

import (
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

type ERequestSource int32

const (
	ERequestSource_SOURCE_INSECURE_CONTROL_CHANNEL ERequestSource = 0
	ERequestSource_SOURCE_SECURE_CONTROL_CHANNEL   ERequestSource = 1
	ERequestSource_SOURCE_TCP_DATA_CHANNEL         ERequestSource = 2
	ERequestSource_SOURCE_FD_DATA_CHANNEL          ERequestSource = 3
	ERequestSource_SOURCE_FD_CONTROL_CHANNEL       ERequestSource = 4
)

// Enum value maps for ERequestSource.
var (
	ERequestSource_name = map[int32]string{
		0: "SOURCE_INSECURE_CONTROL_CHANNEL",
		1: "SOURCE_SECURE_CONTROL_CHANNEL",
		2: "SOURCE_TCP_DATA_CHANNEL",
		3: "SOURCE_FD_DATA_CHANNEL",
		4: "SOURCE_FD_CONTROL_CHANNEL",
	}
	ERequestSource_value = map[string]int32{
		"SOURCE_INSECURE_CONTROL_CHANNEL": 0,
		"SOURCE_SECURE_CONTROL_CHANNEL":   1,
		"SOURCE_TCP_DATA_CHANNEL":         2,
		"SOURCE_FD_DATA_CHANNEL":          3,
		"SOURCE_FD_CONTROL_CHANNEL":       4,
	}
)

func (x ERequestSource) Enum() *ERequestSource {
	p := new(ERequestSource)
	*p = x
	return p
}

func (x ERequestSource) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (ERequestSource) Descriptor() protoreflect.EnumDescriptor {
	return file_cloud_storage_core_protos_request_source_proto_enumTypes[0].Descriptor()
}

func (ERequestSource) Type() protoreflect.EnumType {
	return &file_cloud_storage_core_protos_request_source_proto_enumTypes[0]
}

func (x ERequestSource) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use ERequestSource.Descriptor instead.
func (ERequestSource) EnumDescriptor() ([]byte, []int) {
	return file_cloud_storage_core_protos_request_source_proto_rawDescGZIP(), []int{0}
}

var File_cloud_storage_core_protos_request_source_proto protoreflect.FileDescriptor

var file_cloud_storage_core_protos_request_source_proto_rawDesc = []byte{
	0x0a, 0x2e, 0x63, 0x6c, 0x6f, 0x75, 0x64, 0x2f, 0x73, 0x74, 0x6f, 0x72, 0x61, 0x67, 0x65, 0x2f,
	0x63, 0x6f, 0x72, 0x65, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73, 0x2f, 0x72, 0x65, 0x71, 0x75,
	0x65, 0x73, 0x74, 0x5f, 0x73, 0x6f, 0x75, 0x72, 0x63, 0x65, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x12, 0x0d, 0x4e, 0x43, 0x6c, 0x6f, 0x75, 0x64, 0x2e, 0x4e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2a,
	0xb0, 0x01, 0x0a, 0x0e, 0x45, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x53, 0x6f, 0x75, 0x72,
	0x63, 0x65, 0x12, 0x23, 0x0a, 0x1f, 0x53, 0x4f, 0x55, 0x52, 0x43, 0x45, 0x5f, 0x49, 0x4e, 0x53,
	0x45, 0x43, 0x55, 0x52, 0x45, 0x5f, 0x43, 0x4f, 0x4e, 0x54, 0x52, 0x4f, 0x4c, 0x5f, 0x43, 0x48,
	0x41, 0x4e, 0x4e, 0x45, 0x4c, 0x10, 0x00, 0x12, 0x21, 0x0a, 0x1d, 0x53, 0x4f, 0x55, 0x52, 0x43,
	0x45, 0x5f, 0x53, 0x45, 0x43, 0x55, 0x52, 0x45, 0x5f, 0x43, 0x4f, 0x4e, 0x54, 0x52, 0x4f, 0x4c,
	0x5f, 0x43, 0x48, 0x41, 0x4e, 0x4e, 0x45, 0x4c, 0x10, 0x01, 0x12, 0x1b, 0x0a, 0x17, 0x53, 0x4f,
	0x55, 0x52, 0x43, 0x45, 0x5f, 0x54, 0x43, 0x50, 0x5f, 0x44, 0x41, 0x54, 0x41, 0x5f, 0x43, 0x48,
	0x41, 0x4e, 0x4e, 0x45, 0x4c, 0x10, 0x02, 0x12, 0x1a, 0x0a, 0x16, 0x53, 0x4f, 0x55, 0x52, 0x43,
	0x45, 0x5f, 0x46, 0x44, 0x5f, 0x44, 0x41, 0x54, 0x41, 0x5f, 0x43, 0x48, 0x41, 0x4e, 0x4e, 0x45,
	0x4c, 0x10, 0x03, 0x12, 0x1d, 0x0a, 0x19, 0x53, 0x4f, 0x55, 0x52, 0x43, 0x45, 0x5f, 0x46, 0x44,
	0x5f, 0x43, 0x4f, 0x4e, 0x54, 0x52, 0x4f, 0x4c, 0x5f, 0x43, 0x48, 0x41, 0x4e, 0x4e, 0x45, 0x4c,
	0x10, 0x04, 0x42, 0x2c, 0x5a, 0x2a, 0x61, 0x2e, 0x79, 0x61, 0x6e, 0x64, 0x65, 0x78, 0x2d, 0x74,
	0x65, 0x61, 0x6d, 0x2e, 0x72, 0x75, 0x2f, 0x63, 0x6c, 0x6f, 0x75, 0x64, 0x2f, 0x73, 0x74, 0x6f,
	0x72, 0x61, 0x67, 0x65, 0x2f, 0x63, 0x6f, 0x72, 0x65, 0x2f, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x73,
	0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_cloud_storage_core_protos_request_source_proto_rawDescOnce sync.Once
	file_cloud_storage_core_protos_request_source_proto_rawDescData = file_cloud_storage_core_protos_request_source_proto_rawDesc
)

func file_cloud_storage_core_protos_request_source_proto_rawDescGZIP() []byte {
	file_cloud_storage_core_protos_request_source_proto_rawDescOnce.Do(func() {
		file_cloud_storage_core_protos_request_source_proto_rawDescData = protoimpl.X.CompressGZIP(file_cloud_storage_core_protos_request_source_proto_rawDescData)
	})
	return file_cloud_storage_core_protos_request_source_proto_rawDescData
}

var file_cloud_storage_core_protos_request_source_proto_enumTypes = make([]protoimpl.EnumInfo, 1)
var file_cloud_storage_core_protos_request_source_proto_goTypes = []interface{}{
	(ERequestSource)(0), // 0: NCloud.NProto.ERequestSource
}
var file_cloud_storage_core_protos_request_source_proto_depIdxs = []int32{
	0, // [0:0] is the sub-list for method output_type
	0, // [0:0] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_cloud_storage_core_protos_request_source_proto_init() }
func file_cloud_storage_core_protos_request_source_proto_init() {
	if File_cloud_storage_core_protos_request_source_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_cloud_storage_core_protos_request_source_proto_rawDesc,
			NumEnums:      1,
			NumMessages:   0,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_cloud_storage_core_protos_request_source_proto_goTypes,
		DependencyIndexes: file_cloud_storage_core_protos_request_source_proto_depIdxs,
		EnumInfos:         file_cloud_storage_core_protos_request_source_proto_enumTypes,
	}.Build()
	File_cloud_storage_core_protos_request_source_proto = out.File
	file_cloud_storage_core_protos_request_source_proto_rawDesc = nil
	file_cloud_storage_core_protos_request_source_proto_goTypes = nil
	file_cloud_storage_core_protos_request_source_proto_depIdxs = nil
}